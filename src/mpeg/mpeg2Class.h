/*
 * =====================================================================================
 *
 *       Filename:  mpeg2Class.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/10/2008 07:52:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gamil.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "mpeg2.h"
#include "mpeg2convert.h"
#include <fltk/Widget.h>
#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/ValueSlider.h>
#include <fltk/draw.h>
#include <fltk/RadioButton.h>
#include <fltk/Image.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
using namespace fltk;
struct fbuf_s {
    uint8_t * mbuf[3];
    uint8_t * buf[3];
    int used;
    Image *ximage;
    fbuf_s(){ximage = NULL;}
} ;

class mpeg2player : public Widget
{
    public:
        mpeg2player(int X,int Y,int W,int H):Widget(X,Y,W,H){};
        mpeg2player(int X,int Y,int W,int H,char *fn):Widget(X,Y,W,H){
            fileName = fn;
            initial();
        };
        ~mpeg2player();
        void play();
        void pause();
        void stop();
        void next();
        void replay() { fseek(videoFilep,0,SEEK_SET);};
        void run();
        void decodeMpeg2();
    private:
        void draw();
        void initial();
        void deleteTimer();
        int create_fbuf(int width,int height);
        int create_fbuf(int width,int height,int ,int );
        int  setup_fbuf(uint8_t **buf,void **id);
        int start_fbuf(uint8_t * const*,void*);
        void displayFrame(uint8_t * const *,void*);
        struct fbuf_s* get_fbuf();
        struct fbuf_s fbuf[3];
        char * fileName;
        FILE *videoFilep;
        mpeg2dec_t *mpeg2dec;
        int video_w;
        int video_h;
        int bytes_perLine;
        uint8_t *videoData;
        Image *m_rgbImage;
        void * myconvert;
        int displayFrameid;
        int bufferFrameid;
        sem_t semPlay;
};
