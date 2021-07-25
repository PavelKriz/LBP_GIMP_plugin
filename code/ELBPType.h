//----------------------------------------------------------------------------------------
/**
 * \file       ELBPType.h
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Contains ELBPType enum
*/
//----------------------------------------------------------------------------------------

//include guard
#ifndef ELBPTYPE_HPP
#define ELBPTYPE_HPP

/// ELBPType is enum that describe LBP method type
enum ELBPType{
    ELBP, EmLBP, EULBP, ECLBP_SMC, ECLBP_S, ECLBP_M, ECLBP_C  
};

#endif // ELBPTYPE_HPP