#include "CMLBP.hpp"

void CMLBP::mlbp(unsigned char * inBuf, unsigned char * outBuf, int outWidth, int outHeight, int radius, int pointsCount, int channels){

        int width = outWidth + 2 *radius;
        int height = outHeight + 2 *radius;
        int i, j, I, J; //I and J are coordinates in inBuf and i j in outBuf
        //iteration over part of image
        for (i = 0, I = radius; i < outHeight; ++i, ++I) {
            for (j = 0, J = radius; j < outWidth; ++j, ++J) {
                unsigned int pixelLBP = 0;

                // calculating avarage value of neighborhood
                float centerAvarage = 0;
                for(int iCenter = -1; iCenter < 2; ++iCenter){
                    for(int jCenter = -1; jCenter < 2; ++jCenter){
                        centerAvarage +=  inBuf[ (I + (iCenter)) * width  + (J + (jCenter))];
                    }
                }
                centerAvarage = centerAvarage / 9;
                guchar center = (guchar) centerAvarage;

                // calculating mLBP
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