/* 
 * File:   KSPDisplayAnalogExt.h
 * Author: CraigVella
 *
 * Created on August 15, 2013, 5:11 PM
 */

#ifndef KSPDISPLAYANALOGEXT_H
#define	KSPDISPLAYANALOGEXT_H

#include "ConfigBits.h"
#define BFWAIT() while (!SSPSTATbits.BF)

void initializeModule();

#endif	/* KSPDISPLAYANALOGEXT_H */

