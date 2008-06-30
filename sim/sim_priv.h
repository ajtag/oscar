/*! @file sim_priv.h
 * @brief Private simulation module definition
 * 
 * @author Markus Berner
 */
#ifndef SIM_PRIV_H_
#define SIM_PRIV_H_

#include <log/log_pub.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#ifdef LCV_HOST
    #include <framework_types_host.h>
#else
    #include <framework_types_target.h>
#endif /* LCV_HOST */

/*! @brief Prefix of the test image file names.
 * 
 * The test image file names are generated by concatenating this prefix,
 * a number corresponding to the timestep and TEST_IMG_SUFFIX. */
#define TEST_IMG_PREFIX "testdata/Bogen001_"
/*! @brief The digits in the test image sequence numbers. 
 * (Is it Img00001.bmp or Img001.bmp?) */
#define TEST_IMG_SEQ_NR_DIGITS 5
/*! @brief Suffix (image type) of the test image file names */
#define TEST_IMG_SUFFIX ".bmp"
/*! @brief Maximal number of callback functions allowed to register */
#define MAX_NUMBER_CALLBACK_FUNCTION 100


/*! @brief The object struct of the simulation module */
struct LCV_SIM_OBJ
{
    /*! @brief The current timestep, i.e. image analysed */
    uint32          curTimeStep;
    /*! @brief List with Callback functions to call every cycle */
    uint16 numCycleCallback;
#if defined(LCV_HOST) || defined(LCV_SIM)    
    void (*aryCycleCallback[ MAX_NUMBER_CALLBACK_FUNCTION])(void);
#endif /*LCV_HOST*/    
};

/*======================= Private methods ==============================*/


#endif /*SIM_PRIV_H_*/
