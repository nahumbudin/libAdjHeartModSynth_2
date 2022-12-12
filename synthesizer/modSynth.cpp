/**
* @file		modSynth.cpp
*	@author		Nahum Budin
*	@date		7-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*					2. Moving general settings manager (audio, midi) from AdjSynth to Mod Synth
*					3. Adding sample-rate and audio block-size settings
*					4. Adding ALSA MIDI
*					5. Adding start/stop audio
*	
*	@brief		This is the main libraray synthesizer object.
*
*	History:\n
*	
*	version 1.0		5-Oct-2019:		First version*		
*/

#include <omp.h>

#include "modSynth.h"

#include "../midi/midiAlsaQclient.h"
#include "../midi/midiParser.h"
#include "../midi/midiHandler.h"

#include "../midi/midiControlMapper.h"

#include "../alsa/alsaMidi.h"
#include "../alsa/alsaMidiSequencerClient.h"
#include "../alsa/alsaMidiSequencerEventsHandler.h"

#include "../alsa/alsaBtClientOutput.h"


#include "../serialPort/serialPort.h"

#include "../utils/XMLfiles.h"

#include "../cpuUtilizaion/CPUSnapshot.h"

//#include "../midi/midiParser.h"
//#include "../midi/midiHandler.h"
//#include "../midi/midiBtQclientIn.h"
//#include "../midi/midiAlsaSequencerEventsHandler.h"

/* modSynth instance */
ModSynth *ModSynth::mod_synth = NULL;

int ModSynth::cpu_utilization = 0;
volatile bool cheack_cpu_utilization_thread_is_running;

// Serial port - used for USB FTDI input of MIDI control messages
//				 (e.g. external knob or slider using sysex messages)
SerialPort *serial_port = NULL; // = SerialPort::get_serial_port_instance(16);  // 16=ttyUSB0

/*************************** ALSA MIDI *******************************************/

/* ALSA MIDI sequencer client - Receives input midi data stream from ALSA midi source.
 *								Connet to it using, for example, a JACK Audio Connection Kit. 
 *								Received data is pushed into the alsa_seq_client_rx_queue */
AlsaMidiSequencerInputClient *alsa_midi_sequencer_input_client = new AlsaMidiSequencerInputClient();
/* Pulls out the events from alsa_seq_client_rx_queue and executes commands */
AlsaMidiSeqencerEventsHandler alsa_midi_seqencer_events_handler_1(_MIDI_STAGE_2);

//#define _MAX_NUM_OF_MIDI_CONNECTIONS 1

#if (_MAX_NUM_OF_MIDI_CONNECTIONS > 0)
/* MIDI ALSA input client - waits for data in the alsa_rx_queue */
MidiAlsaQclientIn alsa_midi_in_1(&AlsaMidi::get_instance()->alsa_rx_queue[0], _MIDI_STAGE_0);
MidiParser midi_parser_alsa_1(_MIDI_STAGE_1);
MidiHandler midi_handler_alsa_1(_MIDI_STAGE_2); 

/* ALSA MIDI connection ALSA-client -> parser */
MidiConnection midi_connection_alsa_1(&alsa_midi_in_1, 0, &midi_parser_alsa_1, 0);
/* ALSA MIDI connection parser -> handler */
MidiConnection midi_connection_alsa_handler_1(&midi_parser_alsa_1, 0, &midi_handler_alsa_1, 0);

#endif

#if (_MAX_NUM_OF_MIDI_CONNECTIONS > 1)
MidiAlsaQclientIn alsa_midi_in_2(&AlsaMidi::get_instance()->alsa_rx_queue[1], _MIDI_STAGE_0);
MidiParser midi_parser_alsa_2(_MIDI_STAGE_1);
//MidiHandler midi_handler_alsa_2(_MIDI_STAGE_2);
MidiConnection midi_connection_alsa_2(&alsa_midi_in_2, 0, &midi_parser_alsa_2, 0);
MidiConnection midi_connection_alsa_handler_2(&midi_parser_alsa_2, 0, &midi_handler_alsa_1, 1);

#endif

#if (_MAX_NUM_OF_MIDI_CONNECTIONS > 2)
MidiAlsaQclientIn alsa_midi_in_3(&AlsaMidi::get_instance()->alsa_rx_queue[2], _MIDI_STAGE_0);
MidiParser midi_parser_alsa_3(_MIDI_STAGE_1);
//MidiHandler midi_handler_alsa_3(_MIDI_STAGE_2);
MidiConnection midi_connection_alsa_3(&alsa_midi_in_3, 0, &midi_parser_alsa_3, 0);
MidiConnection midi_connection_alsa_handler_3(&midi_parser_alsa_3, 0, &midi_handler_alsa_1, 2);

#endif

#if (_MAX_NUM_OF_MIDI_CONNECTIONS > 3)
MidiAlsaQclientIn alsa_midi_in_4(&AlsaMidi::get_instance()->alsa_rx_queue[3], _MIDI_STAGE_0);
MidiParser midi_parser_alsa_4(_MIDI_STAGE_1);
//MidiHandler midi_handler_alsa_4(_MIDI_STAGE_2);
MidiConnection midi_connection_alsa_4(&alsa_midi_in_4, 0, &midi_parser_alsa_4, 0);
MidiConnection midi_connection_alsa_handler_4(&midi_parser_alsa_4, 0, &midi_handler_alsa_1, 3);

#endif

#if (_MAX_NUM_OF_MIDI_CONNECTIONS > 4)
MidiAlsaQclientIn alsa_midi_in_5(&AlsaMidi::get_instance()->alsa_rx_queue[4], _MIDI_STAGE_0);
MidiParser midi_parser_alsa_5(_MIDI_STAGE_1);
//MidiHandler midi_handler_alsa_5(_MIDI_STAGE_2);
MidiConnection midi_connection_alsa_5(&alsa_midi_in_5, 0, &midi_parser_alsa_5, 0);
MidiConnection midi_connection_alsa_handler_5(&midi_parser_alsa_5, 0, &midi_handler_alsa_1, 4);

#endif

#if (_MAX_NUM_OF_MIDI_CONNECTIONS > 5)
MidiAlsaQclientIn alsa_midi_in_6(&AlsaMidi::get_instance()->alsa_rx_queue[5], _MIDI_STAGE_0);
MidiParser midi_parser_alsa_6(_MIDI_STAGE_1);
//MidiHandler midi_handler_alsa_6(_MIDI_STAGE_2);
MidiConnection midi_connection_alsa_6(&alsa_midi_in_6, 0, &midi_parser_alsa_6, 0);
MidiConnection midi_connection_alsa_handler_6(&midi_parser_alsa_6, 0, &midi_handler_alsa_1, 5);

#endif

#if (_MAX_NUM_OF_MIDI_CONNECTIONS > 6)
MidiAlsaQclientIn alsa_midi_in_7(&AlsaMidi::get_instance()->alsa_rx_queue[6], _MIDI_STAGE_0);
MidiParser midi_parser_alsa_7(_MIDI_STAGE_1);
//MidiHandler midi_handler_alsa_7(_MIDI_STAGE_2);
MidiConnection midi_connection_alsa_7(&alsa_midi_in_7, 0, &midi_parser_alsa_7, 0);
MidiConnection midi_connection_alsa_handler_7(&midi_parser_alsa_7, 0, &midi_handler_alsa_1, 6);

#endif

#if (_MAX_NUM_OF_MIDI_CONNECTIONS > 7)
MidiAlsaQclientIn alsa_midi_in_8(&AlsaMidi::get_instance()->alsa_rx_queue[7], _MIDI_STAGE_0);
MidiParser midi_parser_alsa_8(_MIDI_STAGE_1);
//MidiHandler midi_handler_alsa_8(_MIDI_STAGE_2);
MidiConnection midi_connection_alsa_8(&alsa_midi_in_8, 0, &midi_parser_alsa_8, 0);
MidiConnection midi_connection_alsa_handler_8(&midi_parser_alsa_8, 0, &midi_handler_alsa_1, 7);

#endif


/* TODO: used for directlly connecting to MIDI input devices without JACK connection kit*/
//AlsaMidi *alsaMidi = new AlsaMidi();

/* TODO: MIDI streaming - currently ot in use - use alsa midi and Jack-Control above */

/* BT MIDI input client */
//MidiQbtClientIn bt1_midi_in;
/* BT channel 1 MIDI handler */
//MidiHandler midi_handler_bt_1(_MIDI_STAGE_2);
/* BT channel 1 MIDI connection BT-client <-> parser */
//MidiConnection midi_connectionn_bt1(&bt1_midi_in, 0, &midi_parser_bt_1, 0);
/* BT channel 1 MIDI parser <-> handler */
//MidiConnection midi_connection_bt_handler_1(&midi_parser_bt_1, 0, &midi_handler_bt_1, 0);

/* MIDI messages parsser */
//MidiParser midi_parser_bt_1(_MIDI_STAGE_1);

// TODO enable MIDI connections without Jack Control

// TODO: to enable channel seperation, parser should be able to collect data based on
//	channel ID number 

//MidiParser midiparser_bt_2(_MIDI_STAGE_1);
/* BT channel 1 MIDI handler */
//MidiHandler midihandler_bt_2(_MIDI_STAGE_2);
/* BT channel 1 MIDI connection BT-client <-> parser */
//MidiConnection midi_connectionn_bt2(bt1_midi_in, 0, midiparser_bt_2, 0);
/* BT channel 1 MIDI parser <-> handler */
//MidiConnection midi_connection_bt_andler_2(midiparser_bt_2, 0, midihandler_bt_2, 0);

//MidiParser midiparser_bt_3(_MIDI_STAGE_1);
/* BT channel 1 MIDI handler */
//MidiHandler midihandler_bt_3(_MIDI_STAGE_2);
/* BT channel 1 MIDI connection BT-client <-> parser */
//MidiConnection midi_connectionn_bt3(bt1_midi_in, 0, midiparser_bt_3, 0);
/* BT channel 1 MIDI parser <-> handler */
//MidiConnection midi_connection_bt_andler_3(midiparser_bt_3, 0, midihandler_bt_3, 0);

//MidiParser midiparser_bt_4(_MIDI_STAGE_1);
/* BT channel 1 MIDI handler */
//MidiHandler midihandler_bt_4(_MIDI_STAGE_2);
/* BT channel 1 MIDI connection BT-client <-> parser */
//MidiConnection midi_connectionn_bt4(bt1_midi_in, 0, midiparser_bt_4, 0);
/* BT channel 1 MIDI parser <-> handler */
//MidiConnection midi_connection_bt_andler_4(midiparser_bt_4, 0, midihandler_bt_4, 0);

// Callback that is initiated by the AudioManager audio - update thread.
void callback_audio_update_cycle_start_tasks_wrapper(int param)
{
	ModSynth::get_instance()->update_tasks(param);
}

// Settings callbacks
int set_audio_driver_type_cb(int drive, int dummy)
{
	return ModSynth::get_instance()->set_audio_driver_type(drive);	
}
		
int set_audio_block_size_cb(int size, int dummy)
{
	return ModSynth::get_instance()->set_audio_block_size(size);	
}	
	
int set_audio_sample_rate_cb(int rate, int dummy)
{
	return ModSynth::get_instance()->set_sample_rate(rate);	
}	
		


ModSynth::ModSynth()
{	
	mod_synth = this;
	int i, res;

	serial_port = SerialPort::get_serial_port_instance(16); // 16=ttyUSB0

	for (i = _PROGRAM_0; i < _PROGRAM_15; i++)
	{
		// Init all programs to synthersizer not assigned state
		midi_channel_synth[i] = _MIDI_CHAN_ASSIGNED_SYNTH_NONE;
	}
	// Init sketc-programs state to assigned to AdjSynth (this will never change)
	midi_channel_synth[_SKETCH_PROGRAM_1] = _MIDI_CHAN_ASSIGNED_SYNTH_ADJ;
	midi_channel_synth[_SKETCH_PROGRAM_2] = _MIDI_CHAN_ASSIGNED_SYNTH_ADJ;
	midi_channel_synth[_SKETCH_PROGRAM_3] = _MIDI_CHAN_ASSIGNED_SYNTH_ADJ;

	// TODO: Get the default settings directories from a file - currentlly hard coded below
	
	// ModSynth default settings directory
	string adj_synth_general_settings_file_path_name = "/home/pi/AdjHeartModSynth_2/Settings/ModSynth_general_settings_1.xml";
	// FluidSynth default settings directory
	// AdjSynth default patch settings directory
	// AdjSynth default settings directory
	
	// create the general setting manager (mange audio and midi settings)
	general_settings_manager = new ModSynthSettings(&active_general_synth_settings_params);
	// Create the FLuidSynth settings manager
	fluid_synth_settings_manager = new ModSynthSettings(&active_fluid_synth_settings_params);

	// Create a FluidSynth instace (initialized hereafter using init() ).
	fluid_synth = FluidSynthInterface::create_instance(fluid_synth_settings_manager, &active_fluid_synth_settings_params);
	// Create an AdjSynth instance.
	adj_synth =  AdjSynth::get_instance(); // new AdjSynth();
	
	set_audio_driver_type(_DEFAULT_AUDIO_DRIVER);
	set_audio_block_size(_DEFAULT_BLOCK_SIZE);
	set_sample_rate(_DEFAULT_SAMPLE_RATE);
	
	// Init Fluid and Adj synthesizers
	init();

	adj_synth->audio_manager->register_callback_audio_update_cycle_start_tasks(&callback_audio_update_cycle_start_tasks_wrapper);

	res = open_mod_synth_general_settings_file(adj_synth_general_settings_file_path_name);
	if (res != 0)
	{
		// File read failed - set to default settings parameters
		set_default_general_settings_parameters(&active_general_synth_settings_params);
	}

	// Init temp presets
	preset_temp.name = "Preset_Temp";
	preset_temp.settings_type = _PARAM_TYPE_MOD_SYNTH_PRESET;
	preset_temp.version = general_settings_manager->get_settings_version();
	set_default_preset_parameters(&preset_temp);
	
	ModSynthPresets::init();
	
	// Init the midi control mapper (maps midi control to synth controls)
	midi_control_mapper = new MidiControlMapper();

//	alsa_midi = AlsaMidi::get_instance();
	
	// Create a Bluetooth ALSA output Client.
	bt_alsa_out = AlsaBtClientOutput::get_instance();

	// Create a serial port ALSA output Client.
	serial_port_alsa_out = AlsaSerialPortClientOutput::get_instance();
	
	alsa_midi_sys_control = AlsaMidiSysControl::get_instance();
	
	jack_connections = JackConnections::get_instance();
	
	// Start the CPU utilization measuring thread.
	start_cheack_cpu_utilization_thread();
}

ModSynth::~ModSynth()
{
	fluid_synth->deinitialize_fluid_synthesizer();

	stop_cheack_cpu_utilization_thread();
}

/**
*   @brief  retruns the single modular synth instance
*
*   @param  none
*   @return the single modular synth instance
*/
ModSynth *ModSynth::get_instance()
{
	if (mod_synth == NULL)
	{
		mod_synth = new(ModSynth);
	}
	
	return mod_synth;
}

/**
*   @brief  Initializes the synthesizer engine.
*			Must be called first before any other call.
*   @param  none
*   @return void
*/
int ModSynth::init()
{
	settings_res_t res;	
	
	// Init the FluidSynth
	fluid_synth->initialize_fluid_synthesizer();
	
	// Assign the ModSynth general settings manager, active general settings settings parameters..
	adj_synth->set_settings_params(general_settings_manager, &active_general_synth_settings_params);
	// Create and initialize the ModSynth default General Settings parameters values.
	set_default_general_settings_parameters(&active_general_synth_settings_params);

	// Assign the AdjSynth settings manager, active settings parameters..
	adj_synth->set_settings_params(adj_synth->adj_synth_settings_manager,
								   adj_synth->get_active_settings_params());
	// Create and initialize the AdjSynth default Settings parameters values.
	set_adj_synth_default_settings(adj_synth->get_active_settings_params());
	
	// Init programs
	adj_synth->init_synth_programs(/*&active_adj_synth_patch*/);
	// Init voices
	adj_synth->init_synth_voices();
	// Init polyphony manager
	adj_synth->init_poly();
	// Inut JACK audio
	adj_synth->init_jack();
	// Start the audio dervice
//	adj_synth->start_audio();

	res = general_settings_manager->set_int_param(
											&active_general_synth_settings_params,
											"synth.master_volume",
											_DEFAULT_MASTER_VOLUME, 100, 0,
											_PARAM_TYPE_MOD_SYNTH_SETUP,
											NULL,	// no callbacks,
											0, 0, NULL,
											_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE,
											-1);

	set_fluid_synth_volume(_DEFAULT_FLUID_SYNTH_VOLUME);
	set_master_volume(_DEFAULT_MASTER_VOLUME);

// TODO:	fluid_synth_volume = settings->set_int_param
	return 0;
}

/**
*   @brief  Set the default settings callbacks.
*   @param  none
*   @return 0 if done OK
*/
settings_res_t ModSynth::set_callbacks()
{
	settings_res_t res = 0;

	res |= fluid_synth->set_callbacks();

	return res;
}

/**
*   @brief  Closing and cleanup when application goes down.
*			Must be called whenever terminating the application.
*   @param  none
*   @return void
*/
void ModSynth::on_exit()
{
	if (fluid_synth != NULL)
	{
		fluid_synth->deinitialize_fluid_synthesizer();
	}
	
	this->stop_cheack_cpu_utilization_thread();
//TODO: stop whatever else should be terminated
}

/**
*	@brief	Sets the audio driver type
*	@param	driver  _AUDIO_JACK, _AUDIO_ALSA (default: _DEFAULT_AUDIO)
*	@param	restart_audio if true set value and restart audio
*	@return set audio-driver
*/
int ModSynth::set_audio_driver_type(int driver, bool restart_audio)
{
	if (is_valid_audio_driver(driver))
	{
		audio_driver = driver;
	}
	else
	{
		audio_driver = _DEFAULT_AUDIO_DRIVER;
	}
	
	if (restart_audio)
	{
		stop_audio();
		usleep(100000);
		start_audio();
	}
	
	return audio_driver;
}

/**
*	@brief	Sets the audio driver type  value only (without restarting audio)
*			Settings will be effective only after the next call to start_audio().
*	@param	driver  _AUDIO_JACK, _AUDIO_ALSA (default: _DEFAULT_AUDIO)
*	@param	set_only if true only set value, false - set value and restart audio
*	@return set audio-driver
*/
int ModSynth::set_audio_driver_type(int driver)
{
	return set_audio_driver_type(driver, false);
}

/**
*	@brief	Returns the audio-driver type
*	@param	none
*	@return audio driver type
*/	
int ModSynth::get_audio_driver_type() { return audio_driver; }

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*	@param	restart_audio if true only restart audio
*					
*	@return set sample-rate
*/
int ModSynth::set_sample_rate(int samp_rate, bool restart_audio)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}
	
	//	adj_synth->set_sample_rate(sample_rate);
		// TODO: fluid_synth
	
	if(restart_audio)
	{
		stop_audio();
		usleep(100000);
		start_audio();
	}
	
	return sample_rate;
}

/**
*	@brief	Sets the sample-rate value only (without restarting audio)
*			Settings will be effective only after the next call to start_audio().
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/
int ModSynth::set_sample_rate(int samp_rate)
{
	return set_sample_rate(samp_rate, false);
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int ModSynth::get_sample_rate() { return sample_rate; }

/**
*   @brief  sets the audio block size 
*			Settings will be effective only after the next call to start_audio().
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @param	restart_audio if true restart audio
*   @return size if OK; -1 param out of range
*/
int ModSynth::set_audio_block_size(int size, bool restart_audio)
{	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;		
		
		//		adj_synth->set_audio_block_size(audio_block_size);
				// TODO: fluid_synth
		
		if(restart_audio)
		{
			stop_audio();
			usleep(100000);
			start_audio();
		}
		
		return audio_block_size;
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  sets the audio block size value only (without restarting audio)
*			Settings will be effective only after the next call to start_audio().
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return size if OK; -1 param out of range
*/
int ModSynth::set_audio_block_size(int size)
{
	return set_audio_block_size(size, false);
}

	
/**
*   @brief  retruns the audio block size  
*   @param  none
*   @return buffer size
*/	
int ModSynth::get_audio_block_size() { return audio_block_size; }

/**
*   @brief  Starts the audio service.
*			Must be called after setting the audio driver type, 
*			sample-rate and audio block-size (else, default values will be used)..
*   @param  none
*   @return 0 if done
*/
int ModSynth::start_audio()
{
	int res = 0;
	// TODO:: res = start FluidSynth audio handling
	res |= adj_synth->start_audio(audio_driver, sample_rate, audio_block_size);
	
	return res;
}

/**
*   @brief  Stops the audio service.
*   @param  none
*   @return 0 if done
*/
int ModSynth::stop_audio()
{
	int res = 0;
	// TODO:: res = stop FluidSynth audio handling
	res |= adj_synth->stop_audio();
	
	return res;
}


/**
*   @brief  Handle synthesizer events mapping to enable midi control keyboards mapping
*   @param  none
*   @return 0 if done
*/
int ModSynth::synth_event_mapping(int moduleId, int controlId, int val)
{
	callback_update_synth_controls_data(moduleId, controlId, val);
	
	midi_control_mapper->update_synth_control_data(moduleId, controlId);
	
	return 0;
}

/**
*   @brief  retruns the FluidSynth settings
*
*   @param  none
*   @return global settings
*/
_setting_params_t *ModSynth::get_fluid_synth_active_settings_params() { return &active_fluid_synth_settings_params; }

/**
*   @brief  retruns a pointer to the fluid synthesizr insytance
*   @param  none
*   @return pointer to fluid synthesizr
*/
FluidSynthInterface *ModSynth::get_fluid_synth()
{
	return fluid_synth;
}

/**
*   @brief  retruns a pointer to the AdjHeart synthesizr
*   @param  none
*   @return pointer to adjheart synthesizr
*/
AdjSynth *ModSynth::get_adj_synth()
{
	return adj_synth;
}

/**
*   @brief  performs periodic update tasks - called by the audio processing update cycle process
*   @param  voc	voice number (na)
*   @return pointer to adjheart synthesizr
*/
void ModSynth::update_tasks(int voc)
{
	if (adj_synth->kbd1->portamento_is_enabled())
	{
		adj_synth->kbd1->update_actual_frequency();
		AdjSynth::get_instance()->synth_voice[_SYNTH_VOICE_1]->dsp_voice->set_voice_frequency(adj_synth->kbd1->get_note_frequency());
	}
}

/**
*   @brief  retruns a pointer to the global bluetooth alsa midi out client
*   @param  none
*   @return the global bluetooth alsa midi out client
*/
AlsaBtClientOutput *ModSynth::get_bt_alsa_out()
{
	return bt_alsa_out;
}

/**
*   @brief  retruns a pointer to the serial port
*
*   @param  none
*   @return the global serial port
*/
SerialPort *ModSynth::get_serial_port()
{
	return serial_port;
}

/**
*   @brief  Set the general settings parameters (audio, midi) to their default values
*   @param	params	a _setting_params_t parameters struct
*   @return 0 if done
*/
int ModSynth::set_default_general_settings_parameters(_setting_params_t *params)
{
	int res = 0;

	res = set_default_settings_parameters_audio(params);
	// TODO: general volume, midi

	return res;
}

/**
*   @brief  Returns a pointer to the general settings params
*   @param	none
*   @return pointer to the general settings params
*/
_setting_params_t *ModSynth::get_active_general_settings_params()
{
	return &active_general_synth_settings_params;
}

/**
*   @brief  retruns a pointer to the alsa serial port midi out client
*   @param  none
*   @return a pointer to the alsa serial port midi out client
*/
AlsaSerialPortClientOutput *ModSynth::get_serial_port_alsa_out()
{
	return serial_port_alsa_out;
}

/**
*   @brief  retruns a pointer to the alsa midi system control instance
*   @param  none
*   @return a pointer to the alsa serial port midi out client
*/
AlsaMidiSysControl *ModSynth::get_alsa_midi_sys_control()
{
	return alsa_midi_sys_control;
}

/**
*   @brief  retruns a pointer to the jack clients connection control instance
*   @param  none
*   @return a pointer to the jack clients connection control instance
*/
JackConnections *ModSynth::get_jack_connection()
{
	return jack_connections;
}

/**
*   @brief  retruns a pointer to the alsa-midi object
*   @param  none
*   @return a pointer to the alsa-midi object
*/
//AlsaMidi* ModSynth::get_alsa_midi()
//{
//	return alsa_midi;
//}

/**
 * @brief Create all AdjSynth settings paramters and set them to their default values.?????
 * @param none
 * @return settings version
 */
int ModSynth::set_adj_synth_default_settings(_setting_params_t *params)
{
	AdjSynth::get_instance()->set_default_settings_parameters(params);
	return params->version;
}

/**
 * @brief Create all AdjSynth general settings paramters and set them to their default values.?????
 * @param none
 * @return settings version
 */
//int ModSynth::set_adj_synth_default_general_settings(_setting_params_t *params)
//{
//	adj_synth->set_default_general_settings_parameters(params);
//	return 0;
//}

/**
*   @brief  Save AdjSynth Patch parameters as XML file
*   @param  path settings XML file full path
*   @param	settings a pointer to a ModSynthSettings settings handling object
*   @param	param	a pointer to a _setting_params_t params structure
*	@param save_mask bit map to set saved params type #_SAVE_FLUID_SETTINGS, #_SAVE_ADJ_SYNTH_PATCH, _SAVE_ADJ_SYNTH_SETTINGS
*   @return 0 if done
*/
int ModSynth::save_adj_synth_patch_file(string path, ModSynthSettings *settings, _setting_params_t *params)
{
	int res;
	XML_files *xml_files = new XML_files();
	
	return_val_if_true(params == NULL || settings == NULL, _SETTINGS_BAD_PARAMETERS);

	res = settings->write_settings_file(
		params,
		settings->get_settings_version(),
		xml_files->get_xml_file_name(path),
		path,
		_SAVE_ADJ_SYNTH_PATCH);
	
	//	printf("Save settings to  %s\n", path.c_str());

		return res;
}

/**
*   @brief  Open an AdjSynth Patch parameters XML file and set it parameters
*   @param  path settings XML file full path
*   @param	settings a pointer to a ModSynthSettings settings handling object
*   @param	param	a pointer to a _setting_params_t params structure
*   @return 0 if done
*/
int ModSynth::open_adj_synth_patch_file(string path, ModSynthSettings *settings, _setting_params_t *params, int channel)
{
	settings_res_t res;
	
	return_val_if_true(params == NULL || settings == NULL, _SETTINGS_BAD_PARAMETERS);
	
	res = settings->read_settings_file(params, path, _READ_ADJ_SYNTH_PATCH, channel);

	if (res == _SETTINGS_OK)
	{
//		adj_synth->synth_program[channel]->set_program_patch_params(params);
		
		adj_synth->synth_program[channel]->synth_pad_creator->generate_wavetable(
			adj_synth->synth_program[channel]->program_wavetable);

		adj_synth->synth_program[channel]->mso_wtab->calc_segments_lengths(
			&adj_synth->synth_program[channel]->mso_wtab->morphed_segment_lengths, 
			&adj_synth->synth_program[channel]->mso_wtab->morphed_segment_positions);
		
		adj_synth->synth_program[channel]->mso_wtab->calc_wtab(
			adj_synth->synth_program[channel]->mso_wtab->morphed_waveform_tab, 
			&adj_synth->synth_program[channel]->mso_wtab->morphed_segment_lengths, 
			&adj_synth->synth_program[channel]->mso_wtab->morphed_segment_positions);
		
		//	printf("Open settings  %s\n", path.c_str());
		return 0;
	}
	else
	{
		return -1;
	}	

	return res;
}

/**
*   @brief  Save the active AdjSynth settings parameters as XML file
*   @param  path settings XML file full path
*   @param	settings a pointer to a ModSynthSettings settings handling object
*   @param	param	a pointer to a _setting_params_t params structure
*	@param save_mask bit map to set saved params type #_SAVE_FLUID_SETTINGS, #_SAVE_ADJ_SYNTH_PATCH, _SAVE_ADJ_SYNTH_SETTINGS
*   @return 0 if done
*/
int ModSynth::save_adj_synth_settings_file(string path, ModSynthSettings *settings, _setting_params_t *params)
{
	XML_files *xml_files = new XML_files();
	int res;
	
	return_val_if_true(params == NULL || settings == NULL, _SETTINGS_BAD_PARAMETERS);

	res = settings->write_settings_file(
		params, 
		settings->get_settings_version(),
		xml_files->get_xml_file_name(path),
		path,
		_SAVE_ADJ_SYNTH_SETTINGS);
	return res;
}

/**
*   @brief  Open an AdjSynth settings parameters XML file and set it as the active settings
*   @param  path settings XML file full path
*   @param	settings a pointer to a ModSynthSettings settings handling object
*   @param	param	a pointer to a _setting_params_t params structure
*   @return 0 if done
*/
int ModSynth::open_adj_synth_settings_file(string path, ModSynthSettings *settings, _setting_params_t *params)
{
	int res;
	
	return_val_if_true(params == NULL || settings == NULL, _SETTINGS_BAD_PARAMETERS);
	
	res = settings->read_settings_file(params, path, _READ_ADJ_SYNTH_SETTINGS);
	
	//	printf("Open settings  %s\n", path.c_str());

	return res;
}

/**
*   @brief  Save the active FluidSynth settings parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int ModSynth::save_fluid_synth_settings_file(string path)
{
	int res;
	
	return_val_if_true(fluid_synth_settings_manager == NULL, _SETTINGS_BAD_PARAMETERS);

	res = fluid_synth_settings_manager->write_settings_file(
		&active_fluid_synth_settings_params,
		fluid_synth_settings_manager->get_settings_version(),
		"",
		path,
		_SAVE_FLUID_SETTINGS);
	return res;
}

/**
*   @brief  Open FluidSynth settings parameters XML file and set it as the FluidSynth active settings
*   @param  path settings XML file full path
*   @return 0 if done
*/
int ModSynth::open_fluid_synth_settings_file(string path)
{	
	return_val_if_true(fluid_synth_settings_manager == NULL, _SETTINGS_BAD_PARAMETERS);
	
	XML_files *xml_files = new XML_files();
	
	int res = fluid_synth_settings_manager->read_settings_file(&active_fluid_synth_settings_params, path, _READ_FLUID_SETTINGS);
	fluid_synth->set_fluid_synth_chorus_params();
	fluid_synth->set_fluid_synth_reverb_params();
	if (res == 0)
	{
		callback_update_ui();
		callback_update_fluid_synth_settings_text(xml_files->remove_file_extention(xml_files->get_xml_file_name(path)));
	}

	return 0;
}
	
/**
*   @brief  Save the active AdjSynth patch parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int ModSynth::save_adj_synth_patch_file(string path)
{
	return save_adj_synth_patch_file(path, adj_synth->adj_synth_settings_manager, 
									 &adj_synth->synth_program[mod_synth_get_active_sketch()]->active_patch_params);
}

/**
*   @brief  Open settings parameters XML file and set it as the AdjSynth active patch parameters
*   @param  path settings XML file full path
*	@param	channel	midi channel (0-15, 16-18 for active patch).
*   @return 0 if done
*/
int ModSynth::open_adj_synth_patch_file(string path, int channel)
{
	return open_adj_synth_patch_file(path, adj_synth->adj_synth_settings_manager, 
									&adj_synth->synth_program[channel]->active_patch_params,
									channel);
}

/**
*   @brief  Save the active AdjSynth settings parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int ModSynth::save_adj_synth_settings_file(string path)
{
	return save_adj_synth_settings_file(path, adj_synth->adj_synth_settings_manager, 
										adj_synth->get_active_settings_params());
}

/**
*   @brief  Open settings parameters XML file and set it as the AdjSynth active settings parameters
*   @param  path settings XML file full path
*   @return 0 if done
*/
int ModSynth::open_adj_synth_settings_file(string path)
{
	return open_adj_synth_settings_file(path, adj_synth->adj_synth_settings_manager,
										adj_synth->get_active_settings_params());
}

/**
*   @brief  Save the active ModSynth general settings parameters as XML file
*   @param  path general settings XML file full path
*   @return 0 if done
*/
int ModSynth::save_mod_synth_general_settings_file(string path)
{
	return save_adj_synth_settings_file(path, general_settings_manager,
										&active_general_synth_settings_params);
}

/**
*   @brief  Open general settings parameters XML file and set it as the AdjSynth active settings parameters
*   @param  path settings XML file full path
*   @return 0 if done
*/
int ModSynth::open_mod_synth_general_settings_file(string path)
{
	return open_adj_synth_settings_file(path, general_settings_manager, 
										&active_general_synth_settings_params);
}


/**
*   @brief  Open a preset parameters XML file and set it as the ModSynth active preset parameters
*   @param  path settings XML file full path
*   @return 0 if done
*/
int ModSynth::open_mod_synth_preset_file(
	string path, 
	_setting_params_t *settings,
	mod_synth_preset_t *preset, 
	string *summary)
{
	int res = -1;
	
	return_val_if_true(presets == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(settings == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);
	
	mod_synth_preset_t *presets = NULL;

	res = general_settings_manager->read_settings_file(
		settings,
		path,
		_READ_MOD_SYNTH_PRESET);
	
	if (res == _SETTINGS_OK)
	{
		presets = get_synth_temp_preset_params();
		if (presets != NULL)
		{			
			ModSynthPresets::generate_summary(presets, summary);			
			ModSynthPresets::set_synth_settings(presets);
		}
		
		ModSynthPresets::copy_presets(presets, preset);
	}
	
	return res;
}

/**
*   @brief  Save a preset parameters as XML file
*   @param  path settings XML file full path
*	@param  a pointer to a modSynthPreset_t struct holding the preset parameters
*   @return 0 if done
*/
int ModSynth::save_mod_synth_preset_file(string path)
{
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);

	XML_files *xml_files = new XML_files();
	
	int res = 0;

	res = collect_mod_synth_preset_parms(&preset_temp);
	if (res != 0)
	{
		return -1;
	}
	
	preset_temp.name = xml_files->get_xml_file_name(path);

	res = general_settings_manager->write_settings_file(
		&preset_temp,
		general_settings_manager->get_settings_version(),
		xml_files->get_xml_file_name(path),
		path,
		_SAVE_MOD_SYNTH_PRESET);

	return res;
}


/**
*   @brief  Clear all playing notes and polyphonic allocations
*   @param  none
*   @return void
*/
void ModSynth::synth_panic_action()
{
	fluid_synth->fluid_synth_panic_action();
}

/**
*   @brief  Scan and build MIDI active ports
*   @param  none
*   @return 0 if OK; -1 if failed
*/
int ModSynth::scan_midi_ports()
{
	//return alsa_midi->scan_midi_ports();
}

/**
*   @brief  Enable direct transfer of bluetooth midi events.
*   @param  none
*   @return void
*/
void ModSynth::connect_bt_midi()
{
//	midi_handler_bt_1.connect_bt_midi();
}

/**
*   @brief  Disable direct transfer of bluetooth midi events.
*   @param  none
*   @return void
*/
void ModSynth::disconnect_bt_midi()
{
//	midi_handler_bt_1.disconnect_bt_midi();
}

/**
*   @brief  Set the master volume level.
*   @param  vol volume level (0-100)
*   @return none
*/
void ModSynth::set_master_volume(int vol)
{
	double gain;
	settings_res_t res;

	// for correct limit testing, synth.master_volume param must be already intialized
	res = general_settings_manager->set_int_param(
											&active_general_synth_settings_params,
											"synth.master_volume",
											vol,
											100,
											0, _PARAM_TYPE_MOD_SYNTH_SETUP,
											NULL, _SET_VALUE);

	if (res == _SETTINGS_OK)
	{
		master_volume = vol;
		// Update all other volume levels
		// Fluid synth - max gain 0.25
		gain = (float)(fluid_synth_volume*master_volume) / 40000.f;
		fluid_synth->set_fluid_synth_gain(gain);

		AdjSynth::get_instance()->set_master_volume(vol);
	}
}

/**
*   @brief  Get the master volume level.
*   @param  none
*   @return volume level (0-100)
*/
int ModSynth::get_master_volume() { return master_volume; }

/**
*   @brief  Set the fluid synth volume level.
*   @param  vol volume level (0-100)
*   @return none
*/
void ModSynth::set_fluid_synth_volume(int vol)
{
	// 0-0.25
	double gain = (float)(vol*master_volume) / 40000.f; // 100 * 100 / 40000 = 0.25

	settings_res_t res = fluid_synth->set_fluid_synth_gain(gain);
	// set gain also verifies range
	if (res == _SETTINGS_OK)
	{
		fluid_synth_volume = vol;			
	}
}

/**
*   @brief  Get the fluid synth volume level.
*   @param  none
*   @return volume level (0-100)
*/
int ModSynth::get_fluid_synth_volume() { return fluid_synth_volume; }

/**
*   @brief  Get the midi channel assigned synthesizer.
*   @param  chan	midi channel number 0-15
*   @return channel associated synthesizer id
*/
int ModSynth::get_midi_channel_synth(int chan)
{
	if ((chan >= 0) && (chan <= 15))
	{
		return midi_channel_synth[chan];
	}
	else
	{
		return 0;
	}
}

/**
*   @brief  Set the midi channel assigned synthesizer.
*   @param  chan	midi channel number 0-15
*	@parm	synth	synthesizer id
*   @return void
*/
void ModSynth::set_midi_channel_synth(int chan, int synth)
{
	if ((chan >= 0) && (chan <= 15))
	{
		 midi_channel_synth[chan] = synth;
	}
}


void ModSynth::start_cheack_cpu_utilization_thread()
{
	cheack_cpu_utilization_thread_is_running = true;
	pthread_create(&cheack_cpu_utilization_thread_id, NULL, cheack_cpu_utilization_thread, NULL);
	pthread_setname_np(cheack_cpu_utilization_thread_id, "cpuutilthread");
}

void ModSynth::stop_cheack_cpu_utilization_thread()
{
	cheack_cpu_utilization_thread_is_running = false;
//	pthread_create(&cheack_cpu_utilization_thread_id, NULL, cheackCpuUtilizationThread, NULL);
//	pthread_setname_np(cheack_cpu_utilization_thread_id, "cpuutilthread");
}

/**
*   @brief  Execute a mapped control.
*   @param  ch	midi channel number 0-15
*	@param	control_num	synthesizer control code
*	@param	val	control value
*   @return void
*/
int ModSynth::execute_mapped_control(int ch, int control_num, int val)
{
	synth_control_t synth_event;
	int res;

	// see if this event is mapped
	res = midi_control_mapper->get_mapped_synth_event(
									&synth_event,
									midi_control_mapper->get_midi_map(),
									midi_control_mapper->get_midi_map_nc(),
									ch, control_num,
									midi_control_mapper->get_midi_control_sequences_use_channel_state());
	if (res == 0)
	{
		// Execute on mappes synth module and control
		execute_synth_control(synth_event.module_id, synth_event.control_id, val);
	}
	
	return res;
}


void ModSynth::note_on(uint8_t channel, uint8_t note, uint8_t velocity)
{
	int res;

	fprintf(stderr, "modSynth noteOn %i chan %i \n", note, channel);
	
	if (mod_synth_get_active_midi_mapping_mode() == _MIDI_MAPPING_MODE_SKETCH)		
	{
		AdjSynth::get_instance()->midi_play_note_on(channel, note, velocity);
	}
	else if (AdjSynth::get_instance()->midi_mapping_mode == _MIDI_MAPPING_MODE_MAPPING)
	{
		if (get_midi_channel_synth(channel) == _MIDI_CHAN_ASSIGNED_SYNTH_ADJ)
		{
			AdjSynth::get_instance()->midi_play_note_on(channel, note, velocity);
		}
		else if (get_midi_channel_synth(channel) == _MIDI_CHAN_ASSIGNED_SYNTH_FLUID)
		{
			velocity = (uint8_t)(float(velocity) * program_level_1[channel]);
			res = ModSynth::get_instance()->get_fluid_synth()->fluid_synth_note_on_event(channel, note, velocity);
		}
	}
}

void ModSynth::note_off(uint8_t channel, uint8_t note)
{
	int res;

	fprintf(stderr, "modSynth off %i  \n", note);
	if (mod_synth_get_active_midi_mapping_mode() == _MIDI_MAPPING_MODE_SKETCH)

	{
		AdjSynth::get_instance()->midi_play_note_on(channel, note, 0);
	}
	else if (AdjSynth::get_instance()->midi_mapping_mode == _MIDI_MAPPING_MODE_MAPPING)
	{
		if (get_midi_channel_synth(channel) == _MIDI_CHAN_ASSIGNED_SYNTH_ADJ)
		{
			AdjSynth::get_instance()->midi_play_note_on(channel, note, 0);
		}
		else if (get_midi_channel_synth(channel) == _MIDI_CHAN_ASSIGNED_SYNTH_FLUID)
		{
			res = ModSynth::get_instance()->get_fluid_synth()->fluid_synth_note_on_event(channel, note, 0);
		}
	}
}

void ModSynth::change_program(uint8_t channel, uint8_t program) {

	fprintf(stderr, "modSynth channel %i\n", channel);
	if (get_midi_channel_synth(channel) == _MIDI_CHAN_ASSIGNED_SYNTH_ADJ)
	{
		// TODO:
	}
	else if (get_midi_channel_synth(channel) == _MIDI_CHAN_ASSIGNED_SYNTH_FLUID)
	{
		ModSynth::get_instance()->get_fluid_synth()->fluid_synth_program_change_event(channel, program);
	}
}


void ModSynth::channel_pressure(uint8_t channel, uint8_t val)
{
	if (get_midi_channel_synth(channel) == _MIDI_CHAN_ASSIGNED_SYNTH_ADJ)
	{

	}
	else if (get_midi_channel_synth(channel) == _MIDI_CHAN_ASSIGNED_SYNTH_FLUID)
	{	
		ModSynth::get_instance()->get_fluid_synth()->fluid_synth_channel_pressure_event(channel, val);
	}
}


void ModSynth::controller_event(uint8_t channel, uint8_t num, uint8_t val)
{
	fprintf(stderr, "modSynth conroller channel %i  number %i  value %i\n", channel, num, val);
	// Handle control mapping	
	
	if (get_midi_channel_synth(channel) == _MIDI_CHAN_ASSIGNED_SYNTH_ADJ)
	{

	}
	else if (get_midi_channel_synth(channel) == _MIDI_CHAN_ASSIGNED_SYNTH_FLUID)
	{
		ModSynth::get_instance()->get_fluid_synth()->fluid_synth_controller_event(channel, num, val);
	}
}

void ModSynth::pitch_bend(uint8_t channel, int pitch)
{
	fprintf(stderr, "modSynth pitch bend channel %i  pitch %i\n", channel, pitch);
	if (get_midi_channel_synth(channel) == _MIDI_CHAN_ASSIGNED_SYNTH_ADJ)
	{

	}
	else if (get_midi_channel_synth(channel) == _MIDI_CHAN_ASSIGNED_SYNTH_FLUID)
	{
		ModSynth::get_instance()->get_fluid_synth()->fluid_synth_pitch_bend_event(channel, pitch + 8192);
	}
}

void *cheack_cpu_utilization_thread(void *arg)
{
	sleep(1);
	while (cheack_cpu_utilization_thread_is_running)
	{
		CPUSnapshot previousSnap;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		CPUSnapshot curSnap;

		const float ACTIVE_TIME = curSnap.GetActiveTimeTotal() - previousSnap.GetActiveTimeTotal();
		const float IDLE_TIME = curSnap.GetIdleTimeTotal() - previousSnap.GetIdleTimeTotal();
		const float TOTAL_TIME = ACTIVE_TIME + IDLE_TIME;
		ModSynth::cpu_utilization = (int)(100.f * ACTIVE_TIME / TOTAL_TIME);
	}

	return NULL;
}

