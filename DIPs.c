#include <stdlib.h>
#include <assert.h>
#include "DIPs.h"
#include "Constants.h"
#include "string.h"
#include "math.h"

/* Black and White */
Image *BlackNWhite(Image *image)
{
	assert(image);
	int x, y, tmp;

	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image); x++) {
		tmp = (GetPixelR(image, x, y) + GetPixelG(image, x, y) + GetPixelB(image, x, y)) / 3;
			SetPixelR(image, x, y, tmp);
			SetPixelG(image, x, y, tmp);
		SetPixelB(image, x, y, tmp);
		}
	}
	return image;
}

/* Mirror image horizontal  */
Image *HMirror(Image *image) {
	assert(image);
	int x, y;
	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image)/2; x++) {
			SetPixelG(image, ImageWidth(image) - 1 - x, y, GetPixelG(image, x, y));
			SetPixelB(image, ImageWidth(image) - 1 - x, y, GetPixelB(image, x, y));
			SetPixelR(image, ImageWidth(image) - 1 - x, y, GetPixelR(image, x, y));
		}
	}
	return image;
}

/* Edge detection */
Image *Edge(Image *image) {
	assert(image);
	Image *tmpImage = CreateImage(ImageWidth(image), ImageHeight(image));
	assert(tmpImage);

	int x, y, m, n;
	memcpy(tmpImage->R, image->R, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	memcpy(tmpImage->G, image->G, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	memcpy(tmpImage->B, image->B, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	for (y = 1; y < ImageHeight(image) - 1; y++) {
		for (x = 1; x < ImageWidth(image) - 1; x++) {
			int tmpR = 0;
			int tmpG = 0;
			int tmpB = 0;
			for (m = -1; m <= 1; m++) {
				for (n = -1; n <= 1; n++) {
					tmpR += (GetPixelR(tmpImage, x, y) - GetPixelR(tmpImage, x + n, y + m));
					tmpG += (GetPixelG(tmpImage, x, y) - GetPixelG(tmpImage, x + n, y + m));
					tmpB += (GetPixelB(tmpImage, x, y) - GetPixelB(tmpImage, x + n, y + m));
				}
			}
			SetPixelR(image, x, y, (tmpR > 255) ? 255 : (tmpR < 0) ? 0 : tmpR);
			SetPixelG(image, x, y, (tmpG > 255) ? 255 : (tmpG < 0) ? 0 : tmpG);
			SetPixelB(image, x, y, (tmpB > 255) ? 255 : (tmpB < 0) ? 0 : tmpB);
		}
	}
	for (y = 0; y < ImageHeight(image); y++) {
		x = 0;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
		x = ImageWidth(image) - 1;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
	}
	for (x = 0; x < ImageWidth(image); x++) {
		y = 0;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
		y = ImageHeight(image) - 1;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
	}
	DeleteImage(tmpImage);
	tmpImage = NULL;
	return image;
}

/* Add a watermark to an image */
Image *Watermark(Image *image, const Image *watermark,
	 unsigned int topLeftX, unsigned int topLeftY)
{
	unsigned int x, y;
	assert(image);
	assert(watermark);
	assert(topLeftX + watermark->W <= watermark->W);
	assert(topLeftY + watermark->H <= watermark->H);
	for (y = 0; y < watermark->H; y++) {
		for (x = 0; x < watermark->W; x++) {
			unsigned char R = GetPixelR(watermark, x, y);
			unsigned char G = GetPixelG(watermark, x, y);
			unsigned char B = GetPixelB(watermark, x, y);
			unsigned char imageR = GetPixelR(image, topLeftX + x, topLeftY + y);
			unsigned char imageG = GetPixelG(image, topLeftX + x, topLeftY + y);
			unsigned char imageB = GetPixelB(image, topLeftX + x, topLeftY + y);
			SetPixelR(image, topLeftX + x, topLeftY + y, (imageR + R) / 2);
			SetPixelG(image, topLeftX + x, topLeftY + y, (imageG + G) / 2);
			SetPixelB(image, topLeftX + x, topLeftY + y, (imageB + B) / 2);
		}
	}
	return image;
}

/* Spotlight */
Image *Spotlight(Image *image, int centerX, int centerY, unsigned int radius)
{
	unsigned int x, y;
	assert(image);
	for (y = 0; y < image->H; y++) {
                for (x = 0; x < image->W; x++) {
			int dx = x - centerX;
			int dy = y - centerY;
			double distance = sqrt(dx * dx + dy * dy);
			if (distance > radius) {
				SetPixelR(image, x, y, 0);
				SetPixelG(image, x, y, 0);
				SetPixelB(image, x, y, 0);
			}
		}
	}
	return image;
}

/* Rotate and zoom an image */
Image *Rotate(Image *image, double Angle, double ScaleFactor)
{
	unsigned int x, y;
	assert(image);
	Image *temp = CreateImage(image->W, image->H);
	assert(temp);
	double radians = Angle * 3.14159265358979323846 / 180.0;
	int centerX = image->W / 2;
	int centerY = image->H / 2;
	for (y = 0; y < image->H; y++) {
                for (x = 0; x < image->W; x++) {
			int dx = x - centerX;
			int dy = y - centerY;
			int srcX = (int)(centerX + (dx * cos(radians) - dy * sin(radians)) / ScaleFactor);
			int srcY = (int)(centerY + (dx * sin(radians) - dy * cos(radians)) / ScaleFactor);
			if (srcX >= 0 && srcX < image->W && srcY >= 0 && srcY < image->H) {
				SetPixelR(temp, x, y, GetPixelR(image, srcX, srcY));
				SetPixelG(temp, x, y, GetPixelG(image, srcX, srcY));
				SetPixelB(temp, x, y, GetPixelB(image, srcX, srcY));
			}
			else {
				SetPixelR(temp, x, y, 0);
				SetPixelG(temp, x, y, 0);
				SetPixelB(temp, x, y, 0);
			}
		}
	}
	for (y = 0; y < image->H; y++) {
                for (x = 0; x < image->W; x++) {
			SetPixelR(image, x, y, GetPixelR(temp, x, y));
			SetPixelG(image, x, y, GetPixelG(temp, x, y));
			SetPixelB(image, x, y, GetPixelB(temp, x, y));
		}
	}
	DeleteImage(temp);
	return image;
}
