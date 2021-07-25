//----------------------------------------------------------------------------------------
/**
 * \file       CAlgorithm.hpp
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Contains Interface( abstract class) for algorithms
*/
//----------------------------------------------------------------------------------------


// include guard
#ifndef CALGORITHM_HPP_INCLUDED
#define CALGORITHM_HPP_INCLUDED

#include <libgimp/gimp.h>
#include "SInOutRgns.h"

/// Interface(abstract class) for algorithms
/**
 * This abstract class provides an interface for algorithms used in plugin .
 * Methods of this class are called in plugin classes methods: CTile::LoadAndProcessTile and CProcessHandler::process
 * Its easy to add another algorithms using this interface. But it may be necessary to change UI and functions handling UI.
 * 
 * Its not recommended to allocate or dealocate memory inside implementation of this interface, if not necessary ofcourse, becouse methods that are calling methods of this interface
 * are checking memory allocation for you. 
 * INBUFFERS AND OUTBUFFERS ARE ALREADY ALLOCATED IN RIGHT SIZE
 */
class CAlgorithm{
protected:
    SInOutRgns inOut;   ///< struct holding pointers to in and out regions (gimp_pixel_rgn) 
public:
    /// Constructor, just initializes input output struct
    /**
     * \param[in] toInOut   struct holding pointers to in and out regions (gimp_pixel_rgn), there is needed for pixels input and output 
     */
    CAlgorithm(SInOutRgns toInOut): inOut(toInOut){}
    /// preparation() is method that prepares image data for later processing
    /**
     * PURE VIRTUAL, must be implemented
     * Advantage of using this method instead of using only process() method is that preparation algorithm can change number of channels per pixel.
     * So, algorithm can change number of channels per pixels without allocation in algorithm.
     * 
     * \param[in] inBuf         pointer to input pixel array (2d array stored in 1d array)
     * \param[out] outBuf       pointer to output pixel array (2d array stored in 1d array)
     * \param[in] channels      number of channels per pixel of inBuf
     * \param[in] width         width of 2d array stored in inBuf
     * \param[in] height        height of 2d array stored in inBuf
     * \param[out] outChannels  number of channels per pixel of outBuf
     */
    virtual void preparation(unsigned char * inBuf, unsigned char * outBuf, unsigned int channels, int width, int height, int & outChannels) = 0;
    /// process() is method determined to run the main algorithm over the image
    /**
     * PURE VIRTUAL, must be implemented
     * Process method cant change number of channels between inBuf and outBuf.
     * 
     * \param[in] inBuf         pointer to input pixel array (2d array stored in 1d array)
     * \param[out] outBuf       pointer to input pixel array (2d array stored in 1d array)
     * \param[in] outWidth      is width of output array(outBuf)
     * \param[in] outHeight     is width of output array(outBuf)
     * \param[in] channels      number of channels per pixel of outBuf
     */
    virtual void process(unsigned char * inBuf, unsigned char * outBuf, int outWidth, int outHeight, int channels) = 0;
    /// channelsAfterPrep() should return number of channels after preparation
    /**
     * PURE VIRTUAL, must be implemented
     */
    virtual int channelsAfterPrep() = 0;
    /// iterationsNeededToFinish() returns number of iterations over image needed to complete process
    /**
     * PURE VIRTUAL, must be implemented
     */
    virtual int iterationsNeededToFinish() = 0;
    /// sizeOfTileBorders() returns size of neighbourhood (radius) in pixels that is needed to calculate value of one pixel(or area)
    /**
     * PURE VIRTUAL, must be implemented
     */
    virtual int sizeOfTileBorders() = 0;
    /// isFinished() return true if is process finished and output is done, otherwise returns true
    /**
     * PURE VIRTUAL, must be implemented
     */
    virtual bool isFinished() = 0;
    /// loop() provides CAlgorithm piece of information that one loop over the image has been finished
    /**
     * PURE VIRTUAL, must be implemented
     * is called in loop that is in CProcessHandler. This loop calls processing of image or it's part.
     */
    virtual void loop() = 0;
    /// in() is method that loads data of image
    /**
     * Probably most implementations would use this implementation, but it can be override.
     * \param[out]  loadBuffer  pointer to buffer in which will be stored data of loaded part of image
     * \param[in]   inX0        start (left upper corner of image) x coordinate of part of image which is going to be loaded 
     * \param[in]   inY0        start (left upper corner of image) y coordinate of part of image which is going to be loaded
     * \param[in]   loadWidth   width of part of image which is going to be loaded
     * \param[in]   loadHeight  height of part of image which is going to be loaded
     */
    virtual void in( guchar *& loadBuffer, int inX0, int inY0, int loadWidth, int loadHeight){
        gimp_pixel_rgn_get_rect(&(inOut.rgn_in), loadBuffer, inX0, inY0, loadWidth, loadHeight);
    }
    /// out()) is method that sends out data of processed image
    /**
     * Probably most implementations would use this implementation, but it can be override
     * \param[in]   outBuffer   pointer to data buffer of processed part of image which is going to be send out 
     * \param[in]   inX0        start (left upper corner of image) x coordinate of part of image which is going to be send out
     * \param[in]   inY0        start (left upper corner of image) y coordinate of part of image which is going to be send out
     * \param[in]   loadWidth   width of part of image which is going to be send out
     * \param[in]   loadHeight  height of part of image which is going to be send out
     */
    virtual void out( guchar * outBuffer, int outX0, int outY0, int width, int height){
        gimp_pixel_rgn_set_rect(&(inOut.rgn_out), outBuffer, outX0, outY0, width, height);
    }
};

//include guard end
#endif // CALGORITHM_HPP_INCLUDED