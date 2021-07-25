//----------------------------------------------------------------------------------------
/**
 * \file       CMLBP.hpp
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Contains median LBP algorithm class
*/
//----------------------------------------------------------------------------------------

#include <libgimp/gimp.h>
#include <math.h>
#include "CAbstractLBP.hpp"

//include guard
#ifndef CMLBP_HPP
#define CMLBP_HPP

/// CMLBP is class that provides implementation of improved LBP method
/**
 * in mLBP is value of center computed from median of original center value and neighbours
 */
class CMLBP : public CAbstractLBP {
    /// EInnerState represents state of MLBP process, simple just working(IN_PROCESS) and finished(FINISHED)
    enum EInnerState{IN_PROCESS,FINISHED};
    EInnerState innerState; ///< innerState represents state of MLBP process
    /// finished() is called when is process finished
    void finished(){innerState = EInnerState::FINISHED;}
    /// mlbp() calculates mLBP of given part of image
    /**
     * "CASE1"
     * radius = 2
     *  inBuf - 7x7     outBuf - 3x3
     *  # = borders - radius @ = image(outBuf content)
     * 
     *  # # # # # # # 
     *  # # # # # # #
     *  # # @ @ @ # #    ->      @ @ @
     *  # # @ @ @ # #    ->      @ @ @
     *  # # @ @ @ # #    ->      @ @ @
     *  # # # # # # #
     *  # # # # # # #
     * 
     * if channels==4 (RGBA) then is alpha channel set to 255
     * 
     * \param[in] inBuf        (pointer to input pixel array (2d array stored in 1d array)) Image data that are used to calculate theirs mLBP, NOTE!: It must be bigger then outBuf (relatively to radius as is shown on CASE1)
     * \param[in] outBuf       outPut Buffer where is stored result - pointer to output pixel array (2d array stored in 1d array)
     * \param[in] outWidth     is width of output array(outBuf)
     * \param[in] outHeight    is height of output array(outBuf)
     * \param[in] radius       Radius is parameter used in mLBP calculations (alse it is important for size of buffers -example: CASE1) - radius is distance of neighbor points from central point
     * \param[in] pointsCount  number of neighbor points to central points
     * \param[in] channels     number of channels per pixel of outBuf
    */ 
    void mlbp(unsigned char * inBuf, unsigned char * outBuf, int outWidth, int outHeight, int radius, int pointsCount, int channels);
public: 
    /// Constructor just calls its parents constructor and sets inner state
    /**
     * \param[in]   toInOut                 struct holding pointers to in and out regions (gimp_pixel_rgn), there is needed for pixels input and output 
     * \param[in]   toRadius                radius for mLBP
     * \param[in]   toNeighbourPointsCount  number of neighbor points - mLBP
     */
    CMLBP(SInOutRgns toInOut, int toRadius, int toNeighbourPointsCount): CAbstractLBP(toInOut, toRadius, toNeighbourPointsCount),
                                                                         innerState(IN_PROCESS){}
    /// process() just passes parameters to private mlbp function
    /**
     * === Documentation from CAlgorithm: ===
     * process() is method determined to run the main algorithm over the image
     * 
     * Process method cant change number of channels between inBuf and outBuf.
     * 
     * \param[in] inBuf         pointer to input pixel array (2d array stored in 1d array)
     * \param[out] outBuf       pointer to input pixel array (2d array stored in 1d array)
     * \param[in] outWidth      is width of output array(outBuf)
     * \param[in] outHeight     is width of output array(outBuf)
     * \param[in] channels      number of channels per pixel of outBuf
     */                                                                      
    void process(unsigned char * inBuf, unsigned char * outBuf, int outWidth, int outHeight, int channels) override {
        mlbp(inBuf, outBuf, outWidth, outHeight, radius, neighbourPointsCount, channels);
    }
    /// isFinished() returns ture if is inner state set to finished, otherwise false
    /**
     * === Documentation from CAlgorithm: ===
     * isFinished() return true if is process finished and output is done, otherwise returns true
     */
    bool isFinished() override {return innerState == EInnerState::FINISHED;}
    /// loop() just calls finished, becouse after one iteration over image is mLBP finished
    /**
     * === Documentation from CAlgorithm: ===
     * loop() provides CAlgorithm piece of information that one loop over the image has been finished
     * 
     * is called in loop that is in CProcessHandler. This loop calls processing of image or it's part.
     */
    void loop() override {finished();}
};

#endif // CMLBP_HPP