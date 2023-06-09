/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001-2004 The ScummVM project
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
 * $Header: /cvsroot/scummvm/scummvm/sound/mpu401.cpp,v 1.25 2004/01/06 12:45:33 fingolfin Exp $
 */

#include "stdafx.h"
#include "mpu401.h"
#include "common/timer.h"
#include "common/util.h"	// for ARRAYSIZE

void MidiChannel_MPU401::init(MidiDriver_MPU401 *owner, byte channel) {
	_owner = owner;
	_channel = channel;
	_allocated = false;
}

MidiDriver *MidiChannel_MPU401::device() {
	return _owner;
}

void MidiChannel_MPU401::send (uint32 b) {
	_owner->send (b & 0xFFFFFFF0 | (_channel & 0xF));
}

void MidiChannel_MPU401::noteOff (byte note) {
	_owner->send(note << 8 | 0x80 | _channel);
}

void MidiChannel_MPU401::noteOn(byte note, byte velocity) {
	_owner->send(velocity << 16 | note << 8 | 0x90 | _channel);
}

void MidiChannel_MPU401::programChange(byte program) {
	_owner->send(program << 8 | 0xC0 | _channel);
}

void MidiChannel_MPU401::pitchBend(int16 bend) {
	_owner->send((((bend + 0x2000) >> 7) & 0x7F) << 16 | ((bend + 0x2000) & 0x7F) << 8 | 0xE0 | _channel);
}

void MidiChannel_MPU401::controlChange(byte control, byte value) {
	_owner->send(value << 16 | control << 8 | 0xB0 | _channel);
}

void MidiChannel_MPU401::pitchBendFactor(byte value) {
	_owner->setPitchBendRange(_channel, value);
}

void MidiChannel_MPU401::sysEx_customInstrument(uint32 type, byte *instr) {
	_owner->sysEx_customInstrument(_channel, type, instr);
}

const char *MidiDriver::getErrorName(int error_code) {
	static const char *const midi_errors[] = {
		"No error",
		"Cannot connect",
		"Streaming not available",
		"Device not available",
		"Driver already open"
	};

	if ((uint) error_code >= ARRAYSIZE(midi_errors))
		return "Unknown Error";
	return midi_errors[error_code];
}

MidiDriver_MPU401::MidiDriver_MPU401() :
	MidiDriver(),
	_timer_proc (0),
	_channel_mask (0xFFFF) // Permit all 16 channels by default
{
	
	uint i;
	for (i = 0; i < ARRAYSIZE(_midi_channels); ++i) {
		_midi_channels [i].init (this, i);
	}
}

void MidiDriver_MPU401::close() {
	if (_timer_proc)
		g_timer->removeTimerProc(_timer_proc);
	_timer_proc = 0;
	for (int i = 0; i < 16; ++i)
		send (0x7B << 8 | 0xB0 | i);
}

uint32 MidiDriver_MPU401::property (int prop, uint32 param) {
	switch (prop) {
		case PROP_CHANNEL_MASK:
			_channel_mask = param & 0xFFFF;
			return 1;
	}

	return 0;
}

MidiChannel *MidiDriver_MPU401::allocateChannel() {
	MidiChannel_MPU401 *chan;
	uint i;

	for (i = 0; i < ARRAYSIZE(_midi_channels); ++i) {
		if (i == 9 || !(_channel_mask & (1 << i)))
			continue;
		chan = &_midi_channels[i];
		if (!chan->_allocated) {
			chan->allocate();
			return chan;
		}
	}
	return NULL;
}

void MidiDriver_MPU401::setTimerCallback(void *timer_param, TimerProc timer_proc) {
	if (!_timer_proc || !timer_proc) {
		if (_timer_proc)
			g_timer->removeTimerProc(_timer_proc);
		_timer_proc = timer_proc;
		if (timer_proc)
			g_timer->installTimerProc(timer_proc, 10000, timer_param);
	}
}
