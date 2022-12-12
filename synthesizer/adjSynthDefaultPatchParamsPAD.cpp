/**
*	@file		adjSynthDefaultPatchParamsPAD.cpp
*	@author		Nahum Budin
*	@date		15_Nov-2019
*	@version	1.0
*	
*	@brief		Set default patch PAD Synthesizer parameters
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

int AdjSynth::set_default_patch_parameters_pad(_setting_params_t *params, int prog)
{
	int res;
	
	res = adj_synth_settings_manager->set_bool_param
				(params,
				"adjsynth.pad_synth.enabled",
				false,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_enabled_cb,
				_SET_VALUE | _SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.tune_offset_octave",
				0,
				_OSC_DETUNE_MAX_OCTAVE,
				_OSC_DETUNE_MIN_OCTAVE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_detune_octave_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.tune_offset_semitones",
				0,
				_OSC_DETUNE_MAX_SEMITONES,
				_OSC_DETUNE_MIN_SEMITONES,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_detune_semitones_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.tune_offset_cents",
				0,
				_OSC_DETUNE_MAX_CENTS / _OSC_DETUNE_CENTS_FACTORIAL,
				_OSC_DETUNE_MIN_CENTS / _OSC_DETUNE_CENTS_FACTORIAL,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_detune_cents_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.send_filter_1",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_send_filter_1_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.send_filter_2",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_send_filter_2_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.freq_modulation_lfo_num",
				_LFO_NONE,
				_LFO_5_DELAYED_2000MS,
				_LFO_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_freq_modulation_lfo_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.freq_modulation_lfo_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_freq_modulation_lfo_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.freq_modulation_env_num",
				_ENV_NONE,
				_ENV_5,
				_ENV_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_freq_modulation_env_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.freq_modulation_env_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_freq_modulation_env_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.amp_modulation_lfo_num",
				_LFO_NONE,
				_LFO_5_DELAYED_2000MS,
				_LFO_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_amp_modulation_lfo_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.amp_modulation_lfo_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_amp_modulation_lfo_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.amp_modulation_env_num",
				_ENV_NONE,
				_ENV_5,
				_ENV_NONE,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_amp_modulation_env_num_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.amp_modulation_env_level",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				NULL,
				0,
				num_of_voices - 1,
				set_voice_block_pad_synth_amp_modulation_env_level_cb,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_BLOCK_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.quality",
				_PAD_QUALITY_128K,
				_PAD_QUALITY_1024K,
				_PAD_QUALITY_32K,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_quality_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.base_note",
				_PAD_BASE_NOTE_C4,
				_PAD_BASE_NOTE_G6,
				_PAD_BASE_NOTE_C2,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_base_note_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.base_width",
				30,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_base_width_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.shape",
				_PAD_SHAPE_RECTANGULAR,
				_PAD_SHAPE_DOUBLE_EXP,
				_PAD_SHAPE_RECTANGULAR,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_shape_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.shape_cutoff",
				_PAD_SHAPE_CUTOFF_FULL,
				_PAD_SHAPE_CUTOFF_LOWER,
				_PAD_SHAPE_CUTOFF_FULL,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_shape_cutoff_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.harmonies_level_0",
				100,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_harmonies_level_0_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.harmonies_level_1",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_harmonies_level_1_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.harmonies_level_2",
				30,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_harmonies_level_2_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.harmonies_level_3",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_harmonies_level_3_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.harmonies_level_4",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_harmonies_level_4_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.harmonies_level_5",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_harmonies_level_5_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.harmonies_level_6",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_harmonies_level_6_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.harmonies_level_7",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_harmonies_level_7_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.harmonies_level_8",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_harmonies_level_8_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.harmonies_level_9",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_harmonies_level_9_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	res |= adj_synth_settings_manager->set_int_param
				(params,
				"adjsynth.pad_synth.harmonies_detune",
				0,
				100,
				0,
				_PARAM_TYPE_ADJ_SYNTH_PATCH,
				set_voice_block_pad_synth_harmonies_detune_cb,
				0,
				num_of_voices - 1,
				NULL,
				_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | 
				_SET_TYPE | _SET_BLOCK_START_INDEX | 
				_SET_BLOCK_STOP_INDEX | _SET_CALLBACK,
				prog);
	
	return res;
}
