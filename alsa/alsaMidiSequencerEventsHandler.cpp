/**
*	@file		midiAlsaSequrncerEventsHandler.h
*	@author		Nahum Budin
*	@date		13-12-2019
*	@version	1.3
*					1. Code refactoring and notaion.
*
*	@brief		Recives data from a alsa midi by puling it out of the midi in rx data queue.
*	
*	History:
*	version 1.2 26-Nov-2020
*					1. Add support in Serial Port MIDI input.
*					2. Add ALSA client to support serial port input stream..*
*	version	1.1 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 Sep 2018)
*		
*/

#include "alsaMidiSequencerEventsHandler.h"
#include "alsaMidiSequencerClient.h"
#include "../midi/midiStream.h"
#include "../synthesizer/modSynth.h"

void(*callback_note_off)(uint8_t ch, uint8_t note, uint8_t vel);
void(*callback_note_on)(uint8_t ch, uint8_t note, uint8_t vel);
void(*callback_poly_aftertouch)(uint8_t ch, uint8_t note, uint8_t pres);
void(*callback_channel_control)(uint8_t ch, uint8_t controller, uint8_t value);
void(*callback_program_change)(uint8_t ch, uint8_t prog);
void(*callback_channel_pressure)(uint8_t ch, uint8_t pres);
void(*callback_pitch_bend)(uint8_t ch, int pitch);
void(*callback_sysex_message)(uint8_t *msg, int len);

void alsa_seq_note_on(uint8_t channel, uint8_t byte2, uint8_t byte3)
{
//	fprintf(stderr, "handler on %i  ", byte2);
	ModSynth::get_instance()->note_on(channel, byte2, byte3);
}

void alsa_seq_note_off(uint8_t channel, uint8_t byte2, uint8_t byte3)
{
//	fprintf(stderr, "handler off %i  ", byte2);
	ModSynth::get_instance()->note_off(channel, byte2);
}

void alsa_seq_change_program(uint8_t channel, uint8_t program)
{
	ModSynth::get_instance()->change_program(channel, program);
}

void alsa_seq_channel_pressure(uint8_t channel, uint8_t val)
{
	ModSynth::get_instance()->channel_pressure(channel, val);
}

void alsa_seq_controller_event(uint8_t channel, uint8_t num, uint8_t val)
{
	ModSynth::get_instance()->controller_event(channel, num, val);
	
	if (ModSynth::get_instance()->midi_control_mapper->get_midi_control_sequences_training_state())
	{
		// Used for updating UI application training dialog display
		callback_update_midi_control_sequnces_data(channel, num, val);
		// Set last data, should this event will be added to the map.
		ModSynth::get_instance()->midi_control_mapper->update_midi_control_sequence_data(channel, num);
	}
	
	// TODO: en/dis mapping?	
	ModSynth::get_instance()->execute_mapped_control(channel, num, val);
}

void alsa_seq_pitch_bend(uint8_t channel, int pitch)
{
	ModSynth::get_instance()->pitch_bend(channel, pitch);
}

inline void register_callback_note_off(void(*fptr)(uint8_t channel, uint8_t note, uint8_t velocity))
{
	callback_note_off = fptr;
}

inline void register_callback_note_on(void(*fptr)(uint8_t channel, uint8_t note, uint8_t velocity))
{
	callback_note_on = fptr;
}

inline void register_callback_poly_aftertouch(void(*fptr)(uint8_t channel, uint8_t note, uint8_t pressure))
{
	callback_poly_aftertouch = fptr;
}

inline void register_callback_channel_control(void(*fptr)(uint8_t channel, uint8_t controller, uint8_t value))
{
	callback_channel_control = fptr;
}

inline void register_callback_program_change(void(*fptr)(uint8_t channel, uint8_t program))
{
	callback_program_change = fptr;
}

inline void register_callback_channel_pressure(void(*fptr)(uint8_t channel, uint8_t pressure))
{
	callback_channel_pressure = fptr;
}

inline void register_callback_pitch_bend(void(*fptr)(uint8_t channel, int pitch))
{
	callback_pitch_bend = fptr;
}

inline void register_callback_sysex_mssg(void(*fptr)(uint8_t *msg, int length))
{
	callback_sysex_message = fptr;
}

void alsa_seq_sysex(uint8_t *message, int len)
{
	uint8_t val1, val2;
	
	if ((message[0] == _MIDI_SYSEX_START) && (message[len - 1] == _MIDI_SYSEX_END))
	{
		// Verify once again it is a sysex message (just in case)
		if ((message[1] == _MIDI_TILTUNE_SYSEX_VENDOR_ID_0) &&
			(message[2] == _MIDI_TILTUNE_SYSEX_VENDOR_ID_1) &&
			(message[3] == _MIDI_TILTUNE_SYSEX_VENDOR_ID_2))
		{
			// Verified TilTune sysex message
			val1 = *(message + 5);
			val2 = *(message + 6);
			
			switch (message[4])
			{
				case _MIDI_SYSEX_RASPI_OFF_MESSAGE_ID_1:
					if ((message[5] == _MIDI_SYSEX_RASPI_OFF_MESSAGE_ID_2) && (message[6] == 0))
					{
						// Turn off raspi
						system("Sysex: sudo poweroff");
					}

					break;

				case _MIDI_SYSEX_AUX_KNOB_1_MESSAGE_ID:
					fprintf(stderr, "Sysex: Knob: %i\n", val1);
					// Updating UI application
					if ((ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_id() != -1) &&
						(ModSynth::get_instance()->midi_control_mapper->get_in_focus_module_id() != -1))
					{
						if ((ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_type() == _CONTROL_TYPE_DIAL) ||
							(ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_type() == _CONTROL_TYPE_SLIDER))
						{
							ModSynth::get_instance()->execute_synth_control(ModSynth::get_instance()->midi_control_mapper->get_in_focus_module_id(),
								ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_id(),
																		  val1);
						}
					}
					
					break;
				

				case _MIDI_SYSEX_AUX_KNOB_2_MESSAGE_ID:

					break;

				case _MIDI_SYSEX_AUX_KNOB_1_SW_MESSAGE_ID:
					fprintf(stderr, "Knob Switch: %i\n", val1);
					
					if ((ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_id() != -1) &&
					  (ModSynth::get_instance()->midi_control_mapper->get_in_focus_module_id() != -1))
					{
						// TOD: handle control switc for select actions
					}
					
					break;

				case _MIDI_SYSEX_AUX_KNOB_2_SW_MESSAGE_ID:

					break;

				case _MIDI_SYSEX_AUX_SLIDER_1_MESSAGE_ID:
					fprintf(stderr, "Sysex: Slider: %i\n", (val1 & 0x7f) + ((uint16_t )val2 << 7));
					
					if ((ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_id() != -1) &&
					  (ModSynth::get_instance()->midi_control_mapper->get_in_focus_module_id() != -1))
					{
						if ((ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_type() == _CONTROL_TYPE_DIAL) ||
							(ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_type() == _CONTROL_TYPE_SLIDER))
						{
							ModSynth::get_instance()->execute_synth_control(ModSynth::get_instance()->midi_control_mapper->get_in_focus_module_id(),
								ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_id(),
																		  ((val1 & 0x7f) + ((uint16_t)val2 << 7)) / 10); // 0-1350 -> 0-127
						}
					}
					
					break;

				case _MIDI_SYSEX_AUX_SLIDER_2_MESSAGE_ID:

					break;				
			}
		}
	}
}



AlsaMidiSeqencerEventsHandler::AlsaMidiSeqencerEventsHandler(uint8_t stage)
	: MidiStream(0, NULL, stage)
{
	register_callback_note_on(&alsa_seq_note_on);
	register_callback_note_off(&alsa_seq_note_off);
	register_callback_channel_pressure(&alsa_seq_channel_pressure);
	register_callback_channel_control(&alsa_seq_controller_event);
	register_callback_program_change(&alsa_seq_change_program);
	register_callback_pitch_bend(&alsa_seq_pitch_bend);
	register_callback_sysex_mssg(&alsa_seq_sysex);

	// Not connected, so must active here
	this->active = true;
}

void AlsaMidiSeqencerEventsHandler::update()
{
	snd_seq_event_t *qev = NULL;
	bool timeout, blocking = false;
	uint8_t sysex_message[512];
	int i;

	qev = AlsaMidiSequencerInputClient::alsa_seq_client_rx_queue.dequeue(0, &timeout, &blocking); //// Non blocking get from Q
	while (qev != NULL)
	{

		switch (qev->type) 
		{
			case SND_SEQ_EVENT_NOTEON:
				if (callback_note_on) 
				{	
					(*callback_note_on)(qev->data.control.channel, qev->data.note.note, qev->data.note.velocity);
				}
				fprintf(stderr,
					"ALSA: Note On event on Channel %2d: %5d       \r",
					qev->data.control.channel,
					qev->data.note.note);
				
				break;
				
			case SND_SEQ_EVENT_NOTEOFF:
				if (callback_note_off) 
				{
					(*callback_note_off)(qev->data.control.channel, qev->data.note.note, qev->data.note.velocity);
				}
				fprintf(stderr,
					"ALSA: Note Off event on Channel %2d: %5d      \r",
					qev->data.control.channel,
					qev->data.note.note);
				
				break;
				
			case SND_SEQ_EVENT_PGMCHANGE:
				if (callback_program_change) 
				{
					(*callback_program_change)(qev->data.control.channel, qev->data.control.value);
				}
				callback_midi_program_change_event(qev->data.control.channel, qev->data.control.value);
				fprintf(stderr,
					"ALSA: Change program event on Channel %2d: %5d      \r",
					qev->data.control.channel,
					qev->data.control.value);
				break;

			case SND_SEQ_EVENT_CHANPRESS:
				if (callback_channel_pressure)
				{
					(*callback_channel_pressure)(qev->data.control.channel, qev->data.control.value);
				}
				fprintf(stderr,
					"ALSA: Channel pressure event on Channel %2d: %5d      \r",
					qev->data.control.channel,
					qev->data.control.value);

				break;			
			
			case SND_SEQ_EVENT_CONTROLLER:
				if (callback_channel_control)
				{
					(*callback_channel_control)(qev->data.control.channel, qev->data.control.param, qev->data.control.value);
				}				
				fprintf(stderr, 
					"ALSA: Control event on Channel %2d: %5d       \r",
					qev->data.control.channel, 
					qev->data.control.value);

				break;

			case SND_SEQ_EVENT_PITCHBEND:
				if (callback_pitch_bend)
				{
					(*callback_pitch_bend)(qev->data.control.channel, qev->data.control.value);
				}				
				fprintf(stderr, 
					"ALSA: Pitchbender event on Channel %2d: %5d   \r",
					qev->data.control.channel, 
					qev->data.control.value);

				break;
				
			case SND_SEQ_EVENT_SYSEX:

				if ((qev->data.ext.ptr != NULL) && (qev->data.ext.len >= 7 )) // min: f0 + 3xvendor id + command + value1 + f7
				{

					fprintf(stderr, "ALSA: Sysex event: ");

					for (i = 0; i < qev->data.ext.len; i++)
					{
						sysex_message[i] = *((uint8_t *)qev->data.ext.ptr + i);
						fprintf(stderr, "%x ", sysex_message[i]);
					}
					fprintf(stderr, "\n");

					if (callback_sysex_message)
					{
						callback_sysex_message(&sysex_message[0], qev->data.ext.len);
					}
				}

				break;
		}

		delete qev;
		qev = AlsaMidiSequencerInputClient::alsa_seq_client_rx_queue.dequeue(0, &timeout, &blocking);
	}
}
