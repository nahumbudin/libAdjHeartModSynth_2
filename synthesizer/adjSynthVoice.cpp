/**
*	@file		adjSynthVoice.cpp
*	@author		Nahum Budin
*	@date		2-Feb-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and bloc-size settings
*					
*	@version	1.0		9-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 June 13, 2018)
*
*	@brief		Synthesizer voice object
*/

#include "adjSynthVoice.h"

/**
*	@brief	Creates a SynthVoice instance
*	@param	vnum	voice id number 
*	@param	prg		allocated to program number
*	@param	params	a pointer to a _setting_params_t structure (patch parameters) 
*	@param	msolut	a pointer to a DSP_MorphingSinusOscLUT object
*	@param	synthPADwavetable a pointer to a Wavetable object
*	@return none
*/
SynthVoice::SynthVoice(
	int vnum,
	int prg,
	int samp_rate,
	int block_size,
	_setting_params_t *params,
	DSP_MorphingSinusOscWTAB *msotab,
	Wavetable *synth_pad_wavetable, 
	AudioManager *aud_mng)
{
	set_sample_rate(samp_rate);
	set_audio_block_size(block_size);
	
	audio_manager = aud_mng;
	
	mso_wtab = msotab;
	pad_wavetable = synth_pad_wavetable;
	
	update_in_progress = false;

	voice_num = vnum;
	set_allocated_program(prg);
	allocated_to_program_voice_num = -1;
	ModSynthSettings::settings_params_deep_copy(&active_params, params);
		
	dsp_voice = new DSP_Voice(voice_num, sample_rate, audio_block_size, mso_wtab, pad_wavetable);
	audio_voice = new AudioVoiceFloat(
			_AUDIO_STAGE_2,			// process stage 2
			sample_rate,
			audio_block_size,
			voice_num, 
			2,						// number of outputs
			dsp_voice, 
			&audio_first_update[_AUDIO_STAGE_2]); 

	audio_out = new AudioOutputFloat(
			_AUDIO_STAGE_7, 
			audio_block_size,
			audio_manager->audio_block_stereo_float_shared_memory_voices_output[voice_num], // Output sample shared momory page
			&audio_first_update[_AUDIO_STAGE_7]);
	
	// Audio connections - Synth Voice -> Audio Output
	connection_voice_out_ch1 = audio_manager->connections_manager->get_audio_connection();
	connection_voice_out_ch2 = audio_manager->connections_manager->get_audio_connection();
	
	connection_voice_out_ch1->connect(audio_voice, _SYNTH_VOICE_OUT_1, audio_out, _AMP_AUDIO_IN_CH1);
	connection_voice_out_ch2->connect(audio_voice, _SYNTH_VOICE_OUT_2, audio_out, _AMP_AUDIO_IN_CH2);

	settings_manager = new ModSynthSettings(&active_params);

	set_voice_params(&active_params);
}

/**
*   @brief  Return a pointer to the SynthVoice object instance
*   @param  none
*   @return a pointer to the SynthVoice object instance
*/
SynthVoice *SynthVoice::get_instance() { return this; }

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/
int SynthVoice::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}

	if (audio_voice)
	{
		audio_voice->set_sample_rate(sample_rate);
	}

	if (mso_wtab)
	{
		mso_wtab->set_sample_rate(sample_rate);
	}

	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int SynthVoice::get_sample_rate() { return sample_rate; }

/**
*   @brief  sets the audio block size 
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int SynthVoice::set_audio_block_size(int size)
{
	int res = 0;
	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;

		if (audio_voice)
		{
			audio_voice->set_audio_block_size(audio_block_size);
		}

		if (audio_out)
		{
			audio_out->set_audio_block_size(audio_block_size);
		}

		res = audio_block_size;
	}
	else
	{
		res = -1;
	}
	
	return res;
}
	
/**
*   @brief  retruns the audio block size  
*   @param  none
*   @return buffer size
*/	
int SynthVoice::get_audio_block_size() { return audio_block_size; }

/**
*   @brief  Set voice allocated program number
*   @param  prg		program number
*   @return void
*/
void SynthVoice::set_allocated_program(int prg)
{
	if ((prg >= 0) && (prg <= mod_synth_get_synthesizer_num_of_programs()))
	{
		allocated_to_program_num = prg;
	}
}

/**
*   @brief  Return the voice allocated program number
*   @param  none
*   @return the voice allocated program number
*/
int SynthVoice::get_allocated_program() { return allocated_to_program_num;  }

/**
*   @brief  Calculate next voice output audio block samples and transmit the blocks
*   @param  none
*   @return void
*/
void SynthVoice::update_all()
{
	AudioBlockFloat *p;
	uint8_t stage;
	int coreNum, ID;
	static uint32_t count = 0;

	if (!update_in_progress)
	{
		update_in_progress = true;
		struct timeval startts;
		struct timeval stopts;

		//if (update_enable) <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		//{
		pthread_mutex_lock(&update_mutex[voice_num]);		// mutex defind at audioManager
		for(stage = 0 ; stage < _MAX_STAGE_NUM ; stage++)
		{
			for (p = audio_first_update[stage]; p; p = p->audio_next_update)
			{
				p->update();
			}
		}

		update_in_progress = false;

		pthread_mutex_unlock(&update_mutex[voice_num]);
		//} if (update_enable)
	}
}

/**
*   @brief  Assign a DSP_Voice object instance
*   @param  dspv	a pointer to a DSP_Voice object instance
*   @return void
*/
void SynthVoice::assign_dsp_voice(DSP_Voice *dspv)
{
	if (dspv)
	{
		dsp_voice = dspv;
		audio_voice->dsp_voice = dspv;
	}
}

/**
*   @brief  Return a pointer to the active setting (patch) parametersce
*   @parm	none
*   @return a pointer to the active setting (patch) parametersce
*/
_setting_params_t *SynthVoice::get_voice_params() { return &active_params;  }

/**
*   @brief  Set the paramters (patch) using the setting parameters
*   @parm	params	 a pointer to a _setting_params_t struct holding the parameters (patch)
*   @return void
*/
void SynthVoice::set_voice_params(_setting_params_t *params)
{
	int i, j = 0, res;
	
	_mod_synth_settings_bool_param_t bool_param;
	_mod_synth_settings_int_param_t int_param;
	
	res = settings_manager->get_bool_param(params, "adjsynth.osc1.enabled", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1_active = bool_param.value;
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.waveform", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_waveform(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.tune_offset_oct", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_freq_detune_oct(int_param.value);
	}
	 
	res = settings_manager->get_int_param(params, "adjsynth.osc1.tune_offset_semitones", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_freq_detune_semitones(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.tune_offset_cents", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_freq_detune_cents_set_value(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.symmetry", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_pwm_dcycle_set_val(int_param.value);
		dsp_voice->osc1->set_pwm_dcycle(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.send_filter_1", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_send_filter1_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.send_filter_2", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_send_filter2_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_detune", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_harmonies_detune(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_distortion", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_harmonies_distortion(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_mode", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_unison_mode(int_param.value);
		// callback to GUI for updating unison mode and labales.
		callback_set_osc1_unison_mode(int_param.value);

	}
	
	res = settings_manager->get_bool_param(params, "adjsynth.osc1.unison_square_wave", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		if (bool_param.value)
		{
			dsp_voice->osc1->enable_unison_square();
		}
		else
		{
			dsp_voice->osc1->disable_unison_square();
		}

	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_1", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_harmony_level(0, int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_2", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_harmony_level(1, int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_3", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_harmony_level(2, int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_4", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_harmony_level(3, int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_5", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_harmony_level(4, int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_6", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_harmony_level(5, int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_7", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_harmony_level(6, int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_8", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_harmony_level(7, int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.osc1.unison_level_9", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc1->set_harmony_level(8, int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.osc1.freq_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_freq_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.freq_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_freq_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.freq_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_freq_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.freq_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_freq_mod_env_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.pwm_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_pwm_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.pwm_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_pwm_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.pwm_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_pwm_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.pwm_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_pwm_mod_env_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.amp_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_amp_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.amp_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_amp_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.amp_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_amp_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc1.amp_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_amp_mod_env_level(int_param.value);
	}
	

	res = settings_manager->get_bool_param(params, "adjsynth.osc2.enabled", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc2_active = bool_param.value;
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.waveform", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc2->set_waveform(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.tune_offset_oct", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc2->set_freq_detune_oct(int_param.value);
	}
	 
	res = settings_manager->get_int_param(params, "adjsynth.osc2.tune_offset_semitones", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc2->set_freq_detune_semitones(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.tune_offset_cents", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc2->set_freq_detune_cents_set_value(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.symmetry", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->osc2->set_pwm_dcycle_set_val(int_param.value);
		dsp_voice->osc2->set_pwm_dcycle(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.send_filter_1", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc2_send_filter1_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.send_filter_2", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc2_send_filter2_level(int_param.value);
	}
	
	res = settings_manager->get_bool_param(params, "adjsynth.osc2.sync_on_osc_1", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		if (int_param.value)
		{
			dsp_voice->set_osc2_sync_on_osc1();
		}
		else
		{
			dsp_voice->set_osc2_not_sync_on_osc1();
		}
	}
	
	//	dsp_voice->osc2->setFixToneState(params->osc2FixToneIsOn);	
	
		res = settings_manager->get_int_param(params, "adjsynth.osc2.freq_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_freq_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.freq_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_freq_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.freq_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_pwm_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.freq_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc2_freq_mod_env_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.pwm_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_pwm_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.pwm_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_freq_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.pwm_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_pwm_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.pwm_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc2_pwm_mod_env_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.amp_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_amp_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.amp_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_amp_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.amp_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_amp_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.osc2.amp_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_osc1_amp_mod_env_level(int_param.value);
	}
	
	
	res = settings_manager->get_bool_param(params, "adjsynth.noise.enabled", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->noise1_active = int_param.value;
	}

	res = settings_manager->get_int_param(params, "adjsynth.noise.color", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->noise1->set_noise_type(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.noise.send_filter_1", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_noise1_send_filter1_level(int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.noise.send_filter_2", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_noise1_send_filter2_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.noise.amp_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_noise_amp_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.noise.amp_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_noise_amp_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.noise.amp_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_noise_amp_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.noise.amp_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_noise_amp_mod_env_level(int_param.value);
	}

	
	res = settings_manager->get_bool_param(params, "adjsynth.karplus_synth.enabled", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->karpuls1_active = int_param.value;
	}

	res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.excitation_waveform_type", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->karplus1->set_excitation_waveform_type(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.excitation_waveform_variations", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->karplus1->set_excitation_waveform_variations(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.string_damping", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->karplus1->set_string_damping(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.string_damping_variations", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->karplus1->set_string_damping_variation(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.string_damping_calculation_mode", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->karplus1->set_string_dumping_calculation_mode(int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.pluck_damping", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->karplus1->set_pluck_damping(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.pluck_damping_variations", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->karplus1->set_pluck_damping_variation(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.on_decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->karplus1->set_on_decay(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.off_decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->karplus1->set_off_decay(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.send_filter_1", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_karplus1_send_filter1_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.karplus_synth.send_filter_2", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_karplus1_send_filter2_level(int_param.value);
	}

	
	res = settings_manager->get_bool_param(params, "adjsynth.mso_synth.enabled", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->mso1_active = int_param.value;
	}

	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.symmetry", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->mso1->set_pwm_dcycle(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.send_filter_1", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_send_filter1_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.send_filter_2", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_send_filter2_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.tune_offset_oct", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->mso1->set_freq_detune_oct(int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.tune_offset_semitones", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->mso1->set_freq_detune_semitones(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.tune_offset_cents", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->mso1->set_freq_detune_cents(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.freq_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_freq_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.freq_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_freq_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.freq_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_freq_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.freq_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_freq_mod_env_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.pwm_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_pwm_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.pwm_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_pwm_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.pwm_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_freq_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.pwm_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_pwm_mod_env_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.amp_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_amp_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.amp_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_amp_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.amp_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_freq_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.amp_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_mso1_amp_mod_env_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.segment_position_a", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->mso1->wtab->set_segment_position(pos_a, int_param.value, &dsp_voice->mso1->wtab->morphed_segment_positions);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.segment_position_b", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->mso1->wtab->set_segment_position(pos_b, int_param.value, &dsp_voice->mso1->wtab->morphed_segment_positions);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.segment_position_c", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->mso1->wtab->set_segment_position(pos_c, int_param.value, &dsp_voice->mso1->wtab->morphed_segment_positions);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.segment_position_d", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->mso1->wtab->set_segment_position(pos_d, int_param.value, &dsp_voice->mso1->wtab->morphed_segment_positions);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.segment_position_e", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->mso1->wtab->set_segment_position(pos_e, int_param.value, &dsp_voice->mso1->wtab->morphed_segment_positions);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.mso_synth.segment_position_f", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->mso1->wtab->set_segment_position(pos_f, int_param.value, &dsp_voice->mso1->wtab->morphed_segment_positions);
	}
	
	
	res = settings_manager->get_bool_param(params, "adjsynth.pad_synth.enabled", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->wavetable1_active = int_param.value;
	}

	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.send_filter_1", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_pad1_send_filter1_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.send_filter_2", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_pad1_send_filter2_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.tune_offset_octave", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->wavetable1->set_freq_detune_oct(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.tune_offset_semitones", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->wavetable1->set_freq_detune_semitones(int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.tune_offset_cents", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->wavetable1->set_freq_detune_cents(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.freq_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_pad1_freq_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.freq_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_pad1_freq_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.freq_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_pad1_freq_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.freq_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_pad1_freq_mod_env_level(int_param.value);
	}

	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.amp_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_pad1_amp_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.amp_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_pad1_amp_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.amp_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_pad1_amp_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.pad_synth.amp_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_pad1_amp_mod_env_level(int_param.value);
	}

	
	res = settings_manager->get_int_param(params, "adjsynth.filter1.frequency", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->filter1->set_frequency(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter1.octave", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->filter1->set_octave(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter1.q", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->filter1->set_resonance(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter1.keyboard_track", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->filter1->set_kbd_track(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter1.band", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->filter1->set_band(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter1.freq_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_amp2_pan_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter1.freq_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_amp2_pan_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter1.freq_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_filter1_freq_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter1.freq_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_filter1_freq_mod_env_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter2.frequency", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->filter2->set_frequency(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter2.octave", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->filter2->set_octave(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter2.q", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->filter2->set_resonance(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter2.keyboard_track", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->filter2->set_kbd_track(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter2.band", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->filter2->set_band(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter2.freq_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_filter1_freq_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter2.freq_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_filter1_freq_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter2.freq_modulation_env_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_filter1_freq_mod_env(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.filter2.freq_modulation_env_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_filter1_freq_mod_env_level(int_param.value);
	}
	
	
	res = settings_manager->get_int_param(params, "adjsynth.distortion_1.blend", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->distortion1->set_blend((float)int_param.value / 100.f);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.distortion_1.drive", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->distortion1->set_drive((float)int_param.value / 100.f);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.distortion_1.range", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->distortion1->set_range((float)int_param.value / 100.f);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.distortion_2.blend", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->distortion2->set_blend((float)int_param.value / 100.f);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.distortion_2.drive", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->distortion2->set_drive((float)int_param.value / 100.f);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.distortion_2.range", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->distortion2->set_range((float)int_param.value / 100.f);
	}

	res = settings_manager->get_bool_param(params, "adjsynth.distortion.enabled", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->distortion1_active = bool_param.value;
		dsp_voice->distortion2_active = bool_param.value;	
	}
	
	res = settings_manager->get_bool_param(params, "adjsynth.distortion.auto_gain_enabled", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		if (bool_param.value)
		{
			dsp_voice->distortion1->enable_auto_gain();
			dsp_voice->distortion2->enable_auto_gain();
		}
		else
		{
			dsp_voice->distortion1->disable_auto_gain();
			dsp_voice->distortion2->disable_auto_gain();
		}
	}
	
	// Changed to AudioMixer for programs support?
	if(dsp_voice->out_amp->levels_are_fixed() == false)
	{
		res = settings_manager->get_int_param(params, "adjsynth.amp_ch1.level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->out_amp->set_ch1_gain(int_param.value);
		}
		
		res = settings_manager->get_int_param(params, "adjsynth.amp_ch1.pan", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->out_amp->set_ch1_pan(int_param.value);
		}
		
		res = settings_manager->get_int_param(params, "adjsynth.amp_ch2.level", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->out_amp->set_ch2_gain(int_param.value);
		}
		
		res = settings_manager->get_int_param(params, "adjsynth.amp_ch2.pan", &int_param);
		if (res == _SETTINGS_KEY_FOUND)
		{
			dsp_voice->out_amp->set_ch2_pan(int_param.value);
		}
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.amp_ch1.pan_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_amp1_pan_mod_lfo(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.amp_ch1.pan_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_amp1_pan_mod_lfo_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.amp_ch2.pan_modulation_lfo_num", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_amp2_pan_mod_lfo(int_param.value);		
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.amp_ch2.pan_modulation_lfo_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_amp2_pan_mod_lfo_level(int_param.value);
	}
	
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_1.waveform", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->lfo1->set_waveform(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_1.rate", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_lfo1_frequency(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_1.symmetry", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->lfo1->set_pwm_dcycle(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_2.waveform", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->lfo2->set_waveform(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_2.rate", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_lfo2_frequency(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_2.symmetry", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->lfo2->set_pwm_dcycle(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_3.waveform", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->lfo3->set_waveform(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_3.rate", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_lfo3_frequency(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_3.symmetry", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->lfo3->set_pwm_dcycle(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_4.waveform", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->lfo4->set_waveform(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_4.rate", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_lfo4_frequency(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_4.symmetry", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->lfo4->set_pwm_dcycle(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_5.waveform", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->lfo5->set_waveform(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_5.rate", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->set_lfo5_frequency(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.lfo_5.symmetry", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->lfo5->set_pwm_dcycle(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_1.attack", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr1->set_attack_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_1.decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr1->set_decay_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_1.sustain", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr1->set_sustain_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_1.release", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr1->set_release_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_2.attack", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr2->set_attack_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_2.decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr2->set_decay_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_2.sustain", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr2->set_sustain_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_2.release", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr2->set_release_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_3.attack", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr3->set_attack_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_3.decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr3->set_decay_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_3.sustain", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr3->set_sustain_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_3.release", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr3->set_release_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_4.attack", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr4->set_attack_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_4.decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr4->set_decay_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_4.sustain", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr4->set_sustain_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_4.release", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr4->set_release_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_5.attack", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr5->set_attack_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_5.decay", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr5->set_decay_time_sec(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_5.sustain", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr5->set_sustain_level(int_param.value);
	}
	
	res = settings_manager->get_int_param(params, "adjsynth.env_5.release", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		dsp_voice->adsr5->set_release_time_sec(int_param.value);
	}	
}


