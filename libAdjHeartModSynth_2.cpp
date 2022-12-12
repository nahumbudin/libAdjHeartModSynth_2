/**
* @file		libAdjHeartModSynth_1.h
*	@author		Nahum Budin
*	@date		23-Jan-2021
*	@version	2.0
*		1. Code refactoring
*		2. Adding local midi devices handling (not through jack connection kit)
*		3. Adding local JACK clients handling (not through jack connection kit)
*		4. Seperating JACK auto connection of audio and midi
*		5. Adding start/stop audio
*/

#include <stdio.h>
#include <string>

#include "libAdjHeartModSynth_2.h"

#include "../synthesizer/modSynth.h"

#include "../midi/midiStream.h"

#include "jack/jackAudioClients.h"

#include "../utils/log.h"
#include "../utils/utils.h"

ModSynthSettings *settings_manager;

// Modular synthesizer main object 
ModSynth *mod_synth; // = new ModSynth();

// Callbacks functions pointers
func_ptr_void_void_t callback_ptr_update_ui = NULL;
func_ptr_void_void_t callback_ptr_update_midi_devices_combo = NULL;
func_ptr_void_int_t callback_ptr_mark_voice_not_busy = NULL;
func_ptr_void_int_t callback_ptr_mark_voice_busy = NULL;

func_ptr_void_string_t callback_ptr_update_ui_status_text = NULL;
func_ptr_void_string_t callback_ptr_update_fluid_synth_settings_text = NULL;

func_ptr_void_int_t callback_ptr_update_utilization_bar = NULL;
func_ptr_void_int_t callback_ptr_update_left_level = NULL;
func_ptr_void_int_t callback_ptr_update_right_level = NULL;

func_ptr_void_void_t callback_ptr_update_ui_bt_connection_status_active = NULL;
func_ptr_void_void_t callback_ptr_update_ui_bt_connection_status_not_active = NULL;
func_ptr_void_void_t callback_ptr_update_ui_jack_connection_status_active = NULL;
func_ptr_void_void_t callback_ptr_update_ui_jack_connection_status_not_active = NULL;

func_ptr_void_float_ptr_float_ptr_int_t callback_ptr_update_signal_display = NULL;

func_ptr_void_int_t callback_ptr_message_id = NULL;
func_ptr_void_int_t callback_ptr_set_osc1_unison_labales_callback = NULL;

func_ptr_void_int_int_t callback_ptr_midi_program_change_event = NULL;

func_ptr_void_int_int_int_t callback_ptr_update_midi_control_sequnces_data = NULL;
func_ptr_void_int_int_int_t callback_ptr_update_synth_controls_data = NULL;

func_ptr_void_int_t callback_ptr_initiate_update_gui = NULL;

settings_res_t res;
mod_synth_settings_str_param_t string_param;
mod_synth_settings_int_param_t int_param;
mod_synth_settings_float_param_t float_param;
mod_synth_settings_bool_param_t bool_param;

int mod_synth_init()
{
	// Debuger
	FILELog::ReportingLevel() = FILELog::FromString("DEBUG4");
	FILE *pFile = fopen("AdjHeartModSynth_1_0.log", "a");
	Output2FILE::Stream() = pFile;
	FILE_LOG(logINFO) << "...";
	
	mod_synth = ModSynth::get_instance(); // new ModSynth();

	return 0;
}

int mod_synth_start_audio()
{
	return mod_synth->start_audio();
}

int mod_synth_stop_audio()
{
	return mod_synth->stop_audio();
}

int mod_synth_init_bt_services()
{
	/* Inilize */
	Raspi3BluetoothQ *bluetooth = Raspi3BluetoothQ::get_instance();
	bluetooth->start_bt_main_thread();
	mod_synth->get_bt_alsa_out()->start_bt_alsa_out_thread();
	return 0;
}

int mod_synth_deinit_bt_services()
{
	Raspi3BluetoothQ::stop_bt_main_thread();
	mod_synth->get_bt_alsa_out()->stop_bt_alsa_out_thread();
	return 0;
}

int mod_synth_init_serial_port_services(int portNum, int baud, const char *mode, int flowCtl)
{
	int res;

	res = mod_synth->get_serial_port()->open_port(portNum, baud, mode, flowCtl);
	if (res != 0)
	{
		return -1;
	}

	mod_synth->get_serial_port_alsa_out()->start_alsa_serial_port_out_thread();

	return 0;
}

int mod_synth_deinit_serial_port_services()
{
	res = mod_synth->get_serial_port()->close_port();
	mod_synth->get_serial_port()->stop_serial_port_RX_thread();

	mod_synth->get_serial_port_alsa_out()->stop_alsa_serial_port_out_thread();

	return 0;
}

int mod_synth_init_midi_services() 
{
	// Allocate midi blocks data memory pool
	allocate_midi_stream_messages_memory_pool(_MAX_MIDI_STREAM_MESSAGES_POOL_SIZE);
	allocate_raw_data_mssgs_memory_pool(_MAX_RAWDATA_MSSGS_POOL_SIZE);
	MidiStream::start_thread();
	

	return 0;
}

int mod_synth_deinit_midi_services()
{
	MidiStream::stop_thread();

	return 0;
}

void mod_synth_connect_bt_to_midi()
{
	mod_synth->connect_bt_midi();
}

void mod_synth_disconnect_bt_to_midi()
{
	mod_synth->disconnect_bt_midi();
}

void mod_synth_on_exit()
{
	//	jackExit(); ???
	mod_synth_deinit_bt_services();
	mod_synth_deinit_midi_services();
	mod_synth->get_fluid_synth()->deinitialize_fluid_synthesizer();

	mod_synth->get_adj_synth()->audio_manager->stop_audio_service();
}

int mod_synth_get_synthesizer_num_of_polyphonic_voices() 
{ 
	return AdjSynth::get_instance()->get_num_of_voices(); 
}

int mod_synth_get_synthesizer_num_of_programs() 
{ 
	return AdjSynth::get_instance()->get_num_of_programs(); 
}

int mod_synth_get_number_of_cores() 
{ 
	if (AdjSynth::num_of_cores > 1) 
	{	
		return AdjSynth::num_of_cores;
	}
	else 
	{
		return 1; 
	}
}

int mod_synth_get_cpu_utilization() 
{ 
	return ModSynth::cpu_utilization; 
}

int mod_synth_set_audio_driver(int driver)
{
	return ModSynth::get_instance()->set_audio_driver_type(driver, true); // set and restart audio
}

int mod_synth_get_audio_driver() // TODO:
{ 
	return ModSynth::get_instance()->get_audio_driver_type();
}

int mod_synth_get_jack_mode() 
{ 
	return get_jack_mode(); 
}

int mod_synth_set_audio_sample_rate(int rate) 
{  
	return ModSynth::get_instance()->set_sample_rate(rate, true); // set and restart audio
}

int mod_synth_get_audio_sample_rate() 
{  
	return ModSynth::get_instance()->get_sample_rate(); 
}

int mod_synth_set_audio_block_size(int size) 
{ 
	return ModSynth::get_instance()->set_audio_block_size(size, true); // set and restart audio
}

int mod_synth_get_audio_block_size() 
{ 
	return ModSynth::get_instance()->get_audio_block_size(); 
}

bool mod_synth_set_jack_auto_start_state(bool state) 
{ 
	if (state)
	{
		enable_jack_auto_start();
	}
	else
	{
		disable_jack_auto_start();
	}
	
	return get_jack_auto_start_state(); 
}
bool mod_synth_get_jack_auto_start_state() 
{ 
	return get_jack_auto_start_state(); 
}

bool mod_synth_set_jack_auto_connect_audio_state(bool state) 
{ 
	if (state)
	{
		enable_jack_auto_connect_audio();
	}
	else
	{
		disable_jack_auto_connect_audio();
	}
	
	return get_jack_auto_connect_audio_state(); 
}

bool mod_synth_get_jack_auto_connect_audio_state() 
{ 
	return get_jack_auto_connect_audio_state(); 
}

bool mod_synth_get_jack_auto_connect_midi_state() 
{ 
	return get_jack_auto_connect_midi_state(); 
}


int mod_synth_refresh_jack_clients_data()
{
	return ModSynth::get_instance()->get_jack_connection()->refresh_jack_clients_data();
}
	
int mod_synth_get_num_of_input_jack_clients()
{
	return ModSynth::get_instance()->get_jack_connection()->get_num_of_input_jack_clients();
}
	
int mod_synth_get_num_of_output_jack_clients()
{
	return ModSynth::get_instance()->get_jack_connection()->get_num_of_output_jack_clients();
}	
	
int mod_synth_get_num_of_jack_input_client_ports(int cln)
{
	return ModSynth::get_instance()->get_jack_connection()->get_num_of_jack_input_client_ports(cln);
}
	
int mod_synth_get_num_of_jack_output_client_ports(int cln)
{
	return ModSynth::get_instance()->get_jack_connection()->get_num_of_jack_output_client_ports(cln);
}
	
std::string mod_synth_get_jack_input_client_name(int cln)
{
	return ModSynth::get_instance()->get_jack_connection()->get_jack_input_client_name(cln);
}
	
std::string mod_synth_get_jack_output_client_name(int cln)
{
	return ModSynth::get_instance()->get_jack_connection()->get_jack_output_client_name(cln);
}
		
std::string mod_synth_get_jack_input_client_port_name(int cln, int prt)
{
	return ModSynth::get_instance()->get_jack_connection()->get_jack_input_client_port_name(cln, prt);
}
	
std::string mod_synth_get_jack_output_client_port_name(int cln, int prt)
{
	return ModSynth::get_instance()->get_jack_connection()->get_jack_output_client_port_name(cln, prt);
}

int mod_synth_get_num_of_jack_clients_connections()
{
	return ModSynth::get_instance()->get_jack_connection()->get_num_of_jack_clients_connections();
}

std::string mod_synth_get_jack_connection_in_client_name(int con_num)
{
	return ModSynth::get_instance()->get_jack_connection()->get_jack_connection_in_client_name(con_num);
}

std::string mod_synth_get_jack_connection_in_client_port_name(int con_num)
{
	return ModSynth::get_instance()->get_jack_connection()->get_jack_connection_in_client_port_name(con_num);
}

std::string mod_synth_get_jack_connection_out_client_name(int con_num)
{
	return ModSynth::get_instance()->get_jack_connection()->get_jack_connection_out_client_name(con_num);
}

std::string mod_synth_get_jack_connection_out_client_port_name(int con_num)
{
	return ModSynth::get_instance()->get_jack_connection()->get_jack_connection_out_client_port_name(con_num);
}

int mod_synth_connect_jack_connection(
	std::string in_client_name,
	std::string in_client_port_name,
	std::string out_client_name,
	std::string out_client_port_name)
{
	return ModSynth::get_instance()->get_jack_connection()->connect_jack_connection(
		in_client_name,
		in_client_port_name,
		out_client_name,
		out_client_port_name);
}

int mod_synth_disconnect_jack_connection(
	std::string in_client_name,
	std::string in_client_port_name,
	std::string out_client_name,
	std::string out_client_port_name)
{
	return ModSynth::get_instance()->get_jack_connection()->disconnect_jack_connection(
		in_client_name,
		in_client_port_name,
		out_client_name,
		out_client_port_name);
}

int mod_synth_disconnect_all_jack_connections()
{
	return ModSynth::get_instance()->get_jack_connection()->disconnect_all_jack_connections();
}



void mod_synth_set_active_sketch(int ask)
{
	AdjSynth::get_instance()->set_active_sketch(ask);
}

int mod_synth_get_active_sketch()
{
	return AdjSynth::get_instance()->get_active_sketch();
}

int mod_synth_copy_sketch(int src_sk, int dest_sk)
{
	return AdjSynth::get_instance()->copy_sketch(src_sk, dest_sk);
}

int mod_synth_enable_midi_control_sequences_training()
{
	return ModSynth::get_instance()->midi_control_mapper->enable_midi_control_sequences_training();
}

int mod_synth_disable_midi_control_sequences_training()
{
	return ModSynth::get_instance()->midi_control_mapper->disable_midi_control_sequences_training();
}

bool mod_synth_get_midi_control_sequences_training_enabled()
{
	return ModSynth::get_instance()->midi_control_mapper->get_midi_control_sequences_training_state();
}

void mod_synth_set_midi_control_sequences_use_channel(bool use)
{
	ModSynth::get_instance()->midi_control_mapper->set_midi_control_sequences_use_channel(use);
}

bool mod_synth_get_midi_control_sequences_use_channel_state()
{
	return ModSynth::get_instance()->midi_control_mapper->get_midi_control_sequences_use_channel_state();
}

int mod_synth_map_control_sequence_event()
{
	return ModSynth::get_instance()->midi_control_mapper->add_mapped_midi_control_sequence_event();
}

int mod_synth_save_midi_keyboard_control_map_file(string path)
{
	return ModSynth::get_instance()->midi_control_mapper->save_midi_keyboard_control_map_file(path);
}

int mod_synth_load_midi_keyboard_control_map_file(string path)
{
	return ModSynth::get_instance()->midi_control_mapper->load_midi_keyboard_control_map_file(path);
}

int mod_synth_set_in_focus_module_control(int module, int control, int type)
{
	return ModSynth::get_instance()->midi_control_mapper->set_in_focus_module_control(module, control, type);
}

int mod_synth_get_in_focus_module_id()
{
	return ModSynth::get_instance()->midi_control_mapper->get_in_focus_module_id();
}

int mod_synth_get_in_focus_control_id()
{
	return ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_id();
}

int mod_synth_get_in_focus_control_type()
{
	return ModSynth::get_instance()->midi_control_mapper->get_in_focus_control_type();
}

int mod_synth_scan_midi_ports()
{
	return ModSynth::get_instance()->scan_midi_ports();
}

int mod_synth_connect_midi_device(int dev, int in_out)
{
	if (in_out == _MIDI_DEVICE_INPUT)
	{
//		return ModSynth::get_instance()->get_alsa_midi()->connect_device(dev, _MIDI_DEVICE_INPUT);
	}
	else if (in_out == _MIDI_DEVICE_OUTPUT)
	{
//		return ModSynth::get_instance()->get_alsa_midi()->connect_device(dev, _MIDI_DEVICE_OUTPUT);
	}
	
	return -1;
}

int mod_synth_disconnect_midi_device(int dev, int in_out)
{
	if (in_out == _MIDI_DEVICE_INPUT)
	{
//		return ModSynth::get_instance()->get_alsa_midi()->disconnect_device(dev, _MIDI_DEVICE_INPUT);
	}
	else if (in_out == _MIDI_DEVICE_OUTPUT)
	{
//		return ModSynth::get_instance()->get_alsa_midi()->disconnect_device(dev, _MIDI_DEVICE_OUTPUT);
	}
	
	return -1;
}



int mod_synth_refresh_alsa_midi_clients_data()
{
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->refresh_alsa_clients_data();
}


int mod_synth_connect_midi_clients(int in_client, int in_port, int out_client, int out_port)
{
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->connect_midi_clients(
		in_client,
		in_port,
		out_client,
		out_port);
}

int mod_synth_disconnect_midi_clients(int in_client, int in_port, int out_client, int out_port)
{
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->disconnect_midi_clients(
		in_client,
		in_port,
		out_client,
		out_port);
}

int mod_synth_disconnect_all_midi_clients()
{
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->disconnect_all_midi_clients();
}

int mod_synth_get_num_of_input_midi_clients()
{
	//	return ModSynth::get_instance()->get_alsa_midi()->get_total_num_of_midi_devices();
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->get_num_of_input_midi_clients();
}

int mod_synth_get_num_of_output_midi_clients()
{
	//	return ModSynth::get_instance()->get_alsa_midi()->get_total_num_of_midi_devices();
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->get_num_of_output_midi_clients();
}

int mod_synth_get_num_of_input_midi_client_ports(int cln)
{
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->get_num_of_input_midi_client_ports(cln);	
}

int mod_synth_get_num_of_output_midi_client_ports(int cln)
{
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->get_num_of_output_midi_client_ports(cln);	
}

void mod_synth_get_midi_input_client_name_string(int cln, std::string* name)
{
	ModSynth::get_instance()->get_alsa_midi_sys_control()->get_midi_input_client_name_string(cln, name);	
}

void mod_synth_get_midi_output_client_name_string(int cln, std::string* name)
{
	ModSynth::get_instance()->get_alsa_midi_sys_control()->get_midi_output_client_name_string(cln, name);	
}

void mod_synth_get_midi_input_client_port_name_string(int cln, int port, std::string* name)
{
	ModSynth::get_instance()->get_alsa_midi_sys_control()->get_midi_input_client_port_name_string(cln, port, name);	
}

void mod_synth_get_midi_output_client_port_name_string(int cln, int port, std::string* name)
{
	ModSynth::get_instance()->get_alsa_midi_sys_control()->get_midi_output_client_port_name_string(cln, port, name);		
}

int mod_synth_get_num_of_midi_clients_connections()
{
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->get_num_of_midi_clients_connections();
}

int mod_synth_get_midi_client_connection_connected_client_num(int con_num)
{
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->get_midi_client_connection_connected_client_num(con_num);
}

int mod_synth_get_midi_client_connection_connected_port_num(int con_num)
{
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->get_midi_client_connection_connected_port_num(con_num);
}

int mod_synth_get_midi_client_connection_connecting_to_client_num(int con_num)
{
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->get_midi_client_connection_connecting_to_client_num(con_num);
}

int mod_synth_get_midi_client_connection_connecting_to_port_num(int con_num)
{
	return ModSynth::get_instance()->get_alsa_midi_sys_control()->get_midi_client_connection_connecting_to_port_num(con_num);
}




void mod_synth_get_midi_device_string(int dev, char* name)
{
//	ModSynth::get_instance()->get_alsa_midi()->get_midi_device_string(dev, name);
}

void mod_synth_get_midi_device_name(int dev, char* name)
{
//	ModSynth::get_instance()->get_alsa_midi()->get_midi_device_name(dev, name);
}

void mod_synth_get_midi_device_sub_name(int dev, char* sub_name)
{
//	ModSynth::get_instance()->get_alsa_midi()->get_midi_device_sub_name(dev, sub_name);
}

void mod_synth_set_master_volume(int vol)
{
	mod_synth->set_master_volume(vol);	
	mod_synth->synth_event_mapping(_MOD_SYNTH_EVENT, _MASTER_VOLUME, vol);
}

void mod_synth_set_fluid_synth_volume(int vol)
{
	mod_synth->set_fluid_synth_volume(vol);
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_VOLUME, vol);
}

void mod_synth_set_fluid_synth_reverb_room_size(int val)
{
	// 0-1.0
	int res;
	double fsize = (float)(val) / 100.f;

	res = mod_synth->get_fluid_synth()->set_fluid_synth_reverb_room_size_value(fsize);
	res |= mod_synth->get_fluid_synth()->set_fluid_synth_reverb_params();
	
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_REVERB_ROOM_SIZE, val);
}

void mod_synth_set_fluid_synth_reverb_damp(int val)
{
	int res = 0;

	// 0-1.0
	double fdamp = (float)(val) / 100.f;

	res = mod_synth->get_fluid_synth()->set_fluid_synth_reverb_damp_value(fdamp);
	res |= mod_synth->get_fluid_synth()->set_fluid_synth_reverb_params();
	
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_REVERB_DAMP, val);
}

void mod_synth_set_fluid_synth_reverb_width(int val)
{
	int res = 0;

	// 0-1.0 (not 0-100.0)
	double fwidth = (float)(val) / 100.0;

	res = mod_synth->get_fluid_synth()->set_fluid_synth_reverb_width_value(fwidth);
	res |= mod_synth->get_fluid_synth()->set_fluid_synth_reverb_params();
	
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_REVERB_WIDTH, val);
}

void mod_synth_set_fluid_synth_reverb_level(int val)
{
	int res = 0;
	
	// 0-1.0
	double flevel = (float)(val) / 100.f;
	
	res = mod_synth->get_fluid_synth()->set_fluid_synth_reverb_level_value(flevel);
	res |= mod_synth->get_fluid_synth()->set_fluid_synth_reverb_params();
	
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_REVERB_LEVEL, val);
}


void mod_synth_set_fluid_synth_chorus_number(int val)
{
	int res = 0;

	// number of stages 0-99
	res = mod_synth->get_fluid_synth()->set_fluid_synth_chorus_N_value(val);
	res |= mod_synth->get_fluid_synth()->set_fluid_synth_chorus_params();
	
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_CHORUS_NUMBER, val);
}

void mod_synth_set_fluid_synth_chorus_level(int val)
{
	int res = 0;

	// 0-10.0
	double flevel = (float)(val) / 10.f;

	res = mod_synth->get_fluid_synth()->set_fluid_synth_chorus_level_value(flevel);
	res |= mod_synth->get_fluid_synth()->set_fluid_synth_chorus_params();
	
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_CHORUS_LEVEL, val);
}

void mod_synth_set_fluid_synth_chorus_speed(int val)
{
	int res = 0;

	// 0.29-5.0
	double fspeed = (float)(val) / 100.f;

	res = mod_synth->get_fluid_synth()->set_fluid_synth_chorus_speed_value(fspeed);
	res |= mod_synth->get_fluid_synth()->set_fluid_synth_chorus_params();
	
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_CHORUS_SPEED, val);
}

void mod_synth_set_fluid_synth_chorus_depth(int val)
{
	int res = 0;

	// 0-21.0
	double fdepth = (float)(val) / 10.f;

	res = mod_synth->get_fluid_synth()->set_fluid_synth_chorus_depth_value(fdepth);
	res |= mod_synth->get_fluid_synth()->set_fluid_synth_chorus_params();
	
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_CHORUS_DEPTH, val);
}

void mod_synth_set_fluid_synth_chorus_waveform(int val)
{
	int res = 0;
	
	res = mod_synth->get_fluid_synth()->set_fluid_synth_chorus_waveform_value(val);
	res |= mod_synth->get_fluid_synth()->set_fluid_synth_chorus_params();
	
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_CHORUS_WAVEFORM, val);
}


void mod_synth_set_fluid_synth_enable_reverb()
{
	mod_synth->get_fluid_synth()->enable_fluid_synth_reverb();
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_REVERB_ENABLE, 127);
}

void mod_synth_set_fluid_synth_disable_reverb()
{
	mod_synth->get_fluid_synth()->disable_fluid_synth_reverb();
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_REVERB_ENABLE, 0);
}


void mod_synth_set_fluid_synth_enable_chorus()
{
	mod_synth->get_fluid_synth()->enable_fluid_synth_chorus();
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_CHORUS_ENABLE, 127);
}

void mod_synth_set_fluid_synth_disable_chorus()
{
	mod_synth->get_fluid_synth()->disable_fluid_synth_chorus();
	mod_synth->synth_event_mapping(_FLUID_SYNTH_1_EVENT, _FLUID_SYNTH_CHORUS_ENABLE, 0);
}

void mod_synth_set_fluid_synth_soundfont(std::string path)
{
	int res = 0;
	
	res = mod_synth->get_fluid_synth()->load_and_set_fluid_synth_soundfont(path);
}

int mod_synth_set_fluid_synth_program_select(int chan,
	unsigned int sfid,
	unsigned int bank,
	unsigned int program)
{
	return mod_synth->get_fluid_synth()->set_fluid_synth_program_select(chan, sfid, bank, program);
}

void mod_synth_set_midi_channel_synth(int chan, int synth)
{
	mod_synth->set_midi_channel_synth(chan, synth);
}

int mod_synth_get_midi_channel_synth(int chan)
{
	return mod_synth->get_midi_channel_synth(chan);
}

void mod_synth_audio_event_int(int audid, int eventid, int val)
{
	mod_synth->get_adj_synth()->audio_event_int(audid,
		eventid,
		val,
		mod_synth->get_active_general_settings_params(), 
		-1);
}

void mod_synth_audio_event_bool(int audid, int eventid, bool val)
{
	mod_synth->get_adj_synth()->audio_event_bool(audid,
		eventid,
		val,
		mod_synth->get_active_general_settings_params(),
		-1);
}

void mod_synth_vco_event(int vcoid, int eventid, int val)
{
	mod_synth->get_adj_synth()->vco_event(vcoid,
		eventid,
		val, 
		mod_synth->get_adj_synth()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	mod_synth->synth_event_mapping(vcoid, eventid, val);
}

void mod_synth_vco_event_bool(int vcoid, int eventid, bool val)
{
	mod_synth->get_adj_synth()->vco_event_bool(vcoid,
		eventid,
		val, 
		mod_synth->get_adj_synth()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	if (val)
	{
		mod_synth->synth_event_mapping(vcoid, eventid, 127);
	}
	else
	{
		mod_synth->synth_event_mapping(vcoid, eventid, 0);
	}
}


void mod_synth_mso_event(int msoid, int eventid, int val)
{
	mod_synth->get_adj_synth()->mso_event(msoid,
		eventid,
		val, 
		mod_synth->get_adj_synth()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	mod_synth->synth_event_mapping(msoid, eventid, val);
}

void mod_synth_noise_event(int noiseid, int eventid, int val)
{
	mod_synth->get_adj_synth()->noise_event(noiseid,
		eventid,
		val, 
		mod_synth->get_adj_synth()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	mod_synth->synth_event_mapping(noiseid, eventid, val);
}

void mod_synth_karplus_event(int karpid, int eventid, int val)
{
	mod_synth->get_adj_synth()->karplus_event(karpid,
		eventid,
		val, 
		mod_synth->get_adj_synth()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	mod_synth->synth_event_mapping(karpid, eventid, val);
}

void mod_synth_pad_event(int padid, int eventid, int val)
{
	mod_synth->get_adj_synth()->pad_event(padid,
		eventid,
		val, 
		mod_synth->get_adj_synth()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	mod_synth->synth_event_mapping(padid, eventid, val);
}


void mod_synth_filter_event(int filtid, int eventid, int val)
{
	mod_synth->get_adj_synth()->filter_event(filtid,
		eventid,
		val, 
		mod_synth->get_adj_synth()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	mod_synth->synth_event_mapping(filtid, eventid, val);
}


void mod_synth_amp_event(int ampid, int eventid, int val)
{
	mod_synth->get_adj_synth()->amp_event(ampid,
		eventid,
		val, 
		mod_synth->get_adj_synth()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	mod_synth->synth_event_mapping(ampid, eventid, val);
}

void mod_synth_midi_mixer_event(int mixid, int eventid, int val)
{
	mod_synth->get_adj_synth()->midi_mixer_event(mixid, eventid, val, mod_synth->get_adj_synth()->get_active_settings_params());
	
	mod_synth->synth_event_mapping(mixid, eventid, val);
}

void mod_synth_reverb_event(int rvbid, int eventid, int val)
{
	mod_synth->get_adj_synth()->reverb_event(rvbid,
		eventid,
		val,
		mod_synth->get_adj_synth()->get_active_settings_params());
	
	mod_synth->synth_event_mapping(rvbid, eventid, val);
}

void mod_synth_reverb_event_bool(int rvbid, int eventid, bool val)
{
	mod_synth->get_adj_synth()->reverb_event_bool(rvbid,
		eventid,
		val,
		mod_synth->get_adj_synth()->get_active_settings_params());
	
	if (val)
	{
		mod_synth->synth_event_mapping(rvbid, eventid, 127);
	}
	else
	{
		mod_synth->synth_event_mapping(rvbid, eventid, 0);
	}
}

void mod_synth_distortion_event(int distid, int eventid, int val)
{
	mod_synth->get_adj_synth()->distortion_event(distid,
		eventid,
		val, 
		mod_synth->get_adj_synth()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	mod_synth->synth_event_mapping(distid, eventid, val);
}

void mod_synth_distortion_event_bool(int distid, int eventid, bool val)
{
	mod_synth->get_adj_synth()->distortion_event_bool(distid,
		eventid,
		val, 
		mod_synth->get_adj_synth()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	if (val)
	{
		mod_synth->synth_event_mapping(distid, eventid, 127);
	}
	else
	{
		mod_synth->synth_event_mapping(distid, eventid, 0);
	}
}

void mod_synth_band_equalizer_event(int beqid, int eventid, int val)
{
	mod_synth->get_adj_synth()->band_equilizer_event(beqid, eventid, val, mod_synth->get_adj_synth()->get_active_settings_params());
	
	mod_synth->synth_event_mapping(beqid, eventid, val);
}

void mod_synth_band_equalizer_event_bool(int beqid, int eventid, bool val)
{
	//	Synthesizer::getInstance()->bandEquilizerEventBool(beqid, eventid, val);
}

void mod_synth_modulator_event(int modid, int eventid, int val)
{
	mod_synth->get_adj_synth()->modulator_event(modid,
		eventid,
		val, 
		mod_synth->get_adj_synth()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	mod_synth->synth_event_mapping(modid, eventid, val);
}


void mod_synth_keyboard_event(int kbdid, int eventid, int val)
{
	AdjSynth::get_instance()->kbd_event(kbdid, eventid, val, mod_synth->get_adj_synth()->get_active_settings_params());
	
	mod_synth->synth_event_mapping(kbdid, eventid, val);
}

void mod_synth_keyboard_event_bool(int kbdid, int eventid, bool val)
{
	AdjSynth::get_instance()->kbd_event_bool(kbdid, eventid, val, mod_synth->get_adj_synth()->get_active_settings_params());
	
	if (val)
	{
		mod_synth->synth_event_mapping(kbdid, eventid, 127);
	}
	else
	{
		mod_synth->synth_event_mapping(kbdid, eventid, 0);
	}
}

void mod_synth_enable_osc1() 
{ 
	AdjSynth::get_instance()->vco_event_bool(_OSC_1_EVENT,
		_OSC_ENABLE,
		_ENABLE, 
		AdjSynth::get_instance()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	mod_synth->synth_event_mapping(_OSC_1_EVENT, _OSC_ENABLE, 127);
}

void mod_synth_disable_osc1() 
{ 
	AdjSynth::get_instance()->vco_event_bool(_OSC_1_EVENT,
		_OSC_ENABLE,
		_DISABLE, 
		AdjSynth::get_instance()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch()); 
	
	mod_synth->synth_event_mapping(_OSC_1_EVENT, _OSC_ENABLE, 0);
}

void mod_synth_enable_osc2() 
{ 
	AdjSynth::get_instance()->vco_event_bool(_OSC_2_EVENT,
		_OSC_ENABLE,
		_ENABLE, 
		AdjSynth::get_instance()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch()); 
	
	mod_synth->synth_event_mapping(_OSC_2_EVENT, _OSC_ENABLE, 127);
}

void mod_synth_disable_osc2() 
{ 
	AdjSynth::get_instance()->vco_event_bool(_OSC_2_EVENT,
		_OSC_ENABLE,
		_DISABLE, 
		AdjSynth::get_instance()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());
	
	mod_synth->synth_event_mapping(_OSC_1_EVENT, _OSC_ENABLE, 0);
}

void mod_synth_enable_noise() 
{ 
	AdjSynth::get_instance()->noise_event_bool(_NOISE_1_EVENT,
		_NOISE_ENABLE,
		_ENABLE, 
		AdjSynth::get_instance()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());

	mod_synth->synth_event_mapping(_NOISE_1_EVENT, _NOISE_ENABLE, 127);
}

void mod_synth_disable_noise() 
{ 
	AdjSynth::get_instance()->noise_event_bool(_NOISE_1_EVENT,
		_NOISE_ENABLE,
		_DISABLE, 
		AdjSynth::get_instance()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());

	mod_synth->synth_event_mapping(_NOISE_1_EVENT, _NOISE_ENABLE, 0);
}

void mod_synth_enable_karplus() 
{
	AdjSynth::get_instance()->karplus_event_bool(_KARPLUS_1_EVENT,
		_KARPLUS_STRONG_ENABLE,
		_ENABLE,
		AdjSynth::get_instance()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());

	mod_synth->synth_event_mapping(_KARPLUS_1_EVENT, _KARPLUS_STRONG_ENABLE, 127);
}

void mod_synth_disable_karplus() 
{ 
	AdjSynth::get_instance()->karplus_event_bool(_KARPLUS_1_EVENT,
		_KARPLUS_STRONG_ENABLE,
		_DISABLE,
		AdjSynth::get_instance()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());

	mod_synth->synth_event_mapping(_KARPLUS_1_EVENT, _KARPLUS_STRONG_ENABLE, 0);

}
//void mod_synth_enable_drawbars() { AdjSynth::get_instance()->drawbars_event(_DRAWBARS_1_EVENT, _DRAWBARS_ENABLE, _ENABLE, 
//								AdjSynth::get_instance()->get_active_patch_params()); }
//void mod_synth_disable_drawbars() { AdjSynth::get_instance()->drawbars_event(_DRAWBARS_1_EVENT, _DRAWBARS_ENABLE, _DISABLE,
//								AdjSynth::get_instance()->get_active_patch_params()); }) *  /; }

void mod_synth_enable_morphsin() 
{ 
	AdjSynth::get_instance()->mso_event_bool(_MSO_1_EVENT,
		_MSO_ENABLE,
		_ENABLE, 
		AdjSynth::get_instance()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());

	mod_synth->synth_event_mapping(_MSO_1_EVENT, _MSO_ENABLE, 127);
}

void mod_synth_disable_morphsin() 
{ 
	AdjSynth::get_instance()->mso_event_bool(_MSO_1_EVENT,
		_MSO_ENABLE,
		_DISABLE, 
		AdjSynth::get_instance()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());

	mod_synth->synth_event_mapping(_MSO_1_EVENT, _MSO_ENABLE, 0);
}

void mod_synth_enable_pad_synth() 
{ 
	AdjSynth::get_instance()->pad_event_bool(_PAD_1_EVENT,
		_PAD_ENABLE,
		_ENABLE, 
		AdjSynth::get_instance()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());

	mod_synth->synth_event_mapping(_PAD_1_EVENT, _PAD_ENABLE, 127);
}

void mod_synth_disable_pad_synth() 
{ 
	AdjSynth::get_instance()->pad_event_bool(_PAD_1_EVENT,
		_PAD_ENABLE,
		_DISABLE, 
		AdjSynth::get_instance()->get_active_patch_params(),
		mod_synth->get_adj_synth()->get_active_sketch());

	mod_synth->synth_event_mapping(_PAD_1_EVENT, _PAD_ENABLE, 0);
}

void mod_synth_update_mso_main_wtab() 
{ 
	/*Synthesizer::getInstance()->updateMsoLUTs(Synthesizer::getInstance()->programsMsoLUT[0]); */
}

float *mod_synth_get_pad_base_harmony_profile() 
{ 
	return AdjSynth::get_instance()->synth_program[mod_synth_get_active_sketch()]->synth_pad_creator->get_profile_data(); 
}

int mod_synth_get_pad_base_harmony_profile_size() 
{
	return AdjSynth::get_instance()->synth_program[mod_synth_get_active_sketch()]->synth_pad_creator->get_profile_size();
}

float *mod_synth_get_pad_spectrum()  
{ 
	return AdjSynth::get_instance()->synth_program[mod_synth_get_active_sketch()]->synth_pad_creator->get_spectrum_data();
}

int mod_synth_get_pad_spectrum_size() 
{ 
	return AdjSynth::get_instance()->synth_program[mod_synth_get_active_sketch()]->synth_pad_creator->get_spectrum_size();
}

int mod_synth_save_fluid_synth_settings_file(string path)
{
	return mod_synth->save_fluid_synth_settings_file(path);
}

int mod_synth_open_fluid_synth_settings_file(string path)
{
	return mod_synth->open_fluid_synth_settings_file(path);
}

int mod_synth_save_adj_synth_patch_file(string path)
{
	return mod_synth->save_adj_synth_patch_file(path);
}

int mod_synth_open_adj_synth_patch_file(std::string path, int channel)
{
	return mod_synth->open_adj_synth_patch_file(path, channel);
}

std::string mod_synth_get_program_patch_name(int prog)
{
	return AdjSynth::get_instance()->get_program_patch_name(prog);
}

int mod_synth_save_adj_synth_settings_file(string path)
{
	return mod_synth->save_adj_synth_settings_file(path);
}

int mod_synth_open_adj_synth_settings_file(std::string path)
{
	return mod_synth->open_adj_synth_settings_file(path);
}

int mod_synth_save_mod_synth_general_settings_file(string path)
{
	return mod_synth->save_mod_synth_general_settings_file(path);
}

int mod_synth_open_mod_synth_general_settings_file(std::string path)
{
	return mod_synth->open_mod_synth_general_settings_file(path);
}

int mod_synth_save_mod_synth_preset_file(string path)
{
	return mod_synth->save_mod_synth_preset_file(path);
}

int mod_synth_open_mod_synth_preset_file(string path, int preset_num)
{
	return mod_synth->open_mod_synth_preset_file(
		path, 
		&mod_synth->preset_temp, 
		ModSynthPresets::get_synth_preset_params(preset_num), 
		ModSynthPresets::get_synth_preset_summary_text(preset_num));
}

int mod_synth_get_mod_synth_active_preset_num()
{
	return ModSynthPresets::get_active_preset();
}

string *mod_synth_get_preset_summary_string(int preset_num)
{
	if ((preset_num >= 0) || (preset_num < _NUM_OF_PRESETS))
	{
		return ModSynthPresets::get_synth_preset_summary_text(preset_num);
	}
	else
	{
		return NULL;
	}
}


void mod_synth_set_mod_synth_active_preset_num(int preset_num)
{
	if ((preset_num >= 0) || (preset_num < _NUM_OF_PRESETS))
	{
		ModSynthPresets::set_active_preset(preset_num);
		ModSynthPresets::set_synth_settings(ModSynthPresets::get_synth_preset_params(preset_num));
	}		
}

int mod_synth_get_active_master_volume() 
{
	return mod_synth->get_master_volume();
}

int mod_synth_get_active_fluid_synth_volume()
{
	return mod_synth->get_fluid_synth_volume();
}

int mod_synth_get_active_fluid_synth_reverb_room_size()
{
	fluid_res_t res = 0;
	
	mod_synth_settings_float_param_t param;

	res = mod_synth->fluid_synth_settings_manager->get_float_param(mod_synth->get_fluid_synth_active_settings_params(), "fluid.synth.reverb.room-size", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-1.0 -> 0-100
	return (int)(param.value * 100.0);
}

int mod_synth_get_active_fluid_synth_reverb_damp()
{
	fluid_res_t res = 0;
	mod_synth_settings_float_param_t param;
	
	res = mod_synth->fluid_synth_settings_manager->get_float_param(mod_synth->get_fluid_synth_active_settings_params(), "fluid.synth.reverb.damp", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-1.0 -> 0-100
	return (int)(param.value * 100);
}

int mod_synth_get_active_fluid_synth_reverb_width()
{
	fluid_res_t res = 0;
	mod_synth_settings_float_param_t param;

	res = mod_synth->fluid_synth_settings_manager->get_float_param(mod_synth->get_fluid_synth_active_settings_params(), "fluid.synth.reverb.width", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-1.0 -> 0-100
	return (int)(param.value * 100);
}

int mod_synth_get_active_fluid_synth_reverb_level()
{
	fluid_res_t res = 0;
	mod_synth_settings_float_param_t param;

	res = mod_synth->fluid_synth_settings_manager->get_float_param(mod_synth->get_fluid_synth_active_settings_params(), "fluid.synth.reverb.level", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-1.0 -> 0-100
	return (int)(param.value * 100);
}


int mod_synth_get_active_fluid_synth_chorus_number()
{
	fluid_res_t res = 0;
	mod_synth_settings_int_param_t param;

	res = mod_synth->fluid_synth_settings_manager->get_int_param(mod_synth->get_fluid_synth_active_settings_params(), "fluid.synth.chorus.nr", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-99
	return param.value;
}

int mod_synth_get_active_fluid_synth_chorus_level()
{
	fluid_res_t res = 0;
	mod_synth_settings_float_param_t param;

	res = mod_synth->fluid_synth_settings_manager->get_float_param(mod_synth->get_fluid_synth_active_settings_params(), "fluid.synth.chorus.level", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-10.0 -> 0-100
	return (int)(param.value * 10);
}

int mod_synth_get_active_fluid_synth_chorus_speed()
{
	fluid_res_t res = 0;
	mod_synth_settings_float_param_t param;

	res = mod_synth->fluid_synth_settings_manager->get_float_param(mod_synth->get_fluid_synth_active_settings_params(), "fluid.synth.chorus.speed", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0.29-5.0 -> 29-500
	return (int)(param.value * 100);
}

int mod_synth_get_active_fluid_synth_chorus_depth()
{
	fluid_res_t res = 0;
	mod_synth_settings_float_param_t param;
	
	res = mod_synth->fluid_synth_settings_manager->get_float_param(mod_synth->get_fluid_synth_active_settings_params(), "fluid.synth.chorus.depth", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0-21.0 -> 0-210
	return (int)(param.value * 10);
}

int mod_synth_get_active_fluid_synth_chorus_waveform()
{
	fluid_res_t res = 0;
	mod_synth_settings_int_param_t param;

	res = mod_synth->fluid_synth_settings_manager->get_int_param(mod_synth->get_fluid_synth_active_settings_params(), "fluid.synth.chorus.waveform", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}
	// 0, 1
	return param.value;
}


bool mod_synth_get_active_fluid_synth_reverb_activation_state()
{
	fluid_res_t res = 0;
	mod_synth_settings_bool_param_t param;

	res = mod_synth->fluid_synth_settings_manager->get_bool_param(mod_synth->get_fluid_synth_active_settings_params(), "fluid.synth.reverb.active", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return false;
	}

	return param.value;
}

bool mod_synth_get_active_fluid_synth_chorus_activation_state()
{
	fluid_res_t res = 0;
	mod_synth_settings_bool_param_t param;

	res = mod_synth->fluid_synth_settings_manager->get_bool_param(mod_synth->get_fluid_synth_active_settings_params(), "fluid.synth.chorus.active", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	return param.value;
}

int mod_synth_get_active_midi_mapping_mode()
{
	return AdjSynth::get_instance()->get_midi_mapping_mode();
}

void mod_synth_set_active_midi_mapping_mode(int mode)
{
	AdjSynth::get_instance()->set_midi_mapping_mode(mode);
}

std::string mod_synth_get_fluid_synth_soundfont()
{
	fluid_res_t res = 0;
	mod_synth_settings_str_param_t param;

	res = mod_synth->fluid_synth_settings_manager->get_string_param(mod_synth->get_fluid_synth_active_settings_params(), "fluid.synth.soundfont-file", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return "SoundFont file not selected!";
	}
	
	return param.value;
}

int mod_synth_get_fluid_synth_channel_sfont_id(int chan)
{
	if ((chan < 0) || (chan > 15))
		return -1;

	return mod_synth->get_fluid_synth()->get_fluid_synth_channel_sfont_id(chan);
}

int mod_synth_get_fluid_synth_channel_bank(int chan)
{
	if ((chan < 0) || (chan > 15))
		return -1;

	return mod_synth->get_fluid_synth()->get_fluid_synth_channel_bank(chan);
}

int mod_synth_get_fluid_synth_channel_program(int chan)
{
	if ((chan < 0) || (chan > 15))
		return -1;

	return mod_synth->get_fluid_synth()->get_fluid_synth_channel_program(chan);
}

std::string mod_synth_get_fluid_synth_channel_preset_name(int chan)
{
	if ((chan < 0) || (chan > 15))
		return "";

	return mod_synth->get_fluid_synth()->get_fluid_synth_channel_preset_name(chan);
}

std::string mod_synth_get_fluid_synth_channel_preset_soundfont_name(int chan)
{
	string full_path;
	
	if ((chan < 0) || (chan > 15))
		return "";

	full_path = mod_synth->get_fluid_synth()->get_fluid_synth_channel_preset_soundfont_name(chan);

	// Extract file name only
	int pos2 = full_path.size();
	int pos1 = full_path.find_last_of("/", pos2);
	if ((pos2 - pos1) >= 1)
	{
		return full_path.substr(pos1 + 1, pos2);
	}
	else
	{
		return "";
	}
}

std::string mod_synth_get_fluid_synth_soundfont_id_name(int sfid)
{
	string full_path;

	full_path = mod_synth->get_fluid_synth()->get_fluid_synth_soundfont_id_name(sfid);

	// Extract file name only
	int pos2 = full_path.size();
	int pos1 = full_path.find_last_of("/", pos2);
	if ((pos2 - pos1) >= 1)
	{
		return full_path.substr(pos1 + 1, pos2);
	}
	else
	{
		return "";
	}
}

int mod_synth_get_num_of_fluid_synth_loaded_soundfonts_files()
{
	return mod_synth->get_fluid_synth()->get_num_of_fluid_synth_loaded_soundFonts_files();
}

int mod_synth_get_fluid_synth_sound_font_presets(int sfnum, std::vector<_soundfont_presets_data_t> *presets)
{
	return mod_synth->get_fluid_synth()->get_fluid_synth_sound_font_presets(sfnum, presets);
}

int mod_synth_get_fluid_synth_loaded_soundfonts_presets(std::vector<_soundfont_presets_data_t> *presets)
{
	return mod_synth->get_fluid_synth()->get_fluid_synth_loaded_sound_fonts_presets(presets);
}

void mod_synth_panic_action()
{
	mod_synth->synth_panic_action();

	mod_synth->get_adj_synth()->init_poly();
}

// Utils
float mod_synth_log_scale_100_float(float min, float max, float base, int val)
{
	return Utils::calc_log_scale_100_float(min, max, base, val);
}

int mod_synth_log_scale_100_int(int min, int max, int base, int val)
{
	return Utils::calc_log_scale_100_float(min, max, base, val);
}


// Callback Functions Registration

void register_callback_update_ui(func_ptr_void_void_t ptr)
{
	callback_ptr_update_ui = ptr;
}

void register_callback_update_midi_devices_combo(func_ptr_void_void_t ptr)
{
	callback_ptr_update_midi_devices_combo = ptr;
}

void register_callback_mark_voice_not_busy(func_ptr_void_int_t ptr)
{
	callback_ptr_mark_voice_not_busy = ptr;
}

void register_callback_mark_voice_busy(func_ptr_void_int_t ptr)
{
	callback_ptr_mark_voice_busy = ptr;
}

void register_callback_update_ui_status_text(func_ptr_void_string_t ptr)
{
	callback_ptr_update_ui_status_text = ptr;
}

void register_callback_update_fluid_synth_settings_text(func_ptr_void_string_t ptr)
{
	callback_ptr_update_fluid_synth_settings_text = ptr;
}

void register_callback_update_utilization_bar(func_ptr_void_int_t ptr)
{
	callback_ptr_update_utilization_bar = ptr;
}

void register_callback_update_left_level(func_ptr_void_int_t ptr)
{
	callback_ptr_update_left_level = ptr;
}

void register_callback_update_right_level(func_ptr_void_int_t ptr)
{
	callback_ptr_update_right_level = ptr;
}

void register_callback_update_ui_bt_connection_status_active(func_ptr_void_void_t ptr)
{
	callback_ptr_update_ui_bt_connection_status_active = ptr;
}

void register_callback_update_ui_bt_connection_status_not_active(func_ptr_void_void_t ptr)
{
	callback_ptr_update_ui_bt_connection_status_not_active = ptr;
}

void registerCallbackUpdateUiJackConnectionStatusActive(func_ptr_void_void_t ptr)
{
	callback_ptr_update_ui_jack_connection_status_active = ptr;
}

void registerCallbackUpdateUiJackConnectionStatusNotActive(func_ptr_void_void_t ptr)
{
	callback_ptr_update_ui_jack_connection_status_not_active = ptr;
}

void register_callback_midi_program_change_event(func_ptr_void_int_int_t ptr)
{
	callback_ptr_midi_program_change_event = ptr;
}

void register_callback_update_signal_display(func_ptr_void_float_ptr_float_ptr_int_t ptr)
{
	callback_ptr_update_signal_display = ptr;
}

void registerCallbackMessageId(func_ptr_void_int_t ptr)
{
	callback_ptr_message_id = ptr;
}


void register_set_osc1_unison_mode_callback(func_ptr_void_int_t ptr)
{
	callback_ptr_set_osc1_unison_labales_callback = ptr;
}

void register_update_midi_control_sequnces_data_callback(func_ptr_void_int_int_int_t ptr)
{
	callback_ptr_update_midi_control_sequnces_data = ptr;
}

void register_update_synth_controls_data_callback(func_ptr_void_int_int_int_t ptr)
{
	callback_ptr_update_synth_controls_data = ptr;
}

void register_initiate_update_gui_callback(func_ptr_void_int_t ptr)
{
	callback_ptr_initiate_update_gui = ptr;
}

// Callback Functions Activation

void callback_update_ui()
{
	if (callback_ptr_update_ui)
	{	
		callback_ptr_update_ui();
	}
}

void callback_update_midi_devices_combo()
{
	if (callback_ptr_update_midi_devices_combo)
		(*callback_ptr_update_midi_devices_combo)();
}

void callback_mark_voice_busy(int voice)
{
	if (callback_ptr_mark_voice_busy)
	{
		(*callback_ptr_mark_voice_busy)(voice);
	}
}

void callback_mark_voice_not_busy(int voice)
{
	if (callback_ptr_mark_voice_not_busy)
	{
		(*callback_ptr_mark_voice_not_busy)(voice);
	}
}

void callback_update_ui_status_text(string txt)
{
	if (callback_ptr_update_ui_status_text)
	{	
		callback_ptr_update_ui_status_text(txt);
	}
}

void callback_update_fluid_synth_settings_text(string txt)
{
	if (callback_ptr_update_fluid_synth_settings_text != NULL)
	{	
		callback_ptr_update_fluid_synth_settings_text(txt);
	}
}

void callback_update_ui_bt_connection_status_active()
{
	if (callback_ptr_update_ui_bt_connection_status_active != NULL)
	{	
		callback_ptr_update_ui_bt_connection_status_active();
	}
}

void callback_update_ui_bt_connection_status_not_active()
{
	if (callback_ptr_update_ui_bt_connection_status_not_active != NULL)
	{	
		callback_ptr_update_ui_bt_connection_status_not_active();
	}
}

void callback_update_ui_jack_connection_status_active()
{
	if (callback_ptr_update_ui_jack_connection_status_active != NULL)
	{	
		callback_ptr_update_ui_jack_connection_status_active();
	}
}

void callback_update_ui_jack_connection_status_not_active()
{
	if (callback_ptr_update_ui_jack_connection_status_not_active != NULL)
	{	
		callback_ptr_update_ui_jack_connection_status_not_active();
	}
}

void callback_midi_program_change_event(int channel, int program)
{
	if (callback_ptr_midi_program_change_event != NULL)
	{	callback_ptr_midi_program_change_event(channel, program);
	}
}

void callback_update_utilization_bar(int util)
{
	if (callback_ptr_update_utilization_bar)
	{	
		(*callback_ptr_update_utilization_bar)(util);
	}
}

void callback_update_left_level(int lev)
{
	if (callback_ptr_update_left_level)
	{	
		(*callback_ptr_update_left_level)(lev);
	}
}

void callback_update_right_level(int lev)
{
	if (callback_ptr_update_right_level)
	{	
		(*callback_ptr_update_right_level)(lev);
	}
}

void callback_update_signal_display(float *sigL, float *sigR, int size)
{
	if (callback_ptr_update_signal_display)
	{	
		(*callback_ptr_update_signal_display)(sigL, sigR, size);
	}
}

void callback_message_id(int mssgid)
{
	if (callback_ptr_message_id)
	{	
		(*callback_ptr_message_id)(mssgid);
	}
}

void callback_set_osc1_unison_mode(int mode)
{
	if (callback_ptr_set_osc1_unison_labales_callback)
	{
		(*callback_ptr_set_osc1_unison_labales_callback)(mode);
	}
}

void callback_update_midi_control_sequnces_data(int ch, int controlNum, int val)
{
	if (callback_ptr_update_midi_control_sequnces_data != NULL)
	{	
		callback_ptr_update_midi_control_sequnces_data(ch, controlNum, val);
	}
}

void callback_update_synth_controls_data(int moduleId, int controlId, int val)
{
	if (callback_ptr_update_synth_controls_data != NULL)
	{
		callback_ptr_update_synth_controls_data(moduleId, controlId, val);
	}	
}

void callback_initiate_update_gui(int moduleId)
{
	if (callback_ptr_initiate_update_gui != NULL)
	{
		callback_ptr_initiate_update_gui(moduleId);
	}
}

