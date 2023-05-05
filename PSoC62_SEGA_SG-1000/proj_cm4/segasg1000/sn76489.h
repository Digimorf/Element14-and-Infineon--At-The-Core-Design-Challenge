/*//////////////////////////////////////////////////////////////////////////////
* File Name:   sn76489.h
*
* Description: Element14 - At The Core Design Challenge
*              Digimorf, SEGA SG-1000 Emulator lite
*
* Created on: Apr 30 2023
*     Author: Francesco De Simone - Digimorf
*
//////////////////////////////////////////////////////////////////////////////*/

#ifndef __SN76489_H
#define __SN76489_H

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// /////////////////////////////////////////////////////////////////////////////
// Export
// /////////////////////////////////////////////////////////////////////////////
extern uint16_t *gAudioBufferPtr;

// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////

typedef struct Channel_Audio
{
    int16_t counter;
    uint8_t  attenuator;
    //uint16_t  output;
    int8_t  output;
    uint8_t  oscillator;
    uint16_t frequency_period_divider;

} Channel_Audio_t, *Channel_Audio_ptr;

typedef struct Chip_sn76489
{
    Channel_Audio_t channel[4];
    uint16_t ticks;
    uint8_t latched_channel;
    uint8_t latched_volume;
    uint16_t noiseLFSR;
    uint8_t noiseTAP;
    uint16_t ticks_per_sample;
    uint16_t samples_per_frame;

} Chip_sn76489_t, *Chip_sn76489_ptr;

// /////////////////////////////////////////////////////////////////////////////
// Exported globals
// /////////////////////////////////////////////////////////////////////////////
extern Chip_sn76489_t gPSG;

// /////////////////////////////////////////////////////////////////////////////
// Exported functions
// /////////////////////////////////////////////////////////////////////////////

void SN76489_Reset(Chip_sn76489_t *pPSG);
void SN76489_Init(Chip_sn76489_t *pPSG, uint32_t pClock, uint16_t pSampleRate, uint8_t pFPS);
void SN76489_Write(Chip_sn76489_t *pPSG, uint8_t pData);
void SN76489_Render(Chip_sn76489_t *pPSG, uint16_t lScanline);

#endif // __SN76489_H
