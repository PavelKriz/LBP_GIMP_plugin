//----------------------------------------------------------------------------------------
/**
 * \file       CTileManager.hpp
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Contains class managing tiles
*/
//----------------------------------------------------------------------------------------


#include <libgimp/gimp.h>
#include "CTile.hpp"
#include "params.h"

//include guard
#ifndef CTILEMANAGER_HPP
#define CTILEMANAGER_HPP

/// CTileManager is class that divides image to tiles and then manages these tiles.
/**
 * Proper use is call at first CTileManager init and then call run
 *
 * "Borders" in documantation meens this:
 * Some algorithms need their neigboorhood (radius) so the tiles must be bigger then the processing area. Then the tile need 
 * not processed borders which contains pixels from neighbour tiles or if its not possible those borders are mirrod from edges of the tile.
*/
class CTileManager{
    int imgWidth;       ///< width of part of image which is going to be processed (Part of image may be whole image)
    int imgHeight;      ///< height of part of image which is going to be processed (Part of image may be whole image)
    int inX0;           ///< start (left upper corner of image) x coordinate of part of image which is going to be processed 
    int inY0;           ///< start (left upper corner of image) y coordinate of part of image which is going to be processed 
    int tileCountWidth;     ///< Info about count of tiles per width in tile grid (for cyclus in run method)
    int tileCountHeight;    ///< Info about count of tiles per height in tile grid (for cyclus in run method)
    int defaultTileSize;    ///< original size(square edge size) of tile without borders and without deforming (in edge cases tiles cannot be squares)
    int tileBorderRadius;   ///< size of borders (radius) of tiles in pixels
public:
    /// Constructor
    /**
     * Initialization of manger. Numbers of tiles per width and height are calculated and default size which will be used of tile is calculated also.
     * 
     * \param[in] imgWidth          width of part of image which is going to be processed (Part of image may be whole image)
     * \param[in] imgHeight         height of part of image which is going to be processed (Part of image may be whole image)
     * \param[in] inX0              start (left upper corner of image) x coordinate of part of image which is going to be processed 
     * \param[in] inY0              start (left upper corner of image) y coordinate of part of image which is going to be processed 
     * \param[in] tileBorderRadius  size of borders (radius) of tiles in pixels
     */
    CTileManager(int imgWidth, int imgHeight, int inX0, int inY0, int tileBorderRadius );
    /// run (itarates over image)
    /**
     * Method run iterates over image and processes (initializes CTiles and calls load and process tile method) the image by currenttly used CAlgorithm implementation.
     * 
     * \param[in] processInfo       contains info about count of channels and pointer to CAlgorithm implemenation.
     * \param[in] iteration         current iteration
     * \param[in] iterationCount    total number of iterations (some algorithms need more then one iteration over image)
     */
    void run( SProcessInfo & processInfo, int iteration, int iterationCount);
};

#endif //CTILEMANAGER_HPP