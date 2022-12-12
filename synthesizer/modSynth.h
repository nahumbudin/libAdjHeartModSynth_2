/**
*	@file		modSynth.h
*	@author		Nahum Budin
*	@date		7-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*					2. Moving general settings manager (audio, midi) from AdjSynth to Mod Synth
*					3. Adding sample-rate and audio block-size settings
*					4. Adding ALSA MIDI
*					5. Adding start/stop audio
*	
*	@brief		This is the main modular synthesizer libraray object.
*
*	History:\n
*	
*	version 1.0		5-Oct-2019:		First version
*		
*/

#include <string>

#include "fluidSynthInterface.h"
#include "adjSynth.h"
#include "modSynthPreset.h"
#include "../bluetooth/rspiBluetoothServicesQueuesVer.h"
//#include "../midi/btClientMidiOutput.h"
#include "../midi/midiControlMapper.h"
#include "../alsa/alsaBtClientOutput.h"
#include "../alsa/alsaSerialPortClientOutput.h"
#include "../serialPort/serialPort.h"
#include "../alsa/alsaMidi.h"
#include "../alsa/alsaMidiSystemControl.h"

#include "../jack/jackConnections.h"

#ifndef _MODSYNTH
#define _MODSYNTH

//class AdjSynth;

enum mod_synth_type
{
	SYNTH_TYPE_FLUID,
	SYNTH_TYPE_ADDITIVE,
	SYNTH_TYPE_KARPLUS,			// Karplus Strong String
	SYNTH_TYPE_MSO,				// Modified Sinus 
	SYNTH_TYPE_PAD				// PAD Synth http://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htm#c_implementation	
};

void callback_audio_update_cycle_start_tasks_wrapper(int param);


class ModSynth
{
public:
	
	ModSynth();
	~ModSynth();
	static ModSynth *get_instance();

	int init();
	int set_callbacks();
	void on_exit();	
	
	int set_audio_driver_type(int driver, bool restart_audio);
	int set_audio_driver_type(int driver);
	int get_audio_driver_type();
	
	int set_sample_rate(int samp_rate, bool restart_audio);
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size, bool restart_audio);
	int set_audio_block_size(int size);
	int get_audio_block_size();
	
	int start_audio();
	int stop_audio();
	
	int synth_event_mapping(int module_id, int control_id, int val);

	_setting_params_t *get_fluid_synth_active_settings_params();


	FluidSynthInterface *get_fluid_synth();
	AdjSynth *get_adj_synth();

	void update_tasks(int voc);		
	
	AlsaBtClientOutput *get_bt_alsa_out();
	AlsaSerialPortClientOutput *get_serial_port_alsa_out();

	//AlsaMidi* get_alsa_midi();
	
	AlsaMidiSysControl *get_alsa_midi_sys_control();
	
	JackConnections *get_jack_connection();

	SerialPort *get_serial_port();
	
	int set_default_general_settings_parameters(_setting_params_t *params);
	int set_default_settings_parameters_audio(_setting_params_t *params);
	_setting_params_t *get_active_general_settings_params();
	
	int set_adj_synth_default_settings(_setting_params_t *params);
	//int set_adj_synth_default_general_settings(_setting_params_t *params);

	int save_adj_synth_patch_file(string path, ModSynthSettings *settings, _setting_params_t *params);
	int open_adj_synth_patch_file(string path, ModSynthSettings *settings, _setting_params_t *params, int channel);
	
	int save_adj_synth_settings_file(string path, ModSynthSettings *settings, _setting_params_t *params);
	int open_adj_synth_settings_file(string path, ModSynthSettings *settings, _setting_params_t *params);
	
	int save_fluid_synth_settings_file(string path);
	int open_fluid_synth_settings_file(string path);
	
	int save_adj_synth_patch_file(string path);
	int open_adj_synth_patch_file(string path, int channel);

	int save_adj_synth_settings_file(string path);
	int open_adj_synth_settings_file(string path);

	int save_mod_synth_general_settings_file(string path);
	int open_mod_synth_general_settings_file(string path);

	int save_mod_synth_preset_file(string path);
	int open_mod_synth_preset_file(
		string path = "", 
		_setting_params_t *settings = NULL,
		mod_synth_preset_t *preset = NULL,  
		string *summary = NULL);
	int collect_mod_synth_preset_parms(_setting_params_t *params);
	int set_default_preset_parameters(_setting_params_t *params);

	int collect_mod_synth_preset_parms(mod_synth_preset_t *params);

	void synth_panic_action();

	int scan_midi_ports();
	
	void connect_bt_midi();
	void disconnect_bt_midi();

	void set_master_volume(int vol);
	int get_master_volume();

	void set_fluid_synth_volume(int vol);
	int get_fluid_synth_volume();

	int get_midi_channel_synth(int chan);
	void set_midi_channel_synth(int chan, int synth);
	
	void start_cheack_cpu_utilization_thread();
	void stop_cheack_cpu_utilization_thread();
	
	int execute_mapped_control(int ch, int control_num, int val);
	int execute_synth_control(int module_id, int control_id, int val);
	
	void note_on(uint8_t channel, uint8_t note, uint8_t velocity);
	void note_off(uint8_t channel, uint8_t note);
	void change_program(uint8_t channel, uint8_t program);
	void channel_pressure(uint8_t channel, uint8_t val);
	void controller_event(uint8_t channel, uint8_t num, uint8_t val);
	void pitch_bend(uint8_t channel, int pitch);
	
	ModSynthSettings *general_settings_manager;
	ModSynthSettings *fluid_synth_settings_manager;
	
	
	/* Mapps midi keyboard-controller control-sequences to modSynth control */
	MidiControlMapper *midi_control_mapper;
	
	static int cpu_utilization;

	

	_setting_params_t preset_temp;
	std::string preset_temp_summary_text;

  private:
	
	static ModSynth *mod_synth;

	/* Active modular synth parameters */
	//_setting_params_t fluid_synth_active_settings_params;

	FluidSynthInterface *fluid_synth;
	AdjSynth *adj_synth;

	AlsaBtClientOutput *bt_alsa_out;
	AlsaSerialPortClientOutput *serial_port_alsa_out;

	//AlsaMidi* alsa_midi;
	
	AlsaMidiSysControl *alsa_midi_sys_control;
	
	JackConnections *jack_connections;

	/* The modular synthesizer master volume level (0-100) */
	int master_volume;
	/* The fluid synthesizer  volume level (0-100) */
	int fluid_synth_volume;
	
	/* Holds the general (global) settings parameters */
	_setting_params_t active_general_synth_settings_params;
	
	/* Holds the FluidSynth settings parameters */
	_setting_params_t active_fluid_synth_settings_params;
	
	int audio_driver, sample_rate, audio_block_size;

	/* Holds a bit mask for each MIDI channel 0-15, sketc 1-3 (16-18) indicating assigned synthesizer
	   Channel 0 is the main edditing channel */
	uint16_t midi_channel_synth[_SKETCH_PROGRAM_3 + 1];
	
	pthread_t cheack_cpu_utilization_thread_id;	
};


// Thread checking cpu utilization
void *cheack_cpu_utilization_thread(void *arg);

#endif
