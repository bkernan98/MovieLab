#ifndef MOVIE_H
#define MOVIE_H

#include "ImageList.h"

/* the movie structure */
typedef struct {
	ILIST *Frames; /* Pointer to the frame list */
} Movie;

/* Allocate the memory space for the movie and the memory space */
/* for the frame list. Return the pointer to the movie. */
Movie *CreateMovie(void);

/* Release the memory space for the frame list. */
/* Release the memory space for the movie. */
void DeleteMovie(Movie *movie);

/* Convert a YUV movie to a RGB movie */
void YUV2RGBMovie(Movie *movie);

/* Convert a RGB movie to a YUV movie */
void RGB2YUVMovie(Movie *movie);

#endif
