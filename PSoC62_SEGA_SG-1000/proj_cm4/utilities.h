/*//////////////////////////////////////////////////////////////////////////////
* File Name:   utilities.c
*
* Description: Element14 - At The Core Design Challenge
*              Digimorf, SEGA SG-1000 Emulator lite
*              Misc utilities functions
*
* Created on: Mar 16 2023
*     Author: Francesco De Simone - Digimorf
*
//////////////////////////////////////////////////////////////////////////////*/

#ifndef UTILITIES_H_
#define UTILITIES_H_

#ifdef __cplusplus
 extern "C" {
#endif

// C standard libraries.
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

// /////////////////////////////////////////////////////////////////////////////
// Definitions
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

 // Character conversion macros.
 #define CHR_IS_LOWCASEALPHA(ch)   ((ch >= ASCII_a)&&(ch <= ASCII_z))

#define ASSEMBLE_ARGB(A,R,G,B) (A << 24 | R << 16 | G << 8 | B)
#define ABS(x)                 ((x) > 0 ? (x) : -(x))
#define MIN(x, y)              ((x) > (y) ? (y) : (x))
#define MAX(x, y)              ((x) < (y) ? (y) : (x))
#define ROUND(x)               ((x)>=0?(int32_t)((x)+0.5):(int32_t)((x)-0.5))

// /////////////////////////////////////////////////////////////////////////////
// Global elements
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////

char *Utility_Get_String_ID
(
  uint32_t pId,   // The id of the string.
  char *pBuffer   // The pointer to the buffer.
);
// -----------------------------------------------------------------------------
char *Utility_Skip_Blank_Chars
(
  char *pString  // The pointer to the string.
);
// -----------------------------------------------------------------------------
char *Utility_Skip_Line
(
  char *pString  // The pointer to the string.
);
// -----------------------------------------------------------------------------
void Utility_Goto_Char
(
  char *pString,   // The pointer to the string.
  char pCharacter  // The character of the string to point to.
);
// -----------------------------------------------------------------------------
char *Utility_Get_Token
(
  char **pLineTokenPtr,   // The pointer to the string.
  char pNextDivider       // The pointer to the extracted token.
);
// -----------------------------------------------------------------------------
char Utility_Character_At
(
  char *pString,      // The pointer to the string.
  uint32_t pCounter   // The position counter of the character.
);
// -----------------------------------------------------------------------------
uint8_t Utility_String_Match
(
  char *pFirst,   // The pFirst string.
  char *pSecond   // The pSecond string.
);
// -----------------------------------------------------------------------------
void Utility_String_Match_Rename
(
  char *pFirst,    // The pFirst string.
  char *pSecond,   // The pSecond string.
  char *pThird     // The new name that results from
);
// -----------------------------------------------------------------------------
void Utility_Uppercase
(
  char *pString   // The pointer to the string.
);
// -----------------------------------------------------------------------------
void Memory_FillCopy
(
    uint8_t *pBufferTo,    // The buffer where to copy into.
    uint8_t *pBufferFrom,  // The buffer where to copy from.
    uint32_t pSize,          
    uint8_t pValue,          
    uint8_t pMode         
);
// -----------------------------------------------------------------------------
void Utility_Show_Bytes
(
  void *pMemoryPointer,   // The pointer to any memory location to start showing the bytes from.
  uint32_t pNumberOfBytes // The amount of bytes to show.
);
// -----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif // UTILITIES_H_
