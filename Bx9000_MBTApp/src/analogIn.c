/*
 * file:                analogIn.c
 * purpose:             subroutine to perform calculation for analog input signal. calculation
 *                      depends on type of signal. 
 *
 * created:             02-Sep-2009
 *
 * revision history:
 *   02-Sep-2009        Sonya Hoobler             initial version
 *
 * remarks:             Possible signal types are:
 *                        1  linear scaling
 *                        2  log scaling
 *                        3  phase mixer
 *
 *                      If type=1, perform linear scaling, write result to val
 *                      If type=2, perform log scaling, write result to val
 *                      If type 3, perform phase mixer conversion, write result to val
*                                                                             
 *                       Inputs:                                                                      
 *                       a = type                                                             
 *                       b = raw signal          
 *                       c = offs for linear, coef for log                                            
 *                       d = coef for linear, coef of exponent for log                                
 *                       e = val of associated device (phase mixer only)
 *                       f = sign (1 or -1) (phase mixer only)
 */

#include <stdio.h>
#include <math.h>

#include <registryFunction.h>
#include <epicsExport.h>
#include <subRecord.h>


/* Add MACRO Definitions for min/max */
/* Needed as of EPICS BASE R3-14-11 */
/* provide an macro that implements max() and min() */

#ifndef __cplusplus
#ifndef max
#define max(a,b) (((a)>(b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a)<(b)) ? (a) : (b))
#endif
#endif


static long
InitAnalogCalc( subRecord *psub)
        {

        return 0;
        }

static long
AnalogCalc( subRecord *psub)
        {
        int type;
        float t;
        float b;
        float c;
        float d;
        float e;
        float f;

        if( psub == NULL)
           return 0;

        type = psub->a;
        b = psub->b;
        c = psub->c;
        d = psub->d;
        e = psub->e;
        f = psub->f;

        if(type == 1) /* linear scaling */
           psub->val = c + d*b;

        if(type == 2) /* log scaling */
           psub->val = c * (pow(10,d*b));

        if(type == 3) /* phase mixer */ 
           {
           t = min(max(d*0.01745329*b,-1.2),1.2); 
           psub->val = c + 57.29577*(t + 0.1667*pow(t,5)) + f*e;
           }
          return 0;
        }
epicsRegisterFunction(InitAnalogCalc);
epicsRegisterFunction(AnalogCalc);
