/*//////////////////////////////////////////////////////////////////////////////
* File Name:   VGA_rgb.c
*
* Description: Element14 - At The Core Design Challenge
*              Digimorf, SEGA SG-1000 Emulator lite
*              VGA RGB output driver for PSoC62S4 Pioneer kit
*
* Created on: 20 Mar 2023
*     Author: Francesco De Simone - Digimorf
*
//////////////////////////////////////////////////////////////////////////////*/

// /////////////////////////////////////////////////////////////////////////////
// Includes
// /////////////////////////////////////////////////////////////////////////////
#include "cy_pdl.h"

#include "ipc_shared_memory.h"

#include "vga_rgb.h"
#include "audio_dac.h"

// /////////////////////////////////////////////////////////////////////////////
// Private variables
// /////////////////////////////////////////////////////////////////////////////

volatile uint16_t gVGAScanline = 0;
volatile uint8_t gScanlineSub = 0;

GPIO_PRT_Type* portAddr = GPIO_PRT2;
GPIO_PRT_Type* portDbgAddr = GPIO_PRT10;
volatile uint8_t *lBuffPtr;
uint8_t *gVGAScanline_RGB_Port;
uint8_t gBlankVGA = VGA_BLANK;
int16_t gMachineLine = 0;

resolution_t gVideoMode;

volatile uint8_t gFrameBuffer[257*192];
volatile uint8_t *gFrontBuffer, *gBackBuffer;
volatile uint8_t gVBlank = 0;
volatile uint8_t gCmd = 0;
volatile uint8_t gSemaphore = 0;

// /////////////////////////////////////////////////////////////////////////////
// Function prototypes
// /////////////////////////////////////////////////////////////////////////////

void NvicMux0_IRQHandler(void);

// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////

void RGB_VGA_Init(void)
{
  /*
  * DESCRIPTION: Initialise the VGA Video driver by configuring and starting
  *              basic components such as TCPWMs and DMA.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing
  */

  gVideoMode.height = VGA_RGB_H * 2;
  gVideoMode.width = VGA_RGB_W;
  gVideoMode.top = (480 - (VGA_RGB_H * 2)) / 2;
  gVideoMode.background = VGA_BLANK;

#if 0
  gFrontBuffer = &gFrameBuffer[0];
  gBackBuffer = &gFrameBuffer[257 * 96];
  *((uint32_t*)gVideoBufferAddr) = (uint32_t)(gBackBuffer); // Shared location
#else
  *((uint32_t*)gVideoBufferAddr) = (uint32_t)(gFrameBuffer); // Shared location
#endif

  *((uint32_t*)gCmdAddr)    = (uint32_t)(&gCmd);            // Shared location
  *((uint32_t*)gVBlankAddr) = (uint32_t)(&gVBlank);         // Shared location

  gScanlineSub = 0;
  gMachineLine = 0;
  gVBlank = 0;

  memset(gFrameBuffer, VGA_BLANK, 257*192);

  /* -------------------------------------------------------------------------*/
  /* Assign GPIO port for the output to the resistor DAC, see HARDWARE.txt */
  gVGAScanline_RGB_Port = (uint8_t *)(&GPIO_PRT2->OUT);

  /* ---------------------------------------------------------------------------
     HSync signal for VGA standard video signal
     ---------------------------------------------------------------------------

     Horizontal :

          :     ________________  :
          :    |                | :
      ____:____| Video signal   |_:_______
          :                       :
      ____:  _____________________:  _____
      PWM :_|                     :_|
          : 0  1 CCn              :TC

      CC0   381  HSync pulse width
      CC1   512  CC0 + Back Porch
      TC   3178  Whole scan line
                 CC1 = RGB data starts

   -------------------------------------------------------------------------- */

  /* Initialisation of TCPWM channel */
  if (CY_TCPWM_SUCCESS != Cy_TCPWM_PWM_Init(scanline_hsync_pwm_HW,
                                            scanline_hsync_pwm_NUM,
                                            &scanline_hsync_pwm_config)) {}

  /* Enable the initialised PWM */
  Cy_TCPWM_PWM_Enable(scanline_hsync_pwm_HW,
                      scanline_hsync_pwm_NUM);

  cy_stc_sysint_t HSyncInt_cfg = {
      .intrSrc = NvicMux0_IRQn,
      .cm0pSrc = scanline_hsync_pwm_IRQ,
      .intrPriority = 0
  };

  Cy_SysInt_Init(&HSyncInt_cfg, (cy_israddress)NvicMux0_IRQHandler);

  NVIC_ClearPendingIRQ(HSyncInt_cfg.intrSrc);
  NVIC_EnableIRQ(HSyncInt_cfg.intrSrc);

  /* ---------------------------------------------------------------------------
     Pixel CLock signal for streaming pixels through DMA
     ---------------------------------------------------------------------------

     PWM TC trigger out -----------__----------------__---> Trig DMA transfer
                                __|n |            __|n |
                             __|n-1  |         __|n-1  |
                          __|        |      __|        |
                       __|1          |   __|1          |
     TCPWM Counter..__|0             |__|0             |__..
                    :                :                 :
                    :< pixel period >:< pixel period >:

     -------------------------------------------------------------------------*/

  /* Initialization of TCPWM channel */
  if (CY_TCPWM_SUCCESS != Cy_TCPWM_PWM_Init(scanline_pixel_clock_HW,
                                            scanline_pixel_clock_NUM,
                                           &scanline_pixel_clock_config)) {}

  /* Enable the initialized PWM */
  Cy_TCPWM_PWM_Enable(scanline_pixel_clock_HW,
                      scanline_pixel_clock_NUM);

  // ---------------------------------------------------------------------------
  Cy_DMAC_Descriptor_Init(&scanline_pixel_dma_descriptor_line,
                          &scanline_pixel_dma_descriptor_line_cfg);
  Cy_DMAC_Descriptor_SetSrcAddress(&scanline_pixel_dma_descriptor_line,
                                   (uint8_t*)(gFrameBuffer));
  Cy_DMAC_Descriptor_SetDstAddress(&scanline_pixel_dma_descriptor_line,
                                   (uint8_t*)(gVGAScanline_RGB_Port));

  Cy_DMAC_Descriptor_SetXloopDataCount(&scanline_pixel_dma_descriptor_line,
                                       DMA_SCANLINE + 1);

  Cy_DMAC_Channel_Init(scanline_pixel_dma_HW,
                       scanline_pixel_dma_CH,
                       &scanline_pixel_dma_CH_cfg);

  Cy_DMAC_Channel_SetDescriptor(scanline_pixel_dma_HW,
                                scanline_pixel_dma_CH,
                                &scanline_pixel_dma_descriptor_line);

} // End RGB_VGA_Init.

// -----------------------------------------------------------------------------
void RGB_VGA_Start(void)
{
  /*
  * DESCRIPTION: Starts the VGA driver by starting the main HSync signal.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing
  */

  /* Then start the HSync scan line driver */
  Cy_TCPWM_TriggerStart_Single(scanline_hsync_pwm_HW,
                               scanline_hsync_pwm_NUM);

} // End RGB_VGA_Start.

// -----------------------------------------------------------------------------
void NvicMux0_IRQHandler(void)
{
  /*
  * DESCRIPTION: This is the ISR function called every scan line to handle the
  *              scan lines counting system, the VSync signal, and the streaming
  *              of pixels to the GPIO port.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing
  */
  uint16_t *tmpPtr;

  /* Get all the enabled pending interrupts */
  uint32_t interrupts =
      Cy_TCPWM_GetInterruptStatusMasked(scanline_hsync_pwm_HW,
                                        scanline_hsync_pwm_NUM);

  /* End HSync negative pulse */
  if (CY_TCPWM_INT_ON_TC & interrupts)
  {
    Cy_CTDAC_SetValue(audio_dac_HW, gAudioBufferFront[gVGAScanline]);

    if (gVGAScanline == 0)
    {
      tmpPtr = gAudioBufferFront;
      gAudioBufferFront = gAudioBufferBack;
      gAudioBufferBack = tmpPtr;

      *((uint32_t*)gAudioBufferAddr) = (uint32_t)gAudioBufferBack; // Shared location
    }

    if ((gVGAScanline >= VGA_ACTIVE_START) && (gVGAScanline < VGA_ACTIVE_END))
    {
      _SHARED_gVBlank = 0;

      if (gScanlineSub == 0)
      {
        /* reset the DMA transfer */
        Cy_DMAC_Descriptor_SetSrcAddress(&scanline_pixel_dma_descriptor_line,
                                         (uint8_t*)(gFrameBuffer + (gMachineLine * (DMA_SCANLINE + 1))));

        Cy_DMAC_Channel_SetDescriptor(scanline_pixel_dma_HW,
                                      scanline_pixel_dma_CH,
                                      &scanline_pixel_dma_descriptor_line);
      }

      gScanlineSub++;
      if (gScanlineSub == VGA_LINE_HEIGHT)
      {
        gScanlineSub = 0;
        gMachineLine++;
      }
    }
    else switch (gVGAScanline)
    {
      /* VSync handler */
      case VGA_ACTIVE_END:
        _SHARED_gVBlank = 1;
        _SHARED_gCommand = 192;

        gScanlineSub = 0;
        gMachineLine = 0;
        break;
      case VGA_VSYNC_START: Cy_GPIO_Clr(GPIO_PRT9, 1); break;
      case VGA_VSYNC_END: Cy_GPIO_Set(GPIO_PRT9, 1); break;
    }

    gVGAScanline++;
    if (gVGAScanline > VGA_FRAME_END)
      gVGAScanline = VGA_FRAME_START;
  }
  else
  /* After the back porch starts the Pixel Clock that triggers the DMA transfer */
  if (CY_TCPWM_INT_ON_CC0 & interrupts)
  {
    if ((gVGAScanline >= VGA_ACTIVE_START) && (gVGAScanline < VGA_ACTIVE_END))
    {
      if (gScanlineSub == 1)
        _SHARED_gCommand = gMachineLine;
    }
  }

  /* After the back porch starts the Pixel Clock that triggers the DMA transfer */
  if (CY_TCPWM_INT_ON_CC1 & interrupts)
  {
    if ((gVGAScanline >= VGA_ACTIVE_START) && (gVGAScanline <= VGA_ACTIVE_END))
    {
      if (gScanlineSub == 0)
      {
        Cy_DMAC_Channel_Enable(scanline_pixel_dma_HW, scanline_pixel_dma_CH);
        Cy_DMAC_Enable(scanline_pixel_dma_HW);
      }
    }
  }

  /* Clear the interrupt */
  Cy_TCPWM_ClearInterrupt(scanline_hsync_pwm_HW,
                          scanline_hsync_pwm_NUM,
                          interrupts);

} // End NvicMux0_IRQHandler.
