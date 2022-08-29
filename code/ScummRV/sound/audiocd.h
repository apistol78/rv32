/* ScummVM - Scumm Interpreter
 * Copyright (C) 2003-2004 The ScummVM project
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
 * $Header: /cvsroot/scummvm/scummvm/sound/audiocd.h,v 1.5 2004/01/06 12:45:33 fingolfin Exp $
 *
 */

#ifndef SOUND_AUDIOCD_H
#define SOUND_AUDIOCD_H

#include "stdafx.h"
#include "common/scummsys.h"
#include "common/singleton.h"
#include "sound/mixer.h"

class DigitalTrackInfo {
public:
	virtual bool error() = 0;
	virtual void play(SoundMixer *mixer, PlayingSoundHandle *handle, int startFrame, int duration) = 0;
	virtual ~DigitalTrackInfo() { }
};


class AudioCDManager : public Common::Singleton<AudioCDManager> {
public:
	struct Status {
		bool playing;
		int track;
		int start;
		int duration;
		int numLoops;
	};

	void play(int track, int numLoops, int startFrame, int duration);
	void stop();
	bool isPlaying() const;

	void updateCD();

	Status getStatus() const;

private:
	friend class Common::Singleton<AudioCDManager>;
	AudioCDManager();

	int getCachedTrack(int track);

private:
	/* used for emulated CD music */
	struct ExtStatus : Status {
		PlayingSoundHandle handle;
	};
	ExtStatus _cd;

	enum {
		CACHE_TRACKS = 10
	};
	int _cached_tracks[CACHE_TRACKS];
	DigitalTrackInfo *_track_info[CACHE_TRACKS];
	int _current_cache;

};

/** Shortcut for accessing the audio CD manager. */
#define AudioCD		AudioCDManager::instance()

#endif
