/**
*	@file		adjSynthDefaultPatchParamsAmp.cpp
*	@author		Nahum Budin
*	@date		15_Nov-2019
*	@version	1.0
*	
*	@brief		Set default patch Amps parameters
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

int AdjSynth::set_default_patch_parameters_amp(_setting_params_t *params, int prog)
{
	int res;
//   Amp level, pan send mixer settings not patch?
	res = adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.amp_ch1.level_cb",
				50,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_amp_ch_1_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.amp_ch1.pan",
				50,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_amp_ch_1_pan_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.amp_ch1.send",
				50,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_amp_ch_1_send_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.amp_ch1.pan_modulation_lfo_num",
				_LFO_NONE,
				_LFO_5,
				_LFO_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_amp_ch_1_pan_modulation_lfo_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.amp_ch1.pan_modulation_lfo_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_amp_ch_1_pan_modulation_lfo_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
//   Amp level, pan send mixer settings not patch?
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.amp_ch2.level",
				50,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_amp_ch_2_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.amp_ch2.pan",
				50,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_amp_ch_2_pan_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.amp_ch2.send",
				50,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_amp_ch_2_send_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.amp_ch2.pan_modulation_lfo_num",
				_LFO_NONE,
				_LFO_5,
				_LFO_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_amp_ch_2_pan_modulation_lfo_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.amp_ch2.pan_modulation_lfo_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_amp_ch_2_pan_modulation_lfo_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);

	res |= adj_synth_settings_manager->set_bool_param
				(params,
				"adjsynth.amp.fixed_levels_enabled",
				true,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_amp_fixed_levels_state_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	return res;
}