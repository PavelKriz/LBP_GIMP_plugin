//----------------------------------------------------------------------------------------
/**
 * \file       SInOutRgns.h
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Contains SInOutRgns struct
*/
//----------------------------------------------------------------------------------------

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

//include guard
#ifndef SINOUTRGNS_H
#define SINOUTRGNS_H

/// SInOutRgns is struct holding pointers to in and out regions (gimp_pixel_rgn) 
struct SInOutRgns
{
    GimpPixelRgn rgn_in;    ///< used for reading pixels
    GimpPixelRgn rgn_out;   ///< used for writing pixels
};

#endif //SINOUTRGNS_H
