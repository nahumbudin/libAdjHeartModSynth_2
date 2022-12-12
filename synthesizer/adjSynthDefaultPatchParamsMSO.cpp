/**
*	@file		adjSynthDefaultPatchParamsMSO.cpp
*	@author		Nahum Budin
*	@date		15_Nov-2019
*	@version	1.0
*	
*	@brief		Set default patch Morphed Sine Generator parameters
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

int AdjSynth::set_default_patch_parameters_mso(_setting_params_t *params, int prog)
{
	int res;
	
	res = adj_synth_settings_manager->set_bool_param
				(params,
				"adjsynth.mso_synth.enabled",
				false,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_enabled_cb,
				_SET_VALUE | _SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res = adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.symmetry",
				70,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_mso_synth_symmetry_cb,
				0,
				0,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.send_filter_1",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_send_filter_1_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.send_filter_2",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_send_filter_2_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.tune_offset_oct",
				0,
				_OSC_DETUNE_MAX_OCTAVE,
				_OSC_DETUNE_MIN_OCTAVE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_tune_offset_oct_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.tune_offset_semitones",
				0,
				_OSC_DETUNE_MAX_SEMITONES,
				_OSC_DETUNE_MIN_SEMITONES,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_tune_offset_semitones_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.tune_offset_cents",
				0,
				_OSC_DETUNE_MAX_CENTS / _OSC_DETUNE_CENTS_FACTORIAL,
				_OSC_DETUNE_MIN_CENTS / _OSC_DETUNE_CENTS_FACTORIAL,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_tune_offset_cents_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.freq_modulation_lfo_num",
				_LFO_NONE,
				_LFO_5_DELAYED_2000MS,
				_LFO_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_freq_modulation_lfo_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.freq_modulation_lfo_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_freq_modulation_lfo_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.freq_modulation_env_num",
				_ENV_NONE,
				_ENV_5,
				_ENV_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_freq_modulation_env_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.freq_modulation_env_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_freq_modulation_env_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.pwm_modulation_lfo_num",
				_LFO_NONE,
				_LFO_5_DELAYED_2000MS,
				_LFO_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_pwm_modulation_lfo_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.pwm_modulation_lfo_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_pwm_modulation_lfo_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.pwm_modulation_env_num",
				_ENV_NONE,
				_ENV_5,
				_ENV_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_pwm_modulation_env_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.pwm_modulation_env_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_pwm_modulation_env_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.amp_modulation_lfo_num",
				_LFO_NONE,
				_LFO_5_DELAYED_2000MS,
				_LFO_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_amp_modulation_lfo_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.amp_modulation_lfo_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_amp_modulation_lfo_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.amp_modulation_env_num",
				_ENV_NONE,
				_ENV_5,
				_ENV_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_amp_modulation_env_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.amp_modulation_env_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_mso_synth_amp_modulation_env_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.segment_position_a",
				512,
				2047,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_mso_synth_segment_position_a_cb,
				0,
				0,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.segment_position_b",
				513,
				2047,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_mso_synth_segment_position_b_cb,
				0,
				0,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.segment_position_c",
				1024,
				2047,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_mso_synth_segment_position_c_cb,
				0,
				0,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.segment_position_d",
				1025,
				2047,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_mso_synth_segment_position_d_cb,
				0,
				0,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.segment_position_e",
				1536,
				2047,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_mso_synth_segment_position_e_cb,
				0,
				0,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.mso_synth.segment_position_f_cb",
				1537,
				2047,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_mso_synth_segment_position_f_cb,
				0,
				0,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);

/*	res |= adj_synth_settings_manager->set_int_param
	(params,
		"adjsynth.mso_synth.preset",
		1, // sine
		4, // saw TODO:
		0,
		_PARAM_TYPE_ADJ_SYNTH_PATCH,
		set_mso_synth_segment_preset,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_BLOCK_START_INDEX |
		_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
		prog);
*/
	
	return res;
}