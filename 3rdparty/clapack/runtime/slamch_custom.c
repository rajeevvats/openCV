#include "f2c.h"
#include <float.h>
#include <stdio.h>

/* *********************************************************************** */

double slamc3_(float *a, float *b)
{
    /* System generated locals */
    float ret_val;
    
    
    /*  -- LAPACK auxiliary routine (version 3.1) -- */
    /*     Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd.. */
    /*     November 2006 */
    
    /*     .. Scalar Arguments .. */
    /*     .. */
    
    /*  Purpose */
    /*  ======= */
    
    /*  SLAMC3  is intended to force  A  and  B  to be stored prior to doing */
    /*  the addition of  A  and  B ,  for use in situations where optimizers */
    /*  might hold one of these in a register. */
    
    /*  Arguments */
    /*  ========= */
    
    /*  A       (input) REAL */
    /*  B       (input) REAL */
    /*          The values A and B. */
    
    /* ===================================================================== */
    
    /*     .. Executable Statements .. */
    
    ret_val = *a + *b;
    
    return ret_val;
    
    /*     End of SLAMC3 */
    
} /* slamc3_ */

/* simpler version of slamch for the case of IEEE754-compliant FPU module by Piotr Luszczek S.
 taken from http://www.mail-archive.com/numpy-discussion@lists.sourceforge.net/msg02448.html */

#ifndef FLT_DIGITS
#define FLT_DIGITS 24
#endif

const double lapack_slamch_tab[] =
{
    0, FLT_RADIX, FLT_EPSILON, FLT_MAX_EXP, FLT_MIN_EXP, FLT_DIGITS, FLT_MAX,
    FLT_EPSILON*FLT_RADIX, 1, FLT_MIN*(1 + FLT_EPSILON), FLT_MIN
};

double slamch_(char* cmach)
{
    return lapack_slamch_tab[lapack_lamch_tab[(unsigned char)cmach[0]]];
}
