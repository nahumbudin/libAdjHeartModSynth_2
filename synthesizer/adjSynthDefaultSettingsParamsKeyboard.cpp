/**
*	@file		adjSynthDefaultSettingsKeyboard.cpp
*	@author		Nahum Budin
*	@date		27-Dec-2019
*	@version	1.0
*
*	@brief		Set default settings Keyboard parameters
*
*	History:\n
*
*	version 1.0		15_Nov-2019:
*		First version
*
*/

#include "adjSynth.h"

/**
*   @brief  Set the settings Keyboard parameters to their default values
*   @param	params	a _setting_params_t parameters struct
*   @return 0 if done
*/
int AdjSynth::set_default_settings_parameters_keyboard(_setting_params_t *params)
{
	int res = 0;

	return_val_if_true(params == NULL, _SETTINGS_BAD_PARAMETERS);

	res = adj_synth_settings_manager->set_int_param
	(params,
		"adjsynth.keyboard.portamento",
		1,
		100,
		1,
		_PARAM_TYPE_ADJ_SYNTH_SETUP,
		set_keyboard_portamento_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK, 
		-1);	// -1: no prog (common resource)

	res |= adj_synth_settings_manager->set_int_param
		(params,
		"adjsynth.keyboard.sensetivity",
		50,
		100,
		0,
		_PARAM_TYPE_ADJ_SYNTH_SETUP,
		set_keyboard_sensetivity_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= adj_synth_settings_manager->set_int_param
		(params,
		"adjsynth.keyboard.sensetivity_low",
		50,
		100,
		0,
		_PARAM_TYPE_ADJ_SYNTH_SETUP,
		set_keyboard_sensetivity_low_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= adj_synth_settings_manager->set_int_param
		(params,
		"adjsynth.keyboard.split_point",
		_KBD_SPLIT_POINT_NONE,
		_KBD_SPLIT_POINT_C5,
		_KBD_SPLIT_POINT_NONE,
		_PARAM_TYPE_ADJ_SYNTH_SETUP,
		set_keyboard_split_point_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= adj_synth_settings_manager->set_int_param
		(params,
		"adjsynth.keyboard.polyphonic_mode",
		_KBD_POLY_MODE_FIFO,
		_KBD_POLY_MODE_REUSE,
		_KBD_POLY_MODE_LIMIT,
		_PARAM_TYPE_ADJ_SYNTH_SETUP,
		set_keyboard_polyphonic_mode_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL |
		_SET_TYPE | _SET_CALLBACK,
		-1);

	res |= adj_synth_settings_manager->set_bool_param
		(params,
		"adjsynth.keyboard.portamento_state",
		false,
		_PARAM_TYPE_ADJ_SYNTH_SETUP,
		set_keyboard_portamento_enable_state_cb,
		0,
		0,
		NULL,
		_SET_VALUE | _SET_TYPE | _SET_CALLBACK,
		-1);

	return res;
}