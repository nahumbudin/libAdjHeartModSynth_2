/**
*	@file		adjSynthDefaultPatchParamsKPS.cpp
*	@author		Nahum Budin
*	@date		15_Nov-2019
*	@version	1.0
*	
*	@brief		Set default patch Karplus Strong String Generator parameters
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

int AdjSynth::set_default_patch_parameters_kps(_setting_params_t *params, int prog)
{
	int res;

	res = adj_synth_settings_manager->set_bool_param
				(params,
				"adjsynth.karplus_synth.enabled",
				false,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_enabled_cb,
				_SET_VALUE | _SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.karplus_synth.excitation_waveform_type",
				_KARPLUS_STRONG_EXCITATION_WHITE_NOISE,
				_KARPLUS_STRONG_EXCITATION_SQUARE_WAVE,
				_KARPLUS_STRONG_EXCITATION_WHITE_NOISE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_excitation_waveform_type_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.karplus_synth.excitation_waveform_variations",
				50,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_excitation_waveform_variations_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.karplus_synth.decay",
				80,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_decay_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.karplus_synth.pluck_damping",
				50,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_pluck_damping_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.karplus_synth.pluck_damping_variations",
				50,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_pluck_damping_variations_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.karplus_synth.string_damping",
				50,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_string_damping_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.karplus_synth.string_damping_variations",
				50,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_string_damping_variations_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.karplus_synth.string_damping_calculation_mode",
				_KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT,
				_KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC,
				_KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_string_damping_calculation_mode_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.karplus_synth.send_filter_1",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_send_filter_1_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.karplus_synth.send_filter_2",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_send_filter_2_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.karplus_synth.on_decay",
				80,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_on_decay_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.karplus_synth.off_decay",
				80,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_karplus_synth_off_decay_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	return res;
}