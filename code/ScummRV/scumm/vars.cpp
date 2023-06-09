/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001  Ludvig Strigeus
 * Copyright (C) 2001-2004 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header: /cvsroot/scummvm/scummvm/scumm/vars.cpp,v 1.71 2004/02/11 15:59:25 kirben Exp $
 *
 */


#include "stdafx.h"
#include "scumm/scumm.h"
#include "scumm/intern.h"

namespace Scumm {

void ScummEngine::setupScummVars() {
	VAR_KEYPRESS = 0;
	VAR_EGO = 1;
	VAR_CAMERA_POS_X = 2;
	VAR_HAVE_MSG = 3;
	VAR_ROOM = 4;
	VAR_OVERRIDE = 5;
	VAR_MACHINE_SPEED = 6;
	VAR_ME = 7;
	VAR_NUM_ACTOR = 8;
	VAR_CURRENTDRIVE = 10;
	VAR_TMR_1 = 11;
	VAR_TMR_2 = 12;
	VAR_TMR_3 = 13;
	VAR_MUSIC_TIMER = 14;
	VAR_ACTOR_RANGE_MIN = 15;
	VAR_ACTOR_RANGE_MAX = 16;
	VAR_CAMERA_MIN_X = 17;
	VAR_CAMERA_MAX_X = 18;
	VAR_TIMER_NEXT = 19;
	VAR_VIRT_MOUSE_X = 20;
	VAR_VIRT_MOUSE_Y = 21;
	VAR_ROOM_RESOURCE = 22;
	VAR_LAST_SOUND = 23;
	VAR_CUTSCENEEXIT_KEY = 24;
	VAR_TALK_ACTOR = 25;
	VAR_CAMERA_FAST_X = 26;
	VAR_SCROLL_SCRIPT = 27;
	VAR_ENTRY_SCRIPT = 28;
	VAR_ENTRY_SCRIPT2 = 29;
	VAR_EXIT_SCRIPT = 30;
	VAR_EXIT_SCRIPT2 = 31;
	VAR_VERB_SCRIPT = 32;
	VAR_SENTENCE_SCRIPT = 33;
	VAR_INVENTORY_SCRIPT = 34;
	VAR_CUTSCENE_START_SCRIPT = 35;
	VAR_CUTSCENE_END_SCRIPT = 36;
	VAR_CHARINC = 37;
	VAR_WALKTO_OBJ = 38;
	VAR_DEBUGMODE = 39;
	VAR_HEAPSPACE = 40;
	VAR_RESTART_KEY = 42;
	VAR_PAUSE_KEY = 43;
	VAR_MOUSE_X = 44;
	VAR_MOUSE_Y = 45;
	VAR_TIMER = 46;
	VAR_TMR_4 = 47;
	VAR_SOUNDCARD = 48;
	VAR_VIDEOMODE = 49;
	VAR_MAINMENU_KEY = 50;
	VAR_FIXEDDISK = 51;
	VAR_CURSORSTATE = 52;
	VAR_USERPUT = 53;
	VAR_SOUNDRESULT = 56;
	VAR_TALKSTOP_KEY = 57;
	VAR_NOSUBTITLES = 60; // for loomcd

	VAR_SOUNDPARAM = 64;
	VAR_SOUNDPARAM2 = 65;
	VAR_SOUNDPARAM3 = 66;
	VAR_MOUSEPRESENT = 67;
	VAR_PERFORMANCE_1 = 68;
	if (!(_features & GF_SMALL_HEADER)) {
		VAR_PERFORMANCE_2 = 69;	// Zak256 Note: Cashcard for Zak
		VAR_ROOM_FLAG = 70;	// Zak256 Note: Cashcard for Annie
		VAR_GAME_LOADED = 71;	// Zak256 Note: Cashcard for Melissa
		VAR_NEW_ROOM = 72;	// Zak256 Note: Cashcard for Leslie
	}
	VAR_VERSION = 75;
}

void ScummEngine_v2::setupScummVars() {
	VAR_EGO = 0;
	VAR_CAMERA_POS_X = 2;
	VAR_HAVE_MSG = 3;
	VAR_ROOM = 4;
	VAR_OVERRIDE = 5;
	VAR_MACHINE_SPEED = 6;
	VAR_CHARCOUNT = 7;
	VAR_ACTIVE_VERB = 8;
	VAR_ACTIVE_OBJECT1 = 9;
	VAR_ACTIVE_OBJECT2 = 10;
	VAR_NUM_ACTOR = 11;
	VAR_CURRENT_LIGHTS = 12;
	VAR_CURRENTDRIVE = 13;
	VAR_MUSIC_TIMER = 17;
	VAR_VERB_ALLOWED = 18;
	VAR_ACTOR_RANGE_MIN = 19;
	VAR_ACTOR_RANGE_MAX = 20;
	VAR_CURSORSTATE = 21;
	VAR_CAMERA_MIN_X = 23;
	VAR_CAMERA_MAX_X = 24;
	VAR_TIMER_NEXT = 25;
	VAR_SENTENCE_VERB = 26;
	VAR_SENTENCE_OBJECT1 = 27;
	VAR_SENTENCE_OBJECT2 = 28;
	VAR_SENTENCE_PREPOSITION = 29;
	VAR_VIRT_MOUSE_X = 30;
	VAR_VIRT_MOUSE_Y = 31;
	VAR_CLICK_AREA = 32;
	VAR_ROOM_RESOURCE = 36;
	VAR_LAST_SOUND = 37;
	VAR_BACKUP_VERB = 38;
	VAR_KEYPRESS = 39;
	VAR_CUTSCENEEXIT_KEY = 40;
	VAR_TALK_ACTOR = 41;
}

void ScummEngine_v5::setupScummVars() {
	// Many vars are the same as in V5 & V6 games, so just call the inherited method first
	ScummEngine::setupScummVars();

	VAR_CURRENT_LIGHTS = 9;
	VAR_V5_TALK_STRING_Y = 54;
}

void ScummEngine_v6::setupScummVars() {
	// Many vars are the same as in V5 & V6 games, so just call the inherited method first
	ScummEngine::setupScummVars();

	VAR_V6_SCREEN_WIDTH = 41;
	VAR_V6_SCREEN_HEIGHT = 54;
	VAR_V6_EMSSPACE = 76;
	VAR_RANDOM_NR = 118;
	
	VAR_V6_SOUNDMODE = 9;

	VAR_TIMEDATE_YEAR = 119;
	VAR_TIMEDATE_MONTH = 129;
	VAR_TIMEDATE_DAY = 128;
	VAR_TIMEDATE_HOUR = 125;
	VAR_TIMEDATE_MINUTE = 126;
}

void ScummEngine_v7::setupScummVars() {
	VAR_MOUSE_X = 1;
	VAR_MOUSE_Y = 2;
	VAR_VIRT_MOUSE_X = 3;
	VAR_VIRT_MOUSE_Y = 4;
	VAR_V6_SCREEN_WIDTH = 5;
	VAR_V6_SCREEN_HEIGHT = 6;
	VAR_CAMERA_POS_X = 7;
	VAR_CAMERA_POS_Y = 8;
	VAR_OVERRIDE = 9;
	VAR_ROOM = 10;
	VAR_ROOM_RESOURCE = 11;
	VAR_TALK_ACTOR = 12;
	VAR_HAVE_MSG = 13;
	VAR_TIMER = 14;
	VAR_TMR_4 = 15;
	VAR_LEFTBTN_DOWN = 22;
	VAR_RIGHTBTN_DOWN = 23;
	VAR_LEFTBTN_HOLD = 24;
	VAR_RIGHTBTN_HOLD = 25;

	VAR_PERFORMANCE_1 = 26;
	VAR_PERFORMANCE_2 = 27;
	VAR_GAME_LOADED = 29;
	VAR_V6_EMSSPACE = 32;
	VAR_RANDOM_NR = 34;
	VAR_NEW_ROOM = 35;
	VAR_WALKTO_OBJ = 36;

	VAR_CAMERA_DEST_X = 38;
	VAR_CAMERA_DEST_Y = 39;
	VAR_CAMERA_FOLLOWED_ACTOR = 40;

	VAR_SCROLL_SCRIPT = 50;
	VAR_ENTRY_SCRIPT = 51;
	VAR_ENTRY_SCRIPT2 = 52;
	VAR_EXIT_SCRIPT = 53;
	VAR_EXIT_SCRIPT2 = 54;
	VAR_VERB_SCRIPT = 55;
	VAR_SENTENCE_SCRIPT = 56;
	VAR_INVENTORY_SCRIPT = 57;
	VAR_CUTSCENE_START_SCRIPT = 58;
	VAR_CUTSCENE_END_SCRIPT = 59;
	VAR_SAVELOAD_SCRIPT = 60;
	VAR_SAVELOAD_SCRIPT2 = 61;

	VAR_CUTSCENEEXIT_KEY = 62;
	VAR_RESTART_KEY = 63;			// ???
	VAR_PAUSE_KEY = 64;
	VAR_MAINMENU_KEY = 65;	// ???
	VAR_TALKSTOP_KEY = 67;
	VAR_KEYPRESS = 118;

	VAR_TIMER_NEXT = 97;
	VAR_TMR_1 = 98;
	VAR_TMR_2 = 99;
	VAR_TMR_3 = 100;

	VAR_CAMERA_MIN_X = 101;
	VAR_CAMERA_MAX_X = 102;
	VAR_CAMERA_MIN_Y = 103;
	VAR_CAMERA_MAX_Y = 104;
	VAR_CAMERA_THRESHOLD_X = 105;
	VAR_CAMERA_THRESHOLD_Y = 106;
	VAR_CAMERA_SPEED_X = 107;
	VAR_CAMERA_SPEED_Y = 108;
	VAR_CAMERA_ACCEL_X = 109;
	VAR_CAMERA_ACCEL_Y = 110;

	VAR_EGO = 111;

	VAR_CURSORSTATE = 112;
	VAR_USERPUT = 113;
	VAR_DEFAULT_TALK_DELAY = 114;
	VAR_CHARINC = 115;
	VAR_DEBUGMODE = 116;

	VAR_CHARSET_MASK = 119;
	VAR_VIDEONAME = 123;
	VAR_CUSTOMSCALETABLE = 131;

	VAR_STRING2DRAW = 130;
}

void ScummEngine_v8::setupScummVars() {
	// VAR_ROOM_HEIGHT = 1;
	// VAR_ROOM_WIDTH = 2;

	VAR_MOUSE_X = 3;
	VAR_MOUSE_Y = 4;
	VAR_VIRT_MOUSE_X = 5;
	VAR_VIRT_MOUSE_Y = 6;

	VAR_CURSORSTATE = 7;
	VAR_USERPUT = 8;

	VAR_CAMERA_POS_X = 9;
	VAR_CAMERA_POS_Y = 10;
	VAR_CAMERA_DEST_X = 11;
	VAR_CAMERA_DEST_Y = 12;
	VAR_CAMERA_FOLLOWED_ACTOR = 13;

	VAR_TALK_ACTOR = 14;
	VAR_HAVE_MSG = 15;
	VAR_MOUSE_BUTTONS = 16;
	VAR_RIGHTBTN_HOLD = 17; // TODO: Guess (used in room 10 script 2004, and ship-to-ship combat)
	VAR_MOUSE_HOLD = 18;

	VAR_TIMEDATE_YEAR = 24;
	VAR_TIMEDATE_MONTH = 25;
	VAR_TIMEDATE_DAY = 26;
	VAR_TIMEDATE_HOUR = 27;
	VAR_TIMEDATE_MINUTE = 28;
	VAR_TIMEDATE_SECOND = 29;

	VAR_OVERRIDE = 30;
	VAR_ROOM = 31;

	//VAR_VOICE_MODE = 39; // 0 is voice, 1 is voice+text, 2 is text only
	VAR_GAME_LOADED = 40;
	VAR_LANGUAGE = 41;
	
	VAR_CURRENTDISK = 42;

	VAR_SCROLL_SCRIPT = 50;
	VAR_ENTRY_SCRIPT = 51;
	VAR_ENTRY_SCRIPT2 = 52;
	VAR_EXIT_SCRIPT = 53;
	VAR_EXIT_SCRIPT2 = 54;
	VAR_VERB_SCRIPT = 55;
	VAR_SENTENCE_SCRIPT = 56;
	VAR_INVENTORY_SCRIPT = 57;
	VAR_CUTSCENE_START_SCRIPT = 58;
	VAR_CUTSCENE_END_SCRIPT = 59;

	VAR_CUTSCENEEXIT_KEY = 62;

	VAR_PAUSE_KEY = 64;
	VAR_MAINMENU_KEY = 65;
	VAR_TALKSTOP_KEY = 67;

	VAR_CUSTOMSCALETABLE = 111;

	VAR_TIMER_NEXT = 112;
	VAR_TMR_1 = 113;
	VAR_TMR_2 = 114;
	VAR_TMR_3 = 115;

	VAR_CAMERA_MIN_X = 116;
	VAR_CAMERA_MAX_X = 117;
	VAR_CAMERA_MIN_Y = 118;
	VAR_CAMERA_MAX_Y = 119;
	VAR_CAMERA_SPEED_X = 120;
	VAR_CAMERA_SPEED_Y = 121;
	VAR_CAMERA_ACCEL_X = 122;
	VAR_CAMERA_ACCEL_Y = 123;
	VAR_CAMERA_THRESHOLD_X = 124;
	VAR_CAMERA_THRESHOLD_Y = 125;

	VAR_EGO = 126;

	VAR_DEBUGMODE = 130;
	VAR_KEYPRESS = 132;
	VAR_SYNC = 134;

	VAR_CHARINC = 221;
}

} // End of namespace Scumm
