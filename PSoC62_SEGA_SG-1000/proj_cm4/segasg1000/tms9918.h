/*//////////////////////////////////////////////////////////////////////////////
* File Name:   tms9918.h
*
* Description: Element14 - At The Core Design Challenge
*              Digimorf, SEGA SG-1000 Emulator lite
*
* Created on: Apr 30 2023
*     Author: Francesco De Simone - Digimorf
*
//////////////////////////////////////////////////////////////////////////////*/

#ifndef _TMS9918_H_
#define _TMS9918_H_

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// /////////////////////////////////////////////////////////////////////////////
// Define
// /////////////////////////////////////////////////////////////////////////////
#define TMS9918_VRAM_SIZE               0x4000

#define TMS9918_VIDEO_WIDTH             256
#define TMS9918_VIDEO_WIDTH_TILE        (TMS9918_VIDEO_WIDTH << 3)
#define TMS9918_VIDEO_HEIGHT            192
#define TMS9918_NUMBER_SPRITES          32
#define TMS9918_INLINE_SPRITE_MAX       4

#define TMS9918_CONTROL_REGISTERS       8
#define TMS9918_CONTROL_REGISTERS_MASK  8

#define TMS9918_COLORS                  16
#define TMS9918_GFXMODES_COLORS         32
#define TMS9918_COLORS_SHADE            4
#define TMS9918_COLOR_FOREGROUND        0
#define TMS9918_COLOR_BACKGROUND        1
#define TMS9918_COLOR_BORDER            2

#define TMS9918_VIDEO_MODES             4
#define SCREEN_MODE_GFX1                1
#define SCREEN_MODE_GFX2                2
#define SCREEN_MODE_MULTICOLOR          3
#define SCREEN_MODE_TEXT                0

// Status Register Masks
#define _VBLANK  0x80 // 1: VBlank has occured
#define _HINT    0x04 // Line interrupt Pending
#define _5THSPR  0x40 // 1: 5th sprite detected
#define _COINC   0x20 // 1: Sprites overlap
#define _5THNUM  0x1F // Number of the 5th sprite

// Register 1 masks
#define _SCRENA  0x40 // 1: Enable display
#define _IRQ     0x20 // 1: IRQs on VBlanks
#define _SIZE    0x02 // 1: 16x16 sprites (0=8x8)
#define _MAG     0x01 // 1: Magnify sprites x2

// VDP interrupt modes
#define INT_IE0      0x01
#define INT_IE1      0x02
#define INT_IE2      0x04

 #define MY_SEMA_NUM (3UL)

//#define FRAME_BUFFER_PTR ((uint8_t*)(*((uint32_t*)gRenderBufferAddr)))

// ////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Exported globals
// /////////////////////////////////////////////////////////////////////////////
extern unsigned char Z80InterruptLine;
// /////////////////////////////////////////////////////////////////////////////
// Exported functions
// /////////////////////////////////////////////////////////////////////////////
void TMS9918_Reset(void);
void TMS9918_Data_Write(register uint8_t pData);
uint8_t TMS9918_Data_Read(void);
uint8_t TMS9918_Register_Read(void);
uint8_t TMS9918_Register_Write(register uint8_t pValue);
void TMS9918_Background(uint16_t lScanline);
void TMS9918_GFX2_Mode(uint16_t lScanline);
void TMS9918_Sprites_GFX12(uint16_t lScanline);
uint8_t TMS9918_Check_VBlank(uint16_t lScanline);
void TMS9918_Sprites(uint16_t lScanline);
void TMS9918_Refresh (uint16_t lScanline);
void TMS9918_Init (void);

#endif // _TMS9918_H_
