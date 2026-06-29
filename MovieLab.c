#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "FileIO.h"
#include "DIPs.h"
#include "Movie.h"
#include "Constants.h"
#include "Image.h"

/* Load one movie frame from the input file */
YUVImage* LoadOneFrame(const char* fname, int n,
	unsigned int width, unsigned height);

/* Load the movie frames from the input file */
Movie *LoadMovie(const char *fname, int frameNum,
	unsigned int width, unsigned height);

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVImage *image, const char *fname, FILE *file);

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, Movie *movie);

/* Print the interactive menu */
void PrintMenu(void);

/* Apply a simple RGB filter (one that takes only an Image*) to every frame */
void ApplyRGBFilter(Movie *movie, Image *(*filter)(Image *));

/* Throw away the rest of the current input line (after a bad scanf) */
void ClearLine(void);

int main(void)
{
	char         inputFile[SLEN];
	char         outputFile[SLEN];
	char         watermarkFile[SLEN];
	unsigned int width = 512;
	unsigned int height = 288;
	int          frameNum = 60;
	int          choice;
	int          running = 1;

	printf("Welcome to MovieLab!\n");

	/* Ask for the movie to load up front, since every effect needs one */
	printf("Enter the input movie file name: ");
	if (scanf("%79s", inputFile) != 1) {
		printf("No input file given. Exiting.\n");
		return 1;
	}

	printf("Enter the resolution as WidthxHeight (e.g. 512x288): ");
	if (scanf("%ux%u", &width, &height) != 2 || width == 0 || height == 0) {
		printf("Invalid resolution. Exiting.\n");
		return 1;
	}

	printf("Enter the number of frames to load: ");
	if (scanf("%d", &frameNum) != 1 || frameNum <= 0) {
		printf("Invalid frame count. Exiting.\n");
		return 1;
	}

	Movie *movie = LoadMovie(inputFile, frameNum, width, height);
	assert(movie);
	printf("\nLoaded %d frame(s) from \"%s\" at %ux%u.\n",
		frameNum, inputFile, width, height);

	/* Main menu loop: keep applying effects until the user exits */
	while (running) {
		PrintMenu();
		printf("Choose an option: ");

		if (scanf("%d", &choice) != 1) {
			ClearLine();
			printf("\nPlease enter a number.\n");
			continue;
		}

		switch (choice) {

		case 1:  /* Black and white */
			ApplyRGBFilter(movie, BlackNWhite);
			printf("\nApplied black and white to all frames.\n");
			break;

		case 2:  /* Horizontal mirror */
			ApplyRGBFilter(movie, HMirror);
			printf("\nApplied horizontal mirror to all frames.\n");
			break;

		case 3:  /* Edge detection */
			ApplyRGBFilter(movie, Edge);
			printf("\nApplied edge detection to all frames.\n");
			break;

		case 4: { /* Crop */
			int start, end;
			printf("Enter the start frame: ");
			if (scanf("%d", &start) != 1) { ClearLine(); printf("\nInvalid input.\n"); break; }
			printf("Enter the end frame: ");
			if (scanf("%d", &end) != 1) { ClearLine(); printf("\nInvalid input.\n"); break; }

			if (start < 0 || end < start || (unsigned int)end >= movie->Frames->Length) {
				printf("\nInvalid range. The movie has %u frame(s) (0 to %u).\n",
					movie->Frames->Length, movie->Frames->Length - 1);
				break;
			}
			CropImageList(movie->Frames, start, end);
			printf("\nCropped to frames %d through %d (%u frame(s) left).\n",
				start, end, movie->Frames->Length);
			break;
		}

		case 5: { /* Fast forward */
			int factor;
			printf("Enter the fast forward factor (1 or more): ");
			if (scanf("%d", &factor) != 1) { ClearLine(); printf("\nInvalid input.\n"); break; }
			if (factor < 1) {
				printf("\nFactor must be at least 1.\n");
				break;
			}
			FastImageList(movie->Frames, factor);
			printf("\nFast forwarded by %d (%u frame(s) left).\n",
				factor, movie->Frames->Length);
			break;
		}

		case 6:  /* Reverse */
			ReverseImageList(movie->Frames);
			printf("\nReversed the frame order.\n");
			break;

		case 7: { /* Watermark */
			printf("Enter the watermark image file (.ppm): ");
			if (scanf("%79s", watermarkFile) != 1) { ClearLine(); printf("\nInvalid input.\n"); break; }

			Image *watermark = LoadImage(watermarkFile);
			if (watermark == NULL) {
				printf("\nFailed to load watermark image \"%s\".\n", watermarkFile);
				break;
			}
			if (watermark->W > width || watermark->H > height) {
				printf("\nWatermark (%ux%u) is larger than the frames (%ux%u). Skipping.\n",
					watermark->W, watermark->H, width, height);
				DeleteImage(watermark);
				break;
			}

			YUV2RGBMovie(movie);
			IENTRY *current = movie->Frames->First;
			while (current != NULL) {
				Watermark((Image *)current->image, watermark, 0, 0);
				current = current->Next;
			}
			RGB2YUVMovie(movie);
			DeleteImage(watermark);
			printf("\nApplied watermark to all frames.\n");
			break;
		}

		case 8: { /* Spotlight */
			int radius;
			printf("Enter the spotlight radius (in pixels): ");
			if (scanf("%d", &radius) != 1) { ClearLine(); printf("\nInvalid input.\n"); break; }
			if (radius <= 0) {
				printf("\nRadius must be positive.\n");
				break;
			}

			YUV2RGBMovie(movie);
			IENTRY *current = movie->Frames->First;
			while (current != NULL) {
				Spotlight((Image *)current->image, width / 2, height / 2, radius);
				current = current->Next;
			}
			RGB2YUVMovie(movie);
			printf("\nApplied spotlight to all frames.\n");
			break;
		}

		case 9: { /* Rotate / zoom */
			double angle, scale;
			printf("Enter the rotation angle in degrees: ");
			if (scanf("%lf", &angle) != 1) { ClearLine(); printf("\nInvalid input.\n"); break; }
			printf("Enter the zoom/scale factor (e.g. 1.0 for none): ");
			if (scanf("%lf", &scale) != 1) { ClearLine(); printf("\nInvalid input.\n"); break; }
			if (scale == 0.0) {
				printf("\nScale factor cannot be 0.\n");
				break;
			}

			YUV2RGBMovie(movie);
			IENTRY *current = movie->Frames->First;
			while (current != NULL) {
				Rotate((Image *)current->image, angle, scale);
				current = current->Next;
			}
			RGB2YUVMovie(movie);
			printf("\nApplied rotate/zoom to all frames.\n");
			break;
		}

		case 10: /* Save */
			printf("Enter the output movie file name: ");
			if (scanf("%79s", outputFile) != 1) { ClearLine(); printf("\nInvalid input.\n"); break; }
			if (SaveMovie(outputFile, movie) != 0) {
				printf("\nFailed to save the movie.\n");
			}
			break;

		case 11: /* Exit */
			running = 0;
			break;

		default:
			printf("\nInvalid option. Please pick 1 through 11.\n");
			break;
		}
	}

	DeleteMovie(movie);
	printf("\nGoodbye!\n");
	return 0;
}

void PrintUsage()
{
	printf("\nUsage: MovieLab -i <file> -o <file> -f <framenum> [options]\n"
	       "Options:\n"
	       "-s <WidthxHeight>     To set the resolution of the input stream (WidthxHeight)\n"
	       "-bw                   Activate the black and white filter on every movie frame\n"
	       "-hmirror              Activate horizontal mirror on every movie frame\n"
	       "-edge                 Activate the edge filter on every movie frame\n"
	       "-crop <start-end>     Crop the movie frames from <start> to <end>\n"
	       "-fast <factor>        Fast forward the movie by <factor>\n"
	       "-reverse              Reverse the frame order of the input movie\n"
	       "-watermark <file>     Add a watermark from <file> to every movie frame\n"
	       "-spotlight            Spotlight the fade in and fade out\n"
	       "-rotate               Rotate and zoom in/out the input movie\n"
	       "-h                    Display this usage information\n"
	      );
}

/* Load one movie frame from the input file */
YUVImage* LoadOneFrame(const char* fname, int n,
	unsigned int width, unsigned height)
{
	FILE *file;
	unsigned int x, y;
	unsigned char c;
	YUVImage* YUVimage;

	/* Check errors */
	assert(fname);
	assert(n >= 0);

	YUVimage = CreateYUVImage(width, height);
	if (YUVimage == NULL) {
		return NULL;
	}

	/* Open the input file */
	file = fopen(fname, "r");
	if (file == NULL) {
		DeleteYUVImage(YUVimage);
		return NULL;
	}

	/* Find the desired frame */
	fseek(file, 1.5 * n * width * height, SEEK_SET);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			c = fgetc(file);
			SetPixelY(YUVimage, x, y, c);
		} /*rof*/
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelU(YUVimage, x, y, c);
			SetPixelU(YUVimage, x + 1, y, c);
			SetPixelU(YUVimage, x, y + 1, c);
			SetPixelU(YUVimage, x + 1, y + 1, c);
		}
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelV(YUVimage, x, y, c);
			SetPixelV(YUVimage, x + 1, y, c);
			SetPixelV(YUVimage, x, y + 1, c);
			SetPixelV(YUVimage, x + 1, y + 1, c);
		}
	}

	/* Check errors */
	assert(ferror(file) == 0);

	/* Close the input file and return */
	fclose(file);
	file = NULL;
	return YUVimage;
}

/* Load the movie frames from the input file */
Movie *LoadMovie(const char *fname, int frameNum,
	unsigned int width, unsigned height)
{
	assert(fname != NULL);
	assert(frameNum>0);
	assert(width>0);
	assert(height>0);
	Movie *movie = CreateMovie();
	assert(movie != NULL);
	for (int i = 0; i < frameNum; i++) {
		YUVImage *YUVimage = LoadOneFrame(fname, i, width, height);
		assert(YUVimage != NULL);	
		AppendYUVImage(movie->Frames, YUVimage);
	}
	return movie;
}

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, Movie *movie)
{
	int count;
	FILE *file;
	IENTRY *curr;

	/* Open the output file */
	file = fopen(fname, "w");
	if (file == NULL) {
		return 1;
	}

	count = 0;
	curr = movie->Frames->First;
	while (curr != NULL) {

		SaveOneFrame(curr->image, fname, file);
		curr = curr->Next;
		count++;
	}

	fclose(file);
	file = NULL;

	printf("The movie file %s has been written successfully!\n", fname);
	printf("%d frames are written to the file %s in total.\n", count, fname);
	return 0;
}

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVImage *image, const char *fname, FILE *file)
{
	int x, y;
	for (y = 0; y < image->H; y++) {
		for (x = 0; x < image->W; x++) {
			fputc(GetPixelY(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelU(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelV(image, x, y), file);
		}
	}
}

