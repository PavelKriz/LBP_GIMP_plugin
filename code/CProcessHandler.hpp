//----------------------------------------------------------------------------------------
/**
 * \file       CProcessHandler.hpp
 * \author     Pavel Kříž, Ing. Jiří Chludil
 * \date       2019/11/30
 * \brief      Contains class handling communication between UI and algorithm classes. It also do init start run and end of process.
*/
//----------------------------------------------------------------------------------------

#include <libgimp/gimp.h>
#include "CTileManager.hpp"
#include "SUIInfo.h"
#include "CLBP.hpp"
#include "CMLBP.hpp"
#include "CULBP.hpp"
#include "CCLBP.hpp"

//include guard
#ifndef CPROCESSHANDLER_HPP
#define CPROCESSHANDLER_HPP
/// CProcessHandler is class handling communication between UI and algorithm classes.
/**
 * It takes necessary information about chosen Algorithm method and parameters needed for running the algorithm.
 * It also needs pointers to gimp drawable or preview.
 * 
 * In init it initilizes parameters and input/output regions of image by calling gimp api functions
 * In process it construct currently chosen CAlgorithm (in LBP it is a LBP method), then it inits and runs CTileManager.run().
 * It runs CTileMananager in loop so many times as is return value of processInfo.algorithm->iterationsNeededToFinish().
 * In the same loop should be called CAlgorithm.loop() method. So the CAlgorithm implementation has information, that one iteration
 * over image has been finished.
 */
class CProcessHandler{
    gint channels;  ///< number of channels per pixel
    gint width;     ///< width of part of image which is going to be processed (Part of image may be whole image)
    gint height;    ///< height of part of image which is going to be processed (Part of image may be whole image)
    gint x1;        ///< start (left upper corner of image) x coordinate of part of image which is going to be processed 
    gint y1;        ///< start (left upper corner of image) y coordinate of part of image which is going to be processed 
    gint x2;        ///< end (right lower corner of image) x coordinate of part of image which is going to be processed 
    gint y2;        ///< end (right lower corner of image) y coordinate of part of image which is going to be processed 
    SInOutRgns inOut;           ///< struct holding pointers to in and out regions (gimp_pixel_rgn) 
    GimpDrawable *drawable;     ///< pointer to gimp drawable (canvas of image going to be processed)
    GimpPreview  *preview;      ///< preview in plugin UI - preview is small result( image after process) preview
    SUIInfo global;             ///< UI global info, mainly pointers to UI widgets and Algorithm parameters
public:
    /// Constructor
    /**
     * It initilizes parameters and input/output regions of image by calling gimp api functions.
     * 
     * /param[in] drawable  pointer to gimp drawable (canvas of image going to be processed)
     * /param[in] preview   preview in plugin UI - preview is small result( image after process) preview
     * /param[in] global    UI global info, mainly pointers to UI widgets and Algorithm parameters
     */
    CProcessHandler(GimpDrawable * drawable, GimpPreview  * preview, SUIInfo global);
    /// process is method that starts processing of image
    /**
     * It constructs currently chosen CAlgorithm (in LBP it is a LBP method), then it inits and runs CTileManager.run().
     * It runs CTileMananager in loop so many times as is return value of processInfo.algorithm->iterationsNeededToFinish().
     * In the same loop should be called CAlgorithm.loop() method. So the CAlgorithm implementation has information, that one iteration
     * over image has been finished.
     */
    void process(ELBPType methodType);
};

#endif //CPROCESSHANDLER_HPP