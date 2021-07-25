#include "CAbstractLBP.hpp"

void CAbstractLBP::pointsLocCoordsCalc(){
    //calculate coordinates of pixels for every point
    for(int i = 0; i < neighbourPointsCount; ++i){
            //compute radian angle on unit circle and later position of first pixel
            float angle = 2*M_PI/neighbourPointsCount * i;
            float x = cos(angle) * radius;
            float y = (- sin(angle)) * radius;
            float xRound = round(cos(angle) * radius);
            float yRound = round((- sin(angle)) * radius);

            //find out how much it was rounded and in which direction so later I can determine which pixel in neighborhood of first pixel will be second pixel
            float distX = x - xRound; //max 0.5
            float distY = y - yRound; //max 0.5
            pointsLocCoords[i].fx = (int) xRound;
            pointsLocCoords[i].fy = (int) yRound;
            
            /** second point is distributed like this:
             * This diagram represents one pixel, every ascii symbol represents square with side length 0.25(pixel length). Left lower corner has coordinates [-0.5,-0.5]
             * Second pixel will be on place depending on coordinates  of [distX, distY]
             * @ - second point will be also center
             * ^ > V < - second point will be upper or right or lower or left neighbor pixel
             * ┌ ┐ └ ┘ - second point will be one of the corner neighbors
             * 
             *  ┌ ^ ^ ┐
             *  < @ @ >
             *  < @ @ >
             *  └ V V ┘
             * 
             */

            //CORNERS
            if(abs(distX) + abs(distY) > 0.75) {
                //right corners
                if(distX > 0){
                    pointsLocCoords[i].sx = (int) xRound + 1;
                //left corners
                } else {
                    pointsLocCoords[i].sx = (int) xRound - 1;
                }
                //upper corners
                if(distY > 0){
                    pointsLocCoords[i].sy = (int) yRound + 1;
                //down corners
                } else {
                    pointsLocCoords[i].sy = (int) yRound - 1;
                }
                //weight calculation
                pointsLocCoords[i].fWeight = 1 - sqrt(pow(abs(distX), 2) + pow(abs(distY),2));
                pointsLocCoords[i].sWeight = 1 - pointsLocCoords[i].fWeight;
            //CENTER
            } else if (abs(distX) + abs(distY) < 0.25) {
                pointsLocCoords[i].sx = (int) xRound;
                pointsLocCoords[i].sy = (int) yRound;
                //weight calculation
                pointsLocCoords[i].fWeight = 0.5;
                pointsLocCoords[i].sWeight = 0.5;
            // UP DOWN LEFT RIGHT SIDES
            } else {
                //x has bigger influence
                if(abs(distX) > abs(distY)){
                    //right side
                    if(distX > 0){
                        pointsLocCoords[i].sx = (int) xRound + 1;
                        pointsLocCoords[i].sy = (int) yRound;
                    //left side
                    } else {
                        pointsLocCoords[i].sx = (int) xRound + 1;
                        pointsLocCoords[i].sy = (int) yRound;
                    }
                    //weight calculation
                    pointsLocCoords[i].fWeight = 1 - abs(distX);
                    pointsLocCoords[i].sWeight = abs(distX);
                //y has bigger influence
                } else {
                    //upper side
                    if(distY > 0){
                        pointsLocCoords[i].sx = (int) xRound;
                        pointsLocCoords[i].sy = (int) yRound + 1;
                    //lower side
                    } else {
                        pointsLocCoords[i].sx = (int) xRound;
                        pointsLocCoords[i].sy = (int) yRound - 1;
                    }
                    //weight calculation
                    pointsLocCoords[i].fWeight = 1 - abs(distY);
                    pointsLocCoords[i].sWeight = abs(distY);
                }
            }
    }
}

void CAbstractLBP::preparation(unsigned char * inBuf, unsigned char * outBuf, unsigned int channels, int width, int height, int & outChannels){
    arrayToGray(inBuf, outBuf, channels, width * height);
    outChannels = channelsAfterPrep();
    pointsLocCoordsCalc();
}

void CAbstractLBP::arrayToGray(unsigned char * inBuf, unsigned char * outBuf, unsigned int channels, int pixelCount){
    if(channels == 1){
        for(int i = 0; i < pixelCount; ++i){
            outBuf[i] = inBuf[i];
        }

    } else if(channels == 2){
        for(int i = 0; i < pixelCount; ++i){
            outBuf[i] = inBuf[i*2];
        }

    } else if (channels == 3){
        for(int i = 0; i < pixelCount; ++i){
            outBuf[i] = (unsigned char) inBuf[ i * channels + 0] * 0.3 + inBuf[ i * channels + 1] * 0.59 + inBuf[ i * channels + 2] * 0.11;
        }
        
    } else if (channels == 4){
        for(int i = 0; i < pixelCount; ++i){
            outBuf[i] = (unsigned char) inBuf[ i * channels + 0] * 0.3 + inBuf[ i * channels + 1] * 0.59 + inBuf[ i * channels + 2] * 0.11;
        }
    }

}