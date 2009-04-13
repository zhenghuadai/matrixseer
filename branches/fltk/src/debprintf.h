#ifndef MATRIXDEB      
#define MATRIXDEB
#ifdef DZHDEB
#define debprintf(format,...) { \
    fprintf(stdout,format,##__VA_ARGS__);\
}
#else
#define debprintf(format,...) 
#endif
#define DEBprintf debprintf
#endif             
