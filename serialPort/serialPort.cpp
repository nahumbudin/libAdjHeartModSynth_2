/**
* @file		serialPort.cpp
*	@author		Nahum Budin
*	@date		21-Oxt-2020
*	@version	1.0
*
*	@brief		Handle serial port
*
*/

#include <thread>

#include "serialPort.h"
#include "../misc/priorities.h"

ADJRS232 *SerialPort::port = NULL;
bool SerialPort::serial_port_rx_thread_is_running = false;
pthread_t SerialPort::serial_port_id;// = NULL;
bool SerialPort::serial_port_is_opened = false;
int SerialPort::port_number = 1;
char SerialPort::default_mode[3] = { '8', 'N', '1' };
SerialPort *SerialPort::serial_port_instance = NULL;
SafeQueue<serialPortData_t *> SerialPort::alsa_serial_port_rx_queue;

SerialPort *SerialPort::get_serial_port_instance(int port_num)
{
	if (serial_port_instance == NULL)
	{
		serial_port_instance = new SerialPort(port_num);
	}

	return serial_port_instance;
}

SerialPort::SerialPort(int serNum)
{
	serial_port_instance = this;
	port_number = serNum;
	port = ADJRS232::getAdjRS232instance();
}

void SerialPort::start_serial_port_RX_thread()
{
	int ret, err, policy;
	pthread_attr_t tattr;
	struct sched_param params;
	// Verify that the Serial Port thread is not allready running
	if(serial_port_rx_thread_is_running)
		return;

	// initialized with default attributes
	ret = pthread_attr_init(&tattr);
	// safe to get existing scheduling param
	ret = pthread_attr_getschedparam(&tattr, &params);
	// set the priority; others are unchanged
	params.sched_priority = sched_get_priority_max(SCHED_RR) - _THREAD_PRIORITY_SERIAL_PORT;
	ret = pthread_attr_setinheritsched(&tattr, PTHREAD_EXPLICIT_SCHED);
	policy = SCHED_RR;
	ret = pthread_attr_setschedpolicy(&tattr, policy);
	// set the new scheduling param
	ret = pthread_attr_setschedparam(&tattr, &params);
	//	err = errno; // for debug
	//	ret = pthread_setschedparam(updatThreadId, SCHED_RR, &params);
	if(ret != 0)
	{
		// Print the error
		fprintf(stderr, "Unsuccessful in setting Serial Port Rx thread realtime prio\n");
	}
	// Set the control flag
	serial_port_rx_thread_is_running = true;
	// Create and start the thread
	ret = pthread_create(&serial_port_id, &tattr, serial_port_rx_thread, (void *)500);
	char thname[] = { "serial_portr_thread" };
	pthread_setname_np(serial_port_id, &thname[0]);
}

void SerialPort::stop_serial_port_RX_thread()
{
	if (port)
	{
		close_port();
		serial_port_is_opened = false;
	}

	// Reset the control flag to terminate the thread
	serial_port_rx_thread_is_running = false;
}

int SerialPort::open_port(int portNum, int baud, const char *mode, int flowCtl)
{
	if (port == NULL)
	{
		printf("Comport not initiated\n");

		return -1;
	}

	if (port->open_comport(portNum, baud, mode, flowCtl))
	{
		printf("Can not open comport\n");

		return (-2);
	}

	port_number = portNum;
	serial_port_is_opened = true;

	start_serial_port_RX_thread();

	return 0;
}

int SerialPort::close_port()
{
	if (port == NULL)
	{
		printf("Comport not initiated\n");

		return -1;
	}

	serial_port_is_opened = false;
	port->close_comport(port_number);

	return 0;
}

//#define _SERIAL_DBG_PRINT_IS_ON

void *SerialPort::serial_port_rx_thread(void *threadid)
{
	int num_of_RX_bytes = 0, i;
	unsigned char rx_buf[1024];

	serialPortData_t *serialPortRxAlsaData;

	fprintf(stderr, "Serial Port thread started\n");

	while (serial_port_rx_thread_is_running)
	{
		if (serial_port_is_opened)
		{
			num_of_RX_bytes = port->poll_comport(port_number, rx_buf, sizeof(rx_buf));

			if (num_of_RX_bytes > 0)
			{
				// New data received - Create a new rx data object (Don't forget to free after processing!!!)
				serialPortRxAlsaData = new serialPortData_t;
				serialPortRxAlsaData->port_num = port_number;
				serialPortRxAlsaData->mssg_len = num_of_RX_bytes;
				for (i = 0; (i < num_of_RX_bytes) && (i < sizeof(serialPortRxAlsaData->message)); i++) // TODO: break longer messages
				{
					serialPortRxAlsaData->message[i] = rx_buf[i];
				}

				if (serialPortRxAlsaData->mssg_len > 100)
				{
					printf("ser data big");
				}

				alsa_serial_port_rx_queue.enqueue(serialPortRxAlsaData);

#ifdef _SERIAL_DBG_PRINT_IS_ON
				printf("Ser port rx bytes: n=%i  ", num_of_RX_bytes);
				for (i = 0; i < num_of_RX_bytes; i++)
				{
					printf("%x ", rx_buf[i]);
				}
				printf("\n");
#endif
			}
		}

		usleep(5000);
	}

	return NULL;
}