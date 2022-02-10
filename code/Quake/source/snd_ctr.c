/*
Copyright (C) Rinnegatamante <rinnegatamante@gmail.com>
Copyright (C) 2015 Felipe Izzo <masterfeizz@gmail.com>
Copyright (C) 1996-1997 Id Software, Inc.
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <stdio.h>
#include "quakedef.h"

#define TICKS_PER_SEC 268111856LL
#define QUAKE_SAMPLERATE 32000
#define CSND_SAMPLERATE 32730
#define CSND_BUFSIZE 16384
#define DSP_BUFSIZE 16384
qboolean isDSP = true;
uint32_t SAMPLERATE = QUAKE_SAMPLERATE<<1;
uint32_t *audiobuffer;
uint64_t initial_tick;
uint64_t csnd_pause_tick;//For fixing sound desync on resume
int snd_inited;

/* static aptHookCookie csndAptCookie; */

//Used to fix csnd problems when pressing the home button
/* static void csndAptHook(APT_HookType hook, void* param) */
/* { */
/* 	switch (hook) */
/* 	{ */
/* 		case APTHOOK_ONWAKEUP: */
/* 			break; */
/* 		case APTHOOK_ONRESTORE: */
/* 			initial_tick += (svcGetSystemTick() - csnd_pause_tick); */
/* 			CSND_SetPlayState(0x08, 1); */
/* 			CSND_UpdateInfo(0); */
/* 			break; */

/* 		case APTHOOK_ONSLEEP: */
/* 			break; */
/* 		case APTHOOK_ONSUSPEND: */
/* 			CSND_SetPlayState(0x08, 0); */
/* 			CSND_UpdateInfo(0); */
/* 			csnd_pause_tick = svcGetSystemTick(); */
/* 			break; */

/* 		default: */
/* 			break; */
/* 	} */
/* } */

// createDspBlock: Create a new block for DSP service
/* void createDspBlock(ndspWaveBuf* waveBuf, u16 bps, u32 size, u8 loop, u32* data){ */
	/* waveBuf->data_vaddr = (void*)data; */
	/* waveBuf->nsamples = size / bps; */
	/* waveBuf->looping = loop; */
	/* waveBuf->offset = 0;	 */
	/* DSP_FlushDataCache(data, size); */
/* } */

qboolean SNDDMA_Init(void)
{
  /* snd_initialized = 0; */

  return 0;  //mute


  /* Fill the audio DMA information block */
  shm = &sn;
  shm->splitbuffer = 0;
  shm->samplebits = 16;
  shm->speed = QUAKE_SAMPLERATE;
  SAMPLERATE = QUAKE_SAMPLERATE;
  shm->channels = 1;
  shm->samples = CSND_BUFSIZE;
  shm->samplepos = 0;
  shm->submission_chunk = 1;
  shm->buffer = audiobuffer;
	
  initial_tick = svcGetSystemTick();

  snd_initialized = 1;
  return 1;
}

int SNDDMA_GetDMAPos(void)
{
	/* if(!snd_initialized) */
    	/* return 0; */

	/* u64 delta = (svcGetSystemTick() - initial_tick); */
	/* u64 samplepos = delta * (SAMPLERATE) / TICKS_PER_SEC; */
	/* shm->samplepos = samplepos; */
	/* return samplepos; */
  return 0;
}

void SNDDMA_Shutdown(void)
{
  /* if(snd_initialized){ */
  /* 	if (isDSP){ */
  /* 		ndspChnWaveBufClear(0x08); */
  /* 		ndspExit(); */
  /* 		free(waveBuf); */
  /* 	}else{ */
  /* 		CSND_SetPlayState(0x08, 0); */
  /* 		CSND_UpdateInfo(0); */
  /* 		csndExit(); */
  /* 		#ifdef _3DS_CIA */
  /* 		aptUnhook(&csndAptCookie); */
  /* 		#endif */
  /* 	} */
  /* 	linearFree(audiobuffer); */
  /* } */
}

/*
==============
SNDDMA_Submit
Send sound to device if buffer isn't really the dma buffer
===============
*/
void SNDDMA_Submit(void)
{
	/* if (snd_initialized){ */
	/* 	if (isDSP) DSP_FlushDataCache(audiobuffer, DSP_BUFSIZE<<2); */
	/* 	else CSND_FlushDataCache(audiobuffer, CSND_BUFSIZE); */
	/* } */
}
