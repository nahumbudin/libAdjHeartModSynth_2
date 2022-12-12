/**
*	@file		dspReverbModel.h
*	@author		Nahum Budin
*	@date		25_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	30-Oct-2019   1.0
*
*	@brief		Common definitions
*				
*/

#ifndef _COMMON_DEFS
#define _COMMON_DEFS

#include <unistd.h>		// sleep

#include "libAdjHeartModSynth_2.h"

// Control sub samplig rate divider
#define _CONTROL_SUB_SAMPLING				16

#define _OSC_MAX_FREQUENCY					12543.8539514160f	// MIDI G11
#define _OSC_MIN_FREQUENCY					0.1f				// LFO 8.1757989156f		// MIDI C1

// Must be no more than 8
#define _MAX_STAGE_NUM 8
#if (_MAX_STAGE_NUM > 8)
	Error : Must be no more than 8
#endif

#define _AUDIO_STAGE_0						0 
#define _AUDIO_STAGE_1						1 
#define _AUDIO_STAGE_2						2 
#define _AUDIO_STAGE_3						3 
#define _AUDIO_STAGE_4						4 
#define _AUDIO_STAGE_5						5 
#define _AUDIO_STAGE_6						6 
#define _AUDIO_STAGE_7						7 

#define _MAX_NUM_OF_MIDI_CONNECTIONS		8
#if (_MAX_NUM_OF_MIDI_CONNECTIONS > _MAX_NUM_OF_MIDI_DEVICES)
	error max num of midi connections cannot be higher than max num off midi devices
#endif


typedef struct voice_resource
{
	voice_resource()
		: id(-1)
		, used(false)
		, pending(false)
		, note(-1)
		, timestamp(0) {}
	int id;
	bool used;
	bool pending;
	int note;
	uint64_t timestamp;
} voice_resource_t;
	
	


#endif
