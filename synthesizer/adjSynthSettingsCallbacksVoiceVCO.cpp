/**
*	@file		adjSynthSettingsCallbacksVoiceVCO.cpp
*	@author		Nahum Budin
*	@date		5-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*	
*	@brief		Callback to handle voice block VCOs settings
*
*	History:\n
*	
*	version 1.0		15_Nov-2019:		
*		First version
*		
*/

#include "adjSynth.h"

int set_voice_block_osc_1_enabled_cb(bool enable, int voice, int prog)
{
	if (enable)
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->enable_osc1();
	}
	else
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->disable_osc1();
	}
	return 0;
}

int set_voice_block_osc_1_waveform_cb(int wvf, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_waveform(wvf);
	return 0;
}

int set_voice_block_osc_1_pwm_symmetry_cb(int sym, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_pwm_dcycle(sym);
	return 0;
}

int set_voice_block_osc_1_send_filter_1_cb(int snd, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_send_filter1_level(snd);
	return 0;
}

int set_voice_block_osc_1_send_filter_2_cb(int snd, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_send_filter2_level(snd);
	return 0;
}

int set_voice_block_osc_1_tune_offset_oct_cb(int oct, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_freq_detune_oct(oct);
	return 0;
}

int set_voice_block_osc_1_tune_offset_semitones_cb(int smt, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_freq_detune_semitones(smt);
	return 0;
}

int set_voice_block_osc_1_tune_offset_cents_cb(int cnt, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_freq_detune_cents(cnt);
	return 0;
}

int set_voice_block_osc_1_freq_modulation_lfo_num_cb(int lfon, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_freq_mod_lfo(lfon);
	return 0;
}

int set_voice_block_osc_1_freq_modulation_lfo_level_cb(int lfolev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_freq_mod_lfo_level(lfolev);
	return 0;
}

int set_voice_block_osc_1_freq_modulation_env_num_cb(int envn, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_freq_mod_env(envn);
	return 0;
}

int set_voice_block_osc_1_freq_modulation_env_level_cb(int envlev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_freq_mod_env_level(envlev);
	return 0;
}

int set_voice_block_osc_1_pwm_modulation_lfo_num_cb(int lfon, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_pwm_mod_lfo(lfon);
	return 0;
}

int set_voice_block_osc_1_pwm_modulation_lfo_level_cb(int lfolev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_pwm_mod_lfo_level(lfolev);
	return 0;
}

int set_voice_block_osc_1_pwm_modulation_env_num_cb(int envn, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_pwm_mod_env(envn);
	return 0;
}

int set_voice_block_osc_1_pwm_modulation_env_level_cb(int envlev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_pwm_mod_env_level(envlev);
	return 0;
}

int set_voice_block_osc_1_amp_modulation_lfo_num_cb(int lfon, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_amp_mod_lfo(lfon);
	return 0;
}

int set_voice_block_osc_1_amp_modulation_lfo_level_cb(int lfolev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_amp_mod_lfo_level(lfolev);
	return 0;
}

int set_voice_block_osc_1_amp_modulation_env_num_cb(int envn, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_amp_mod_env(envn);
	return 0;
}

int set_voice_block_osc_1_amp_modulation_env_level_cb(int envlev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc1_amp_mod_env_level(envlev);
	return 0;
}

int set_voice_block_osc_1_unison_mod_cb(int unimod, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_unison_mode(unimod);
	if (voice == 0)
	{
		callback_set_osc1_unison_mode(unimod);
	}
	return 0;
}

int set_voice_block_osc_1_hammond_percussion_mode_cb(int pmode, int voice, int prog)
{
	if (voice == 0)
	{
		AdjSynth::get_instance()->set_hammond_percusion_mode(pmode, AdjSynth::get_instance()->get_active_settings_params(), prog);
	}
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->set_voice_params(AdjSynth::get_instance()->get_active_settings_params());
	return 0;
}

int set_voice_block_osc_1_unison_level_1_cb(int level, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_harmony_level(0, level);
	return 0;
}

int set_voice_block_osc_1_unison_level_2_cb(int level, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_harmony_level(1, level);
	return 0;
}

int set_voice_block_osc_1_unison_level_3_cb(int level, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_harmony_level(2, level);
	return 0;
}

int set_voice_block_osc_1_unison_level_4_cb(int level, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_harmony_level(3, level);
	return 0;
}

int set_voice_block_osc_1_unison_level_5_cb(int level, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_harmony_level(4, level);
	return 0;
}

int set_voice_block_osc_1_unison_level_6_cb(int level, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_harmony_level(5, level);
	return 0;
}

int set_voice_block_osc_1_unison_level_7_cb(int level, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_harmony_level(6, level);
	return 0;
}

int set_voice_block_osc_1_unison_level_8_cb(int level, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_harmony_level(7, level);
	return 0;
}

int set_voice_block_osc_1_unison_level_9_cb(int level, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_harmony_level(8, level);
	return 0;
}

int set_voice_block_osc_1_unison_distortion_cb(int dist, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_harmonies_distortion(dist);
	return 0;
}

int set_voice_block_osc_1_unison_detune_cb(int det, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->set_harmonies_detune(det);
	return 0;
}

int set_voice_block_osc_1_unison_set_square_cb(bool sqr, int voice, int prog)
{
	if (sqr)
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->enable_unison_square();
	}
	else
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc1->disable_unison_square();
	}
	return 0;
}


int set_voice_block_osc_2_enabled_cb(bool enable, int voice, int prog)
{
	if (enable)
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->enable_osc2();
	}
	else
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->disable_osc2();
	}
	return 0;
}

int set_voice_block_osc_2_waveform_cb(int wvf, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc2->set_waveform(wvf);
	return 0;
}

int set_voice_block_osc_2_pwm_symmetry_cb(int sym, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc2->set_pwm_dcycle(sym);
	return 0;
}

int set_voice_block_osc_2_send_filter_1_cb(int snd, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_send_filter1_level(snd);
	return 0;
}

int set_voice_block_osc_2_send_filter_2_cb(int snd, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_send_filter2_level(snd);
	return 0;
}

int set_voice_block_osc_2_tune_offset_oct_cb(int oct, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc2->set_freq_detune_oct(oct);
	return 0;
}

int set_voice_block_osc_2_tune_offset_semitones_cb(int smt, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc2->set_freq_detune_semitones(smt);
	return 0;
}

int set_voice_block_osc_2_tune_offset_cents_cb(int cnt, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->osc2->set_freq_detune_cents(cnt);
	return 0;
}

int set_voice_block_osc_2_freq_modulation_lfo_num_cb(int lfon, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_freq_mod_lfo(lfon);
	return 0;
}

int set_voice_block_osc_2_freq_modulation_lfo_level_cb(int lfolev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_freq_mod_lfo_level(lfolev);
	return 0;
}

int set_voice_block_osc_2_freq_modulation_env_num_cb(int envn, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_freq_mod_env(envn);
	return 0;
}

int set_voice_block_osc_2_freq_modulation_env_level_cb(int envlev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_freq_mod_env_level(envlev);
	return 0;
}

int set_voice_block_osc_2_pwm_modulation_lfo_num_cb(int lfon, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_pwm_mod_lfo(lfon);
	return 0;
}

int set_voice_block_osc_2_pwm_modulation_lfo_level_cb(int lfolev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_pwm_mod_lfo_level(lfolev);
	return 0;
}

int set_voice_block_osc_2_pwm_modulation_env_num_cb(int envn, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_pwm_mod_env(envn);
	return 0;
}

int set_voice_block_osc_2_pwm_modulation_env_level_cb(int envlev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_pwm_mod_env_level(envlev);
	return 0;
}

int set_voice_block_osc_2_amp_modulation_lfo_num_cb(int lfon, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_amp_mod_lfo(lfon);
	return 0;
}

int set_voice_block_osc_2_amp_modulation_lfo_level_cb(int lfolev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_amp_mod_lfo_level(lfolev);
	return 0;
}

int set_voice_block_osc_2_amp_modulation_env_num_cb(int envn, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_amp_mod_env(envn);
	return 0;
}

int set_voice_block_osc_2_amp_modulation_env_level_cb(int envlev, int voice, int prog)
{
	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_amp_mod_env_level(envlev);
	return 0;
}

int set_voice_block_osc_2_sync_on_osc_1_state_cb(bool sync, int voice, int prog)
{
	//	AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dspVoice->osc2->setSyncState(sync);
	if(sync)
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_sync_on_osc1();
	}
	else
	{
		AdjSynth::get_instance()->synth_program[prog]->synth_voices[voice]->dsp_voice->set_osc2_not_sync_on_osc1();
	}
	
	return 0;
}

