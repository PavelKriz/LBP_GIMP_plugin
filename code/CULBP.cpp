#include "CULBP.hpp"

void CULBP::ulbp(unsigned char * inBuf, unsigned char * outBuf, int outWidth, int outHeight, int radius, int pointsCount, int channels){

    int width = outWidth + 2 *radius;
    int height = outHeight + 2 *radius;
    int i, j, I, J; //I and J are coordinates in inBuf and i j in outBuf
    //iteration over part of image
    for (i = 0, I = radius; i < outHeight; ++i, ++I) {
        for (j = 0, J = radius; j < outWidth; ++j, ++J) {
            //=== LBP part ===
            guchar center =  inBuf[ I * width  + J];
            unsigned int pixelLBP = 0;
            for(int k = 0; k < pointsCount; ++k){
                pixelLBP |= (calcValByInterpol(inBuf, pointsLocCoords[k], J, I, width) > center) << k;
            }
            
            //=== find out if its uniform part ===
            unsigned long shiftOne = 1;
            bool zero = false;
            int counter = 0;
            int shiftTmp = shiftOne;
            shiftOne = shiftOne & pixelLBP;
            if(shiftOne != shiftTmp){
                zero = true;
            }

            bool uniform = true;
            for(int i = 0; i < pointsCount; ++i, shiftOne <<= 1 ){
                shiftTmp = shiftOne;
                shiftOne = shiftOne & pixelLBP;
                if(zero){
                    if(shiftOne == shiftTmp){
                        ++counter;
                        zero = false;
                    }
                } else {
                    if(shiftOne !=shiftTmp){
                        ++counter;
                        zero = true;
                    }
                }
                if(counter > 2){
                    //It isnt uniform
                    uniform = false;
                    break;
                }
                shiftOne = shiftTmp;
            }

            //=== output part ===
            if(!uniform){
                    // blue is value for non uniforms
                    outBuf[ i * outWidth * channels + j * channels + 0] = 0;
                    outBuf[ i * outWidth * channels + j * channels + 1] = 0;
                    outBuf[ i * outWidth * channels + j * channels + 2] = 255;
                    if(channels == 4){ outBuf[ i * outWidth * channels + j * channels + 3 /*channel*/] = 255;} //alphaCanal
            } else {
                //it isuniform
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
}