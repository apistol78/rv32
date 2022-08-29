/* ScummVM - Scumm Interpreter
 * Copyright (C) 2002-2004 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header: /cvsroot/scummvm/scummvm/scumm/help.h,v 1.7 2004/01/06 12:45:30 fingolfin Exp $
 *
 */

#ifndef SCUMM_HELP_H
#define SCUMM_HELP_H

#include "common/str.h"

namespace Scumm {

#define HELP_NUM_LINES 15

class ScummHelp {
protected:
	typedef Common::String String;

public:
	static int numPages(byte gameId);
	static void updateStrings(byte gameId, byte version, int page,
			String &title, String *&key, String *&dsc);
};

} // End of namespace Scumm

#endif

