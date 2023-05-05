/*//////////////////////////////////////////////////////////////////////////////
* File Name:   sega_sg-1000.c
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

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include "ipc_shared_memory.h"

#include "sega_sg-1000.h"
#include "tms9918.h"
#include "SK1100.h"
#include "sn76489.h"
#include "Z80/Z80.h"

// /////////////////////////////////////////////////////////////////////////////
// Extern
// /////////////////////////////////////////////////////////////////////////////
extern void *WrZ80_Fn_Ptr, *RdZ80_Fn_Ptr, *OutZ80_Fn_Ptr, *InZ80_Fn_Ptr;

Z80 gCPU;

extern uint8_t Z80InterruptLine;

uint32_t gMachineFrame = 0;
uint16_t gMachineLine = 0;
int16_t gCPUCyclesLeft = 0;
uint8_t gMachineRAM[SG1000_RAM_SIZE], *gMachineROM = NULL;
uint32_t gMachineRAMSize = SG1000_RAM_SIZE, gMachineROMSize = 0;

// /////////////////////////////////////////////////////////////////////////////
// Machine globals
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////
void Sega_SG1000_Memory_Write(word Addr, byte Value)
{
  if (Addr >= SG1000_RAM_START)
    gMachineRAM[(Addr - SG1000_RAM_START) & SG1000_RAM_MIRROR] = Value;

} // End Sega_SG1000_Memory_Write.

// /////////////////////////////////////////////////////////////////////////////
byte Sega_SG1000_Memory_Read(word Addr)
{
  uint8_t lByte = 0;

  if (Addr < SG1000_ROM_END)
    lByte = gMachineROM[Addr];
  else if (Addr >= SG1000_RAM_START)
    lByte = gMachineRAM[(Addr - SG1000_RAM_START) & SG1000_RAM_MIRROR];

  return lByte;

} // End Sega_SG1000_Memory_Read.

// /////////////////////////////////////////////////////////////////////////////
void Sega_SG1000_Port_Out(word Port, byte Value)
{
  /*
   000H - 03FH: unused
   040H - 07FH: PSG SN 76489, using port 07FH
   080H - 0BFH: Use VDP TMS9918A, port 0 BEH - 0 BFH
   0C0H - 0DFH: Use keyboard + JOY terminal, port 0DCH - 0DFH
   0E0H - 0FFH: SF - 7000 for expansion
   */

  if ((Port & 0xFF) >= 0x40)
  {
    if ((Port & 0xFF) < 0x80)
    {
      SN76489_Write(&gPSG, Value);
    }
    else if ((Port & 0xFF) < 0xC0)
    {
      if (Port & 0x1)
      {
        TMS9918_Register_Write(Value);
      }
      else
        TMS9918_Data_Write(Value);
    }
    else if ((Port & 0xFF) < 0xE0)
    {
      if ((Port & 0x1) == 0)
      {
        SK1100_Write_PortC(Value & 0x7);
      }
    } // End if chain.

  } // End if.

} // End Sega_SG1000Port_Out.

// /////////////////////////////////////////////////////////////////////////////
byte Sega_SG1000_Port_In(word Port)
{
  /*
  000H - 03FH: unused
  040H - 07FH: PSG SN 76489, using port 07FH
  080H - 0BFH: Use VDP TMS9918A, port 0 BEH - 0 BFH
  0C0H - 0DFH: Use keyboard + JOY terminal, port 0DCH - 0DFH
  0E0H - 0FFH: SF - 7000 for expansion
  */

  byte lByte = 0xFF;

  if ((Port & 0xFF) >= 0x7F)
  {
    if ((Port & 0xFF) < 0xC0)
    {
      if (Port & 0x1)
        lByte = TMS9918_Register_Read();
      else
        lByte = TMS9918_Data_Read();
    }
    else if ((Port & 0xFF) < 0xE0)
    {
      if (Port & 0x1)
      {
        lByte = SK1100_Read_PortB();
      }
      else
      {
        lByte = SK1100_Read_PortA();

      }  // End if.

    } // End if.

  } // End if.

  return lByte;
} // End Sega_SG1000_Port_In.

// /////////////////////////////////////////////////////////////////////////////
void Sega_SG1000_Init(unsigned char *prom, uint32_t pROMSize)
{
  // Memory
  gMachineROMSize = pROMSize * sizeof(uint8_t);
  gMachineROM = prom;

  // Z80 settings -------------------------
  WrZ80_Fn_Ptr = &Sega_SG1000_Memory_Write;
  RdZ80_Fn_Ptr = &Sega_SG1000_Memory_Read;
  OutZ80_Fn_Ptr = &Sega_SG1000_Port_Out;
  InZ80_Fn_Ptr = &Sega_SG1000_Port_In;
  ResetZ80(&gCPU);
  gCPU.IAutoReset = FALSE;

  // Joy and Kbd settings -----------------
  SK1100_Init();

  // PSG settings -------------------------
  SN76489_Init(&gPSG, 3579545, 31500, 60);

  // VDP init -----------------------------
  TMS9918_Init();

} // End Sega_SG1000_Init.

// /////////////////////////////////////////////////////////////////////////////
inline void Sega_SG1000_Refresh_PSG(uint16_t gMachineLine)
{
  SN76489_Render(&gPSG, gMachineLine);

} // End.

// /////////////////////////////////////////////////////////////////////////////
inline void Sega_SG1000_Refresh_CPU(uint16_t gMachineLine)
{
  // ------------------
  ExecZ80(&gCPU, 228);
  if (Z80InterruptLine)
  {
    IntZ80(&gCPU, INT_IRQ);
    gCPU.IRequest = INT_IRQ;
  }
  else
    gCPU.IRequest = INT_NONE;
  // ------------------
  TMS9918_Check_VBlank(gMachineLine);

} // End.

// /////////////////////////////////////////////////////////////////////////////
