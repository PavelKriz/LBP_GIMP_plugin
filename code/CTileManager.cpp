#include "CTileManager.hpp"

CTileManager::CTileManager(int imgWidth, int imgHeight, int inX0, int inY0, int tileBorderRadius ){
    this->tileBorderRadius = tileBorderRadius;
    this->imgWidth = imgWidth;
    this->imgHeight = imgHeight;
    this->inX0 = inX0;
    this->inY0 = inY0;

    defaultTileSize = DEFAULT_TILE_SIZE;

    //changing size of TILESIZE so it will never happen that is one side of tile smaller then radius
    while(imgWidth % defaultTileSize < tileBorderRadius || imgHeight % defaultTileSize < tileBorderRadius){
        --defaultTileSize;
    }

    tileCountWidth = ((int) imgWidth / defaultTileSize) + 1;
    tileCountHeight = ((int) imgHeight / defaultTileSize) + 1;
}

void CTileManager::run( SProcessInfo & processInfo, int iteration, int iterationCount ){
    int tileHeight = defaultTileSize;
    int tileWidth = defaultTileSize;

    //looping over tiles of image
    for(int it = 0; it < tileCountHeight; ++it){
        //tiles on bottom may be smaller
        if((imgHeight - it * defaultTileSize) < defaultTileSize){
            tileHeight = (imgHeight - it * defaultTileSize);
        } else {
            tileHeight = defaultTileSize;
        }

        for(int jt = 0; jt < tileCountWidth; ++jt){
            //tiles on right may be smaller
	    	if((imgWidth - jt * defaultTileSize) < defaultTileSize){
	    		tileWidth = (imgWidth - jt * defaultTileSize);
	    	} else {
	    		tileWidth = defaultTileSize;
	    	}

            // initialize and process Ctile in this iteration
            CTile tile = CTile(it,jt, inX0, inY0, tileWidth, tileHeight, tileCountWidth, tileCountHeight, tileBorderRadius, defaultTileSize);
            tile.LoadAndProcessTile( processInfo );

            //update progress bars UI and also commandline
	    	gimp_progress_update ( (gdouble) (((it * tileCountWidth + jt) / (gdouble) (tileCountWidth * tileCountHeight)) / iterationCount
                                + iteration / (gdouble) iterationCount));
            g_print(".");
        }
    }
}
