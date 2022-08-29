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
 * $Header: /cvsroot/scummvm/scummvm/scumm/dialogs.cpp,v 1.103 2004/01/26 07:40:14 arisme Exp $
 */

#include "stdafx.h"

#include "common/config-manager.h"

#include "gui/chooser.h"
#include "gui/newgui.h"
#include "gui/ListWidget.h"

#include "scumm/dialogs.h"
#include "scumm/sound.h"
#include "scumm/scumm.h"
#include "scumm/imuse.h"
#include "scumm/player_v2.h"
#include "scumm/verbs.h"
#include "sound/mididrv.h"
#include "sound/mixer.h"

#ifndef DISABLE_HELP
#include "scumm/help.h"
#endif

#ifdef _WIN32_WCE
#include "backends/wince/CEKeysDialog.h"
#endif

using GUI::CommandSender;
using GUI::StaticTextWidget;
using GUI::kButtonWidth;
using GUI::kCloseCmd;
using GUI::kTextAlignCenter;
using GUI::kTextAlignLeft;
using GUI::WIDGET_ENABLED;

typedef GUI::OptionsDialog GUI_OptionsDialog;

namespace Scumm {

struct ResString {
	int num;
	char string[80];
};

#ifdef __PALM_OS__
static ResString *string_map_table_v7;
static ResString *string_map_table_v6;
static ResString *string_map_table_v5;
#else
static ResString string_map_table_v7[] = {
	{96, "game name and version"}, //that's how it's supposed to be
	{77, "Select a game to LOAD"},
	{76, "Name your SAVE game"},
	{70, "save"}, //boot8
	{71, "load"}, //boot9
	{72, "play"}, //boot10
	{73, "cancel"}, //boot11
	{74, "quit"}, //boot12
	{75, "ok"}, //boot13
	{85, "game paused"}, // boot3					

	/* this is the almost complete string map for v7
	{63, "how may I serve you?"},
	{64, "the dig v1.0"}, //(game name/version)
	{67, "text display only"},
	{68, "c:\\dig"}, //boot007 (save path ?)
	{69, "the dig"}, //boot21 (game name)
	{70, "save"}, //boot8
	{71, "load"}, //boot9
	{72, "play"}, //boot10
	{73, "cancel"}, //boot11
	{74, "quit"}, //boot12
	{75, "ok"}, //boot13
	{76, "name your save game"}, //boot19
	{77, "select a game to load"}, //boot20
	{78, "you must enter a name"},//boot14
	{79, "saving '%s'"}, //boot17
	{80, "loading '%s'"}, //boot18
	{81, "the game was NOT saved"}, //boot15
	{82, "the game was NOT loaded"}, //boot16
	{83, "how may I serve you?"},
	{84, "how may I serve you?"},
	{85, "game paused"}, // boot3
	{86, "Are you sure you want to restart"},
	{87, "Are you sure you want to quit?"}, //boot05
	{89, "how may I serve you?"},
	{90, "music"}, //boot22
	{91, "voice"}, //boot23
	{92, "sfx"}, //boot24
	{93, "disabled"}, //boot25
	{94, "text speed"}, //boot26
	{95, "text display"}, //boot27
	{96, "the dig v1.0"},*/
	
};

static ResString string_map_table_v6[] = {
	{117, "How may I serve you?"}, 
	{109, "Select a game to LOAD"}, 
	{108, "Name your SAVE game"}, 
	{96, "Save"}, 
	{97, "Load"}, 
	{98, "Play"}, 
	{99, "Cancel"}, 
	{100, "Quit"}, 
	{101, "OK"}, 
	{93, "Game paused"}, 
};

static ResString string_map_table_v5[] = {
	{28, "How may I serve you?"}, 
	{20, "Select a game to LOAD"},
	{19, "Name your SAVE game"},
	{7, "Save"},
	{8, "Load"},
	{9, "Play"},
	{10, "Cancel"},
	{11, "Quit"},
	{12, "OK"},
	{4, "Game paused"}
};
#endif

#pragma mark -


const Common::String ScummDialog::queryResString(int stringno) {
	byte *result;

	if (stringno == 0)
		return String();

	if (_vm->_version >= 7)
		result = _vm->getStringAddressVar(string_map_table_v7[stringno - 1].num);
	else if (_vm->_version == 6)
		result = _vm->getStringAddressVar(string_map_table_v6[stringno - 1].num);
	else if (_vm->_version == 5)
		result = _vm->getStringAddress(string_map_table_v5[stringno - 1].num);
	else
		// TODO: For V8 games, maybe grab the strings from the language file?
		return string_map_table_v5[stringno - 1].string;

	if (result && *result == '/') {
		byte tmp[256];
		_vm->translateText(result, tmp);
		
		// FIXME: AARGH! We shouldn't just strcpy into the data we got from getStringAddress
		strcpy((char *)result, (char *)tmp);
	}

	if (!result || *result == '\0') {	// Gracelessly degrade to english :)
		return string_map_table_v5[stringno - 1].string;
	}

	// Convert to a proper string (take care of FF codes)
	byte chr;
	String tmp;
	while ((chr = *result++)) {		
		if (chr == 0xFF) {
			result += 3;
		} else if (chr != '@') {
			tmp += chr;
		}
	}
	return tmp;
}

#pragma mark -

enum {
	kSaveCmd = 'SAVE',
	kLoadCmd = 'LOAD',
	kPlayCmd = 'PLAY',
	kOptionsCmd = 'OPTN',
	kHelpCmd = 'HELP',
	kAboutCmd = 'ABOU',
	kQuitCmd = 'QUIT'
};

class SaveLoadChooser : public GUI::ChooserDialog {
	typedef Common::String String;
	typedef Common::StringList StringList;
protected:
	bool _saveMode;

public:
	SaveLoadChooser(const String &title, const String &buttonLabel, bool saveMode);
	
	virtual void handleCommand(CommandSender *sender, uint32 cmd, uint32 data);
	const String &getResultString() const;
};

SaveLoadChooser::SaveLoadChooser(const String &title, const String &buttonLabel, bool saveMode)
	: ChooserDialog(title, buttonLabel, 182), _saveMode(saveMode) {

	_list->setEditable(saveMode);
	_list->setNumberingMode(saveMode ? GUI::kListNumberingOne : GUI::kListNumberingZero);
}

const Common::String &SaveLoadChooser::getResultString() const {
	return _list->getSelectedString();
}

void SaveLoadChooser::handleCommand(CommandSender *sender, uint32 cmd, uint32 data) {
	int selItem = _list->getSelected();
	switch (cmd) {
	case GUI::kListItemActivatedCmd:
	case GUI::kListItemDoubleClickedCmd:
		if (selItem >= 0) {
			if (_saveMode || !getResultString().isEmpty()) {
				setResult(selItem);
				close();
			}
		}
		break;
	case GUI::kListSelectionChangedCmd:
		if (_saveMode) {
			_list->startEditMode();
		}
		// Disable button if nothing is selected, or (in load mode) if an empty
		// list item is selected. We allow choosing an empty item in save mode
		// because we then just assign a default name.
		_chooseButton->setEnabled(selItem >= 0 && (_saveMode || !getResultString().isEmpty()));
		_chooseButton->draw();
		break;
	default:
		ChooserDialog::handleCommand(sender, cmd, data);
	}
}

Common::StringList generateSavegameList(ScummEngine *scumm, bool saveMode) {
	// Get savegame names
	Common::StringList l;
	char name[32];
	uint i = saveMode ? 1 : 0;
	bool avail_saves[81];

	SaveFileManager *mgr = OSystem::instance()->get_savefile_manager();

	scumm->listSavegames(avail_saves, ARRAYSIZE(avail_saves), mgr);
	for (; i < ARRAYSIZE(avail_saves); i++) {
		if (avail_saves[i])
			scumm->getSavegameName(i, name, mgr);
		else
			name[0] = 0;
		l.push_back(name);
	}

	delete mgr;

	return l;
}

enum {
	kRowHeight = 18,
	kBigButtonWidth = 90,
	kMainMenuWidth 	= (kBigButtonWidth + 2 * 8),
	kMainMenuHeight 	= 7 * kRowHeight + 3 * 5 + 7 + 5
};

#define addBigButton(label, cmd, hotkey) \
	new GUI::ButtonWidget(this, x, y, kBigButtonWidth, 16, label, cmd, hotkey); y += kRowHeight

MainMenuDialog::MainMenuDialog(ScummEngine *scumm)
	: ScummDialog(scumm, (320 - kMainMenuWidth) / 2, (200 - kMainMenuHeight)/2, kMainMenuWidth, kMainMenuHeight) {
	int y = 7;

	const int x = (_w - kBigButtonWidth) / 2;
	addBigButton("Resume", kPlayCmd, 'P');
	y += 5;

	addBigButton("Load", kLoadCmd, 'L');
	addBigButton("Save", kSaveCmd, 'S');
	y += 5;

	addBigButton("Options", kOptionsCmd, 'O');
#ifndef DISABLE_HELP
	addBigButton("Help", kHelpCmd, 'H');
#endif
	addBigButton("About", kAboutCmd, 'A');
	y += 5;

	addBigButton("Quit", kQuitCmd, 'Q');

	//
	// Create the sub dialog(s)
	//
	_aboutDialog = new GUI::AboutDialog();
#ifndef DISABLE_HELP
	_helpDialog = new HelpDialog(scumm);
#endif
	_saveDialog = new SaveLoadChooser("Save game:", "Save", true);
	_loadDialog = new SaveLoadChooser("Load game:", "Load", false);
}

MainMenuDialog::~MainMenuDialog() {
	delete _aboutDialog;
#ifndef DISABLE_HELP
	delete _helpDialog;
#endif
	delete _saveDialog;
	delete _loadDialog;
}

void MainMenuDialog::open() {
	OSystem::Property prop;
	prop.show_keyboard = true;
	g_system->property(OSystem::PROP_TOGGLE_VIRTUAL_KEYBOARD, &prop);

	ScummDialog::open();
}

void MainMenuDialog::handleCommand(CommandSender *sender, uint32 cmd, uint32 data) {
	switch (cmd) {
	case kSaveCmd:
		save();
		break;
	case kLoadCmd:
		load();
		break;
	case kPlayCmd:
		close();
		break;
	case kOptionsCmd:
		_vm->optionsDialog();
		break;
	case kAboutCmd:
		_aboutDialog->runModal();
		break;
#ifndef DISABLE_HELP
	case kHelpCmd:
		_helpDialog->runModal();
		break;
#endif
	case kQuitCmd:
		_vm->_quit = true;
		close();
		break;
	default:
		ScummDialog::handleCommand(sender, cmd, data);
	}
}

void MainMenuDialog::close() {
	OSystem::Property prop;

	ScummDialog::close();

	prop.show_keyboard = false;
	g_system->property(OSystem::PROP_TOGGLE_VIRTUAL_KEYBOARD, &prop);
}

void MainMenuDialog::save() {
	int idx;
	_saveDialog->setList(generateSavegameList(_vm, true));
	idx = _saveDialog->runModal();
	if (idx >= 0) {
		const String &result = _saveDialog->getResultString();
		char buffer[20];
		const char *str;
		if (result.isEmpty()) {
			// If the user was lazy and entered no save name, come up with a default name.
			sprintf(buffer, "Save %d", idx + 1);
			str = buffer;
		} else
			str = result.c_str();
		_vm->requestSave(idx + 1, str);
		close();
	}
}

void MainMenuDialog::load() {
	int idx;
	_loadDialog->setList(generateSavegameList(_vm, false));
	idx = _loadDialog->runModal();
	if (idx >= 0) {
		_vm->requestLoad(idx);
		close();
	}
}

#pragma mark -

enum {
	kOKCmd					= 'ok  '
};

enum {
	kKeysCmd = 'KEYS'
};

#ifndef _WIN32_WCE
ConfigDialog::ConfigDialog(ScummEngine *scumm)
	: GUI::OptionsDialog("", 40, 30, 240, 124), _vm(scumm) {
#else
ConfigDialog::ConfigDialog(ScummEngine *scumm)
	: GUI::OptionsDialog("", 40, 30, 240, 124 + 4), _vm(scumm) {
#endif
	//
	// Add the buttons
	//
#ifdef _WIN32_WCE
	addButton(_w - kButtonWidth - 8, _h - 24 - 4, "OK", GUI::OptionsDialog::kOKCmd, 'O');
	addButton(_w - 2 * kButtonWidth - 12, _h - 24 - 4, "Cancel", kCloseCmd, 'C');
	addButton(_w - 3 * kButtonWidth - 16, _h - 24 - 4, "Keys", kKeysCmd, 'K');
#else
	addButton(_w - kButtonWidth-8, _h - 24, "OK", GUI::OptionsDialog::kOKCmd, 'O');
	addButton(_w - 2 * kButtonWidth-12, _h - 24, "Cancel", kCloseCmd, 'C');
#endif

	//
	// Sound controllers
	//
	int yoffset = 8;
	yoffset = addVolumeControls(this, yoffset);

	//
	// Some misc options
	//
	subtitlesCheckbox = new GUI::CheckboxWidget(this, 15, 62, 200, 16, "Show subtitles", 0, 'S');

	//
	// Create the sub dialog(s)
	//
#ifdef _WIN32_WCE
	_keysDialog = new CEKeysDialog();
#endif
}

ConfigDialog::~ConfigDialog() {
#ifdef _WIN32_WCE
	delete _keysDialog;
#endif
}

void ConfigDialog::open() {
	GUI_OptionsDialog::open();

	// update checkboxes, too
	subtitlesCheckbox->setState(ConfMan.getBool("subtitles"));
}

void ConfigDialog::close() {
	
	if (getResult()) {
		// Subtitles
		ConfMan.set("subtitles", subtitlesCheckbox->getState(), _domain);
	}

	GUI_OptionsDialog::close();

	// Sync the engine with the config manager
	int soundVolumeMaster = ConfMan.getInt("master_volume");
	int soundVolumeMusic = ConfMan.getInt("music_volume");
	int soundVolumeSfx = ConfMan.getInt("sfx_volume");

	if (_vm->_imuse) {
		_vm->_imuse->set_music_volume(soundVolumeMusic);
	}
	if (_vm->_musicEngine) {
		_vm->_musicEngine->setMasterVolume(soundVolumeMaster);
	}

	_vm->_mixer->setVolume(soundVolumeSfx * soundVolumeMaster / 255);
	_vm->_mixer->setMusicVolume(soundVolumeMusic);
}


void ConfigDialog::handleCommand(CommandSender *sender, uint32 cmd, uint32 data) {
	switch (cmd) {
	case kKeysCmd:
#ifdef _WIN32_WCE
		_keysDialog->runModal();
#endif
		break;
	default:
		GUI_OptionsDialog::handleCommand (sender, cmd, data);
	}
}

#ifndef DISABLE_HELP

#pragma mark -

enum {
	kNextCmd = 'NEXT',
	kPrevCmd = 'PREV'
};

HelpDialog::HelpDialog(ScummEngine *scumm)
	: ScummDialog(scumm, 5, 5, 310, 190) {

	_page = 1;
	_numPages = ScummHelp::numPages(scumm->_gameId);

	_prevButton = addButton(10, 170, "Previous", kPrevCmd, 'P');
	_nextButton = addButton(90, 170, "Next", kNextCmd, 'N');
	addButton(210, 170, "Close", kCloseCmd, 'C');
	_prevButton->clearFlags(WIDGET_ENABLED);

	_title = new StaticTextWidget(this, 10, 5, 290, 16, "", kTextAlignCenter);
	for (int i = 0; i < HELP_NUM_LINES; i++) {
		_key[i] = new StaticTextWidget(this, 10, 18 + (10 * i), 80, 16, "", kTextAlignLeft);
		_dsc[i] = new StaticTextWidget(this, 90, 18 + (10 * i), 210, 16, "", kTextAlignLeft);
	}

	displayKeyBindings();
}

void HelpDialog::displayKeyBindings() {

	String titleStr, *keyStr, *dscStr;

	ScummHelp::updateStrings(_vm->_gameId, _vm->_version, _page, titleStr, keyStr, dscStr);

	_title->setLabel(titleStr);
	for (int i = 0; i < HELP_NUM_LINES; i++) {
		_key[i]->setLabel(keyStr[i]);
		_dsc[i]->setLabel(dscStr[i]);
	}

	delete [] keyStr;
	delete [] dscStr;
}

void HelpDialog::handleCommand(CommandSender *sender, uint32 cmd, uint32 data) {

	switch (cmd) {
	case kNextCmd:
		_page++;
		if (_page >= _numPages) {
			_nextButton->clearFlags(WIDGET_ENABLED);
		}
		if (_page >= 2) {
			_prevButton->setFlags(WIDGET_ENABLED);
		}
		displayKeyBindings();
		draw();
		break;
	case kPrevCmd:
		_page--;
		if (_page <= _numPages) {
			_nextButton->setFlags(WIDGET_ENABLED);
		}
		if (_page <= 1) {
			_prevButton->clearFlags(WIDGET_ENABLED);
		}
		displayKeyBindings();
		draw();
		break;
	default:
		ScummDialog::handleCommand(sender, cmd, data);
	}
}

#endif

#pragma mark -

InfoDialog::InfoDialog(ScummEngine *scumm, int res)
: ScummDialog(scumm, 0, 80, 0, 16) { // dummy x and w
	setInfoText(queryResString (res));
}

InfoDialog::InfoDialog(ScummEngine *scumm, const String& message)
: ScummDialog(scumm, 0, 80, 0, 16) { // dummy x and w
	setInfoText(message);
}

void InfoDialog::setInfoText(const String& message) {
	int width = g_gui.getStringWidth(message) + 16;

	_x = (_vm->_screenWidth - width) >> 1;
	_w = width;

	new StaticTextWidget(this, 4, 4, _w - 8, _h, message, kTextAlignCenter);
}

#pragma mark -

PauseDialog::PauseDialog(ScummEngine *scumm)
	: InfoDialog(scumm, 10) {
}

void PauseDialog::handleKeyDown(uint16 ascii, int keycode, int modifiers) {
	if (ascii == ' ')  // Close pause dialog if space key is pressed
		close();
	else
		ScummDialog::handleKeyDown(ascii, keycode, modifiers);
}

ConfirmDialog::ConfirmDialog(ScummEngine *scumm, const String& message)
	: InfoDialog(scumm, message) {
}

void ConfirmDialog::handleKeyDown(uint16 ascii, int keycode, int modifiers) {
	if (tolower(ascii) == 'n') {
		setResult(0);
		close();
	} else if (tolower(ascii) == 'y') {
		setResult(1);
		close();
	} else
		ScummDialog::handleKeyDown(ascii, keycode, modifiers);
}

} // End of namespace Scumm

#ifdef __PALM_OS__
#include "scumm_globals.h"

_GINIT(Dialogs)
_GSETPTR(Scumm::string_map_table_v7, GBVARS_STRINGMAPTABLEV7_INDEX, Scumm::ResString, GBVARS_SCUMM)
_GSETPTR(Scumm::string_map_table_v6, GBVARS_STRINGMAPTABLEV6_INDEX, Scumm::ResString, GBVARS_SCUMM)
_GSETPTR(Scumm::string_map_table_v5, GBVARS_STRINGMAPTABLEV5_INDEX, Scumm::ResString, GBVARS_SCUMM)
_GEND

_GRELEASE(Dialogs)
_GRELEASEPTR(GBVARS_STRINGMAPTABLEV7_INDEX, GBVARS_SCUMM)
_GRELEASEPTR(GBVARS_STRINGMAPTABLEV6_INDEX, GBVARS_SCUMM)
_GRELEASEPTR(GBVARS_STRINGMAPTABLEV5_INDEX, GBVARS_SCUMM)
_GEND

#endif
