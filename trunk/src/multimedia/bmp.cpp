#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#define debp(f,...) printf(f,##__VA_ARGS__)
#ifdef WIN32
unsigned char  *                          /* O - Bitmap data */
mLoadbmp(const char *filename, /* I - File to load */
             BITMAPINFO **info)    /* O - Bitmap information */
    {
    FILE             *fp;          /* Open file pointer */
    unsigned char           *bits;        /* Bitmap pixel bits */
    int              bitsize;      /* Size of bitmap */
    int              infosize;     /* Size of header information */
    BITMAPFILEHEADER header;       /* File header */
    int              ret;

    /* Try opening the file; use "rb" mode to read this *binary* file. */
    if ((fp = fopen(filename, "rb")) == NULL){
	debp("open err\n");
        return (NULL);
}

    /* Read the file header and any following bitmap information... */
    if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1)
        {
        /* Couldn't read the file header - return NULL... */
	fclose(fp);
	debp("head err\n");
        return (NULL);
        }

    if (header.bfType != BF_TYPE)	/* Check for BM reversed... */
        {
        /* Not a bitmap file - return NULL... */
        fclose(fp);
	debp("Type err\n");
        return (NULL);
        }

    infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
    if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL)
        {
        /* Couldn't allocate memory for bitmap info - return NULL... */
        fclose(fp);
	debp("malloc err\n");
        return (NULL);
        }
    ret=fread(*info, 1, infosize, fp);
    if (ret < infosize)
        {
        /* Couldn't read the bitmap header - return NULL... */
        free(*info);
        fclose(fp);
	debp("read info err %d:%d\n",ret,infosize);
        return (NULL);
        }

    /* Now that we have all the header info read in, allocate memory for *
 *      * the bitmap and read *it* in...                                    */
    if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
        bitsize = ((*info)->bmiHeader.biWidth *
                   (*info)->bmiHeader.biBitCount + 7) / 8 *
  	           abs((*info)->bmiHeader.biHeight);

    if ((bits =(unsigned char*)  malloc(bitsize)) == NULL)
        {
        /* Couldn't allocate memory - return NULL! */
        free(*info);
        fclose(fp);
	debp("malloc err\n");
        return (NULL);
        }

    if (fread(bits, 1, bitsize, fp) < bitsize){
        /* Couldn't read bitmap - free memory and return NULL! */
        free(*info);
        free(bits);
        fclose(fp);
	debp("read data err\n");
        return (NULL);
        }

    /* OK, everything went fine - return the allocated bitmap... */
    fclose(fp);
    return (bits);
    }
#else
static unsigned short read_word(FILE *fp);
static unsigned int   read_dword(FILE *fp);
static int            read_long(FILE *fp);

static int            write_word(FILE *fp, unsigned short w);
static int            write_dword(FILE *fp, unsigned int dw);
static int            write_long(FILE *fp, int l);


/*
 *  * 'LoadDIBitmap()' - Load a DIB/BMP file from disk.
 *   *
 *    * Returns a pointer to the bitmap if successful, NULL otherwise...
 *     */

unsigned char  *                          /* O - Bitmap data */
mLoadbmp(const char *filename, /* I - File to load */
             BITMAPINFO **info)    /* O - Bitmap information */
    {
    FILE             *fp;          /* Open file pointer */
    unsigned char           *bits;        /* Bitmap pixel bits */
    unsigned char           *ptr;         /* Pointer into bitmap */
    unsigned char           temp;         /* Temporary variable to swap red and blue */
    int              x, y;         /* X and Y position in image */
    int              length;       /* Line length */
    int              bitsize;      /* Size of bitmap */
    int              infosize;     /* Size of header information */
    BITMAPFILEHEADER header;       /* File header */

    /* Try opening the file; use "rb" mode to read this *binary* file. */
    if ((fp = fopen(filename, "rb")) == NULL)
        return (NULL);

    /* Read the file header and any following bitmap information... */
    header.bfType      = read_word(fp);
    header.bfSize      = read_dword(fp);
    header.bfReserved1 = read_word(fp);
    header.bfReserved2 = read_word(fp);
    header.bfOffBits   = read_dword(fp);

    if (header.bfType != BF_TYPE) /* Check for BM reversed... */
        {
        /* Not a bitmap file - return NULL... */
        fclose(fp);
        return (NULL);
        }

    infosize = header.bfOffBits - 18;
    if ((*info = (BITMAPINFO *)malloc(sizeof(BITMAPINFO))) == NULL)
        {
        /* Couldn't allocate memory for bitmap info - return NULL... */
        fclose(fp);
        return (NULL);
        }

    (*info)->bmiHeader.biSize          = read_dword(fp);
    (*info)->bmiHeader.biWidth         = read_long(fp);
    (*info)->bmiHeader.biHeight        = read_long(fp);
    (*info)->bmiHeader.biPlanes        = read_word(fp);
    (*info)->bmiHeader.biBitCount      = read_word(fp);
    (*info)->bmiHeader.biCompression   = read_dword(fp);
    (*info)->bmiHeader.biSizeImage     = read_dword(fp);
    (*info)->bmiHeader.biXPelsPerMeter = read_long(fp);
    (*info)->bmiHeader.biYPelsPerMeter = read_long(fp);
    (*info)->bmiHeader.biClrUsed       = read_dword(fp);
    (*info)->bmiHeader.biClrImportant  = read_dword(fp);

#if 0 
    fseek(fp,infosize-40,SEEK_CUR);
#else 
    if (infosize > 40)
	if (fread((*info)->bmiColors, infosize - 40, 1, fp) < 1)
            {
            /* Couldn't read the bitmap header - return NULL... */
            free(*info);
            fclose(fp);
            return (NULL);
            }
#endif
    /* Now that we have all the header info read in, allocate memory for *
 *      * the bitmap and read *it* in...                                    */
    if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
        bitsize = ((*info)->bmiHeader.biWidth *
                   (*info)->bmiHeader.biBitCount + 7) / 8 *
  	           abs((*info)->bmiHeader.biHeight);

    if ((bits = (unsigned char *)malloc(bitsize)) == NULL)
        {
        /* Couldn't allocate memory - return NULL! */
        free(*info);
        fclose(fp);
        return (NULL);
        }

    if (fread(bits, 1, bitsize, fp) < bitsize)
        {
        /* Couldn't read bitmap - free memory and return NULL! */
        free(*info);
        free(bits);
        fclose(fp);
        return (NULL);
        }

    /* Swap red and blue */
    length = ((*info)->bmiHeader.biWidth * 3 + 3) & ~3;
    for (y = 0; y < (*info)->bmiHeader.biHeight; y ++)
        for (ptr = bits + y * length, x = (*info)->bmiHeader.biWidth;
             x > 0;
	     x --, ptr += 3)
	    {
	    temp   = ptr[0];
	    ptr[0] = ptr[2];
	    ptr[2] = temp;
	    }
    /* OK, everything went fine - return the allocated bitmap... */
    fclose(fp);
    return (bits);
    }
static unsigned short     /* O - 16-bit unsigned integer */
read_word(FILE *fp)       /* I - File to read from */
    {
    unsigned char b0, b1; /* Bytes from file */

    b0 = getc(fp);
    b1 = getc(fp);

    return ((b1 << 8) | b0);
    }


/*
 *  * 'read_dword()' - Read a 32-bit unsigned integer.
 *   */

static unsigned int               /* O - 32-bit unsigned integer */
read_dword(FILE *fp)              /* I - File to read from */
    {
    unsigned char b0, b1, b2, b3; /* Bytes from file */

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
    }


/*
 *  * 'read_long()' - Read a 32-bit signed integer.
 *   */

static int                        /* O - 32-bit signed integer */
read_long(FILE *fp)               /* I - File to read from */
    {
    unsigned char b0, b1, b2, b3; /* Bytes from file */

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
    }


/*
 *  * 'write_word()' - Write a 16-bit unsigned integer.
 *   */

static int                     /* O - 0 on success, -1 on error */
write_word(FILE           *fp, /* I - File to write to */
           unsigned short w)   /* I - Integer to write */
    {
    putc(w, fp);
    return (putc(w >> 8, fp));
    }


/*
 *  * 'write_dword()' - Write a 32-bit unsigned integer.
 *   */

static int                    /* O - 0 on success, -1 on error */
write_dword(FILE         *fp, /* I - File to write to */
            unsigned int dw)  /* I - Integer to write */
    {
    putc(dw, fp);
    putc(dw >> 8, fp);
    putc(dw >> 16, fp);
    return (putc(dw >> 24, fp));
    }


/*
 *  * 'write_long()' - Write a 32-bit signed integer.
 *   */

static int           /* O - 0 on success, -1 on error */
write_long(FILE *fp, /* I - File to write to */
           int  l)   /* I - Integer to write */
    {
    putc(l, fp);
    putc(l >> 8, fp);
    putc(l >> 16, fp);
    return (putc(l >> 24, fp));
    }
#endif
