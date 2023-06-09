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
 * $Header: /cvsroot/scummvm/scummvm/scumm/string.cpp,v 1.193.2.1 2004/02/22 08:04:33 kirben Exp $
 *
 */

#include "stdafx.h"

#include "common/config-manager.h"

#include "scumm/scumm.h"
#include "scumm/actor.h"
#include "scumm/charset.h"
#include "scumm/dialogs.h"
#include "scumm/imuse_digi/dimuse.h"
#include "scumm/verbs.h"
#include "scumm/sound.h"

namespace Scumm {

void ScummEngine::setStringVars(int slot) {
	StringTab *st = &_string[slot];
	st->xpos = st->t_xpos;
	st->ypos = st->t_ypos;
	st->center = st->t_center;
	st->overhead = st->t_overhead;
	st->no_talk_anim = st->t_no_talk_anim;
	st->right = st->t_right;
	st->color = st->t_color;
	st->charset = st->t_charset;
}

void ScummEngine::unkMessage1() {
	byte buffer[100];
	_msgPtrToAdd = buffer;
	_messagePtr = addMessageToStack(_messagePtr);

//	if ((_gameId == GID_CMI) && _debugMode) {	// In CMI, unkMessage1 is used for printDebug output
	if ((buffer[0] != 0xFF) && _debugMode) {
		debug(0, "DEBUG: %s", buffer);
		return;
	}

	if (buffer[0] == 0xFF && buffer[1] == 10) {
		uint32 a, b;

		a = buffer[2] | (buffer[3] << 8) | (buffer[6] << 16) | (buffer[7] << 24);
		b = buffer[10] | (buffer[11] << 8) | (buffer[14] << 16) | (buffer[15] << 24);

		// Sam and Max uses a caching system, printing empty messages
		// and setting VAR_V6_SOUNDMODE beforehand. See patch 609791.
		// FIXME: There are other VAR_V6_SOUNDMODE states, as
		// mentioned in the patch. FIXME after iMUSE is done.
		if (_gameId != GID_SAMNMAX || (VAR(VAR_V6_SOUNDMODE) != 2))
			_sound->talkSound(a, b, 1, -1);
	}
}

void ScummEngine::unkMessage2() {
	byte buf[100];
	const byte *tmp;

	_msgPtrToAdd = buf;
	tmp = _messagePtr = addMessageToStack(_messagePtr);

	if (_string[3].color == 0)
		_string[3].color = 4;

	// FIXME: I know this is the right thing to do for MI1 and MI2. For
	// all other games it's just a guess.
	InfoDialog dialog(this, (char*)buf);
	VAR(VAR_KEYPRESS) = runDialog(dialog);

	_messagePtr = tmp;
}

void ScummEngine::CHARSET_1() {
	uint32 talk_sound_a = 0;
	uint32 talk_sound_b = 0;
	int s, i, t, c;
	int frme = -1;
	Actor *a;
	byte *buffer;
	bool has_talk_sound = false;
	bool has_anim = false;

	if (!_haveMsg)
		return;

	if (!(_features & GF_NEW_CAMERA) && !(_gameId == GID_ZAK256 && talkingActor() == 0xFF)) {
		if ((camera._dest.x / 8) != (camera._cur.x / 8) || camera._cur.x != camera._last.x)
			return;
	}

	a = NULL;
	if (talkingActor() != 0xFF)
		a = derefActorSafe(talkingActor(), "CHARSET_1");

	if (a && _string[0].overhead != 0) {
		if (_version <= 5) {
			_string[0].xpos = a->_pos.x - camera._cur.x + (_screenWidth / 2);

			if (VAR(VAR_V5_TALK_STRING_Y) < 0) {
				s = (a->scaley * (int)VAR(VAR_V5_TALK_STRING_Y)) / 0xFF;
				_string[0].ypos = (int)(((VAR(VAR_V5_TALK_STRING_Y) - s) / 2) + s - a->getElevation() + a->_pos.y);
			} else {
				_string[0].ypos = (int)VAR(VAR_V5_TALK_STRING_Y);
			}

		} else {
			s = a->scaley * a->talkPosY / 0xFF;
			_string[0].ypos = ((a->talkPosY - s) / 2) + s - a->getElevation() + a->_pos.y;

			if (_string[0].ypos < _screenTop)
				_string[0].ypos = _screenTop;

			s = a->scalex * a->talkPosX / 0xFF;
			_string[0].xpos = ((a->talkPosX - s) / 2) + s + a->_pos.x - camera._cur.x + (_screenWidth / 2);
		}

		if (_string[0].ypos < 1)
			_string[0].ypos = 1;

		if (_string[0].xpos < 80)
			_string[0].xpos = 80;
		if (_string[0].xpos > _screenWidth - 80)
			_string[0].xpos = _screenWidth - 80;
	}

	_charset->_top = _string[0].ypos + _screenTop;
	_charset->_startLeft = _charset->_left = _string[0].xpos;

	if (a && a->charset)
		_charset->setCurID(a->charset);
	else
		_charset->setCurID(_string[0].charset);

	_charset->_center = _string[0].center;
	_charset->_right = _string[0].right;
	_charset->setColor(_charsetColor);

	if (_version > 3)	// FIXME
		for (i = 0; i < 4; i++)
			_charsetColorMap[i] = _charsetData[_charset->getCurID()][i];

	if (_keepText) {
		_charset->_str = _charset->_mask;
	}

	if (_talkDelay)
		return;

	if (_haveMsg == 1) {
		if ((_sound->_sfxMode & 2) == 0)
			stopTalk();
		return;
	}

	if (a && !_string[0].no_talk_anim) {
		has_anim = true;
		_useTalkAnims = true;
	}

	// HACK: Most of the audio sync in Loom is handled by the "MI1
	// timer", but some of it depends on text strings timing out at
	// the right moment.

	if (_gameId == GID_LOOM256) {
		_defaultTalkDelay = 100;
		VAR(VAR_CHARINC) = 5;
	}

	_talkDelay = _defaultTalkDelay;

	if (!_keepText) {
		if (_version <= 3 && _gameId != GID_LOOM) {
			_charset->_hasMask = true;
			_charset->_mask.left = _string[0].xpos;
			_charset->_mask.top = _string[0].ypos;
			_charset->_mask.bottom = _string[0].ypos + 8;
			_charset->_mask.right = _screenWidth;
			if (_string[0].ypos <= 16)	// If we are cleaning the text line, clean 2 lines.
				_charset->_mask.bottom = 16;
		}
		_charset->restoreCharsetBg();
	}

	t = _charset->_right - _string[0].xpos - 1;
	if (_charset->_center) {
		if (t > _charset->_nextLeft)
			t = _charset->_nextLeft;
		t *= 2;
	}

	buffer = _charsetBuffer + _charsetBufPos;
	if (_version > 3)
		_charset->addLinebreaks(0, buffer, 0, t);

	if (_charset->_center) {
		_charset->_nextLeft -= _charset->getStringWidth(0, buffer) / 2;
		if (_charset->_nextLeft < 0)
			_charset->_nextLeft = 0;
	}

	_charset->_disableOffsX = _charset->_firstChar = !_keepText;

	do {
		c = *buffer++;
		if (c == 0) {
			// End of text reached, set _haveMsg to 1 so that the text will be
			// removed next time CHARSET_1 is called.
			_haveMsg = 1;
			_keepText = false;
			break;
		}
		if (c == 13) {
		newLine:;
			_charset->_nextLeft = _string[0].xpos;
			if (_charset->_center) {
				_charset->_nextLeft -= _charset->getStringWidth(0, buffer) / 2;
			}
			_charset->_nextTop += _charset->getFontHeight();
			if (_version > 3) {
				// FIXME - is this really needed?
				_charset->_disableOffsX = true;
			}
			continue;
		}

		if (c == 0xFE || c == 0xFF) {
			c = *buffer++;
			switch(c) {
			case 1:
				goto newLine;
			case 2:
				_haveMsg = 0;
				_keepText = true;
				break;
			case 3:
				if (_haveMsg != 0xFE)
					_haveMsg = 0xFF;
				_keepText = false;
				break;
			case 9:
				frme = *buffer++;
				frme |= *buffer++ << 8;
				has_anim = true;
				break;
			case 10:
				talk_sound_a = buffer[0] | (buffer[1] << 8) | (buffer[4] << 16) | (buffer[5] << 24);
				talk_sound_b = buffer[8] | (buffer[9] << 8) | (buffer[12] << 16) | (buffer[13] << 24);
				has_talk_sound = true;
				buffer += 14;
	
				// Set flag that speech variant exist of this msg.
				// TODO: This does not work for the speech system in V7+ games
				// since they encode the voice information differently, and it
				// is being stripped from the string before it ever gets here.
				if (_haveMsg == 0xFF)
					_haveMsg = 0xFE;
				break;
			case 12:
				int color;
				color = *buffer++;
				color |= *buffer++ << 8;
				if (color == 0xFF)
					_charset->setColor(_charsetColor);
				else
					_charset->setColor(color);
				break;
			case 13:
				warning("CHARSET_1: Unknown opcode 13 %d", READ_LE_UINT16(buffer));
				buffer += 2;
				break;
			case 14: {
				int oldy = _charset->getFontHeight();
	
				_charset->setCurID(*buffer++);
				buffer += 2;
				for (i = 0; i < 4; i++)
					_charsetColorMap[i] = _charsetData[_charset->getCurID()][i];
				_charset->_nextTop -= _charset->getFontHeight() - oldy;
				break;
				}
			default:
				warning("CHARSET_1: invalid code %d", c);
			}
		} else {
			_charset->_left = _charset->_nextLeft;
			_charset->_top = _charset->_nextTop;
			if (c & 0x80 && _CJKMode)
				if (_language == 6 && ((c > 0x84 && c < 0x88) || (c > 0x9f && c < 0xe0) || (c > 0xea && c <= 0xff)))
					c = 0x20; //not in S-JIS
				else
					c += *buffer++ * 256;
			if (_version <= 3) {
				_charset->printChar(c);
			} else {
				if (!ConfMan.getBool("subtitles") && (_imuseDigital && _sound->isSoundRunning(kTalkSoundID))) {
					// Special case for games using imuse digital.for sound
				} else if ((_gameId == GID_LOOM256) && !ConfMan.getBool("subtitles") && (_sound->pollCD())) {
					// Special case for loomcd, since it only uses CD audio.for sound
				} else if (!ConfMan.getBool("subtitles") && (_haveMsg == 0xFE || _sound->_talkChannelHandle.isActive())) {
					// Subtitles are turned off, and there is a voice version
					// of this message -> don't print it. 
				} else
					_charset->printChar(c);
			}

			_charset->_nextLeft = _charset->_left;
			_charset->_nextTop = _charset->_top;
			if (_version <= 2) {
				_talkDelay += _defaultTalkDelay / 20;
				VAR(VAR_CHARCOUNT)++;
			} else
				_talkDelay += (int)VAR(VAR_CHARINC);
		}
	} while (c != 2 && c != 3);

	// Even if talkSound() is called, we may still have to call
	// startAnimActor() since actorTalk() may already have caused the
	// wrong animation frame to be drawn, and the talkSound() won't be
	// processed until after the next screen update. Bleah.

	if (has_talk_sound)
		_sound->talkSound(talk_sound_a, talk_sound_b, 2, frme);
	if (a && has_anim)
		a->startAnimActor(frme != -1 ? frme : a->talkStartFrame);

	_charsetBufPos = buffer - _charsetBuffer;

	_charset->_hasMask = (_charset->_str.left != -1);
	_charset->_mask = _charset->_str;
}

void ScummEngine::drawString(int a) {
	byte buf[256];
	byte *space;
	int i, c;
	byte fontHeight = 0;
	uint color;

	_msgPtrToAdd = buf;
	_messagePtr = addMessageToStack(_messagePtr);

	_charset->_top = _string[a].ypos + _screenTop;
	_charset->_startLeft = _charset->_left = _string[a].xpos;
	_charset->_right = _string[a].right;
	_charset->_center = _string[a].center;
	_charset->setColor(_string[a].color);
	_charset->_disableOffsX = _charset->_firstChar = true;
	_charset->setCurID(_string[a].charset);

	if (_version > 3) {
		for (i = 0; i < 4; i++)
			_charsetColorMap[i] = _charsetData[_charset->getCurID()][i];
	}

	fontHeight = _charset->getFontHeight();

	_msgPtrToAdd = buf;

	// trim from the right
	space = NULL;
	while (*_msgPtrToAdd) {
		if (*_msgPtrToAdd == ' ') {
			if (!space)
				space = _msgPtrToAdd;
		} else {
			space = NULL;
		}
		_msgPtrToAdd++;
	}
	if (space)
		*space = '\0';
	if (_charset->_center) {
		_charset->_left -= _charset->getStringWidth(a, buf) / 2;
	}

	if (_version < 7)
		_charset->_ignoreCharsetMask = true;


	// In Full Throttle (and other games?), verb text should always mask
	// and never time out. We can't do it blindly for all games, because
	// it causes problem with the FOA intro.

	if ((_gameId == GID_FT) && a == 4)
		_talkDelay = -1;

	if (!buf[0]) {
		buf[0] = ' ';
		buf[1] = 0;
	}

	for (i = 0; (c = buf[i++]) != 0;) {
		if (c == 0xFE || c == 0xFF) {
			c = buf[i++];
			switch (c) {
			case 9:
			case 10:
			case 13:
			case 14:
				i += 2;
				break;
			case 1:
			case 8:
				if (_charset->_center) {
					_charset->_left = _charset->_startLeft - _charset->getStringWidth(a, buf + i);
				} else {
					_charset->_left = _charset->_startLeft;
				}
				_charset->_top += fontHeight;
				break;
			case 12:
				color = buf[i] + (buf[i + 1] << 8);
				i += 2;
				if (color == 0xFF)
					_charset->setColor(_string[a].color);
				else
					_charset->setColor(color);
				break;
			}
		} else {
			if (a == 1 && _version >= 6) {
				// FIXME: The following code is a bit nasty. It is used for the
				// Highway surfing game in Sam&Max; there, _blitAlso is set to
				// true when writing the highscore numbers. It is also in DOTT
				// for parts the intro and for drawing newspaper headlines. It
				// is also used for scores in bowling mini game in fbear and
				// for names in load/save screen of all HE games. Maybe it is
				// also being used in other places.
				//
				// A better name for _blitAlso might be _imprintOnBackground

				if (_string[a].no_talk_anim == false) {
					// Sam and Max seems to blitAlso 32 a lot, which does
					// nothing anyway. So just hide that one for brevity.
					if (c != 32) 
						debug(1, "Would have set _charset->_blitAlso = true (wanted to print '%c' = %d)", c, c);
					_charset->_blitAlso = true;
				}
			}
			if (c >= 0x80 && _CJKMode)
				c += buf[i++] * 256;
			_charset->printChar(c);
			_charset->_blitAlso = false;
		}
	}

	_charset->_ignoreCharsetMask = false;

	if (a == 0) {
		_charset->_nextLeft = _charset->_left;
		_charset->_nextTop = _charset->_top;
	}


	_string[a].xpos = _charset->_str.right + 8;	// Indy3: Fixes Grail Diary text positioning

	if (_version >= 7) {
		_charset->_hasMask = true;
		_charset->_mask.extend(_charset->_str);
	}
}

const byte *ScummEngine::addMessageToStack(const byte *msg) {
	uint num = 0;
	uint32 val;
	byte chr;
	byte buf[512];

	if (msg == NULL) {
		warning("Bad message in addMessageToStack, ignoring");
		return NULL;
	}

	while ((buf[num++] = chr = *msg++) != 0) {
		if (num >= sizeof(buf))
			error("Message stack overflow");

		if (chr == 0xff) {	// 0xff is an escape character
			buf[num++] = chr = *msg++;	// followed by a "command" code
			if (chr != 1 && chr != 2 && chr != 3 && chr != 8) {
				buf[num++] = *msg++;	// and some commands are followed by parameters to the functions below
				buf[num++] = *msg++;	// these are numbers of names, strings, verbs, variables, etc
				if (_version == 8) {
					buf[num++] = *msg++;
					buf[num++] = *msg++;
				}
			}
		}
	}

	num = 0;

	while (1) {
		chr = buf[num++];
		if (chr == 0)
			break;
		if (chr == 0xFF) {
			chr = buf[num++];
			if (chr == 1 || chr == 2 || chr == 3 || chr == 8) {
				// Simply copy these special codes
				*_msgPtrToAdd++ = 0xFF;
				*_msgPtrToAdd++ = chr;
			} else {
				val = (_version == 8) ? READ_LE_UINT32(buf + num) : READ_LE_UINT16(buf + num);
				switch (chr) {
				case 4:
					addIntToStack(val);
					break;
				case 5:
					addVerbToStack(val);
					break;
				case 6:
					addNameToStack(val);
					break;
				case 7:
					if (_version <= 2) {
						while ((chr = (byte) _scummVars[val++])) {
							if (chr != '@')
								*_msgPtrToAdd++ = chr;
						}
					} else {
						addStringToStack(val);
					}
					break;
				case 9:
				case 10:
				case 12:
				case 13:
				case 14:
					// Simply copy these special codes
					*_msgPtrToAdd++ = 0xFF;
					*_msgPtrToAdd++ = chr;
					*_msgPtrToAdd++ = buf[num+0];
					*_msgPtrToAdd++ = buf[num+1];
					if (_version == 8) {
						*_msgPtrToAdd++ = buf[num+2];
						*_msgPtrToAdd++ = buf[num+3];
					}
					break;
				default:
					warning("addMessageToStack(): string escape sequence %d unknown", chr);
					break;
				}
				num += (_version == 8) ? 4 : 2;
			}
		} else {
			if (chr != '@') {
				*_msgPtrToAdd++ = chr;
			}
		}
	}
	*_msgPtrToAdd = 0;

	return msg;
}

void ScummEngine::addIntToStack(int var) {
	int num;

	num = readVar(var);
	_msgPtrToAdd += sprintf((char *)_msgPtrToAdd, "%d", num);
}

void ScummEngine::addVerbToStack(int var) {
	int num, k;

	num = readVar(var);
	if (num) {
		for (k = 1; k < _numVerbs; k++) {
			if (num == _verbs[k].verbid && !_verbs[k].type && !_verbs[k].saveid) {
				const byte *ptr = getResourceAddress(rtVerb, k);
				ptr = translateTextAndPlaySpeech(ptr);
				addMessageToStack(ptr);
				break;
			}
		}
	}
}

void ScummEngine::addNameToStack(int var) {
	int num;
	const byte *ptr = 0;

	num = readVar(var);
	if (num)
		ptr = getObjOrActorName(num);
	if (ptr) {
		if ((_version == 8) && (ptr[0] == '/')) {
			translateText(ptr, _transText);
			addMessageToStack(_transText);
		} else {
			addMessageToStack(ptr);
		}
	}
}

void ScummEngine::addStringToStack(int var) {
	const byte *ptr;

	if (_version == 3 || _version >= 6)
		var = readVar(var);

	if (var) {
		ptr = getStringAddress(var);
		if (ptr) {
			if ((_version == 8) && (ptr[0] == '/')) {
				translateText(ptr, _transText);
				addMessageToStack(_transText);
			} else {
				addMessageToStack(ptr);
			}
		}
	}
}

void ScummEngine::initCharset(int charsetno) {
	int i;

	if (!getResourceAddress(rtCharset, charsetno))
		loadCharset(charsetno);

	_string[0].t_charset = charsetno;
	_string[1].t_charset = charsetno;

	for (i = 0; i < 16; i++)
		_charsetColorMap[i] = _charsetData[charsetno][i];
}

void ScummEngine::enqueueText(const byte *text, int x, int y, byte color, byte charset, bool center) {
	// The Dig will keep enqueueing texts long after they've scrolled off
	// the screen, eventually overflowing the blast text queue if left
	// unchecked.

	if (y < 0) {
		byte old_charset;
		int font_height;

		old_charset = _charset->getCurID();
		_charset->setCurID(charset);
		font_height = _charset->getFontHeight();
		_charset->setCurID(old_charset);

		if (y <= -font_height)
			return;
	}

	BlastText &bt = _blastTextQueue[_blastTextQueuePos++];
	assert(_blastTextQueuePos <= ARRAYSIZE(_blastTextQueue));
	
	strcpy((char *)bt.text, (const char *)text);
	bt.xpos = x;
	bt.ypos = y;
	bt.color = color;
	bt.charset = charset;
	bt.center = center;
}

void ScummEngine::drawBlastTexts() {
	byte *buf;
	int c;
	int i;

	_charset->_ignoreCharsetMask = true;
	for (i = 0; i < _blastTextQueuePos; i++) {

		buf = _blastTextQueue[i].text;

		_charset->_top = _blastTextQueue[i].ypos + _screenTop;
		_charset->_startLeft = _charset->_left = _blastTextQueue[i].xpos;
		_charset->_right = _screenWidth - 1;
		_charset->_center = _blastTextQueue[i].center;
		_charset->setColor(_blastTextQueue[i].color);
		_charset->_disableOffsX = _charset->_firstChar = true;
		_charset->setCurID(_blastTextQueue[i].charset);
		_charset->_nextLeft = _blastTextQueue[i].xpos;
		_charset->_nextTop = _charset->_top;

		// Center text if necessary
		if (_charset->_center) {
			_charset->_nextLeft -= _charset->getStringWidth(0, buf) / 2;
			if (_charset->_nextLeft < 0)
				_charset->_nextLeft = 0;
		}

		do {
			c = *buf++;
			if (c != 0 && c != 0xFF) {
				_charset->_left = _charset->_nextLeft;
				_charset->_top = _charset->_nextTop;
				if (c >= 0x80 && _CJKMode)
					c += *buf++ * 256;
				_charset->printChar(c);
				_charset->_nextLeft = _charset->_left;
				_charset->_nextTop = _charset->_top;
			}
		} while (c);

		_blastTextQueue[i].rect = _charset->_str;
	}
	_charset->_ignoreCharsetMask = false;
}

void ScummEngine::removeBlastTexts() {
	int i;

	for (i = 0; i < _blastTextQueuePos; i++) {
		restoreBG(_blastTextQueue[i].rect);
	}
	_blastTextQueuePos = 0;
}

int indexCompare(const void *p1, const void *p2) {
	const LangIndexNode *i1 = (const LangIndexNode *) p1;
	const LangIndexNode *i2 = (const LangIndexNode *) p2;

	return strcmp(i1->tag, i2->tag);
}

void ScummEngine::loadLanguageBundle() {
	File file;
	int32 size;

	if (_gameId == GID_DIG) {
		file.open("language.bnd", getGameDataPath());
	} else if (_gameId == GID_CMI) {
		file.open("language.tab", getGameDataPath());
	} else {
		return;
	}
	if (file.isOpen() == false) {
		_existLanguageFile = false;
		return;
	}
	
	_existLanguageFile = true;

	size = file.size();
	_languageBuffer = (char *)calloc(1, size+1);
	file.read(_languageBuffer, size);
	file.close();

	// Create an index of the language file.
	// FIXME: Extend this mechanism to also cover The Dig?

	int32 i;
	char *ptr = _languageBuffer;

	// Count the number of lines in the language file.

	_languageIndexSize = 0;

	for (;;) {
		ptr = strpbrk(ptr, "\n\r");
		if (ptr == NULL)
			break;
		while (*ptr == '\n' || *ptr == '\r')
			ptr++;
		_languageIndexSize++;
	}

	// Fill the language file index. This is just an array of
	// tags and offsets. I did consider using a balanced tree
	// instead, but the extra overhead in the node structure would
	// easily have doubled the memory consumption of the index.

	_languageIndex = (LangIndexNode *)calloc(_languageIndexSize, sizeof(LangIndexNode));

	ptr = _languageBuffer;

	if (_gameId == GID_DIG) {
		int lineCount = _languageIndexSize;
		const char *baseTag = "";
		byte enc = 0;	// Initially assume the language file is not encoded
	
		// We'll determine the real index size as we go.
		_languageIndexSize = 0;
		for (i = 0; i < lineCount; i++) {
			if (*ptr == '!') {
				// Don't know what a line with '!' means, just ignore it
			} else if (*ptr == 'e') {
				// File is encoded!
				enc = 0x13;
			} else if (*ptr == '@') {
				// A new 'base tag'
				baseTag = ptr + 1;
			} else if (*ptr == '#') {
				// Number of subtags following a given basetag. We don't need that
				// information so we just skip it
			} else if (isdigit(*ptr)) {
				int idx = 0;
				// A number (up to three digits)...
				while (isdigit(*ptr)) {
					idx = idx * 10 + (*ptr - '0');
					ptr++;
				}
	
				// ...followed by a slash...
				assert(*ptr == '/');
				ptr++;
	
				// ...and then the translated message, possibly encoded
				_languageIndex[_languageIndexSize].offset = ptr - _languageBuffer;
	
				// Decode string if necessary.
				if (enc) {
					while (*ptr != '\n' && *ptr != '\r')
						*ptr++ ^= enc;
				}
	
				// The tag is the basetag, followed by a dot and then the index
				sprintf(_languageIndex[_languageIndexSize].tag, "%s.%03d", baseTag, idx);
	
				// That was another index entry
				_languageIndexSize++;
			}

			// Skip over newlines (and turn them into null bytes)
			ptr = strpbrk(ptr, "\n\r");
			if (ptr == NULL)
				break;
			while (*ptr == '\n' || *ptr == '\r')
				*ptr++ = 0;
		}
	} else {
		for (i = 0; i < _languageIndexSize; i++) {
			// First 8 chars in the line give the string ID / 'tag'
			int j;
			for (j = 0; j < 8 && !isspace(*ptr); j++, ptr++)
				_languageIndex[i].tag[j] = toupper(*ptr);
			_languageIndex[i].tag[j] = 0;
	
			// After that follows a single space which we skip
			assert(isspace(*ptr));
			ptr++;
	
			// Then comes the translated string: we record an offset to that.
			_languageIndex[i].offset = ptr - _languageBuffer;
	
			// Skip over newlines (and turn them into null bytes)
			ptr = strpbrk(ptr, "\n\r");
			if (ptr == NULL)
				break;
			while (*ptr == '\n' || *ptr == '\r')
				*ptr++ = 0;
		}
	}

	// Sort the index nodes. We'll later use bsearch on it, which is just as efficient
	// as using a binary tree, speed wise.
	qsort(_languageIndex, _languageIndexSize, sizeof(LangIndexNode), indexCompare);
}

const byte *ScummEngine::translateTextAndPlaySpeech(const byte *ptr) {
	if ((_gameId == GID_DIG || _gameId == GID_CMI) && (ptr[0] == '/')) {
		char pointer[20];
		int i, j;

		translateText(ptr, _transText);
		for (i = 0, j = 0; (ptr[i] != '/' || j == 0) && j < 19; i++) {
			if (ptr[i] != '/')
				pointer[j++] = ptr[i];
		}
		pointer[j] = 0;

		// Play speech
		if (!(_features & GF_DEMO) && (_gameId == GID_CMI)) // CMI demo does not have .IMX for voice
			strcat(pointer, ".IMX");
		// FIXME: This is a hack to distinguish between 'real' actor speech and
		// some odd (?) other strings... there is probably a better way to do this.
		// I just don't know which (yet).
		if ((_gameId == GID_DIG) || (_gameId == GID_CMI && ptr[i+1] != 0 && ptr[i+1] != 255)) {
			_sound->stopTalkSound();
			_imuseDigital->startVoice(kTalkSoundID, pointer);
			_sound->talkSound(0, 0, 2, -1);
		}

		ptr = _transText;
	}
	return ptr;
}

void ScummEngine::translateText(const byte *text, byte *trans_buff) {
	int l;
	
	if ((text[0] == '/') && _existLanguageFile) {
		LangIndexNode target;
		LangIndexNode *found = NULL;

		// copy name from text /..../
		for (l = 0; (l < 12) && (text[l + 1] != '/'); l++)
			target.tag[l] = toupper(text[l + 1]);
		target.tag[l] = 0;

		// HACK: These are used for the object line when
		// using one object on another. I don't know if the
		// text in the language file is a placeholder or if
		// we're supposed to use it, but at least in the
		// English version things will work so much better if
		// we can't find translations for these.

		if (strcmp(target.tag, "PU_M001") != 0 && strcmp(target.tag, "PU_M002") != 0)
			found = (LangIndexNode *)bsearch(&target, _languageIndex, _languageIndexSize, sizeof(LangIndexNode), indexCompare);
		if (found != NULL) {
			strcpy((char *)trans_buff, _languageBuffer + found->offset);
			return;
		}
	}
	
	byte *pointer = (byte *)strchr((const char *)text + 1, '/');
	if (pointer != NULL) {
		pointer++;
		if (_gameId == GID_CMI) {
			memcpy(trans_buff, pointer, resStrLen(pointer) + 1);
		} else if (_gameId == GID_DIG) {
			l = 0;
			while (*pointer != '/' && *pointer != 0xff && *pointer != 0) {
				trans_buff[l++] = *pointer++;
			}
			trans_buff[l] = '\0';
		}
	} else {
		memcpy(trans_buff, text, resStrLen(text) + 1);
	}
}

} // End of namespace Scumm
