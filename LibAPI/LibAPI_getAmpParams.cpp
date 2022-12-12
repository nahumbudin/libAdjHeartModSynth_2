/**
*	@file		LibAPI_getAmpParams.cpp
*	@author		Nahum Budin
*	@date		9-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*					
*
*	@brief		Get active Amp settings parameters
*
*	History: 
*				version 1.0		5-Oct-2019	1st version
*
*/


#include "../libAdjHeartModSynth_2.h"
#include "LibAPI_settingsManager.h"

#include "../synthesizer/modSynth.h"

int res_amp;
mod_synth_settings_int_param_t int_param_amp;

int mod_synth_get_active_ch_1_level() 
{
	res_amp = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.amp_ch1.level", &int_param_amp);
	if (res_amp == _SETTINGS_KEY_FOUND)
	{
		return int_param_amp.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_ch_1_pan() 
{
	res_amp = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.amp_ch1.pan", &int_param_amp);
	if (res_amp == _SETTINGS_KEY_FOUND)
	{
		return int_param_amp.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_ch_1_pan_mod_lfo() 
{
	res_amp = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.amp_ch1.pan_modulation_lfo_num", &int_param_amp);
	if (res_amp == _SETTINGS_KEY_FOUND)
	{
		return int_param_amp.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_ch_1_pan_mod_lfo_level() 
{
	res_amp = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.amp_ch1.pan_modulation_lfo_level", &int_param_amp);
	if (res_amp == _SETTINGS_KEY_FOUND)
	{
		return int_param_amp.value;
	}
	else
	{
		return 0;
	}
}
int mod_synth_get_active_ch_1_send() {
	res_amp = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.amp_ch1.send", &int_param_amp);
	if (res_amp == _SETTINGS_KEY_FOUND)
	{
		return int_param_amp.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_ch_2_level()
{
	res_amp = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.amp_ch2.level", &int_param_amp);
	if (res_amp == _SETTINGS_KEY_FOUND)
	{
		return int_param_amp.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_ch_2_pan()
{
	res_amp = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.amp_ch2.pan", &int_param_amp);
	if (res_amp == _SETTINGS_KEY_FOUND)
	{
		return int_param_amp.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_ch_2_pan_mod_lfo()
{
	res_amp = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.amp_ch2.pan_modulation_lfo_num", &int_param_amp);
	if (res_amp == _SETTINGS_KEY_FOUND)
	{
		return int_param_amp.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_ch_2_pan_mod_lfo_level()
{
	res_amp = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.amp_ch2.pan_modulation_lfo_level", &int_param_amp);
	if (res_amp == _SETTINGS_KEY_FOUND)
	{
		return int_param_amp.value;
	}
	else
	{
		return 0;
	}
}
int mod_synth_get_active_ch_2_send() {
	res_amp = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.amp_ch2.send", &int_param_amp);
	if (res_amp == _SETTINGS_KEY_FOUND)
	{
		return int_param_amp.value;
	}
	else
	{
		return 0;
	}
}