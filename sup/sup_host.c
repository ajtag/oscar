/*! @file sup_host.c
 * @brief Support module implementation for host
 * 
 */

#include "oscar_types_host.h"

#include "sup_pub.h"
#include "sup_priv.h"
#include "oscar_intern.h"
#include <time.h>

/*! @brief The module singelton instance. */
struct OSC_SUP sup;		


OSC_ERR OscSupCreate(void *hFw)
{
    struct OSC_FRAMEWORK *pFw;

    pFw = (struct OSC_FRAMEWORK *)hFw;
    if(pFw->sup.useCnt != 0)
    {
        pFw->sup.useCnt++;
        /* The module is already allocated */
        return SUCCESS;
    }
    
	memset(&sup, 0, sizeof(struct OSC_SUP));
	
    /* Increment the use count */
    pFw->sup.hHandle = (void*)&sup;
    pFw->sup.useCnt++; 
    
    return SUCCESS;
}

void OscSupDestroy(void *hFw)
{
    struct OSC_FRAMEWORK *pFw;
            
    pFw = (struct OSC_FRAMEWORK *)hFw; 
    /* Check if we really need to release or whether we still 
     * have users. */
    pFw->sup.useCnt--;
    if(pFw->sup.useCnt > 0)
    {
        return;
    }
    
    /* Make sure the watchdog is closed, otherwise the board may
     * be reset by it after the application has closed. */
    OscSupWdtClose();
    
	memset(&sup, 0, sizeof(struct OSC_SUP));
}

/*=========================== Watchdog =================================*/

OSC_ERR OscSupWdtInit()
{
    /* There is no watchdog on the host => do nothing. */

    return SUCCESS;
}

OSC_ERR OscSupWdtClose()
{
    /* There is no watchdog on the host => do nothing. */

    return SUCCESS;
}

inline void OscSupWdtKeepAlive()
{
    /* There is no watchdog on the host => do nothing. */
}

/*============================= Cycles =================================*/
inline uint32 OscSupCycGet()
{
  /* The host implementation uses the ANSI C function clock() 
   * with much lower precision than on the blackfin. 
   * The actual precision is dependent on the platform. */

  /* ANSI C function, low resolution */
  return ((uint32)clock()); 
}

inline uint32 OscSupCycToMicroSecs(uint32 cycles)
{
  return (cycles/(CLOCKS_PER_SEC/1000000));
}

/*============================== SRAM =================================*/
void* OscSupSramAllocL1DataA(unsigned int size)
{
	if(size > SRAM_L1A_LENGTH)
		return NULL;
		
	/* Just allocate normal memory on host. */
	return malloc(size);	
}

void* OscSupSramAllocL1DataB(unsigned int size)
{
	if(size > SRAM_L1B_LENGTH)
		return NULL;
		
	/* Just allocate normal memory on host. */
	return malloc(size);	
}

void* OscSupSramAllocL1Data(unsigned int size)
{
	if((size > SRAM_L1A_LENGTH) && (size > SRAM_L1B_LENGTH))
		return NULL;
		
	/* Just allocate normal memory on host. */
	return malloc(size);	
}

void* OscSupSramAllocL1Instr(unsigned int size)
{
	if(size > SRAM_INSTR_LENGTH)
		return NULL;
		
	/* Just allocate normal memory on host. */
	return malloc(size);	
}

void* OscSupSramAllocScratch(unsigned int size)
{
	if(size > SRAM_SCRATCH_LENGTH)
		return NULL;
		
	/* Just allocate normal memory on host. */
	return malloc(size);	
}

OSC_ERR OscSupSramFree(void *pAddr)
{
	free(pAddr);
	return SUCCESS;
}


