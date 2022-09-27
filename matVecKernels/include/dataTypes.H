/*
 * @file dataTypes.H
 *
 * @brief definition of the basic data types in yocotfoam
 *
 * @detail the "_k" at the end of the datatypes has to be intended as "_kind".
 *         A kind is a typedef to a native datatype of the language
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */

#ifndef __DATA_TYPES_H__
#define __DATA_TYPES_H__

/**
 * @addtogroup utils
 * @{
 */

/**
 * Type used for memory handling
 */
typedef unsigned char* mem_k;

/**
 * Type used for error handling
 */
typedef unsigned char err_k;

/**
 * Type used for integers
 */
typedef unsigned int idx_k;

/**
 * Type used for the grid geometry data
 */
typedef double grd_k;

/**
 * Type used for the fields data
 */
typedef double fld_k;

/**
 * Type used for the matrix source term
 */
typedef double src_k;

/**
 * Type used for constant strings
 */
typedef const char* str_k;


/**
 * Datatype for the diagonal elements in a LDU matrix
 */
typedef double diag_k;


/**
 * Datatype for the off diagonal elements in a LDU matrix
 */
typedef double offDiag_k;


/**
 * Dimensions in bytes of the datatypes
 */

/**
 * idx_k size in bytes
 */
static const idx_k isz=4;

/**
 * grd_k size in bytes
 */
static const idx_k gsz=8;

/**
 * fld_k size in bytes
 */
static const idx_k fsz=8;

/**
 * err_k size in bytes
 */
static const idx_k esz=1;

/**
 * mem_k size in bytes
 */
static const idx_k msz=1;

/**
 * str_k size in bytes
 */
static const idx_k ssz=1;

/**
 * sde_k size in bytes
 */
static const idx_k dsz=8;

/**
 * soe_k size in bytes
 */
static const idx_k osz=8;

/**
 * mst_k size in bytes
 */
static const idx_k rsz=8;

/**
 * @}
 */

#endif

