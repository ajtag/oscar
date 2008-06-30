/*! @file cpld_target.c
 * @brief Cpld module implementation for target.
 * 
 ************************************************************************/

#include "oscar_types_target.h"

#include "cpld_pub.h"
#include "cpld_priv.h"
#include "oscar_intern.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>


struct OSC_CPLD cpld;		/*!< The cpld module singelton instance */

/*! The dependencies of this module. */
struct OSC_DEPENDENCY cpld_deps[] = {
        {"log", OscLogCreate, OscLogDestroy}
};


OSC_ERR OscCpldCreate(void *hFw)
{
#ifdef TARGET_TYPE_INDXCAM
    struct OSC_FRAMEWORK *pFw;
    OSC_ERR err;
    
    pFw = (struct OSC_FRAMEWORK *)hFw;
    if(pFw->cpld.useCnt != 0)
    {
        pFw->cpld.useCnt++;
        /* The module is already allocated */
        return SUCCESS;
    }
    
    /* Load the module cpld_deps of this module. */
    err = OSCLoadDependencies(pFw, 
            cpld_deps, 
            sizeof(cpld_deps)/sizeof(struct OSC_DEPENDENCY));
    if(err != SUCCESS)
    {
        OscLog(ERROR, "%s: Unable to load cpld_deps! (%d)\n",
                __func__, 
                err);
        return err;
    }
        
 	memset(&cpld, 0, sizeof(struct OSC_CPLD));

    /* Open cpld device driver */ 
    cpld.file = fopen( OSC_CPLD_DRIVER_FILE, "rw+");
    if( unlikely( cpld.file == 0))
    {
        OscLog(ERROR, "%s: Unable to open cpld device file %s.\n",
                __func__, 
                OSC_CPLD_DRIVER_FILE);
        return -ENO_CPLD_DEVICE_FOUND;
    }

    /* Map cpld register space as memory device */
    cpld.addr = mmap(NULL, OSC_CPLD_MAX_REGISTER_NR, 
            PROT_READ|PROT_WRITE, MAP_PRIVATE, fileno( cpld.file), 0);    
    if( unlikely( MAP_FAILED == cpld.addr))
    {
        OscLog(ERROR, "%s: Failed to perform mmap operation (errno: %s).\n",
                __func__,
                strerror(errno));
        return -ENO_CPLD_DEVICE_FOUND;        
    }
               
    /* Increment the use count */
    pFw->cpld.hHandle = (void*)&cpld;
    pFw->cpld.useCnt++;
    
    return SUCCESS;
#else
    OscLog(ERROR, "%s: No CPLD available on this hardware platform!\n",
            __func__);
    return -ENO_SUCH_DEVICE;
#endif /* TARGET_TYPE_INDXCAM */
}

void OscCpldDestroy(void *hFw)
{
    struct OSC_FRAMEWORK *pFw;

    pFw = (struct OSC_FRAMEWORK *)hFw; 
    /* Check if we really need to release or whether we still 
     * have users. */
    pFw->cpld.useCnt--;
    if(pFw->cpld.useCnt > 0)
    {
        return;
    }
        
    OscUnloadDependencies(pFw, 
            cpld_deps, 
            sizeof(cpld_deps)/sizeof(struct OSC_DEPENDENCY));
    
    if(cpld.file != NULL)
    {
        fclose(cpld.file);
    }
	memset(&cpld, 0, sizeof(struct OSC_CPLD));
}

#ifdef TARGET_TYPE_INDXCAM
OSC_ERR OscCpldRset( 
        const uint16 regId, 
        const uint8 val)
{
    cpld.addr[ regId] = val;
    cpld.reg[ regId] = val;
    return SUCCESS;
}
#else
OSC_ERR OscCpldRset( 
        const uint16 regId, 
        const uint8 val)
{
    return -ENO_SUCH_DEVICE;
}
#endif /* TARGET_TYPE_INDXCAM */

#ifdef TARGET_TYPE_INDXCAM
OSC_ERR OscCpldFset( 
        uint16 regId, 
        uint8 field, 
        uint8 val)
{
    uint8 current;
    current = cpld.reg[ regId];
    if( val)
    {
        current = current | field;
    }
    else
    {
        current = current & (0xff ^ field);
    }
    cpld.addr[ regId] = current;
    cpld.reg[ regId] = current;
    return SUCCESS;
}
#else
OSC_ERR OscCpldFset( 
        uint16 regId, 
        uint8 field, 
        uint8 val)
{
    return -ENO_SUCH_DEVICE;
}
#endif /* TARGET_TYPE_INDXCAM */

#ifdef TARGET_TYPE_INDXCAM
OSC_ERR OscCpldRget( 
        const uint16 regId,
        uint8* val)
{
    *val = cpld.addr[ regId];
    return SUCCESS;
}
#else
OSC_ERR OscCpldRget( 
        const uint16 regId,
        uint8* val)
{
    return -ENO_SUCH_DEVICE;
}
#endif /* TARGET_TYPE_INDXCAM */

#ifdef TARGET_TYPE_INDXCAM
OSC_ERR OscCpldFget( 
        const uint16 regId, 
        const uint8 field, 
        uint8* val)
{
    uint8 current = cpld.addr[ regId];
    if( current & field)
    {
        *val = 1;
    }
    else
    {
        *val = 0;
    }
    return SUCCESS;
}
#else
OSC_ERR OscCpldFget( 
        const uint16 regId, 
        const uint8 field, 
        uint8* val)
{
    return -ENO_SUCH_DEVICE;
}
#endif /* TARGET_TYPE_INDXCAM */
