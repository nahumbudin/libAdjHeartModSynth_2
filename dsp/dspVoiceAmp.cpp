/**
*	@file		dspVoiceAmp.cpp
*	@author		Nahum Budin
*	@date		28_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Amp Handling.
*				
*/

#include "dspVoice.h"

/**
*	@brief	Set Amp1 active LFO pan modulator and mode (delayed or not)
*	@param	lfo Amp1 active LFO pan modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_Voice::set_amp1_pan_mod_lfo(int lfo) {
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		amp1_pan_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		amp1_pan_mod_lfo_delay = lfo_delays[lfo];
	} 
}

/**
*	@brief	Set Amp1 active LFO pan modulator level
*	@param	lev Amp1 active LFO pan modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_amp1_pan_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		amp1_pan_mod_lfo_level = (float)lev / 100.0; 
	}
}

/**
*	@brief	Set Amp2 active LFO pan modulator and mode (delayed or not)
*	@param	lfo Amp2 active LFO pan modulator and mode _LFO_NONE to LFO_5_DELAYED_2000MS
*	@return none
*/
void DSP_Voice::set_amp2_pan_mod_lfo(int lfo) 
{ 
	if ((lfo >= _LFO_NONE) && (lfo <= _LFO_5_DELAYED_2000MS))
	{
		amp2_pan_mod_lfo = ((lfo - 1) % _NUM_OF_LFOS) + 1;
		amp2_pan_mod_lfo_delay = lfo_delays[lfo];
	}
}

/**
*	@brief	Set Amp2 active LFO pan modulator level
*	@param	lev Amp2 active LFO pan modulator level 0 to 100
*	@return none
*/
void DSP_Voice::set_amp2_pan_mod_lfo_level(int lev) 
{ 
	if ((lev >= 0) && (lev <= 100))
	{
		amp2_pan_mod_lfo_level = (float)lev / 100.0;
	}
}

/**
*	@brief	Return Amp1 active LFO pan modulator number
*	@param	none
*	@return Amp1 active LFO pan modulator number
*/
int DSP_Voice::get_amp1_pan_lfo_modulation() 
{ 
	return amp1_pan_mod_lfo; 
}

/**
*	@brief	Return Amp1 active LFO pan modulator level
*	@param	none
*	@return Amp1 active LFO pan modulator level
*/
float DSP_Voice::get_amp1_pan_lfo_modulationLevel() 
{ 
	return amp1_pan_mod_lfo_level; 
}

/**
*	@brief	Return Amp2 active LFO pan modulator number
*	@param	none
*	@return Amp2 active LFO pan modulator number
*/
int DSP_Voice::get_amp2_pan_lfo_modulation() 
{ 
	return amp2_pan_mod_lfo; 
}

/**
*	@brief	Return Amp2 active LFO pan modulator level
*	@param	none
*	@return Amp2 active LFO pan modulator level
*/
float DSP_Voice::get_amp2_pan_lfo_modulationLevel() 
{ 
	return amp2_pan_mod_lfo_level; 
}

/**
*	@brief	Set Amp1 active LFO pan modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_Voice::set_amp1_pan_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr1->note_on_elapsed_time < amp1_pan_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	}
	
	if (value > 1.0)
	{
		value = 1.0;
	}
	else if (value < -1.0)
	{
		value = 1.0;
	}
	
	amp1_pan_mod = mod_factor * value + out_amp->get_ch1_pan();
	if (amp1_pan_mod < -1.0f)
	{
		amp1_pan_mod = 1.0f;
	}
	else if (amp1_pan_mod > 1.0f)
	{
		amp1_pan_mod = 1.0f;
	}
}

/**
*	@brief	Set Amp2 active LFO pan modulator modulation value
*	@param	mod_factor	modulation factor (depth) 0.0 to 1.0
*	@param	mod_value	modulation value -1.0 to +1.0
*	@return none
*/
void DSP_Voice::set_amp2_pan_lfo_modulation(float mod_factor, float mod_val)
{
	float value = mod_val;
	
	if (adsr1->note_on_elapsed_time < amp2_pan_mod_lfo_delay)
	{
		// Delayed activation
		value = 0.5f;
	} 
	
	if (value > 1.0)
	{
		value = 1.0;
	}
	else if (value < -1.0)
	{
		value = 1.0;
	}
	
	amp2_pan_mod = mod_factor * value + out_amp->get_ch2_pan();
	if (amp2_pan_mod < -1.0f)
	{
		amp2_pan_mod = 1.0f;
	}
	else if (amp2_pan_mod > 1.0f)
	{
		amp2_pan_mod = 1.0f;
	}
}

