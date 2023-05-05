/*//////////////////////////////////////////////////////////////////////////////
* File Name:   main.c
*
* Description: Element14 - At The Core Design Challenge
*              Digimorf, SEGA SG-1000 Emulator lite
*              Core CM4
*
* Created on: May 05 2023
*     Author: Francesco De Simone - Digimorf
*
//////////////////////////////////////////////////////////////////////////////*/

// /////////////////////////////////////////////////////////////////////////////
// Includes
// /////////////////////////////////////////////////////////////////////////////
#include "cy_pdl.h"
#include "cycfg.h"
#include "cybsp.h"

#include "ipc_shared_memory.h"

#include "segasg1000/sega_sg-1000.h"
#include "segasg1000/tms9918.h"

extern const unsigned char gSpaceInvaders[16384];

// /////////////////////////////////////////////////////////////////////////////
// Main CM4
// /////////////////////////////////////////////////////////////////////////////
int main(void)
{
  __enable_irq();

  uint16_t lScanline = 0;

  cy_rslt_t result;
  result = cybsp_init() ;
  if (result != CY_RSLT_SUCCESS) CY_ASSERT(0);

  Sega_SG1000_Init((uint8_t*)(gSpaceInvaders), 16384);

  _SHARED_gCommand = 255;

  for (;;)
  {
    while (_SHARED_gCommand == 255) {};
    if(_SHARED_gCommand == 192)
    {
      for (lScanline = 0; lScanline < 262; lScanline++)
      {
        Sega_SG1000_Refresh_CPU(lScanline);
        Sega_SG1000_Refresh_PSG(lScanline);
      };
      _SHARED_gCommand = 255;
    }
    else
    {
      TMS9918_GFX2_Mode(_SHARED_gCommand);
      TMS9918_Sprites_GFX12(_SHARED_gCommand);

      _SHARED_gCommand = 255;
    }
  }
}

/* [] END OF FILE */
