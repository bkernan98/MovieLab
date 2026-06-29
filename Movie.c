#include <stdlib.h>
#include <assert.h>
#include "Movie.h"

/* Allocate the memory space for the movie and the memory space */
/* for the frame list. Return the pointer to the movie. */
Movie *CreateMovie(void)
{
	Movie *movie = malloc(sizeof(Movie));
	assert(movie != NULL);
	movie->Frames = CreateImageList();
	assert(movie->Frames != NULL);
	return movie;
}

/* Release the memory space for the frame list. */
/* Release the memory space for the movie. */
void DeleteMovie(Movie *movie)
{
	assert(movie != NULL);
	DeleteImageList(movie->Frames);
	free(movie);
}

/* Convert a YUV movie to a RGB movie */
void YUV2RGBMovie(Movie *movie)
{
	assert(movie != NULL);
	assert(movie->Frames);
	IENTRY *current = movie->Frames->First;
	unsigned int x, y;
	while (current) {
		YUVImage *yuv = (YUVImage *)current->image;
		Image *rgb = CreateImage(yuv->W, yuv->H);
		for (y = 0; y < yuv->H; y++) {
			for (x = 0; x < yuv->W; x++) {
				int Y = GetPixelY(yuv, x, y);
				int U = GetPixelU(yuv, x, y);
				int V = GetPixelV(yuv, x, y);
				int C = Y - 16;
				int D = U - 128;
				int E = V - 128;
				int R = (298 * C + 409 * E + 128) >> 8;
				int G = (298 * C - 100 * D - 208 * E) >> 8;
				int B = (298 * C + 516 * D + 128) >> 8;
				R = (R < 0) ? 0: (R > 255) ? 255: R;
				G = (G < 0) ? 0: (G > 255) ? 255: G;
				B = (B < 0) ? 0: (B > 255) ? 255: B;
				SetPixelR(rgb, x, y, (unsigned char)R);
				SetPixelG(rgb, x, y, (unsigned char)G);
				SetPixelB(rgb, x, y, (unsigned char)B);
			}
		}
		DeleteYUVImage(yuv);
		current->image = rgb;
		current = current->Next;
	}
}

/* Convert a RGB movie to a YUV movie */
void RGB2YUVMovie(Movie *movie)
{
	assert(movie != NULL);
        assert(movie->Frames);
        IENTRY *current = movie->Frames->First;
        unsigned int x, y;
        while (current) {
                Image *rgb = (Image *)current->image;
		YUVImage *yuv = CreateYUVImage(rgb->W, rgb->H);
                for (y = 0; y < rgb->H; y++) {
                        for (x = 0; x < rgb->W; x++) {
                                int R = GetPixelR(rgb, x, y);
                                int G = GetPixelG(rgb, x, y);
                                int B = GetPixelB(rgb, x, y);
				int Y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
				int U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
				int V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;
				Y = (Y < 0) ? 0: (Y > 255) ? 255: Y;
                                U = (U < 0) ? 0: (U > 255) ? 255: U;
                                V = (V < 0) ? 0: (V > 255) ? 255: V;
                                SetPixelY(yuv, x, y, (unsigned char)Y);
                                SetPixelU(yuv, x, y, (unsigned char)U);
                                SetPixelV(yuv, x, y, (unsigned char)V);
			}
		}
		DeleteImage(rgb);
		current->image = yuv;
		current = current->Next;
	}
}
