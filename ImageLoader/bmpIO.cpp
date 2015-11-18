/***********************************************************/
/*                                                         */
/* BMP                                                     */
/*                                                         */
/* Written by Michael Sweet.                               */
/***********************************************************/
#include "bmpIO.h"

static unsigned short read_word(FILE *fp);
static unsigned int   read_dword(FILE *fp);
static int            read_long(FILE *fp);

static int            write_word(FILE *fp, unsigned short w);
static int            write_dword(FILE *fp, unsigned int dw);
static int            write_long(FILE *fp, int l);


/****************************************/
/* Load a DIB/BMP file from disk.       */
/****************************************/
uchar *LoadDIBitmap(char filename[], BITMAPINFO **info) {
uchar            *bits;        /* Bitmap pixel bits */
uchar            *ptr;         /* Pointer into bitmap */
uchar            temp;         /* Temporary variable to swap red and blue */
int               x, y;         /* X and Y position in image */
int              length;       /* Line length */
int              bitsize;      /* Size of bitmap */
int              infosize;     /* Size of header information */
FILE             *fp;          /* Open file pointer */
BITMAPFILEHEADER header;       /* File header */
    if ((fp = fopen(filename, "rb")) == NULL) return NULL;
    header.bfType      = read_word(fp);
    header.bfSize      = read_dword(fp);
    header.bfReserved1 = read_word(fp);
    header.bfReserved2 = read_word(fp);
    header.bfOffBits   = read_dword(fp);
    if (header.bfType != BF_TYPE) {
       fclose(fp);
       return NULL;
    }
    infosize = (header.bfOffBits - 18);
    if ((*info = (BITMAPINFO *)malloc(sizeof(BITMAPINFO))) == NULL) {
       fclose(fp);
       return NULL;
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
    if (infosize > 40) {
       if (fread((*info)->bmiColors, infosize - 40, 1, fp) < 1) {
          free(*info);
          fclose(fp);
          return NULL;
       }
    }
    if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0) {
        bitsize = ((*info)->bmiHeader.biWidth *
                   (*info)->bmiHeader.biBitCount + 7) / 8 *
  	           abs((*info)->bmiHeader.biHeight);
    }
    if ((bits = (uchar *)calloc(bitsize, 1)) == NULL) {
        free(*info);
        fclose(fp);
        return NULL;
    }
    if (fread(bits, 1, bitsize, fp) < bitsize) {
        free(*info);
        free(bits);
        fclose(fp);
        return NULL;
    }
    /* Swap red and blue */
/*
    length = ((*info)->bmiHeader.biWidth * 3 + 3) & ~3;
    for (y = 0; y < (*info)->bmiHeader.biHeight; y ++) {
        for (ptr = bits + y * length, x = (*info)->bmiHeader.biWidth;
             x > 0;
	     x --, ptr += 3) {
	    temp   = ptr[0];
	    ptr[0] = ptr[2];
	    ptr[2] = temp;
	 }
    }
*/
    fclose(fp);
    return bits;
}

/****************************************/
/* Save a DIB/BMP file to a disk.       */
/****************************************/
int SaveDIBitmap(char filename[], BITMAPINFO *info, uchar *bits) {
FILE *fp;                 /* Open file pointer */
int  size,                /* Size of file */
infosize,                 /* Size of bitmap info */
bitsize;                  /* Size of bitmap pixels */
    if ((fp = fopen(filename, "wb")) == NULL) return (-1);
    if (info->bmiHeader.biSizeImage == 0) {
	bitsize = (info->bmiHeader.biWidth *
        	   info->bmiHeader.biBitCount + 7) / 8 *
		   abs(info->bmiHeader.biHeight);
    } else {
	bitsize = info->bmiHeader.biSizeImage;
    }
    infosize = sizeof(BITMAPINFOHEADER);
    switch (info->bmiHeader.biCompression) {
	case BI_BITFIELDS :
            infosize += 12; 
            if (info->bmiHeader.biClrUsed == 0) break;
	case BI_RGB :
            if (info->bmiHeader.biBitCount > 8 &&
                info->bmiHeader.biClrUsed == 0) break;
	case BI_RLE8 :
	case BI_RLE4 :
            if (info->bmiHeader.biClrUsed == 0) {
              infosize += (1 << info->bmiHeader.biBitCount) * 4;
	    } else {
              infosize += info->bmiHeader.biClrUsed * 4;
            }
	    break;
    }

    size = sizeof(BITMAPFILEHEADER) + infosize + bitsize;
    write_word(fp, BF_TYPE);        /* bfType */
    write_dword(fp, size);          /* bfSize */
    write_word(fp, 0);              /* bfReserved1 */
    write_word(fp, 0);              /* bfReserved2 */
    write_dword(fp, 18 + infosize); /* bfOffBits */
    write_dword(fp, info->bmiHeader.biSize);
    write_long(fp, info->bmiHeader.biWidth);
    write_long(fp, info->bmiHeader.biHeight);
    write_word(fp, info->bmiHeader.biPlanes);
    write_word(fp, info->bmiHeader.biBitCount);
    write_dword(fp, info->bmiHeader.biCompression);
    write_dword(fp, info->bmiHeader.biSizeImage);
    write_long(fp, info->bmiHeader.biXPelsPerMeter);
    write_long(fp, info->bmiHeader.biYPelsPerMeter);
    write_dword(fp, info->bmiHeader.biClrUsed);
    write_dword(fp, info->bmiHeader.biClrImportant);

    if (infosize > 40) {
	if (fwrite(info->bmiColors, infosize - 40, 1, fp) < 1) {
            fclose(fp);
            return (-1);
        }
    }
    if (fwrite(bits, 1, bitsize, fp) < bitsize) {
        fclose(fp);
        return (-1);
    }

    fclose(fp);
    return (0);
}


/****************************************/
/* Read a 16-bit unsigned integer.      */
/****************************************/
static unsigned short read_word(FILE *fp) {
uchar b0, b1; 
    b0 = getc(fp);
    b1 = getc(fp);
    return ((b1 << 8) | b0);
}

/****************************************/
/* Read a 32-bit unsigned integer.      */
/****************************************/
static unsigned int read_dword(FILE *fp) {
uchar b0, b1, b2, b3; 
    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);
    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

/****************************************/
/* Read a 32-bit signed integer.        */
/****************************************/
static int read_long(FILE *fp) {
uchar b0, b1, b2, b3; 
    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);
    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

/****************************************/
/* Write a 16-bit unsigned integer.     */
/* O - 0 on success, -1 on error        */
/****************************************/
static int write_word(FILE *fp, unsigned short w) {
    putc(w, fp);
    return (putc(w >> 8, fp));
}

/****************************************/
/* Write a 32-bit unsigned integer.     */
/* O - 0 on success, -1 on error        */
/****************************************/
static int write_dword(FILE *fp, unsigned int dw) {
    putc(dw, fp);
    putc(dw >> 8, fp);
    putc(dw >> 16, fp);
    return (putc(dw >> 24, fp));
}

/****************************************/
/* Write a 32-bit signed integer.       */
/* O - 0 on success, -1 on error        */
/****************************************/
static int write_long(FILE *fp, int l) {
    putc(l, fp);
    putc(l >> 8, fp);
    putc(l >> 16, fp);
    return (putc(l >> 24, fp));
}
