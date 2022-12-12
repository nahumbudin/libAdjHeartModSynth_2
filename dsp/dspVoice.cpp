/**
*	@file		dspReverbModel.cpp
*	@author		Nahum Budin
*	@date		25_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate and bloc-size settings
*					3. Adding wait for not active state (wait until voice energy goes low and then go to not active)
*					
*	@History	9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - general.
*				
*/

#include "dspVoice.h"
#include "../commonDefs.h"
#include "../synthesizer/adjSynth.h"

/**
*   @brief  Initializes a DSP_Voice object instance.
*   @param  voice								polyphonic voice number 
*   @param	samp_rate							audio sample-rate
*   @param	bloc_size							audio block-size
*	@param	mso_wtab							a pointer to a DSP_MorphingSinusOscLUT MSO LUT object
*	@param	synth_pad_							wavetable	a pointer to a PAD wavetable.
*	@param	voice_end_event_callback_pointer	a pointer to a void foo(int voice_num) callback function
*			that if wait for not active state
*			detection is activated, will be 
*			called when voice output energy 
*			goes to zero.
*   @return none
*/
DSP_Voice::DSP_Voice(
	int voice,
	int samp_rate,
	int block_size,	
	DSP_MorphingSinusOscWTAB *mso_wtab, 
	Wavetable *synth_pad_wavetable,
	func_ptr_void_int_t voice_end_event_callback_pointer) : 
	
	voice(voice),
	voice_active(false),
	voice_waits_for_not_active(false),
	frequency(440.0f), 	// MIDI A6
	osc1_out(0),
	osc2_out(0),
	noise1_out(0),
	karpuls1_out(0),
	mso1_out(0),
	osc1_freq_mod_lfo(_LFO_NONE),
	osc1_pwm_mod_lfo(_LFO_NONE),
	osc1_amp_mod_lfo(_LFO_NONE),
	osc1_freq_mod_lfo_level(0),
	osc1_pwm_mod_lfo_level(0),
	osc1_amp_mod_lfo_level(0),
	osc1_freq_mod_env(_ENV_NONE),
	osc1_pwm_mod_env(_ENV_NONE),
	osc1_amp_mod_env(_ENV_NONE),
	osc1_freq_mod_env_level(0),
	osc1_pwm_mod_env_level(0),
	osc1_amp_mod_env_level(0),
	osc2_freq_mod_lfo(_LFO_NONE),
	osc2_pwm_mod_lfo(_LFO_NONE),
	osc2_amp_mod_lfo(_LFO_NONE),
	osc2_freq_mod_lfo_level(0),
	osc2_pwm_mod_lfo_level(0),
	osc2_amp_mod_lfo_level(0),
	osc2_freq_mod_env(_ENV_NONE),
	osc2_pwm_mod_env(_ENV_NONE),
	osc2_amp_mod_env(_ENV_NONE),
	osc2_freq_mod_env_level(0),
	osc2_pwm_mod_env_level(0),
	osc2_amp_mod_env_level(0),
	mso1_freq_mod_lfo(_LFO_NONE),
	mso1_pwm_mod_lfo(_LFO_NONE),
	mso1_amp_mod_lfo(_LFO_NONE),
	mso1_freq_mod_lfo_level(0),
	mso1_pwm_mod_lfo_level(0),
	mso1_amp_mod_lfo_level(0),
	mso1_freq_mod_env(_ENV_NONE),
	mso1_pwm_mod_env(_ENV_NONE),
	mso1_amp_mod_env(_ENV_NONE),
	mso1_freq_mod_env_level(0),
	mso1_pwm_mod_env_level(0),
	mso1_amp_mod_env_level(0),
	noise_amp_mod_lfo(_LFO_NONE),
	noise_amp_mod_lfo_level(0),
	noise_amp_mod_env(_ENV_NONE),
	noise_amp_mod_env_level(0),
	filter1_freq_mod_lfo(_LFO_NONE),
	filter1_freq_mod_lfo_level(0),
	filter1_freq_mod_env(_ENV_NONE),
	filter1_freq_mod_env_level(0),
	filter2_freq_mod_lfo(_LFO_NONE),
	filter2_freq_mod_lfo_level(0),
	filter2_freq_mod_env(_ENV_NONE),
	filter2_freq_mod_env_level(0),
	amp1_pan_mod_lfo(_LFO_NONE),
	amp2_pan_mod_lfo(_LFO_NONE),
	amp1_pan_mod_lfo_level(0),
	amp2_pan_mod_lfo_level(0),
	lfo1_actual_freq(_CONTROL_SUB_SAMPLING),
	lfo2_actual_freq(_CONTROL_SUB_SAMPLING),
	lfo3_actual_freq(_CONTROL_SUB_SAMPLING),
	lfo4_actual_freq(_CONTROL_SUB_SAMPLING),
	lfo5_actual_freq(_CONTROL_SUB_SAMPLING),
	osc1_send_filter1_level(0),
	osc1_send_filter2_level(0),
	osc1_amp_lfo_modulation(1.0f),
	osc1_amp_env_modulation(1.0f),
	osc1_pwm_lfo_modulation(1.0f),
	osc1_pwm_env_modulation(1.0f),
	osc1_freq_lfo_modulation(0.0f),
	osc1_freq_env_modulation(0.0f),
	osc1_active(false),
	osc2_send_filter1_level(0),
	osc2_send_filter2_level(0),
	osc2_amp_lfo_modulation(1.0f),
	osc2_amp_env_modulation(1.0f),
	osc2_pwm_lfo_modulation(1.0f),
	osc2_pwm_env_modulation(1.0f),
	osc2_freq_lfo_modulation(0.0f),
	osc2_freq_env_modulation(0.0f),
	osc2_active(false),
	osc2_sync_on_osc1(false),
	mso1_send_filter1_level(0),
	mso1_send_filter2_level(0),
	mso1_amp_lfo_modulation(1.0f),
	mso1_amp_env_modulation(1.0f),
	mso1_pwm_lfo_modulation(1.0f),
	mso1_pwm_env_modulation(1.0f),
	mso1_freq_lfo_modulation(0.0f),
	mso1_freq_env_modulation(0.0f),
	wavetable1_send_filter1_level(0),
	wavetable1_send_filter2_level(0),
	wavetable1_amp_lfo_modulation(1.0f),
	wavetable1_amp_env_modulation(1.0f),
	wavetable1_freq_lfo_modulation(0.0f),
	wavetable1_freq_env_modulation(0.0f),
	noise1_send_filter1_level(0),
	noise1_send_filter2_level(0),
	noise1_amp_lfo_modulation(1.0f),
	noise1_amp_env_modulation(1.0),
	noise1_active(false),
	karpuls1_send_filter1_level(0),
	karpuls1_send_filter2_level(0),
	karpuls1_active(false),
//	drawbars_1_active(false),
	wavetable1_active(false),
	filter1_freq_lfo_modulation(0.0f),
	filter1_freq_env_modulation(0.0f),
	filter2_freq_lfo_modulation(0.0f),
	filter2_freq_env_modulation(0.0f),
	freq_mod1(0.0f),
	freq_mod2(0.0f),
	freq_mod_mso1(0.0f),
	freq_mod_pad1(0.0f),
	osc_detune1(1.0f),
	osc_detune2(1.0f),
	mso1_detune(1.0f),
	pad1_detune(1.0f),
	act_freq1(frequency),
	act_freq2(frequency),
	act_freq_mso1(frequency),
	act_freq_pad1(frequency),
	pwm_mod1(0.0f),
	pwm_mod2(0.0f),
	pwm_mod_mso1(0.0f),
	mag_modulation1(1.0f),
	mag_modulation2(1.0f),
	mag_modulation_mso1(1.0f),
	mag_modulation_pad1(1.0f),
	filter_freq_mod1(0.0f),
	filter_freq_mod2(0.0f),
	distortion1_active(false),
	distortion2_active(false),
	osc1_freq_mod_lfo_delay(0),
	osc1_pwm_mod_lfo_delay(0),
	osc1_amp_mod_lfo_delay(0),
	osc2_freq_mod_lfo_delay(0),
	osc2_pwm_mod_lfo_delay(0),
	osc2_amp_mod_lfo_delay(0),
	mso1_freq_mod_lfo_delay(0),
	mso1_pwm_mod_lfo_delay(0),
	mso1_amp_mod_lfo_delay(0),
	wavetable1_freq_mod_lfo_delay(0),
	wavetable1_amp_mod_lfo_delay(0),
	noise_amp_mod_lfo_delay(0),
	filter1_freq_mod_lfo_delay(0),
	filter2_freq_mod_lfo_delay(0),
	amp1_pan_mod_lfo_delay(0),
	amp2_pan_mod_lfo_delay(0),
	used(false),
	mso_wtab1(mso_wtab),
	original_mso_wtab1(mso_wtab),
	pad_wavetable(synth_pad_wavetable),
	original_pad_wavetable(synth_pad_wavetable),
	voice_end_event_callback_ptr(voice_end_event_callback_pointer)
{	
	for (int i = 0; i < _NUM_OF_LFOS; i++)
	{
		lfo_out[i] = 0;
	}
	
	for (int i = 0; i < _NUM_OF_ENVS; i++)
	{
		adsr_out[i] = 0;
	}
		
	osc1 = new DSP_Osc(voice + 100,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		15,
		0,
		false,
		false,
		false,
		0,
		0,
		_OSC_UNISON_MODE_12345678);
	
	set_osc1_send_filter1_level(0);
	set_osc1_send_filter2_level(0);
	set_osc1_freq_mod_lfo(_LFO_NONE);
	set_osc1_freq_mod_lfo_level(0);
	set_osc1_freq_mod_env(_ENV_NONE);
	set_osc1_freq_mod_env_level(0);
	set_osc1_pwm_mod_lfo(_LFO_NONE);
	set_osc1_pwm_mod_lfo_level(0);
	set_osc1_pwm_mod_env(_ENV_NONE);
	set_osc1_pwm_mod_env_level(0);
	set_osc1_amp_mod_lfo(_LFO_NONE);
	set_osc1_amp_mod_lfo_level(0);
	set_osc1_amp_mod_env(_ENV_NONE);
	set_osc1_amp_mod_env_level(0);
	
	osc2 = new DSP_Osc(voice + 200,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		15,
		0,
		false,
		false,
		false,
		0,
		0,
		0);
	
	set_osc2_send_filter1_level(0);
	set_osc2_send_filter2_level(0);
	set_osc2_freq_mod_lfo(_LFO_NONE);
	set_osc2_freq_mod_lfo_level(0);
	set_osc2_freq_mod_env(_ENV_NONE);
	set_osc2_freq_mod_env_level(0);
	set_osc2_pwm_mod_lfo(_LFO_NONE);
	set_osc2_pwm_mod_lfo_level(0);
	set_osc2_pwm_mod_env(_ENV_NONE);
	set_osc2_pwm_mod_env_level(0);
	set_osc2_amp_mod_lfo(_LFO_NONE);
	set_osc2_amp_mod_lfo_level(0);
	set_osc2_amp_mod_env(_ENV_NONE);
	set_osc2_amp_mod_env_level(0);

	set_osc2_not_sync_on_osc1();
	
	noise1 = new DSP_Noise(voice + 300);
	noise1->set_amplitude(1.0f);
	noise1->set_noise_type(_WHITE_NOISE);
	set_noise1_send_filter1_level(0);
	set_noise1_send_filter2_level(0);
	set_noise_amp_mod_lfo(_LFO_NONE);
	set_noise_amp_mod_lfo_level(0);
	set_noise_amp_mod_env(_ENV_NONE);
	set_noise_amp_mod_env_level(0);
	
	karplus1 = new DSP_KarplusStrong(voice + 400);
	karplus1->set_excitation_waveform_type(_KARPLUS_STRONG_EXCITATION_WHITE_NOISE);
	karplus1->set_string_dumping_calculation_mode(_KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT);
	karplus1->set_on_decay(80);
	karplus1->set_off_decay(30);
	karplus1->set_string_damping(50);
	karplus1->set_string_damping_variation(50);
	karplus1->set_excitation_waveform_variations(50);
	set_karplus1_send_filter1_level(0);
	set_karplus1_send_filter2_level(0);

	mso1 = new DSP_MorphingSinusOsc(
		mso_wtab1,
		voice + 500,
		0,
		15,
		0);

	wavetable1 = new DSP_Wavetable(voice + 500, pad_wavetable);
	
	filter1 = new DSP_Filter(voice + 1100, _DEFAULT_SAMPLE_RATE);
	filter1->set_frequency((int)((1000.0f - _FILTER_MIN_CENTER_FREQ) / (float)(filter1->get_filter_max_center_frequency() - _FILTER_MIN_CENTER_FREQ) * 100.0f));
	filter1->set_octave(20);
	filter1->set_resonance((int)((1.0f - _FILTER_MIN_Q) / (float)(_FILTER_MAX_Q - _FILTER_MIN_Q) * 100.0f));
	filter1->set_band(_FILTER_BAND_LPF);
	filter1->set_kbd_track(100);
	set_filter1_freq_mod_lfo(_LFO_NONE);
	set_filter1_freq_mod_lfo_level(0);
	set_filter1_freq_mod_env(_ENV_NONE);
	set_filter1_freq_mod_env_level(0);
	
	filter2 = new DSP_Filter(voice + 1200, _DEFAULT_SAMPLE_RATE);
	filter2->set_frequency((int)((1000.0f - _FILTER_MIN_CENTER_FREQ) / (float)(filter2->get_filter_max_center_frequency() - _FILTER_MIN_CENTER_FREQ) * 100.0f)) ;
	filter2->set_octave(20);
	filter2->set_resonance((int)((1.0f - _FILTER_MIN_Q) / (float)(_FILTER_MAX_Q - _FILTER_MIN_Q) * 100.0f));
	filter2->set_band(_FILTER_BAND_LPF);
	filter2->set_kbd_track(100);
	set_filter2_freq_mod_lfo(_LFO_NONE);
	set_filter2_freq_mod_lfo_level(0);
	set_filter2_freq_mod_env(_ENV_NONE);
	set_filter2_freq_mod_env_level(0);

	distortion1 = new DSP_Distortion();
	distortion2 = new DSP_Distortion();

	out_amp = new DSP_Amp();
	out_amp->set_ch1_gain(20);
	out_amp->set_ch2_gain(20);
	out_amp->set_ch1_pan(50);
	out_amp->set_ch2_pan(50);
	set_amp1_pan_mod_lfo(_LFO_NONE);
	set_amp1_pan_mod_lfo_level(0);
	set_amp2_pan_mod_lfo(_LFO_NONE);
	set_amp2_pan_mod_lfo_level(0);

	lfo1 = new DSP_Osc(voice + 2000,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		0,
		0, 
		false,
		false,
		false,
		0.0f,
		0.0f,
		_OSC_UNISON_MODE_12345678);	
	set_lfo1_frequency((float)10);
	
	lfo2 = new DSP_Osc(voice + 2100,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		0,
		0,
		false,
		false,
		false,
		0.0f,
		0.0f,
		_OSC_UNISON_MODE_12345678);
	set_lfo2_frequency((float)10);

	lfo3 = new DSP_Osc(voice + 2200,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		0,
		0,
		false,
		false,
		false,
		0.0f,
		0.0f,
		_OSC_UNISON_MODE_12345678);
	set_lfo3_frequency((float)10);

	lfo4 = new DSP_Osc(voice + 2300,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		0,
		0,
		false,
		false,
		false,
		0.0f,
		0.0f,
		_OSC_UNISON_MODE_12345678);
	set_lfo4_frequency((float)10);

	lfo5 = new DSP_Osc(voice + 2400,
		_OSC_WAVEFORM_SINE,
		50,
		0,
		0,
		0,
		false,
		false,
		false,
		0.0f,
		0.0f,
		_OSC_UNISON_MODE_12345678);
	set_lfo5_frequency((float)10);

	adsr1 = new DSP_Adsr(voice + 3000, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr1->set_attack_time_sec(10);
	adsr1->set_decay_time_sec(10);
	adsr1->set_sustain_level(100);
	adsr1->set_release_time_sec(10);

	adsr2 = new DSP_Adsr(voice + 3100, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr2->set_attack_time_sec(10);
	adsr2->set_decay_time_sec(10);
	adsr2->set_sustain_level(100);
	adsr2->set_release_time_sec(10);

	adsr3 = new DSP_Adsr(voice + 3200, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr3->set_attack_time_sec(10);
	adsr3->set_decay_time_sec(10);
	adsr3->set_sustain_level(100);
	adsr3->set_release_time_sec(10);

	adsr4 = new DSP_Adsr(voice + 3300, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr4->set_attack_time_sec(10);
	adsr4->set_decay_time_sec(10);
	adsr4->set_sustain_level(100);
	adsr4->set_release_time_sec(10);

	adsr5 = new DSP_Adsr(voice + 3400, (float)_CONTROL_SUB_SAMPLING / samp_rate);
	adsr5->set_attack_time_sec(10);
	adsr5->set_decay_time_sec(10);
	adsr5->set_sustain_level(100);
	adsr5->set_release_time_sec(10);
	
	set_sample_rate(samp_rate);
	set_audio_block_size(block_size);
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/	
int DSP_Voice::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}

	if (osc1)
	{
		osc1->set_sample_rate(sample_rate);
	}

	if (osc2)
	{
		osc2->set_sample_rate(sample_rate);
	}

	if (karplus1)
	{
		karplus1->set_sample_rate(sample_rate);
	}

	if (mso1)
	{
		mso1->get_wavetable()->set_sample_rate(sample_rate);
	}

	if (filter1)
	{
		filter1->set_sample_rate(sample_rate);
	}

	if (filter2)
	{
		filter2->set_sample_rate(sample_rate);
	}

	if (lfo1)
	{
		lfo1->set_sample_rate(sample_rate);
	}

	if (lfo2)
	{
		lfo2->set_sample_rate(sample_rate);
	}

	if (lfo3)
	{
		lfo3->set_sample_rate(sample_rate);
	}

	if (lfo4)
	{
		lfo4->set_sample_rate(sample_rate);
	}

	if (lfo5)
	{
		lfo5->set_sample_rate(sample_rate);
	}

	if (adsr1)
	{
		adsr1->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	if (adsr2)
	{
		adsr2->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	if (adsr3)
	{
		adsr3->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	if (adsr4)
	{
		adsr4->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	if (adsr5)
	{
		adsr5->set_update_interval((float)_CONTROL_SUB_SAMPLING / samp_rate);
	}

	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int DSP_Voice::get_sample_rate() { return sample_rate; }

/**
*   @brief  sets the audio block size and the table phase step based on the size  
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int DSP_Voice::set_audio_block_size(int size)
{
	int res = -1;
	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;

		if (osc1)
		{
			osc1->set_audio_block_size(audio_block_size);
		}

		if (osc2)
		{
			osc2->set_audio_block_size(audio_block_size);
		}

		if (lfo1)
		{
			lfo1->set_audio_block_size(audio_block_size);
		}

		if (lfo2)
		{
			lfo2->set_audio_block_size(audio_block_size);
		}

		if (lfo3)
		{
			lfo3->set_audio_block_size(audio_block_size);
		}

		if (lfo4)
		{
			lfo4->set_audio_block_size(audio_block_size);
		}

		if (lfo5)
		{
			lfo5->set_audio_block_size(audio_block_size);
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
int DSP_Voice::get_audio_block_size() {	return audio_block_size; }

/**
*   @brief  sets the wait for not active state to active  
*   @param  none
*   @return void
*/	
void DSP_Voice::set_wait_for_not_active()
{
	voice_waits_for_not_active = true;
}
	
/**
*   @brief  sets the wait for not active state to not-active  
*   @param  none
*   @return void
*/	
void DSP_Voice::reset_wait_for_not_active()
{
	voice_waits_for_not_active = false;
}

/**
*   @brief  retruns wait for not active satate  
*   @param  none
*   @return buffer true if voice wait for not active state is true
*/	
bool DSP_Voice::get_wait_for_not_active_state() { return voice_waits_for_not_active; }

/**
*   @brief  Register a callback function to be called when the  wait for not active 
*			satate changes to not active with the voice number as a parameter. 
*   @param  func_ptr	a pointer to a void foo(int) function
*   @return buffer true if voice wait for not active state is true
*/	
void DSP_Voice::register_voice_end_event_callback(func_ptr_void_int_t func_ptr)
{
	voice_end_event_callback_ptr = func_ptr;
}

/**
*   @brief  initialize lfo delay times (0, 500, 1000, 1500, 2000ms) sub sampling count   
*   @param  none
*   @return 0;
*/	
int DSP_Voice::init_lfo_delays()
{
	int lfo_num, state;
	uint32_t lfo_500_ms_delay_cont = sample_rate / _CONTROL_SUB_SAMPLING / 2;       
	
	lfo_delays[0] = 0;	
	
	for (state = 0; state < _NUM_OF_LFO_DELAY_OPTIONS; state++)
	{
		for (lfo_num = 0; lfo_num < _NUM_OF_LFOS; lfo_num++)
		{
			lfo_delays[state * _NUM_OF_LFO_DELAY_OPTIONS + state] = lfo_500_ms_delay_cont * state;
		}
	}
	
	return 0;
}

/**
*   @brief  Copy the state to a target DSP_Voice object.
*   @param  target	a target DSP_Voice object	
*   @return void
*/
void DSP_Voice::copy_my_state_to(DSP_Voice target)
{
	target.amp1_pan_mod = this->amp1_pan_mod;
	target.amp2_pan_mod = this->amp2_pan_mod;
	target.osc1_freq_mod_lfo_delay = this->osc1_freq_mod_lfo_delay;
	target.osc1_pwm_mod_lfo_delay = this->osc1_pwm_mod_lfo_delay;
	target.osc1_amp_mod_lfo_delay = this->osc1_amp_mod_lfo_delay;
	target.osc2_freq_mod_lfo_delay = this->osc2_freq_mod_lfo_delay;
	target.osc2_pwm_mod_lfo_delay = this->osc2_pwm_mod_lfo_delay;
	target.osc2_amp_mod_lfo_delay = this->osc2_amp_mod_lfo_delay;
	target.noise_amp_mod_lfo_delay = this->noise_amp_mod_lfo_delay;
	target.mso1_freq_mod_lfo_delay = this->mso1_freq_mod_lfo_delay;
	target.mso1_pwm_mod_lfo_delay = this->mso1_pwm_mod_lfo_delay;
	target.mso1_amp_mod_lfo_delay = this->mso1_amp_mod_lfo_delay;
	target.wavetable1_freq_mod_lfo_delay = this->wavetable1_freq_mod_lfo_delay;
	target.wavetable1_amp_mod_lfo_delay = this->wavetable1_amp_mod_lfo_delay;
	target.filter1_freq_mod_lfo_delay = this->filter1_freq_mod_lfo_delay;
	target.filter2_freq_mod_lfo_delay = this->filter2_freq_mod_lfo_delay;
	target.amp1_pan_mod_lfo_delay = this->amp1_pan_mod_lfo_delay;
	target.amp2_pan_mod_lfo_delay = this->amp2_pan_mod_lfo_delay;
	target.used = this->used;
	target.voice = this->voice;
	target.voice_active = this->voice_active;
	target.frequency = this->frequency;
	target.osc1_freq_mod_lfo = this->osc1_freq_mod_lfo;
	target.osc1_pwm_mod_lfo = this->osc1_pwm_mod_lfo;
	target.osc1_amp_mod_lfo = this->osc1_amp_mod_lfo;
	target.osc1_freq_mod_lfo_level = this->osc1_freq_mod_lfo_level;
	target.osc1_pwm_mod_lfo_level = this->osc1_pwm_mod_lfo_level;
	target.osc1_amp_mod_lfo_level = this->osc1_amp_mod_lfo_level;
	target.osc1_freq_mod_env = this->osc1_freq_mod_env;
	target.osc1_pwm_mod_env = this->osc1_pwm_mod_env;
	target.osc1_amp_mod_env = this->osc1_amp_mod_env;
	target.osc1_freq_mod_env_level = this->osc1_freq_mod_env_level;
	target.osc1_pwm_mod_env_level = this->osc1_pwm_mod_env_level;
	target.osc1_amp_mod_env_level = this->osc1_amp_mod_env_level;
	target.osc2_freq_mod_lfo = this->osc2_freq_mod_lfo;
	target.osc2_pwm_mod_lfo = this->osc2_pwm_mod_lfo;
	target.osc2_amp_mod_lfo = this->osc2_amp_mod_lfo;
	target.osc2_freq_mod_lfo_level = this->osc2_freq_mod_lfo_level;
	target.osc2_pwm_mod_lfo_level = this->osc2_pwm_mod_lfo_level;
	target.osc2_amp_mod_lfo_level = this->osc2_amp_mod_lfo_level;
	target.osc2_freq_mod_env = this->osc2_freq_mod_env;
	target.osc2_pwm_mod_env = this->osc2_pwm_mod_env;
	target.osc2_amp_mod_env = this->osc2_amp_mod_env;
	target.osc2_freq_mod_env_level = this->osc2_freq_mod_env_level;
	target.osc2_pwm_mod_env_level = this->osc2_pwm_mod_env_level;
	target.osc2_amp_mod_env_level = this->osc2_amp_mod_env_level;
	target.mso1_freq_mod_lfo = this->mso1_freq_mod_lfo;
	target.mso1_pwm_mod_lfo = this->mso1_pwm_mod_lfo;
	target.mso1_amp_mod_lfo = this->mso1_amp_mod_lfo;
	target.mso1_freq_mod_lfo_level = this->mso1_freq_mod_lfo_level;
	target.mso1_pwm_mod_lfo_level = this->mso1_pwm_mod_lfo_level;
	target.mso1_amp_mod_lfo_level = this->mso1_amp_mod_lfo_level;
	target.mso1_freq_mod_env = this->mso1_freq_mod_env;
	target.mso1_pwm_mod_env = this->mso1_pwm_mod_env;
	target.mso1_amp_mod_env = this->mso1_amp_mod_env;
	target.mso1_freq_mod_env_level = this->mso1_freq_mod_env_level;
	target.mso1_pwm_mod_env_level = this->mso1_pwm_mod_env_level;
	target.mso1_amp_mod_env_level = this->mso1_amp_mod_env_level;
	target.wavetable1_freq_mod_lfo = this->wavetable1_freq_mod_lfo;
	target.wavetable1_amp_mod_lfo = this->wavetable1_amp_mod_lfo;
	target.wavetable1_freq_mod_lfo_level = this->wavetable1_freq_mod_lfo_level;
	target.wavetable1_amp_mod_lfo_level = this->wavetable1_amp_mod_lfo_level;
	target.wavetable1_freq_mod_env = this->wavetable1_freq_mod_env;
	target.wavetable1_amp_mod_env = this->wavetable1_amp_mod_env;
	target.wavetable1_freq_mod_env_level = this->wavetable1_freq_mod_env_level;
	target.wavetable1_amp_mod_env_level = this->wavetable1_amp_mod_env_level;
	target.noise_amp_mod_lfo = this->noise_amp_mod_lfo;
	target.noise_amp_mod_env = this->noise_amp_mod_env;
	target.noise_amp_mod_lfo_level = this->noise_amp_mod_lfo_level;
	target.noise_amp_mod_env_level = this->noise_amp_mod_env_level;
	target.filter1_freq_mod_lfo = this->filter1_freq_mod_lfo;
	target.filter1_freq_mod_env = this->filter1_freq_mod_env;
	target.filter2_freq_mod_lfo = this->filter2_freq_mod_lfo;
	target.filter2_freq_mod_env = this->filter2_freq_mod_env;
	target.filter1_freq_mod_lfo_level = this->filter1_freq_mod_lfo_level;
	target.filter1_freq_mod_env_level = this->filter1_freq_mod_env_level;
	target.filter2_freq_mod_lfo_level = this->filter2_freq_mod_lfo_level;
	target.filter2_freq_mod_env_level = this->filter2_freq_mod_env_level;
	target.amp1_pan_mod_lfo = this->amp1_pan_mod_lfo;
	target.amp2_pan_mod_lfo = this->amp2_pan_mod_lfo;
	target.amp1_pan_mod_lfo_level = this->amp1_pan_mod_lfo_level;
	target.amp2_pan_mod_lfo_level = this->amp2_pan_mod_lfo_level;
	target.lfo1_actual_freq = this->lfo1_actual_freq;
	target.lfo2_actual_freq = this->lfo2_actual_freq;
	target.lfo3_actual_freq = this->lfo3_actual_freq;
	target.lfo4_actual_freq = this->lfo4_actual_freq;
	target.lfo5_actual_freq = this->lfo5_actual_freq;
	target.osc1_send_filter1_level = this->osc1_send_filter1_level;
	target.osc1_send_filter2_level = this->osc1_send_filter2_level;
	target.osc1_amp_lfo_modulation = this->osc1_amp_lfo_modulation;
	target.osc1_amp_env_modulation = this->osc1_amp_env_modulation;
	target.osc1_pwm_lfo_modulation = this->osc1_pwm_lfo_modulation;
	target.osc1_pwm_env_modulation = this->osc1_pwm_env_modulation;
	target.osc1_freq_lfo_modulation = this->osc1_freq_lfo_modulation;
	target.osc1_freq_env_modulation = this->osc1_freq_env_modulation;
	target.osc1_active = this->osc1_active;
	target.osc2_send_filter1_level = this->osc2_send_filter1_level;
	target.osc2_send_filter2_level = this->osc2_send_filter2_level;
	target.osc2_amp_lfo_modulation = this->osc2_amp_lfo_modulation;
	target.osc2_amp_env_modulation = this->osc2_amp_env_modulation;
	target.osc2_pwm_lfo_modulation = this->osc2_pwm_lfo_modulation;
	target.osc2_pwm_env_modulation = this->osc2_pwm_env_modulation;
	target.osc2_freq_lfo_modulation = this->osc2_freq_lfo_modulation;
	target.osc2_freq_env_modulation = this->osc2_freq_env_modulation;
	target.osc2_active = this->osc2_active;
	target.osc2_sync_on_osc1 = this->osc2_sync_on_osc1;
	target.mso1_send_filter1_level = this->mso1_send_filter1_level;
	target.mso1_send_filter2_level = this->mso1_send_filter2_level;
	target.mso1_amp_lfo_modulation = this->mso1_amp_lfo_modulation;
	target.mso1_amp_env_modulation = this->mso1_amp_env_modulation;
	target.mso1_pwm_lfo_modulation = this->mso1_pwm_lfo_modulation;
	target.mso1_pwm_env_modulation = this->mso1_pwm_env_modulation;
	target.mso1_freq_lfo_modulation = this->mso1_freq_lfo_modulation;
	target.mso1_freq_env_modulation = this->mso1_freq_env_modulation;
	target.mso1_active = this->mso1_active;
	target.wavetable1_send_filter1_level = this->wavetable1_send_filter1_level;
	target.wavetable1_send_filter2_level = this->wavetable1_send_filter2_level;
	target.wavetable1_amp_lfo_modulation = this->wavetable1_amp_lfo_modulation;
	target.wavetable1_amp_env_modulation = this->wavetable1_amp_env_modulation;
	target.wavetable1_freq_lfo_modulation = this->wavetable1_freq_lfo_modulation;
	target.wavetable1_freq_env_modulation = this->wavetable1_freq_env_modulation;
	target.wavetable1_active = this->wavetable1_active;
	target.noise1_send_filter1_level = this->noise1_send_filter1_level;
	target.noise1_send_filter2_level = this->noise1_send_filter2_level;
	target.noise1_amp_lfo_modulation = this->noise1_amp_lfo_modulation;
	target.noise1_amp_env_modulation = this->noise1_amp_env_modulation;
	target.noise1_active = this->noise1_active;
	target.karpuls1_send_filter1_level = this->karpuls1_send_filter1_level;
	target.karpuls1_send_filter2_level = this->karpuls1_send_filter2_level;
	target.karpuls1_active = this->karpuls1_active;
//	target.drawbars1active = this->drawbars1active;
	target.filter1_freq_lfo_modulation = this->filter1_freq_lfo_modulation;
	target.filter1_freq_env_modulation = this->filter1_freq_env_modulation;
	target.filter2_freq_lfo_modulation = this->filter2_freq_lfo_modulation;
	target.filter2_freq_env_modulation = this->filter2_freq_env_modulation;
	target.freq_mod1 = this->freq_mod1;
	target.freq_mod2 = this->freq_mod2;
	target.freq_mod_mso1 = this->freq_mod_mso1;
	target.freq_mod_pad1 = this->freq_mod_pad1;
	target.osc_detune1 = this->osc_detune1;
	target.osc_detune2 = this->osc_detune2;
	target.mso1_detune = this->mso1_detune;
	target.pad1_detune = this->pad1_detune;
	target.act_freq1 = this->act_freq1;
	target.act_freq2 = this->act_freq2;
	target.act_freq_mso1 = this->act_freq_mso1;
	target.act_freq_pad1 = this->act_freq_pad1;
	target.pwm_mod1 = this->pwm_mod1;
	target.pwm_mod2 = this->pwm_mod2;
	target.pwm_mod_mso1 = this->pwm_mod_mso1;
	target.mag_modulation1 = this->mag_modulation1;
	target.mag_modulation2 = this->mag_modulation2;
	target.mag_modulation_mso1 = this->mag_modulation_mso1;
	target.mag_modulation_pad1 = this->mag_modulation_pad1;
	target.filter_freq_mod1 = this->filter_freq_mod1;
	target.filter_freq_mod2 = this->filter_freq_mod2;
	target.distortion1_active = this->distortion1_active;
	target.distortion2_active = this->distortion2_active;
}

/**
*   @brief  Set voice status to In-use.
*   @param  none
*   @return void
*/
void DSP_Voice::in_use() { used = true; }

/**
*   @brief  Set voice status to Not-in-use.
*   @param  none
*   @return void
*/
void DSP_Voice::not_in_use() { used = false; }

/**
*   @brief  Return voice In-use status.
*   @param  none
*   @return true if in-use; false otherwise
*/
bool DSP_Voice::is_in_use() { return used; }

/**
*	@brief	Enable Osc1
*	@param none
*	@return void
*/
void DSP_Voice::enable_osc1() { osc1_active = true; }

/**
*	@brief	Enable Osc2
*	@param none
*	@return void
*/
void DSP_Voice::enable_osc2() { osc2_active = true; }

/**
*	@brief	Enable Noise1
*	@param none
*	@return void
*/
void DSP_Voice::enable_noise() { noise1_active = true; }

/**
*	@brief	Enable KPS
*	@param none
*	@return void
*/
void DSP_Voice::enable_karplus() { karpuls1_active = true; }

/**
*	@brief	Enable Drawbars1
*	@param none
*	@return void
*/
//void DSP_Voice::enableDrawbars() { drawbars1active = true; }

/**
*	@brief	Enable Mso1
*	@param none
*	@return void
*/
void DSP_Voice::enable_morphed_sin() { mso1_active = true; }

/**
*	@brief	Enable Pad1
*	@param none
*	@return void
*/
void DSP_Voice::enable_pad_synth() { wavetable1_active = true; }

/**
*	@brief	Enable Distortion
*	@param none
*	@return void
*/
void DSP_Voice::enable_distortion() 
{ 
	distortion1_active = true; 
	distortion2_active = true; 
}

/**
*	@brief	Disble Osc1
*	@param none
*	@return void
*/
void DSP_Voice::disable_osc1() { osc1_active = false; }

/**
*	@brief	Disable Osc2
*	@param none
*	@return void
*/
void DSP_Voice::disable_osc2() { osc2_active = false; }

/**
*	@brief	Disable Noise1
*	@param none
*	@return void
*/
void DSP_Voice::disable_noise() { noise1_active = false; }

/**
*	@brief	Disable Kps1
*	@param none
*	@return void
*/
void DSP_Voice::disable_karplus() { karpuls1_active = false; }

/**
*	@brief	Disable Drawbars1
*	@param none
*	@return void
*/
//void DSP_Voice::disableDrawbars() { drawbars1active = false; }

/**
*	@brief	Disable Mso1
*	@param none
*	@return void
*/
void DSP_Voice::disable_morphed_sin() { mso1_active = false; }

/**
*	@brief	Disable Pad1
*	@param none
*	@return void
*/
void DSP_Voice::disable_pad_synth() { wavetable1_active = false; }

/**
*	@brief	Disable Distortion
*	@param none
*	@return void
*/
void DSP_Voice::disable_distortion() 
{ 
	distortion1_active = false; 
	distortion2_active = false; 
}

/**
*   @brief  Set voice frequency (Hz).
*   @param  freq	voice frequency (Hz) _OSC_MIN_FREQUENCY to _OSC_MAX_FREQUENCY
*   @return void
*/
void DSP_Voice::set_voice_frequency(float frq)
{
	frequency = frq;
	if (frequency < _OSC_MIN_FREQUENCY)
	{
		frequency = _OSC_MIN_FREQUENCY;
	}
	else if (frequency > _OSC_MAX_FREQUENCY)
	{
		frequency = _OSC_MAX_FREQUENCY;
	}

	wavetable1->set_output_frequency(frequency);
}

/**
*   @brief  Return voice frequency (Hz).
*   @param  none
*   @return voice frequency (Hz)
*/
float DSP_Voice::get_voice_frequency() { return frequency; }

/**
*	@brief	Set voice active
*	@param	none
*	@return none
*/
void DSP_Voice::set_voice_active() 
{ 
	voice_active = true; 
}

/**
*	@brief	Set voice inactive
*	@param	none
*	@return none
*/
void DSP_Voice::set_voice_inactive() 
{ 
	voice_active = false; 
}

/**
*	@brief	Return voice active state
*	@param	none
*	@return true if voice is active, false otherwise
*/
bool DSP_Voice::voice_is_active() 
{ 
	return voice_active; 
}

/**
*	@brief	Set voice wait for not active state ON
*	@param	none
*	@return none
*/
void DSP_Voice::set_voice_waits_for_not_active()
{
	voice_waits_for_not_active = true;
}

/**
*	@brief	Set voice wait for not active state OFF
*	@param	none
*	@return none
*/
void DSP_Voice::set_voice_not_waits_for_not_active()
{
	voice_waits_for_not_active = false;
}

bool DSP_Voice::is_voice_waits_for_not_active()
{
	return voice_waits_for_not_active;
}

/**
*	@brief	Calculate and return a frequency detune factor
*	@param	detune_oct	detune Octaves
*	@param	detune_semitones detune semi-tones
*	@param	detune_percents detune percents
*	@return none
*/
float DSP_Voice::detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents) {
		
	float frq_factor;
		
	frq_factor = (float)((pow(2.0, (float)detune_oct + (float)detune_semitones / 12.0)))*(1.0f + detune_percents / 100.0f);
		
	return frq_factor;
}

/**
*	@brief	Calculate and return a frequency detune factor including frequency modulation
*	@param	detune_oct	detune Octaves
*	@param	detune_semitones detune semi-tones
*	@param	detune_percents detune percents
*	@param	modulation	modulation value
*	@return none
*/
float DSP_Voice::detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents, float modulation) {
		
	float frq_factor;
		
	frq_factor = (float)((pow(2.0, (float)detune_oct + modulation / 1.4f + (float)detune_semitones / 12.0)))*(1.0f + detune_percents / 100.0f);
		
	return frq_factor;
}

/**
*	@brief	Calculate next modulation values.
*	@param	none
*	@return void
*/
void DSP_Voice::calc_next_modulation_values()
{
	lfo_out[0] = lfo1->get_next_output_val(lfo1_actual_freq);
	lfo_out[1] = lfo2->get_next_output_val(lfo2_actual_freq);
	lfo_out[2] = lfo3->get_next_output_val(lfo3_actual_freq);
	lfo_out[3] = lfo4->get_next_output_val(lfo4_actual_freq);
	lfo_out[4] = lfo5->get_next_output_val(lfo5_actual_freq);

	//	fprintf(stderr, "note on time : %i %i %i\n", adsr1->getNoteToneElapsedTime(),adsr2->getNoteToneElapsedTime(),adsr3->getNoteToneElapsedTime());

		adsr1->calc_next_envelope_val();
	adsr_out[0] = adsr1->get_output_val();

	adsr2->calc_next_envelope_val();
	adsr_out[1] = adsr2->get_output_val();

	adsr3->calc_next_envelope_val();
	adsr_out[2] = adsr3->get_output_val();

	adsr4->calc_next_envelope_val();
	adsr_out[3] = adsr4->get_output_val();

	adsr5->calc_next_envelope_val();
	adsr_out[4] = adsr5->get_output_val();

	if (osc1_freq_mod_lfo > _LFO_NONE)
	{
		set_osc1_freq_lfo_modulation(osc1_freq_mod_lfo_level, lfo_out[osc1_freq_mod_lfo - 1]);
	}
	else
	{
		osc1_freq_lfo_modulation = 0.f;
	}

	if (osc1_freq_mod_env > _ENV_NONE)
	{
		set_osc1_freq_env_modulation(osc1_freq_mod_env_level, adsr_out[osc1_freq_mod_env - 1]);
	}
	else
	{
		set_osc1_freq_env_modulation(osc1_freq_mod_env_level, 0.0f);
	}

	if (osc1_pwm_mod_lfo > _LFO_NONE)
	{
		set_osc1_pwm_lfo_modulation(osc1_pwm_mod_lfo_level, lfo_out[osc1_pwm_mod_lfo - 1]);
	}
	else
	{
		osc1_pwm_lfo_modulation = 0.f;
	}

	if (osc1_pwm_mod_env > _ENV_NONE)
	{
		set_osc1_pwm_env_modulation(osc1_pwm_mod_env_level, adsr_out[osc1_pwm_mod_env - 1]);
	}
	else
	{
		set_osc1_pwm_env_modulation(osc1_pwm_mod_env_level, 0.0f);
	}

	if (osc1_amp_mod_lfo > _LFO_NONE)
	{
		set_osc1_amp_lfo_modulation(osc1_amp_mod_lfo_level, lfo_out[osc1_amp_mod_lfo - 1]);
	}
	else
	{
		osc1_amp_lfo_modulation = 1.f;
	}

	if (osc1_amp_mod_env > _ENV_NONE)
	{
		set_osc1_amp_env_modulation(osc1_amp_mod_env_level, adsr_out[osc1_amp_mod_env - 1]);
	}
	else
	{
		set_osc1_amp_env_modulation(osc1_amp_mod_env_level, 0.0f);
	}

	if (amp1_pan_mod_lfo > _LFO_NONE)
	{
		set_amp1_pan_lfo_modulation(amp1_pan_mod_lfo_level, lfo_out[amp1_pan_mod_lfo - 1]);
	}
	else
	{
		set_amp1_pan_lfo_modulation(0.0f, 0.0f);
	}
	

	if (osc2_freq_mod_lfo > _LFO_NONE)
	{
		set_osc2_freq_lfo_modulation(osc2_freq_mod_lfo_level, lfo_out[osc2_freq_mod_lfo - 1]);
	}
	else
	{
		osc2_freq_lfo_modulation = 0.f;
	}

	if (osc2_freq_mod_env > _ENV_NONE)
	{
		set_osc2_freq_env_modulation(osc2_freq_mod_env_level, adsr_out[osc2_freq_mod_env - 1]);
	}
	else
	{
		set_osc2_freq_env_modulation(osc2_freq_mod_env_level, 0.0f);
	}

	if (osc2_pwm_mod_lfo > _LFO_NONE)
	{
		set_osc2_pwm_lfo_modulation(osc2_pwm_mod_lfo_level, lfo_out[osc2_pwm_mod_lfo - 1]);
	}
	else
	{
		osc2_pwm_lfo_modulation = 0.f;
	}

	if (osc2_pwm_mod_env > _ENV_NONE)
	{
		set_osc2_pwm_env_modulation(osc2_pwm_mod_env_level, adsr_out[osc2_pwm_mod_env - 1]);
	}
	else
	{
		set_osc2_pwm_env_modulation(osc2_pwm_mod_env_level, 0.0f);
	}

	if (osc2_amp_mod_lfo > _LFO_NONE)
	{
		set_osc2_amp_lfo_modulation(osc2_amp_mod_lfo_level, lfo_out[osc2_amp_mod_lfo - 1]);
	}
	else
	{
		osc2_amp_lfo_modulation = 1.f;
	}

	if (osc2_amp_mod_env > _ENV_NONE)
	{
		set_osc2_amp_env_modulation(osc2_amp_mod_env_level, adsr_out[osc2_amp_mod_env - 1]);
	}
	else
	{
		set_osc2_amp_env_modulation(osc2_amp_mod_env, 0.0f);
	}

	if (amp2_pan_mod_lfo > _LFO_NONE)
	{
		set_amp2_pan_lfo_modulation(amp2_pan_mod_lfo_level, lfo_out[amp2_pan_mod_lfo - 1]);
	}
	else
	{
		set_amp2_pan_lfo_modulation(0.0f, 0.0f);
	}

	
	if (noise_amp_mod_lfo > _LFO_NONE)
	{
		set_noise_amp_lfo_modulation(noise_amp_mod_lfo_level, lfo_out[noise_amp_mod_lfo - 1]);
	}
	else
	{
		noise1_amp_lfo_modulation = 1.f;
	}

	if (noise_amp_mod_env > _ENV_NONE)
	{
		set_noise_amp_env_modulation(noise_amp_mod_env_level, adsr_out[noise_amp_mod_env - 1]);
	}
	else
	{
		set_noise_amp_env_modulation(noise_amp_mod_env_level, 0.0f);
	}

	if (mso1_freq_mod_lfo > _LFO_NONE)
	{
		set_mso1_freq_lfo_modulation(mso1_freq_mod_lfo_level, lfo_out[mso1_freq_mod_lfo - 1]);
	}
	else
	{
		mso1_freq_lfo_modulation = 0.f;
	}

	if (mso1_freq_mod_env > _ENV_NONE)
	{
		set_mso1_freq_env_modulation(mso1_freq_mod_env_level, adsr_out[mso1_freq_mod_env - 1]);
	}
	else
	{
		set_mso1_freq_env_modulation(mso1_freq_mod_env_level, 0.0f);
	}

	if (mso1_pwm_mod_lfo > _LFO_NONE)
	{
		set_mso1_pwm_lfo_modulation(mso1_pwm_mod_lfo_level, lfo_out[mso1_pwm_mod_lfo - 1]);
	}
	else
	{
		mso1_pwm_lfo_modulation = 0.f;
	}

	if (mso1_pwm_mod_env > _ENV_NONE)
	{
		set_mso1_pwm_env_modulation(mso1_pwm_mod_env_level, adsr_out[mso1_pwm_mod_env - 1]);
	}
	else
	{
		set_mso1_pwm_env_modulation(mso1_pwm_mod_env_level, 0.0f);
	}

	if (mso1_amp_mod_lfo > _LFO_NONE)
	{
		set_mso1_amp_lfo_modulation(mso1_amp_mod_lfo_level, lfo_out[mso1_amp_mod_lfo - 1]);
	}
	else
	{
		mso1_amp_lfo_modulation = 1.f;
	}

	if (mso1_amp_mod_env > _ENV_NONE)
	{
		set_mso1_amp_env_modulation(mso1_amp_mod_env_level, adsr_out[mso1_amp_mod_env - 1]);
	}
	else
	{
		set_mso1_amp_env_modulation(mso1_amp_mod_env_level, 0.0f);
	}

	if (wavetable1_freq_mod_lfo > _LFO_NONE)
	{
		set_pad1_freq_lfo_modulation(wavetable1_freq_mod_lfo_level, lfo_out[wavetable1_freq_mod_lfo - 1]);
	}
	else
	{
		wavetable1_freq_lfo_modulation = 0.f;
	}

	if (wavetable1_freq_mod_env > _ENV_NONE)
	{
		set_pad1_freq_env_modulation(wavetable1_freq_mod_env_level, adsr_out[wavetable1_freq_mod_env - 1]);
	}
	else
	{
		set_pad1_freq_env_modulation(wavetable1_freq_mod_env_level, 0.0f);
	}

	if (wavetable1_amp_mod_lfo > _LFO_NONE)
	{
		set_pad1_amp_lfo_modulation(wavetable1_amp_mod_lfo_level, lfo_out[wavetable1_amp_mod_lfo - 1]);
	}
	else
	{
		wavetable1_amp_lfo_modulation = 1.f;
	}

	if (wavetable1_amp_mod_env > _ENV_NONE)
	{
		set_pad1_amp_env_modulation(wavetable1_amp_mod_env_level, adsr_out[wavetable1_amp_mod_env - 1]);
	}
	else
	{
		set_pad1_amp_env_modulation(wavetable1_amp_mod_env_level, 0.0f);
	}

	if (filter1_freq_mod_lfo > _LFO_NONE)
	{
		set_filter1_freq_lfo_modulation(filter1_freq_mod_lfo_level, lfo_out[filter1_freq_mod_lfo - 1]);
	}
	else
	{
		filter1_freq_lfo_modulation = 0.f;
	}

	if (filter1_freq_mod_env > _ENV_NONE)
	{
		set_filter1_freq_env_modulation(filter1_freq_mod_env_level, adsr_out[filter1_freq_mod_env - 1]);
	}
	else
	{
		set_filter1_freq_env_modulation(filter1_freq_mod_env_level, 0.0f);
	}

	if (filter2_freq_mod_lfo > _LFO_NONE)
	{
		set_filter2_freq_lfo_modulation(filter2_freq_mod_lfo_level, lfo_out[filter2_freq_mod_lfo - 1]);
	}
	else
	{
		filter2_freq_lfo_modulation = 0.f;
	}

	if (filter2_freq_mod_env > _ENV_NONE)
	{
		set_filter2_freq_env_modulation(filter2_freq_mod_env_level, adsr_out[filter2_freq_mod_env - 1]);
	}
	else
	{
		set_filter2_freq_env_modulation(filter2_freq_mod_env_level, 0.0f);
	}
}

/**
*	@brief	Update modulation factors
*	@param	voice	voice number to be updated
*	@return void
*/
void DSP_Voice::update_voice_modulation(int voice)
{
	// Frequency modulation
	freq_mod1 = osc1_freq_lfo_modulation + osc1_freq_env_modulation;

	if (freq_mod1 < -1.0f)
	{
		freq_mod1 = -1.0f;
	}
	else if (freq_mod1 > 1.0f)
	{
		freq_mod1 = 1.0f;
	}
	
	freq_mod2 = osc2_freq_lfo_modulation + osc2_freq_env_modulation;
	if (freq_mod2 < -1.0f)
	{
		freq_mod2 = -1.0f;
	}
	else if (freq_mod2 > 1.0f)
	{
		freq_mod2 = 1.0f;
	}

	freq_mod_mso1 = mso1_freq_lfo_modulation + mso1_freq_env_modulation;
	if (freq_mod_mso1 < -1.0f)
	{
		freq_mod_mso1 = -1.0f;
	}
	else if (freq_mod_mso1 > 1.0f)
	{
		freq_mod_mso1 = 1.0f;
	}

	freq_mod_pad1 = wavetable1_freq_lfo_modulation + wavetable1_freq_env_modulation;
	if (freq_mod_pad1 < -1.0f)
	{
		freq_mod_pad1 = -1.0f;
	}
	else if (freq_mod_pad1 > 1.0f)
	{
		freq_mod_pad1 = 1.0f;
	}
	
	osc_detune1 = detune_frequency_factor(
		osc1->get_freq_detune_oct(),
		osc1->get_freq_detune_semitones(),
		osc1->get_freq_detune_cents(),
		freq_mod1);
	
	osc_detune2 = detune_frequency_factor(
		osc2->get_freq_detune_oct(),
		osc2->get_freq_detune_semitones(),
		osc2->get_freq_detune_cents(),
		freq_mod2);

	mso1_detune = detune_frequency_factor(
		mso1->get_freq_detune_oct(),
		mso1->get_freq_detune_semitones(),
		mso1->get_freq_detune_cents(),
		freq_mod_mso1);

	pad1_detune = detune_frequency_factor
		(wavetable1->get_freq_detune_oct(),
		 wavetable1->get_freq_detune_semitones(),
		 wavetable1->get_freq_detune_cents(),
		 freq_mod_pad1);
	
	act_freq1 = frequency * osc_detune1;
	act_freq2 = frequency * osc_detune2;
	act_freq_mso1 = frequency * mso1_detune;
	act_freq_pad1 = frequency * pad1_detune;
	
	// PWM
	if(osc1_pwm_lfo_modulation + osc1_pwm_env_modulation > 0.0)
	{
		pwm_mod1 = osc1->get_pwm_dcycle_set_val() / 100.0f + osc1_pwm_lfo_modulation + osc1_pwm_env_modulation;
		if (pwm_mod1 < -1.0f)
		{
			pwm_mod1 = -1.0f;
		}
		else if (pwm_mod1 > 1.0f)
		{
			pwm_mod1 = 1.0f;
		}
		
		osc1->set_pwm_dcycle(50 + (int)(pwm_mod1 * 45.0f));
	}
	
	if (osc2_pwm_lfo_modulation + osc2_pwm_env_modulation > 0.0)
	{
		pwm_mod2 = osc2->get_pwm_dcycle_set_val() / 100.0f + osc2_pwm_lfo_modulation + osc2_pwm_env_modulation;
		if (pwm_mod2 < -1.0f)
		{
			pwm_mod2 = -1.0f;
		}
		else if (pwm_mod2 > 1.0f)
		{
			pwm_mod2 = 1.0f;
		}
			
		osc2->set_pwm_dcycle(50 + (int)(pwm_mod2 * 45.0f));
	}

	if (mso1_pwm_lfo_modulation + mso1_pwm_env_modulation > 0.0)
	{
		pwm_mod_mso1 = mso1->get_pwm_dcycle_set_val() / 100.0f + mso1_pwm_lfo_modulation + mso1_pwm_env_modulation;
		if (pwm_mod_mso1 < -1.0f)
		{
			pwm_mod_mso1 = -1.0f;
		}
		else if (pwm_mod_mso1 > 1.0f)
		{
			pwm_mod_mso1 = 1.0f;
		}

		osc2->set_pwm_dcycle(50 + (int)(pwm_mod_mso1 * 45.0f));
	}
	
	// Amplitude
	mag_modulation1 = osc1_amp_lfo_modulation * osc1_amp_env_modulation;
	mag_modulation2 = osc2_amp_lfo_modulation * osc2_amp_env_modulation;
	mag_modulation_mso1 = mso1_amp_lfo_modulation * mso1_amp_env_modulation;
	mag_modulation_pad1 = wavetable1_amp_lfo_modulation * wavetable1_amp_env_modulation;
	
	// Look for active or activity
	if (
		(!osc1_active || (osc1_amp_env_modulation < 0.05f) || ((osc1_send_filter1_level < 0.05f) && (osc1_send_filter2_level < 0.05f))) &&
		(!osc2_active || (osc2_amp_env_modulation < 0.05f) || ((osc2_send_filter1_level < 0.05f) && (osc2_send_filter2_level < 0.05f))) &&
		(!mso1_active || (mso1_amp_env_modulation < 0.05f) || ((mso1_send_filter1_level < 0.05f) && (mso1_send_filter2_level < 0.05f))) &&
		(!wavetable1_active || (wavetable1_amp_env_modulation < 0.05f) || ((wavetable1_send_filter1_level < 0.05f) && (wavetable1_send_filter2_level < 0.05f))) &&
		(!noise1_active || (noise1_amp_env_modulation < 0.05f) || ((noise1_send_filter1_level < 0.05f) && (noise1_send_filter2_level < 0.05f))) &&
		(!karpuls1_active || (karplus1->get_energy() < 0.000005) || ((karpuls1_send_filter1_level < 0.05f) && (karpuls1_send_filter2_level < 0.05f))) &&
		voice_waits_for_not_active)
	{
//		if (voice_end_event_callback_ptr)
//		{
//			voice_end_event_callback_ptr(voice);
//		}

		AdjSynth::get_instance()->synth_voice[voice]->audio_voice->set_inactive();
		AdjSynth::get_instance()->synth_voice[voice]->audio_voice->reset_wait_for_not_active();
		AdjSynth::get_instance()->synth_voice[voice]->assign_dsp_voice(AdjSynth::get_instance()->get_original_main_dsp_voices(voice));
		AdjSynth::get_instance()->synth_voice[voice]->mso_wtab = original_mso_wtab1;	
		AdjSynth::get_instance()->synth_voice[voice]->pad_wavetable = original_pad_wavetable;
		AdjSynth::get_instance()->audio_poly_mixer->restore_gain_pan(voice);

	}
	
	// Filter freq
	filter_freq_mod1 = filter1_freq_lfo_modulation + filter1_freq_env_modulation;
	if (filter_freq_mod1 < -1.0f)
	{
		filter_freq_mod1 = -1.0f;
	}
	else if (filter_freq_mod1 > 1.0f)
	{
		filter_freq_mod1 = 1.0f;
	}
	
	filter_freq_mod2 = filter2_freq_lfo_modulation + filter2_freq_env_modulation;
	if (filter_freq_mod2 < -1.0f)
	{
		filter_freq_mod2 = -1.0f;
	}
	else if (filter_freq_mod2 > 1.0f)
	{
		filter_freq_mod2 = 1.0f;
	}
}

/**
*	@brief	Calculate oscilators and generator next output values
*	@param	none
*	@return void
*/
void DSP_Voice::calc_next_oscilators_output_value()
{
	if (osc1_active)
	{
		osc1_out = osc1->get_next_output_val(act_freq1) * mag_modulation1;
	}
	// Sync
	if(osc2_sync_on_osc1)
	{
		if (osc1->getCycle_restarted_sync_state())
		{
			osc2->sync();
		}
	}

	if (osc2_active)
	{
		osc2_out = osc2->get_next_output_val(act_freq2) * mag_modulation2;
	}

	if (noise1_active)
	{
		noise1_out = noise1->get_next_noise_val() * noise1_amp_lfo_modulation * noise1_amp_env_modulation;
	}
	
	if (karpuls1_active)
	{
		karpuls1_out = karplus1->get_next_output_value();
	}

	if (mso1_active)
	{
		mso1_out = mso1->get_next_mso_wtab_val(act_freq_mso1, 0) * mag_modulation_mso1;
	}

	if (wavetable1_active)
	{
		wavetable1->set_output_frequency(act_freq_pad1, false);  // false: do not init pointers
		wavetable1->get_next_wavetable_value(&wavetable1_out1, &wavetable1_out2);
		wavetable1_out1 *= mag_modulation_pad1;
		wavetable1_out2 *= mag_modulation_pad1;
	}
}

/**
*	@brief	Calculate voice channel 1 next output value
*	@param	none
*	@return void
*/
float DSP_Voice::get_next_output_value_ch1()
{		
	float sig1 = osc1_out * osc1_send_filter1_level +
				 osc2_out * osc2_send_filter1_level +
				 noise1_out * noise1_send_filter1_level +
				 karpuls1_out * karpuls1_send_filter1_level +
				 mso1_out * mso1_send_filter1_level +
				 wavetable1_out1 * wavetable1_send_filter1_level;

	if (distortion1_active)
	{
		sig1 = distortion1->get_next_output_val(sig1);
	}
	
	float filter_out = filter1->filter_output(sig1, filter_freq_mod1);
	
	return filter_out;
}

/**
*	@brief	Calculate voice channel 2 next output value
*	@param	none
*	@return void
*/
float DSP_Voice::get_next_output_value_ch2()
{		
	float sig2 = osc1_out * osc1_send_filter2_level +
				 osc2_out * osc2_send_filter2_level +
				 noise1_out * noise1_send_filter2_level +
				 karpuls1_out * karpuls1_send_filter2_level +
				 mso1_out * mso1_send_filter2_level +
				 wavetable1_out2 * wavetable1_send_filter2_level;

	if (distortion2_active)
	{
		sig2 = distortion1->get_next_output_val(sig2);
	}

	float filterOut = filter1->filter_output(sig2, filter_freq_mod2);

	return filterOut;
}



/**
*	@brief	Set on OSC2 sync on OSC1
*	@param	none
*	@return void
*/
void DSP_Voice::set_osc2_sync_on_osc1() 
{ 
	osc2_sync_on_osc1 = true; 
	osc2->set_sync_state(true);
}

/**
*	@brief	Set off OSC2 sync on OSC1
*	@param	none
*	@return void
*/
void DSP_Voice::set_osc2_not_sync_on_osc1() 
{ 
	osc2_sync_on_osc1 = false; 
	osc2->set_sync_state(false);
}

/**
*	@brief	Return OSC2 sync on OSC1 state
*	@param	none
*	@return true if OSC2 sync on OSC1 is on false otherwise
*/
bool DSP_Voice::get_osc2_sync_on_osc1_state() { return osc2_sync_on_osc1; }

/**
*	@brief	Trigger a note-on event on ENV generator
*	@param	adsr a pointer to an ENV (ADSR) generator object
*	@return void
*/
void DSP_Voice::adsr_note_on(DSP_Adsr *adsr)
{
	adsr->set_note_on();

	if (adsr->get_state() == ADSR_STATE_IDLE)
	{
		adsr->reset();
	}
	else if (adsr->get_state() != ADSR_STATE_FORCE_RELEASE)
	{
		adsr->set_state(ADSR_STATE_FORCE_RELEASE);
	}
}

/**
*	@brief	Trigger a note-off event on ENV generator
*	@param	adsr a pointer to an ENV (ADSR) generator object
*	@return void
*/
void DSP_Voice::adsr_note_off(DSP_Adsr *adsr)
{
	adsr->set_note_off();

	if (adsr->get_state() == ADSR_STATE_DECAY)
	{			 
		adsr->set_state(ADSR_STATE_OFF_RELEASE);
	}

	else if ((adsr->get_state() != ADSR_STATE_IDLE) && (adsr->get_state() != ADSR_STATE_FORCE_RELEASE))
	{
		adsr->set_state(ADSR_STATE_RELEASE);
	}
}

