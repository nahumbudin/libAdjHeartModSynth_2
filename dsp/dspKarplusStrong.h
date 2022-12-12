/**
*	@file		dspKarplusStrong.h
*	@author		Nahum Budin
*	@date		23_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					2. Add audio sample-rate settings
*					
*	@History	2-Nov-2019	1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		Karpuls Strong String simulation generator.
*	
*	http://amid.fish/javascript-karplus-strong
*/

#ifndef _DSP_KARPLUS
#define _DSP_KARPLUS

#include <stdint.h>

#include "../libAdjHeartModSynth_2.h"
	
#define _KARPLUS_STRONG_STATE_STEADY_OUTPUT				0
#define _KARPLUS_STRONG_STATE_STEADY_BEGIN_NEXT			1
#define _KARPLUS_STRONG_STATE_PLAYING					2

#define _KARPLUS_STRONG_MIN_FREQ						27.5f			// MIDI A0
#define _KARPLUS_STRONG_MAX_FREQ						4186.0f			// MIDI C8

#define _KARPLUS_STRONG_MAX_PLUCK_DAMPING				0.9f
#define _KARPLUS_STRONG_MIN_PLUCK_DAMPING				0.1f

#define _KARPLUS_STRONG_MAX_PLUCK_DAMPING_VARIATION		0.5f
#define _KARPLUS_STRONG_MIN_PLUCK_DAMPING_VARIATION		0.0f

#define _KARPLUS_STRONG_MAX_STRING_DAMPING				0.7f
#define _KARPLUS_STRONG_MIN_STRING_DAMPING				0.1f

#define _KARPLUS_STRONG_MAX_STRING_DAMPING_VARIATION	0.5f
#define _KARPLUS_STRONG_MIN_STRING_DAMPING_VARIATION	0.0f

#define _KARPLUS_STRONG_MAX_CHARACTER_VARIATION			1.0f
#define _KARPLUS_STRONG_MIN_CHARACTER_VARIATION			0.0f

#define _KARPLUS_STRONG_MAX_DECAY						0.999f
#define _KARPLUS_STRONG_MIN_DECAY						0.9f

class DSP_KarplusStrong
{
public:
	DSP_KarplusStrong(int voice, int samp_rate = _DEFAULT_SAMPLE_RATE);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	int init_buffer();	
	
	void set_excitation_waveform_type(int et);
	int get_excitation_waveform_type();
	
	void set_excitation_waveform_variations(int wv);
	int get_excitation_waveform_variations();
	
	void set_pluck_damping(float pd);
	void set_pluck_damping(int pd);
	float get_pluck_damping();
	
	void set_pluck_damping_variation(float pdv);
	void set_pluck_damping_variation(int pdv);
	float get_pluck_damping_variation();
	
	void set_string_damping(float sd);
	void set_string_damping(int sd);
	float get_string_damping();
	
	void set_string_damping_variation(float sdv);
	void set_string_damping_variation(int sdv);
	float get_string_damping_variation();
	
	void set_character_variation(float cv);
	void set_character_variation(int cv);
	float get_character_variation();
	
	void set_on_decay(float dec);
	void set_on_decay(int dec);
	float get_on_decay();

	void set_off_decay(float dec);
	void set_off_decay(int dec);
	float get_off_decay();
	
	void set_string_dumping_calculation_mode(int dcm);
	int get_string_dumping_calculation_mode();
	
	void note_on(int note_num, float velocity);
	void note_off();
	
	float calc_pluck_damping_coefficient();
	float calc_lpf_smoothing_factor(int noteNum);
	
	void update_prev_samp();
	
	void init_excitation_samples();
	
	float get_next_output_value();
	float get_energy();	
	
	
private:
	int voice;
	
	int get_buffer_max_size();
	
	float get_next_white_noise_val();
	float get_next_pink_noise_val();
	float get_next_brown_noise_val();
	
	float get_next_sawtooth_wave_val(int pos);
	float get_next_square_wave_val(int pos);
	float get_next_sine_chirp_val(int pos);
	
	float get_next_decayed_sine_val(int pos);
	
	float get_next_excitation_val(int pos);
	
	void resonate();
	
	float low_pass(float last_output, float current_input, float smoothing_factor);
	float high_pass(float last_output, float last_iInput, float current_input, float smoothing_factor);
	
	void set_amplitude(float amp);
	float get_amplitude();
	
	float prior_samp;
	float on_decay, off_decay;
	float active_decay;
	float freq;
	
	int excitation_waveform_type;
	int excitation_waveform_variations;
	int state;
	int buffer_len;
	int buffer_index;
	float magnitude;
	float character_variation;
	
	float pluck_damping;
	float pluck_damping_variation;
	float pluck_damping_variation_min;
	float pluck_damping_variation_max;
	float pluck_damping_variation_difference;
	float pluck_damping_coefficient;
	
	float string_damping;
	float string_damping_variation;
	int string_dumping_calculation_mode;
	float lpf_smoothing_factor;
	
	float energy;
	
	uint32_t seed;     // must start at 1
	static uint16_t instance_count;
	float *buffer;
	
	int sample_rate;
	int max_buf_size;
	
}
;

#endif

