/**
*	@file		adjSynthSettingsCallbacksVoiceKPS.cpp
*	@author		Nahum Budin
*	@date		5-Feb-2021
*	@version	1.0
*					1. Code refactoring and notaion.
*	
*	@brief		Callback to handle voice block KarplusStrong String Generator Settings
*	settings
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

int set_voice_block_karplus_synth_enabled_cb(bool enable, int voice, int prog)
{
	if (enable)
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->enable_karplus();
	}
	else
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->disable_karplus();
	}
	return 0;
}

int set_voice_block_karplus_synth_excitation_waveform_type_cb(int type, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->karplus1->set_excitation_waveform_type(type);
	return 0;
}

int set_voice_block_karplus_synth_excitation_waveform_variations_cb(int var, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->karplus1->set_excitation_waveform_variations(var);
	return 0;
}

int set_voice_block_karplus_synth_decay_cb(int dec, int voice, int prog)
{
	// Repalced with on decay and off decay below
	return 0;
}

int set_voice_block_karplus_synth_pluck_damping_cb(int dump, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->karplus1->set_pluck_damping(dump);
	return 0;
}

int set_voice_block_karplus_synth_pluck_damping_variations_cb(int dump, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->karplus1->set_pluck_damping_variation(dump);
	return 0;
}

int set_voice_block_karplus_synth_string_damping_cb(int dump, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->karplus1->set_string_damping(dump);
	return 0;
}

int set_voice_block_karplus_synth_string_damping_variations_cb(int dump, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->karplus1->set_pluck_damping_variation(dump);
	return 0;
}

int set_voice_block_karplus_synth_string_damping_calculation_mode_cb(int mode, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->karplus1->set_string_dumping_calculation_mode(mode);
	return 0;
}

int set_voice_block_karplus_synth_send_filter_1_cb(int snd, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_karplus1_send_filter1_level(snd);
	return 0;
}

int set_voice_block_karplus_synth_send_filter_2_cb(int snd, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_karplus1_send_filter2_level(snd);
	return 0;
}

int set_voice_block_karplus_synth_on_decay_cb(int dec, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->karplus1->set_on_decay(dec);
	return 0;
}

int set_voice_block_karplus_synth_off_decay_cb(int dec, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->karplus1->set_off_decay(dec);
	return 0;
}
