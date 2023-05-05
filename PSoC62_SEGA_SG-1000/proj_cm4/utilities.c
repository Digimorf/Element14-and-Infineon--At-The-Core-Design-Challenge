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

// /////////////////////////////////////////////////////////////////////////////
// Includes.
// /////////////////////////////////////////////////////////////////////////////

  // C standard libraries.
  #include <stdio.h>
  #include <stdint.h>
  #include <inttypes.h>
  #include <ctype.h>
  #include <string.h>

  // Related include files.
  #include "utilities.h"
  #include "ansi_escape.h"

// /////////////////////////////////////////////////////////////////////////////
// Functions.
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Macros
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// External elements
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Global elements
// /////////////////////////////////////////////////////////////////////////////

// /////////////////////////////////////////////////////////////////////////////
// Shared functions.
// /////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
void Utility_Uppercase
(
  char *pString   // The pointer to the string.
)
{
  /*
  * DESCRIPTION: This function modify the string provided to upper case.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  // Temporary character.
  char lTempCharacter = *pString;

  // Go through the characters of the string provided and transform them to upper case.
  while (lTempCharacter)
  {
    // Upper case only alpha lower case.
    if (CHR_IS_LOWCASEALPHA(lTempCharacter))
      *pString = lTempCharacter - 32;

    // Next characters.
    lTempCharacter = *((char*)++pString);

  } // End while.

} // End Utility_Upper case.

// -----------------------------------------------------------------------------
char *Utility_Get_String_ID
(
  uint32_t pId,   // The id of the string.
  char *pBuffer   // The pointer to the buffer.
)
{
  /*
  * DESCRIPTION: This function returns the pointer to the in string found in a buffer.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  // Temporary character.
  uint32_t lTempCharacter = 0;

  char *msg = pBuffer;

  // Go through the characters of the string provided and transform them to upper case.
  while (lTempCharacter != pId)
  {
    if (*msg++ == '\0') lTempCharacter++;

  } // End while.

  return msg;

} // End Utility_Get_String_ID.

// -----------------------------------------------------------------------------
char *Utility_Skip_Blank_Chars
(
  char *pString  // The pointer to the string.
)
{
  /*
  * DESCRIPTION: Update the pointer of a string to the pFirst non blank character.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  // Go through the characters of the string provided until a non space character is found.
  while (*pString == ASCII_SPACE)
  {
    pString++;

  } // End while.

  return pString;

} // End Utility_Skip_Blank_Chars.

// -----------------------------------------------------------------------------
char *Utility_Skip_Line
(
  char *pString  // The pointer to the string.
)
{
  /*
  * DESCRIPTION: Update the pointer of a string to the first character after a new line..
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  // Go through the characters of the string provided until a non space character is found.
  while ((*pString != ASCII_CR) && (*pString != ASCII_LF))
  {
    //ArcadeIT_Terminal_Char_Send(*pString);

    if (*pString == 0)
      break;

    pString++;

  } // End while.

  if (
      ((*pString == ASCII_CR) && (*(pString+1) == ASCII_LF)) ||
      ((*pString == ASCII_LF) && (*(pString+1) == ASCII_CR))
     )
  {
    pString += 2;

  } // End if.

  //ArcadeIT_Terminal_Debug(CURSOR_NEWLINE);

  return pString;

} // End Utility_Skip_Line.

// -----------------------------------------------------------------------------
void Utility_Goto_Char
(
  char *pString,   // The pointer to the string.
  char pCharacter  // The character of the string to point to.
)
{
  /*
  * DESCRIPTION: update the pointer of a string to the pFirst blank character.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  // Go through the characters of the string provided until a non space character is found.
  while (*pString++ != pCharacter) {};

} // End Utility_Goto_Char.

// -----------------------------------------------------------------------------
char *Utility_Get_Token
(
  char **pLineTokenPtr,   // The pointer to the string.
  char pNextDivider       // The pointer to the extracted token.
)
{
  /*
  * DESCRIPTION: Get one subsequent token from a string.
  * PARAMETERS:  See above.
  * RETURNS:     The updated position into the string.
  */

    char *lLineTokenEnd, *lLineTokenStart, lNextChar = 0;

    lLineTokenStart = *pLineTokenPtr;
    lLineTokenEnd = *pLineTokenPtr;

    // Set the string delimiter.
    if (*lLineTokenStart == '"')
    {
        // Set the next break character for the token.
        lNextChar = '"';

        // Update the start point.
        lLineTokenStart++;
        lLineTokenEnd = lLineTokenStart;
    }
    else if (*lLineTokenStart == 0)
    {
      *pLineTokenPtr = 0;

      return lLineTokenStart;
    }
    else
    {
        lNextChar = pNextDivider;

    } // End if.

    // Extract token.
    while (*lLineTokenEnd != lNextChar && *lLineTokenEnd != 0 && *lLineTokenEnd != ASCII_CR && *lLineTokenEnd != ASCII_LF)
    {
      lLineTokenEnd++;

    } // End while.

    // Align to the next token start.
    switch(*lLineTokenEnd)
    {
      case ASCII_CR:
      case ASCII_LF:
        *lLineTokenEnd = 0;
      break;

      case 0:
        *pLineTokenPtr = 0;

        return lLineTokenStart;
      break;

      case '"':
        if (lNextChar == '"')
        {
          /* "text string"token...     -> ..tring\x00token
           * "text string" token...    -> ..tring\x00\x20token
           * "text string"    token... -> ..tring\x00\x20...\x20token
           */
            lLineTokenEnd++;
            *lLineTokenEnd = 0;

            // Skip eventual blank spaces after the token.
            lLineTokenEnd++;
            if (*lLineTokenEnd == ASCII_SPACE)
              while (*lLineTokenEnd++ != ASCII_SPACE) {};

        } // End if.
      break;

      default:
        if ((*lLineTokenEnd == pNextDivider) && (lNextChar == pNextDivider))
        {
            *lLineTokenEnd = 0;

            // Skip eventual blank spaces after the token.
            while (*lLineTokenEnd++ == ASCII_SPACE) {};

        } // End if.
      break;

    } // End switch.

    *pLineTokenPtr = lLineTokenEnd;

    return lLineTokenStart;

} // End Utility_Get_Token.

// -----------------------------------------------------------------------------
char Utility_Character_At
(
  char *pString,      // The pointer to the string.
  uint32_t pCounter   // The position counter of the character.
)
{
  /*
  * DESCRIPTION: Returns a character from a string at a given position.
  *              The position is always calculated wrapping around the length of
  *              the string.
  * PARAMETERS:  See above.
  * RETURNS:     the character.
  */

  return (pString[pCounter % strlen(pString)]);

} // End Utility_Animation_Charfacters.

// -----------------------------------------------------------------------------
uint8_t Utility_String_Match
(
  char *pFirst,   // The pFirst string.
  char *pSecond   // The pSecond string.
)
{
  /*
  * DESCRIPTION: Checks if two given strings match. The pFirst string may contain wildcard characters
  * PARAMETERS:  See above.
  * RETURNS:     TRUE/FALSE.
  */

  // If we reach at the end of both strings, we are done
  if (*pFirst == '\0' && *pSecond == '\0')
    return 1;

  // Make sure that the characters after '*' are present
  // in pSecond string. This function assumes that the pFirst
  // string will not contain two consecutive '*'
  if (*pFirst == '*' && *(pFirst + 1) != '\0' && *pSecond == '\0')
    return 1;

  // If the pFirst string contains '?', or current characters
  // of both strings match
  if (*pFirst == '?' || toupper(*pFirst) == toupper(*pSecond))
    return Utility_String_Match(pFirst + 1, pSecond + 1);

  // If there is *, then there are two possibilities
  // a) We consider current character of pSecond string
  // b) We ignore current character of pSecond string.
  if (*pFirst == '*')
    return Utility_String_Match(pFirst + 1, pSecond) || Utility_String_Match(pFirst, pSecond + 1);

  return 0;

} // End Utility_String_Match.

// -----------------------------------------------------------------------------
void Utility_String_Match_Rename
(
  char *pFirst,    // The pFirst string.
  char *pSecond,   // The pSecond string.
  char *pThird     // The new name that results from
)
{
  /*
  * DESCRIPTION: Matches two strings, one of those may contain wildcards, and
  *              fill a third string with the matched string.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  while (*pFirst != '\0' && *pSecond != '\0')
  {
    switch (*pFirst)
  {
    case '*':
      *pThird = *pSecond++;
      if (*pSecond == '.') pFirst++;
      break;

    case '?':
      *pThird = *pSecond++;
      pFirst++;
      break;

    default:
      *pThird = *pFirst++;
      pSecond++;
      break;

  } // End switch.

    pThird++;

  } // End while.

  *pThird = 0;

} // End Utility_String_Match_Rename.

// -----------------------------------------------------------------------------
void Memory_FillCopy
(
    uint8_t *pBufferTo,    // The buffer where to copy into.
    uint8_t *pBufferFrom,  // The buffer where to copy from.
    uint32_t pSize,          
    uint8_t pValue,          
    uint8_t pMode         
)
{
  /*
  * DESCRIPTION: Copy or Fill the content of a block of memory.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing
  */

  uint32_t lData = pSize;

  if (pMode == 0)
  {
    while (lData)
    {
      *pBufferTo++ = *pBufferFrom++;

      lData--;

    } // End while.
  }
  else if (pMode == 1)
  {
    while (lData)
    {
      *pBufferTo++ = pValue;

      lData--;

    } // End while.
    
  } // End if.

} // End Memory_Copy

// -----------------------------------------------------------------------------
void Utility_Show_Bytes
(
  void *pMemoryPointer,   // The pointer to any memory location to start showing the bytes from.
  uint32_t pNumberOfBytes // The amount of bytes to show.
)
{
  /*
  * DESCRIPTION: This function is used to fill the memory with the provided byte.
  *              of the memory and change its size.
  * PARAMETERS:  See above.
  * RETURNS:     Nothing.
  */

  int32_t lBytes = pNumberOfBytes, lByteIndex = 0;
  uint8_t *lAddress = (uint8_t *)pMemoryPointer;

  printf(CURSOR_NEWLINE"Showing %lu bytes from %p:"CURSOR_NEWLINE, pNumberOfBytes, pMemoryPointer);

  // Read all lines and display it.
  while (lBytes > 0)
  {
    printf("%p: ", lAddress);

    // Print hex values.
    for (lByteIndex = 0; lByteIndex < 8; lByteIndex++)
    {
      printf("%02X ", lAddress[lByteIndex]);

      lBytes--;

    } // End for.

    printf(" | ");

    for (lByteIndex = 0; lByteIndex < 8; lByteIndex++)
    {
      printf( "%c", (lAddress[lByteIndex] >= ASCII_SPACE && lAddress[lByteIndex] < ASCII_DELETE) ? lAddress[lByteIndex] : '.');

    } // End for.


    printf(CURSOR_NEWLINE);

    lAddress += 8;

  } // End while.

} // Utility_Show_Bytes.

// /////////////////////////////////////////////////////////////////////////////
