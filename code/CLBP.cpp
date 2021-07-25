#include "CLBP.hpp"

void CLBP::lbp(unsigned char * inBuf, unsigned char * outBuf, int outWidth, int outHeight, int radius, int pointsCount, int channels){
    
        int width = outWidth + 2 *radius;
        int height = outHeight + 2 *radius;
        int i, j, I, J;  //I and J are coordinates in inBuf and i j in outBuf
        //iteration over part of image
        for (i = 0, I = radius; i < outHeight; ++i, ++I) {
            for (j = 0, J = radius; j < outWidth; ++j, ++J) {
                unsigned int pixelLBP = 0;
                guchar center =  inBuf[ I * width  + J];
                for(int k = 0; k < pointsCount; ++k){
                    pixelLBP |= (calcValByInterpol(inBuf, pointsLocCoords[k], J, I, width) > center) << k;
                }

                unsigned int move = 0b11111111;
                outBuf[ i * outWidth * channels + j * channels + 0 /*channel*/] = (unsigned char) ((pixelLBP & move) >> (0/*channel*/*8));
                move = move << 8;
                outBuf[ i * outWidth * channels + j * channels + 1 /*channel*/] = (unsigned char) ((pixelLBP & move) >> (1/*channel*/*8));
                move = move << 8;
                outBuf[ i * outWidth * channels + j * channels + 2 /*channel*/] = (unsigned char) ((pixelLBP & move) >> (2/*channel*/*8));
                move = move << 8;
                if(channels == 4){ outBuf[ i * outWidth * channels + j * channels + 3 /*channel*/] = 255;} //alphaCanal
            }
        }   
    }