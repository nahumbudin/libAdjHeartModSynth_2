/**
*	@file		alsaMidiSequencerClient.h
*	@author		Nahum Budin
*	@date		6-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*
*	@brief		ALSA MIDI input client. Pushes ALSA midi input data into alsa_seq_client_rx_queue
*				https://ccrma.stanford.edu/~craig/articles/linuxmidi/alsa-1.0/
*				
*	History:\n
*	
*		version 1.0		13-Dec--2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 Dec 2017)
*/

#ifndef _MIDI_ALSA_SEQ
#define _MIDI_ALSA_SEQ

#include <pthread.h>
#include <alsa/asoundlib.h> 

#include "../utils/safeQueues.h"

class AlsaMidiSequencerInputClient
{
public:
	AlsaMidiSequencerInputClient();
	~AlsaMidiSequencerInputClient();

	static snd_seq_t *open_seq();
	void close_seq();

	static bool midi_in_client_thread_is_running;
	static pthread_t midi_client_in_thread;

	static 	snd_seq_t *midi_in_seq;

	static SafeQueue<snd_seq_event_t*> alsa_seq_client_rx_queue;

private:

	void start_midi_in_seq_client_thread();
	void stop_midi_in_seq_client_thread();

};

void *midi_in_seq_client_thread(void *arg);

#endif
