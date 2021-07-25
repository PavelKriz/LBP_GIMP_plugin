//----------------------------------------------------------------------------------------
/**
 * \file       CAbstractLBP.hpp
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Contains abstract LBP class
*/
//----------------------------------------------------------------------------------------


#include "CAlgorithm.hpp"
#include "SInOutRgns.h"
#include <math.h>

//include guard
#ifndef CAbstractLBP_HPP
#define CAbstractLBP_HPP

/// CAbstractLBP is class implementing things that have all LBPs in common.
/**
 * It provides coordinates of neighbor points, even with interpolation between two pixels for higher accuracy of point placement.
 * It provides conversion from multichannel per pixel (Gray, GrayA, RGB, RGBA) to Grayscale [0,255]
 * and other things that have LBPs methods in common.
 */
class CAbstractLBP : public CAlgorithm {
protected:
    ///< Representation of neighbor point
    /**
     * Every point is for higher accuracy defined by two pixels and weight - one point may be also determined only by one pixel and weight 1
     * If is point represented by one pixel then fx==sx and fy==sy and fWeight==sWeight==0.5
     * fweight must be qual to 1-sweight everytime
     */
    struct SPointsLocCoords{
        int fx; ///< x coordinate for first pixel
        int fy; ///< y coordinate for first pixel
        int sx; ///< x coordinate for second pixel
        int sy; ///< y coordinate for second pixel
        float fWeight;  ///< weight of first point [0,1]
        float sWeight;  ///< weight of second point [0,1]
    };
    SPointsLocCoords * pointsLocCoords; ///< array of neighbor points - every point is for higher accuracy defined by two pixels and weight - one point may be also determined only by one pixel and weight 1
    static const int channelsAfterPreparation = 1;  ///< number of channels after preparation is for every LBP 1 (grayscale)
    int iterationsNeededToFinishVal; //!< how many iterations
    int neighbourPointsCount;   ///< number of neighbor points to central points
    int radius; ///< radius is distance of neighbor points from central point
    /// calculates neighbor points local coordinates
    /**
     * It calculates position of every point by cos(angle) for x and -sin(angle) for y. Position coordinates are rounded too and
     * then multiplied by radius so they are in proper distance.
     * Note: angle is computed for every point like this: 2*PI/[number of neighbor points]*[order of point]
     * Note: sometimes are coordinates almost rounded to higher or lower number (example  - 3.42). So for higher accuracy is calculated also second point
     * then its final point interpolated between these two pixels.
     * 
     * sSecond point is distributed like this: 
     * distY/distX are values computed from: coordinate_on_axisX/Y - rounded _coordinate_on_axisX/Y
     * This diagram represents one pixel, every ascii symbol represents square with side length 0.25(pixel length). Left lower corner has coordinates [-0.5,-0.5]
     * Second pixel will be on place depending on coordinates  of [distX, distY]
     * @ - second point will be also center
     * ^ > V < - second point will be upper or right or lower or left neighbor pixel
     * ┌ ┐ └ ┘ - second point will be one of the corner neighbors
     * 
     * DIAGRAM
     * 
     *  ┌ ^ ^ ┐
     *  < @ @ >
     *  < @ @ >
     *  └ V V ┘
     * 
     */
    void pointsLocCoordsCalc();
    /// calcValByInterpol, interpolates between two points by their weight
    /**
     * \param[in] inBuf     pointer to pixel(points) array (2d array stored in 1d array)
     * \param[in] plCoords  local coordinates of two points and their weights which are going to be interpolated
     * \param[in] J         x coordinate of center point in inBuf, (it's also zero points for local coordinates)     
     * \param[in] I         y coordinate of center point in inBuf, (it's also zero points for local coordinates)
     * \param[in] width     width of inBuf 2d array (width means x dimension of 2d array) 
     */
    inline unsigned char calcValByInterpol(unsigned char * inBuf, SPointsLocCoords & plCoords,int J, int I, int width){
         return     (( unsigned char) inBuf[  (I + plCoords.fy) * width + J + plCoords.fx] * plCoords.fWeight +
                    ( unsigned char) inBuf[(I + plCoords.sy) * width + J + plCoords.sx] * plCoords.sWeight);
    }
    /// Converting pixels to gray
    /**
     * Conversion is based on weighted formula for all channels, or only copiing from alpha gray to just gray
     * \param[in] inBuf        Buffer with pixels that should be converted to grayscale    
     * \param[in] outBuf       outPut Buffer where is stored result - only one channel used(grayscale)
     * \param[in] channels     Number of channels used, (1,2,3,4) ... otherwise outBuf is NULL
     */
    void arrayToGray(unsigned char * inBuf, unsigned char * outBuf, unsigned int channels, int pixelCount);
public:
    /// Constructor
    /**
     * sets iterationsNeededToFinishVal to 1, becouse most LBP methods needs only one
     * also it allocates array for local coordinats to all neighbor points (SPointsLocCoords)
     * 
     * \param[in]   toInOut                 struct holding pointers to in and out regions (gimp_pixel_rgn), there is needed for pixels input and output 
     * \param[in]   toRadius                radius for LBP
     * \param[in]   toNeighbourPointsCount  number of neighbor points - LBP
     */
    CAbstractLBP(SInOutRgns toInOut, int toRadius ,int toNeighbourPointsCount ):   CAlgorithm(toInOut),
                                                                                    radius(toRadius),
                                                                                    neighbourPointsCount(toNeighbourPointsCount)
                                                                                    {iterationsNeededToFinishVal = 1;
                                                                                     pointsLocCoords = new SPointsLocCoords[toNeighbourPointsCount];}
    /// Destrcutor just deallocates allocated memory                                                                                 
    ~CAbstractLBP(){delete [] pointsLocCoords;}
    /// preparation() calculates gray value and neighborpoints local positions
    /**
     * Preparation is the same for all implemented LBP extracting methods;
     * All implemented LBP's need gray value input, so this preparation calculates them.
     * All LBP's need coordinates of neighbor points, so this preparation calculates those coords also.
     * 
     * === Documentation from CAlgorithm: ===
     * preparation() is method that prepares image data for later processing
     * 
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
    void preparation(unsigned char * inBuf, unsigned char * outBuf, unsigned int channels, int width, int height, int & outChannels) override;
    /// channelsAfterPrep() just retturn channelsAfterPreparation variable, which should be 1 (gray) for all LBP implementations
    /**
     * === Documentation from CAlgorithm: ===
     * channelsAfterPrep() should return number of channels after preparation
     */
    int channelsAfterPrep() override {return channelsAfterPreparation;}
    /// iterationsNeededToFinish() returns iterationsNeededToFinishVal which must be set in constructor for each LBP differently
    /**
     * === Documentation from CAlgorithm: ===
     * iterationsNeededToFinish() returns number of iterations over image needed to complete process
     */
    int iterationsNeededToFinish() override {return iterationsNeededToFinishVal;}
    /// sizeOfTileBorders() just returns LBP radius
    /**
     * === Documentation from CAlgorithm: ===
     * sizeOfTileBorders() returns size of neighbourhood (radius) in pixels that is needed to calculate value of one pixel(or area)
     */
    int sizeOfTileBorders() override {return radius;}
};

#endif //CAbstractLBP_HPP