/**
*	@file		prioritiesr.h
*	@author		Nahum Budin
*	@date		23_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*	
*	@History	30-Oct-2019	1.0 
*
*	@brief		Envelope generator.
*/

#ifndef _PRIORITIES_H
#define _PRIORITIES_H

#define _THREAD_PRIORITY_CHANGE_CONTROL	9 // Highest
#define _THREAD_PRIORITY_UPDATE_TIMER	10		  
#define _THREAD_PRIORITY_UPDATE			11
#define _THREAD_PRIORITY_JACK			12
#define _THREAD_PRIORITY_ALSA			13
#define _THREAD_PRIORITY_MIDI_IN		20
#define _THREAD_PRIORITY_MIDI_OUT		20
#define _THREAD_PRIORITY_MIDI_STREAM	21
#define _THREAD_PRIORITY_BT_CONNECTED	22
#define _THREAD_PRIORITY_BT_MAIN		23
#define _THREAD_PRIORITY_SERIAL_PORT	24

#endif
