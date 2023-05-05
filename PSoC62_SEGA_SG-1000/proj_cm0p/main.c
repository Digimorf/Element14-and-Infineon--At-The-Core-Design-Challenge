/*//////////////////////////////////////////////////////////////////////////////
* File Name:   main.c
*
* Description: Element14 - At The Core Design Challenge
*              Digimorf, SEGA SG-1000 Emulator lite
*              Core CM0+
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

#include "utilities.h"
#include "ansi_escape.h"

#include "ipc_shared_memory.h"
#include "vga_rgb.h"
#include "audio_dac.h"
#include "capsense_joypad.h"

// /////////////////////////////////////////////////////////////////////////////
// Main CM0+
// /////////////////////////////////////////////////////////////////////////////
int main(void)
{
    cy_rslt_t result;

    __enable_irq();

    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR);

    RGB_VGA_Init();
    Audio_Init();

    capsense_io_init();

    RGB_VGA_Start();

    for (;;)
    {
      while (gVBlank == 0) {};
      capsense_io_check();
      while (gVBlank == 1) {};
    }
}

/* [] END OF FILE */
