/**
*	@file		adjSynthDefaultPatchParamsNoise.cpp
*	@author		Nahum Budin
*	@date		15_Nov-2019
*	@version	1.0
*	
*	@brief		Set default patch Noise Generator parameters
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

int AdjSynth::set_default_patch_parameters_noise(_setting_params_t *params, int prog)
{
	int res;
	
	res = adj_synth_settings_manager->set_bool_param
				(params,
				"adjsynth.noise.enabled",
				false,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_noise_enabled_cb,
				_SET_VALUE | _SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.noise.color",
				_WHITE_NOISE,
				_BROWN_NOISE,
				_WHITE_NOISE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_noise_color_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.noise.send_filter_1",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_noise_send_filter_1_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.noise.send_filter_2",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_noise_send_filter_2_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.noise.amp_modulation_lfo_num",
				_LFO_NONE,
				_LFO_5_DELAYED_2000MS,
				_LFO_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_noise_amp_modulation_lfo_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.noise.amp_modulation_lfo_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_noise_amp_modulation_lfo_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.noise.amp_modulation_env_num",
				_ENV_NONE,
				_ENV_5,
				_ENV_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_noise_amp_modulation_env_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.noise.amp_modulation_env_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_noise_amp_modulation_env_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
		
	return res;	
}
