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
 * $Header: /cvsroot/scummvm/scummvm/gui/EditTextWidget.h,v 1.14 2004/02/05 00:19:54 fingolfin Exp $
 */

#ifndef EDITTEXTWIDGET_H
#define EDITTEXTWIDGET_H

#include "gui/widget.h"
#include "common/str.h"

namespace GUI {

/* EditTextWidget */
class EditTextWidget : public StaticTextWidget {
	typedef Common::StringList StringList;
	typedef Common::String String;
protected:
	String		_backupString;
	bool		_caretVisible;
	uint32		_caretTime;
	int			_pos;
	int			_labelOffset;
public:
	EditTextWidget(GuiObject *boss, int x, int y, int w, int h, const String &text);

	virtual void handleTickle();
	virtual void handleMouseDown(int x, int y, int button, int clickCount);
	virtual bool handleKeyDown(uint16 ascii, int keycode, int modifiers);

	virtual bool wantsFocus() { return true; };

protected:
	void drawWidget(bool hilite);
	void drawCaret(bool erase);
	void lostFocusWidget() { _backupString = _label; drawCaret(true); }

	int getCaretPos() const;
	bool adjustOffset();
};

} // End of namespace GUI

#endif
