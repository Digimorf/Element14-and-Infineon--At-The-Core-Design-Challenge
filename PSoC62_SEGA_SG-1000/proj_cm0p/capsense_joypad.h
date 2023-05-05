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

#ifndef CAPSENSE_JOYPAD_H_
#define CAPSENSE_JOYPAD_H_

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////

// For SEGA SG-1000 Joystick functionality
#define JOYSTICK_BIT_LEFT  0x04
#define JOYSTICK_BIT_UP    0x01
#define JOYSTICK_BIT_RIGHT 0x08
#define JOYSTICK_BIT_DOWN  0x02
#define JOYSTICK_BIT_FR1   0x10
#define JOYSTICK_BIT_FR2   0x20

// /////////////////////////////////////////////////////////////////////////////
// Prototypes
// /////////////////////////////////////////////////////////////////////////////

void capsense_joypad_init(void);
void capsense_joypad_check(void);

#endif /* CAPSENSE_JOYPAD_H_ */
