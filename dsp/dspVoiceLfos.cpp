/**
*	@file		dspVoiceLfos.cpp
*	@author		Nahum Budin
*	@date		28_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - LFOs Handling.
*				
*/

#include "dspVoice.h"
#include "../libAdjHeartModSynth_2.h"
#include "../commonDefs.h"

/**
*	@brief	Set LFO1 frequency
*	@param	freq	LFO1 frequncy 0-100 
*		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
*	@return void
*/
void DSP_Voice::set_lfo1_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate	
	lfo1_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo1_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo1_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
*	@brief	Set LFO2 frequency
*	@param	freq	LFO2 frequncy 0-100
*		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
*	@return void
*/
void DSP_Voice::set_lfo2_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));
	
	// Set lfo frequencies due to subsampling rate
	lfo2_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo2_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo2_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
*	@brief	Set LFO3 frequency
*	@param	freq	LFO3 frequncy 0-100
*		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
*	@return void
*/
void DSP_Voice::set_lfo3_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));
	
	// Set lfo frequencies due to subsampling rate
	lfo3_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo3_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo3_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
*	@brief	Set LFO4 frequency
*	@param	freq	LFO4 frequncy 0-100
*		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
*	@return void
*/
void DSP_Voice::set_lfo4_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo4_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo4_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo4_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

/**
*	@brief	Set LFO5 frequency
*	@param	freq	LFO5 frequncy 0-100
*		(converted into a logaritmic scale _MOD_LFO_MIN_FREQ to _MOD_LFO_MAX_FREQ)
*	@return void
*/
void DSP_Voice::set_lfo5_frequency(float freq)
{
	float logf = (Utils::calc_log_scale_100_float(_MOD_LFO_MIN_FREQ, _MOD_LFO_MAX_FREQ, 10.0, freq));

	// Set lfo frequencies due to subsampling rate
	lfo5_actual_freq = logf * _CONTROL_SUB_SAMPLING;
	if (lfo5_actual_freq > (float)_OSC_MAX_FREQUENCY)
	{
		lfo5_actual_freq = (float)_OSC_MAX_FREQUENCY;
	}
}

