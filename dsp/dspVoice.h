/**
*	@file		dspReverbModel.h
*	@author		Nahum Budin
*	@date		25_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate and bloc-size settings
*					3. Adding wait for not active state (wait until voice energy goes low and then go to not active)
*						and voice-end callback.
*					
*	@History	9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing.
*				
*/

#ifndef _DSP_VOICE
#define _DSP_VOICE

#include "../libAdjHeartModSynth_2.h"
//#include "../audio/audioBlockFloatMultiCore.h"

#include "dspKarplusStrong.h"
#include "dspMorphedSineOsc.h"
#include "dspWavetable.h"
#include "dspAmp.h"
#include "dspFilter.h"
#include "dspNoise.h"
#include "dspOsc.h"
#include "dspDistortion.h"
#include "dspAdsr.h"

//class DSP_Wavetable;
//class DSP_KarplusStrong;


 class DSP_Voice
{
public:

	friend class SynthVoice;
	
	DSP_Voice(
		int voice,
		int samp_rate,
		int block_size,	
		DSP_MorphingSinusOscWTAB *mso_wtab = NULL,
		Wavetable *synth_pad_wavetable = NULL,
		func_ptr_void_int_t voice_end_event_callback_pointer = NULL);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();
	
	void set_wait_for_not_active();
	void reset_wait_for_not_active();
	bool get_wait_for_not_active_state();
	
	void register_voice_end_event_callback(func_ptr_void_int_t func_ptr);

	void copy_my_state_to(DSP_Voice target);
	
	void in_use();
	void not_in_use();
	bool is_in_use();
	
	void set_voice_frequency(float frq);
	float get_voice_frequency();

	void enable_osc1();
	void enable_osc2();
	void enable_noise();
	void enable_karplus();
//	void enable_drawbars();
	void enable_morphed_sin();
	void enable_pad_synth();
	void enable_distortion();

	void disable_osc1();
	void disable_osc2();
	void disable_noise();
	void disable_karplus();
//	void disable_drawbars();
	void disable_morphed_sin();
	void disable_pad_synth();
	void disable_distortion();
	
	void set_osc1_send_filter1_level(float lev);
	void set_osc1_send_filter1_level(int lev);
	float get_osc1_send_filter1_level();
	
	void set_osc1_send_filter2_level(float lev);
	void set_osc1_send_filter2_level(int lev);
	float get_osc1_send_filter2_level();
	
	
	void set_osc2_send_filter1_level(float lev);
	void set_osc2_send_filter1_level(int lev);
	float get_osc2_send_filter1_level();
	
	void set_osc2_send_filter2_level(float lev);
	void set_osc2_send_filter2_level(int lev);
	float get_osc2_send_filter2_level();
	
	void set_noise1_send_filter1_level(float lev);
	void set_noise1_send_filter1_level(int lev);
	float get_noise1_send_filter1_level();
	
	void set_noise1_send_filter2_level(float lev);
	void set_noise1_send_filter2_level(int lev);
	float get_noise1_send_filter2_level();
	
	void set_karplus1_send_filter1_level(float lev);
	void set_karplus1_send_filter1_level(int lev);
	float get_karplus1_send_filter1_level();
	
	void set_karplus1_send_filter2_level(float lev);
	void set_karplus1_send_filter2_level(int lev);
	float get_karplus1_send_filter2_level();

	void set_mso1_send_filter1_level(float lev);
	void set_mso1_send_filter1_level(int lev);
	float get_mso1_send_filter1_level();

	void set_mso1_send_filter2_level(float lev);
	void set_mso1_send_filter2_level(int lev);
	float get_mso1_send_filter2_level();

	void set_pad1_send_filter1_level(float lev);
	void set_pad1_send_filter1_level(int lev);
	float get_pad1_send_filter1_level();

	void set_pad1_send_filter2_level(float lev);
	void set_pad1_send_filter2_level(int lev);
	float get_pad1_send_filter2_level();
	
	void set_voice_active();
	void set_voice_inactive();
	bool voice_is_active();

	void set_voice_waits_for_not_active();
	void set_voice_not_waits_for_not_active();
	bool is_voice_waits_for_not_active();
	
	void set_osc1_freq_mod_lfo(int lfo);
	void set_osc1_freq_mod_lfo_level(int lev);
	void set_osc1_freq_mod_env(int env);
	void set_osc1_freq_mod_env_level(int lev);

	void set_osc1_pwm_mod_lfo(int lfo);
	void set_osc1_pwm_mod_lfo_level(int lev);
	void set_osc1_pwm_mod_env(int env);
	void set_osc1_pwm_mod_env_level(int lev);

	void set_osc1_amp_mod_lfo(int lfo);
	void set_osc1_amp_mod_lfo_level(int lev);
	void set_osc1_amp_mod_env(int env);
	void set_osc1_amp_mod_env_level(int lev);

	int get_osc1_freq_mod_lfo();
	float get_osc1_freq_mod_lfo_level();
	int get_osc1_freq_mod_env();
	float get_osc1_freq_mod_env_level();

	int get_osc1_pwm_mod_lfo();
	float get_osc1_pwm_mod_lfo_level();
	int get_osc1_pwm_mod_env();
	float get_osc1_pwm_mod_env_level();

	int get_osc1_amp_mod_lfo();
	float get_osc1_amp_mod_lfo_level();
	int get_osc1_amp_mod_env();
	float get_osc1_amp_mod_env_level();
	
	void set_osc1_amp_lfo_modulation(float mod_factor, float mod_val); 
	void set_osc1_amp_env_modulation(float mod_factor, float mod_val); 
	void set_osc1_pwm_lfo_modulation(float mod_factor, float mod_val); 
	void set_osc1_pwm_env_modulation(float mod_factor, float mod_val); 
	void set_osc1_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_osc1_freq_env_modulation(float mod_factor, float mod_val);

	void set_osc2_freq_mod_lfo(int lfo);
	void set_osc2_freq_mod_lfo_level(int lev);
	void set_osc2_freq_mod_env(int env);
	void set_osc2_freq_mod_env_level(int lev);

	void set_osc2_pwm_mod_lfo(int lfo);
	void set_osc2_pwm_mod_lfo_level(int lev);
	void set_osc2_pwm_mod_env(int env);
	void set_osc2_pwm_mod_env_level(int lev);

	void set_osc2_amp_mod_lfo(int lfo);
	void set_osc2_amp_mod_lfo_level(int lev);
	void set_osc2_amp_mod_env(int env);
	void set_osc2_amp_mod_env_level(int lev);

	int get_osc2_freq_mod_lfo();
	float get_osc2_freq_mod_lfo_level();
	int get_osc2_req_mod_env();
	float get_osc2_freq_mod_env_level();

	int get_osc2_pwm_mod_lfo();
	float get_osc2_pwm_mod_lfo_level();
	int get_osc2_pwm_mod_env();
	float get_osc2_pwm_mod_env_level();

	int get_osc2_amp_mod_lfo();
	float get_osc2_amp_mod_lfo_level();
	int get_osc2_amp_mod_env();
	float get_osc2_amp_mod_env_level();
	
	void set_osc2_amp_lfo_modulation(float mod_factor, float mod_val); 
	void set_osc2_amp_env_modulation(float mod_factor, float mod_val); 
	void set_osc2_pwm_lfo_modulation(float mod_factor, float mod_val); 
	void set_osc2_pwm_env_modulation(float mod_factor, float mod_val); 
	void set_osc2_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_osc2_freq_env_modulation(float mod_factor, float mod_val);

	void set_noise_amp_mod_lfo(int lfo);
	void set_noise_amp_mod_lfo_level(int lev);
	void set_noise_amp_mod_env(int env);
	void set_noise_amp_mod_env_level(int lev);

	int get_noise_amp_mod_lfo();
	float get_noise_amp_mod_lfo_level();
	int get_noise_amp_mod_env();
	float get_noise_amp_mod_env_level();
	
	void set_noise_amp_lfo_modulation(float mod_factor, float mod_val);
	void set_noise_amp_env_modulation(float mod_factor, float mod_val);

	void set_mso1_freq_mod_lfo(int lfo);
	void set_mso1_freq_mod_lfo_level(int lev);
	void set_mso1_freq_mod_env(int env);
	void set_mso1_freq_mod_env_level(int lev);

	void set_mso1_pwm_mod_lfo(int lLfo);
	void set_mso1_pwm_mod_lfo_level(int lev);
	void set_mso1_pwm_mod_env(int env);
	void set_mso1_pwm_mod_env_level(int lev);

	void set_mso1_amp_mod_lfo(int lfo);
	void set_mso1_amp_mod_lfo_level(int lev);
	void set_mso1_amp_mod_env(int env);
	void set_mso1_amp_mod_env_level(int lev);

	int get_mso1_freq_mod_lfo();
	float get_mso1_freq_mod_lfo_level();
	int get_mso1_freq_mod_env();
	float get_mso1_freq_mod_env_level();

	int get_mso1_pwm_mod_lfo();
	float get_mso1_pwm_mod_lfo_lLevel();
	int get_mso1_pwm_mod_env();
	float get_mso1_pwm_mod_env_level();

	int get_mso1_amp_mod_lfo();
	float get_mso1_amp_mod_lfo_level();
	int get_mso1_amp_mod_env();
	float get_mso1_amp_mod_env_level();

	void set_mso1_amp_lfo_modulation(float mod_factor, float mod_val);
	void set_mso1_amp_env_modulation(float mod_factor, float mod_val);
	void set_mso1_pwm_lfo_modulation(float mod_factor, float mod_val);
	void set_mso1_pwm_env_modulation(float mod_factor, float mod_val);
	void set_mso1_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_mso1_freq_env_modulation(float mod_factor, float mod_val);

	void set_pad1_freq_mod_lfo(int lfo);
	void set_pad1_freq_mod_lfo_level(int lev);
	void set_pad1_freq_mod_env(int env);
	void set_pad1_freq_mod_env_level(int lev);

	void set_pad1_amp_mod_lfo(int lfo);
	void set_pad1_amp_mod_lfo_level(int lev);
	void set_pad1_amp_mod_env(int env);
	void set_pad1_amp_mod_env_level(int lev);

	int get_pad1_freq_mod_lfo();
	float get_pad1_freq_mod_lfo_lLevel();
	int get_pad1_freq_mod_env();
	float get_pad1_freq_mod_env_level();

	int get_pad1_amp_mod_lfo();
	float get_pad1_amp_mod_lfo_level();
	int get_pad1_amp_mod_env();
	float get_pad1_amp_mod_env_level();

	void set_pad1_amp_lfo_modulation(float mod_factor, float mod_val);
	void set_pad1_amp_env_modulation(float mod_factor, float mod__vval);
	void set_pad1_freq_lfo_modulation(float mod_factor, float mod_vVal);
	void set_pad1_freq_env_modulation(float mod_factor, float mod_vVal);

	void set_filter1_freq_mod_lfo(int lfo);
	void set_filter1_freq_mod_lfo_level(int lev);
	void set_filter1_freq_mod_env(int env);
	void set_filter1_freq_mod_env_level(int lev);

	int get_filter1_freq_mod_lfo();
	float get_filter1_freq_mod_lfo_level();
	int get_filter1_freq_mod_env();
	float get_filter1_freq_mod_env_level();
	
	void set_filter1_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_filter1_freq_env_modulation(float mod_factor, float mod_val);

	void set_filter2_freq_mod_lfo(int lfo);
	void set_filter2_freq_mod_lfo_level(int lev);
	void set_filter2_freq_mod_env(int env);
	void set_filter2_freq_mod_env_level(int Lev);

	int get_filter2_freq_mod_lfo();
	float get_filter2_freq_mod_lfo_level();
	int get_filter2_freq_mod_env();
	float get_filter2_freq_mod_env_level();
	
	void set_filter2_freq_lfo_modulation(float mod_factor, float mod_val);
	void set_filter2_freq_env_modulation(float mod_factor, float mod_val);

	void set_distortion1_drive(float drv);
	void set_distortion1_range(float rng);
	void set_distortion1_blend(float bln);
	void enable_distortion1_auto_gain();
	void disable_distortion1_auto_gain();

	void set_distortion2_drive(float drv);
	void set_distortion2_range(float rng);
	void set_distortion2_blend(float bln);
	void enable_distortion2_auto_gain();
	void disable_distortion2_auto_gain();

	void set_amp1_pan_mod_lfo(int lfo);
	void set_amp1_pan_mod_lfo_level(int lev);
	void set_amp2_pan_mod_lfo(int lfo);
	void set_amp2_pan_mod_lfo_level(int lev);

	int get_amp1_pan_lfo_modulation();
	float get_amp1_pan_lfo_modulationLevel();
	int get_amp2_pan_lfo_modulation();
	float get_amp2_pan_lfo_modulationLevel();

	void set_amp1_pan_lfo_modulation(float mod_factor, float mod_val);
	void set_amp2_pan_lfo_modulation(float mod_factor, float mod_val);
	
	float detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents);
	float detune_frequency_factor(int detune_oct, int detune_semitones, float detune_percents, float modulation);
	
	void set_lfo1_frequency(float freq);
	void set_lfo2_frequency(float freq);
	void set_lfo3_frequency(float freq);
	void set_lfo4_frequency(float freq);
	void set_lfo5_frequency(float freq);

	void calc_next_modulation_values();

	void update_voice_modulation(int voice = 0);
	
	void calc_next_oscilators_output_value();
	float get_next_output_value_ch1();
	float get_next_output_value_ch2();

	void set_osc2_sync_on_osc1();
	void set_osc2_not_sync_on_osc1();
	bool get_osc2_sync_on_osc1_state();

	void adsr_note_on(DSP_Adsr *adsr);
	void adsr_note_off(DSP_Adsr *adsr);
	
	DSP_Osc *osc1 = NULL, *osc2 = NULL;	
	DSP_Noise *noise1 = NULL;
	DSP_KarplusStrong *karplus1 = NULL;
	DSP_MorphingSinusOsc *mso1 = NULL;
	DSP_MorphingSinusOscWTAB *mso_wtab1 = NULL, *original_mso_wtab1 = NULL;
	DSP_Filter *filter1 = NULL, *filter2 = NULL;
	DSP_Distortion *distortion1 = NULL, *distortion2 = NULL;
	DSP_Amp *out_amp = NULL;
	DSP_Wavetable *wavetable1 = NULL;
	Wavetable *pad_wavetable = NULL, *original_pad_wavetable = NULL;

	DSP_Osc *lfo1 = NULL;
	DSP_Osc *lfo2 = NULL;
	DSP_Osc *lfo3 = NULL;
	DSP_Osc *lfo4 = NULL;
	DSP_Osc *lfo5 = NULL;

	DSP_Adsr *adsr1 = NULL;
	DSP_Adsr *adsr2 = NULL;
	DSP_Adsr *adsr3 = NULL;
	DSP_Adsr *adsr4 = NULL;
	DSP_Adsr *adsr5 = NULL;

	float amp1_pan_mod;
	float amp2_pan_mod;
	// Public to enable fast direct access (instead of get() )
	uint32_t osc1_freq_mod_lfo_delay, osc1_pwm_mod_lfo_delay, osc1_amp_mod_lfo_delay;
	uint32_t osc2_freq_mod_lfo_delay, osc2_pwm_mod_lfo_delay, osc2_amp_mod_lfo_delay;
	uint32_t noise_amp_mod_lfo_delay;
	uint32_t mso1_freq_mod_lfo_delay, mso1_pwm_mod_lfo_delay, mso1_amp_mod_lfo_delay;
	uint32_t wavetable1_freq_mod_lfo_delay, wavetable1_amp_mod_lfo_delay;
	uint32_t filter1_freq_mod_lfo_delay, filter2_freq_mod_lfo_delay;
	uint32_t amp1_pan_mod_lfo_delay, amp2_pan_mod_lfo_delay;
		
private:
	int init_lfo_delays();
	
	bool used;
	
	int sample_rate;
	int audio_block_size;
	
	uint32_t lfo_delays[_NUM_OF_LFOS * _NUM_OF_LFO_DELAY_OPTIONS + 1];  // 5 LFOs, 5 states; 1 None
	
	int voice;
	bool voice_active;
	bool voice_waits_for_not_active;
	float frequency = 440.0f;
	
	float osc1_out;
	float osc2_out;
	float noise1_out;
	float karpuls1_out;
	float mso1_out;
	float wavetable1_out1, wavetable1_out2;

	float lfo_out[5], adsr_out[5];	

	int osc1_freq_mod_lfo, osc1_pwm_mod_lfo, osc1_amp_mod_lfo;
	float osc1_freq_mod_lfo_level, osc1_pwm_mod_lfo_level, osc1_amp_mod_lfo_level;
	int osc1_freq_mod_env, osc1_pwm_mod_env, osc1_amp_mod_env;
	float osc1_freq_mod_env_level, osc1_pwm_mod_env_level, osc1_amp_mod_env_level;

	int osc2_freq_mod_lfo, osc2_pwm_mod_lfo, osc2_amp_mod_lfo;
	float osc2_freq_mod_lfo_level, osc2_pwm_mod_lfo_level, osc2_amp_mod_lfo_level;
	int osc2_freq_mod_env, osc2_pwm_mod_env, osc2_amp_mod_env;
	float osc2_freq_mod_env_level, osc2_pwm_mod_env_level, osc2_amp_mod_env_level;

	int mso1_freq_mod_lfo, mso1_pwm_mod_lfo, mso1_amp_mod_lfo;
	float mso1_freq_mod_lfo_level, mso1_pwm_mod_lfo_level, mso1_amp_mod_lfo_level;
	int mso1_freq_mod_env, mso1_pwm_mod_env, mso1_amp_mod_env;
	float mso1_freq_mod_env_level, mso1_pwm_mod_env_level, mso1_amp_mod_env_level;

	int wavetable1_freq_mod_lfo, wavetable1_amp_mod_lfo;
	float wavetable1_freq_mod_lfo_level, wavetable1_amp_mod_lfo_level;
	int wavetable1_freq_mod_env, wavetable1_amp_mod_env;
	float wavetable1_freq_mod_env_level, wavetable1_amp_mod_env_level;

	int noise_amp_mod_lfo, noise_amp_mod_env;
	float noise_amp_mod_lfo_level, noise_amp_mod_env_level;

	int filter1_freq_mod_lfo, filter1_freq_mod_env, filter2_freq_mod_lfo, filter2_freq_mod_env;
	float filter1_freq_mod_lfo_level, filter1_freq_mod_env_level, filter2_freq_mod_lfo_level, filter2_freq_mod_env_level;

	int amp1_pan_mod_lfo, amp2_pan_mod_lfo;
	float amp1_pan_mod_lfo_level, amp2_pan_mod_lfo_level;

	float lfo1_actual_freq, lfo2_actual_freq, lfo3_actual_freq, lfo4_actual_freq, lfo5_actual_freq;
	
	float osc1_send_filter1_level;
	float osc1_send_filter2_level;		
	float osc1_amp_lfo_modulation;
	float osc1_amp_env_modulation;
	float osc1_pwm_lfo_modulation;
	float osc1_pwm_env_modulation;
	float osc1_freq_lfo_modulation;
	float osc1_freq_env_modulation;
	bool osc1_active;
	
	float osc2_send_filter1_level;
	float osc2_send_filter2_level;
	float osc2_amp_lfo_modulation; 
	float osc2_amp_env_modulation; 
	float osc2_pwm_lfo_modulation; 
	float osc2_pwm_env_modulation; 
	float osc2_freq_lfo_modulation;
	float osc2_freq_env_modulation;
	bool osc2_active;
	bool osc2_sync_on_osc1;

	float mso1_send_filter1_level;
	float mso1_send_filter2_level;
	float mso1_amp_lfo_modulation; 
	float mso1_amp_env_modulation; 
	float mso1_pwm_lfo_modulation; 
	float mso1_pwm_env_modulation; 
	float mso1_freq_lfo_modulation;
	float mso1_freq_env_modulation;
	bool mso1_active;

	float wavetable1_send_filter1_level;
	float wavetable1_send_filter2_level;
	float wavetable1_amp_lfo_modulation; 
	float wavetable1_amp_env_modulation; 
	float wavetable1_freq_lfo_modulation;
	float wavetable1_freq_env_modulation;
	bool wavetable1_active;
	
	float noise1_send_filter1_level;
	float noise1_send_filter2_level;
	float noise1_amp_lfo_modulation; 
	float noise1_amp_env_modulation; 
	bool noise1_active;
	
	float karpuls1_send_filter1_level;
	float karpuls1_send_filter2_level;
	bool karpuls1_active;

//	bool drawbars1active;
	
	float filter1_freq_lfo_modulation; 
	float filter1_freq_env_modulation; 
	
	float filter2_freq_lfo_modulation; 
	float filter2_freq_env_modulation; 
	
	float freq_mod1; 
	float freq_mod2; 
	float freq_mod_mso1; 
	float freq_mod_pad1;
	float osc_detune1; 
	float osc_detune2; 
	float mso1_detune; 
	float pad1_detune;
	float act_freq1; 
	float act_freq2; 
	float act_freq_mso1; 
	float act_freq_pad1;
	float pwm_mod1; 
	float pwm_mod2; 
	float pwm_mod_mso1; 
	float mag_modulation1; 
	float mag_modulation2; 
	float mag_modulation_mso1; 
	float mag_modulation_pad1;
	float filter_freq_mod1;  
	float filter_freq_mod2; 

	bool distortion1_active, distortion2_active;

	func_ptr_void_int_t voice_end_event_callback_ptr;

}
;

#endif




