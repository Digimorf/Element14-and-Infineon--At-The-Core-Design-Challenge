/*//////////////////////////////////////////////////////////////////////////////
* File Name:   capsense_joypad.h
*
* Description: Element14 - At The Core Design Challenge
*              Digimorf, SEGA SG-1000 Emulator lite
*              Capsense joypad driver for PSoC62S4 Pioneer kit
*
* Created on: Apr 20 2023
*     Author: Francesco De Simone - Digimorf
*
//////////////////////////////////////////////////////////////////////////////*/


// /////////////////////////////////////////////////////////////////////////////
// Includes
// /////////////////////////////////////////////////////////////////////////////
#include "cy_pdl.h"
#include "cycfg.h"
#include "cybsp.h"
#include "cycfg_capsense.h"

#include "ipc_shared_memory.h"
#include "capsense_joypad.h"

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////
#define CAPSENSE_INTR_PRIORITY  (3u)
#define EZI2C_INTR_PRIORITY     (2u)

// /////////////////////////////////////////////////////////////////////////////
// Prototypes
// /////////////////////////////////////////////////////////////////////////////
static uint32_t initialize_capsense(void);
static void process_touch(void);
static void capsense_isr(void);
static void capsense_callback(cy_stc_active_scan_sns_t *);
void handle_error(void);

// /////////////////////////////////////////////////////////////////////////////
// Private
// /////////////////////////////////////////////////////////////////////////////
volatile bool capsense_scan_complete = false;
cy_stc_scb_ezi2c_context_t ezi2c_context;

uint8_t gPPIPort = 0xFF;

// /////////////////////////////////////////////////////////////////////////////
// Functions
// /////////////////////////////////////////////////////////////////////////////
void handle_error(void)
{
    __disable_irq();

    CY_ASSERT(0);
}

void capsense_io_init(void)
{
  cy_rslt_t result;

  gPPIPort = 0xFF;

  *((uint32_t*)gIOAddr) = &gPPIPort;

  result = initialize_capsense();

  if (CYRET_SUCCESS != result)
  {
      CY_ASSERT(0);
  }

  Cy_CapSense_ScanAllWidgets(&cy_capsense_context);
}

void capsense_io_check(void)
{
  if (capsense_scan_complete)
  {
      Cy_CapSense_ProcessAllWidgets(&cy_capsense_context);

      process_touch();

      Cy_CapSense_ScanAllWidgets(&cy_capsense_context);

      capsense_scan_complete = false;
   }
}

static uint32_t initialize_capsense(void)
{
    uint32_t status = CYRET_SUCCESS;

    /* CapSense interrupt configuration parameters */
    static const cy_stc_sysint_t capSense_intr_config =
    {
        .intrSrc = NvicMux3_IRQn,
        .cm0pSrc = csd_interrupt_IRQn,
        .intrPriority = CAPSENSE_INTR_PRIORITY,
    };

    /* Capture the CSD HW block and initialize it to the default state. */
    status = Cy_CapSense_Init(&cy_capsense_context);
    if (CYRET_SUCCESS != status)
    {
        return status;
    }

    /* Initialize CapSense interrupt */
    Cy_SysInt_Init(&capSense_intr_config, capsense_isr);
    NVIC_ClearPendingIRQ(capSense_intr_config.intrSrc);
    NVIC_EnableIRQ(capSense_intr_config.intrSrc);

    /* Initialize the CapSense firmware modules. */
    status = Cy_CapSense_Enable(&cy_capsense_context);
    if (CYRET_SUCCESS != status)
    {
        return status;
    }

    /* Assign a callback function to indicate end of CapSense scan. */
    status = Cy_CapSense_RegisterCallback(CY_CAPSENSE_END_OF_SCAN_E,
            capsense_callback, &cy_capsense_context);
    if (CYRET_SUCCESS != status)
    {
        return status;
    }

    return status;
}

static void capsense_isr(void)
{
    Cy_CapSense_InterruptHandler(CYBSP_CSD_HW, &cy_capsense_context);
}

static void capsense_callback(cy_stc_active_scan_sns_t * ptrActiveScan)
{
    capsense_scan_complete = true;
}

static void process_touch(void)
{
    uint32_t button0_status;
    uint32_t button1_status;
    cy_stc_capsense_touch_t *slider_touch_info;
    uint16_t slider_pos;
    uint8_t slider_touch_status;

    static uint16_t slider_pos_prev;

    /* Get button 0 status */
    button0_status = Cy_CapSense_IsSensorActive(
                                CY_CAPSENSE_BUTTON0_WDGT_ID,
                                CY_CAPSENSE_BUTTON0_SNS0_ID,
                                &cy_capsense_context);

    /* Get button 1 status */
    button1_status = Cy_CapSense_IsSensorActive(
                                CY_CAPSENSE_BUTTON1_WDGT_ID,
                                CY_CAPSENSE_BUTTON0_SNS0_ID,
                                &cy_capsense_context);

    /* Get slider status */
    slider_touch_info = Cy_CapSense_GetTouchInfo(
        CY_CAPSENSE_LINEARSLIDER0_WDGT_ID, &cy_capsense_context);
    slider_touch_status = slider_touch_info->numPosition;
    slider_pos = slider_touch_info->ptrPosition->x;

    if (button0_status == 0)
    _SHARED_gIOPort |= JOYSTICK_BIT_FR1;
    else
    _SHARED_gIOPort &= ~JOYSTICK_BIT_FR1;

    if (button1_status == 0)
    _SHARED_gIOPort |= JOYSTICK_BIT_FR2;
    else
    _SHARED_gIOPort &= ~JOYSTICK_BIT_FR2;

    /* Detect the new touch on slider */
    if ((slider_touch_status) && (slider_pos != slider_pos_prev))
    {
      if (slider_pos > slider_pos_prev)
      {
        _SHARED_gIOPort |= JOYSTICK_BIT_LEFT;
        _SHARED_gIOPort &= ~JOYSTICK_BIT_RIGHT;
      }
      else if (slider_pos < slider_pos_prev)
      {
        _SHARED_gIOPort |= JOYSTICK_BIT_RIGHT;
        _SHARED_gIOPort &= ~JOYSTICK_BIT_LEFT;
      }
    }
    else
    {
        _SHARED_gIOPort &= ~JOYSTICK_BIT_RIGHT;
        _SHARED_gIOPort &= ~JOYSTICK_BIT_LEFT;
    }

    /* Update previous touch status */
    slider_pos_prev = slider_pos;
}
