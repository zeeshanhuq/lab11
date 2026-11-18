<<<<<<< HEAD
# Mandelmovie Labs
=======
# Mandelmovie Labs

Overview  

mandelmovie generates the frames of a Mandelbrot "fly-in" movie by running the provided mandel program once per frame. It does no rendering itself. Instead it schedules child processes:  

The parent reads its options with getopt.  

For each frame it computes a scale, then fork()s a child. The child builds its argument list and calls execv("./mandel", ...), which writes mandel(i).jpg.  

The parent keeps at most -n children running. When that limit is reached it calls wait() for one child to finish before forking the next.  

After the last frame, the parent waits for all remaining children before exiting.  

The scale shrinks by the same ratio every frame (geometric interpolation) from the start scale to the end scale, which gives a smooth zoom. By default the movie zooms into the seahorse valley at x = -0.743643887037151, y = 0.131825904205330.  

Children are fully independent, so no pipes or other communication are needed. Each child also sets alarm() before execv, so a frame that runs too long is killed instead of hanging the parent.  

Build  

make  

Which is equivalent to:
gcc -Wall -o mandel mandel.c jpegrw.c -ljpeg  

gcc -Wall -o mandelmovie mandelmovie.c -lm  


Usage  

./mandelmovie [-n procs] [-f frames] [-x xcenter] [-y ycenter]
              [-s startscale] [-e endscale] [-m maxiter]
              [-W width] [-H height] [-t seconds] [-h]  


Option	Meaning	Default  

-n	simultaneous child processes	1  

-f	number of frames	50  

-x, -y	center of the zoom	seahorse valley  

-s	starting scale (x-axis width)	4  

-e	ending scale	0.0005  

-m	max iterations per pixel	1000  

-W, -H	image size in pixels	600 x 600  

-t	per-frame timeout in seconds (0 = none)	120  

-h	help  

Making the movie  

./mandelmovie -n 5  

ffmpeg -y -i mandel%d.jpg mandel.mpg  


The finished movie is included in this repo as mandel.mpg.  

Results  

Each run generated the same 50 frames with identical settings; only -n changed.  

Processes	Runtime (s)  

1	<65.808>  

2	<49.994>  

5	<25.165>  

10	<14.754>  

20	<18.600>  

Discussion  
There were improvements in runtime from increasing the number of processes up to a certain point. Past 10 processes, the runtime actually started to increase. This shows that an increase in the number of processes does not always result in better performance.
>>>>>>> cf38e9c (updated documentation)
