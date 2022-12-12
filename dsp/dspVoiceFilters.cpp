/**
*	@file		dspVoiceFilter.cpp
*	@author		Nahum Budin
*	@date		28_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Filters Handling.
*				
*/

#include "dspVoice.h"

/**
*	@brief	Set Filter1 active LFO frequency modulator and mode (delayed or not)
*	@param	lfo Filter1 active LFO frequency modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_Voice::set_filter1_freq_mod_lfo(int lfo) 
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		filter1_freq_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		filter1_freq_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set Filter1 active LFO frequency modulator level
*	@param	lev Filter1 active LFO frequency modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_filter1_freq_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		filter1_freq_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Set Filter11 active ENV frequency modulator
*	@param	env Filter11 active ENV frequency modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_Voice::set_filter1_freq_mod_env(int env) 
{ 
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		filter1_freq_mod_env = env;
	}
}

/**
*	@brief	Set Filter1 active ENV frequency modulator level
*	@param	lev Filter1 active ENV frequency modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_filter1_freq_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100)) 
	{ 
		filter1_freq_mod_env_level = (float)lev / 100.0; 
	}
}

/**
*	@brief	Return Filter1 active LFO frequency modulator number
*	@param	none
*	@return Filter1 active LFO frequency modulator number
*/
int DSP_Voice::get_filter1_freq_mod_lfo() 
{ 
	return filter1_freq_mod_lfo; 
}

/**
*	@brief	Return Filter1 active LFO frequency modulator level
*	@param	none
*	@return Filter1 active LFO frequency modulator level
*/
float DSP_Voice::get_filter1_freq_mod_lfo_level() 
{ 
	return filter1_freq_mod_lfo_level; 
}

/**
*	@brief	Return Filter1 active ENV frequency modulator number
*	@param	none
*	@return Filter1 active ENV frequency modulator number
*/
int DSP_Voice::get_filter1_freq_mod_env() 
{ 
	return filter1_freq_mod_env; 
}

/**
*	@brief	Return Filter1 active ENV frequency modulator level
*	@param	none
*	@return Filter1 active ENV frequency modulator level
*/
float DSP_Voice::get_filter1_freq_mod_env_level() 
{ 
	return filter1_freq_mod_env_level; 
}

/**
*	@brief	Set Filter1 active LFO frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_Voice::set_filter1_freq_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr1->note_on_elapsed_time < filter1_freq_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}
	
	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}
	
	if (value < -1.0f)
	{
		value = -1.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}
	
	filter1_freq_lfo_modulation = factor * value / 0.8f; 
}

/**
*	@brief	Set Filter1 active ENV frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_Voice::set_filter1_freq_env_modulation(float mod_factor, float mod_val)
{
	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}
	
	float value = mod_val;
	if (value < 0.0f)
	{
		value = 0.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}
	
	filter1_freq_env_modulation = factor * value; 
}

/**
*	@brief	Set Filter2 active LFO frequency modulator and mode (delayed or not)
*	@param	lfo Filter2 active LFO frequency modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_Voice::set_filter2_freq_mod_lfo(int lfo) {
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		filter2_freq_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		filter2_freq_lfo_modulation = lfo_delays[lfo];
	}
}

/**
*	@brief	Set Filter2 active LFO frequency modulator level
*	@param	lev Filter2 active LFO frequency modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_filter2_freq_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		filter2_freq_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Set Filter2 active ENV frequency modulator
*	@param	env Filter2 active ENV frequency modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_Voice::set_filter2_freq_mod_env(int env) 
{ 
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		filter2_freq_mod_env = env;
	}
}

/**
*	@brief	Set Filter2 active ENV frequency modulator level
*	@param	lev Filter2 active ENV frequency modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_filter2_freq_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100)) 
	{
		filter2_freq_mod_env_level = (float)lev / 100.0; 
	} 
}

/**
*	@brief	Return Filter2 active LFO frequency modulator number
*	@param	none
*	@return Filter1 active LFO frequency modulator number
*/
int DSP_Voice::get_filter2_freq_mod_lfo() 
{ 
	return filter2_freq_mod_lfo;
}

/**
*	@brief	Return Filter2 active LFO frequency modulator level
*	@param	none
*	@return Filter2 active LFO frequency modulator level
*/
float DSP_Voice::get_filter2_freq_mod_lfo_level() 
{ 
	return filter2_freq_mod_lfo_level; 
}

/**
*	@brief	Return Filter2 active ENV frequency modulator number
*	@param	none
*	@return Filter2 active ENV frequency modulator number
*/
int DSP_Voice::get_filter2_freq_mod_env() 
{ 
	return filter2_freq_mod_env; 
}

/**
*	@brief	Return Filter2 active ENV frequency modulator level
*	@param	none
*	@return Filter2 active ENV frequency modulator level
*/
float DSP_Voice::get_filter2_freq_mod_env_level() 
{ 
	return filter2_freq_mod_env_level; 
}

/**
*	@brief	Set Filter2 active LFO frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_Voice::set_filter2_freq_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr1->note_on_elapsed_time < filter2_freq_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}
	
	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}
	
	if (value < -1.0f)
	{
		value = -1.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}
	
	filter2_freq_lfo_modulation = factor * value / 0.8f; 
}

/**
*	@brief	Set Filter2 active ENV frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_Voice::set_filter2_freq_env_modulation(float mod_factor, float mod_val)
{
	float factor = mod_factor;
	if (factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}
	
	float value = mod_val;
	if (value < 0.0f)
	{
		value = 0.0f;
	}
	else if (value > 1.0f)
	{
		value = 1.0f;
	}
	
	filter2_freq_env_modulation = factor * value; 
}

