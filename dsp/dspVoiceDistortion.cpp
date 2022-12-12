/**
*	@file		dspVoiceDistortion.cpp
*	@author		Nahum Budin
*	@date		28_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	9-Jun-2018 (modulators part of voice; no control blocks and connections.)
*
*	@brief		Synthesizer voice dsp processing - Distortion Effect Handling.
*				
*/

#include "dspVoice.h"

/**
*	@brief	Set Distortion1 Drive level
*	@param drv drive level _DISTORTION_MIN_DRIVE to _DISTORTION_MAX_DRIVE
*	@return void
*/
void DSP_Voice::set_distortion1_drive(float drv) 
{ 
	distortion1->set_drive(drv); 
}

/**
*	@brief	Set Distortion1 Range level
*	@param rng range level _DISTORTION_MIN_RANGE to _DISTORTION_MAX_RANGE
*	@return void
*/
void DSP_Voice::set_distortion1_range(float rng) 
{ 
	distortion1->set_range(rng); 
}

/**
*	@brief	Set Distortion1 Blend level
*	@param drv bln level _DISTORTION_MIN_BLEND to _DISTORTION_MAX_BLEND
*	@return void
*/
void DSP_Voice::set_distortion1_blend(float bln) 
{ 
	distortion1->set_blend(bln); 
}

/**
*	@brief	Enable Distortion1 auto gain.
*	@param none
*	@return void
*/
void DSP_Voice::enable_distortion1_auto_gain() 
{ 
	distortion1->auto_gain = true; 
}

/**
*	@brief	Disable Distortion1 auto gain.
*	@param none
*	@return void
*/
void DSP_Voice::disable_distortion1_auto_gain() 
{ 
	distortion1->auto_gain = false; 
}

/**
*	@brief	Set Distortion2 Drive level
*	@param drv drive level _DISTORTION_MIN_DRIVE to _DISTORTION_MAX_DRIVE
*	@return void
*/
void DSP_Voice::set_distortion2_drive(float drv) 
{ 
	distortion2->set_drive(drv); 
}

/**
*	@brief	Set Distortion2 Range level
*	@param rng range level _DISTORTION_MIN_RANGE to _DISTORTION_MAX_RANGE
*	@return void
*/
void DSP_Voice::set_distortion2_range(float rng) 
{ 
	distortion2->set_range(rng); 
}

/**
*	@brief	Set Distortion2 Blend level
*	@param drv bln level _DISTORTION_MIN_BLEND to _DISTORTION_MAX_BLEND
*	@return void
*/
void DSP_Voice::set_distortion2_blend(float bln) 
{ 
	distortion2->set_blend(bln); 
}

/**
*	@brief	Enable Distortion2 auto gain. 
*	@param none
*	@return void
*/
void DSP_Voice::enable_distortion2_auto_gain() 
{ 
	distortion2->auto_gain = true; 
}

/**
*	@brief	Disable Distortion1 auto gain.
*	@param none
*	@return void
*/
void DSP_Voice::disable_distortion2_auto_gain() 
{ 
	distortion2->auto_gain = false; 
}

