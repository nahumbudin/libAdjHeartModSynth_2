/**
*	@file		dspVoiceMso.cpp
*	@author		Nahum Budin
*	@date		25_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Morphed Sine Generator Handling.
*				
*/

#include "dspVoice.h"

/**
*	@brief	Set Mso1 send level to channel 1
*	@param lev Mso1 send level to channel 1 0.0 to 1.0
*	@return void
*/
void DSP_Voice::set_mso1_send_filter1_level(float lev)
{
	mso1_send_filter1_level = lev;
	
	if (mso1_send_filter1_level < 0.0f)
	{
		mso1_send_filter1_level = 0.0f;
	}
	else if (mso1_send_filter1_level > 1.0f)
	{
		mso1_send_filter1_level = 1.0f;
	}
}

/**
*	@brief	Set Mso1 send level to channel 1 (int)
*	@param lev Mso1 send level to channel 1 0-100
*	@return void
*/
void DSP_Voice::set_mso1_send_filter1_level(int lev)
{
	set_mso1_send_filter1_level((float)lev / 100.0f);
}

/**
*	@brief	return Mso1 send level to channel 1
*	@param none
*	@return Mso1 send level to channel 1
*/
float DSP_Voice::get_mso1_send_filter1_level() { return mso1_send_filter1_level; }


void DSP_Voice::set_mso1_send_filter2_level(float lev)
{
	mso1_send_filter2_level = lev;
	
	if (mso1_send_filter2_level < 0.0f)
	{
		mso1_send_filter2_level = 0.0f;
	}
	else if (mso1_send_filter2_level > 1.0f)
	{
		mso1_send_filter2_level = 1.0f;
	}
}

/**
*	@brief	Set Mso1 send level to channel 2 (int)
*	@param lev Mso1 send level to channel 2 (int) 0 to 100
*	@return void
*/
void DSP_Voice::set_mso1_send_filter2_level(int lev)
{
	set_mso1_send_filter2_level((float)lev / 100.0f);
}

/**
*	@brief	return Mso1 send level to channel 2
*	@param none
*	@return Mso1 send level to channel 2
*/
float DSP_Voice::get_mso1_send_filter2_level() { return mso1_send_filter2_level; }

/**
*	@brief	Set MSO1 active LFO frequency modulator and mode (delayed or not)
*	@param	lfo MSO1 active LFO frequency modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_Voice::set_mso1_freq_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		mso1_freq_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		mso1_freq_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set MSO1 active LFO frequency modulator level
*	@param	lev MSO1 active LFO frequency modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_mso1_freq_mod_lfo_level(int lev)
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		mso1_freq_mod_lfo_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev);  // (float)Lev / 100.0;
	}
}

/**
*	@brief	Set MSO1 active ENV frequency modulator
*	@param	env MSO1 active ENV frequency modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_Voice::set_mso1_freq_mod_env(int env) 
{ 
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		mso1_freq_mod_env = env;
	}
}

/**
*	@brief	Set MSO1 active ENV frequency modulator level
*	@param	lev MSO1 active ENV frequency modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_mso1_freq_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		mso1_freq_mod_env_level = Utils::calc_log_scale_100_float(0, 1.0, 10.0, lev);  // (float)Lev / 100.0; 
	}
}

/**
*	@brief	Set MSO1 active LFO PWM modulator and mode (delayed or not)
*	@param	lfo MSO1 active LFO PWM modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_Voice::set_mso1_pwm_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		mso1_pwm_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		mso1_pwm_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set MSO1 active LFO PWM modulator level
*	@param	lev MSO1 active LFO PWM modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_mso1_pwm_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		mso1_pwm_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Set MSO1 active ENV PWM modulator
*	@param	env MSO1 active ENV PWM modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_Voice::set_mso1_pwm_mod_env(int env) 
{ 
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		mso1_pwm_mod_env = env;
	}
}

/**
*	@brief	Set MSO1 active ENV PWM modulator level
*	@param	lev MSO1 active ENV PWM modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_mso1_pwm_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		mso1_pwm_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Set MSO1 active LFO amplitude modulator and mode (delayed or not)
*	@param	lfo MSO1 active LFO amplitude modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_Voice::set_mso1_amp_mod_lfo(int lfo)
{
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		mso1_amp_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		mso1_amp_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set MSO1 active LFO amplitude modulator level
*	@param	lev MSO1 active LFO amplitude modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_mso1_amp_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		mso1_amp_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Set MSO1 active ENV amplitude modulator
*	@param	env MSO1 active ENV amplitude modulator _ENV_NONE to _ENV_5
*	@return none
*/
void DSP_Voice::set_mso1_amp_mod_env(int env) 
{ 
	if ((env >= _ENV_NONE) && (env <= _ENV_5))
	{
		mso1_amp_mod_env = env;
	}
}

/**
*	@brief	Set MSO1 active ENV amplitude modulator level
*	@param	lev MSO1 active ENV amplitude modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_mso1_amp_mod_env_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		mso1_amp_mod_env_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Return MSO1 active LFO frequency modulator number
*	@param	none
*	@return MSO1 active LFO frequency modulator number
*/
int DSP_Voice::get_mso1_freq_mod_lfo() 
{ 
	return mso1_freq_mod_lfo; 
}

/**
*	@brief	Return MSO1 active LFO frequency modulator level
*	@param	none
*	@return MSO1 active LFO frequency modulator level
*/
float DSP_Voice::get_mso1_freq_mod_lfo_level() 
{ 
	return mso1_freq_mod_lfo_level; 
}

/**
*	@brief	Return MSO1 active ENV frequency modulator number
*	@param	none
*	@return MSO1 active ENV frequency modulator number
*/
int DSP_Voice::get_mso1_freq_mod_env() 
{ 
	return mso1_freq_mod_env; 
}

/**
*	@brief	Return MSO1 active ENV frequency modulator level
*	@param	none
*	@return MSO1 active ENV frequency modulator level
*/
float DSP_Voice::get_mso1_freq_mod_env_level() 
{ 
	return mso1_freq_mod_env_level; 
}

/**
*	@brief	Return MSO1 active LFO pwm modulator number
*	@param	none
*	@return MSO1 active LFO pwm modulator number
*/
int DSP_Voice::get_mso1_pwm_mod_lfo() 
{ 
	return mso1_pwm_mod_lfo; 
}

/**
*	@brief	Return MSO1 active LFO pwm modulator level
*	@param	none
*	@return MSO1 active LFO pwm modulator level
*/
float DSP_Voice::get_mso1_pwm_mod_lfo_lLevel() 
{ 
	return mso1_pwm_mod_lfo_level; 
}

/**
*	@brief	Return MSO1 active ENV pwm modulator number
*	@param	none
*	@return MSO1 active ENV pwm modulator number
*/
int DSP_Voice::get_mso1_pwm_mod_env() 
{ 
	return mso1_pwm_mod_env; 
}

/**
*	@brief	Return MSO1 active ENV pwm modulator level
*	@param	none
*	@return MSO1 active ENV pwm modulator level
*/
float DSP_Voice::get_mso1_pwm_mod_env_level() 
{ 
	return mso1_pwm_mod_env_level; 
}

/**
*	@brief	Return MSO1 active LFO amplitude modulator number
*	@param	none
*	@return MSO1 active LFO amplitude modulator number
*/
int DSP_Voice::get_mso1_amp_mod_lfo() 
{ 
	return mso1_amp_mod_lfo; 
}

/**
*	@brief	Return MSO1 active LFO amplitude modulator level
*	@param	none
*	@return MSO1 active LFO amplitude modulator level
*/
float DSP_Voice::get_mso1_amp_mod_lfo_level() 
{ 
	return mso1_amp_mod_lfo_level; 
}

/**
*	@brief	Return MSO1 active ENV amplitude modulator number
*	@param	none
*	@return MSO active ENV amplitude modulator number
*/
int DSP_Voice::get_mso1_amp_mod_env() 
{ 
	return mso1_amp_mod_env; 
}

/**
*	@brief	Return MSO1 active ENV amplitude modulator level
*	@param	none
*	@return MSO1 active ENV amplitude modulator level
*/
float DSP_Voice::get_mso1_amp_mod_env_level() { return mso1_amp_mod_env_level; }

/**
*	@brief	Set MSO1 active LFO amplitude modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_Voice::set_mso1_amp_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;   			// Modulation value (LFO: -1.0 to 1.0; Env: 0 to 1.0)
	
	if (adsr1->note_on_elapsed_time < mso1_amp_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}

	float factor = mod_factor;  		// 0 to 1.0 : indicates the modulator send set-level
	if(factor < 0.0f)
	{
		factor = 0.0f;
	}
	else if(factor > 1.0f)
	{
		factor = 1.0f;
	}
	
	if(value < -1.0f)
	{
		value = -1.0f;
	}
	else if(value > 1.0f)
	{
		value = 1.0f;
	}

	mso1_amp_lfo_modulation = 1 - (1.0f + value) * factor / 2.0f;
}

/**
*	@brief	Set MSO1 active ENV amplitude modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_Voice::set_mso1_amp_env_modulation(float mod_factor, float mod_val)
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

	mso1_amp_env_modulation = factor * value;
}

/**
*	@brief	Set MSO1 active LFO pwm modulator modulation value
*	@param	modFactor	modulation factor (depth) 0.0 to 1.0
*	@param	modValue	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_Voice::set_mso1_pwm_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr1->note_on_elapsed_time < mso1_pwm_mod_lfo_delay)
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

	mso1_pwm_lfo_modulation = factor * value;
}

/**
*	@brief	Set MSO1 active ENV pwm modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_Voice::set_mso1_pwm_env_modulation(float mod_factor, float mod_val)
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

	mso1_pwm_env_modulation = factor * value;
}

/**
*	@brief	Set MSO1 active LFO frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_Voice::set_mso1_freq_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr1->note_on_elapsed_time < mso1_freq_mod_lfo_delay)
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

	mso1_freq_lfo_modulation = factor * value;
}

/**
*	@brief	Set MSO1 active ENV frequency modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value 0.0 to 1.0
*	@return none
*/
void DSP_Voice::set_mso1_freq_env_modulation(float mod_factor, float mod_val)
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

	mso1_freq_env_modulation = factor * value;
}

