/*
 *  Code Set Converters for MICO
 *  Copyright (C) 1997 Marcus Mueller & Thomas Holubar
 *  Copyright (c) 1997-2010 by The Mico Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 *
 *  date     vers.  name            reason
 *  97/06/27 0.1    mueller         CREATED
 */

#include "nkit/tools.h"

int32_t uni_utf16toucs4(uint32_t *ucs4, const uint16_t *utf16, uint8_t *shorts_read)
{
  if(utf16[0] < 0xD800)
  {
    *ucs4 = utf16[0];
    *shorts_read = 1;
    return true;
  }

  if(utf16[0] < 0xDC00) {
    if( (utf16[1] >= 0xDC00) && (utf16[1] < 0xE000) )
    {
      *ucs4 = ( ( (utf16[0] - 0xD800) << 10 )
               + (utf16[1] - 0xDC00) ) + 0x00010000;
      *shorts_read = 2;
      return true;
    }
    else return false;
  }

  if(utf16[0] < 0xFFFE)
  {
    *ucs4 = utf16[0];
    *shorts_read = 1;
    return true;
  }

  return false;
}

bool uni_ucs4toutf8 (char *utf8, uint32_t  ucs4)
{
  if(ucs4 < 0x00000080)
  {
    utf8[0] = ucs4;
    utf8[1] = '\0';
    return true;
  }

  if(ucs4 < 0x00000800)
  {
    utf8[0] = 0xc0 + (ucs4 >> 6);      // div 2^6
    utf8[1] = 0x80 + (ucs4 & 0x3F);    // mod 2^6
    utf8[2] = '\0';
    return true;
  }

  // in UTF8 N.2 values in the range 0000 D800 to 0000 DFFF 
  // shall be excluded from conversion.                     
  // (I don't see the need of this, because this range is   
  // only relevant in UTF16 to decode 32-bit-values)        
  // -----> 97-09-27 Marcus Mueller                         

  if(ucs4 < 0x00010000)
  {
    utf8[0] = 0xe0 + (ucs4 >> 12);
    utf8[1] = 0x80 + ( (ucs4 >> 6 ) & 0x3F );
    utf8[2] = 0x80 + (ucs4 & 0x3F);
    utf8[3] = '\0';
    return true;
  }

  if(ucs4 < 0x00200000)
  {
    utf8[0] = 0xf0 + (ucs4 >> 18);
    utf8[1] = 0x80 + ( (ucs4 >> 12) & 0x3F );
    utf8[2] = 0x80 + ( (ucs4 >> 6 ) & 0x3F );
    utf8[3] = 0x80 + (ucs4 & 0x3F);
    utf8[4] = '\0';
    return true;
  }

  if(ucs4 < 0x04000000)
  {
    utf8[0] = 0xf8 + (ucs4 >> 24);
    utf8[1] = 0x80 + ( (ucs4 >> 18) & 0x3F );
    utf8[2] = 0x80 + ( (ucs4 >> 12) & 0x3F );
    utf8[3] = 0x80 + ( (ucs4 >> 6 ) & 0x3F );
    utf8[4] = 0x80 + (ucs4 & 0x3F);
    utf8[5] = '\0';
    return true;
  }

  if(ucs4 < 0x80000000)
  {
    utf8[0] = 0xfc + (ucs4 >> 30);
    utf8[1] = 0x80 + ( (ucs4 >> 24) & 0x3F );
    utf8[2] = 0x80 + ( (ucs4 >> 18) & 0x3F );
    utf8[3] = 0x80 + ( (ucs4 >> 12) & 0x3F );
    utf8[4] = 0x80 + ( (ucs4 >> 6 ) & 0x3F );
    utf8[5] = 0x80 + (ucs4 & 0x3F);
    utf8[6] = '\0';
    return true;
  }

  return false;
}

bool uni_ucs4toutf16(uint16_t *utf16, uint32_t ucs4)
{
  if(ucs4 <= 0x0000FFFD)
  {
    utf16[0] = (uint16_t)ucs4;
    utf16[1] = 0l;
    return true;
  }

  if( (ucs4 >= 0x00010000) && (ucs4 <= 0x0010FFFF) )
  {
    utf16[0] = ((ucs4 - 0x00010000) >> 10) + 0xD800;
    utf16[1] = ((ucs4 - 0x00010000) & 0x3FF) + 0xDC00;
    utf16[2] = 0l;
    return true;
  }

  return false;
}

int32_t uni_utf8toucs4(uint32_t *ucs4, const char *utf8,
             uint8_t *bytes_read)
{
  register const uint8_t *utf=(uint8_t*)utf8;
  register uint8_t read=0;
  register uint32_t RC=0;

  if( (utf[0]) && ( (utf[0] & 0xC0) != 0x80 ) )
  {
    if(utf[0] < 0xC0)
    {
      RC = utf[0];
      read = 1;
    }
    else if( (utf[1] & 0xC0) == 0x80 )
    {
      if(utf[0] < 0xE0)
      {
        RC = ( ( (utf[0]-0xC0) << 6 )
               + (utf[1]-0x80) );
        read = 2;
      }
      else if( (utf[2] & 0xC0) == 0x80 )
      {
        if(utf[0] < 0xF0)
        {
          RC = ( ( (utf[0]-0xE0) << 12 )
               + ( (utf[1]-0x80) << 6 )
               +   (utf[2]-0x80) );
          read = 3;
        }
        else if( (utf[3] & 0xC0) == 0x80 )
        {
          if(utf[0] < 0xF8)
          {
            RC = ( ( (utf[0]-0xF0) << 18 )
                 + ( (utf[1]-0x80) << 12 )
                 + ( (utf[2]-0x80) << 6 )
                 +   (utf[3]-0x80) );
            read = 4;
          }
          else if( (utf[4] & 0xC0) == 0x80 )
          {
            if(utf[0] < 0xFC)
            {
              RC = ( ( (utf[0]-0xF8) << 24 )
                   + ( (utf[1]-0x80) << 18 )
                   + ( (utf[2]-0x80) << 12 )
                   + ( (utf[3]-0x80) << 6 )
                   +   (utf[4]-0x80) );
              read = 4;
            }
            else if( (utf[5] & 0xC0) == 0x80 )
            {
              if(utf[0] < 0xFE)
              {
                RC = ( ( (utf[0]-0xFC) << 30 )
                     + ( (utf[1]-0x80) << 24 )
                     + ( (utf[2]-0x80) << 18 )
                     + ( (utf[3]-0x80) << 12 )
                     + ( (utf[4]-0x80) << 6 )
                     +   (utf[5]-0x80) );
                read = 5;
              }
            }
          }
        }
      }
    }
  }

  if(read)
  {
    if(bytes_read) *bytes_read = read;
    if(ucs4) *ucs4 = RC;
    return true;
  }

  return false;
}

bool utf8_to_utf16(uint16_t *utf16, const char *utf8, uint8_t *bytes_read)
{
  uint32_t character;
  if (!uni_utf8toucs4(&character, utf8, bytes_read))
    return false;
  return uni_ucs4toutf16(utf16, character);
}

bool utf16_to_utf8(char *utf8, const uint16_t *utf16, uint8_t *shorts_read)
{
  uint32_t character;
  if (!uni_utf16toucs4(&character, utf16, shorts_read))
    return false;
  return uni_ucs4toutf8(utf8, character);
}

uint8_t utf8bytes(char first_utf8_byte)
{
  register uint8_t RC=0;
  register uint8_t first = (uint8_t)first_utf8_byte;

  if(first < 0x80)
    RC = 1;
  else if(first < 0xC0) // 10xxxxxx
    RC = 0;             // ERROR
  else if(first < 0xE0) // 110xxxxx
    RC = 2;
  else if(first < 0xF0) // 1110xxxx
    RC = 3;
  else if(first < 0xF8) // 11110xxx
    RC = 4;
  else if(first < 0xFC) // 111110xx
    RC = 5;
  else if(first < 0xFE) // 1111110x
    RC = 6;
  else                  // ERROR
    RC = 0;

  return(RC);
}
