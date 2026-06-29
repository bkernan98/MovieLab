#ifndef IMAGE_H
#define IMAGE_H

typedef struct {
	unsigned int W;       /* Image width */
	unsigned int H;       /* Image height */
	unsigned char *R;     /* Pointer to the memory storing */
	                      /* all the R intensity values */
	unsigned char *G;     /* Pointer to the memory storing */
	                      /* all the G intensity values */
	unsigned char *B;     /* Pointer to the memory storing */
	                      /* all the B intensity values */
} Image;

/* Get the intensity value of the Red channel of pixel (x, y) */
/* in the RGB image */
unsigned char GetPixelR(const Image *image, unsigned int x, unsigned int y);

/* Get the intensity value of the Green channel of pixel (x, y) */
/* in the RGB image */
unsigned char GetPixelG(const Image *image, unsigned int x, unsigned int y);

/* Get the intensity value of the Blue channel of pixel (x, y) */
/* in the RGB image */
unsigned char GetPixelB(const Image *image, unsigned int x, unsigned int y);

/* Set the intensity value of the Red channel of pixel (x, y) */
/* in the RGB image with valueR */
void SetPixelR(Image *image, unsigned int x, unsigned int y,
	unsigned char valueR);

/* Set the intensity value of the Green channel of pixel (x, y) */
/* in the RGB image with valueG */
void SetPixelG(Image *image, unsigned int x, unsigned int y,
	unsigned char valueG);

/* Set the intensity value of the Blue channel of pixel (x, y) */
/* in the RGB image with valueB */
void SetPixelB(Image *image, unsigned int x, unsigned int y,
	unsigned char valueB);

/* Allocate the memory space for the RGB image and the memory spaces */
/* for the RGB intensity values. Return the pointer to the RGB image. */
Image *CreateImage(unsigned int width, unsigned int height);

/* Release the memory spaces for the RGB intensity values. */
/* Release the memory space for the RGB image. */
void DeleteImage(Image *image);

unsigned int ImageWidth(const Image *image);
unsigned int ImageHeight(const Image *image);

typedef struct {
	unsigned int W;       /* Image width */
	unsigned int H;       /* Image height */
	unsigned char *Y;     /* Pointer to the memory storing */
	                      /* all the Y intensity values */
	unsigned char *U;     /* Pointer to the memory storing */
	                      /* all the U intensity values */
	unsigned char *V;     /* Pointer to the memory storing */
	                      /* all the V intensity values */
} YUVImage;

/* Get the intensity value of the Y channel of pixel (x, y) */
/* in the YUV image */
unsigned char GetPixelY(const YUVImage *YUVimage, unsigned int x, unsigned int y);

/* Get the intensity value of the U channel of pixel (x, y) */
/* in the YUV image */
unsigned char GetPixelU(const YUVImage *YUVimage, unsigned int x, unsigned int y);

/* Get the intensity value of the V channel of pixel (x, y) */
/* in the YUV image */
unsigned char GetPixelV(const YUVImage *YUVimage, unsigned int x, unsigned int y);

/* Set the intensity value of the Y channel of pixel (x, y) */
/* in the YUV image with valueY */
void SetPixelY(YUVImage *YUVimage, unsigned int x, unsigned int y,
	unsigned char valueY);

/* Set the intensity value of the U channel of pixel (x, y) */
/* in the YUV image with valueU */
void SetPixelU(YUVImage *YUVimage, unsigned int x, unsigned int y,
	unsigned char valueU);

/* Set the intensity value of the V channel of pixel (x, y) */
/* in the YUV image with valueV */
void SetPixelV(YUVImage *YUVimage, unsigned int x, unsigned int y,
	unsigned char valueV);

/* Allocate the memory space for the YUV image and the memory spaces */
/* for the YUV intensity values. Return the pointer to the YUV image. */
YUVImage *CreateYUVImage(unsigned int width, unsigned int height);

/* Release the memory spaces for the YUV intensity values. */
/* Release the memory space for the YUV image. */
void DeleteYUVImage(YUVImage *YUVimage);

/* Copy RGB Image */
Image *CopyRGBImage(Image *SrcImage);

#endif
