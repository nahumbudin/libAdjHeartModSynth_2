/**
*	@file		adjSynthSettingsCallbacksVoiceFilter.cpp
*	@author		Nahum Budin
*	@date		5-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*	
*	@brief		Callback to handle voice block Filters Settings
*	settings
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

int set_voice_block_filter_1_frequency_cb(int freq, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->filter1->set_frequency(freq);
	return 0;
}

int set_voice_block_filter_1_octave_cb(int oct, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->filter1->set_octave(oct);
	return 0;
}

int set_voice_block_filter_1_q_cb(int q, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->filter1->set_resonance(q);
	return 0;
}

int set_voice_block_filter_1_kbd_track_cb(int kbdt, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->filter1->set_kbd_track(kbdt);
	return 0;
}

int set_voice_block_filter_1_band_cb(int bnd, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->filter1->set_band(bnd);
	return 0;
}

int set_voice_block_filter_1_freq_modulation_lfo_num_cb(int lfo, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_filter1_freq_mod_lfo(lfo);
	return 0;
}

int set_voice_block_filter_1_freq_modulation_lfo_level_cb(int lev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_filter1_freq_mod_lfo_level(lev);
	return 0;
}

int set_voice_block_filter_1_freq_modulation_env_num_cb(int env, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_filter1_freq_mod_env(env);
	return 0;
}

int set_voice_block_filter_1_freq_modulation_env_level_cb(int lev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_filter1_freq_mod_env_level(lev);
	return 0;
}


int set_voice_block_filter_2_frequency_cb(int freq, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->filter2->set_frequency(freq);
	return 0;
}

int set_voice_block_filter_2_octave_cb(int oct, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->filter2->set_octave(oct);
	return 0;
}

int set_voice_block_filter_2_q_cb(int q, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->filter2->set_resonance(q);
	return 0;
}

int set_voice_block_filter_2_kbd_track_cb(int kbdt, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->filter2->set_kbd_track(kbdt);
	return 0;
}

int set_voice_block_filter_2_band_cb(int bnd, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->filter2->set_band(bnd);
	return 0;
}

int set_voice_block_filter_2_freq_modulation_lfo_num_cb(int lfo, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_filter2_freq_mod_lfo(lfo);
	return 0;
}

int set_voice_block_filter_2_freq_modulation_lfo_level_cb(int lev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_filter2_freq_mod_lfo_level(lev);
	return 0;
}

int set_voice_block_filter_2_freq_modulation_env_num_cb(int env, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_filter2_freq_mod_env(env);
	return 0;
}

int set_voice_block_filter_2_freq_modulation_env_level_cb(int lev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_filter2_freq_mod_env_level(lev);
	return 0;
}

