/**
*	@file		adjSynthDefaultSettingsMixer.cpp
*	@author		Nahum Budin
*	@date		6-Dec-2019
*	@version	1.0
*	
*	@brief		Set default settings Mixer parameters
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

/**
*   @brief  Set the settings Mixer parameters to their default values
*   @param	params	a _setting_params_t parameters struct
*   @return 0 if done
*/
int AdjSynth::set_default_settings_parameters_mixer(_setting_params_t *params)
{
	int res = 0, channel; 
	char keyString[128];
	
	return_val_if_true(params == NULL, _SETTINGS_BAD_PARAMETERS);
	
	
	for (channel = 0; channel < mod_synth_get_synthesizer_num_of_programs(); channel++)
	{
		sprintf(keyString, "adjsynth.mixer_channel_%i.level", channel + 1);
		res |= adj_synth_settings_manager->set_int_param
			(params,
			string(keyString),
			80,
			100,
			0,
			_PARAM_TYPE_ADJ_SYNTH_SETUP,
			set_mixer_channel_level_cb,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
			channel);

		sprintf(keyString, "adjsynth.mixer_channel_%i.pan", channel + 1);
		res |= adj_synth_settings_manager->set_int_param
		(params,
			string(keyString),
			50,
			100,
			0,
			_PARAM_TYPE_ADJ_SYNTH_SETUP,
			set_mixer_channel_pan_cb,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
			channel);

		sprintf(keyString, "adjsynth.mixer_channel_%i.send", channel + 1);
		res |= adj_synth_settings_manager->set_int_param
			(params,
			string(keyString),
			0,
			100,
			0,
			_PARAM_TYPE_ADJ_SYNTH_SETUP,
			set_mixer_channel_send_cb,
			0,
			0,
			NULL,
			_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
			_SET_TYPE | _SET_CALLBACK,
			channel);
	}
	
	return res;
}
	
	