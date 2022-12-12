/**
* @file		fluidSynthInterface.cpp
*	@author		Nahum Budin
*	@date		5-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*
*
*	version	1.0		9-Oct-2019
*
*	@brief		FluidSynth handling.
*/

#include <stdlib.h>

#include "fluidSynthInterface.h"
//#include "modSynth.h"

#include "../libAdjHeartModSynth_2.h"

//const string fluid_default_gm_sound_font_file = "/usr/share/sounds/sf2/FluidR3_GM.sf2";

// Instances of static elements
FluidSynthInterface *FluidSynthInterface::fluid_synth_int_instance = NULL;

FluidSynthInterface::FluidSynthInterface()
{
	
	
	
}

FluidSynthInterface::~FluidSynthInterface()
{
	deinitialize_fluid_synthesizer();
}

/**
 * Creates and returns the fluid synthesizer interface singletone class object instance
 *
 * @param glob_settings a pointer to the synthesizer globalsettings object (mandatory parameter)
 * @return an instance of the fluid_synth_interface
 */
FluidSynthInterface *FluidSynthInterface::create_instance(
	ModSynthSettings *glob_settings,
	 _setting_params_t *global_params)
{
	//fluid_res_t res;
	
	if ((glob_settings == NULL) || (global_params == NULL))
	{
		printf("fatal error - fluid synth - no global settings/params");
		exit(1);
	}
	
	if (fluid_synth_int_instance == NULL)
	{
		fluid_synth_int_instance = new(FluidSynthInterface);
		fluid_synth_int_instance->global_settings = glob_settings;
		fluid_synth_int_instance->active_settings_params = global_params;
	}

	return fluid_synth_int_instance;
}

/**
 * Initialize the FluidSynth with the default setup 
 *
 * @param glob_settings a pointer to the synthesizer globalsettings object (mandatory parameter)
 * @return #_SETTINGS_OK if the value has been set, #SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::initialize_fluid_synthesizer()
{
	fluid_res_t res = 0;

	if (synth != NULL)
	{
		deinitialize_fluid_synthesizer();
	}
	
	loaded_soundfont_map.clear();
	
	// Create a fluid synthesizer settings
	settings = new_fluid_settings();
	// Create a fluid synthesizer	
	synth = new_fluid_synth(settings);
	// Create the midi driver	
	mdriver = new_fluid_midi_driver(settings, handle_midi_event_callback, NULL);
	// Load the default sound font and reset the presets
	sound_font_id = load_fluid_synth_sound_font(fluid_default_gm_sound_font_file, true);
	if (sound_font_id < 0)
	{
		return -1;
	}
	// Set default parameters
	init_default_settings();

	adriver = new_fluid_audio_driver(settings, synth);
	// Select bank 0 and preset 0 for channel 0 
	res = select_fluid_synth_program(0, 0, 0);
	
	return res;
}

/**
 * Deinitialize the FluidSynth - cleanup all resources
 *
 * @return #_SETTINGS_OK if the value has been set, #SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::deinitialize_fluid_synthesizer()
{
	fluid_res_t res = 0;

	printf("fluid synth cleanup");
	
	res = fluid_synth_sfunload(synth, sound_font_id, 1);
	delete_fluid_audio_driver(adriver);
	delete_fluid_midi_driver(mdriver);
	delete_fluid_synth(synth);
	delete_fluid_settings(settings);

	return res;
}

/**
 * Returns the fluid synthesizer interface singletone class object instance
 */
FluidSynthInterface *FluidSynthInterface::get_instance()
{
	return fluid_synth_int_instance;
}

/**
 * Returns the fluid synthesizer settings
 */
fluid_settings_t *FluidSynthInterface::get_fluid_settings()
{ 
	return settings;
}

/**
 * Returns the fluid synthesizer instance
 */
fluid_synth_t *FluidSynthInterface::get_fluid_synth()
{
	return synth; 
}

fluid_res_t FluidSynthInterface::init_default_settings()
{
	fluid_res_t res = 0;
	
	res |= set_fluid_synth_audio_driver("jack");
	res |= set_fluid_synth_audio_jack_id("adjheart_fluid_synth");
	res |= set_fluid_synth_audio_jack_autoconnect(true);
	res |= select_fluid_synth_midi_bank("gm");
	res |= set_fluid_synth_audio_period_size(64); // 64
	res |= set_fluid_synth_audio_num_of_periods(2); // 2

	res |= set_fluid_synth_soundfont_file(_FLUID_DEFAULT_SOUNDFONT_FILE);

	res |= set_fluid_synth_gain(0.04); // master 0.2  fluid 0.8 -> (20 x 80)/40000 (max 0.25)

	res |= disable_fluid_synth_reverb();
	res |= set_fluid_synth_reverb_room_size(0.2);
	res |= set_fluid_synth_reverb_damp(0);
	res |= set_fluid_synth_reverb_width(0.5);
	res |= set_fluid_synth_reverb_level(0.9);

	res |= disable_fluid_synth_chorus();
	res |= set_fluid_synth_chorus_N(3);
	res |= set_fluid_synth_chorus_level(2.0);
	res |= set_fluid_synth_chorus_speed(0.3);
	res |= set_fluid_synth_chorus_depth(8.0);
	res |= set_fluid_synth_chorus_waveform(_FLUID_CHORUS_MOD_SINE);
		

	return res;
}


#define _USED_FOR_LINUX_RASPBERRY_PI  // to set default settings hereafter

/**
 * Initiate a note on midi event.
 *
 * @param channel selected midi channel
 * @param note MIDI note number (0-127)
 * @param velocity MIDI velocity (0-127, 0=noteoff)
 * @return #_FLUID_OK on success, #_FLUID_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::fluid_synth_note_on_event(int channel, int note, int velocity)
{
	return fluid_synth_noteon(synth, channel, note, velocity);
}

/**
* Initiate a note off midi event.
*
* @param channel selected midi channel
* @param note MIDI note number(0 - 127)
* @return #_FLUID_OK on success, #_FLUID_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::fluid_synth_note_off_event(int channel, int note)
{
	return fluid_synth_noteoff(synth, channel, note);
}

/**
* Initiate a controller midi event.
*
* @param channel selected midi channel
* @param num controller number
* @param val controller value
* @return #_FLUID_OK on success, #_FLUID_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::fluid_synth_controller_event(uint8_t channel, uint8_t num, uint8_t val)
{
	return fluid_synth_cc(synth, channel, num, val);
}

/**
* Initiate a pitch bend midi event.
*
* @param channel selected midi channel
* @param pitch pitch value
* @return #_FLUID_OK on success, #_FLUID_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::fluid_synth_pitch_bend_event(uint8_t channel, int pitch)
{
	return fluid_synth_pitch_bend(synth, channel, pitch);
}

/**
* Initiate a change program midi event.
*
* @param channel selected midi channel
* @param program selected program
* @return #_FLUID_OK on success, #_FLUID_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::fluid_synth_program_change_event(uint8_t channel, int program)
{
	return fluid_synth_program_change(synth, channel, program);
}

/**
 * Initiate an channel pressure midi event.
 *
 * @param channel selected midi channel
 * @param val pressure value
 * @return #_FLUID_OK on success, #_FLUID_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::fluid_synth_channel_pressure_event(int chan, int val)
{
	return fluid_synth_channel_pressure(synth, chan, val);
}

/**
 * Set the FluidSynth audio driver type
 *
 * @param driver type: "jack" is the only applicable type for Linux/Raspbian 
 * @return #_SETTINGS_OK if the value has been set, #SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::set_fluid_synth_audio_driver(string driver)
{
#ifdef _USED_FOR_LINUX_RASPBERRY_PI
	return global_settings->set_string_param(active_settings_params,
							"fluid.audio.driver",
							"jack", _PARAM_TYPE_FLUID_SYNTH_SETUP,							
							&set_fluid_synth_audio_driver_callback,
							0, 0, NULL,
							_SET_VALUE | _SET_TYPE | _SET_CALLBACK | _EXEC_CALLBACK);
#else
	return mod_synth::get_instance()->get_settings()->set_string_param(mod_synth::get_instance()->get_settings(),
							"fluid.audio.driver", driver, &set_fluid_synth_audio_driver_callback, 
							_SETTINGS_UPDATE_SETTING);
#endif
}

/**
 * set the FluidSynth Jack client identifier
 *
 * @param id the Jack client identifier string 
 * @return #_SETTINGS_OK if the value has been set, #SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::set_fluid_synth_audio_jack_id(string id)
{
	return global_settings->set_string_param(active_settings_params,
							"fluid.audio.jack.id",
							id, _PARAM_TYPE_FLUID_SYNTH_SETUP, 
							&set_fluid_synth_audio_jack_id_callback,
							0, 0, NULL,
							_SET_VALUE | _SET_TYPE | _SET_CALLBACK | _EXEC_CALLBACK);
}

/**
 * Set the FluidSynth Jack connection to auto/non-auto
 *
 * @param auto_connect if true, then FluidSynth output is automatically connected to jack system audio output.
 * @return #_SETTINGS_OK if the value has been set, #SETTING_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::set_fluid_synth_audio_jack_autoconnect(bool auto_connect)
{
	return global_settings->set_bool_param(active_settings_params,
							"fluid.audio.jack.autoconnect",
							auto_connect, _PARAM_TYPE_FLUID_SYNTH_SETUP,
							&set_fluid_synth_audio_jack_autoconnect_callback,
							0, 0, NULL,
							_SET_VALUE | _SET_TYPE | _SET_CALLBACK | _EXEC_CALLBACK);
}

/**
 * Sets the FluidSynth t size of the audio buffers (in frames).
 *
 * @param frames the size of the audio buffers (64-8192) default: Lynux 64, Windows 512
 * @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::set_fluid_synth_audio_period_size(int size)
{
	return global_settings->set_int_param(active_settings_params,
							"fluid.audio.period-size", size, 8192, 64,
							_PARAM_TYPE_FLUID_SYNTH_SETUP,
							&set_fluid_synth_audio_period_size_callback,
							0, 0, NULL,
							_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE | _SET_CALLBACK | _EXEC_CALLBACK);
}

/**
 * Sets the FluidSynth number of the audio buffers used by the driver.
 *
 * @param periods the number of the audio buffers used by the driver (2-64) default: Lynux 16, Windows 8
 * @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::set_fluid_synth_audio_num_of_periods(int periods)
{
	return global_settings->set_int_param(active_settings_params,
							"fluid.audio.periods", periods, 64, 2,
							_PARAM_TYPE_FLUID_SYNTH_SETUP,
							&set_fluid_synth_audio_num_of_periods_callback,
							0, 0, NULL,
							_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE | _SET_CALLBACK | _EXEC_CALLBACK);
}

/**
 * Select how the synthesizer interprets Bank Select messages..
 *
 * @param bank gm: ignores CC0 and CC32 messages, gs: (default) CC0 becomes the bank number, CC32 is ignored.
 *		xg: CC32 becomes the bank number, CC0 toggles between melodic or drum channel. mma: bank is calculated as CC0*128+CC32.
 * @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::select_fluid_synth_midi_bank(string bank)
{
	return global_settings->set_string_param(active_settings_params,
							"fluid.synth.midi-bank-select", bank,
							_PARAM_TYPE_FLUID_SYNTH_SETUP,
							&select_fluid_synth_midi_bank_callback, 
							0, 0, NULL,
							_SET_VALUE | _SET_TYPE | _SET_CALLBACK | _EXEC_CALLBACK);
}

/**
 * Loads a sound font file
 *
 * @param path full path of the sounf font file (example: "/usr/share/sounds/sf2/FluidR3_GM.sf2")
 * @param reset_presets 
 * @return the sound font id (to be used as an handle) or a negative number if action fails.
 */
fluid_res_t FluidSynthInterface::load_fluid_synth_sound_font(string path, bool reset_presets)
{
	int sfid;
	
	sfid =  fluid_synth_sfload(synth, path.c_str(), reset_presets);
	if (sfid != FLUID_FAILED)
	{
		printf("SoundFont %s loaded OK.", path.c_str());
		loaded_soundfont_map[path] = sfid;
	}
	else
	{
		printf("SoundFont %s loadding failed.", path.c_str());
	}
	
	return sfid;
}

/**
 * Sets a bank and a program to a channel.
 *
 * @param channel selected midi channel
 * @param bank_num midi bank number
 * @param program midi program number
 * @return #_FLUID_OK on success, #_FLUID_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::select_fluid_synth_program(int channel, unsigned int bank_num, int program)
{
	return fluid_synth_program_select(synth, channel, sound_font_id, bank_num, program);
}


/**
 * Sets the FluidSynth gain value.
 *
 * @param gain gain value (0-0.25)
 * @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::set_fluid_synth_gain(double gain) {

	return global_settings->set_float_param(active_settings_params,
							"fluid.synth.gain", gain, 0.25, 0.0,
							_PARAM_TYPE_FLUID_SYNTH_SETUP,
							&set_fluid_synth_gain_callback,
							0, 0, NULL,
							_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE | _SET_CALLBACK | _EXEC_CALLBACK);
}

/**
 * Enables fluid synth reverb effect.
 *
 * @param none
 * @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::enable_fluid_synth_reverb()
{
	fluid_res_t res = 0;

	res = set_fluid_synth_reverb_params();
	
	res |= global_settings->set_bool_param(active_settings_params,
							"fluid.synth.reverb.active", true,
							_PARAM_TYPE_FLUID_SYNTH_SETUP,
							&set_fluid_synth_reverb_active_callback,
							0, 0, NULL,
							_SET_VALUE | _SET_TYPE | _SET_CALLBACK | _EXEC_CALLBACK);

	return res;
}

/**
 * Disables fluid synth reverb effect.
 *
 * @param none
 * @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::disable_fluid_synth_reverb()
{
	return global_settings->set_bool_param(active_settings_params,
						"fluid.synth.reverb.active", false,
						_PARAM_TYPE_FLUID_SYNTH_SETUP,
						&set_fluid_synth_reverb_active_callback,
						0, 0, NULL,
						_SET_VALUE | _SET_TYPE | _SET_CALLBACK | _EXEC_CALLBACK);
}

/**
* Sets the FluidSynth reverb room-size (amount of wet).
*
* @param size size value (0-1.0)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_reverb_room_size(double size)
{
	return global_settings->set_float_param(active_settings_params,
							"fluid.synth.reverb.room-size", size, 1.0, 0.0,
							_PARAM_TYPE_FLUID_SYNTH_SETUP, NULL,
							0, 0, NULL,
							_SET_VALUE | _SET_MAX_VAL |_SET_MIN_VAL |_SET_TYPE);
}

/**
* Sets the FluidSynth reverb damp.
*
* @param damp dampping value (0-1.0)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_reverb_damp(double damp)
{
	return global_settings->set_float_param(active_settings_params,
							"fluid.synth.reverb.damp", damp, 1.0, 0.0,
							_PARAM_TYPE_FLUID_SYNTH_SETUP, NULL,
							0, 0, NULL,
							_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE);
}

/**
* Sets the FluidSynth reverb width.
*
* @param width width value (0-1.0) ( not 0-100)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_reverb_width(double width)
{
	return global_settings->set_float_param(active_settings_params,
							"fluid.synth.reverb.width", width, 1.0, 0.0,
							_PARAM_TYPE_FLUID_SYNTH_SETUP, NULL,
							0, 0, NULL,
							_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE);
}

/**
* Sets the FluidSynth reverb level (output amplitude).
*
* @param level level value (0-1.0)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_reverb_level(double level)
{
	return global_settings->set_float_param(active_settings_params,
							"fluid.synth.reverb.level", level, 1.0, 0.0,
							_PARAM_TYPE_FLUID_SYNTH_SETUP, NULL,
							0, 0, NULL,
							_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE);
}

/**
* Sets the FluidSynth reverb paramters to the settings parameters values.
*
* @param none
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_reverb_params()
{
	fluid_res_t res = 0;
	mod_synth_settings_float_param_t param;
	float room_size, damp, width, level;

	res = global_settings->get_float_param(active_settings_params, "fluid.synth.reverb.room-size", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	room_size = param.value;

	res = global_settings->get_float_param(active_settings_params, "fluid.synth.reverb.damp", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	damp = param.value;

	res = global_settings->get_float_param(active_settings_params, "fluid.synth.reverb.width", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	width = param.value;

	res = global_settings->get_float_param(active_settings_params, "fluid.synth.reverb.level", &param);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	level = param.value;

	fluid_synth_set_reverb(synth, room_size, damp, width, level);

	return res;
}

/**
 * Set fluid synth reverb effect enabele - value only.
 *
 * @param en
 * @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::set_fluid_synth_reverb_enabled_value(bool en)
{
	fluid_res_t res = 0;

	res = set_fluid_synth_reverb_params();
	
	res |= global_settings->set_bool_param_value(active_settings_params,
		"fluid.synth.reverb.active",
		en,
		_SET_VALUE | _EXEC_CALLBACK,
		-1); // no program

	return res;
}

/**
* Sets the FluidSynth reverb room-size value (amount of wet).
*
* @param size size value (0-1.0)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_reverb_room_size_value(double size)
{
	return global_settings->set_float_param_value(active_settings_params,
		"fluid.synth.reverb.room-size",
		size,
		_SET_VALUE,
		-1);
}

/**
* Sets the FluidSynth reverb damp value.
*
* @param damp dampping value (0-1.0)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_reverb_damp_value(double damp)
{
	return global_settings->set_float_param_value(active_settings_params,
		"fluid.synth.reverb.damp",
		damp,
		_SET_VALUE,
		-1);
}

/**
* Sets the FluidSynth reverb width value.
*
* @param width width value (0-1.0) ( not 0-100)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_reverb_width_value(double width)
{
	return global_settings->set_float_param_value(active_settings_params,
		"fluid.synth.reverb.width",
		width,
		_SET_VALUE,
		-1);
}

/**
* Sets the FluidSynth reverb level value (output amplitude).
*
* @param level level value (0-1.0)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_reverb_level_value(double level)
{
	return global_settings->set_float_param_value(active_settings_params,
		"fluid.synth.reverb.level",
		level,
		_SET_VALUE,
		-1);
}


/**
 * Enables fluid synth chorus effect.
 *
 * @param none
 * @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::enable_fluid_synth_chorus()
{
	fluid_res_t res = 0;
	
	res = set_fluid_synth_chorus_params();
	
	res |= global_settings->set_bool_param(active_settings_params,
							"fluid.synth.chorus.active", true,
							_PARAM_TYPE_FLUID_SYNTH_SETUP,
							&set_fluid_synth_chorus_active_callback,
							0, 0, NULL,
							_SET_VALUE | _SET_TYPE | _SET_CALLBACK | _EXEC_CALLBACK);

	return res;
}

/**
 * Disables fluid synth chorus effect.
 *
 * @param none
 * @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::disable_fluid_synth_chorus()
{
	return global_settings->set_bool_param(active_settings_params,
							"fluid.synth.chorus.active", false,
							_PARAM_TYPE_FLUID_SYNTH_SETUP,
							&set_fluid_synth_chorus_active_callback,
							0, 0, NULL,
							_SET_VALUE | _SET_TYPE | _SET_CALLBACK | _EXEC_CALLBACK);
}

/**
* Sets the FluidSynth chorus N (voice count).
*
* @param N voice count (0-99)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_chorus_N(int N)
{
	return global_settings->set_int_param(active_settings_params,
							"fluid.synth.chorus.nr", N, 99, 0,
							_PARAM_TYPE_FLUID_SYNTH_SETUP, NULL,
							0, 0, NULL,
							_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE);
}

/**
* Sets the FluidSynth chorus level (output amplitude).
*
* @param level output level (0-10.0)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_chorus_level(double level)
{
	return global_settings->set_float_param(active_settings_params,
							"fluid.synth.chorus.level", level, 10.0, 0.0,
							_PARAM_TYPE_FLUID_SYNTH_SETUP, NULL,
							0, 0, NULL,
							_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE);
}

/**
* Sets the FluidSynth chorus speed (modulation speed in Hz).
*
* @param speed  modulation speed in Hz (029-5.0)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_chorus_speed(double speed)
{
	return global_settings->set_float_param(active_settings_params,
							"fluid.synth.chorus.speed", speed, 5.0, 0.29,
							_PARAM_TYPE_FLUID_SYNTH_SETUP, NULL,
							0, 0, NULL,
							_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE);
}

/**
* Sets the FluidSynth chorus depth (modulation depth).
*
* @param depth  modulation depth (0-256)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_chorus_depth(double depth)
{
	return global_settings->set_float_param(active_settings_params,
							"fluid.synth.chorus.depth", depth, 256.0, 0.0,
							_PARAM_TYPE_FLUID_SYNTH_SETUP, NULL,
							0, 0, NULL,
							_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE);
}

/**
* Sets the FluidSynth chorus modulation waveform.
*
* @param wform modulation waveform _FLUID_CHORUS_MOD_SINE (0) or _FLUID_CHORUS_MOD_TRIANGLE (1)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_chorus_waveform(int wform)
{
	return global_settings->set_int_param(active_settings_params,
							"fluid.synth.chorus.waveform", wform, 1, 0,
							_PARAM_TYPE_FLUID_SYNTH_SETUP, NULL,
							0, 0, NULL,
							_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE);
}

/**
* Sets the FluidSynth chorus paramters to the settings parameters values.
*
* @param none
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_chorus_params()
{
	fluid_res_t res = 0;
	mod_synth_settings_int_param_t iparam;
	mod_synth_settings_float_param_t fparam;
	int N;
	float level, speed, depth;
	
	res = global_settings->get_int_param(active_settings_params, "fluid.synth.chorus.nr", &iparam);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	N = iparam.value;

	res = global_settings->get_float_param(active_settings_params, "fluid.synth.chorus.level", &fparam);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	level = fparam.value;

	res = global_settings->get_float_param(active_settings_params, "fluid.synth.chorus.speed", &fparam);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	speed = fparam.value;

	res = global_settings->get_float_param(active_settings_params, "fluid.synth.chorus.depth", &fparam);
	if (res != _SETTINGS_KEY_FOUND)
	{
		return _SETTINGS_FAILED;
	}

	depth = fparam.value;

	fluid_synth_set_chorus(synth, N, level, speed, depth, FLUID_CHORUS_MOD_SINE); // TODO: select waveform

	return res;
}

/**
 * Set fluid synth chorus effect enabled value.
 *
 * @param en
 * @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::set_fluid_synth_chorus_enabled_value(bool en)
{
	fluid_res_t res = 0;
	
	res = set_fluid_synth_chorus_params();
	
	res |= global_settings->set_bool_param_value(active_settings_params,
		"fluid.synth.chorus.active",
		en,
		_SET_VALUE | _EXEC_CALLBACK,
		-1);

	return res;
}

/**
* Sets the FluidSynth chorus N (voice count) value.
*
* @param N voice count (0-99)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_chorus_N_value(int N)
{
	return global_settings->set_int_param_value(active_settings_params,
		"fluid.synth.chorus.nr",
		N,
		_SET_VALUE,
		-1);
}

/**
* Sets the FluidSynth chorus level value (output amplitude).
*
* @param level output level (0-10.0)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_chorus_level_value(double level)
{
	return global_settings->set_float_param_value(active_settings_params,
		"fluid.synth.chorus.level",
		level,
		_SET_VALUE,
		-1);
}

/**
* Sets the FluidSynth chorus speed value (modulation speed in Hz).
*
* @param speed  modulation speed in Hz (029-5.0)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_chorus_speed_value(double speed)
{
	return global_settings->set_float_param_value(active_settings_params,
		"fluid.synth.chorus.speed",
		speed,
		_SET_VALUE,
		-1);
}

/**
* Sets the FluidSynth chorus depth value (modulation depth).
*
* @param depth  modulation depth (0-256)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_chorus_depth_value (double depth)
{
	return global_settings->set_float_param_value(active_settings_params,
		"fluid.synth.chorus.depth",
		depth,
		_SET_VALUE,
		-1);
}

/**
* Sets the FluidSynth chorus modulation waveform.
*
* @param wform modulation waveform value _FLUID_CHORUS_MOD_SINE (0) or _FLUID_CHORUS_MOD_TRIANGLE (1)
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_chorus_waveform_value(int wform)
{
	return global_settings->set_int_param(active_settings_params,
		"fluid.synth.chorus.waveform",
		wform,
		_SET_VALUE,
		-1);
}

/**
* Sets the FluidSynth soundfont file settings value (not loading).
*
* @param file full path of the soundfont file
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_soundfont_file(string path)
{
	return global_settings->set_string_param(active_settings_params,
											"fluid.synth.soundfont-file", path,
											_PARAM_TYPE_FLUID_SYNTH_SETUP, NULL,
											0, 0, NULL,
											_SET_VALUE | _SET_MAX_VAL | _SET_MIN_VAL | _SET_TYPE);
}

/**
* Sets the FluidSynth soundfont file settings value, load and activate it.
*
* @param none
* @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
*/
fluid_res_t FluidSynthInterface::load_and_set_fluid_synth_soundfont(string path)
{
	fluid_res_t res = set_fluid_synth_soundfont_file(path);
	if (res == _SETTINGS_OK)
	{
		// Load and reset reset presets (so that new instruments get used from the SoundFont)
		sound_font_id = fluid_synth_sfload(synth, path.c_str(), 1);
	}

	if (sound_font_id != FLUID_FAILED)
	{
		printf("SoundFont %s loaded OK.", path.c_str());
		loaded_soundfont_map[path] = sound_font_id;
	}
	else
	{
		printf("SoundFont %s loadding failed.", path.c_str());
	}

	return res;
}

/**
*   @brief  Select an instrument on a MIDI channel by SoundFont ID, bank and program numbers.
*   @param  chan MIDI channel 0-15
*	@param	sfid ID of loaded sondfone
*	@param	bank bank number
*	@param	program MIDI program number
*   @return 0 on successes -1 otherwise
*/
fluid_res_t FluidSynthInterface::set_fluid_synth_program_select(int chan, unsigned int sfid,
															unsigned int bank, unsigned int program)
{
	return fluid_synth_program_select(synth, chan, sfid, bank, program);
}

/**
*   @brief  Get the current fluid synth channel sfont id
*   @param  int chan
*   @return sfont id
*/
int FluidSynthInterface::get_fluid_synth_channel_sfont_id(int chan)
{
	int res;
	fluid_synth_channel_info_t info;
	
	res = fluid_synth_get_channel_info(synth, chan, &info);

	if (res == FLUID_OK)
	{
		return info.sfont_id;
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  Get the current fluid synth channel bank
*   @param  int chan
*   @return bank
*/
int FluidSynthInterface::get_fluid_synth_channel_bank(int chan)
{
	int res;
	fluid_synth_channel_info_t info;

	res = fluid_synth_get_channel_info(synth, chan, &info);

	if (res == FLUID_OK)
	{
		return info.bank;
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  Get the current fluid synth channel program
*   @param  int chan
*   @return program
*/
int FluidSynthInterface::get_fluid_synth_channel_program(int chan)
{
	int res;
	fluid_synth_channel_info_t info;

	res = fluid_synth_get_channel_info(synth, chan, &info);

	if (res == FLUID_OK)
	{
		return info.program;
	}
	else
	{
		return -1;
	}
}

/**
*   @brief  Get the current fluid synth channel preset name
*   @param  int chan
*   @return preset name
*/
std::string FluidSynthInterface::get_fluid_synth_channel_preset_name(int chan)
{
	int res;
	fluid_synth_channel_info_t info;

	res = fluid_synth_get_channel_info(synth, chan, &info);

	if (res == FLUID_OK)
	{
		return string(info.name);
	}
	else
	{
		return "";
	}
}

std::string FluidSynthInterface::get_fluid_synth_channel_preset_soundfont_name(int chan)
{
	char *sound_font_name;
	fluid_sfont_t *sound_font;
	int sfid;

	sfid = get_fluid_synth_channel_sfont_id(chan);
	if (sfid < 0)
	{
		return "";
	}

	sound_font = fluid_synth_get_sfont_by_id(synth, sfid); 
	if (sound_font != NULL)
	{
		sound_font_name = sound_font->get_name(sound_font);
	}

	return sound_font_name;
}

/**
*   @brief  Get the name of the soundfont id
*   @param  int sfid
*   @return  soundfont name
*/
std::string FluidSynthInterface::get_fluid_synth_soundfont_id_name(int sfid) {

	char *sound_font_name;
	fluid_sfont_t *sound_font;

	sound_font = fluid_synth_get_sfont_by_id(synth, sfid);

	if (sound_font == NULL)
	{
		return "";
	}

	return sound_font->get_name(sound_font);
}

int FluidSynthInterface::get_num_of_fluid_synth_loaded_soundFonts_files()
{
	return fluid_synth_sfcount(synth);
}

int FluidSynthInterface::get_fluid_synth_sound_font_presets(int sfnum, 
									std::vector<_soundfont_presets_data_t> *presets)
{
	int more;
	fluid_preset_t* preset;
	fluid_sfont_t *sound_font;
	_soundfont_presets_data_t preset_data;
	
	preset = new fluid_preset_t();
	sound_font = fluid_synth_get_sfont(synth, sfnum);
	if ((sound_font != NULL) && (presets != NULL))
	{
		// Reset the iteration
		sound_font->iteration_start(sound_font);
		// Go through all the presets within the soundfont
		more = 1;
		while (more) {
			more = sound_font->iteration_next(sound_font, preset); // Will return 0 if no more soundfonts left
			if (more) {
				// Get preset data
				preset_data.banknum = preset->get_banknum(preset);
				preset_data.presetname = preset->get_name(preset);
				preset_data.program = preset->get_num(preset);
				preset_data.soundFontId = sound_font->id;

				presets->push_back(preset_data);
			}
		}
	}
	else
	{
		return FLUID_FAILED;
	}

	return FLUID_OK;
}

/**
*   @brief  Get the presets of all fluid soundfont files.
*	@param	presets a pointer to a _soundfont_presets_data_t vector to return
*			the specified sound font presets data
*   @return 0 if OK
*/
int FluidSynthInterface::get_fluid_synth_loaded_sound_fonts_presets(std::vector<_soundfont_presets_data_t> *presets)
{
	int num_of_loadded_Sound_fonts, sf, res;

	num_of_loadded_Sound_fonts = get_num_of_fluid_synth_loaded_soundFonts_files();

	if (num_of_loadded_Sound_fonts < 1)
	{
		return FLUID_FAILED;
	}

	for (sf = 0; sf < num_of_loadded_Sound_fonts; sf++)
	{
		res = get_fluid_synth_sound_font_presets(sf, presets);
	
		if (res == FLUID_FAILED)
		{
			return res;
		}
	}

	return FLUID_OK;
}

/**
*   @brief  Activate fluid synth panic cleanup
*   @param  none
*   @return none
*/
void FluidSynthInterface::fluid_synth_panic_action()
{
	for (int chan = 0; chan < 16; chan++)
	{
		fluid_synth_all_sounds_off(synth, chan);
	}

	// BIG PANIC
//	fluid_synth_system_reset();
}

/**
 * Sets the FluidSynth preset settings callbacks.
 *
 * @param none
 * @return #_SETTINGS_OK if the value has been set, #_SETTINGS_FAILED otherwise
 */
fluid_res_t FluidSynthInterface::set_callbacks()
{
	fluid_res_t res;
	
	res = global_settings->set_string_param(active_settings_params,
					"fluid.audio.driver", "jack", _PARAM_TYPE_FLUID_SYNTH_SETUP,
					&set_fluid_synth_audio_driver_callback,
					_SET_CALLBACK | _EXEC_CALLBACK);

	res |=  global_settings->set_string_param(active_settings_params,
					"fluid.audio.jack.id", 0, _PARAM_TYPE_FLUID_SYNTH_SETUP,
					&set_fluid_synth_audio_jack_id_callback,
					_SET_CALLBACK | _EXEC_CALLBACK);

	res |= global_settings->set_bool_param(active_settings_params,
					"fluid.audio.jack.autoconnect",
					false, _PARAM_TYPE_FLUID_SYNTH_SETUP,
					&set_fluid_synth_audio_jack_autoconnect_callback,
					_SET_CALLBACK | _EXEC_CALLBACK);

	res |= global_settings->set_int_param(active_settings_params,
					"fluid.audio.period-size", 64, 8192, 64,
					_PARAM_TYPE_FLUID_SYNTH_SETUP,
					&set_fluid_synth_audio_period_size_callback,
					_SET_CALLBACK | _EXEC_CALLBACK);

	res |= global_settings->set_int_param(active_settings_params,
					"fluid.audio.periods", 2, 64, 2,
					_PARAM_TYPE_FLUID_SYNTH_SETUP,
					&set_fluid_synth_audio_num_of_periods_callback,
					_SET_CALLBACK | _EXEC_CALLBACK);

	res |= global_settings->set_string_param(active_settings_params,
					"fluid.synth.midi-bank-select", "gm", _PARAM_TYPE_FLUID_SYNTH_SETUP,
					&select_fluid_synth_midi_bank_callback,
					_SET_CALLBACK | _EXEC_CALLBACK);

	res |= global_settings->set_bool_param(active_settings_params,
					"fluid.synth.reverb.active", false,
					_PARAM_TYPE_FLUID_SYNTH_SETUP,
					&set_fluid_synth_reverb_active_callback,
					_SET_CALLBACK | _EXEC_CALLBACK);

	res |= global_settings->set_bool_param(active_settings_params,
					"fluid.synth.chorus.active", false,
					_PARAM_TYPE_FLUID_SYNTH_SETUP,
					&set_fluid_synth_chorus_active_callback,
					_SET_CALLBACK | _EXEC_CALLBACK);

	return res;
}


/**
 * Strips the setting name prefix (prefix.x.y[.z] -> x.y[.z])
 *
 * @param name the full settings name 
 * @return the stripped name
 */
string strip_settings_name_prefix(string name)
{
	int prefix_end = name.find_first_of('.');
	
	return name.substr(prefix_end + 1, name.length() - 1);
}

// FluidSynth settings callbacks

/**
 * A direct access to the FluidSynth to set the audio driver type
 *
 * @param driver type "jack" - the only applicable type for Linux/Raspbian 
 * @param dummy	NA (set to zero)
 * @return #FLUID_OK if the value has been set, #FLUID_FAILED otherwise
 */
int set_fluid_synth_audio_driver_callback(string driver, int dummy)
{
#ifdef _USED_FOR_LINUX_RASPBERRY_PI
	return fluid_settings_setstr(FluidSynthInterface::get_instance()->get_fluid_settings(), "audio.driver", "jack");
#else
	return fluid_settings_setstr(fluid_synth_interface::get_instance()->get_fluid_settings(), "audio.driver", driver.c_str()); 
#endif
}

/**
 * A direct access to the FluidSynth to set the Jack client identifier
 *
 * @param id the Jack client identifier string 
 * @param dummy	NA (set to zero)
 * @return #FLUID_OK if the value has been set, #FLUID_FAILED otherwise
 */
int set_fluid_synth_audio_jack_id_callback(string id, int dummy)
{
	return fluid_settings_setstr(FluidSynthInterface::get_instance()->get_fluid_settings(), "audio.jack.id", id.c_str());
}

/**
 * A direct access to the FluidSynth to set the Jack connection to auto/non-auto
 *
 * @param connect if true, then FluidSynth output is automatically connected to jack system audio output.
 * @param dummy	NA (set to zero)
 * @return #FLUID_OK if the value has been set, #FLUID_FAILED otherwise
 */
int set_fluid_synth_audio_jack_autoconnect_callback(bool auto_connect, int dummy)
{
	return fluid_settings_setint(FluidSynthInterface::get_instance()->get_fluid_settings(), "audio.jack.autoconnect",
								 auto_connect ? 1 : 0);
}

/**
 * A direct access to the FluidSynth to set the size of the audio buffers (in frames).
 *
 * @param frames the size of the audio buffers (64-8192) default: Lynux 64, Windows 512
 * @param dummy	NA (set to zero)
 * @return #FLUID_OK if the value has been set, #FLUID_FAILED otherwise
 */
int set_fluid_synth_audio_period_size_callback(int size, int dummy)
{
	return fluid_settings_setint(FluidSynthInterface::get_instance()->get_fluid_settings(), "audio.period-size", size);
}

/**
 * A direct access to the FluidSynth to set the number of the audio buffers used by the driver.
 *
 * @param periods the number of the audio buffers used by the driver (2-64) default: Lynux 16, Windows 8
 * @param dummy	NA (set to zero)
 * @return #FLUID_OK if the value has been set, #FLUID_FAILED otherwise
 */
int set_fluid_synth_audio_num_of_periods_callback(int periods, int dummy)
{
	return fluid_settings_setint(FluidSynthInterface::get_instance()->get_fluid_settings(), "audio.periods", periods);
}

/**
 * A direct access to define how the synthesizer interprets Bank Select messages..
 *
 * @param bank gm: ignores CC0 and CC32 messages, gs: (default) CC0 becomes the bank number, CC32 is ignored.
 *		xg: CC32 becomes the bank number, CC0 toggles between melodic or drum channel. mma: bank is calculated as CC0*128+CC32.
 * @param dummy	NA (set to zero)
 * @return #FLUID_OK if the value has been set, #FLUID_FAILED otherwise
 */	
int select_fluid_synth_midi_bank_callback(string bank, int dummy)
{
	return fluid_settings_setstr(FluidSynthInterface::get_instance()->get_fluid_settings(), "synth.midi-bank-select", bank.c_str());
}

/**
 * A callback to handle midi events received by the midi driver input/
 *
 * @return #FLUID_OK if the value has been set, #FLUID_FAILED otherwise
 */
int handle_midi_event_callback(void* data, fluid_midi_event_t* event)
{
	fluid_res_t res;
	printf("event type: %d channel: %d\n", fluid_midi_event_get_type(event), fluid_midi_event_get_channel(event));
	//res = fluid_synth_handle_midi_event(ModSynth::get_instance()->get_fluid_synth()->get_fluid_synth(), event);
	
	res = fluid_synth_handle_midi_event(FluidSynthInterface::get_instance()->get_fluid_synth(), event);
	
	if (fluid_midi_event_get_type(event) == 0xc0)
	{
		printf("program changed\n);");
		callback_midi_program_change_event(fluid_midi_event_get_channel(event), fluid_midi_event_get_program(event));
	}

	return res;
}

int set_fluid_synth_gain_callback(double gain, int dummy)
{
	fluid_res_t res;

	res = fluid_settings_setnum(FluidSynthInterface::get_instance()->get_fluid_settings(), "synth.gain", gain);

	return res;
}

int set_fluid_synth_reverb_active_callback(bool state, int dummy)
{
//	res = fluid_settings_setint(FluidSynthInterface::get_instance()->get_fluid_settings(), "synth.reverb.active", state);
	
	fluid_synth_set_reverb_on(FluidSynthInterface::get_instance()->get_fluid_synth(), state ? 1 : 0);

	return 0;
}

int get_fluid_synth_reverb_active_callback()
{
	fluid_res_t res;
	int param = 0; // default - disabled

	res = fluid_settings_getint(FluidSynthInterface::get_instance()->get_fluid_settings(), 
								"synth.reverb.active", &param);
	
	// Should 1 (enabled) or 0 (disabled)
	return param;
}

int set_fluid_synth_chorus_active_callback(bool state, int dummy)
{
//	fluid_res_t res;
//	res = fluid_settings_setint(FluidSynthInterface::get_instance()->get_fluid_settings(), "synth.chorus.active", state);
	fluid_synth_set_chorus_on(FluidSynthInterface::get_instance()->get_fluid_synth(), state ? 1 : 0);

	return 0;
}

int get_fluid_synth_chorus_active_callback()
{
	fluid_res_t res;
	int param = 0; // default - disabled;

	res = fluid_settings_getint(FluidSynthInterface::get_instance()->get_fluid_settings(), "synth.chorus.active", &param);

	// Should 1 (enabled) or 0 (disabled)
	return param;
}



