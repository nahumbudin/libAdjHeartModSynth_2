/**
*	@file		midiControlMapper.h
*	@author		Nahum Budin
*	@date		6-Frb-2021
*	@version	1.2
*					1. Code refactoring and notaion.
*	
*	@brief		Used for mapping midi control-keboard control-sequnces\n
*				into synthesizer settings. For example: keyboard knob may be\n
*				mapped to change a synthesizer control knob (parameter).
*
*	History:\n
*	
*	version 1,1		26-May-2020
*	version 1.0		5-Oct-2019:	first version
*		
*/

#ifndef _MIDI_MAPPING
#define _MIDI_MAPPING

#include <stdint.h>
#include <string.h>
#include <map>
#include <vector>

typedef struct midi_control_event
{
	int ch;
	int control_num;
	
	bool operator==(const midi_control_event &mck) const
	{
		return (ch == mck.ch) && (control_num == mck.control_num);
	}
	
	bool operator<(const midi_control_event &mck) const
	{
		return (ch < mck.ch) || ((ch == mck.ch) && (control_num < mck.control_num));
	}

} midi_control_event_t;

typedef struct synth_control
{
	int module_id;
	int control_id;
	int type;
} synth_control_t;

typedef struct map_element
{
	midi_control_event_t midi_control_event;
	synth_control_t synth_control;
} map_element_t;

typedef struct map_element_ignore_channel
{
	int control_num;
	synth_control_t synth_control;
} map_element_ignore_channel_t;

typedef std::map<midi_control_event_t, synth_control_t> midi_controls_map_t;
typedef std::map<int, synth_control_t> midi_controls_map_ignore_channel_t;

class MidiControlMapper
{
public:
	
	MidiControlMapper();
	
	midi_controls_map_t *get_midi_map() { return &midi_map; }
	midi_controls_map_ignore_channel_t *get_midi_map_nc() { return &midi_map_nc; } // ignore channel map
	
	int enable_midi_control_sequences_training();
	int disable_midi_control_sequences_training();
	bool get_midi_control_sequences_training_state();

	void set_midi_control_sequences_use_channel(bool use);
	bool get_midi_control_sequences_use_channel_state();
	
	int update_midi_control_sequence_data(int channel, int control_num);
	int update_synth_control_data(int module, int control_id);

	bool add_control_event(midi_controls_map_t *map, midi_controls_map_ignore_channel_t *map_nc,
						 int ch, int midi_control_num, int synth_module_id, int synth_event);
	
	int add_mapped_midi_control_sequence_event();

	int get_mapped_synth_event(synth_control_t *event, midi_controls_map_t *map, 
							   midi_controls_map_ignore_channel_t *map_nc,
							   int ch, int midi_control_num, bool ignore_channel);
	
	int save_midi_keyboard_control_map_file(std::string path);
	int load_midi_keyboard_control_map_file(std::string path);

	int set_in_focus_module_control(int module, int control, int type);
	int get_in_focus_module_id();
	int get_in_focus_control_id();
	int get_in_focus_control_type();

  private:
	std::vector<map_element_t> get_xml_map_param(const std::string &xml_text);

	int last_midi_control_channel, last_midi_control_num;
	int last_synth_control_module, last_synth_control_id;

	/* True if MIDI control-sequnces training is enabled */
	bool midi_control_sequences_training_enabled;
	/* True if midi channel is used for control mapping; false if channel is ignored */
	bool midi_control_sequences_use_channel;
	
	midi_controls_map_t midi_map;
	midi_controls_map_ignore_channel_t midi_map_nc;

	// GUI selected control
	synth_control in_focus_control;
};
	
	
#endif

