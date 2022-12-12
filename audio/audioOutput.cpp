/**
*	@file		audioOutput.cpp
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

#include "audioOutput.h"
#include "audioManager.h"
#include "../commonDefs.h"

extern pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

AudioManager *_oaudio_manager = NULL;

/**
*   @brief  Create an AudioOutputFloat object instance.
*   @param  stage	audio update sequence stage number
*   @param	bloc_size  audio block-size
*   @param	shared_memory a pointer to a shared_memory_audio_block_float_stereo_struct_t block
*   @param	audio_first_update_ptr  a pointer to an audio block object instance
*								    that is the first in the update chain
*   @return none
*/
AudioOutputFloat::AudioOutputFloat(
				uint8_t stage,
				int block_size,
				shared_memory_audio_block_float_stereo_struct_t *shared_memory,
				AudioBlockFloat **audio_first_update_ptr)
				: AudioBlockFloat(
						2, 						// 2 audio inputs
						input_queue_array, 		// audio block input queue
						audio_first_update_ptr,
						stage) 
{
	audio_block_stereo_float_shared_memory = shared_memory;
	master_gain = 0.2f;
	set_audio_block_size(block_size);
}

/**
*   @brief  sets the audio block size  
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return 0 OK; -1 param out of range
*/
int AudioOutputFloat::set_audio_block_size(int size)
{
	int res = 0;
	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;				
		res = audio_block_size;
	}
	else
	{
		res = -1;
	}
	
	return res;
}
	
/**
*   @brief  retruns the audio block size  
*   @param  none
*   @return buffer size
*/	
int AudioOutputFloat::get_audio_block_size() {	return audio_block_size; }

/**
*   @brief   set the adj synth master volume
*   @param  vol	volume level 0-1.0
*   @return void
*/
void AudioOutputFloat::set_master_volume(float vol)
{
	if ((vol >= 0) && (vol <= 1.00))
	{
		master_gain = vol;
	}
}

/**
*   @brief  get the adj synth master volume
*   @param	none
*   @return volume level 0-100
*/
float AudioOutputFloat::get_master_volume()
{
	return master_gain;
}

/**
*   @brief  Execute an update cycle - get input samples, process and 
*			write it into the shared memory.
*   @param  none
*   @return void
*/
void AudioOutputFloat::update(void) {
	
	audio_block_float_mono_t *in[2];
	volatile  unsigned int i;
	static unsigned int id = 0;

	//	fprintf(stderr, "Update Output\n");
	
	// Get input samples
	in[_LEFT] = receive_audio_block_read_only(_LEFT);
	if (in[_LEFT]) 
	{
		for (i = 0; i < audio_block_size; i++) 
		{
			audio_block_stereo_float_shared_memory->data[_LEFT][i] = in[_LEFT]->data[i] * master_gain;
		}
	}
	else 
	{
		for (i = 0; i < audio_block_size; i++) 
		{
			// No input - fill with silence.
			audio_block_stereo_float_shared_memory->data[_LEFT][i] = 0;
		}
	}
	
	in[_RIGHT] = receive_audio_block_read_only(_RIGHT);
	if (in[_RIGHT]) 
	{
		for (i = 0; i < audio_block_size; i++) 
		{
			audio_block_stereo_float_shared_memory->data[_RIGHT][i] = in[_RIGHT]->data[i] * master_gain;
		}
	}
	else 
	{
		for (i = 0; i < audio_block_size; i++) 
		{
			// No input - fill with silence.
			audio_block_stereo_float_shared_memory->data[_RIGHT][i] = 0;
		}
	}

	//printf("out  %x ", (int)audio_block_stereo_float_shared_memory);
	
	audio_block_stereo_float_shared_memory->id = id;
	id++;
	//	if ((id % (int)(10000000/_PERIOD_TIME_USEC)) == 0)
	//		printf("#transfers: %u  %i sec \n\r", id, (id / (1000000/_PERIOD_TIME_USEC)));

		pthread_mutex_lock(&voice_mem_blocks_allocation_control_mutex);	
	if (in[_LEFT])
	{		
		release_audio_block(in[_LEFT]);
	}
	if (in[_RIGHT])
	{		
		release_audio_block(in[_RIGHT]);
	}			
	pthread_mutex_unlock(&voice_mem_blocks_allocation_control_mutex);
	//	fprintf(stderr, "RO %i\n", in[channel]->memory_pool_index);	
}

