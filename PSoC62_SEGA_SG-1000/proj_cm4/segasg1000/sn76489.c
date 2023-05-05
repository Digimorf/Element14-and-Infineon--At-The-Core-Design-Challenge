/*//////////////////////////////////////////////////////////////////////////////
* File Name:   sn76489.c
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

#include "ipc_shared_memory.h"

#include "sn76489.h"

// /////////////////////////////////////////////////////////////////////////////
const uint16_t gVolumeTable[16] = {64, 62, 49, 39, 31, 25, 20, 16, 12, 10, 8, 6, 5, 4, 3, 0};
Chip_sn76489_t gPSG;
uint16_t *gAudioBufferPtr;
// /////////////////////////////////////////////////////////////////////////////
void SN76489_Reset(Chip_sn76489_t *pPSG)
{
    /*
    DESCRIPTION:  Resets the chip values.
    PARMS:        See above.
    RETURNS:      Nothing.
    */

    uint8_t lChannel=0;
    for (lChannel=0; lChannel<4; lChannel++)
    {
        pPSG->channel[lChannel].attenuator = 15;
        pPSG->channel[lChannel].frequency_period_divider = 0xFFFF;
        pPSG->channel[lChannel].output = 0;
        pPSG->channel[lChannel].oscillator = 1;
        pPSG->channel[lChannel].counter = pPSG->channel[lChannel].frequency_period_divider;

    } // End for.

    pPSG->latched_channel = 0;
    pPSG->latched_volume = 15;
    pPSG->noiseLFSR = 0x8000;
    pPSG->noiseTAP = 0;
    pPSG->ticks = pPSG->ticks_per_sample;

} // End SN76489_Reset.

// /////////////////////////////////////////////////////////////////////////////
void SN76489_Init(Chip_sn76489_t *pPSG, uint32_t pClock, uint16_t pSampleRate, uint8_t pFPS)
{
    /*
    DESCRIPTION:  Initialize the chip values.
    PARMS:        See above.
    RETURNS:      Nothing.
    */

    pPSG->ticks_per_sample = ( pClock >> 4 ) / pSampleRate;
    pPSG->samples_per_frame = (pSampleRate / pFPS);

    SN76489_Reset(pPSG);

    gAudioBufferPtr = (uint16_t*)(*((uint32_t*)gAudioBufferAddr));

} // End SN76489_Init.

// /////////////////////////////////////////////////////////////////////////////
__inline void SN76489_Write(Chip_sn76489_t *pPSG, uint8_t pData)
{
    /*
    DESCRIPTION:  Write the chip register.
    PARMS:        See above.
    RETURNS:      Nothing.
    */

    uint8_t lChannel = 0;
    uint32_t lDivider = 0;

    // Write to change the tone of a channel
    if (pData & 0x80)
    {
        // Check if byte is a register byte or data byte
        lChannel = (pData >> 5) & 0x03;
        lDivider = (pPSG->channel[lChannel].frequency_period_divider & 0xFFF0) | (pData & 0x0F);

        pPSG->latched_channel = lChannel;
        pPSG->latched_volume = (pData & 0x10) ? 1 : 0;
    }
    else
    {
        lChannel =  pPSG->latched_channel;
        lDivider = (pPSG->channel[lChannel].frequency_period_divider & 0x0F) | ((pData & 0x3F) << 4);

    } // End if.

    // Write to change the volume of a channel.
    if (pPSG->latched_volume)
    {
        pPSG->channel[lChannel].attenuator = pData & 0xF;
    }
    else
    {
        pPSG->channel[lChannel].frequency_period_divider = lDivider;

        if (lChannel == 3)
        {
            pPSG->noiseTAP = ((lDivider >> 2) & 1) ? 9 : 1;
            pPSG->noiseLFSR = 0x8000;

        } // End if.

    } // End if.

} // End SN76489_Write.

// /////////////////////////////////////////////////////////////////////////////
void SN76489_Render(Chip_sn76489_t *pPSG, uint16_t lScanline)
{
    /*
    DESCRIPTION:  Renders the channels to the buffer provided.
    PARMS:        See above.
    RETURNS:      Nothing.

    553 cycles more or less.

    */

    uint8_t lChannel = 0;
    uint16_t lDivider = 0;
    uint16_t lTap = 0;

    Channel_Audio_t *lTempChannel;

    if (lScanline == 0) gAudioBufferPtr = (uint16_t*)(*((uint32_t*)gAudioBufferAddr));

    // This is the main cycle used to render all the samples requested.
    // Ticks are the clock cycle of the audio chip.
    while (pPSG->ticks)
    {
        // Cycle through the channels.
        for (lChannel = 0; lChannel < 3; lChannel++)
        {
            lTempChannel = &pPSG->channel[lChannel];

            // Decrease each counter and when it goes to zero we update the oscillator.
            lTempChannel->counter--;
            if (lTempChannel->counter == 0)
            {
                // This is a particular behavior of this chip. if a period is provided the oscillator
                // is active, but if the period is set to 0 the oscillator is OFF.
                if (lTempChannel->frequency_period_divider)
                {
                    // Toggle the oscillator and calculate the output value according the
                    // Volume table.
                    lTempChannel->oscillator = (lTempChannel->oscillator) ? 0 : 1;
                    lTempChannel->output = (lTempChannel->oscillator) ? gVolumeTable[lTempChannel->attenuator] : 0;
                }
                else
                {
                    // When the oscillator is OFF we use the volume value. This lets the chip to play sample
                    // by modulating the volume only. Very low resolution but it works.
                    lTempChannel->output = gVolumeTable[lTempChannel->attenuator];

                } // End if.

                // Resets the counter for the next cycle.
                lTempChannel->counter = lTempChannel->frequency_period_divider;

            } // End if.

        } // End for.

        lTempChannel = &pPSG->channel[3];

        // Update the noise channel.
        lTempChannel->counter--;
        if (lTempChannel->counter < 0)
        {
            lDivider = lTempChannel->frequency_period_divider & 3;
            if (lDivider < 3)
                lTempChannel->counter = 0x10 << lDivider;
            else
                lTempChannel->counter = pPSG->channel[2].frequency_period_divider << 1;

            if (pPSG->noiseTAP == 9)
            {
                lTap = pPSG->noiseLFSR & pPSG->noiseTAP;
                lTap ^= lTap >> 8;
                lTap ^= lTap >> 4;
                lTap ^= lTap >> 2;
                lTap ^= lTap >> 1;
                lTap &= 1;
            }
            else
            {
                lTap = pPSG->noiseLFSR & 1;

            } // End if.

            pPSG->noiseLFSR = (pPSG->noiseLFSR >> 1) | (lTap << 15);
            lTempChannel->oscillator = ((pPSG->noiseLFSR & 1) ^ 1);
            lTempChannel->output = lTempChannel->oscillator  * gVolumeTable[lTempChannel->attenuator];

        } // End if.

        // Update the ticks.
        pPSG->ticks--;

    } // End while.

    // Update the ticks with the reminder to have a smooth output between cycles.
    pPSG->ticks = pPSG->ticks_per_sample;

    // Mix the channels and return the sample.
    gAudioBufferPtr[(lScanline<<1)+1] = gAudioBufferPtr[lScanline<<1] = (pPSG->channel[0].output + pPSG->channel[1].output + pPSG->channel[2].output + pPSG->channel[3].output) << 5;

} // End SN76489_Render.
