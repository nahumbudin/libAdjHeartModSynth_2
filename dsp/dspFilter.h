/**
*	@file		dspFilter.h
*	@author		Nahum Budin
*	@date		23_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					2. Adding sample-rate settings
*					
*	@History	31-Oct-20199		1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		State variable filter.
*/

#ifndef _DSP_FILTER
#define _DSP_FILTER

#include <math.h>

#define _FILTER_MIN_CENTER_FREQ		20.0f
#define _FILTER_MIN_Q				0.7f
#define _FILTER_MAX_Q				5.0f

class DSP_Filter
{
public:
	DSP_Filter(int iD, int samp_rate);
	
	void clear();
	void set_frequency(float fr);
	void set_frequency(int fr);
	void set_octave(float oct);
	void set_octave(int oct);
	void set_resonance(float res);
	void set_resonance(int res);
	void set_band(int bnd);
	void set_kbd_freq(float frq);
	void set_kbd_freq(int frq);
	void set_kbd_track(float kbdt);
	void set_kbd_track(int kbdt);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	float get_filter_max_center_frequency();
	
	float filter_output(float input, float fmod = 1.0f);
	
	
private:
	int filter_band;
	int filters_balance;
	
	int id;
	float setting_fcenter;
	float setting_fmult;
	float setting_octave_mult;
	float setting_damp;
	float state_input_prev;
	float state_lowpass;
	float state_highpass;
	float state_bandpass;
	float setting_kbd_fcenter;
	float setting_kbd_fmult;
	float kbd_track;
	float lowpass_tmp, bandpass_tmp, highpass_tmp;
	
	int sample_rate;
	
	static float max_setting_fcenter;
	static float max_setting_fmult;
};

#endif

