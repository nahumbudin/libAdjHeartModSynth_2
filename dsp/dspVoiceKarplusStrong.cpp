/**
*	@file		dspVoiceKarplusStrong.cpp
*	@author		Nahum Budin
*	@date		25_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					2. Adding sample-rate and bloc-size settings
*					
*	@History	9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Karplus Strong Generator Handling.
*				
*/

#include "dspVoice.h"

/**
*	@brief	Set Kps1 send level to channel 1
*	@param lev Kps1 send level to channel 1 0.0 to 1.0
*	@return void
*/
void DSP_Voice::set_karplus1_send_filter1_level(float lev)
{
	karpuls1_send_filter1_level = lev;
	if (karpuls1_send_filter1_level < 0.0f)
	{
		karpuls1_send_filter1_level = 0.0f;
	}
	else if (karpuls1_send_filter1_level > 1.0f)
	{
		karpuls1_send_filter1_level = 1.0f;
	}
}

/**
*	@brief	Set Kps1 send level to channel 1 (int)
*	@param lev Kps1 send level to channel 1 0-100
*	@return void
*/
void DSP_Voice::set_karplus1_send_filter1_level(int lev)
{
	set_karplus1_send_filter1_level((float)lev / 100.0f);
}

/**
*	@brief	return Kps1 send level to channel 1
*	@param none
*	@return Kps1 send level to channel 1
*/
float DSP_Voice::get_karplus1_send_filter1_level() { return karpuls1_send_filter1_level; }

/**
*	@brief	Set Kps1 send level to channel 2
*	@param lev Kps1 send level to channel 2 0.0 to 1.0
*	@return void
*/
void DSP_Voice::set_karplus1_send_filter2_level(float lev)
{
	karpuls1_send_filter2_level = lev;
	if (karpuls1_send_filter2_level < 0.0f)
	{
		karpuls1_send_filter2_level = 0.0f;
	}
	else if (karpuls1_send_filter2_level > 1.0f)
	{
		karpuls1_send_filter2_level = 1.0f;
	}
}

/**
*	@brief	Set Kps1 send level to channel 2 (int)
*	@param lev Kps1 send level to channel 2 (int) 0 to 100
*	@return void
*/
void DSP_Voice::set_karplus1_send_filter2_level(int lev)
{
	set_karplus1_send_filter2_level((float)lev / 100.0f);
}

/**
*	@brief	return Kps1 send level to channel 2
*	@param none
*	@return Kps1 send level to channel 2
*/
float DSP_Voice::get_karplus1_send_filter2_level() { return karpuls1_send_filter2_level; }
	
	
	