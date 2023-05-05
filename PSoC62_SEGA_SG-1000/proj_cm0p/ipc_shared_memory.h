/*//////////////////////////////////////////////////////////////////////////////
* File Name:   ipc_shared_memory.c
*
* Description: Element14 - At The Core Design Challenge
*              Digimorf, SEGA SG-1000 Emulator lite
*              Memory mapped registers
*
* Created on: Mar 27 2023
*     Author: Francesco De Simone - Digimorf
*
//////////////////////////////////////////////////////////////////////////////*/

#ifndef _IPC_SHARED_MEMORY_H_
#define _IPC_SHARED_MEMORY_H_

// /////////////////////////////////////////////////////////////////////////////
// Includes
// /////////////////////////////////////////////////////////////////////////////
#include "cy_pdl.h"
#include "stdint.h"

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////
#define SHARED_MEMORY 0x0801F000

#define _SHARED_gCommand *((uint8_t*)(*((uint32_t*)gCmdAddr)))
#define _SHARED_gVBlank *((uint8_t*)(*((uint32_t*)gVBlankAddr)))
#define _SHARED_gIOPort *((uint8_t*)(*((uint32_t*)gIOAddr)))
#define _SHARED_gSemaPort *((uint8_t*)(*((uint32_t*)gSemaAddr)))

// /////////////////////////////////////////////////////////////////////////////
// Exported
// /////////////////////////////////////////////////////////////////////////////
extern volatile uint32_t gCmdAddr;
extern volatile uint32_t gVBlankAddr;
extern volatile uint32_t gVideoBufferAddr;
extern volatile uint32_t gAudioBufferAddr;
extern volatile uint32_t gIOAddr;

#endif /* _IPC_SHARED_MEMORY_H_ */