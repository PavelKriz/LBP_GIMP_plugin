#include "CTile.hpp"

bool CTile::wasAlocationFail(guchar * pointer){
    if( pointer == NULL ){
          g_message("Not enough memory for image of this size!\n");
          fullMemory = true;
          return true;
    }
    return false;
}



CTile::CTile(int it, int jt, int x1, int y1, int tileWidth, int tileHeight, int tileCountWidth, int tileCountHeight, int tileBorderRadius, int defaultTileSize){
    fullMemory = false;
    tileMirrorInfo.up = false;
    tileMirrorInfo.left = false;
    tileMirrorInfo.right = false;
    tileMirrorInfo.down = false;
    inX0 = x1 + (jt * defaultTileSize);;
    inY0 = y1 + (it * defaultTileSize);
    this->defaultTileSize = defaultTileSize;
    outX0 = x1 + (jt * defaultTileSize);
    outY0 = y1 + (it * defaultTileSize);
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    imgLoadTileWidth = 0;
    imgLoadTileHeight = 0;
    this->tileBorderRadius = tileBorderRadius;  

    //if tile deonst need any surrroundings
    if(tileBorderRadius == 0){
        return;
    }

    // set parameters depending on which tile is this tile in tile grid in CTile Manager   
    //INNER TILE
    //SIZE OF IMAGE LESS THEN defaultTileSize also
    if(it > 0 && it < (tileCountHeight - 1) && jt > 0 && jt < (tileCountWidth - 1)){
      inX0 = x1 + (jt * defaultTileSize) - tileBorderRadius;
      inY0 = y1 + (it * defaultTileSize) - tileBorderRadius;
      imgLoadTileWidth = tileWidth + 2 * tileBorderRadius;
      imgLoadTileHeight= tileHeight + 2 * tileBorderRadius;
    } else if (it == (tileCountHeight - 1) && it == 0 && jt == (tileCountWidth - 1) && jt == 0) {
        inX0 = x1;
        inY0 = y1;
        imgLoadTileWidth = tileWidth;
        imgLoadTileHeight= tileHeight;                  
        tileMirrorInfo.left = true;
        tileMirrorInfo.up = true;
        tileMirrorInfo.right = true;
        tileMirrorInfo.down = true;
    //LEFT UPPER RIGHT THREE SIDE CORNER TILE
    } else if (jt == 0 && it == 0 && jt == (tileCountWidth - 1)) {
        inX0 = x1;
        inY0 = y1;
        imgLoadTileWidth = tileWidth;
        imgLoadTileHeight= tileHeight + tileBorderRadius;
        tileMirrorInfo.left = true;
        tileMirrorInfo.up = true;
        tileMirrorInfo.right = true;
    //UPPER RIGHT LOWER THREE SIDE CORNER TILE 
    } else if (it == 0 && jt == (tileCountWidth - 1) && it == (tileCountHeight - 1)){
        inX0 = x1 + (jt * defaultTileSize) - tileBorderRadius;
        inY0 = y1;
        imgLoadTileWidth = tileWidth + tileBorderRadius;
        imgLoadTileHeight= tileHeight;
        tileMirrorInfo.up = true;
        tileMirrorInfo.right = true;
        tileMirrorInfo.down = true;
    //RIGHT LOWER LEFT THREE SIDE CORNER TILE
    } else if ( jt == (tileCountWidth - 1) && it == (tileCountHeight - 1) && jt == 0){
        inX0 = x1;
        inY0 = y1 + (it * defaultTileSize) - tileBorderRadius;
        imgLoadTileWidth = tileWidth;
        imgLoadTileHeight= tileHeight + tileBorderRadius;
        tileMirrorInfo.right = true;
        tileMirrorInfo.down = true;
        tileMirrorInfo.left = true;  
    //LOWER LEFT UPPER THREE SIDE CORNER TILE
    } else if ( it == (tileCountHeight - 1) && jt == 0 && it == 0){
        inX0 = x1;
        inY0 = y1;
        imgLoadTileWidth = tileWidth + tileBorderRadius;
        imgLoadTileHeight= tileHeight;
        tileMirrorInfo.down = true;
        tileMirrorInfo.left = true; 
        tileMirrorInfo.up = true;
    //LEFT UPPER CORNER TILE
    } else if (it == 0 && jt == 0) {
        inX0 = x1;
        inY0 = y1;
        imgLoadTileWidth = tileWidth + tileBorderRadius;
        imgLoadTileHeight = tileHeight + tileBorderRadius;
        tileMirrorInfo.up = true;
        tileMirrorInfo.left = true;
    //RIGHT UPPER CORNER TILE
    } else if (it == 0 && jt == (tileCountWidth - 1)){
        inX0 = x1 + (jt * defaultTileSize) - tileBorderRadius;
        inY0 = y1;
        imgLoadTileWidth = tileWidth + tileBorderRadius;
        imgLoadTileHeight= tileHeight + tileBorderRadius;
        tileMirrorInfo.up = true;
        tileMirrorInfo.right = true;
    //LEFT LOWER CORNER TILE
    } else if ( it == (tileCountHeight - 1) && jt == 0){
        inX0 = x1;
        inY0 = y1 + (it * defaultTileSize) - tileBorderRadius;
        imgLoadTileWidth = tileWidth + tileBorderRadius;
        imgLoadTileHeight= tileHeight + tileBorderRadius;
        tileMirrorInfo.left = true;
        tileMirrorInfo.down = true;          
    //RIGHT LOWER CORNER TILE
    } else if ( it == (tileCountHeight - 1) && jt == (tileCountWidth - 1)){
        inX0 = x1 + (jt * defaultTileSize) - tileBorderRadius;
        inY0 = y1 + (it * defaultTileSize) - tileBorderRadius;
        imgLoadTileWidth = tileWidth + tileBorderRadius;
        imgLoadTileHeight= tileHeight + tileBorderRadius;
        tileMirrorInfo.right = true;
        tileMirrorInfo.down = true;
    //UPPER and LOWER BORDERS TILE  
    } else if (it == 0 && it == (tileCountHeight - 1)){
        inX0 = x1 + (jt * defaultTileSize) - tileBorderRadius;
        inY0 = y1;
        imgLoadTileWidth = tileWidth + 2 * tileBorderRadius;
        imgLoadTileHeight = tileHeight;
        tileMirrorInfo.up = true;
        tileMirrorInfo.down = true;

    //LEFT RIGHT  BORDERS TILE 
    } else if (jt == 0 && jt == (tileCountWidth - 1)){
        inX0 = x1;
        inY0 = y1 + (it * defaultTileSize) - tileBorderRadius;
        imgLoadTileWidth = tileWidth;
        imgLoadTileHeight = tileHeight + 2 * tileBorderRadius;
        tileMirrorInfo.left = true;
        tileMirrorInfo.right = true;
    //UPPER SIDE BORDERS ONLY
    } else if (it == 0){
        inX0 = x1 + (jt * defaultTileSize) - tileBorderRadius;
        inY0 = y1;
        imgLoadTileWidth = tileWidth + 2 * tileBorderRadius;
        imgLoadTileHeight= tileHeight + tileBorderRadius; 
        tileMirrorInfo.up = true;         
    //LOWER SIDE BORDERS ONLY
    } else if (it == (tileCountHeight - 1)) {
        inX0 = x1 + (jt * defaultTileSize) - tileBorderRadius;
        inY0 = y1 + (it * defaultTileSize) - tileBorderRadius;
        imgLoadTileWidth = tileWidth + 2 * tileBorderRadius;
        imgLoadTileHeight= tileHeight + tileBorderRadius;
        tileMirrorInfo.down = true;          
    //LEFT SIDE BORDERS ONLY
    } else if (jt == 0){
        inX0 = x1;
        inY0 = y1 + (it * defaultTileSize) - tileBorderRadius;
        imgLoadTileWidth = tileWidth + tileBorderRadius;
        imgLoadTileHeight= tileHeight + 2 * tileBorderRadius;
        tileMirrorInfo.left = true;          
    //RIGHT SIDE BORDERS ONLY
    } else if(jt == (tileCountWidth - 1)){
        inX0 = x1 + (jt * defaultTileSize) - tileBorderRadius;
        inY0 = y1 + (it * defaultTileSize) - tileBorderRadius;
        imgLoadTileWidth = tileWidth + tileBorderRadius;
        imgLoadTileHeight= tileHeight + 2 * tileBorderRadius;
        tileMirrorInfo.right = true;          
    }
}



void CTile::addMirrorUp(unsigned char * inBuf, unsigned char * outBuf, int inWidth, int inHeight, int howMuch, int channels){
    int outHeight = inHeight + howMuch;
    for(int i = 0; i < outHeight; ++i){
        for(int j = 0; j < inWidth; ++j){
            if(i < howMuch){
                for(int c = 0; c < channels; ++c){
                    outBuf[i * inWidth * channels + j + c] = inBuf[(howMuch - 1 - i) * inWidth * channels + j + c];
                }
            } else {
                for(int c = 0; c < channels; ++c){
                    outBuf[i * inWidth * channels + j + c] = inBuf[(i-howMuch) * inWidth * channels + j + c];
                }
            }
        }
    }
}

void CTile::addMirrorDown(unsigned char * inBuf, unsigned char * outBuf, int inWidth, int inHeight, int howMuch, int channels){
    int outHeight = inHeight + howMuch;
    for(int i = 0; i < outHeight; ++i){
        for(int j = 0; j < inWidth; ++j){
            if(i > (inHeight - 1)){
                for(int c = 0; c < channels; ++c){
                    outBuf[i * inWidth * channels + j + c] = inBuf[(inHeight - 1 - (i - inHeight)) * inWidth * channels + j + c];
                }
            } else {
                for(int c = 0; c < channels; ++c){
                    outBuf[i * inWidth * channels + j + c] = inBuf[ i * inWidth * channels + j + c];
                }
            }
        }
    }
}

void CTile::addMirrorLeft(unsigned char * inBuf, unsigned char * outBuf, int inWidth, int inHeight, int howMuch, int channels){
    int outWidth = inWidth + howMuch;
    for(int i = 0; i < inHeight; ++i){
        for(int j = 0; j < outWidth; ++j){
            if(j < howMuch){
                for(int c = 0; c < channels; ++c){
                    outBuf[i * outWidth * channels + j + c] = inBuf[ i * inWidth * channels  + (howMuch - 1 - j) + c];
                }
            } else {
                for(int c = 0; c < channels; ++c){
                    outBuf[i * outWidth * channels + j + c] = inBuf[ i * inWidth * channels + (j - howMuch) + c];
                }
            }
        }
    }
}

void CTile::addMirrorRight(unsigned char * inBuf, unsigned char * outBuf, int inWidth, int inHeight, int howMuch, int channels){
    int outWidth = inWidth + howMuch;
    for(int i = 0; i < inHeight; ++i){
        for(int j = 0; j < outWidth; ++j){
            if(j > (inWidth - 1)){
                for(int c = 0; c < channels; ++c){
                    outBuf[i * outWidth * channels + j + c] = inBuf[ i * inWidth * channels + inWidth - 1 + (inWidth - j) + c];
                }
            } else {
                for(int c = 0; c < channels; ++c){
                    outBuf[i * outWidth * channels + j + c] = inBuf[ i * inWidth * channels + j + c];
                }
            }
        }
    }
}

void CTile::MirrorTileManager(unsigned char *& buffPointer, int channels){
    int nowY = imgLoadTileHeight;
    int nowX = imgLoadTileWidth;

    if(tileMirrorInfo.up){
      guchar * addUp = g_try_new (guchar, nowX * (nowY + tileBorderRadius) * channels);
      CTILE_CHECK_ALOCATION_OF(addUp);   
	  addMirrorUp(buffPointer, addUp, nowX, nowY, tileBorderRadius, channels);
      g_free(buffPointer);
      buffPointer = addUp;
      nowY += tileBorderRadius;
    }
    if(tileMirrorInfo.down){
      guchar * addDown = g_try_new (guchar, nowX * (nowY + tileBorderRadius) * channels);
      CTILE_CHECK_ALOCATION_OF(addDown);   
	  addMirrorDown(buffPointer, addDown, nowX, nowY, tileBorderRadius, channels);
      g_free(buffPointer);
      buffPointer = addDown;
      nowY += tileBorderRadius;
    }
    if(tileMirrorInfo.left){
      guchar * addLeft = g_try_new (guchar, (nowX + tileBorderRadius) * nowY * channels);
      CTILE_CHECK_ALOCATION_OF(addLeft); 
	  addMirrorLeft(buffPointer, addLeft, nowX, nowY, tileBorderRadius, channels);
      g_free(buffPointer);
      buffPointer = addLeft;
      nowX += tileBorderRadius;
    }
    if(tileMirrorInfo.right){
      guchar * addRight = g_try_new (guchar, (nowX + tileBorderRadius) * nowY * channels);
      CTILE_CHECK_ALOCATION_OF(addRight);  
	  addMirrorRight(buffPointer, addRight, nowX, nowY, tileBorderRadius, channels);
      g_free(buffPointer);
      buffPointer = addRight;
      nowX += tileBorderRadius;
    }
}

void CTile::LoadAndProcessTile(SProcessInfo & processInfo){
    CTILE_FULL_MEMORY_CHECK()

    //initialization of input buffer and for grayscale buffer
	guchar * intile = g_try_new (guchar, imgLoadTileWidth * imgLoadTileHeight * processInfo.channels);
    CTILE_CHECK_ALOCATION_OF(intile);
	guchar * prepTile = g_try_new (guchar, imgLoadTileWidth * imgLoadTileHeight * processInfo.algorithm->channelsAfterPrep()); 
    CTILE_CHECK_ALOCATION_OF(prepTile);

	//load tile and do preparation
    processInfo.algorithm->in(intile, inX0, inY0, imgLoadTileWidth, imgLoadTileHeight);
    int afterPrepChannels = 0;
    processInfo.algorithm->preparation(intile, prepTile, processInfo.channels, imgLoadTileWidth, imgLoadTileHeight, afterPrepChannels);
	g_free (intile); 
    guchar * buffPointer = prepTile;

    MirrorTileManager(buffPointer, afterPrepChannels);
    CTILE_FULL_MEMORY_CHECK()

    //calculating and setting output
    guchar * outtile = g_try_new (guchar, tileWidth * tileHeight * processInfo.channels);
    CTILE_CHECK_ALOCATION_OF(outtile);

    processInfo.algorithm->process(buffPointer, outtile,  (int) tileWidth, (int) tileHeight, processInfo.channels); 
    
    processInfo.algorithm->out(outtile, outX0, outY0, tileWidth, tileHeight);
    //free last alloceted buffers
    g_free(buffPointer);
    g_free (outtile);
}
