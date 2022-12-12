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

#include "alsaSerialPortClientOutput.h"
#include "../serialPort/serialPort.h"
#include "../misc/priorities.h"

bool AlsaSerialPortClientOutput::alsa_serial_port_out_thread_is_running = false;
pthread_t AlsaSerialPortClientOutput::alsa_serial_port_out_thread_id;
snd_rawmidi_t *AlsaSerialPortClientOutput::midi_out = NULL;
AlsaSerialPortClientOutput *AlsaSerialPortClientOutput::alsa_serial_port_client_output = NULL;

/* Will hold data received from Serial port */
serialPortData_t *serial_port_rx_alsa_channel_data = NULL;

AlsaSerialPortClientOutput::AlsaSerialPortClientOutput()
{
	const char *port_name = "virtual";
	int status;
	snd_rawmidi_info_t *info;
	

	mode = SND_RAWMIDI_SYNC;
	
	status = snd_rawmidi_open(NULL, &midi_out, port_name, mode);

	if (status < 0)
	{
		printf("ALSA serialport client: Problem opening MIDI output: %s", snd_strerror(status));
		//		exit(1);
	}
	
//	status = snd_rawmidi_info(midi_out, info);
	
	if (status < 0)
	{
		
		
	}
}

AlsaSerialPortClientOutput::~AlsaSerialPortClientOutput()
{
	snd_rawmidi_close(midi_out);
	midi_out = NULL; // snd_rawmidi_close() does not clear invalid pointer,
}

/**
*   @brief  retruns the single serial port client alsa output instance
*
*   @param  none
*   @return the single serial port client alsa output
*/
AlsaSerialPortClientOutput *AlsaSerialPortClientOutput::get_instance()
{
	if (alsa_serial_port_client_output == NULL)
	{
		alsa_serial_port_client_output = new(AlsaSerialPortClientOutput);
	}

	return alsa_serial_port_client_output;
}

/**
 * Starts the serial port client thread looking for new bt data
 */
void AlsaSerialPortClientOutput::start_alsa_serial_port_out_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;
	// Verify that the serial port thread is not allready running
	if(alsa_serial_port_out_thread_is_running)
	{
		return;
	}

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_MIDI_OUT;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// set the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	//	err = errno; // for debug
	//	ret = pthread_setschedparam(updatThreadId, SCHED_RR, &params);
	if (ret != 0)
	{
		// Print the error
		fprintf(stderr, "ALSA serialport client: Unsuccessful in setting serial port alsa out thread realtime prio\n");
	}
	// Set the control flag
	alsa_serial_port_out_thread_is_running = true;
	// Create and start the thread
	ret = pthread_create(&alsa_serial_port_out_thread_id, &tattr, alsa_serial_port_out_thread, (void *)100);
	char thname[] = {"alsa_ser_port_out_thread"};
	pthread_setname_np(alsa_serial_port_out_thread_id, &thname[0]);
}

/**
 * Stop the serial port client thread.
 */
void AlsaSerialPortClientOutput::stop_alsa_serial_port_out_thread()
{
	// Reset the control flag to terminate the thread
	alsa_serial_port_out_thread_is_running = false;
}

/**
* Serial alsa output thread.
*
* 1. Polls for a new data in the serial port Rx alsa data queue every 5msc.
* 2. Sends new data through the alsa output
*/
void *AlsaSerialPortClientOutput::alsa_serial_port_out_thread(void *threadid)
{
	bool timeout = false, blocking = true, non_blocking = false;
	int count, last_count, status, timeouts = 0;
	uint8_t bytes[512];

	fprintf(stderr, "Serial Prt alsa out thread started\n");

	while (alsa_serial_port_out_thread_is_running)
	{
		// Wait for new data - non blocking
		serial_port_rx_alsa_channel_data = SerialPort::alsa_serial_port_rx_queue.dequeue(0, &timeout, &non_blocking);
		// Get all new data
		count = 0;
		last_count = 0;
		while (serial_port_rx_alsa_channel_data != NULL)
		{
			// Collect all available data
			for (count; count < (last_count + serial_port_rx_alsa_channel_data->mssg_len) &&
						count < sizeof(serial_port_rx_alsa_channel_data->message) &&
						count < sizeof(bytes);
				 count++)
			{
				bytes[count] = serial_port_rx_alsa_channel_data->message[count - last_count];
			}
			last_count = count;
			delete serial_port_rx_alsa_channel_data;
			serial_port_rx_alsa_channel_data = SerialPort::alsa_serial_port_rx_queue.dequeue(0, &timeout, &non_blocking);
			//			printf("Serial port in data alsa thread while data\n");
		}

		if (count > 0)
		{
			// New data is available
			//			printf("\nSerial Port in data alsa thread: count: %i  data: ", count);
			//			for (int i = 0; i < count; i++)
			//				printf("%x ", bytes[i]);
			//			printf("\n");

			if ((status = snd_rawmidi_write(midi_out, bytes, count)) < 0)
			{
				printf("Problem writing serial port data to MIDI output: %s", snd_strerror(status));
				//					exit(1);
			}
		}

		usleep(5000);
	}

	return 0;
}
