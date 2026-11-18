/*
* filename: mandelmovie.c
* author: Zeeshan Huq
* date: 11/10/26
* compile: gcc -o mandelmovie mandelmovie.c -lm
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

static int parse_pos_int(const char *s, char opt) {
    char *endptr;
    errno = 0;
    long val = strtol(s, &endptr, 10);
    if (errno !=0 || *endptr != '\0' || val < 0) {
        fprintf(stderr, "Invalid positive integer: %s for -%c\n", s, opt);
        exit(EXIT_FAILURE);
    }
    return (int)val;
}

static double parse_double(const char *s, char opt) {
    char *endptr;
    errno = 0;
    double val = strtod(s, &endptr);
    if (errno != 0 || *endptr != '\0') {
        fprintf(stderr, "Invalid double: %s for -%c\n", s, opt);
        exit(EXIT_FAILURE);
    }
    return val;
}

static pid_t launch_frame(int frame, int width, int height, double scale, double x, double y, int max_iter) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        // Child process
        char sx[64], sy[64], ss[64], sm[32], sw[32], sh[32], out[64];
        snprintf(sx, sizeof sx, "%0.15f", x);
        snprintf(sy, sizeof sy, "%0.15f", y);
        snprintf(ss, sizeof ss, "%0.15g", scale);
        snprintf(sm, sizeof sm, "%d", max_iter);
        snprintf(sw, sizeof sw, "%d", width);
        snprintf(sh, sizeof sh, "%d", height);
        snprintf(out, sizeof out, "mandel%d.jpg", frame);
        char *args[] = {"./mandel", "-x", sx, "-y", sy, "-s", ss, 
            "-m", sm, "-W", sw, "-H", sh, "-o", out, NULL};
        execv(args[0], args);
        perror("execv ./mandel");
        exit(EXIT_FAILURE);
    }
    return pid;
}

static int reap_one(void){
    int status;
    pid_t pid;

    do{
        pid = wait(&status);
    } while(pid < 0 && errno == EINTR);

    if (pid < 0){
        perror("wait");
        return -1;
    }

    if(!WIFEXITED(status) || WEXITSTATUS(status) != 0){
        fprintf(stderr, "mandelmovie: child %d did not finish", (int) pid);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int procs = PROCS;
    int frames = FRAMES;
    int width = WIDTH;
    int height = HEIGHT;
    int iter = MAX_ITER;
    double x = X;
    double y = Y;
    double start = START;
    double end = END;
    int c;

    while((c = getopt(argc, argv, "n:f:W:H:s:e:x:y:i:h")) != -1){
        switch(c) {
            case 'n': 
                procs = parse_pos_int(optarg, 'n'); 
                break;
            case 'f':
                frames = parse_pos_int(optarg, 'f'); 
                break;
            case 'W':
                width = parse_pos_int(optarg, 'W'); 
                break;
            case 'H':
                height = parse_pos_int(optarg, 'H'); 
                break;
            case 's':
                start = parse_double(optarg, 's'); 
                break;
            case 'e':
                end = parse_double(optarg, 'e'); 
                break;
            case 'x':
                x = parse_double(optarg, 'x'); 
                break;
            case 'y':
                y = parse_double(optarg, 'y'); 
                break;
            case 'i':
                iter = parse_pos_int(optarg, 'i'); 
                break;
            case 'h':
                usage(argv[0]);
                return 0;
            default:
                usage(argv[0]);
                return 1;
        }
    }

    if(start <= 0 || end <=0){
        fprintf(stderr, "mandelmovie: scale must be positive");
    }

    if(procs > frames){
        procs = frames;
    }

    int running= 0;
    int failed = 0;

    for(int i = 1; i < frames+1; i++){
        double t = (frames > 1) ? (double)(i-1) / (frames -1) : 0.0;
        double scale = start * pow(end / start , t);

        while(running > procs){
            if(reap_one() ==0){
                running--;
            } else {
                return 1;
            }
        }

        if(launch_frame(i, width, height, scale, x, y, iter) < 0){
            failed++;
        } else {
            running++;
        }
    }
    
    while(running > 0 ){
        if(reap_one() != 0){
            break;
        }
        running--;
    }

    if(failed > 0){
        fprintf(stderr, "mandelmovie: %d frame(s) could not be generated", failed);
        return 1;
    }
    return 0;
}