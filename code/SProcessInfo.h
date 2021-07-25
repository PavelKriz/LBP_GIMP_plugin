//----------------------------------------------------------------------------------------
/**
 * \file       SProcessInfo.h
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Contains SProcessInfo struct
*/
//----------------------------------------------------------------------------------------

#include "CAlgorithm.hpp"

//include guard
#ifndef SPROCESSINFO_H
#define SPROCESSINFO_H

/// SProcessInfo holds information needed during process
struct SProcessInfo{
    int channels;               ///< number of channels per pixel used in image
    CAlgorithm * algorithm;     ///< pointer to algorithm which will be used
};

#endif //SPROCESSINFO_H