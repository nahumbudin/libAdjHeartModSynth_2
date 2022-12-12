/**
*	@file		audioReverb.h
*	@author		Nahum Budin
*	@date		2-Feb-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and bloc-size settings
*					
*	@version	1.0		9-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 May 15, 2018)
*
*	@brief		Audio Reverb
*/

#ifndef _AUDIO_REVERB
#define _AUDIO_REVERB

//#include "../headers.h"

#include "audioBlock.h"
#include "../dsp/dspReverbModel.h"
#include "../dsp/dspFreeverb3mod2.h"

class AudioReverb : public AudioBlockFloat
{
public:
	AudioReverb(
		uint8_t stage = 0, 
		int samp_rate = _DEFAULT_SAMPLE_RATE,
		int block_size = _DEFAULT_BLOCK_SIZE,
		AudioBlockFloat **audio_first_update_ptr = NULL);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();

	
	void rev_enable(); 
	void rev_disable();

	void rev3m_enable();
	void rev3m_disable();
		
	virtual void update(void);
	
	DSP_RevModel *reverb;
	sf_reverb_state_st rv3m;

private:
	audio_block_float_mono *audio_input_queue_array[2];

	bool rev_enabled, rev3m_enabled;
	
	int sample_rate, audio_block_size;
}
;

#endif

