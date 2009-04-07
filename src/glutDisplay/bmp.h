#ifndef __MBMPHEAD
#define __MBMPHEAD
#include <stdio.h>
typedef struct                       /**** BMP file header structure ****/
    {
    unsigned short bfType;           /* Magic number for file */
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
    } BITMAPFILEHEADER;

#  define BF_TYPE 0x4D42             /* "MB" */

typedef struct                       /**** BMP file info structure ****/
    {
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
    } BITMAPINFOHEADER;
/*
typedef struct {
   unsigned short int Type;                 / * Magic identifier            * /
   unsigned int fSize;                       / * File size in bytes          * /
   unsigned short int Reserved1, Reserved2;
   unsigned int dataOffset;                     / * Offset to image data, bytes * /
} BMPFILEHEADER;
typedef struct {
   unsigned int Size;               / * Header size in bytes      * / 
   int width,height;                / * Width and height of image * /
   unsigned short int Planes;       / * Number of colour planes   * /
   unsigned short int Bits;         / * Bits per pixel            * /
   unsigned int Compression;        / * Compression type          * /
   unsigned int Imagesize;          / * Image size in bytes       * /
   int Xresolution,Yresolution;     / * Pixels per meter          * /
   unsigned int Ncolours;           / * Number of colours         * /
   unsigned int Importantcolours;   / * Important colours         * /
} BMPINFOHEADER;
*/
typedef struct                       /**** Colormap entry structure ****/
    {
    unsigned char  rgbBlue;          /* Blue value */
    unsigned char  rgbGreen;         /* Green value */
    unsigned char  rgbRed;           /* Red value */
    unsigned char  rgbReserved;      /* Reserved */
    } RGBQUAD;
typedef struct                       /**** Bitmap information structure ****/
    {
    BITMAPINFOHEADER bmiHeader;      /* Image header */
    RGBQUAD          bmiColors[1]; /* Image colormap */
    } BITMAPINFO;
typedef struct
{
    BITMAPINFOHEADER bmpiHeader;
    RGBQUAD * colormap;
    unsigned char *   data;	
}MBMPINFO;
unsigned char * mLoadbmp(const char *fn,BITMAPINFO ** info);
#endif
