//----------------------------------------------------------------------------------------
/**
 * \file       CULBP.hpp
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Contains uniform LBP algorithm class
*/
//----------------------------------------------------------------------------------------

#include <libgimp/gimp.h>
#include <math.h>
#include "CAbstractLBP.hpp"

//include guard
#ifndef CULBP_HPP
#define CULBP_HPP

/// CULBP is class that provides implementation of improved LBP method
/**
 * in ULBP are taken into account only uniform values - uniform value is value that has in binary form max two transitions between 0 and1
 * examples:
 * 00011111 - uniform
 * 11000111 - uniform
 * 01000110 - non-uniform
 * 01010101 - non-uniform
 */
class CULBP : public CAbstractLBP {
    /// EInnerState represents state of ULBP process, simple just working(IN_PROCESS) and finished(FINISHED)
    enum EInnerState{IN_PROCESS,FINISHED};
    EInnerState innerState; ///< innerState represents state of ULBP process
    /// finished() is called when is process finished
    void finished(){innerState = EInnerState::FINISHED;}
    /// ulbp() calculates uLBP of given part of image
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
     * \param[in] inBuf        (pointer to input pixel array (2d array stored in 1d array)) Image data that are used to calculate theirs ULBP, NOTE!: It must be bigger then outBuf (relatively to radius as is shown on CASE1)
     * \param[in] outBuf       outPut Buffer where is stored result - pointer to output pixel array (2d array stored in 1d array)
     * \param[in] outWidth     is width of output array(outBuf)
     * \param[in] outHeight    is height of output array(outBuf)
     * \param[in] radius       Radius is parameter used in ULBP calculations (alse it is important for size of buffers -example: CASE1) - radius is distance of neighbor points from central point
     * \param[in] pointsCount  number of neighbor points to central points
     * \param[in] channels     number of channels per pixel of outBuf
    */ 
    void ulbp(unsigned char * inBuf, unsigned char * outBuf, int outWidth, int outHeight, int radius, int pointsCount, int channels);
public: 
    /// Constructor just calls its parents constructor and sets inner state
    /**
     * \param[in]   toInOut                 struct holding pointers to in and out regions (gimp_pixel_rgn), there is needed for pixels input and output 
     * \param[in]   toRadius                radius for ULBP
     * \param[in]   toNeighbourPointsCount  number of neighbor points - ULBP
     */
    CULBP(SInOutRgns toInOut, int toRadius, int toNeighbourPointsCount): CAbstractLBP(toInOut, toRadius, toNeighbourPointsCount),
                                                                         innerState(IN_PROCESS){}
    /// process() just passes parameters to private ulbp function
    /**
     * Computes classic LBP and then set non-uniforms to blue. Uniforms have combination of red and green. 
     * 
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
        ulbp(inBuf, outBuf, outWidth, outHeight, radius, neighbourPointsCount, channels);
    }
    /// isFinished() returns ture if is inner state set to finished, otherwise false
    /**
     * === Documentation from CAlgorithm: ===
     * isFinished() return true if is process finished and output is done, otherwise returns true
     */
    bool isFinished() override {return innerState == EInnerState::FINISHED;}
    /// loop() just calls finished, becouse after one iteration over image is ULBP finished
    /**
     * === Documentation from CAlgorithm: ===
     * loop() provides CAlgorithm piece of information that one loop over the image has been finished
     * 
     * is called in loop that is in CProcessHandler. This loop calls processing of image or it's part.
     */
    void loop() override {finished();}
};

#endif // CULBP_HPP