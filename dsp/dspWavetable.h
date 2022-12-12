/**
*	@file		dspWavetable.h
*	@author		Nahum Budin
*	@date		23_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	2-Nov-2019	1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		Wavetables handling.
*	
*/

#ifndef _DSP_WAVETABLE
#define _DSP_WAVETABLE

#include <math.h>
#include <stdlib.h>

typedef struct Wavetable 
{
	int    size;
	float  base_freq;
	float *samples;
} Wavetable_t;


class DSP_Wavetable
{
public:
	DSP_Wavetable(int iD = 0, Wavetable_t *table = NULL);
	~DSP_Wavetable();

	void set_output_frequency(float out_freq = 440.f, bool init_pointers = true);

	void get_next_wavetable_value(float *out1, float *out2);

	float *get_wavetable();
	int get_wavetable_size();

	void randomize_play();

	int get_id();

	void set_freq_detune_oct(int oct);
	int get_freq_detune_oct();

	void set_freq_detune_semitones(int sem);
	int get_freq_detune_semitones();

	void set_freq_detune_cents_set_value(int cnt);
	int get_freq_detune_cents_set_value();

	void set_freq_detune_cents(float cnt);
	float get_freq_detune_cents();

	float set_send_level_1(float snd);
	float set_send_level_1(int snd);
	float set_send_level_2(float snd);
	float set_send_level_2(int snd);
	float get_send_level_1();
	float get_send_level_2();

	void set_magnitude(float mag);
	float get_magnitde();

private:

	void init();
	
	int id;

	Wavetable_t* wavetable;
	// Table pointers
	int pos_h1, pos_h2;
	// Residual
	float pos_l;
	// The wavetavle sampled frequency
	float wt_sample_freq;
	// Wavetable actual frequency step
	float wt_step;
	// Integer part of step
	int wt_step_hi;
	// residual part step
	float wt_step_lo;
	// Generated frequency
	float gen_freq;

	int detune_octave;
	int detune_semitones;
	float detune_cents;
	int detune_cents_set_value;   // -15.0, -14.5 ... 0 ... +14.5, +15.0 -> 0..60 values

	float magnitude;

	// Output send level
	float send_level_1;
	float send_level_2;
};

#endif

