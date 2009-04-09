/*	
	JPEG encoder library
	Original Implementation by Nitin Gupta
	Adapted to leanXcam by Reto Baettig
	
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

/*! @file jpg_pub.h
 * @brief API definition for jpeg module
 * 
 */
#ifndef JPG_PUB_H_
#define JPG_PUB_H_

#include "oscar_error.h"
#ifdef OSC_HOST
	#include "oscar_types_host.h"
	#include "oscar_host.h"
#else
	#include "oscar_types_target.h"
	#include "oscar_target.h"
#endif /* OSC_HOST */



/*====================== API functions =================================*/

/*********************************************************************//*!
 * @brief Constructor
 * 
 * @param hFw Pointer to the handle of the framework.
 * @return SUCCESS or an appropriate error code otherwise
 *//*********************************************************************/
OSC_ERR OscJpgCreate(void *hFw);

/*********************************************************************//*!
 * @brief Destructor
 * 
 * @param hFw Pointer to the handle of the framework.
 *//*********************************************************************/
void OscJpgDestroy(void *hFw);

/*********************************************************************//*!
 * @brief Encode a bitmap image to a JPEG file
 * 
 * @param pic Pointer to the image
 * @param output_ptr Pointer to the memory where the JPEG output will be stored
 * @param quality_factor 1024 means heavy compression
 * @return Pointer to the end of the data in the JPEG output buffer
 *//*********************************************************************/
uint8* OscJpgEncode(struct OSC_PICTURE *pic, uint8 *output_ptr, uint32 quality_factor);

#endif /*JPG_PUB_H_*/