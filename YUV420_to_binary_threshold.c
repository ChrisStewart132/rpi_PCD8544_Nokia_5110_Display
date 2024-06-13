/**
 * Author: Christopher Stewart (Christopher.ray.stewart@gmail.com)
 * Date: 13062024
 * Description: program to convert an incoming stream of 128x160 yuv420 images to a 84x48 binary threshold bitmap
 * 
 * gcc -o YUV420_to_binary_threshold YUV420_to_binary_threshold.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define WIDTH 84
#define HEIGHT 48
#define RPICAM_WIDTH 128 // had to use this aspect ratio to get working yuv420 output
#define RPICAM_HEIGHT 160 

#define THRESHOLD 135 // pixels brighter than threshold will render white else black

int main(int argc, char** argv) {
    uint8_t y[RPICAM_HEIGHT][RPICAM_WIDTH];
    uint8_t u[RPICAM_HEIGHT/2][RPICAM_WIDTH/2];
    uint8_t v[RPICAM_HEIGHT/2][RPICAM_WIDTH/2];
    uint8_t bitmap[HEIGHT][WIDTH];
    int bytes_read;
    while(1){
        // read from stdin a YUV420 frame
        for(int i = 0; i < RPICAM_HEIGHT; i++){
            bytes_read = read(0, y[i], sizeof(uint8_t)*RPICAM_WIDTH);
        }
        if(bytes_read <= 0)
            break;

        for(int i = 0; i < RPICAM_HEIGHT/2; i++){
            read(0, u[i], sizeof(uint8_t)*RPICAM_WIDTH/2);
        }
        for(int i = 0; i < RPICAM_HEIGHT/2; i++){
            read(0, v[i], sizeof(uint8_t)*RPICAM_WIDTH/2);
        }
        
        // sub sample to the centre of the captured stream
        const int offsetx = (RPICAM_WIDTH-WIDTH)/2;
        const int offsety = (RPICAM_HEIGHT-HEIGHT)/2;
        for(int i = offsety; i < RPICAM_HEIGHT-offsety; i++){
            for(int j = offsetx; j < RPICAM_WIDTH-offsetx; j++){
                bitmap[i-offsety][j-offsetx] = y[i][j] > THRESHOLD ? 1 : 0;                
            }
        }

        for(int i = 0; i < HEIGHT; i++){
            write(1, bitmap[i], sizeof(uint8_t)*WIDTH);
        }
    }
	return EXIT_SUCCESS;
}