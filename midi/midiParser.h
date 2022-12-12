/**
* @file		midiParser.h
*	@author		Nahum Budin
*	@date		12-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*
*	@brief	The midi parser is a midi-stream object with a single input 
*			that parsses midi data streams into midi events.
*			
*			Gets a new data block from input queue.
*			If it is a MIDISTREAM_HEADER_TYPE_BULK message block, reads incoming bytes until byte value > 0x80, 
*			 which means a MIDI status byte. (ignore messages that starts with 0xf1-6, 0xf8-e)
*			If there was a previos command and byte <0x80, than rebuilt command as a running-status command (currently not supported).
*			According to the status byte get MIDI length for non Sysex nor a Reset command that
*			starts with an 0xfx (0xf0, 0xf7, 0xff).
*
*	Based on Music Open Lab Library.
*	Copyright AdjHeart Nahum Budin May 2017 and
*
*	History:\n
*
*	version 1.0		13-Oct-2019: 	First version
*
*/

#ifndef _MIDI_PARSSER
#define _MIDI_PARSSER

#include "midiStream.h"

class MidiParser : public MidiStream {
	 
	public:
		MidiParser(uint8_t stage = 0);
	
		virtual void update(void);	
		
		/* How many bytes in MIDI commands. Last is unknown for Sysex */
		static constexpr uint8_t midi_commands_length[8] = {3, 3, 3, 3, 2, 2, 3, 0};
	
	private:

		raw_data_mssg_block_t *m_data;
		uint8_t midi_parser_state;
		midi_stream_mssg_block_t *input_queue_array[1];
		
		// Holds incoming data
		uint8_t midi_in_buffer[_MIDI_STREAM_MESSAGE_LEN*4];
		// Points to next input byte buffer index
		volatile unsigned int midi_buff_in;
		// Points to next output byte buffer index
		volatile unsigned int midi_buff_out;
		// A mask for buffer length bitset
		volatile const unsigned int buff_length_mask = sizeof(midi_in_buffer) - 1;
		// Holds a command candidate data when collected
		uint8_t midi_command[_MIDI_MSSG_MAX_LEN];
		// Holds a pointer to midi command candidate byte index
		volatile uint8_t midi_command_candidate_index;
		
//		int parse(midi_stream_mssg_block_t *block);
};

#endif

