#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
extern "C" { 
#include <gif_lib.h> 
}

#define min(a,b) ((a) < (b) ? (a) : (b))
#define gflush return(FH_ERROR_FILE);
#define grflush { DGifCloseFile(gft); return(FH_ERROR_FORMAT); }
#define mgrflush { free(lb); free(slb); DGifCloseFile(gft); return(FH_ERROR_FORMAT); }
#define agflush return(FH_ERROR_FORMAT);
#define agrflush { DGifCloseFile(gft); return(FH_ERROR_FORMAT); }

/***********************************************************/
/* GLOBAL DEFINES                                          */
/***********************************************************/
typedef unsigned char uchar;

struct formathandler {
    struct formathandler *next;
    int (*get_size)(char *, int *, int*);
    int (*get_pic)(char *, uchar *, int, int);
    int (*id_pic)(char *);
};

#define FH_ERROR_OK 0
#define FH_ERROR_FILE 1
#define FH_ERROR_FORMAT 2

/***********************************************************/
/* FUNTION PROTOTYPES                                      */
/***********************************************************/
void add_format(int (*picsize)(char *,int *,int*),
                int (*picread)(char *,unsigned char *,int,int), 
                int (*id)(char*));
extern int fh_gif_getsize(char *, int *, int*);
extern int fh_gif_load(char *,uchar *, int,int);
extern int fh_gif_id(char *);
void GIF_FILE_READ(char *FileName, uchar data[], int *color);
