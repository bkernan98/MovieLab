# Movie Lab
A program that loads a movie made of raw YUV video
frames, lets you apply image processing effects to it through a menu, and
saves the result back out as a new YUV file.
 
It runs as an interactive program. You start it, tell it which movie to load,
and then pick effects from a menu one at a time. Each effect is applied to the
whole movie that's currently in memory, so you can stack a few of them together
before saving and quitting.
 
Under the hood the movie is just a doubly linked list of frames. Each frame is
one image. Things like cropping, reversing, and fast-forwarding are really just
operations on that list.
 
 
## What it can do

- Black and white
- Horizontal mirror
- Edge detection
- Crop down to a range of frames
- Fast forward (keep every Nth frame)
- Reverse the whole thing
- Add a watermark image to every frame
- Spotlight (everything outside a circle goes black)
- Rotate / zoom each frame
- Save the movie to a file
 
 
## How the files fit together

Image.c / Image.h
    The pixel-level layer. Defines the RGB Image and YUV YUVImage structs,
    the get/set pixel functions, and the create/delete/copy helpers.
    Everything else is built on top of this.
 
DIPs.c / DIPs.h
    The actual image filters: BlackNWhite, HMirror, Edge, Watermark,
    Spotlight, and Rotate. These all operate on RGB images.
 
ImageList.c / ImageList.h
    The doubly linked list that holds the frames. Append, prepend, crop,
    fast forward, reverse, and the cleanup code live here.
 
Movie.c / Movie.h
    Wraps the frame list into a Movie and converts the whole movie between
    YUV and RGB. The filters run in RGB, but the file format is YUV, so the
    program converts to RGB, filters, then converts back.
 
FileIO.c / FileIO.h
    Loads and saves single .ppm images. This is used for the watermark image.
 
MovieLab.c
    The main program. Handles the menu, prompts the user, and applies the
    chosen effects. The code that reads and writes the raw YUV frames lives
    here too (LoadMovie / SaveMovie and their one-frame helpers).
 
Constants.h
    A few shared #define values, including SLEN (the max filename length).
 
Makefile
    Build rules.
 
## After running

It will first ask you for:
  - the input movie file name
  - the resolution, typed as WidthxHeight (for example 512x288)
  - how many frames to load
 
Once the movie is loaded, the menu appears:
 
     1. Black and white
     2. Horizontal mirror
     3. Edge detection
     4. Crop frames
     5. Fast forward
     6. Reverse
     7. Watermark
     8. Spotlight
     9. Rotate / zoom
    10. Save movie to file
    11. Exit
 
Type a number and press Enter. Some options ask for a little more info:
  - Crop asks for a start and end frame
  - Fast forward asks for a factor
  - Watermark asks for the .ppm file to overlay
  - Spotlight asks for a radius in pixels
  - Rotate asks for an angle in degrees and a zoom/scale factor
 
You can apply as many effects as you want before saving. When you're done,
choose Save to write the result out, then Exit to quit.
 
 
## A few notes

- The input is raw YUV (4:2:0), so the program has no way of knowing the
  resolution or frame count on its own. You have to enter them correctly at
  the start, or the frames will come out garbled.
 
- The watermark image has to be a P6 .ppm file, and it can't be bigger than
  the movie's frames. If it is, the program tells you and skips it.
 
- Effects are applied in the order you pick them, and they build on each
  other since they all modify the same in-memory movie.
 
- Bad input (letters where a number is expected, an out-of-range crop, a
  zero scale factor, etc.) is caught and just sends you back to the menu
  instead of crashing.
