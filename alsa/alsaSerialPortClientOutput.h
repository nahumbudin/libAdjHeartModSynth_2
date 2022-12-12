/**
*	@file		alsaSerialPortClientOutput.h
*	@author		Nahum Budin
*	@date		6-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*	
*		version 1.0		27-Nov-2020
*
*	@brief		Serial Port input based alsa output client (serial->alsa)
*				
*/

#ifndef _ALSA_SERIAL_PORT_OUT
#define _ALSA_SERIAL_PORT_OUT

#include <pthread.h>
#include <alsa/asoundlib.h>

#include "../utils/safeQueues.h"
#include "../../../../RS232lib/adjRS232.h"

class AlsaSerialPortClientOutput
{
  public:
	~AlsaSerialPortClientOutput();

	static AlsaSerialPortClientOutput *get_instance();

	void start_alsa_serial_port_out_thread();
	void stop_alsa_serial_port_out_thread();
	
	
  private:
	AlsaSerialPortClientOutput();

	static void *alsa_serial_port_out_thread(void *thread_id);

	/* When true the Serial Port alsa out thread is running when false the thread will terminate */
	static bool alsa_serial_port_out_thread_is_running;
	/* Serial port alsa out thread handler */
	static pthread_t alsa_serial_port_out_thread_id;

	int mode;
	static snd_rawmidi_t *midi_out;

	static AlsaSerialPortClientOutput *alsa_serial_port_client_output;
};

#endif

