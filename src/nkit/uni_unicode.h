/*
 *  Code Set Converters for MICO
 *  Copyright (C) 1997 Marcus Mueller & Thomas Holubar
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
 *  Send comments and/or bug reports to:
 *                 mico@informatik.uni-frankfurt.de
 *
 *  date     vers.  name            reason
 *  97/06/27 0.1    mueller         CREATED
 */

#ifndef _UNICODE_H
#define _UNICODE_H

#include "nkit/types.h"

bool    uni_utf16toutf8(char *utf8, const uint16_t *utf16, uint8_t *shorts_read);
bool    uni_utf8toutf16(uint16_t *utf16, const char *utf8, uint8_t *bytes_read);
uint8_t uni_utf8bytes(char first_utf8_byte);


/* String length:
** - utf7:  1 character has max. 5 Bytes  plus 1 Byte  NULL-terminator.
** - utf8:  1 character has max. 6 Bytes  plus 1 Byte  NULL-terminator.
** - utf16: 1 character has max. 2 Shorts plus 1 Short NULL-terminator.
*/

#endif
