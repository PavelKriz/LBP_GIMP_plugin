//----------------------------------------------------------------------------------------
/**
 * \file       CTile.hpp
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Contains class that represent and process image tile
*/
//----------------------------------------------------------------------------------------

#include <libgimp/gimp.h>
#include "SInOutRgns.h"
#include "ELBPType.h"
#include "SProcessInfo.h"

//include guard
#ifndef CTILE_HPP
#define CTILE_HPP

/// Class that represent and process image tile
/**
  Helps manage processing tile. Proper use is call at first CTile init and then call LoadAndProcessTile

  "Borders" in documantation meens this:
  Some algorithms need their neigboorhood (radius) so the tile must be bigger then the processing area. Then the tile need 
  not processed borders which contains pixels from neighbour tiles or if its not possible those borders are mirrod from edges of the tile.
*/
class CTile{
/// A macro that checks if pointer x was allocated properly. It is relevant only for CTile
#define CTILE_CHECK_ALOCATION_OF(x)     if(wasAlocationFail(x)){return;}
/// A macro that checks if anything wasnt allocated properly in the past and if so then will take place return. It is relevant only for CTile
#define CTILE_FULL_MEMORY_CHECK()         if(fullMemory){ return;}
    /// Struct that holds info from which side it need mirror(make) borders
    typedef struct{
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
    } STileMirrorInfo;
    STileMirrorInfo tileMirrorInfo;  ///< holds info from which side it need mirror(make) borders
    int tileBorderRadius; ///< size of borders (radius) in pixels
    int tileWidth;  ///< tile width that doesnt inlcude radius
    int tileHeight; ///< tile height that doesnt inlcude radius
    int inX0 = 0;   ///< zeroX (startX) from which will be loaded pixels from image
    int inY0 = 0;   ///< zeroY (startY) from which will be loaded pixels from image
    int outX0 = 0;  ///< zeroX (startX) from which will be set pixels to image
    int outY0 = 0;  ///< zeroY (startY) from which will be set pixels to image
    int imgLoadTileWidth = 0;   ///< width of tile that is loaded from image (not computed with mirroring)
    int imgLoadTileHeight = 0;  ///< height of tile that is loaded from image (not computed with mirroring)
    int defaultTileSize = 0;    ///< original size(square edge size) of tile without borders and without deforming (in edge cases tiles cannot be squares)
    bool fullMemory;    ///< false if all alocations went well, otherwise true

    /// wasAlocationFail
    /**
    Checks if the memory has been correctly allocated. NULL was fail.

    \param[in] pointer    Pointer which is going to be checked
    */
    bool wasAlocationFail(guchar * pointer);   
public:
    /// Constructor
    /**
    Constructs CTile with correct parameters for loading image and later possible mirroring of tile.

    \param[in] it    Y(height) order of this tile in tile grid (for cyclus in CTileManager)
    \param[in] jt    X(width) order of this tile in tile grid (for cyclus in CTileManager)
    \param[in] x1    zeroX (startX) in image
    \param[in] y1    zeroY (startY) in image
    \param[in] tileWidth            tile width that doesnt inlcude radius
    \param[in] tileHeight           tile height that doesnt inlcude radius
    \param[in] tileCountWidth       Info about count of tiles per width in tile grid (for cyclus in CTileManager)
    \param[in] tileCountHeight      Info about count of tiles per height in tile grid (for cyclus in CTileManager)
    \param[in] tileBorderRadius     size of borders (radius) in pixels
    \param[in] defaultTileSize      original size(square edge size) of tile without borders and without deforming (in edge cases tiles cannot be squares)
    */
    CTile(int it, int jt, int x1, int y1, int tileWidth, int tileHeight, int tileCountWidth, int tileCountHeight, int tileBorderRadius, int defaultTileSize);
    int getImgLoadTileWidth() {return imgLoadTileWidth;}
    int getImgLoadTileHeight() {return imgLoadTileHeight;}
    int getTileWidth(){return tileWidth;}
    int getTileHeight(){return tileHeight;}
    int getInX0(){return inX0;}
    int getInY0(){return inY0;}
    int getOutX0(){return outX0;}
    int getOutY0(){return outY0;}
    /// LoadAndProcessTile
    /**
    It uses methods provides by CAlgorithm implementation. By these methods it provide allocation of memory for these methods and order of running of these methods

    \param[in] processInfo      contains info about number of channels per pixel and pointer to CAlgorithm implemenation.
    */
    void LoadAndProcessTile( SProcessInfo & processInfo);
private:
    /// Add mirror of given size(howMuch) around axis(upper edge)
    /**
    \param[in] inBuf        Buffer with pixels that should be partly mirrored (2d array in buffer) - ( should contain 2d array stored in buffer)
    \param[in] outBuf       outPut Buffer where is stored result - the size must be bigger then inBuf by howMuch in Y axis - ( will contain 2d array stored in buffer)
    \param[in] inWidth      Width of 2d array stored inBuf
    \param[in] inHeight     Height of 2d array stored inBuf
    \param[in] howMuch      Is size of mirrored radius - should be <1-n)
    */
    void addMirrorUp(unsigned char * inBuf, unsigned char * outBuf, int inWidth, int inHeight, int howMuch, int channels);
    /// Add mirror of given size(howMuch) around axis(down edge)
    /**
    \param[in] inBuf        Buffer with pixels that should be partly mirrored (2d array in buffer) - ( should contain 2d array stored in buffer)
    \param[in] outBuf       outPut Buffer where is stored result - the size must be bigger then inBuf by howMuch in Y axis - ( will contain 2d array stored in buffer)
    \param[in] inWidth      Width of 2d array stored inBuf
    \param[in] inHeight     Height of 2d array stored inBuf
    \param[in] howMuch      Is size of mirrored radius - should be <1-n)
    */
    void addMirrorDown(unsigned char * inBuf, unsigned char * outBuf, int inWidth, int inHeight, int howMuch, int channels);
    /// Add mirror of given size(howMuch) around axis(left edge)
    /**
    \param[in] inBuf        Buffer with pixels that should be partly mirrored (2d array in buffer) - ( should contain 2d array stored in buffer)
    \param[in] outBuf       outPut Buffer where is stored result - the size must be bigger then inBuf by howMuch in X axis - ( will contain 2d array stored in buffer)
    \param[in] inWidth      Width of 2d array stored inBuf
    \param[in] inHeight     Height of 2d array stored inBuf
    \param[in] howMuch      Is size of mirrored radius - should be <1-n)
    */
    void addMirrorLeft(unsigned char * inBuf, unsigned char * outBuf, int inWidth, int inHeight, int howMuch, int channels);
    /// Add mirror of given size(howMuch) around axis(right edge)
    /**
    \param[in] inBuf        Buffer with pixels that should be partly mirrored (2d array in buffer) - ( should contain 2d array stored in buffer)
    \param[in] outBuf       outPut Buffer where is stored result - the size must be bigger then inBuf by howMuch in X axis - ( will contain 2d array stored in buffer)
    \param[in] inWidth      Width of 2d array stored inBuf
    \param[in] inHeight     Height of 2d array stored inBuf
    \param[in] howMuch      Is size of mirrored radius - should be <1-n)
    */
    void addMirrorRight(unsigned char * inBuf, unsigned char * outBuf, int inWidth, int inHeight, int howMuch, int channels);
    /// MirrorTileManager
    /**
     * Manages any mirroring that is set to be done in tileMirrorInfo 
     * 
    \param[in] buffPointer  contains pixels of tile
    \param[in] channels     count of channels per pixel in buffPointer
    */
    void MirrorTileManager(unsigned char *& buffPointer, int channels);
};

#endif //CTILEMANAGER_HPP