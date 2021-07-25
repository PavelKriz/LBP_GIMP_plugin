//----------------------------------------------------------------------------------------
/**
 * \file       CCLBP.hpp
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Contains completed LBP algorithm class
*/
//----------------------------------------------------------------------------------------

#include <libgimp/gimp.h>
#include <math.h>
#include "CAbstractLBP.hpp"
#include <utility>

//include guard
#ifndef CCLBP_HPP
#define CCLBP_HPP

/// CCLBP is class that provides implementation of improved LBP method
/**
 * in CLBP(completed LBP) are computed three parts(S,M,C):
 * 
 * S(classic LBP) - for( all neigbourPoints){ S |= (neigbourPoint_k  >= meanMpValue.first) << k}
 * 
 * M(magnitude - like LBP but for every neighbor point looks calculation this: for( all neigbourPoints){  M |= (magnitude_k >= MM) << k}
 * 
 *      MM - means mean magnitude of whole image, magnitude_k = |neigbourPoint_k - center|
 * 
 * C(LBP but on place of center is there mean gray value of whole image (CI) ): for( all neigbourPoints){ C |= (neigbourPoint_k >= CI) << k}
 * 
 * its recommended to take a closer look on this method first somewhere else, becouse its more complicated then other LBP methods
 */
class CCLBP : public CAbstractLBP {
public:
    /// Enum that provide information about output. If the whole CLBP will be show or only its parts
    enum ECLBPType{ ECLBP_SMC, ECLBP_S, ECLBP_M, ECLBP_C};
private:
    ECLBPType clbpType; ///< holds info about sub-type of CLBP
    std::pair<double, int> meanGrayValue; ///< computed mean gray value of whole image / first is value - second is weight (number of avaraged pixels)
    std::pair<double, int> meanMpValue;   ///< computed mean magnitude value of whole image / first is value - second is weight (number of avaraged pixels)
    /// EInnerState represents state of CLBP process, computing mean values (MEAN_VALUES), final computing of CLBP(CLBP) and finished(FINISHED)
    enum EInnerState{MEAN_VALUES, CLBP, FINISHED};
    EInnerState innerState; ///< innerState represents state of CLBP process
    
    /// nextState is called when is CLBP process going to next State of processing
    void nextState();
    /// meanValues() computes mean gray value and magnitude value of whole image
    /**
     * 
     * \param[in] inBuf         `   pointer to input pixel array (2d array stored in 1d array)
     * \param[in] outWidth          is width without borders(radius)
     * \param[in] outHeight         is height without borders(radius)
     * \param[in] radius            Radius is parameter used in CLBP mean values calculations - radius is distance of neighbor points from central point
     * \param[in] pointsCount       number of neighbor points to central points
     */
    void meanValues(unsigned char * inBuf, int outWidth, int outHeight, int radius, int pointsCount);
    /// clbp_smc() calculates cLBP(or its component) of given part of image
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
     * \param[in] inBuf        (pointer to input pixel array (2d array stored in 1d array)) Image data that are used to calculate theirs CLBP, NOTE!: It must be bigger then outBuf (relatively to radius as is shown on CASE1)
     * \param[in] outBuf       outPut Buffer where is stored result - pointer to output pixel array (2d array stored in 1d array)
     * \param[in] outWidth     is width of output array(outBuf)
     * \param[in] outHeight    is height of output array(outBuf)
     * \param[in] radius       Radius is parameter used in CLBP calculations (alse it is important for size of buffers -example: CASE1) - radius is distance of neighbor points from central point
     * \param[in] pointsCount  number of neighbor points to central points
     * \param[in] channels     number of channels per pixel of outBuf
    */ 
    void clbp_smc(unsigned char * inBuf, unsigned char * outBuf, int outWidth, int outHeight, int radius, int pointsCount, int channels);
    /// updateMean() universal method to update mean vealue
    /**
     * \param[out] original         mean value to be updated - first is value and second is weight (number of avaraged values recoomended)
     * \param[in] updateValue       value to be avaraged with original mean value
     * \param[in] updatetWeight     weight of update value (recommended to be count of points with this value)
     */
    void updateMean(std::pair<double, int> & original, int updateValue, int updatetWeight);
    /// updateMeanGray() method to update mean gray value of whole image
    /**
     * \param[in] updateValue       value to be avaraged with original mean gray value
     * \param[in] updatetWeight     weight of update value (recommended to be count of points with this value)
     */
    inline void updateMeanGray(int updateValue, int updatetWeight){ updateMean(meanGrayValue, updateValue, updatetWeight);}
    /// updateMeanMp() method to update mean magnitude value of whole image
    /**
     * \param[in] updateValue       value to be avaraged with original mean magnitude value
     * \param[in] updatetWeight     weight of update value (recommended to be count of points with this value)
     */
    inline void updateMeanMp(int updateValue, int updateWeight){ updateMean(meanMpValue, updateValue, updateWeight);}
public: 
    /// Constructor calls its parents constructor and sets inner state and what its important it changes iterationsNeededToFinishVal to 2
    /**
     * NOTE: CLBP need two passes over image at least
     * \param[in]   toInOut                 struct holding pointers to in and out regions (gimp_pixel_rgn), there is needed for pixels input and output 
     * \param[in]   toRadius                radius for CLBP
     * \param[in]   toNeighbourPointsCount  number of neighbor points - CLBP
     */
    CCLBP(SInOutRgns toInOut, int toRadius, int toNeighbourPointsCount, ECLBPType toClbpType): CAbstractLBP(toInOut, toRadius, toNeighbourPointsCount),
                                                                         innerState(MEAN_VALUES),
                                                                         clbpType(toClbpType)
                                                                         { iterationsNeededToFinishVal = 2;}
    /// process() just passes parameters to other private functions
    /**
     * depends on state it passes parameters to meanValues or clbp_smc
     * it needs to switch becouse in first iteration it calculates only mean values
     * in second itearion it calculates clbp(or its components)
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
    void process(unsigned char * inBuf, unsigned char * outBuf, int outWidth, int outHeight, int channels) override ;
    /// out()) is method that sends out data of processed image
    /**
     * OVERRIDES CAlgorithm implementation becouse in first iteration is there no output.
     * It does send output data on second iteration, it just calls CAlgorithm output.
     *  
     * \param[in]   outBuffer   pointer to data buffer of processed part of image which is going to be send out 
     * \param[in]   inX0        start (left upper corner of image) x coordinate of part of image which is going to be send out
     * \param[in]   inY0        start (left upper corner of image) y coordinate of part of image which is going to be send out
     * \param[in]   loadWidth   width of part of image which is going to be send out
     * \param[in]   loadHeight  height of part of image which is going to be send out
     */
    void out(guchar * outBuffer, int outX0, int outY0, int width, int height) override ;
    /// isFinished() returns ture if is inner state set to finished, otherwise false
    /**
     * === Documentation from CAlgorithm: ===
     * isFinished() return true if is process finished and output is done, otherwise returns true
     */
    bool isFinished() override {return innerState == EInnerState::FINISHED;}
     /// loop() calls nextState(), becouse after two iterations over image is CLBP finished
    /**
     * === Documentation from CAlgorithm: ===
     * loop() provides CAlgorithm piece of information that one loop over the image has been finished
     * 
     * is called in loop that is in CProcessHandler. This loop calls processing of image or it's part.
     */
    void loop() override {nextState();}
};

#endif //CCLBP_HPP