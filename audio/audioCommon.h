/**
*	@file		audioSettinge.h
*	@author		Nahum Budin
*	@date		19-Jan-2021
*	@version	1.0 (
*
*	@brief		Audio common settings handling.
*/

#ifndef _AUDIO_SETTINGS_H
#define _AUDIO_SETTINGS_H

#include "../libAdjHeartModSynth_2.h"

#define _AUDIO_MAX_BUF_SIZE				2048

// Audio block of mono samples
typedef struct audio_block_float_mono {

//	audio_block_float_mono(int size)
//		: data(new float[size]) {}
//	audio_block_float_mono()
//		: data(new float[mod_synth_get_audio_block_size()]) {}

	unsigned int id;
	unsigned int memory_pool_index;
	unsigned int ref_count;
	float  *data;
} audio_block_float_mono_t;

typedef struct shared_memory_audio_block_float_stereo_struct {

	shared_memory_audio_block_float_stereo_struct()
		: size(_AUDIO_MAX_BUF_SIZE) {}
	unsigned int id;
	unsigned int ref_count;
	unsigned int size;
	float data[2][_AUDIO_MAX_BUF_SIZE];  // TODO: NOTE MAX BUFFER SIZE
} shared_memory_audio_block_float_stereo_struct_t;




#endif

