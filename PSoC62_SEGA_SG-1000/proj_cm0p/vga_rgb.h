/*//////////////////////////////////////////////////////////////////////////////
* File Name:   VGA_rgb.h
*
* Description: Element14 - At The Core Design Challenge
*              Digimorf, SEGA SG-1000 Emulator lite
*              VGA RGB output driver for PSoC62S4 Pioneer kit
*
* Created on: 20 Mar 2023
*     Author: Francesco De Simone - Digimorf
*
//////////////////////////////////////////////////////////////////////////////*/

#ifndef VGA_RGB_H_
#define VGA_RGB_H_

// /////////////////////////////////////////////////////////////////////////////
// Includes
// /////////////////////////////////////////////////////////////////////////////
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////

// Definitions from the "ModusToolbox Device Configurator"
#define scanline_hsync_pwm_HW       tcpwm_0_group_1_cnt_4_HW
#define scanline_hsync_pwm_NUM      tcpwm_0_group_1_cnt_4_NUM
#define scanline_hsync_pwm_IRQ      tcpwm_0_group_1_cnt_4_IRQ
#define scanline_hsync_pwm_config   tcpwm_0_group_1_cnt_4_config
#if 1
#define scanline_pixel_clock_HW       tcpwm_0_group_1_cnt_0_HW
#define scanline_pixel_clock_NUM      tcpwm_0_group_1_cnt_0_NUM
#define scanline_pixel_clock_IRQ      tcpwm_0_group_1_cnt_0_IRQ
#define scanline_pixel_clock_config   tcpwm_0_group_1_cnt_0_config

#define scanline_pixel_dma_HW                   cpuss_0_dmac_0_chan_0_HW
#define scanline_pixel_dma_descriptor_line      cpuss_0_dmac_0_chan_0_Descriptor_0
#define scanline_pixel_dma_descriptor_line_cfg  cpuss_0_dmac_0_chan_0_Descriptor_0_config

#define scanline_pixel_dma_CH             cpuss_0_dmac_0_chan_0_CHANNEL
#define scanline_pixel_dma_CH_cfg         cpuss_0_dmac_0_chan_0_channelConfig
#define scanline_pixel_dma_IRQ            cpuss_0_dmac_0_chan_0_IRQ
#endif

#define VGA_RGB_W         256
#define VGA_RGB_H         192

#define VGA_BLANK 0

#define VGA_VSYNC_START   10
#define VGA_VSYNC_END     12

#define VGA_FRAME_START   0
#define VGA_FRAME_END     524

#define VGA_LINE_HEIGHT   2
#define VGA_ACTIVE_HEIGHT (192 * VGA_LINE_HEIGHT)

#define VGA_TOP           48
#define VGA_ACTIVE_START  (VGA_VSYNC_END + 33 + VGA_TOP)
#define VGA_ACTIVE_END    (VGA_ACTIVE_START + VGA_ACTIVE_HEIGHT)
#define VGA_VBLANK        VGA_ACTIVE_END

#define DMA_SCANLINE      256

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////
typedef struct
{
  uint16_t scanline_active;
  volatile uint8_t *buffer_ptr;
  uint16_t width;
  uint16_t height;
  uint16_t top;
  uint8_t background;

} resolution_t;

// /////////////////////////////////////////////////////////////////////////////
// Exported
// /////////////////////////////////////////////////////////////////////////////
extern volatile uint8_t gRenderScanline, gScanlineSub;
extern volatile uint8_t gFrameBuffer[257*192];
extern volatile uint8_t gVBlankPtr;
extern volatile uint8_t gVBlank;
extern volatile uint16_t gVGAScanline;

// /////////////////////////////////////////////////////////////////////////////
// Function prototypes
// /////////////////////////////////////////////////////////////////////////////
void RGB_VGA_Init(void);

// -----------------------------------------------------------------------------
void RGB_VGA_Start(void);

// /////////////////////////////////////////////////////////////////////////////
#endif /* VGA_RGB_H_ */
