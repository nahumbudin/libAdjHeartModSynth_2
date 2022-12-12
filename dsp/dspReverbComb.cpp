/**
*	@file		dspReverbAllpass.cpp
*	@author		Nahum Budin
*	@date		25_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	2-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018)
*
*	@brief		Reverb comb filter.
*				Written by Jezar at Dreampoint, June 2000
*/

#include "dspReverbComb.h"

DSP_ReverbComb::DSP_ReverbComb()
{
	filter_store = 0;
	buf_idx = 0;
}

/**
*	@brief	Set a buffer
*	@param	buf a pointer to a float buffer
*	@param	size buffer size
*	@return void
*/
void DSP_ReverbComb::set_buffer(float *buf, int size) 
{
	buffer = buf; 
	buf_size = size;
}

/**
*	@brief	Mute reverb - zero all buffer samples
*	@param	none
*	@return void
*/
void DSP_ReverbComb::mute()
{
	for (int i = 0; i < buf_size; i++)
	{
		buffer[i] = 0;
	}
}

/**
*	@brief	Set damp level
*	@param	val damp value
*	@return void
*/
void DSP_ReverbComb::set_damp(float val) 
{
	damp1 = val; 
	damp2 = 1 - val;
}

/**
*	@brief	Return damp level
*	@param	none
*	@return damp value
*/
float DSP_ReverbComb::get_damp() 
{
	return damp1;
}

/**
*	@brief	Set feedback level
*	@param	val feedback value
*	@return void
*/
void DSP_ReverbComb::set_feedback(float val) 
{
	feedback = val;
}

/**
*	@brief	Return feedback level
*	@param	none
*	@return feedback value
*/
float DSP_ReverbComb::get_feedback() 
{
	return feedback;
}
