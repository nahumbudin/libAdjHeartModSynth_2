/**
*	@file		dspOsc.h
*	@author		Nahum Budin
*	@date		25_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate settings
*					
*	@History	31-Oct-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018)
*
*	@brief		Generates random samples -0.8 to +0.8.
*/


#ifndef _DSP_OSC
#define _DSP_OSC

#include "../libAdjHeartModSynth_2.h"

#include "dspSineWaveGenerator.h"
#include "dspSquareWaveGenerator.h"
#include "dspTriangleWaveGenerator.h"
#include "dspSampleHoldWaveGenerator.h"

class DSP_Osc
{
public:
	DSP_Osc(int iD,
		int form,
		int pwm_dc,
		int det_oct,
		int det_s_tone,
		int det_cnts, 
		bool snc,
		bool trk,
		bool fix,
		float harm_detune,
		float harm_dist,
		int unison_mod,
		float mag = 1.0f,
		int samp_rate = _DEFAULT_SAMPLE_RATE,
		int block_size = _DEFAULT_BLOCK_SIZE);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();
	
	int init(int samp_rate = _SAMPLE_RATE_44, int block_size = _AUDIO_BLOCK_SIZE_512);
	
	int get_id();
	
	int set_waveform(int wform);
	int getWaveform();
	
	int set_pwm_dcycle_set_val(int dc);
	int get_pwm_dcycle_set_val();
	
	int set_pwm_dcycle(int dc);
	int get_pwm_dcycle();
	
	int set_freq_detune_oct(int oct);
	int get_freq_detune_oct();
	
	int set_freq_detune_semitones(int sem);
	int get_freq_detune_semitones();
	
	int set_freq_detune_cents_set_value(int cnt);
	int get_freq_detune_cents_set_value();
	
	float set_freq_detune_cents(float cnt);
	float get_freq_detune_cents();
	void set_sync_state(bool snc);
	bool get_sync_state();
	
	void set_track_state(bool trk);
	bool get_track_state();
	
	float get_next_output_val(float freq);
	
	float set_harmonies_detune(float det);
	float set_harmonies_detune(int det);
	float get_harmonies_detune();
	
	float set_harmonies_distortion(float dis);
	float set_harmonies_distortion(int dis);
	float get_harmonies_distortion();
	
	int set_unison_mode(int mod);
	int get_unison_mode();
	
	float set_harmony_level(int harm, int lev);
	int get_harmony_level(int harm);
	
	void set_fix_tone_state(bool fix);
	bool get_fix_tone_state();
	
	float set_magnitude(float mag);
	float get_magnitde();
	
	void sync();	
	bool getCycle_restarted_sync_state();

	void enable_unison_square();
	void disable_unison_square();
	bool get_unison_square_state();

	float set_send_level_1(float snd);
	int set_send_level_1(int snd);
	float set_send_level_2(float snd);
	int set_send_level_2(int snd);
	float get_send_level_1();
	float get_send_level_2();
	
private:
	// OSC1, OSC2, LFO1, LFO2......
	int id;
	int waveform;
	// PWM [%]
	int pwm_percents_set;
	int pwm_percents;
	int detune_octave;
	int detune_semitones;
	float detune_cents;
	int detune_cents_set_value;   // -15.0, -14.5 ... 0 ... +14.5, +15.0 -> 0..60 values
	bool sync_is_on;
	bool track_is_on;
	bool fix_tone_is_on;
	float harmonies_dist_factor;
	float harmonies_detune_factor;
	int unison_mode;
	float magnitude;
	
	int sample_rate;
	int audio_block_size;

	// Output send level
	float send_level_1;
	float send_level_2;
	// Generators
	DSP_SineWaveGenerator *sine_wave = NULL;
	DSP_TriangleWaveGenerator *triangle_wave = NULL;
	DSP_SquareWaveGenerator *square_wave = NULL;
	DSP_SampleHoldWaveGenerator *sample_hold_wave = NULL;
};

#endif

