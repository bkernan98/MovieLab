# Variables
CC = gcc
CFLAGS = -std=c11 -Wall -lm
SRCS = MovieLab.c FileIO.c DIPs.c Image.c ImageList.c Movie.c
OBJS = $(SRCS:.c=.o)
TARGET = MovieLab
# Default target
all: $(TARGET)
######################### Generate object files #######################
# Target for MovieLab.o
MovieLab.o: MovieLab.c Movie.h ImageList.h Image.h DIPs.h FileIO.h Constants.h
	$(CC) $(CFLAGS) -c MovieLab.c -o MovieLab.o
# Target for FileIO.o
FileIO.o: FileIO.c FileIO.h Image.h Constants.h
	$(CC) $(CFLAGS) -c FileIO.c -o FileIO.o
# Target for DIPs.o
DIPs.o: DIPs.c DIPs.h Image.h Constants.h
	$(CC) $(CFLAGS) -c DIPs.c -o DIPs.o
# Target for Image.o
Image.o: Image.c Image.h Constants.h
	$(CC) $(CFLAGS) -c Image.c -o Image.o
# Target for ImageList.o
ImageList.o: ImageList.c ImageList.h Image.h
	$(CC) $(CFLAGS) -c ImageList.c -o ImageList.o
# Target for Movie.o
Movie.o: Movie.c Movie.h ImageList.h Image.h
	$(CC) $(CFLAGS) -c Movie.c -o Movie.o
######################### Generate the executable #####################
# Target for MovieLab
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
###############################  others  ##############################
# Target for clean-up
clean:
	rm -f $(OBJS) $(TARGET) out.yuv
