/**
*	@file		audioOutput.h
*	@author		Nahum Budin
*	@date		29_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					2. Adding bloc-size settings
*					
*	@version	1.0		11-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 May 17, 2017)
*
*	@brief		Audio system output block
*/

#ifndef _AUDIO_OUTPUT
#define _AUDIO_OUTPUT

#include "audioBlock.h"

class AudioOutputFloat : public AudioBlockFloat 
{
public:
	AudioOutputFloat(
		uint8_t stage = 0,
		int block_size = _DEFAULT_BLOCK_SIZE,	
		shared_memory_audio_block_float_stereo_struct_t *shared_memory = NULL,
		AudioBlockFloat **audio_first_update_ptr = NULL);

	int set_audio_block_size(int size);
	int get_audio_block_size();
	
	void set_master_volume(float vol);
	float get_master_volume();
		
	virtual void update(void);
				
private:
	// 2 input queues for L and R sterams
	audio_block_float_mono_t *input_queue_array[2];
	// Shared memory for transfering data to audio driver
	shared_memory_audio_block_float_stereo_struct_t *audio_block_stereo_float_shared_memory;

	float master_gain;
	
	int audio_block_size;
		
};

#endif

