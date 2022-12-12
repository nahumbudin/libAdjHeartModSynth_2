/**
*	@file		adjSynthDefaultSettingsParamsAudio.cpp
*	@author		Nahum Budin
*	@date		18-Jan-2021
*	@version	1.0
*	
*	@brief		Set default audio settings parameters
*
*	History:\n
*	
*/

#include "modSynth.h"


int ModSynth::set_default_settings_parameters_audio(_setting_params_t *params)
{
	int res;

	res = general_settings_manager->set_int_param(params,
										   "adjsynth.audio.driver_type",
										   _AUDIO_JACK,
										   _AUDIO_ALSA,
										   _AUDIO_JACK,
										   _PARAM_TYPE_ADJ_SYNTH_SETUP,
										   set_audio_driver_type_cb,
										   0,
										   0,
										   NULL,
										   _SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
											   _SET_TYPE | _SET_CALLBACK,
										   -1);
	
	res |= general_settings_manager->set_int_param(params,
										   "adjsynth.audio.block_size",
										   _AUDIO_BLOCK_SIZE_512,
										   _AUDIO_BLOCK_SIZE_1024,
										   _AUDIO_BLOCK_SIZE_256,
										   _PARAM_TYPE_ADJ_SYNTH_SETUP,
										   set_audio_block_size_cb,
										   0,
										   0,
										   NULL,
										   _SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
											   _SET_TYPE | _SET_CALLBACK,
										   -1);

	res |= general_settings_manager->set_int_param(params,
										   "adjsynth.audio.sample_rate",
										   _SAMPLE_RATE_44,
										   _SAMPLE_RATE_48,
										   _SAMPLE_RATE_44,
										   _PARAM_TYPE_ADJ_SYNTH_SETUP,
										   set_audio_sample_rate_cb,
										   0,
										   0,
										   NULL,
										   _SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
											   _SET_TYPE | _SET_CALLBACK,
										   -1);

	res |= general_settings_manager->set_bool_param(params,
											"adjsynth.audio_jack.auto_start_state",
											_JACK_AUTO_START_EN,
											_PARAM_TYPE_ADJ_SYNTH_SETUP,
											set_audio_jack_auto_start_state_cb,
											0,
											0,
											NULL,
											_SET_VALUE | _SET_TYPE | _SET_CALLBACK,
											-1);
	
	res |= general_settings_manager->set_int_param(params,
										  "adjsynth.audio_jack.mode",
										  _DEFAULT_JACK_MODE,
										  _JACK_MODE_SERVER_CONTROL,
										  _JACK_MODE_APP_CONTROL,
										  _PARAM_TYPE_ADJ_SYNTH_SETUP,
											set_audio_jack_mode_cb,
										  0,
										  0,
										  NULL,
										  _SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
										  _SET_TYPE | _SET_CALLBACK,
										  -1);	// Global, no program specific

	res |= general_settings_manager->set_bool_param(params,
											"adjsynth.audio_jack.auto_connect_state",
											_JACK_AUTO_CONNECT_AUDIO_EN,
											_PARAM_TYPE_ADJ_SYNTH_SETUP,
											set_audio_jack_auto_connect_state_cb,
											0,
											0,
											NULL,
											_SET_VALUE | _SET_TYPE | _SET_CALLBACK,
											-1);
	return res;
}