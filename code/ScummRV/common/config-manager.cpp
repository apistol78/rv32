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
 * $Header: /cvsroot/scummvm/scummvm/common/config-manager.cpp,v 1.17 2004/02/07 04:53:59 ender Exp $
 *
 */

#include "stdafx.h"

#include "common/config-manager.h"

#if defined(UNIX)
#include <sys/param.h>
#ifndef MAXPATHLEN
#define MAXPATHLEN 256
#endif
#ifdef MACOSX
#define DEFAULT_CONFIG_FILE "Library/Preferences/ScummVM Preferences"
#else
#define DEFAULT_CONFIG_FILE ".scummvmrc"
#endif
#else
#define DEFAULT_CONFIG_FILE "scummvm.ini"
#endif

#define MAXLINELEN 256

static char *ltrim(char *t) {
	while (*t == ' ')
		t++;
	return t;
}

static char *rtrim(char *t) {
	int l = strlen(t) - 1;
	while (l >= 0 && t[l] == ' ')
		t[l--] = 0;
	return t;
}

namespace Common {

const String ConfigManager::kApplicationDomain("scummvm");
const String ConfigManager::kTransientDomain("__TRANSIENT");

const String trueStr("true");
const String falseStr("false");


#pragma mark -


ConfigManager::ConfigManager() {

#if defined(UNIX)
	char configFile[MAXPATHLEN];
	if(getenv("HOME") != NULL)
		sprintf(configFile,"%s/%s", getenv("HOME"), DEFAULT_CONFIG_FILE);
	else strcpy(configFile,DEFAULT_CONFIG_FILE);
#else
	char configFile[256];
	#if defined (WIN32) && !defined(_WIN32_WCE)
		GetWindowsDirectory(configFile, 256);
		strcat(configFile, "\\");
		strcat(configFile, DEFAULT_CONFIG_FILE);
	#elif defined(__PALM_OS__)
		strcpy(configFile,"/PALM/Programs/ScummVM/");
		strcat(configFile, DEFAULT_CONFIG_FILE);
	#else
		strcpy(configFile, DEFAULT_CONFIG_FILE);
	#endif
#endif

	switchFile(configFile);
}

void ConfigManager::switchFile(const String &filename) {
	_globalDomains.clear();
	_gameDomains.clear();
	_transientDomain.clear();

	// Ensure the global domain(s) are setup.
	_globalDomains.addKey(kApplicationDomain);
#ifdef _WIN32_WCE
	// WinCE for some reasons uses additional global domains.
	_globalDomains.addKey("wince");
	_globalDomains.addKey("smartfon-keys");
#endif

	_filename = filename;
	loadFile(_filename);
	printf("Switched to configuration %s\n", _filename.c_str());
}

void ConfigManager::loadFile(const String &filename) {
	FILE *cfg_file;
	char buf[MAXLINELEN];
	char *t;
	String domain;

	if (!(cfg_file = fopen(filename.c_str(), "r"))) {
		debug(1, "Unable to open configuration file: %s.\n", filename.c_str());
	} else {
		while (!feof(cfg_file)) {
			t = buf;
			if (!fgets(t, MAXLINELEN, cfg_file))
				continue;
			if (t[0] && t[0] != '#') {
				if (t[0] == '[') {
					// It's a new domain which begins here.
					char *p = strchr(t, ']');
					if (!p) {
						error("Config file buggy: no ] at the end of the domain name.\n");
					} else {
						*p = 0;
						// TODO: Some kind of domain name verification might be nice.
						// E.g. restrict to only a-zA-Z0-9 and maybe -_  or so...
						domain = t + 1;
					}
				} else {
					// Skip leading whitespaces
					while (*t && isspace(*t)) {
						t++;
					}
					// Skip empty lines
					if (*t == 0)
						continue;

					// If no domain has been set, this config file is invalid!
					if (domain.isEmpty()) {
						error("Config file buggy: we have a key without a domain first.\n");
					}

					// It's a new key in the domain.
					char *p = strchr(t, '\n');
					if (p)
						*p = 0;
					p = strchr(t, '\r');
					if (p)
						*p = 0;

					if (!(p = strchr(t, '='))) {
						if (strlen(t))
							warning("Config file buggy: there is junk: %s\n", t);
					} else {
						*p = 0;
						String key = ltrim(rtrim(t));
						String value = ltrim(p + 1);
						set(key, value, domain);
					}
				}
			}
		}
		fclose(cfg_file);
	}
}

void ConfigManager::flushToDisk() {
	FILE *cfg_file;

// TODO
//	if (!willwrite)
//		return;

	if (!(cfg_file = fopen(_filename.c_str(), "w"))) {
		warning("Unable to write configuration file: %s.\n", _filename.c_str());
	} else {
		DomainMap::const_iterator d;

		// First write the global domains
		for (d = _globalDomains.begin(); d != _globalDomains.end(); ++d) {
			writeDomain(cfg_file, d->_key, d->_value);
		}
		
		// Second, write the game domains
		for (d = _gameDomains.begin(); d != _gameDomains.end(); ++d) {
			writeDomain(cfg_file, d->_key, d->_value);
		}

		fclose(cfg_file);
	}
}

void ConfigManager::writeDomain(FILE *file, const String &name, const Domain &domain) {
	if (domain.isEmpty())
		return;		// Don't bother writing empty domains.
	
	fprintf(file, "[%s]\n", name.c_str());

	Domain::const_iterator x;
	for (x = domain.begin(); x != domain.end(); ++x) {
		const String &value = x->_value;
		if (!value.isEmpty())
			fprintf(file, "%s=%s\n", x->_key.c_str(), value.c_str());
	}
	fprintf(file, "\n");
}

#pragma mark -


bool ConfigManager::hasKey(const String &key) const {
	// Search the domains in the following order:
	// 1) Transient domain
	// 2) Active game domain (if any)
	// 3) All global domains
	// The defaults domain is explicitly *not* checked.
	
	if (_transientDomain.contains(key))
		return true;

	if (!_activeDomain.isEmpty() && _gameDomains[_activeDomain].contains(key))
		return true;
	
	DomainMap::const_iterator iter;
	for (iter = _globalDomains.begin(); iter != _globalDomains.end(); ++iter) {
		if (iter->_value.contains(key))
			return true;
	}

	return false;
}

bool ConfigManager::hasKey(const String &key, const String &dom) const {
	assert(!dom.isEmpty());

	if (dom == kTransientDomain)
		return _transientDomain.contains(key);
	if (_gameDomains.contains(dom))
		return _gameDomains[dom].contains(key);
	if (_globalDomains.contains(dom))
		return _globalDomains[dom].contains(key);
	
	return false;
}

void ConfigManager::removeKey(const String &key, const String &dom) {
	assert(!dom.isEmpty());

	if (dom == kTransientDomain)
		_transientDomain.remove(key);
	else if (_gameDomains.contains(dom))
		_gameDomains[dom].remove(key);
	else if (_globalDomains.contains(dom))
		_globalDomains[dom].remove(key);
	else
		error("Removing key '%s' from non-existent domain '%s'", key.c_str(), dom.c_str());
}


#pragma mark -


const String & ConfigManager::get(const String &key, const String &domain) const {
	// Search the domains in the following order:
	// 1) Transient domain
	// 2) Active game domain (if any)
	// 3) All global domains
	// 4) The defaults 


	if ((domain.isEmpty() || domain == kTransientDomain) && _transientDomain.contains(key))
		return _transientDomain[key];

	const String &dom = domain.isEmpty() ? _activeDomain : domain;

	if (!dom.isEmpty() && _gameDomains.contains(dom) && _gameDomains[dom].contains(key))
		return _gameDomains[dom][key];

	DomainMap::const_iterator iter;
	for (iter = _globalDomains.begin(); iter != _globalDomains.end(); ++iter) {
		if (iter->_value.contains(key))
			return iter->_value[key];
	}

	return _defaultsDomain.get(key);
}

int ConfigManager::getInt(const String &key, const String &dom) const {
	String value(get(key, dom));
	// Convert the string to an integer.
	// TODO: We should perform some error checking.
	return (int)strtol(value.c_str(), 0, 10);
}

bool ConfigManager::getBool(const String &key, const String &dom) const {
	String value(get(key, dom));
	// '1', 'true' and 'yes' are accepted as true values; everything else
	// maps to value 'false'.
	return (value == trueStr) || (value == "yes") || (value == "1");
}


#pragma mark -


void ConfigManager::set(const String &key, const String &value, const String &dom) {
	if (dom.isEmpty()) {
		// Remove the transient domain value
		_transientDomain.remove(key);
	
		if (_activeDomain.isEmpty())
			_globalDomains[kApplicationDomain][key] = value;
		else
			_gameDomains[_activeDomain][key] = value;

	} else {

		if (dom == kTransientDomain)
			_transientDomain[key] = value;
		else {
			if (_globalDomains.contains(dom)) {
				_globalDomains[dom][key] = value;
				if (_activeDomain.isEmpty() || !_gameDomains[_activeDomain].contains(key))
					_transientDomain.remove(key);
			} else {
				_gameDomains[dom][key] = value;
				if (dom == _activeDomain)
					_transientDomain.remove(key);
			}
		}
	}
}

void ConfigManager::set(const String &key, const char *value, const String &dom) {
	set(key, String(value), dom);
}

void ConfigManager::set(const String &key, int value, const String &dom) {
	char tmp[128];
	snprintf(tmp, sizeof(tmp), "%i", value);
	set(key, String(tmp), dom);
}

void ConfigManager::set(const String &key, bool value, const String &dom) {
	set(key, value ? trueStr : falseStr, dom);
}


#pragma mark -


void ConfigManager::registerDefault(const String &key, const String &value) {
	_defaultsDomain[key] = value;
}

void ConfigManager::registerDefault(const String &key, const char *value) {
	registerDefault(key, String(value));
}

void ConfigManager::registerDefault(const String &key, int value) {
	char tmp[128];
	snprintf(tmp, sizeof(tmp), "%i", value);
	registerDefault(key, tmp);
}

void ConfigManager::registerDefault(const String &key, bool value) {
	registerDefault(key, value ? trueStr : falseStr);
}


#pragma mark -


void ConfigManager::setActiveDomain(const String &domain) {
	assert(!domain.isEmpty());
	_activeDomain = domain;
	_gameDomains.addKey(domain);
}

void ConfigManager::removeGameDomain(const String &domain) {
	assert(!domain.isEmpty());
	_gameDomains.remove(domain);
}

void ConfigManager::renameGameDomain(const String &oldName, const String &newName) {
	if (oldName == newName)
		return;

	assert(!oldName.isEmpty());
	assert(!newName.isEmpty());

	_gameDomains[newName].merge(_gameDomains[oldName]);
	
	_gameDomains.remove(oldName);
}

bool ConfigManager::hasGameDomain(const String &domain) const {
	assert(!domain.isEmpty());
	return _gameDomains.contains(domain);
}

}	// End of namespace Common
