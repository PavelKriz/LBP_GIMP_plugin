#include "CCLBP.hpp"

void CCLBP::nextState(){
    switch (innerState)
    {
    case MEAN_VALUES:
        innerState = CLBP;
        break;
    case CLBP:
        innerState = FINISHED;
        break;
    default:
        innerState = FINISHED;
        break;
    }
}

void CCLBP::meanValues(unsigned char * inBuf, int outWidth, int outHeight, int radius, int pointsCount){
   
    int width = outWidth + 2 *radius;
    int height = outHeight + 2 *radius;
    int i, j, I, J;
    //iteration over part of image
    for (i = 0, I = radius; i < outHeight; ++i, ++I) {
        for (j = 0, J = radius; j < outWidth; ++j, ++J) {
            guchar center =  inBuf[ I * width  + J];
            //computation of meanGray
            updateMeanGray((double)center, 1);

            char pixelMpMean = 0;
            int pixelMpSum = 0;
            for(int k = 0; k < pointsCount; ++k){
                pixelMpSum += abs(calcValByInterpol(inBuf, pointsLocCoords[k], J, I, width) - center);
            }
            pixelMpMean = (char) (pixelMpSum / pointsCount);

            //computation of meanMp
            updateMeanMp(pixelMpMean, 1);
        }
    }
}

void CCLBP::clbp_smc(unsigned char * inBuf, unsigned char * outBuf, int outWidth, int outHeight, int radius, int pointsCount, int channels){
    int width = outWidth + 2 *radius;
    int height = outHeight + 2 *radius;
    int i, j, I, J; //I and J are coordinates in inBuf and i j in outBuf
    //iteration over part of image
    for (i = 0, I = radius; i < outHeight; ++i, ++I) {
        for (j = 0, J = radius; j < outWidth; ++j, ++J) {
            guchar center =  inBuf[ I * width  + J];
            
            //CLBP_S
            unsigned char pixelCLBP_S = 0;
            for(int k = 0; k < pointsCount; ++k){
                pixelCLBP_S |= (calcValByInterpol(inBuf, pointsLocCoords[k], J, I, width) >= center) << k;
            }

            //CLBP_M
            unsigned char pixelCLBP_M = 0;

            for(int k = 0; k < pointsCount; ++k){
                pixelCLBP_M |= (abs(calcValByInterpol(inBuf, pointsLocCoords[k], J, I, width) - center) >= meanMpValue.first) << k;
            }

            //CLBP_C
            unsigned char pixelCLBP_C = 0;
            for(int k = 0; k < pointsCount; ++k){
                pixelCLBP_C |= ((calcValByInterpol(inBuf, pointsLocCoords[k], J, I, width)) >= meanGrayValue.first) << k;
            }
            
            if(clbpType == ECLBP_SMC){
                outBuf[ i * outWidth * channels + j * channels + 0 /*channel*/] = (unsigned char) pixelCLBP_S;
                outBuf[ i * outWidth * channels + j * channels + 1 /*channel*/] = (unsigned char) pixelCLBP_M;
                outBuf[ i * outWidth * channels + j * channels + 2 /*channel*/] = (unsigned char) pixelCLBP_C;
                if(channels == 4){ outBuf[ i * outWidth * channels + j * channels + 3 /*channels*/] = 255;} //alphaCanal

            } else if(clbpType == ECLBP_S){
                outBuf[ i * outWidth * channels + j * channels + 0 /*channel*/] = (unsigned char) pixelCLBP_S;
                outBuf[ i * outWidth * channels + j * channels + 1 /*channel*/] = (unsigned char) pixelCLBP_S * 0.7;
                outBuf[ i * outWidth * channels + j * channels + 2 /*channel*/] = (unsigned char) pixelCLBP_S * 0.7;
                if(channels == 4){ outBuf[ i * outWidth * channels + j * channels + 3 /*channels*/] = 255;} //alphaCanal

            } else if (clbpType == ECLBP_M) {
                outBuf[ i * outWidth * channels + j * channels + 0 /*channel*/] = (unsigned char) pixelCLBP_M * 0.7;
                outBuf[ i * outWidth * channels + j * channels + 1 /*channel*/] = (unsigned char) pixelCLBP_M;
                outBuf[ i * outWidth * channels + j * channels + 2 /*channel*/] = (unsigned char) pixelCLBP_M * 0.7;
                if(channels == 4){ outBuf[ i * outWidth * channels + j * channels + 3 /*channels*/] = 255;} //alphaCanal

            } else if (clbpType == ECLBP_C) {
                outBuf[ i * outWidth * channels + j * channels + 0 /*channel*/] = (unsigned char) pixelCLBP_C * 0.7;
                outBuf[ i * outWidth * channels + j * channels + 1 /*channel*/] = (unsigned char) pixelCLBP_C * 0.7;
                outBuf[ i * outWidth * channels + j * channels + 2 /*channel*/] = (unsigned char) pixelCLBP_C;
                if(channels == 4){ outBuf[ i * outWidth * channels + j * channels + 3 /*channel*/] = 255;} //alphaCanal
            }
        }
    }
}

void CCLBP::updateMean(std::pair<double, int> & original,  int updateValue, int updatetWeigh){
    int originalWeight = original.second;
    double originalValue = original.first;

    original.first = (double) (originalValue*originalWeight + updateValue*updatetWeigh) / (originalWeight + updatetWeigh);
    original.second = originalWeight + updatetWeigh;
}

void CCLBP::process(unsigned char * inBuf, unsigned char * outBuf, int outWidth, int outHeight, int channels){
    switch (innerState)
    {
    case MEAN_VALUES:
        meanValues(inBuf, outWidth, outHeight, radius, neighbourPointsCount);
        break;
    case CLBP:
        clbp_smc(inBuf, outBuf, outWidth, outHeight, radius, neighbourPointsCount, channels);
        break;
    default:
        return;
    }
}

void CCLBP::out(guchar * outBuffer, int outX0, int outY0, int width, int height){
    switch (innerState)
    {
    case MEAN_VALUES:
        return;
        break;
    case CLBP:
        CAlgorithm::out(outBuffer, outX0, outY0, width, height);
        break;
    default:
        break;
    }
}
