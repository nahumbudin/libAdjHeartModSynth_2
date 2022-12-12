/**
*	@file		LibAPI_getNoiseParams.cpp
*	@author		Nahum Budin
*	@date		9-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*
*	@brief		Get active Noise Generator settings parameters
*
*	History: 
*				version 1.0		5-Oct-2019	1st version
*
*/


#include "../libAdjHeartModSynth_2.h"
#include "LibAPI_settingsManager.h"

#include "../synthesizer/modSynth.h"

int res_noise;
mod_synth_settings_int_param_t int_param_noise;
mod_synth_settings_bool_param_t bool_param_noise;

bool mod_synth_get_active_noise_enable_state()
{
	res_noise = settings_manager->get_bool_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.noise.enabled", &bool_param_noise);
	if (res_noise == _SETTINGS_KEY_FOUND)
	{
		return bool_param_noise.value;
	}
	else
	{
		return false;
	}
}

int mod_synth_get_active_noise_color()
{
	res_noise = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.noise.color", &int_param_noise);
	if (res_noise == _SETTINGS_KEY_FOUND)
	{
		return int_param_noise.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_noise_send_filter_1()
{
	res_noise = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.noise.send_filter_1", &int_param_noise);
	if (res_noise == _SETTINGS_KEY_FOUND)
	{
		return int_param_noise.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_noise_send_filter_2()
{
	res_noise = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.noise.send_filter_2", &int_param_noise);
	if (res_noise == _SETTINGS_KEY_FOUND)
	{
		return int_param_noise.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_noise_amp_mod_lfo()
{
	res_noise = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.noise.amp_modulation_lfo_num", &int_param_noise);
	if (res_noise == _SETTINGS_KEY_FOUND)
	{
		return int_param_noise.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_noise_amp_mod_lfo_level()
{
	res_noise = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.noise.amp_modulation_lfo_level", &int_param_noise);
	if (res_noise == _SETTINGS_KEY_FOUND)
	{
		return int_param_noise.value;
	}
	else
	{
		return 0;
	}
}
int mod_synth_get_active_noise_amp_mod_env()
{
	res_noise = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.noise.amp_modulation_env_num", &int_param_noise);
	if (res_noise == _SETTINGS_KEY_FOUND)
	{
		return int_param_noise.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_noise_amp_mod_env_level()
{
	res_noise = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.noise.amp_modulation_env_level", &int_param_noise);
	if (res_noise == _SETTINGS_KEY_FOUND)
	{
		return int_param_noise.value;
	}
	else
	{
		return 0;
	}
}
