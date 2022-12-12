/**
* @file		midiParser.h
*	@author		Nahum Budin
*	@date		12-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*
*	@brief		MidiParser
*	
*				The midi parser is a midi-stream object with a single input 
*				that parsses midi data streams into midi events.
*
*				Gets a new data block from input queue.
*				If it is a MIDISTREAM_HEADER_TYPE_BULK message block, reads incoming bytes until byte value > 0x80, 
*				 which means a MIDI status byte. (ignore messages that starts with 0xf1-6, 0xf8-e)
*				If there was a previos command and byte <0x80, than rebuilt command as a running-status command (currently not supported).
*				According to the status byte get MIDI length for non Sysex nor a Reset command that
*				starts with an 0xfx (0xf0, 0xf7, 0xff).
*
*	Based on Music Open Lab Library.
*	Copyright AdjHeart Nahum Budin May 2017 and
*
*	History:\n
*
*	version 1.0		13-Oct-2019: 	First version
*/

#include <stdio.h>
#include "midiParser.h"
 
 constexpr uint8_t MidiParser::midi_commands_length[8];
 
MidiParser::MidiParser(uint8_t stage)
	: MidiStream(1, input_queue_array, stage)
{
	update_setup();
			
	num_outputs = 1;
	midi_parser_state = _MIDI_PARSER_STATE_IDLE;			
	midi_buff_in = 0;
	midi_buff_out = 0;			
	midi_command_candidate_index = 0;
}

/**
*   @brief  The block update processing function.
*   @param  none
*   @return void
*/
 void MidiParser::update() {

	//int stream;

	static int tmpcnt = 0;

	midi_stream_mssg_block_t *rx_block;
	midi_stream_mssg_block_t *tx_block;
	raw_data_mssg_block_t *midi_data;
	// Counts valid MIDI messages bytes
 	volatile static int midi_mssg_index = 0;
	// Holds valid MIDI command length
 	volatile static uint8_t command_len;
	// Holds the valid MIDI command command byte
 	volatile uint8_t command;
	// Holds the number of valid commands in this raw message
 	volatile uint8_t num_of_commands = 0;
	// Holds the number of unprocessed bytes in input buffer
 	volatile int remaining_bytes = 0;
	 
	// Get the input buffer
	rx_block = receive_read_only(0);
	if (rx_block != NULL) 
	{
//		printf("parser new block: %i\n", tmpcnt++);
		
		//	fprintf(stderr, "rxBlock pool indx: %i  midiMssgIndex: %i  header length: %i\n", 
		//					rxblock->memory_pool_index, midiMssgIndex, rxblock->header_length);	
		if (rx_block->header_type == _MIDISTREAM_HEADER_TYPE_RAW) 
		{
			// Handle only raw data messages
			//fprintf(stderr, "Parser Raw state. Length: %i\n", rx_block->header_length);
			// Add new data to input buffer
			//fprintf(stderr, "Beffer in indx: %i : data: ", midi_buff_in);
			for (int i = 0; i < rx_block->header_length; i++)
			{
				midi_in_buffer[midi_buff_in] = rx_block->data_t.bytes[i];
				//fprintf(stderr, "%x ", midi_in_buffer[midi_buff_in]);
				midi_buff_in = (midi_buff_in + 1) & buff_length_mask;
			}
			//fprintf(stderr, "\n");			

			tx_block = allocate_midi_stream_block(); ////
			
			//	fprintf(stderr, "data pool indx: %i\n", mididata->memory_pool_index);
			midi_mssg_index = 0;
			// Go over all received bytes and look for commands 
			remaining_bytes = midi_buff_in - midi_buff_out;
			if (remaining_bytes < 0)
			{
				// Total = leftover + new
				remaining_bytes += sizeof(midi_in_buffer);
				//fprintf(stderr, "Remainig bytes: %i\n", remaining_bytes);
			}
			
			while ((remaining_bytes > 0) && (midi_mssg_index < sizeof(raw_data_mssg_block_t::bytes))) 
			{
				if (midi_parser_state == _MIDI_PARSER_STATE_IDLE) 
				{
					fprintf(stderr, "Idle\n");
					// Look for a MIDI command byte (NOTE - running status is not supported!!!)
					if (midi_in_buffer[midi_buff_out] < 0x80) 
					{
						// Not a command byte - goto next byte
						midi_buff_out = (midi_buff_out + 1) & buff_length_mask;
					}
					else 
					{
						// A command byte found
						//fprintf(stderr, "New command\n");
						if (tx_block)	
						{
							//	fprintf(stderr, "Data allocated\n");
							midi_command_candidate_index = 0;
							command = midi_in_buffer[midi_buff_out];
							// Get MIDI command length (0 if command starts with Fx)
							command_len = midi_commands_length[((command - 0x80) >> 4)];
							//fprintf(stderr, "Command: %x len: %x\n", command, command_len);
							// TODO: Does received data includes all this MIDI command bytes?							
							remaining_bytes = midi_buff_in - midi_buff_out;
							// Handle wraparound
							if(remaining_bytes < 0)
							{
								remaining_bytes += sizeof(midi_in_buffer);
								//fprintf(stderr, "Remainig bytes: %i\n", remaining_bytes);							
							}
							if (remaining_bytes >= command_len) 
							{
								// Yes we have all the bytes of this command - Collect data	
								//fprintf(stderr, "Full command\n");
								midi_command[midi_command_candidate_index++] = command;
								midi_buff_out = (midi_buff_out + 1) & buff_length_mask;
								if (command_len != 0) 
								{
									// Handle MIDI commands that have 2 or 3 bytes (0xf_ command len is marked as 0)
									midi_parser_state = _MIDI_PARSER_STATE_COMMAND_IN_PROGRESS;
								}
								else if (command == 0xf0) 
								{
									// Sysex command start
//									fprintf(stderr, "Sysex start\n");
									midi_parser_state = _MIDI_PARSER_STATE_SYSEX_IN_PROGRESS;
								}
								else if (command == 0xff) 
								{
									// Handle Reset commands - it is a single byte command
									tx_block->data_t.bytes[midi_mssg_index++] = command;
									// Add a delimiter byte at the end of the command
									tx_block->data_t.bytes[midi_mssg_index++] = _MIDISTREAM_MIDI_MULTI_DATA_DELIMITER;
									num_of_commands++;
									midi_parser_state = _MIDI_PARSER_STATE_IDLE;
								}
								else 
								{
									// Handle Fx messages
//									fprintf(stderr, "Fx command\n");
								}
							}
							else 
							{
								// No, we do not have all this command bytes - stop collecting data and wait for more data
								break;
							}
						}
						else 
						{
							// No  buffer allocated - abort
							fprintf(stderr, "Midi parser no tx block available: \n");
							break;
						}
					}
				}
				else if (midi_parser_state == _MIDI_PARSER_STATE_COMMAND_IN_PROGRESS) 
				{
					// MIDI command parssing in progress - get more command bytes
					if (midi_in_buffer[midi_buff_out] < 0x80) 
					{
						// command data bytes
						midi_command[midi_command_candidate_index++] = midi_in_buffer[midi_buff_out];
						midi_buff_out = (midi_buff_out + 1) & buff_length_mask;
						command_len--;
						if (command_len <= 1) 
						{
							// Last command byte - add a delimiter byte at the end of the command
							midi_command[midi_command_candidate_index++] = _MIDISTREAM_MIDI_MULTI_DATA_DELIMITER;
							//fprintf(stderr, "Candidate indx: %i\n", midi_command_candidate_index);
							// Add the comand bytes to the data buffer
							for (int i = 0; (i < midi_command_candidate_index) && (midi_mssg_index < sizeof(raw_data_mssg_block_t::bytes)); i++) 
							{
								tx_block->data_t.bytes[midi_mssg_index++] = midi_command[i];
							}
							num_of_commands++;
							midi_parser_state = _MIDI_PARSER_STATE_IDLE;
						}
					}
					else 
					{
						// Illegale byte/command - abort command collection;
						midi_parser_state = _MIDI_PARSER_STATE_IDLE;
						fprintf(stderr, "MIDI parser: command interupted\n");
					}
				}
				else if (midi_parser_state == _MIDI_PARSER_STATE_SYSEX_IN_PROGRESS) 
				{
					midi_command[midi_command_candidate_index++] = midi_in_buffer[midi_buff_out];
					if (midi_in_buffer[midi_buff_out] == 0xf7) 
					{
						// Last Sysexcommand byte- add a delimiter byte at the end of the command
						midi_command[midi_command_candidate_index++] = _MIDISTREAM_MIDI_MULTI_DATA_DELIMITER;
						//	fprintf(stderr, "last sysex: %i\n", midiMssgIndex);
						for (int j = 0; (j < midi_command_candidate_index) && (midi_mssg_index < sizeof(raw_data_mssg_block_t::bytes)); j++)
						{
							//	fprintf(stderr, "%x ", midiCommand[j]);		
							tx_block->data_t.bytes[midi_mssg_index++] = midi_command[j];
						}
						//	fprintf(stderr, "\n");
						num_of_commands++;
						midi_parser_state = _MIDI_PARSER_STATE_IDLE;
					}
					midi_buff_out = (midi_buff_out + 1) & buff_length_mask;
				}

				remaining_bytes = midi_buff_in - midi_buff_out;
				if (remaining_bytes < 0)
					remaining_bytes += sizeof(midi_in_buffer);
				//	fprintf(stderr, "Remaining bytes: %i\n", remainingBytes);
			}
			// No more data
			// Any commands
//			fprintf(stderr, "parser num of cmds: %i\n", numOfCommands);
			if (num_of_commands > 0) 
			{
				// Valid commands found - transmit it
				if (tx_block)  
				{
					//fprintf(stderr, "\num of cmds: %i Out buff: %i data: ", num_of_commands, midi_buff_out);
					//for (int i = 0; i < midi_mssg_index; i++) 
					//{
					//	fprintf(stderr, "%x ", (uint8_t)tx_block->data_t.bytes[i]);
					//}
					//fprintf(stderr, "\n");				

					tx_block->header_type = _MIDISTREAM_HEADER_TYPE_MIDI_MULTI;
					tx_block->header_length = midi_mssg_index;
					// Transmit on channel 0
					transmit(tx_block, 0);
					release_midi_stream_block(tx_block);
					midi_mssg_index = 0;
				}
				
				else
				{
					// Error no block
					fprintf(stderr, "Midi parser no tx block available: \n");
				}

				midi_mssg_index = 0;
				num_of_commands = 0;

			}
			//	else { }
		}
		// Not a raw data message or end of process
//		if (rx_block->ref_count <= 1) 
//		{
//		}
		release_midi_stream_block(rx_block);
	}
	else
	{
		//  no block received
//		fprintf(stderr, "Midi parser no rx block received: \n");
	}
} 
					  

 	
 				
 		
 		
 		
 	
 		
 				