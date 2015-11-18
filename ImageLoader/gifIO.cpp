#include "gifIO.h"

int fh_gif_id(char *name) {
int fd;
char id[4];
    fd = open(name,O_RDONLY); if(fd==-1) return(0);
    read(fd,id,4);
    close(fd);
    if (id[0] == 'G' && id[1] == 'I' && id[2] == 'F') return(1);
    return(0);
}

inline void m_rend_gif_decodecolormap(uchar *cmb, uchar *rgbb, ColorMapObject *cm, 
                                      int s, int l) {
int i;
GifColorType *cmentry;
    for (i = 0; i < l; i++) {
	cmentry=&cm->Colors[cmb[i]];
	*(rgbb++)=cmentry->Red;
	*(rgbb++)=cmentry->Green;
	*(rgbb++)=cmentry->Blue;
    }
}

int fh_gif_load(char *name, uchar *buffer,int x,int y) {
int   i, j;
int   px, py, fby, fbx, fbl, ibxs;
int   eheight;
int   extcode;
int   spid;
int   cmaps;
uchar *fbptr;
uchar *lb;
uchar *slb;
GifFileType    *gft;
GifByteType    *extension;
GifRecordType  rt;
ColorMapObject *cmap;
    gft = DGifOpenFileName(name);
    if (gft == NULL) gflush;
    do {
       if (DGifGetRecordType(gft,&rt) == GIF_ERROR) grflush;
       switch(rt) {
	    case IMAGE_DESC_RECORD_TYPE:
		if (DGifGetImageDesc(gft) == GIF_ERROR) grflush;
		px  = gft->Image.Width;
		py  = gft->Image.Height;
		lb  = (uchar*)malloc(px*3);
		slb = (uchar*) malloc(px);
		if (lb != NULL && slb != NULL) {
		   cmap  = (gft->Image.ColorMap ? gft->Image.ColorMap : gft->SColorMap);
		   cmaps = cmap->ColorCount;
    		   ibxs  = ibxs * 3;
    		   fbptr = buffer;
		   if (!(gft->Image.Interlace)) {
		      for (i = 0; i < py; i++, fbptr += px*3) {
		    	  if (DGifGetLine(gft,slb,px) == GIF_ERROR) mgrflush;
		    	  m_rend_gif_decodecolormap(slb,lb,cmap,cmaps,px);
			  memcpy(fbptr,lb,px*3);
		      }
		    } else {
		      for (j = 0; j < 4; j++) {
			  fbptr = buffer;
			  for (i = 0; i < py; i++, fbptr += px*3) {
		    	      if(DGifGetLine(gft,slb,px)==GIF_ERROR) mgrflush;
		    	      m_rend_gif_decodecolormap(slb,lb,cmap,cmaps,px);
			      memcpy(fbptr,lb,px*3);
			  }
		      }
		    }
		}
		if (lb)  free(lb);
		if (slb) free(slb);
		break;
	    case EXTENSION_RECORD_TYPE:
		if (DGifGetExtension(gft,&extcode,&extension) == GIF_ERROR) grflush;
		while (extension != NULL)
		   if(DGifGetExtensionNext(gft,&extension) == GIF_ERROR) grflush;
		break;
	    default:
		break;
	}  
    }
    while (rt != TERMINATE_RECORD_TYPE);
    DGifCloseFile(gft);
    return(FH_ERROR_OK);
}

int fh_gif_getsize(char *name, int *x, int *y) {
int   i, j;
int   px, py, fby, fbx, fbl, ibxs;
int   eheight;
int   extcode;
int   spid;
int   cmaps;
uchar *fbptr;
uchar *lb;
uchar *slb;
GifFileType    *gft;
GifByteType    *extension;
GifRecordType  rt;
ColorMapObject *cmap;
    gft = DGifOpenFileName(name);
    if (gft == NULL) gflush;
    do {
       if (DGifGetRecordType(gft,&rt) == GIF_ERROR) grflush;
       switch(rt) {
	   case IMAGE_DESC_RECORD_TYPE:
		if (DGifGetImageDesc(gft) == GIF_ERROR) grflush;
		px = gft->Image.Width;
		py = gft->Image.Height;
		*x = px;
                *y = py;
		DGifCloseFile(gft);
		return(FH_ERROR_OK);
		break;
	    case EXTENSION_RECORD_TYPE:
		if (DGifGetExtension(gft,&extcode,&extension) == GIF_ERROR) grflush;
		while (extension != NULL)
		    if(DGifGetExtensionNext(gft,&extension) == GIF_ERROR) grflush;
		break;
	    default:
		break;
	}  
    }
    while (rt != TERMINATE_RECORD_TYPE);
    DGifCloseFile(gft);
    return(FH_ERROR_FORMAT);
}

/***********************************************************/
/* GIF FILE IO ROUTINES                                    */
/***********************************************************/
struct formathandler *fh_root = NULL;

void add_format(int (*picsize)(char *, int *, int*),
                int (*picread)(char *, unsigned char *, int, int), 
                int (*id)(char*)) {
struct formathandler *fhn;
    fhn=(struct formathandler*) malloc(sizeof(struct formathandler));
    fhn->get_size = picsize; 
    fhn->get_pic  = picread; 
    fhn->id_pic   = id;
    fhn->next     = fh_root; 
    fh_root       = fhn;
}

extern int fh_gif_getsize(char *, int *, int*);
extern int fh_gif_load(char *, uchar *, int, int);
extern int fh_gif_id(char *);

struct formathandler *fh_getsize(char *name, int *x, int *y) {
    struct formathandler *fh;
    for (fh = fh_root; fh != NULL; fh = fh->next) {
	if (fh->id_pic(name))
	    if(fh->get_size(name,x,y) == FH_ERROR_OK) return(fh);
    }
    return(NULL);
}

void GIF_FILE_READ(char *FileName, uchar data[], int *color) { 
int x, y;
struct formathandler *fh;
    add_format(fh_gif_getsize, fh_gif_load, fh_gif_id);
    if (fh = fh_getsize(FileName, &x, &y)) {
	if (fh->get_pic(FileName, data, x, y) == FH_ERROR_OK) *color = 1;
    } 
}



