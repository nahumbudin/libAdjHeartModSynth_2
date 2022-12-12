/**
*	@file		fluidSynthEventsHandling.cpp
*	@author		Nahum Budin
*	@date		29-May-2020
*	@version	1.0
*	
*	@brief		Used for executing fluid synthesizer commands and settings.

*
*	History:\n
*	
*	version 1.0		29-May-2020:		
*		First version
*		
*/

#include "fluidSynthInterface.h"

int FluidSynthInterface::fluid_synth_event(int settingId, int value)
{
	switch (settingId)
	{
		case _FLUID_SYNTH_VOLUME:
			mod_synth_set_fluid_synth_volume(value);
			break;
			
		case _FLUID_SYNTH_REVERB_ROOM_SIZE:
			mod_synth_set_fluid_synth_reverb_room_size(value);
			break;
			
		case _FLUID_SYNTH_REVERB_DAMP:
			mod_synth_set_fluid_synth_reverb_damp(value);
			break;
			
		case _FLUID_SYNTH_REVERB_WIDTH:
			mod_synth_set_fluid_synth_reverb_width(value);
			break;
			
		case _FLUID_SYNTH_REVERB_LEVEL:
			mod_synth_set_fluid_synth_reverb_level(value);
			break;
			
		case _FLUID_SYNTH_CHORUS_NUMBER:
			mod_synth_set_fluid_synth_chorus_number(value);
			break;
			
		case _FLUID_SYNTH_CHORUS_LEVEL:
			mod_synth_set_fluid_synth_chorus_level(value);
			break;
			
		case _FLUID_SYNTH_CHORUS_SPEED:
			mod_synth_set_fluid_synth_chorus_speed(value);
			break;
			
		case _FLUID_SYNTH_CHORUS_DEPTH:
			mod_synth_set_fluid_synth_chorus_depth(value);
			break;
			
		case _FLUID_SYNTH_CHORUS_WAVEFORM:
			mod_synth_set_fluid_synth_chorus_waveform(value);
			break;
			
		case _FLUID_SYNTH_REVERB_ENABLE:
			if (value > 0)
			{
				mod_synth_set_fluid_synth_enable_reverb();
			}
			else
			{
				mod_synth_set_fluid_synth_disable_reverb();
			}
			break;
			
		case _FLUID_SYNTH_CHORUS_ENABLE:
			if (value > 0)
			{
				mod_synth_set_fluid_synth_enable_chorus();
			}
			else
			{
				mod_synth_set_fluid_synth_disable_chorus();
			}
			break;
			
		default:
			return -1;
				
		}
	
	return 0;
}