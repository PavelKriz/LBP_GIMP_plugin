//----------------------------------------------------------------------------------------
/**
 * \file       params.h
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Contains all parameters which can be changed, but without knowledge its not recommended at all.
*/
//----------------------------------------------------------------------------------------

#include<string>

//include guard
#ifndef PARAMS_H
#define PARAMS_H

//=============Params which are needed through the whole process================

/// DEFAULT_TILE_SIZE is default size of tile(part of image, big images are  processed  in more tiles) used in plugin - CAN BE CHANGED IN RANGE [10,infinity]
const int DEFAULT_TILE_SIZE = 1000;

//====================Params which are needed only in interface=================
/// Label that is shown in UI, this label depends to LBP method choice box
const std::string sPickLBPMethodLabel = "<b>Pick LBP method</b>";
/// Label that is shown in UI, this label depends to parameters choice box
const std::string sMofifyParametsLabel = "<b>Modify parameters</b>";
/// Label that is shown in UI before radius spin button
const std::string sRadiusLabel = "_Radius:";
/// Label that is shown in UI before points spin button
const std::string sPointsLabel = "_Points:";

///string used in plugin user interface in methods choosing combobox, it is also used for determining which method will be used - CAN BE CHANGED
const std::string sLBP = "LBP";
///string used in plugin user interface in methods choosing combobox, it is also used for determining which method will be used - CAN BE CHANGED
const std::string smLBP = "mLBP";
///string used in plugin user interface in methods choosing combobox, it is also used for determining which method will be used - CAN BE CHANGED
const std::string sULBP = "ULBP";
///string used in plugin user interface in methods choosing combobox, it is also used for determining which method will be used - CAN BE CHANGED
const std::string sCLBP = "CLBP";

///string used in plugin user interface in sub methods choosing combobox, it is also used for determining which method will be used - CAN BE CHANGED
const std::string sCLBP_SMC = "CLBP_SMC";
///string used in plugin user interface in sub methods choosing combobox, it is also used for determining which method will be used - CAN BE CHANGED
const std::string sCLBP_S = "CLBP_S";
///string used in plugin user interface in sub methods choosing combobox, it is also used for determining which method will be used - CAN BE CHANGED
const std::string sCLBP_M = "CLBP_M";
///string used in plugin user interface in sub methods choosing combobox, it is also used for determining which method will be used - CAN BE CHANGED
const std::string sCLBP_C = "CLBP_C";

/// MAX_RADIUS used in LBP methods - CAN BE CHANGED IN RANGE [1, DEFAULT_TILE_SIZE-1]
const int MAX_RADIUS = 190;
/// MIN_RADIUS used in LBP methods - CAN BE CHANGED IN RANGE [1, MAX_RADIUS]
const int MIN_RADIUS = 1;
/// MAX_POINTS_LBP the maximum number of neighbour points in LBP  - CAN BE CHANGED IN RANGE [1, 24]
const int MAX_POINTS_LBP = 24;
/// MAX_POINTS_MLBP the maximum number of neighbour points in MLBP - CAN BE CHANGED IN RANGE [1, 24]
const int MAX_POINTS_MLBP = 24;
/// MAX_POINTS_ULBP the maximum number of neighbour points in ULBP - CAN BE CHANGED IN RANGE [1, 16]
const int MAX_POINTS_ULBP = 16;
/// MAX_POINTS_CLBP the maximum number of neighbour points in CLBP - CAN BE CHANGED IN RANGE [1, 8]
const int MAX_POINTS_CLBP = 8;
/// MIN_POINTS the minimum number of neighbour points in all LBP methods - CAN BE CHANGED IN RANGE [1, min(MAX_POINTS_[LBP method])]
const int MIN_POINTS = 4;
/// DEFAULT_POINTS number of neighbour points in all LBP methods - CAN BE CHANGED IN RANGE [1, min(MAX_POINTS_[LBP method])]
const int DEFAULT_POINTS = 8;
/// DEFAULT_RADIUS used in LBP methods - CAN BE CHANGED IN RANGE [1, MAX_RADIUS]
const int DEFAULT_RADIUS = 10;

#endif //PARAMS_H
