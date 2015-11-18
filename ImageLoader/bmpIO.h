/***********************************************************/
/*                                                         */
/* BMP                                                     */
/*                                                         */
/* Written by Michael Sweet.                               */
/***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


/***********************************************************/
/* GLOBAL DEFINES                                          */
/***********************************************************/
typedef unsigned char uchar;

#define BF_TYPE 0x4D42             

#define BI_RGB       0             /* No compression - straight BGR data */
#define BI_RLE8      1             /* 8-bit run-length compression */
#define BI_RLE4      2             /* 4-bit run-length compression */
#define BI_BITFIELDS 3             /* RGB bitmap with RGB masks */

/***********************************************************/
/* GLOBAL CONSTANTS                                        */
/***********************************************************/

/***********************************************************/
/* GLOBAL STRUCTURES                                       */
/***********************************************************/
typedef struct {                     
    unsigned short bfType;           
    unsigned int   bfSize;           
    unsigned short bfReserved1;      
    unsigned short bfReserved2;      
    unsigned int   bfOffBits;        
} BITMAPFILEHEADER;

typedef struct {                     
    unsigned int   biSize;           
    int            biWidth;          
    int            biHeight;         
    unsigned short biPlanes;         
    unsigned short biBitCount;       
    unsigned int   biCompression;    
    unsigned int   biSizeImage;      
    int            biXPelsPerMeter;  
    int            biYPelsPerMeter;  
    unsigned int   biClrUsed;        
    unsigned int   biClrImportant;   
} BITMAPINFOHEADER;

typedef struct {                      
    unsigned char  rgbBlue;          
    unsigned char  rgbGreen;         
    unsigned char  rgbRed;           
    unsigned char  rgbReserved;     
} RGBQUAD;

typedef struct {                      
    BITMAPINFOHEADER bmiHeader;      
    RGBQUAD          bmiColors[256]; 
} BITMAPINFO;


/***********************************************************/
/* FUNTION PROTOTYPES                                      */
/***********************************************************/
extern uchar *LoadDIBitmap(char filename[], BITMAPINFO **info);
extern int    SaveDIBitmap(char filename[], BITMAPINFO *info, uchar *bits);

