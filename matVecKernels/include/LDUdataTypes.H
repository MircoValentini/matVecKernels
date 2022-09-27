/*
 * @file LDUdataTypes.H
 *
 * @brief definition of the basic data types in yocotfoam for representing 
 *        LDU matrices
 *
 * @detail the "_k" at the end of the datatypes has to be intended as "_kind".
 *         A kind is a typedef to a native datatype of the language
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */

#ifndef __LDU_DATA_TYPES_H__
#define __LDU_DATA_TYPES_H__

/**
 * Datatype for the diagonal elements in a LDU matrix
 */
typedef double LDUdiag_k;


/**
 * Datatype for the off diagonal elements in a LDU matrix
 */
typedef double LDUoffDiag_k;


#endif
