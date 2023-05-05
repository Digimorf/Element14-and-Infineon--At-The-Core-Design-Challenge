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

// /////////////////////////////////////////////////////////////////////////////
// Includes
// /////////////////////////////////////////////////////////////////////////////
#include "cy_pdl.h"
#include "stdint.h"

#include "ipc_shared_memory.h"

// /////////////////////////////////////////////////////////////////////////////
// Privates
// /////////////////////////////////////////////////////////////////////////////
volatile uint32_t gCmdAddr          = (SHARED_MEMORY);
volatile uint32_t gVBlankAddr       = (SHARED_MEMORY + 4);
volatile uint32_t gVideoBufferAddr  = (SHARED_MEMORY + 8);
volatile uint32_t gAudioBufferAddr  = (SHARED_MEMORY + 12);
volatile uint32_t gIOAddr           = (SHARED_MEMORY + 16);