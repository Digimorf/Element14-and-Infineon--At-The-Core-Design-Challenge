/*//////////////////////////////////////////////////////////////////////////////
* File Name:   sega_sg-1000.h
*
* Description: Element14 - At The Core Design Challenge
*              Digimorf, SEGA SG-1000 Emulator lite
*
* Created on: Apr 30 2023
*     Author: Francesco De Simone - Digimorf
*
//////////////////////////////////////////////////////////////////////////////*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef SEGA_SG_1000_H_
#define SEGA_SG_1000_H_

// Basic definitions.
#ifndef TRUE
  #define TRUE 1
#endif
#ifndef FALSE
  #define FALSE 0
#endif
#ifndef ON
  #define ON 1
#endif
#ifndef OFF
  #define OFF 0
#endif
#ifndef NULL
  #define NULL 0
#endif

#define WAIT             0
#define EXE_CPU          1
#define EXE_VDP_BK       2
#define EXE_VDP_SPR      3
#define CPY_LINE_BUFFER  4

#define SG1000_EXTERNAL_ROM 0x01
#define SG1000_EXTERNAL_RAM 0x02

#define SG1000_RAM_SIZE     0x800
#define SG1000_RAM_MIRROR   0x7FF
#define SG1000_RAM_START    0xC000
#define SG1000_RAM_END      0xFFFF

#define SG1000_ROM_START    0x0000
#define SG1000_ROM_END      0xC000

void Sega_SG1000_Init(unsigned char *prom, uint32_t pROMSize);
void Sega_SG1000_Start(void);

void Sega_SG1000_Refresh_CPU(uint16_t gMachineLine);
void Sega_SG1000_Refresh_VDP(uint16_t gMachineLine);
void Sega_SG1000_Refresh_PSG(uint16_t gMachineLine);

#endif /* SEGA_SG_1000_H_ */
