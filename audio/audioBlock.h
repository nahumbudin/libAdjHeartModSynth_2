/**
*	@file		audioBlock.h
*	@author		Nahum Budin
*	@date		29_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					
*	@version	1.1		4-Nov-2019 (13/6/2018 No control blocks)
*
*	@brief		Audio blocks and connections .
*/

#ifndef _AUDIO_BLOCKS
#define _AUDIO_BLOCKS

#include <stdint.h>
#include <stdlib.h>

#include "audioCommon.h"

// Create a pool of audio blocks to avoid memory allocation in runtime
#define AllocateAudioMemoryBlocksFloatPool(num, block_size) ({ \
	static audio_block_float_mono_t blocks[num]; \
	AudioBlockFloat::initialize_audio_memory(blocks, num, block_size); \
})
	

// Must be a multiple of 32
#define _MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE 1024
#if ((_MAX_AUDIO_SBLOCKS_MESSAGES_POOL_SIZE & 0x1f) != 0)
Error : must be n * 32
#endif
	// Must be no more than 2048
#if (_MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE > 2048)
	Error : must be less than 2048
#endif
	// Used for allocation 32 bits masks
#define _AUDIO_BLOCKS_MESSAGES_POOL_NUM_OF_MASKS (_MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE >> 5) 

#define _MAX_NUM_OF_AUDIO_CONNECTIONS			2048

#define _CONNECTION_OK							0
#define _CONNECTION_NOT_CONNECTED				-1
#define _NULL_CONNECTION						-2
#define _CONNECTION_LIST_EMPTY					-3

class AudioConnectionFloat;

class AudioBlockFloat 
{
public:
	
	AudioBlockFloat(
		uint16_t num_of_aud_inputs,
		audio_block_float_mono_t **audio_queues,
		AudioBlockFloat **audio_first_update_ptr,
		uint16_t stage = 0); 
	
	AudioConnectionFloat *get_audio_destination_list() 
	{ 
		return audio_destination_list; 
	}
	
	int get_stage();
	
	static void enable_updates();
	static void disable_updates();
	
	static bool update_is_in_progress();
	
//	static int set_period_time_us(unsigned long ptu);
	
	static void initialize_audio_memory(audio_block_float_mono_t *blocks, uint16_t num, uint16_t block_size);	
	
	static int16_t random(int16_t min, int16_t max);	

	// Perform block processing 
	virtual void update(void) = 0;

	// for update_all
	AudioBlockFloat *audio_next_update;
	// for update_all - holds the 1st object in the propagate chain
	/*static*/ AudioBlockFloat **audio_first_update;
		
protected:
	static audio_block_float_mono_t * allocate_audio_block(void);	
	static void release_audio_block(audio_block_float_mono_t * block);	
	void transmit_audio_block(audio_block_float_mono_t *block, uint16_t index = 0);	
	audio_block_float_mono_t * receive_audio_block_read_only(uint16_t index = 0);	
	audio_block_float_mono_t * receive_audio_block_writable(uint16_t index = 0);	
	
	static void free_used_audio_blocks();
	
	void clear_audio_destinations_list() {audio_destination_list = NULL; } 
	
	static void update_start(void);
	static void update_stop(void);
	
//	// Used audio driver: alsa or jack audio
//	int audio_driver_type;
	// Number of audio inputs 
	uint16_t num_of_audio_inputs;
	// Number of audio outputs */
	uint16_t num_of_audio_outputs;
	// object is associated with this stage num
	volatile uint16_t stage_num;
	// Object is run on this core
	//volatile uint8_t core_num;
	
	bool active;
		
private:
	
	// holds a linked list of objects to be propagate audio data through
	AudioConnectionFloat *audio_destination_list; 
	// Input queues
	audio_block_float_mono **audio_input_queue;
	// When true update process is enables
	volatile static bool update_enable;
	volatile static bool update_in_progress;
	// A global memory pool of audio blocks
	static audio_block_float_mono_t *audio_data_blocks_memory_pool;
	static uint32_t audio_data_blocks_memory_pool_available_mask[];
	static uint16_t audio_data_blocks_memory_pool_first_mask;
	static uint16_t audio_block_size;
	
	// period time in us 
//	static unsigned int period_time;
	
	friend class AudioConnectionFloat;
	friend class AudioConnectionsManagerFloat;
};

//class AudioBlockFloat;

class AudioConnectionFloat
{
public:
	AudioConnectionFloat();

	void connect(AudioBlockFloat *source, AudioBlockFloat *destination);
	void connect(AudioBlockFloat *source,
		uint16_t source_output,
		AudioBlockFloat *destination,
		uint16_t destination_input);
	uint32_t get_connection_id();

	bool is_connected();
	void disconnect_this();
	bool to_disconnect();
	void was_disconnected();

protected:
	AudioBlockFloat *src;
	AudioBlockFloat *dst;
	uint16_t src_index;
	uint16_t dest_index;
	AudioConnectionFloat *next_dest;

	friend class AudioBlockFloat;
	friend class AudioConnectionsManagerFloat;

private:
	uint32_t id;
	bool connected;
	bool disconnect;

}
;

class AudioConnectionsManagerFloat
{
public:
	AudioConnectionsManagerFloat();

	AudioConnectionFloat *get_audio_connection();
	int delete_audio_connection(uint16_t id);

private:
	static AudioConnectionFloat *connections[_MAX_NUM_OF_AUDIO_CONNECTIONS];
};

#endif

