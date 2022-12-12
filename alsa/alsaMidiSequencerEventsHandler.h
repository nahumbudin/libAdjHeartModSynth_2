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

#ifndef _MIDI_ALSA_Q_EVH
#define _MIDI_ALSA_Q_EVH

#include <stdint.h>
//#include <alsa/asoundlib.h> 

#include "../utils/safeQueues.h"
#include "../midi/midiStream.h"
//#include "../alsa/alsaMidiSequencerClient.h"

void alsa_seq_note_on(uint8_t channel, uint8_t byte2, uint8_t byte3);
void alsa_seq_note_off(uint8_t channel, uint8_t byte2, uint8_t byte3);
void alsa_seq_change_program(uint8_t channel, uint8_t program);
void alsa_seq_chennel_pressure(uint8_t channel, uint8_t val);
void alsa_seq_controller_event(uint8_t channel, uint8_t num, uint8_t val);
void alsa_seq_pitch_bend(uint8_t channel, int pitch);
void alsa_seq_sysex(uint8_t *message, int len);

inline void register_callback_note_off(void(*fptr)(uint8_t channel, uint8_t note, uint8_t velocity));
inline void register_callback_note_on(void(*fptr)(uint8_t channel, uint8_t note, uint8_t velocity));
inline void register_callback_poly_aftertouch(void(*fptr)(uint8_t channel, uint8_t note, uint8_t pressure));
inline void register_callback_channel_control(void(*fptr)(uint8_t channel, uint8_t controller, uint8_t value));
inline void register_callback_program_change(void(*fptr)(uint8_t channel, uint8_t program));
inline void register_callback_channel_pressure(void(*fptr)(uint8_t channel, uint8_t pressure));
inline void register_callback_pitch_bend(void(*fptr)(uint8_t channel, int pitch));
inline void register_callback_sysex_mssg(void(*fptr)(uint8_t *msg, int length));

class AlsaMidiSeqencerEventsHandler : public MidiStream {

public:
	AlsaMidiSeqencerEventsHandler(uint8_t stage = 0);

	virtual void update(void);
	
};

#endif
