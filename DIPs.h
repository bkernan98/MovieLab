#ifndef DIPS_H
#define DIPS_H

#include "Image.h"


/* Black and White */
Image *BlackNWhite(Image *image);

/* Mirror image horizontal  */
Image *HMirror(Image *image) ;

/* Edge detection */
Image *Edge(Image *image);

/* Add a watermark to an image */
Image *Watermark(Image *image, const Image *watermark, unsigned int topLeftX, unsigned int topLeftY);

/* Spotlight */
Image *Spotlight(Image *image, int centerX, int centerY, unsigned int radius);

/* Zoom an image */
Image *Rotate(Image *image, double Angle, double ScaleFactor);

#endif
