/**
*	@file		adjSynthSettingsCallbacksAudio.cpp
*	@author		Nahum Budin
*	@date		5-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*					2. Adding support in sample-rate and audio block-size settings of all modules
*	
*	@brief		Callback to handle audio settings
*	settings
*
*	History:\n
*	
*	version 1.0		18-Jan-2021:		
*		First version
*		
*/

#include "adjSynth.h"
#include "../jack/jackAudioClients.h"

int set_audio_jack_mode_cb(int mode, int prog)
{
	set_jack_mode(mode);
	return 0;
}

int set_audio_jack_auto_start_state_cb(bool state, int prog)
{
	if (state)
	{
		enable_jack_auto_start();
	}
	else
	{
		disable_jack_auto_start();
	}

	return 0;
}

int set_audio_jack_auto_connect_state_cb(bool state, int prog)
{
	if (state)
	{
		enable_jack_auto_connect_audio();
	}
	else
	{
		disable_jack_auto_connect_audio();
	}

	return 0;
}
