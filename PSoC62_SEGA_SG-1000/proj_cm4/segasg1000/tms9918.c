/*//////////////////////////////////////////////////////////////////////////////
* File Name:   tms9918.c
*
* Description: Element14 - At The Core Design Challenge
*              Digimorf, SEGA SG-1000 Emulator lite
*
* Created on: Apr 30 2023
*     Author: Francesco De Simone - Digimorf
*
//////////////////////////////////////////////////////////////////////////////*/

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

#include "ipc_shared_memory.h"

#include "tms9918.h"

// /////////////////////////////////////////////////////////////////////////////
// Externals
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Globals
// /////////////////////////////////////////////////////////////////////////////
// Corresponding colors of the TMS9918 in the palette R3G3B2
uint8_t gPalette[16] =
{
  0x00,
  0x00,
  0x55,
  0x79,
  0x4a,
  0x8f,
  0xa8,
  0x5b,
  0xcd,
  0xf1,
  0xd9,
  0xda,
  0x30,
  0xae,
  0xdf,
  0xff
};
uint8_t *lTempPtr = NULL, *lFrameBufferPtrM = NULL,  *lFrameBufferPtrL = NULL, *lFrameBufferToPtr = NULL;
uint8_t TMS9918_vram[TMS9918_VRAM_SIZE];
uint8_t Z80InterruptLine = 0;
uint8_t  gTMS9918_ReadAhead = 0;
uint16_t gTMS9918_VRAM_Address = 0;
uint8_t  gTMS9918_VRAM_WriteMode = 0;

int8_t   gTMS9918_Latched_Byte = 0;
uint8_t  gTMS9918_FirstByte = 1;
uint8_t  gTMS9918_Status_r = 0;
uint8_t  TMS9918_control_r[TMS9918_CONTROL_REGISTERS];
uint8_t  gVDPControlRMask[TMS9918_VIDEO_MODES][TMS9918_CONTROL_REGISTERS_MASK] =
{
    // Name Color Patt SAT SprPa
    // 0 1 2 3 4 5 6 7
    {0x00, 0x00, 0x0F, 0xFF, 0x07, 0x7F, 0x07, 0x00}
    ,// Txt
    {0x00, 0x00, 0x0F, 0xFF, 0x07, 0x7F, 0x07, 0x00}
    ,// GFX 1
    {0x00, 0x00, 0x0F, 0x80, 0x04, 0x7F, 0x07, 0x00}
    ,// GFX 2
    {0x00, 0x00, 0x0F, 0xFF, 0x07, 0x7F, 0x07, 0x00}// Multicolor
};
uint8_t gVideoModeColors[3];

uint8_t *TMS9918_table_name = NULL;
uint8_t *TMS9918_table_colors = NULL;
uint8_t *TMS9918_table_pattern = NULL;
uint8_t *TMS9918_table_sprites_attrtibute = NULL;
uint8_t *TMS9918_table_sprites_pattern = NULL;

uint8_t TMS9918_screen_mode = 0, TMS9918_screen_mode_old = 0;

// Misc helper variables.
uint8_t gShiftX, gSpritesMag, gSpritesSize, gSpritesIsZoomed, gSpritesIs8x16;
uint8_t gSpriteDimension, gSpriteDimension2, gSpritesRealSize;
uint8_t gScreenEnable, gInterruptEnable;

#if 0
uint8_t gUseSecondPattern;

uint8_t gScrollHorizontal, gHorizontalStartColumn, gHorizontalFineScroll;
uint8_t gCommand;
uint8_t gRed, gGreen, gBlue;
uint8_t gVerticalScrollLatch, gCurrentRow;
uint16_t gLineInterruptCounter, gCurrentMultiplier;
uint16_t gActiveAreaScanline = 0;
uint8_t gValue = 0;
uint8_t lRGBColor;

// Variables used during the rendering of video mode TEXT.
uint16_t lStartBuffer, lPixelBackground, lPixelSprite;

uint16_t lVideoTileRow, lFromTileNumber, lToTileNumber, lTileDelta, lTileHoriz;
uint8_t lPatternByte, lBit, lFG, lBG;

uint16_t gCounter = 0, bg, fg, bits;
uint32_t lTempVDPColors8x4, lTempPixelPattern;

uint8_t *lFromTileNumber_ptr, *lToTileNumber_ptr;

// Added for video mode GFX1.
uint8_t lColorByte;

// Added for video mode GFX2.
uint16_t lThird, lTileByte;

#endif

uint8_t gSpritesToRender = 0;
uint8_t TMS9918_number_sprites = 32;
uint8_t TMS9918_inline_sprite_max = 8;

// Added for sprites in GFX modes 1 and 2
uint8_t lCurrentSprite, lInlineSprites;
int16_t lSpriteY, lSpriteX;
uint16_t lSpriteAddress;
uint8_t lSpriteShape, lSpriteColor, lSpriteColorIndex, lSpriteEarlyClock, lBitX, lSpriteDelta,
        lSpriteDeltaStep;
uint16_t lBytePtr, lSpriteByte;
uint16_t lByteMask;

// /////////////////////////////////////////////////////////////////////////////
#if 0
void TMS9918_GFX1_Mode(uint16_t lScanline);
void TMS9918_TEXT_Mode(uint16_t lScanline);
#endif

void TMS9918_GFX2_Mode(uint16_t lScanline);
void TMS9918_Sprites_GFX12(uint16_t lScanline);

// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////
void TMS9918_Reset(void)
{
    gTMS9918_FirstByte = 1;
    gTMS9918_VRAM_Address = 0x0000;
    gTMS9918_Status_r = 0x0;
    TMS9918_screen_mode = 0x0;
    Z80InterruptLine = 0;

    // Init control registers.
    uint8_t lRegister = 0;
    for (lRegister = 0; lRegister < TMS9918_CONTROL_REGISTERS; lRegister++)
        TMS9918_control_r[lRegister] = 0;
/*
    memset(TMS9918_vram, 0x55, 0x1800);
    memset(TMS9918_vram+0x2000, 0xF0, 0x1800);
    memset(TMS9918_vram+0x3800, 0x0, (32*24));
*/
    TMS9918_table_name =
    TMS9918_table_colors =
    TMS9918_table_pattern =
    TMS9918_table_sprites_attrtibute =
    TMS9918_table_sprites_pattern = NULL;

    gVideoModeColors[0] = 0;// Fore color
    gVideoModeColors[1] = 0;// Back color

}// End TMS9918_Reset.

// /////////////////////////////////////////////////////////////////////////////
void TMS9918_Init(void)
{
    // Copy the VDP palette to the screen palette
    gVideoModeColors[0] = 0;// Fore color
    gVideoModeColors[1] = 0;// Back color

    TMS9918_Reset();

    uint8_t *lBuffer = (uint8_t*)(*((uint32_t*)gVideoBufferAddr));

#if 0
    for (uint8_t y=0; y<192; y++) {
      memset(lBuffer+(y * 257), gPalette[(y>>3)&15], 256);
      *(lBuffer+(y * 257) + 256) = 0;
    }
#endif

}// End TMS9918_Init.

// /////////////////////////////////////////////////////////////////////////////
inline void TMS9918_Data_Write(uint8_t pData)
{
    if (gTMS9918_VRAM_WriteMode)
    {
        gTMS9918_ReadAhead = TMS9918_vram[gTMS9918_VRAM_Address] = pData;
        gTMS9918_VRAM_Address++;
       // gTMS9918_VRAM_Address &= 0x3FFF;
    }
    else
    {
        gTMS9918_ReadAhead = TMS9918_vram[gTMS9918_VRAM_Address];
        gTMS9918_VRAM_Address++;
       // gTMS9918_VRAM_Address &= 0x3FFF;
        TMS9918_vram[gTMS9918_VRAM_Address] = pData;
    }

    gTMS9918_FirstByte = 1;

}// End TMS9918_Data_Write.

// /////////////////////////////////////////////////////////////////////////////
inline uint8_t TMS9918_Data_Read(void)
{
    uint8_t lValue = gTMS9918_ReadAhead;

    gTMS9918_VRAM_Address++;
    //gTMS9918_VRAM_Address &= 0x3FFF;
    gTMS9918_ReadAhead = TMS9918_vram[gTMS9918_VRAM_Address];

    gTMS9918_FirstByte = 1;

    return lValue;

}// End TMS9918_Data_Read.

// /////////////////////////////////////////////////////////////////////////////
inline uint8_t TMS9918_Register_Read(void)
{
    uint8_t lValue = gTMS9918_Status_r;

    gTMS9918_Status_r &= _5THNUM;
    gTMS9918_FirstByte = 1;
    Z80InterruptLine = 0;

    return lValue;

}// End TMS9918_Register_Read.

// /////////////////////////////////////////////////////////////////////////////
void TMS9918_Get_screen_mode(void)
{
    /*
     * M1  M2  M3
     * ----------
     * 0   0   0  00 GFX1
     * 0   0   1  00 GFX2
     * 0   1   0  00 Multicolor
     * 1   0   0  00 Text
     *
     */
    // char gString[256];

    // 12
    // 18
    // 76543210

    uint8_t lValue = (((TMS9918_control_r[1] & 0x18) >> 2) | (TMS9918_control_r[0] & 0x02) >> 1);
    switch (lValue)
    {
    case 0x00:
        TMS9918_screen_mode = SCREEN_MODE_GFX1;
        break;
    case 0x01:
        TMS9918_screen_mode = SCREEN_MODE_GFX2;
        break;
    case 0x02:
        TMS9918_screen_mode = SCREEN_MODE_MULTICOLOR;
        break;
    case 0x04:
        TMS9918_screen_mode = SCREEN_MODE_TEXT;
        break;
    }// End switch

}// End TMS9918_Get_screen_mode.

// /////////////////////////////////////////////////////////////////////////////
inline uint8_t TMS9918_Register_Write(uint8_t pValue)
{
    uint8_t lRegister = 0, lReturn = 0, lCommand = 0;// This is the first byte for a
    // register access (latch)
    if (gTMS9918_FirstByte)
    {
        gTMS9918_Latched_Byte = pValue;
        gTMS9918_VRAM_Address = (gTMS9918_VRAM_Address & 0x3F00) | gTMS9918_Latched_Byte;
        gTMS9918_FirstByte = 0;
    }
    else
    {
        lCommand = (pValue & 0xC0);// Bit 7 and 6 for operation
        // gTMS9918_VRAM_Address = ((pValue & 0x3F) << 8) |
        // gTMS9918_Latched_Byte;
        gTMS9918_VRAM_Address = ((pValue & 0x3F) << 8) | (gTMS9918_VRAM_Address & 0xFF);
        gTMS9918_FirstByte = 1;

        switch (lCommand)
        {
        case 0x40:// Command (00)00 0000, set address for
        // reading from Video memory
        case 0x00:// Command (00)00 0000, set address for
            // reading from Video memory
            gTMS9918_VRAM_WriteMode = lCommand;
            gTMS9918_ReadAhead = TMS9918_vram[gTMS9918_VRAM_Address];
            break;

        case 0x80:// Command (10)000000, write to a register
            lRegister = pValue & 0xF;

            switch (lRegister)
            {
            case 0:
                TMS9918_control_r[lRegister] = gTMS9918_Latched_Byte;

                TMS9918_Get_screen_mode();

                gShiftX = (TMS9918_control_r[lRegister] & 0x1);
                break;

            case 1:
                TMS9918_control_r[lRegister] = gTMS9918_Latched_Byte;

                // enable or disable interrupts calling
                if (TMS9918_control_r[lRegister] & _IRQ)
                {
                    gInterruptEnable = 1;
                    if (gTMS9918_Status_r & _VBLANK)
                    {
                        Z80InterruptLine = 1;

                    }// End if.

                }// End if.
                else
                    gInterruptEnable = 0;

                TMS9918_Get_screen_mode();

                gSpritesMag = (TMS9918_control_r[lRegister] & _MAG);// get
                gSpritesSize = (TMS9918_control_r[lRegister] & _SIZE) ? 16 : 8;// get
                gSpritesIsZoomed = (TMS9918_control_r[lRegister] & _MAG);
                gSpritesIs8x16 = ((TMS9918_control_r[lRegister] & _SIZE) >> 1);

                // Precalc Sprite Handler variables
                gSpriteDimension = gSpritesSize;// dimension of sprite
                gSpriteDimension2 = gSpriteDimension - 1;// pre calculates
                gSpritesRealSize = gSpriteDimension << gSpritesMag;// Real

                // enable or disable screen operations
                if (TMS9918_control_r[lRegister] & _SCRENA)
                    gScreenEnable = 1;
                else
                    gScreenEnable = 0;

                break;
            // ------------
            case 2:
                TMS9918_control_r[lRegister] = gTMS9918_Latched_Byte;

                TMS9918_table_name =
                  &TMS9918_vram[(((TMS9918_control_r[lRegister] & gVDPControlRMask[TMS9918_screen_mode][lRegister]) << 10))];
                break;
            // ------------
            case 3:
                TMS9918_control_r[lRegister] = gTMS9918_Latched_Byte;

                TMS9918_table_colors =
                  &TMS9918_vram[(((TMS9918_control_r[lRegister] & gVDPControlRMask[TMS9918_screen_mode][lRegister]) << 6))];
                break;
            // ------------
            case 4:
                TMS9918_control_r[lRegister] = gTMS9918_Latched_Byte;

                TMS9918_table_pattern =
                  &TMS9918_vram[(((TMS9918_control_r[lRegister] & gVDPControlRMask[TMS9918_screen_mode][lRegister]) << 11))];
                break;
            // ------------
            case 5:
                TMS9918_control_r[lRegister] = gTMS9918_Latched_Byte;

                TMS9918_table_sprites_attrtibute =
                  &TMS9918_vram[(((TMS9918_control_r[lRegister] & gVDPControlRMask[TMS9918_screen_mode][lRegister]) << 7))];
                break;
            // ------------
            case 6:
                TMS9918_control_r[lRegister] = gTMS9918_Latched_Byte;

                TMS9918_table_sprites_pattern =
                  &TMS9918_vram[(((TMS9918_control_r[lRegister] & gVDPControlRMask[TMS9918_screen_mode][lRegister]) << 11))];
                break;
            // ------------
            case 7:
                TMS9918_control_r[lRegister] = gTMS9918_Latched_Byte;

                gVideoModeColors[1] = (TMS9918_control_r[lRegister] >> 4);// Fore
                gVideoModeColors[0] = (TMS9918_control_r[lRegister] & 0xF);// Back
                gPalette[0] = gPalette[gVideoModeColors[0]];
                break;
                // ------------

            }// End switch (gCommand).
            break;

        }// End switch (gCommand).

    }// End if.

    return lReturn;

}// End TMS9918_Register_Write.

// /////////////////////////////////////////////////////////////////////////////
uint32_t gVDPColors8x4[TMS9918_COLORS] =
{
    0x00000000,
    0x00000000,
    0x55555555,
    0x79797979,
    0x4a4a4a4a,
    0x8f8f8f8f,
    0xa8a8a8a8,
    0x5b5b5b5b,
    0xcdcdcdcd,
    0xf1f1f1f1,
    0xd9d9d9d9,
    0xdadadada,
    0x30303030,
    0xaeaeaeae,
    0xdfdfdfdf,
    0xffffffff
};
uint32_t gVDPPixels8x4FG[16] =
{
    0xFFFFFFFF,
    0x00FFFFFF,
    0xFF00FFFF,
    0x0000FFFF,
    0xFFFF00FF,
    0x00FF00FF,
    0xFF0000FF,
    0x000000FF,
    0xFFFFFF00,
    0x00FFFF00,
    0xFF00FF00,
    0x0000FF00,
    0xFFFF0000,
    0x00FF0000,
    0xFF000000,
    0x00000000
};
uint32_t gVDPPixels8x4BG[16] =
{
    0x00000000,
    0xFF000000,
    0x00FF0000,
    0xFFFF0000,
    0x0000FF00,
    0xFF00FF00,
    0x00FFFF00,
    0xFFFFFF00,
    0x000000FF,
    0xFF0000FF,
    0x00FF00FF,
    0xFFFF00FF,
    0x0000FFFF,
    0xFF00FFFF,
    0x00FFFFFF,
    0xFFFFFFFF
};

uint16_t lVideoTileRow;
uint8_t *lFromTileNumber;
uint8_t lTileDelta;
uint16_t lThird;
uint32_t bg, fg;
uint8_t lTempVDPColors8x4, lTempPixelPatternL, lTempPixelPatternR;
uint16_t lTileByte;
uint16_t gCounter;

// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////

void TMS9918_Sprites_GFX12(uint16_t lScanline)
{
#if 1
    // Draw sprites only when the screen enable flag is set.
    if (gScreenEnable)
    {
        lSpriteByte = 0;
        lCurrentSprite = 0;
        lInlineSprites = 0;

        uint8_t *lFrameBufferLinePtr = (uint8_t*)(*((uint32_t*)gVideoBufferAddr)) + (lScanline * 257);

        uint8_t *lFrameBufferLinePtrX = NULL;
        uint8_t *SpritePtr = TMS9918_table_sprites_attrtibute;

        while (lCurrentSprite < TMS9918_number_sprites)
        {
            if (lInlineSprites > TMS9918_inline_sprite_max)
            {
                // set Bit 6 of Status register for 5th sprite;
                gTMS9918_Status_r |= _5THSPR;

                // Set first 0-4 bits to Spr ( 5th in count )
                gTMS9918_Status_r = gTMS9918_Status_r + lCurrentSprite;

                return;
            }
            else
            {
                // get Y coordinate for this sprite.
                lSpriteY = *(SpritePtr++);
                lSpriteX = *(SpritePtr++);
                lSpriteShape = *(SpritePtr++);
                lSpriteColor = *SpritePtr & 0xF;
                lSpriteEarlyClock = (*SpritePtr & 0x80) >> 2;
                SpritePtr++;

                // sprite that have 208 break sprite rendering.
                if (lSpriteY == 0xD0)
                {
                    return;
                }

                // if y is greater than 208 adjust y coord.
                if (lSpriteY > 0xD0)
                    lSpriteY = -(~lSpriteY & 0xFF);
                else
                    lSpriteY++;

                // Check if the sprite touches the current scanline.
                if ((lScanline >= lSpriteY) && (lScanline < (lSpriteY + gSpritesRealSize)))
                {
                    // and is in this scanline cycle.
                    lInlineSprites++;

                    lSpriteX -= lSpriteEarlyClock;

                    // precalculate X coord considering EC and horiz borders.
                    lFrameBufferLinePtrX = lFrameBufferLinePtr + lSpriteX;

                    // Precalc steps for Mag sprites rendering.
                    lSpriteDelta = (lScanline - lSpriteY) >> gSpritesMag;

                    // precalculate a byte or 2 bytes for source sprite line
                    // rendering.
                    switch (gSpriteDimension)
                    {
                    case 8:
                        lSpriteByte =
                            *(TMS9918_table_sprites_pattern+
                              ((lSpriteShape << 3) + lSpriteDelta));
                        // The mask used to check every pixel of the sprite
                        // line is 8 bits wide.
                        lByteMask = 0x80;
                        break;

                    case 16:
                        lSpriteByte =
                            (*
                             (TMS9918_table_sprites_pattern+
                              (((lSpriteShape & 0xFC) << 3) +
                               lSpriteDelta)) << 8) | (*(TMS9918_table_sprites_pattern+
                                                         (((lSpriteShape & 0xFC) << 3) +
                                                          lSpriteDelta) + 16));
                        // The mask used to check every pixel of the sprite
                        // line is 16 bits wide.
                        lByteMask = 0x8000;
                        break;

                    }// End switch.
                    // --------------------------------

                    // draws sprite line
                    lBitX = 0;
                    while (lBitX < gSpritesRealSize)
                    {
                        if ((lSpriteX + lBitX >= 0) && (lSpriteX + lBitX < 256))
                            if (lSpriteByte & lByteMask)
                            {
                                // the bit of data is 1 or 0.
                                /*if (*lFrameBufferLinePtrX & 0x10)
                                {
                                    // if it finds an existent pixel on the
                                    // buffer it means overlapping!
                                    gTMS9918_Status_r |= _COINC;

                                }// End if.
                                else*/
                                {
                                    // if color is different than 0 draw
                                    // pixel, else doesn't draw it.
                                    if (lSpriteColor)
                                    {
                                        //GPIO_PRT10->OUT |= 4;
                                        *lFrameBufferLinePtrX = gPalette[lSpriteColor];// | 0x10;
                                        //GPIO_PRT10->OUT &= 0xFB;
                                    }
                                }// End if.
                            }// End if.

                        // According to the MAG flag we shift the mask every
                        // two pixels or every pixel.
                        // The result is a duplicate of one pixel so an
                        // enlarged sprite.
                        if (gSpritesMag)
                            lByteMask >>= (lBitX & 1);
                        else
                            lByteMask >>= 1;

                        // Next pixel.
                        lBitX++;
                        lFrameBufferLinePtrX = lFrameBufferLinePtrX + 1;

                    }// End while.

                }// End if.

            }// End if.

            lCurrentSprite++;

        }// End while.
#endif
    }// End if screen enable.
};// end TMS9918_Sprites_GFX12.

// /////////////////////////////////////////////////////////////////////////////
void TMS9918_GFX2_Mode(uint16_t lScanline)
{
      //if (lScanline > 95) lScanline -= 96;
      lFrameBufferPtrM = (uint8_t*)(*((uint32_t*)gVideoBufferAddr) + (lScanline * 257));
      lFrameBufferPtrL = lFrameBufferPtrM + 4;
      lFrameBufferToPtr = lFrameBufferPtrM + 256;

#if 1
    if (gScreenEnable)
    {
      lVideoTileRow = (lScanline >> 3) << 5;
      lFromTileNumber = TMS9918_table_name + lVideoTileRow;
      lTileDelta = (lScanline & 0x7);
      lThird = lVideoTileRow & 0x300;

      while (lFrameBufferPtrM < lFrameBufferToPtr)
      {
        lTileByte = ((*lFromTileNumber + lThird) << 3) + lTileDelta;
        lTempVDPColors8x4 = *(TMS9918_table_colors + lTileByte);
        lTempPixelPatternL = (*(TMS9918_table_pattern + lTileByte)) >> 4;
        lTempPixelPatternR = (*(TMS9918_table_pattern + lTileByte)) & 15;

        bg = gVDPColors8x4[lTempVDPColors8x4 >> 4];
        fg = gVDPColors8x4[lTempVDPColors8x4 & 0xF];

        //GPIO_PRT10->OUT |= 2;
        *((uint32_t*)lFrameBufferPtrM) = (fg & gVDPPixels8x4FG[lTempPixelPatternL]) | (bg & gVDPPixels8x4BG[lTempPixelPatternL]);
        *((uint32_t*)lFrameBufferPtrL) = (fg & gVDPPixels8x4FG[lTempPixelPatternR]) | (bg & gVDPPixels8x4BG[lTempPixelPatternR]);
        //GPIO_PRT10->OUT &= 0xFD;

        lFrameBufferPtrM += 8;
        lFrameBufferPtrL += 8;
        lFromTileNumber++;

      }// End for.

      *lFrameBufferPtrM = 0;
    }
    else
    {
        // If the bit 0x40 is set the screen is not rendered and it's filled
        // with the
        // Background color
        gCounter = 64;
        while (gCounter--)
        {
          *((uint32_t*)lFrameBufferPtrM) = gVDPColors8x4[gVideoModeColors[0]];
          lFrameBufferPtrM += 4;
        }// End for.

        *lFrameBufferPtrM = 0;

    }// End if screen enable

#endif

}// End TMS9918_GFX2_Mode.

// /////////////////////////////////////////////////////////////////////////////
uint8_t TMS9918_Check_VBlank(uint16_t lScanline)
{
    if (lScanline == TMS9918_VIDEO_HEIGHT)
    {
        if (gInterruptEnable)
        {
            Z80InterruptLine = 1;
            gTMS9918_Status_r |= _VBLANK;

        }// End if.

    }// End if.

    return 0;

}// End TMS9918_Check_VBlank.

// /////////////////////////////////////////////////////////////////////////////
