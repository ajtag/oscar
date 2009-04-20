/*	Oscar, a hardware abstraction framework for the LeanXcam and IndXcam.
	Copyright (C) 2008 Supercomputing Systems AG
	
	This library is free software; you can redistribute it and/or modify it
	under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation; either version 2.1 of the License, or (at
	your option) any later version.
	
	This library is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
	General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with this library; if not, write to the Free Software Foundation,
	Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*! @file swr_priv.h
 * @brief Private stimuli writer module definition
 * 
	************************************************************************/
#ifndef SWR_PRIV_H_
#define SWR_PRIV_H_

#include <string.h>
#include <stdio.h>

#include <log/log_pub.h>

#ifdef OSC_HOST
#include <oscar_types_host.h>
#else
#include <oscar_types_target.h>
#endif /* OSC_HOST */


#if defined(OSC_HOST) || defined(OSC_SIM)
/*! @brief Limited number of writer instances */
#define MAX_NR_WRITER               10
/*! @brief Limited number of signal instances per writer */
#define MAX_NR_SIGNAL_PER_WRITER    20

/*! @brief Limited string length for string type value */
#define MAX_LENGTH_STRING_VALUE    200

/*!@brief Union for signal value (function argument)*/
union uOSC_SWR_VALUE
{
	int32 nValue;    /*!< @brief Integer coded value */
	float fValue;    /*!< @brief Float coded value */
	char strValue[MAX_LENGTH_STRING_VALUE]; /*!< @brief String coded value */
};

/*!@brief Signal object struct */
struct OSC_SWR_SIGNAL
{
	char* strName;  /*!< @brief Signal name for stimuli file report*/
	enum EnOscSwrSignalType type; /*!< @brief Signal value type*/
	union uOSC_SWR_VALUE value; /*!< @brief Signal value */
	char strFormat[20]; /*!< @brief logging format string */
};

/*!@brief Writer object struct */
struct OSC_SWR_WRITER
{
	FILE* pFile;              /*!< @brief Handle to writer file */
	char strFile[64];
	uint16 nrOfSignals;       /*!< @brief Number of managed signals */
	bool bDescriptorPrinted;  /*!< @brief File descripter line already printed */
	bool bReportTime;
	bool bReportCyclic;
	/*! @brief Signal instance array*/
	struct OSC_SWR_SIGNAL sig[ MAX_NR_SIGNAL_PER_WRITER];
};
#endif /* OSC_HOST or OSC_SIM*/

/*!@brief Stimuli writer module object struct */
struct OSC_SWR
{
	uint16 nrOfWriters;     /*!< @brief Number of managed writers */
	/*! @brief Writer instance array */
	#if defined(OSC_HOST) || defined(OSC_SIM)
	/*! @brief Array of managed writers */
	struct OSC_SWR_WRITER wr[ MAX_NR_WRITER];
	#endif
};

#if defined(OSC_HOST) || defined(OSC_SIM)
/*! @brief Fuction */
void OscSwrCycleCallback( void);
void OscSwrReport( const void* pWriter);
#endif /*OSC_HOST or OSC_SIM*/


#endif /* SWR_PRIV_H_ */