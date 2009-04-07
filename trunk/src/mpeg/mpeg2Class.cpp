/*
 * mpeg2dec.c
 * Copyright (C) 2000-2003 Michel Lespinasse <walken@zoy.org>
 * Copyright (C) 1999-2000 Aaron Holtzman <aholtzma@ess.engr.uvic.ca>
 *
 * This file is part of mpeg2dec, a free MPEG-2 video stream mpeg2Decoder.
 * See http://libmpeg2.sourceforge.net/ for updates.
 *
 * mpeg2dec is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * mpeg2dec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <getopt.h>
#include <inttypes.h>
#include <sys/time.h>
#include "mpeg2.h"
//#include "gettimeofday.h"
#include "mpeg2Class.h"

static int buffer_size = 40960;//*1024;
static int sigint = 0;

#define ALIGN_16(p) ((void *)(((uint32_t)(p) + 15) & ~((uint32_t)15)))//uintptr_t

#ifdef HAVE_GETTIMEOFDAY

static RETSIGTYPE signal_handler (int sig)
{
    sigint = 1;
    signal (sig, SIG_DFL);
    return (RETSIGTYPE)0;
}

static void print_fps (int final)
{
    static uint32_t frame_counter = 0;
    static struct timeval tv_beg, tv_start;
    static int total_elapsed;
    static int last_count = 0;
    struct timeval tv_end;
    float fps, tfps;
    int frames, elapsed;


    gettimeofday (&tv_end, NULL);

    if (!frame_counter) {
	tv_start = tv_beg = tv_end;
	signal (SIGINT, signal_handler);
    }

    elapsed = (tv_end.tv_sec - tv_beg.tv_sec) * 100 +
	(tv_end.tv_usec - tv_beg.tv_usec) / 10000;
    total_elapsed = (tv_end.tv_sec - tv_start.tv_sec) * 100 +
	(tv_end.tv_usec - tv_start.tv_usec) / 10000;

    if (final) {
	if (total_elapsed)
	    tfps = frame_counter * 100.0 / total_elapsed;
	else
	    tfps = 0;

	fprintf (stderr,"\n%d frames decoded in %.2f seconds (%.2f fps)\n",
		 frame_counter, total_elapsed / 100.0, tfps);

	return;
    }

    frame_counter++;

    if (elapsed < 50)	/* only display every 0.50 seconds */
	return;

    tv_beg = tv_end;
    frames = frame_counter - last_count;

    fps = frames * 100.0 / elapsed;
    tfps = frame_counter * 100.0 / total_elapsed;

    fprintf (stderr, "%d frames in %.2f sec (%.2f fps), "
	     "%d last %.2f sec (%.2f fps)\033[K\r", frame_counter,
	     total_elapsed / 100.0, tfps, frames, elapsed / 100.0, fps);

    last_count = frame_counter;
}

#else /* !HAVE_GETTIMEOFDAY */

static void print_fps (int final)
{
}

#endif

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

void mpeg2player::run (void)
{

}

void mpeg2player:: initial()
{
    mpeg2dec = mpeg2_init ();
    mpeg2_malloc_hooks (malloc_hook, NULL);
    videoFilep = fopen(fileName,"r");
    videoData = NULL;
    m_rgbImage = NULL;

    displayFrameid =0;
    bufferFrameid = 0;
    sem_init(&semPlay,0,1);
}



void mpeg2player::displayFrame(uint8_t * const *buf,void* frame)
{
    videoData = buf[0];
    m_rgbImage =((struct fbuf_s*) frame) -> ximage; 
            if(m_rgbImage)  
            {
                //printf("draw %d \n",buf[0][20]);
                m_rgbImage ->buffer_changed();
                //m_rgbImage -> draw( fltk::Rectangle((int)x(), (int)y(), video_w, video_h));
            }
    redraw();
}

void copyimgData(unsigned int *dst,unsigned int *src,int w,int h)
{
    int i,j;
    for(i =0;i<h;i++)
        for(j=0;j<w;j++)
            dst[i*w +j] = src [i*w +j];
}

void mpeg2player::draw()
{
        //setcolor(GRAY20);
            //fillrect(0,0,w(),h());

    //if(videoData)
        //drawimage((uchar*)videoData,  RGB32,
                //fltk::Rectangle((int)x(), (int)y(), video_w, video_h));
//Widget::draw();
            if(m_rgbImage)  
            {
                //printf("%d\n",videoData[4]);
                m_rgbImage -> draw( fltk::Rectangle((int)x(), (int)0, video_w, video_h));
            }
            m_rgbImage = NULL;
}

struct fbuf_s * mpeg2player::get_fbuf (void)
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

int mpeg2player::create_fbuf(int width,int height,int chroma_width,int chroma_height)
{
    int j;
    int size = width * height * 4;
    video_w = width;
    video_h = height;
                myconvert = (void*)mpeg2convert_rgb(MPEG2CONVERT_RGB,32); 
    for (int i = 0; i < 3; i++) 
    {
        //if(fbuf[i].ximage) delete fbuf[i].ximage;
        if(fbuf[i].ximage ==NULL)
        fbuf[i].ximage = new Image(RGB32,width,height);
        fbuf[i].mbuf[0] = fbuf[i].ximage->buffer();//(uint8_t *) malloc (size + 15);
        if (!fbuf[i].mbuf[0]) 
        {
            fprintf (stderr, "Could not allocate an output buffer.\n");
            exit (1);
        }
        fbuf[i].mbuf[1] = NULL;
        fbuf[i].mbuf[2] = NULL;
        fbuf[i].buf[1] = NULL;
        fbuf[i].buf[2] = NULL;
        fbuf[i].buf[0] = fbuf[i].mbuf[0];
#ifdef YUV_
        fbuf[i].mbuf[1] = (uint8_t *) malloc (chroma_width * chroma_height + 15);
        fbuf[i].mbuf[2] = (uint8_t *) malloc (chroma_width * chroma_height + 15);
        if (!fbuf[i].mbuf[0] || !fbuf[i].mbuf[1] || !fbuf[i].mbuf[2]) 
        {
            fprintf (stderr, "Could not allocate an output buffer.\n");
            exit (1);
        }
        fbuf[i].buf[0] = fbuf[i].mbuf[0];
        for (j = 1; j < 3; j++)
            fbuf[i].buf[j] = (uint8_t*)ALIGN_16 (fbuf[i].mbuf[j]);
#endif
        fbuf[i].used = 0;
    }
}

void mpeg2player::deleteTimer()
{
    setitimer(ITIMER_REAL,NULL,NULL);
}

void mpeg2player::next()
{
    //if(displayFrameid == bufferFrameid)
    {sem_post(&semPlay);};
    displayFrameid ++;
    //printf("semPost\n");
}
void mpeg2player::decodeMpeg2() 
{
#define BUFFER_SIZE 4096//16384//4096
    uint8_t buffer[BUFFER_SIZE*128];
    mpeg2dec_t * mpeg2Decoder;
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
    int frameid=0;
    fpout1 = fopen("./YUVtest1.buf", "wb");
    mpeg2Decoder = mpeg2_init ();
    if (mpeg2Decoder == NULL) 
    {
        fprintf (stderr, "Could not allocate a mpeg2Decoder object.\n");
        exit (1);
    }
    info = mpeg2_info (mpeg2Decoder);

    size = (size_t)-1;
    do 
    {
        state = mpeg2_parse (mpeg2Decoder);
        sequence = info->sequence;
        switch (state) 
        {
            case STATE_BUFFER:
                //ÊäÈëÎªBUFFER_SIZE´óÐ¡µÄbuffer
                size = fread (buffer, 1, BUFFER_SIZE, videoFilep);
                //delN+=1000;
                mpeg2_buffer (mpeg2Decoder, buffer, buffer + size);
                break;
            case STATE_SEQUENCE:
                typedef int (* myconvert_t) (int stage, void * id,
                        const struct mpeg2_sequence_s * sequence,
                        int stride, uint32_t accel, void * arg,
                        struct mpeg2_convert_init_s * result);
                //mpeg2_convert_t*
                //myconvert = (void*)mpeg2convert_rgb(MPEG2CONVERT_RGB,32); 
                //mpeg2_custom_fbuf (mpeg2Decoder, 1);
                create_fbuf(sequence->width,sequence->height,
                        sequence->chroma_width,sequence->chroma_height);
                mpeg2_convert(mpeg2Decoder,(myconvert_t)myconvert,NULL);
                for (i = 0; i < 3; i++) 
                {
                    current_fbuf = get_fbuf ();
                    mpeg2_set_buf (mpeg2Decoder, current_fbuf->buf, current_fbuf);
                }
                break;
            case STATE_PICTURE:
                //printf("decode %d\n",frameid);
                //if(displayFrameid == bufferFrameid)
                {sem_wait(&semPlay);};
                bufferFrameid = displayFrameid;
                //current_fbuf = get_fbuf ();
                //mpeg2_set_buf (mpeg2Decoder, current_fbuf->buf, current_fbuf);
                break;
            case STATE_SLICE:
            case STATE_END:
            case STATE_INVALID_END:
                //Êä³öYUV420Êý¾ÝÎªinfo->display_fbuf->buf
                if (info->display_fbuf)
                {
                    //save_pgm (sequence->width, sequence->height,
                    //	  sequence->chroma_width, sequence->chroma_height,
                    //	  info->display_fbuf->buf, framenum++);
                    //if((delNFrame>3)&&(delNFrame<60))
                    {
                        displayFrame(info->display_fbuf->buf,info->display_fbuf->id);
                frameid ++;
                //printf("play%d\n",frameid);
                    }
                }

                //if (info->discard_fbuf)
                    //((struct fbuf_s *)info->discard_fbuf->id)->used = 0;
                if (state != STATE_SLICE)
                {
                    //for (i = 0; i < 3; i++)
                        //for (j = 0; j < 3; j++)
                            //free (fbuf[i].mbuf[j]);
                    deleteTimer();
                }
                break;
            default:
                break;
        }//end switch
    } while (size);
    fclose(fpout1);

    mpeg2_close (mpeg2Decoder);
}

mpeg2player::~mpeg2player ()
{
    int i,j;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++){
            //free (fbuf[i].mbuf[j]);
            delete fbuf[i].ximage;
            fbuf[i].ximage = NULL;
            }

}
