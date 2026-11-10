/*
* filename: mandelmovie.c
* author: Zeeshan Huq
* date: 11/10/26
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>  
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <errno.h> 

#define FRAMES 50
#define PROCS 1
#define WIDTH 800
#define HEIGHT 800
#define START 4.0
#define END 0.0005  
#define X -0.743643887037151
#define Y 0.131825904205330
#define MAX_ITER 1500
  
static void usage(char *progname) {
    printf("Usage: %s [options]\n", progname);
    printf(" -n <procs> : number of processes to use (default: %d)\n", PROCS);
    printf(" -f <frames> : number of frames to generate (default: %d)\n", FRAMES);
    printf(" -W <width> : width of each frame (default: %d)\n", WIDTH);
    printf(" -H <height> : height of each frame (default: %d)\n", HEIGHT);
    printf(" -s <scale> : starting scale for the Mandelbrot set (default: %f)\n", START);
    printf(" -e <scale> : ending scale for the Mandelbrot set (default: %f)\n", END);
    printf(" -x <x> : x-coordinate of the center of the Mandelbrot set (default: %f)\n", X);
    printf(" -y <y> : y-coordinate of the center of the Mandelbrot set (default: %f)\n", Y);
    printf(" -i <iter> : maximum number of iterations (default: %d)\n", MAX_ITER);
    printf(" -h : display this help message\n");
}

static parse_pos_int(char *arg) {
    char *endptr;
    long val = strtol(arg, &endptr, 10);
    if (*endptr != '\0' || val < 0) {
        fprintf(stderr, "Invalid positive integer: %s\n", arg);
        exit(EXIT_FAILURE);
    }
    return (int)val;
}

static parse_double(char *arg) {
    char *endptr;
    double val = strtod(arg, &endptr);
    if (*endptr != '\0') {
        fprintf(stderr, "Invalid double: %s\n", arg);
        exit(EXIT_FAILURE);
    }
    return val;
}

int main(int argc, char *argv[]) {
  
  
  
  return 0;
}