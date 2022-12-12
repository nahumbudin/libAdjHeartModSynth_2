/**
* @file		alsaBtClientOutput.h
*	@author		Nahum Budin
*	@date		6-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*
*	@brief		Handles the BT RFCOMM (SPP) midi input streams and 
*				uses ALSA output (source) to enable connecting them to any alsa input.
*
*	version 1.0		11-Oct-2019:
*		First version
*/


#ifndef _ALSA_BT_OUT
#define _ALSA_BT_OUT

#include <pthread.h>

#include <alsa/asoundlib.h> 

class AlsaBtClientOutput
{
public:
	
	~AlsaBtClientOutput();

	static AlsaBtClientOutput *get_instance();

	void start_bt_alsa_out_thread();
	void stop_bt_alsa_out_thread();

private:

	AlsaBtClientOutput();

	static void *bt_alsa_out_thread(void *thread_id);

	/* When true the BT alsa out thread is running when false the thread will terminate */
	static bool bt_alsa_out_thread_is_running;
	/* BT alsa out thread handler */
	static pthread_t bt_alsa_out_thread_id;

	int mode;
	static snd_rawmidi_t* midi_out;

	static AlsaBtClientOutput *alsa_bt_client_output;
};




#endif
