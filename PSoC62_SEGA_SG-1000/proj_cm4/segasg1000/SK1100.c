// #########################################################
//  SK1100 Keyboard and joystick interface for SG-1000
// #########################################################
//  ArcadeIt emulation system
//  by Francesco De Simone
//  www.digimorf.com
//  --------------------------------------------------------
// #########################################################

/*
 SK1100 Keyboard, that is embedded into the SEGA SC-3000

       Columns
       PPI Port A                      PPI Port B
  Rows D0  D1  D2  D3  D4  D5  D6  D7  D0  D1  D2  D3
   -   ------------------------------- ---------------
   0   '1' 'Q' 'A' 'Z' ED  ',' 'K' 'I' '8' --- --- ---
   1   '2' 'W' 'S' 'X' SPC '.' 'L' 'O' '9' --- --- ---
   2   '3' 'E' 'D' 'C' HC  '/' ';' 'P' '0' --- --- ---
   3   '4' 'R' 'F' 'V' ID  PI  ':' '@' '-' --- --- ---
   4   '5' 'T' 'G' 'B' --- DA  ']' '[' '^' --- --- ---
   5   '6' 'Y' 'H' 'N' --- LA  CR  --- YEN --- --- FNC
   6   '7' 'U' 'J' 'M' --- RA  UA  --- BRK GRP CTL SHF
   7   1U  1D  1L  1R  1TL 1TR 2U  2D  2L  2R  2TL 2TR

  ED  = "ENG DIER'S"
  SPC = (Spacebar)a
  HC  = "HOME CLR"
  ID  = "INS DEL"
  PI  = (PI symbol)
  DA  = (Down arrow on keypad)
  LA  = (Left arrow on keypad)
  RA  = (Right arrow on keypad)
  CR  = "CR" (Enter)
  UA  = (Up arrow on keypad)
  YEN = (Yen symbol)
  BRK = "BREAK"
  GRP = "GRAPH"
  CTL = "CTRL"
  FNC = "FUNC"
  SHF = "SHIFT"
  1U  = Joystick #1 up
  1D  = Joystick #1 down
  1L  = Joystick #1 left
  1R  = Joystick #1 right
  1TL = Joystick #1 left trigger
  1TR = Joystick #1 right trigger
  2U  = Joystick #2 up
  2D  = Joystick #2 down
  2L  = Joystick #2 left
  2R  = Joystick #2 right
  2TL = Joystick #2 left trigger
  2TR = Joystick #2 right trigger
  --- = Key is unused, always returns 1

  ============================================================================
  Joystick connectors
  ============================================================================

  Pin diagram of male DB9 plug on back of console:

    1 2 3 4 5
     6 7 8 9

  1 : Joystick up
  2 : Joystick down
  3 : Joystick left
  4 : Joystick right
  5 : Unused (not connected to anything)
  6 :
  7 : Unused (not connected to anything)
  8 : Common
  9 : Trigger right

  Pin 8 is the common side of all switches in the joystick and is connected
  to output line 7 of the 74LS145. It is LOW when the seventh keyboard row
  is selected (enabling the joystick ports for input) and HIGH otherwise
  (joystick ports disabled).
 */

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "ipc_shared_memory.h"

#include "SK1100.h"

// /////////////////////////////////////////////////////////////////////////////
// Types
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Extern
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Exported globals
// /////////////////////////////////////////////////////////////////////////////
uint8_t gPortC = 0, gPortA = 0, gPortB = 0;
uint8_t *gPPIPort;

// /////////////////////////////////////////////////////////////////////////////
// Exported functions
// /////////////////////////////////////////////////////////////////////////////

void SK1100_Init(void)
{
  /*
  DESCRIPTION:  Initialize the chip values.
  PARMS:        See above.
  RETURNS:      Nothing.
  */

  gPPIPort = (uint8_t*)(*((uint32_t*)gIOAddr));
}

void SK1100_Write_PortC(uint8_t pValue)
{
  gPortC = pValue;

}; // End SK1100_Write_PortC.

uint8_t SK1100_Read_PortA(void)
{
/*
       PPI Port A                      PPI Port B
 Rows D0  D1  D2  D3  D4  D5  D6  D7  D0  D1  D2  D3
  -   ------------------------------- ---------------
  0   '1' 'Q' 'A' 'Z' ED  ',' 'K' 'I' '8' --- --- ---
  1   '2' 'W' 'S' 'X' SPC '.' 'L' 'O' '9' --- --- ---
  2   '3' 'E' 'D' 'C' HC  '/' ';' 'P' '0' --- --- ---
  3   '4' 'R' 'F' 'V' ID  PI  ':' '@' '-' --- --- ---
  4   '5' 'T' 'G' 'B' --- DA  ']' '[' '^' --- --- ---
  5   '6' 'Y' 'H' 'N' --- LA  CR  --- YEN --- --- FNC
  6   '7' 'U' 'J' 'M' --- RA  UA  --- BRK GRP CTL SHF
  7   1U  1D  1L  1R  1TL 1TR 2U  2D  2L  2R  2TL 2TR
 */


  gPortA =  _SHARED_gIOPort;

  return gPortA;

}; // End SK1100_Read_PortA.

uint8_t SK1100_Read_PortB(void)
{
  /*
       PPI Port A                      PPI Port B
 Rows D0  D1  D2  D3  D4  D5  D6  D7  D0  D1  D2  D3
  -   ------------------------------- ---------------
  0   '1' 'Q' 'A' 'Z' ED  ',' 'K' 'I' '8' --- --- ---
  1   '2' 'W' 'S' 'X' SPC '.' 'L' 'O' '9' --- --- ---
  2   '3' 'E' 'D' 'C' HC  '/' ';' 'P' '0' --- --- ---
  3   '4' 'R' 'F' 'V' ID  PI  ':' '@' '-' --- --- ---
  4   '5' 'T' 'G' 'B' --- DA  ']' '[' '^' --- --- ---
  5   '6' 'Y' 'H' 'N' --- LA  CR  --- YEN --- --- FNC
  6   '7' 'U' 'J' 'M' --- RA  UA  --- BRK GRP CTL SHF
  7   1U  1D  1L  1R  1TL 1TR 2U  2D  2L  2R  2TL 2TR
 */

  switch (gPortC)
  {
    case 0x7:
      //        PPI Port B
      //  Rows D0  D1  D2  D3  D4  D5  D6  D7
      //   -   -------------------------------
      //   7   -   -   -   -   -   -   -   -
      gPortB = 0xFF;
    break;

    default:
      gPortB = 0xFF;
    break;

  } // End switch.

  return gPortB;

}; // End SK1100_Read_PortB.

// /////////////////////////////////////////////////////////////////////////////
