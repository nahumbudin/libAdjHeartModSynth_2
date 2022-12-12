/**
*	@file		adjSynthEventsHandlingKeyboard.cpp
*	@author		Nahum Budin
*	@date		16-12-2019
*	@version	1.0
*
*	@brief		AdjHeart Synthesizer Keyboard Events Handling
*
*	History:\n
*
*	version 1.0		16-12-2019:
*		First version
*
*/

#include "adjSynth.h"

int AdjSynth::kbd_event(int kbid, int eventid, int val, _setting_params_t *params)
{
	if (kbid == _KBD_1_EVENT)
	{
		switch (eventid)
		{
			case _KBD_PORTAMENTO_LEVEL:
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.keyboard.portamento",
					val,
					_EXEC_CALLBACK,
					-1);	// -1: no program (common resource)

				break;

			case _KBD_SENSITIVITY_LEVEL:
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.keyboard.sensetivity",
					val,
					_EXEC_CALLBACK,
					-1);

				break;

			case _KBD_LOW_SENSITIVITY_LEVEL:
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.keyboard.sensetivity_low",
					val,
					_EXEC_CALLBACK,
					-1);

				break;

			case _KBD_SPLIT_POINT:
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.keyboard.split_point",
					val,
					_EXEC_CALLBACK,
					-1);

				break;

			default:
				return 0;
		}
	}

	return 0;
}

int AdjSynth::kbd_event_bool(int kbid, int eventid, bool val, _setting_params_t *params)
{
	if (kbid == _KBD_1_EVENT)
	{
		switch (eventid)
		{
			case _KBD_PORTAMENTO_ENABLE:
				adj_synth_settings_manager->set_bool_param_value
					(params,
					"adjsynth.keyboard.portamento_state",
					val,
					_EXEC_CALLBACK,
					-1);

				break;

			case _KBD_POLY_MODE_LIMIT:
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.keyboard.polyphonic_mode",
					_KBD_POLY_MODE_LIMIT,
					_EXEC_CALLBACK,
					-1);

				
				break;

			case _KBD_POLY_MODE_FIFO:
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.keyboard.polyphonic_mode",
					_KBD_POLY_MODE_FIFO,
					_EXEC_CALLBACK,
					-1);
				
				break;

			case _KBD_POLY_MODE_REUSE:
				adj_synth_settings_manager->set_int_param_value
					(params,
					"adjsynth.keyboard.polyphonic_mode",
					_KBD_POLY_MODE_REUSE,
					_EXEC_CALLBACK,
					-1);

				break;

			default:
				return 0;
		}
	}

	return 0;
}
