


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include "mpeg2.h"
#include "mdtime.h"
static int buffer_size = 4096;
static FILE * in_file;
static int demux_track = 0;
static int demux_pid = 0;
static int demux_pva = 0;
static mpeg2dec_t * mpeg2dec;
static int sigint = 0;
static int total_offset = 0;
static int verbose = 0;


void dump_state (FILE * f, mpeg2_state_t state, const mpeg2_info_t * info,
		 int offset, int verbose);



static void * malloc_hook (unsigned size, mpeg2_alloc_t reason)
{
    void * buf;

    /*
     * Invalid streams can refer to fbufs that have not been
     * initialized yet. For example the stream could start with a
     * picture type onther than I. Or it could have a B picture before
     * it gets two reference frames. Or, some slices could be missing.
     *
     * Consequently, the output depends on the content 2 output
     * buffers have when the sequence begins. In release builds, this
     * does not matter (garbage in, garbage out), but in test code, we
     * always zero all our output buffers to:
     * - make our test produce deterministic outputs
     * - hint checkergcc that it is fine to read from all our output
     *   buffers at any time
     */
    if ((int)reason < 0) {
        return NULL;
    }
    buf = mpeg2_malloc (size, (mpeg2_alloc_t)-1);
    if (buf && (reason == MPEG2_ALLOC_YUV || reason == MPEG2_ALLOC_CONVERTED))
        memset (buf, 0, size);
    return buf;
}
static void decode_mpeg2 (uint8_t * current, uint8_t * end)
{
    const mpeg2_info_t * info;
    mpeg2_state_t state;
    //by hdzhang vo_setup_result_t setup_result;

    mpeg2_buffer (mpeg2dec, current, end);
    total_offset += end - current;

    info = mpeg2_info (mpeg2dec);
    while (1) {//解码该缓冲内的所有数据
		state = mpeg2_parse (mpeg2dec);
		if (verbose)
			dump_state (stderr, state, info,
				total_offset - mpeg2_getpos (mpeg2dec), verbose);
		switch (state) {
			case STATE_BUFFER:
				return;
			case STATE_SEQUENCE:
				mpeg2_skip (mpeg2dec, 0);//by hdzhang(output->draw == NULL));
				break;
			case STATE_PICTURE:
				/* might skip */
				break;
			case STATE_SLICE:
			case STATE_END:
			case STATE_INVALID_END:
				/* draw current picture */
				break;
			default:
				break;
		}//end switch
    }//end while
}

#define DEMUX_PAYLOAD_START 1
static int demux (uint8_t * buf, uint8_t * end, int flags)
{
    static int mpeg1_skip_table[16] = {
	0, 0, 4, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    /*
     * the demuxer keeps some state between calls:
     * if "state" = DEMUX_HEADER, then "head_buf" contains the first
     *     "bytes" bytes from some header.
     * if "state" == DEMUX_DATA, then we need to copy "bytes" bytes
     *     of ES data before the next header.
     * if "state" == DEMUX_SKIP, then we need to skip "bytes" bytes
     *     of data before the next header.
     *
     * NEEDBYTES makes sure we have the requested number of bytes for a
     * header. If we dont, it copies what we have into head_buf and returns,
     * so that when we come back with more data we finish decoding this header.
     *
     * DONEBYTES updates "buf" to point after the header we just parsed.
     */

#define DEMUX_HEADER 0
#define DEMUX_DATA 1
#define DEMUX_SKIP 2
    static int state = DEMUX_SKIP;
    static int state_bytes = 0;
    static uint8_t head_buf[264];

    uint8_t * header;
    int bytes;
    int len;

#define NEEDBYTES(x)						\
    do {							\
	int missing;						\
								\
	missing = (x) - bytes;					\
	if (missing > 0) {					\
	    if (header == head_buf) {				\
		if (missing <= end - buf) {			\
		    memcpy (header + bytes, buf, missing);	\
		    buf += missing;				\
		    bytes = (x);				\
		} else {					\
		    memcpy (header + bytes, buf, end - buf);	\
		    state_bytes = bytes + end - buf;		\
		    return 0;					\
		}						\
	    } else {						\
		memcpy (head_buf, header, bytes);		\
		state = DEMUX_HEADER;				\
		state_bytes = bytes;				\
		return 0;					\
	    }							\
	}							\
    } while (0)

#define DONEBYTES(x)		\
    do {			\
	if (header != head_buf)	\
	    buf = header + (x);	\
    } while (0)

    if (flags & DEMUX_PAYLOAD_START)
	goto payload_start;
    switch (state) {
    case DEMUX_HEADER:
	if (state_bytes > 0) {
	    header = head_buf;
	    bytes = state_bytes;
	    goto continue_header;
	}
	break;
    case DEMUX_DATA:
	if (demux_pid || (state_bytes > end - buf)) {
	    decode_mpeg2 (buf, end);
	    state_bytes -= end - buf;
	    return 0;
	}
	decode_mpeg2 (buf, buf + state_bytes);
	buf += state_bytes;
	break;
    case DEMUX_SKIP:
	if (demux_pid || (state_bytes > end - buf)) {
	    state_bytes -= end - buf;
	    return 0;
	}
	buf += state_bytes;
	break;
    }

    while (1) {
	if (demux_pid) {
	    state = DEMUX_SKIP;
	    return 0;
	}
    payload_start:
	header = buf;
	bytes = end - buf;
    continue_header:
	NEEDBYTES (4);
	if (header[0] || header[1] || (header[2] != 1)) {
	    if (demux_pid) {
		state = DEMUX_SKIP;
		return 0;
	    } else if (header != head_buf) {
		buf++;
		goto payload_start;
	    } else {
		header[0] = header[1];
		header[1] = header[2];
		header[2] = header[3];
		bytes = 3;
		goto continue_header;
	    }
	}
	if (demux_pid) {
	    if ((header[3] >= 0xe0) && (header[3] <= 0xef))
		goto pes;
	    fprintf (stderr, "bad stream id %x\n", header[3]);
	    exit (1);
	}
	switch (header[3]) {
	case 0xb9:	/* program end code */
	    /* DONEBYTES (4); */
	    /* break;         */
	    return 1;
	case 0xba:	/* pack header */
	    NEEDBYTES (5);
	    if ((header[4] & 0xc0) == 0x40) {	/* mpeg2 */
		NEEDBYTES (14);
		len = 14 + (header[13] & 7);
		NEEDBYTES (len);
		DONEBYTES (len);
		/* header points to the mpeg2 pack header */
	    } else if ((header[4] & 0xf0) == 0x20) {	/* mpeg1 */
		NEEDBYTES (12);
		DONEBYTES (12);
		/* header points to the mpeg1 pack header */
	    } else {
		fprintf (stderr, "weird pack header\n");
		DONEBYTES (5);
	    }
	    break;
	default:
	    if (header[3] == demux_track) {
	    pes:
		NEEDBYTES (7);
		if ((header[6] & 0xc0) == 0x80) {	/* mpeg2 */
		    NEEDBYTES (9);
		    len = 9 + header[8];
		    NEEDBYTES (len);
		    /* header points to the mpeg2 pes header */
		    if (header[7] & 0x80) {
			uint32_t pts, dts;

			pts = (((header[9] >> 1) << 30) |
			       (header[10] << 22) | ((header[11] >> 1) << 15) |
			       (header[12] << 7) | (header[13] >> 1));
			dts = (!(header[7] & 0x40) ? pts :
			       (((header[14] >> 1) << 30) |
				(header[15] << 22) |
				((header[16] >> 1) << 15) |
				(header[17] << 7) | (header[18] >> 1)));
			mpeg2_tag_picture (mpeg2dec, pts, dts);
		    }
		} else {	/* mpeg1 */
		    int len_skip;
		    uint8_t * ptsbuf;

		    len = 7;
		    while (header[len - 1] == 0xff) {
			len++;
			NEEDBYTES (len);
			if (len > 23) {
			    fprintf (stderr, "too much stuffing\n");
			    break;
			}
		    }
		    if ((header[len - 1] & 0xc0) == 0x40) {
			len += 2;
			NEEDBYTES (len);
		    }
		    len_skip = len;
		    len += mpeg1_skip_table[header[len - 1] >> 4];
		    NEEDBYTES (len);
		    /* header points to the mpeg1 pes header */
		    ptsbuf = header + len_skip;
		    if ((ptsbuf[-1] & 0xe0) == 0x20) {
			uint32_t pts, dts;

			pts = (((ptsbuf[-1] >> 1) << 30) |
			       (ptsbuf[0] << 22) | ((ptsbuf[1] >> 1) << 15) |
			       (ptsbuf[2] << 7) | (ptsbuf[3] >> 1));
			dts = (((ptsbuf[-1] & 0xf0) != 0x30) ? pts :
			       (((ptsbuf[4] >> 1) << 30) |
				(ptsbuf[5] << 22) | ((ptsbuf[6] >> 1) << 15) |
				(ptsbuf[7] << 7) | (ptsbuf[18] >> 1)));
			mpeg2_tag_picture (mpeg2dec, pts, dts);
		    }
		}
		DONEBYTES (len);
		bytes = 6 + (header[4] << 8) + header[5] - len;
		if (demux_pid || (bytes > end - buf)) {
		    decode_mpeg2 (buf, end);
		    state = DEMUX_DATA;
		    state_bytes = bytes - (end - buf);
		    return 0;
		} else if (bytes > 0) {
		    decode_mpeg2 (buf, buf + bytes);
		    buf += bytes;
		}
	    } else if (header[3] < 0xb9) {
		fprintf (stderr,
			 "looks like a video stream, not system stream\n");
		DONEBYTES (4);
	    } else {
		NEEDBYTES (6);
		DONEBYTES (6);
		bytes = (header[4] << 8) + header[5];
		if (bytes > end - buf) {
		    state = DEMUX_SKIP;
		    state_bytes = bytes - (end - buf);
		    return 0;
		}
		buf += bytes;
	    }
	}
    }
}

static void ps_loop (void)
{
    uint8_t * buffer = (uint8_t *) malloc (buffer_size);
    uint8_t * end;

    if (buffer == NULL)
	exit (1);
    do {
	end = buffer + fread (buffer, 1, buffer_size, in_file);
	if (demux (buffer, end, 0))
	    break;	/* hit program_end_code */
    } while (end == buffer + buffer_size && !sigint);
    free (buffer);
}

static int pva_demux (uint8_t * buf, uint8_t * end)
{
    static int state = DEMUX_SKIP;
    static int state_bytes = 0;
    static uint8_t head_buf[15];

    uint8_t * header;
    int bytes;
    int len;

    switch (state) {
    case DEMUX_HEADER:
        if (state_bytes > 0) {
            header = head_buf;
            bytes = state_bytes;
            goto continue_header;
        }
        break;
    case DEMUX_DATA:
        if (state_bytes > end - buf) {
            decode_mpeg2 (buf, end);
            state_bytes -= end - buf;
            return 0;
        }
        decode_mpeg2 (buf, buf + state_bytes);
        buf += state_bytes;
        break;
    case DEMUX_SKIP:
        if (state_bytes > end - buf) {
            state_bytes -= end - buf;
            return 0;
        }
        buf += state_bytes;
        break;
    }

    while (1) {
    payload_start:
	header = buf;
	bytes = end - buf;
    continue_header:
	NEEDBYTES (2);
	if (header[0] != 0x41 || header[1] != 0x56) {
	    if (header != head_buf) {
		buf++;
		goto payload_start;
	    } else {
		header[0] = header[1];
		bytes = 1;
		goto continue_header;
	    }
	}
	NEEDBYTES (8);
	if (header[2] != 1) {
	    DONEBYTES (8);
	    bytes = (header[6] << 8) + header[7];
	    if (bytes > end - buf) {
		state = DEMUX_SKIP;
		state_bytes = bytes - (end - buf);
		return 0;
	    } 
	    buf += bytes; 
	} else {
	    len = 8;
	    if (header[5] & 0x10) {
		len = 12 + (header[5] & 3);
		NEEDBYTES (len);
		decode_mpeg2 (header + 12, header + len);
		mpeg2_tag_picture (mpeg2dec,
				   ((header[8] << 24) | (header[9] << 16) |
				    (header[10] << 8) | header[11]), 0);
	    }
	    DONEBYTES (len);
	    bytes = (header[6] << 8) + header[7] + 8 - len;
	    if (bytes > end - buf) {
		decode_mpeg2 (buf, end);
		state = DEMUX_DATA;
		state_bytes = bytes - (end - buf);
		return 0;
	    } else if (bytes > 0) {
		decode_mpeg2 (buf, buf + bytes);
		buf += bytes;
	    }
	}
    }
}

static void pva_loop (void)
{
    uint8_t * buffer = (uint8_t *) malloc (buffer_size);
    uint8_t * end;

    if (buffer == NULL)
	exit (1);
    do {
	end = buffer + fread (buffer, 1, buffer_size, in_file);
	pva_demux (buffer, end);
    } while (end == buffer + buffer_size && !sigint);
    free (buffer);
}

static void ts_loop (void)
{
    uint8_t * buffer = (uint8_t *) malloc (buffer_size);
    uint8_t * buf;
    uint8_t * nextbuf;
    uint8_t * data;
    uint8_t * end;
    int pid;

    if (buffer == NULL || buffer_size < 188)
	exit (1);
    buf = buffer;
    do {
	end = buf + fread (buf, 1, buffer + buffer_size - buf, in_file);
	buf = buffer;
	for (; (nextbuf = buf + 188) <= end; buf = nextbuf) {
	    if (*buf != 0x47) {
		fprintf (stderr, "bad sync byte\n");
		nextbuf = buf + 1;
		continue;
	    }
	    pid = ((buf[1] << 8) + buf[2]) & 0x1fff;
	    if (pid != demux_pid)
		continue;
	    data = buf + 4;
	    if (buf[3] & 0x20) {	/* buf contains an adaptation field */
		data = buf + 5 + buf[4];
		if (data > nextbuf)
		    continue;
	    }
	    if (buf[3] & 0x10)
		demux (data, nextbuf,
		       (buf[1] & 0x40) ? DEMUX_PAYLOAD_START : 0);
	}
	if (end != buffer + buffer_size)
	    break;
	memcpy (buffer, buf, end - buf);
	buf = buffer + (end - buf);
    } while (!sigint);
    free (buffer);
}

static void es_loop (void)
{
    uint8_t * buffer = (uint8_t *) malloc (buffer_size);
    uint8_t * end;

    if (buffer == NULL)
	exit (1);
    do {//每次读取buffer_size即4096字节进行解码（帧数不确定）
	end = buffer + fread (buffer, 1, buffer_size, in_file);
	decode_mpeg2 (buffer, end);
    } while (end == buffer + buffer_size && !sigint);
    free (buffer);
}


static void save_pgm (int width, int height,
		      int chroma_width, int chroma_height,
		      uint8_t * const * buf, int num)
{
    char filename[100];
    FILE * pgmfile;
    int i;
    static uint8_t black[16384] = { 0 };

    sprintf (filename, "%d.pgm", num);
    pgmfile = fopen (filename, "wb");
    if (!pgmfile) {
	fprintf (stderr, "Could not open file \"%s\".\n", filename);
	exit (1);
    }
    fprintf (pgmfile, "P5\n%d %d\n255\n",
	     2 * chroma_width, height + chroma_height);
    for (i = 0; i < height; i++) {
	fwrite (buf[0] + i * width, width, 1, pgmfile);
	fwrite (black, 2 * chroma_width - width, 1, pgmfile);
    }
    for (i = 0; i < chroma_height; i++) {
	fwrite (buf[1] + i * chroma_width, chroma_width, 1, pgmfile);
	fwrite (buf[2] + i * chroma_width, chroma_width, 1, pgmfile);
    }
    fclose (pgmfile);
}

static struct fbuf_s {
    uint8_t * mbuf[3];
    uint8_t * yuv[3];
    int used;
} fbuf[3];

static struct fbuf_s * get_fbuf (void)
{
    int i;

    for (i = 0; i < 3; i++)
	if (!fbuf[i].used) {
	    fbuf[i].used = 1;
	    return fbuf + i;
	}
    fprintf (stderr, "Could not find a free fbuf.\n");
    exit (1);
}

static void sample5 (FILE * mpgfile)
{
#define BUFFER_SIZE 4096//16384//4096
#define ALIGN_16(p) ((void *)(((uint32_t)(p) + 15) & ~((uint32_t)15)))//uintptr_t
    uint8_t buffer[BUFFER_SIZE*100];
    mpeg2dec_t * decoder;
    const mpeg2_info_t * info;
    const mpeg2_sequence_t * sequence;
    mpeg2_state_t state;
    size_t size;
    int framenum = 0;
    int i, j;
    struct fbuf_s * current_fbuf;

	FILE *fpout1;
	int del01=0;

	int delN=0;
	int delNFrame=0;

					fpout1 = fopen("./YUVtest1.yuv", "wb");
    decoder = mpeg2_init ();
    if (decoder == NULL) 
	{
		fprintf (stderr, "Could not allocate a decoder object.\n");
		exit (1);
    }
    info = mpeg2_info (decoder);

    size = (size_t)-1;
    do 
	{
		state = mpeg2_parse (decoder);
		sequence = info->sequence;
		switch (state) 
		{
			case STATE_BUFFER:
				//输入为BUFFER_SIZE大小的buffer
				size = fread (buffer, 1, BUFFER_SIZE, mpgfile);
				//delN+=1000;
				mpeg2_buffer (decoder, buffer, buffer + size);
				break;
			case STATE_SEQUENCE:
				mpeg2_custom_fbuf (decoder, 1);
				for (i = 0; i < 3; i++) 
				{
					fbuf[i].mbuf[0] = (uint8_t *) malloc (sequence->width *
									 sequence->height + 15);
					fbuf[i].mbuf[1] = (uint8_t *) malloc (sequence->chroma_width * 
									 sequence->chroma_height + 15);
					fbuf[i].mbuf[2] = (uint8_t *) malloc (sequence->chroma_width *  
									 sequence->chroma_height + 15);
					if (!fbuf[i].mbuf[0] || !fbuf[i].mbuf[1] || !fbuf[i].mbuf[2]) 
					{
						fprintf (stderr, "Could not allocate an output buffer.\n");
						exit (1);
					}
					for (j = 0; j < 3; j++)
						fbuf[i].yuv[j] = ALIGN_16 (fbuf[i].mbuf[j]);
					fbuf[i].used = 0;
				}
				for (i = 0; i < 2; i++) 
				{
					current_fbuf = get_fbuf ();
					mpeg2_set_buf (decoder, current_fbuf->yuv, current_fbuf);
				}
				break;
			case STATE_PICTURE:
				current_fbuf = get_fbuf ();
				mpeg2_set_buf (decoder, current_fbuf->yuv, current_fbuf);
				break;
			case STATE_SLICE:
			case STATE_END:
			case STATE_INVALID_END:
				//输出YUV420数据为info->display_fbuf->buf
				if (info->display_fbuf)
				{
					size+=0;
					delNFrame++;

					//save_pgm (sequence->width, sequence->height,
					//	  sequence->chroma_width, sequence->chroma_height,
					//	  info->display_fbuf->buf, framenum++);
					if((delNFrame>3)&&(delNFrame<6))
					{
                    #if 0
					del01=fwrite(info->display_fbuf->buf[0],720*576 , 1, fpout1);
					del01=fwrite(info->display_fbuf->buf[1],720*576/4 , 1, fpout1);
					del01=fwrite(info->display_fbuf->buf[2],720*576/4 , 1, fpout1);
                    #endif
					size+=0;
					}
					

				}

				if (info->discard_fbuf)
						((struct fbuf_s *)info->discard_fbuf->id)->used = 0;
				if (state != STATE_SLICE)
					for (i = 0; i < 3; i++)
						for (j = 0; j < 3; j++)
							free (fbuf[i].mbuf[j]);
				break;
			default:
				break;
		}//end switch
    } while (size);
	fclose(fpout1);

    mpeg2_close (decoder);
}
//主函数入口
#if 1 
int main (int argc, char ** argv)
{
    FILE * mpgfile;
    double t3;
    int t1,t2,t4;

    if (argc > 1) {
	mpgfile = fopen (argv[1], "rb");//
	if (!mpgfile) {
	    fprintf (stderr, "Could not open file \"%s\".\n", argv[1]);
	    exit (1);
	}
    } else
		mpgfile = stdin;

	mpgfile = fopen ("mpeg2.mpg", "rb");//argv[1]
    mdtime(0);
    t1 = clock();
    sample5 (mpgfile);
    t2 = clock();
    t4 = t2 -t1;
    t3=mdtime(1);
    printf("the time is %f\n",t3);
    printf("the time is %d\n",t4);

    return 0;
}
#endif
void cpdata(int16_t *s,int16_t *d)
{
    int i;
    for(i=0;i<64;i++)
    {
        d[i]=s[i];
    }
}
#if 0
int maino()
{
    int16_t data0[64]={6336,0,0,0,160,0,0,0,\
    368,0,0,0,208,0,0,0,\
    0,0,0,0,0,0,0,0,\
    65280,0,0,0,0,0,0,0,\
    64896,0,0,0,65056,0,0,0,\
    65328,0,0,0,0,0,0,0,\
    256,0,0,0,0,0,0,0,\
    0,0,0,0,0,0,0,0};
    uint8_t odata[64];
    int16_t data[64];
    mpeg2_init();
    cpdata(data0,data);
    mpeg2_idct_sse2_init();
    mpeg2_idct_copy_sse2(data,odata,8);
    printf("c \n");
    mpeg2_idct_init(0);
    cpdata(data0,data);
    mpeg2_idct_copy_c(data,odata,8);
  //  mpeg2_idct_copy(data,odata,8);
}
#endif






