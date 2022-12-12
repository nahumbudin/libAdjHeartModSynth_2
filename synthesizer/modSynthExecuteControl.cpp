/**
*	@file		modSynthExecuteControl.cpp
*	@author		Nahum Budin
*	@date		7-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*	
*	@brief		Used for executing synthesizer settings. For example: \n
*				change a synthesizer control knob/parameter like midi-mixer chan1 level.
*
*	History:\n
*	
*	version 1.0		28-May-2020:		
*		First version
*		
*/

#include "modSynth.h"


/**
*   @brief  Execute a synthesizer control operation.
*   @param	module_id	synth module identifier: _OSC_1_EVENT, _OSC_2_EVENT, _FILTER_1_EVENT, _FILTER_2_EVENT,
*												 _AMP_CH1_EVENT, _AMP_CH2_EVENT, _KBD_1_EVENT, _NOISE_1_EVENT,
*												 _KARPLUS_1_EVENT, _REVERB_EVENT, _DISTORTION_1_EVENT, _DISTORTION_2_EVENT,
*												 _BAND_EQUALIZER_EVENT, _MSO_1_EVENT, _MIDI_MAPPING_EVENT, _PAD_1_EVENT,
*												 _SEQUENCER_1_EVENT, _RECORDER_1_EVENT, _MIDI_MIXER_1_EVENT, _FLUID_SYNTH_1_EVENT,
*												 _MOD_SYNTH_EVENT
*	@param	control_id	module specific control
*	@param	val			control value
*   @return void
*/
int ModSynth::execute_synth_control(int module_id, int control_id, int val)
{
	int value = (val * 100) / 127;
	
	if ((module_id < _OSC_1_EVENT) || (module_id > _MOD_SYNTH_EVENT) ||
		(module_id < 0) || (module_id > 0xffff) || (val < 0))
	{
		return -1;
	}
	
	switch (module_id)
	{
		case _OSC_1_EVENT:
			if ((control_id == _OSC_SYNC) || (control_id == _OSC_FIX_TONE) || 
				(control_id == _OSC_1_UNISON_SQUARE) || (control_id == _OSC_ENABLE))
			{
				if (val > 0)
				{
					adj_synth->vco_event_bool(_OSC_1_EVENT,
						control_id,
						true,
						get_adj_synth()->get_active_patch_params(),
						get_adj_synth()->get_active_sketch());
				}
				else
				{
					adj_synth->vco_event_bool(_OSC_1_EVENT,
						control_id,
						false,
						get_adj_synth()->get_active_patch_params(),
						get_adj_synth()->get_active_sketch());
				}
			}
			else
			{
				adj_synth->vco_event(_OSC_1_EVENT, 
					control_id, 
					value, 
					get_adj_synth()->get_active_patch_params(),
					get_adj_synth()->get_active_sketch());
			}				
			callback_initiate_update_gui(_OSC_1_EVENT);		
			break;
			
		case _OSC_2_EVENT:
			if ((control_id == _OSC_SYNC) || (control_id == _OSC_FIX_TONE) || 
				(control_id == _OSC_1_UNISON_SQUARE) || (control_id == _OSC_ENABLE))
			{
				if (val > 0)
				{
					adj_synth->vco_event_bool(_OSC_2_EVENT,
						control_id,
						true,
						get_adj_synth()->get_active_patch_params(),
						get_adj_synth()->get_active_sketch());
				}
				else
				{
					adj_synth->vco_event_bool(_OSC_2_EVENT,
						control_id,
						false,
						get_adj_synth()->get_active_patch_params(),
						get_adj_synth()->get_active_sketch());
				}
			}
			else
			{
				adj_synth->vco_event(_OSC_2_EVENT, 
					control_id, 
					value, 
					get_adj_synth()->get_active_patch_params(),
					get_adj_synth()->get_active_sketch());
			}				
			callback_initiate_update_gui(_OSC_2_EVENT);				
			break;
		
		case _LFO_1_EVENT:
		case _LFO_2_EVENT:
		case _LFO_3_EVENT:
		case _LFO_4_EVENT:
		case _LFO_5_EVENT:
		case _ENV_1_EVENT:
		case _ENV_2_EVENT:
		case _ENV_3_EVENT:
		case _ENV_4_EVENT:
		case _ENV_5_EVENT:
			adj_synth->modulator_event(module_id,
				control_id,
				value,
				get_adj_synth()->get_active_patch_params(),
				get_adj_synth()->get_active_sketch());
				callback_initiate_update_gui(module_id);
			break;
		
		case _FILTER_1_EVENT:
			if ((control_id != _FILTER_2_TRACK_FILT_1))
			{
				adj_synth->filter_event(_FILTER_1_EVENT, 
					control_id, 
					value, 
					get_adj_synth()->get_active_patch_params(),
					get_adj_synth()->get_active_sketch());
			}			
			callback_initiate_update_gui(_FILTER_1_EVENT);			
			break;
		
		case _FILTER_2_EVENT:
			if ((control_id != _FILTER_2_TRACK_FILT_1))
			{
				adj_synth->filter_event(_FILTER_2_EVENT, 
					control_id, 
					value, 
					get_adj_synth()->get_active_patch_params(),
					get_adj_synth()->get_active_sketch());
			}			
			callback_initiate_update_gui(_FILTER_2_EVENT);			
			break;
		
		case _AMP_CH1_EVENT:
			if (control_id != _AMP_ENV_MOD_ENABLE)
			{
				adj_synth->amp_event(_AMP_CH1_EVENT, 
					control_id, 
					value, 
					get_adj_synth()->get_active_patch_params(),
					get_adj_synth()->get_active_sketch());
			}				
			callback_initiate_update_gui(_AMP_CH1_EVENT);				
			break;
		
		case _AMP_CH2_EVENT:
			if (control_id != _AMP_ENV_MOD_ENABLE)
			{
				adj_synth->amp_event(_AMP_CH2_EVENT, 
					control_id, 
					value, 
					get_adj_synth()->get_active_patch_params(),
					get_adj_synth()->get_active_sketch());
			}				
			callback_initiate_update_gui(_AMP_CH2_EVENT);				
			break;
		
		case _KBD_1_EVENT:
			if ((control_id == _KBD_PORTAMENTO_ENABLE) || (control_id == _KBD_POLY_MODE_LIMIT) || 
				(control_id == _KBD_POLY_MODE_FIFO) || (control_id == _KBD_POLY_MODE_REUSE))
			{
				if (val > 0)
				{
					adj_synth->kbd_event_bool(_KBD_1_EVENT,
						control_id,
						true,
						adj_synth->get_active_settings_params());
				}
				else
				{
					adj_synth->kbd_event_bool(_KBD_1_EVENT,
						control_id,
											  false,
											  adj_synth->get_active_settings_params());
				}
			}
			else
			{
				adj_synth->kbd_event(_KBD_1_EVENT,
					control_id,
									 value,
									 adj_synth->get_active_settings_params());
			}				
			callback_initiate_update_gui(_KBD_1_EVENT);				
			break;
		
		case _NOISE_1_EVENT:
			if (control_id == _NOISE_ENABLE)
			{
				if (val > 0)
				{
					adj_synth->noise_event_bool(_NOISE_1_EVENT,
						control_id,
						true,
						get_adj_synth()->get_active_patch_params(),
						get_adj_synth()->get_active_sketch());
				}
				else
				{
					adj_synth->noise_event_bool(_NOISE_1_EVENT,
						control_id,
						false,
						get_adj_synth()->get_active_patch_params(),
						get_adj_synth()->get_active_sketch());
				}
			}
			else
			{
				adj_synth->noise_event(_NOISE_1_EVENT, 
					control_id, 
					value, 
					get_adj_synth()->get_active_patch_params(),
					get_adj_synth()->get_active_sketch());
			}				
			callback_initiate_update_gui(_NOISE_1_EVENT);				
			break;
		
		case _KARPLUS_1_EVENT:
			if (control_id == _KARPLUS_STRONG_ENABLE)
			{
				if (val > 0)
				{
					adj_synth->karplus_event_bool(_KARPLUS_1_EVENT,
						control_id,
						true,
						get_adj_synth()->get_active_patch_params(),
						get_adj_synth()->get_active_sketch());
				}
				else
				{
					adj_synth->karplus_event_bool(_KARPLUS_1_EVENT,
						control_id,
						false,
						get_adj_synth()->get_active_patch_params(),
						get_adj_synth()->get_active_sketch());
				}
			}
			else
			{
				adj_synth->karplus_event(_KARPLUS_1_EVENT, 
					control_id, 
					value, 
					get_adj_synth()->get_active_patch_params(),
					get_adj_synth()->get_active_sketch());
			}				
			callback_initiate_update_gui(_KARPLUS_1_EVENT);				
			break;
		
		case _REVERB_EVENT:			
			callback_initiate_update_gui(_REVERB_EVENT);
			break;
		
		case _DISTORTION_1_EVENT:			
			callback_initiate_update_gui(_DISTORTION_1_EVENT);
			break;
		
		case _DISTORTION_2_EVENT:			
			callback_initiate_update_gui(_DISTORTION_2_EVENT);
			break;
		
		
		case _BAND_EQUALIZER_EVENT:			
			callback_initiate_update_gui(_BAND_EQUALIZER_EVENT);
			break;


		case _MSO_1_EVENT:
			if (control_id == _MSO_ENABLE)
			{
				if (val > 0)
				{
					adj_synth->mso_event_bool(_MSO_1_EVENT,
											  control_id,
											  true,
											  get_adj_synth()->get_active_patch_params(),
											  get_adj_synth()->get_active_sketch());
				}
				else
				{
					adj_synth->mso_event_bool(_MSO_1_EVENT,
											  control_id,
											  false,
											  get_adj_synth()->get_active_patch_params(),
											  get_adj_synth()->get_active_sketch());
				}
			}
			else if ((control_id >= _MSO_SEGMENT_A_POSITION) && (control_id <= _MSO_SEGMENT_F_POSITION))
			{
				adj_synth->mso_event(_MSO_1_EVENT,
									 control_id,
									 (value * 2045) / 100 + 1, // 0-100 -> 1-2046
									 get_adj_synth()->get_active_patch_params(),
									 get_adj_synth()->get_active_sketch());
			}
			else
			{
				adj_synth->mso_event(_MSO_1_EVENT,
									 control_id,
									 value,
									 get_adj_synth()->get_active_patch_params(),
									 get_adj_synth()->get_active_sketch());
			}

			callback_initiate_update_gui(_MSO_1_EVENT);				
			break;

		case _MIDI_MAPPING_EVENT:			
			callback_initiate_update_gui(_MIDI_MAPPING_EVENT);
			break;

		case _PAD_1_EVENT:
			if (control_id == _PAD_ENABLE)
			{
				if (val > 0)
				{
					adj_synth->pad_event_bool(_PAD_1_EVENT,
						control_id,
						true,
						get_adj_synth()->get_active_patch_params(),
						get_adj_synth()->get_active_sketch());
				}
				else
				{
					adj_synth->pad_event_bool(_PAD_1_EVENT,
						control_id,
						false,
						get_adj_synth()->get_active_patch_params(),
						get_adj_synth()->get_active_sketch());
				}
			}
			else
			{
				adj_synth->pad_event(_PAD_1_EVENT, 
					control_id, 
					value, 
					get_adj_synth()->get_active_patch_params(),
					get_adj_synth()->get_active_sketch());
			}				
			callback_initiate_update_gui(_PAD_1_EVENT);				
			break;

		case _SEQUENCER_1_EVENT:			
			callback_initiate_update_gui(_SEQUENCER_1_EVENT);
			break;

		case _RECORDER_1_EVENT:			
			callback_initiate_update_gui(_RECORDER_1_EVENT);
			break;
			
		case _MIDI_MIXER_1_EVENT:
			adj_synth->midi_mixer_event(_MIDI_MIXER_1_EVENT, 
				control_id, 
				value,
				adj_synth->get_active_settings_params());			
			callback_initiate_update_gui(_MIDI_MIXER_1_EVENT);			
			break;
			
		case _FLUID_SYNTH_1_EVENT:
			if (control_id == _FLUID_SYNTH_CHORUS_SPEED)
			{
				fluid_synth->fluid_synth_event(control_id, (val * 461) / 127 + 29);  // 0-127 -> 29-500
			}
			else if (control_id == _FLUID_SYNTH_CHORUS_DEPTH)
			{
				fluid_synth->fluid_synth_event(control_id, (val * 210) / 127);  // 0-127 -> 0-210
			}
			else
			{
				fluid_synth->fluid_synth_event(control_id, val);
			}

			callback_initiate_update_gui(_FLUID_SYNTH_1_EVENT);
			break;
			
		case _MOD_SYNTH_EVENT:
			if (control_id == _MASTER_VOLUME)
			{
				set_master_volume(value);
				callback_initiate_update_gui(_MOD_SYNTH_EVENT);
			}
			break;
	}
	
	return 0;
}