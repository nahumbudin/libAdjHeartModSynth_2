/**
*	@file		dspDistortion.h
*	@author		Nahum Budin
*	@date		23_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					
*	@History	2-Nov-2019		1.0 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1)
*
*	@brief		Distortion effect.
*				Based on: https://github.com/marcoalkema/cpp-guitar_effects
*/

#ifndef _DSP_DISTORTION
#define _DSP_DISTORTION

class DSP_Distortion
{
public:
	
	DSP_Distortion();
	
	void set_drive(float drv);
	void set_range(float rng);
	void set_blend(float bln);
	float get_next_output_val(float in);
	void enable_auto_gain() { auto_gain = true; }	
	void disable_auto_gain() { auto_gain = false; }

	float drive;
	float range;
	float blend;

	bool auto_gain;
};

#endif

