/**
*	@file		dspNoise.h
*	@author		Nahum Budin
*	@date		24_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion. 
*					
*	@History	02-Nov-2019 (dspWaveformLUT) (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 19-Jul-2018)
*
*	@brief		Noise generator.
*
*	// https://noisehack.com/generate-noise-web-audio-api/	
*/

#ifndef _DSP_NOISE
#define _DSP_NOISE

#include <stdint.h>

class DSP_Noise
{
public:
	DSP_Noise(int Voice = 0);
	
	float set_amplitude(float amp);
	float get_amplitude();
	int set_noise_type(int nt);
	int get_noise_type();
	
	float get_next_white_noise_val();
	float get_next_pink_noise_val();
	float get_next_brown_noise_val();
	float get_next_noise_val();
	
private:
	int voice;
	float level;
	uint32_t seed;    // must start at 1
	static uint16_t instance_count;
	int noise_type;
};

#endif


