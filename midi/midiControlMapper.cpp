/**
*	@file		midiControlMapper.cpp
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

#include "midiControlMapper.h"
#include "../libAdjHeartModSynth_2.h"
#include "../utils/XMLfiles.h"
#include "../utils/utils.h"

static const int version = 20210206;

MidiControlMapper::MidiControlMapper()
{
	midi_control_sequences_training_enabled = false;
	midi_control_sequences_use_channel = false;

	last_midi_control_channel = -1;
	last_midi_control_num = -1;
	last_synth_control_module = -1;
	last_synth_control_id = -1;	
}

/**
*   @brief  Enables MIDI control keyboard control-sequences training mode
*   @param  none
*   @return 0 if done
*/
int MidiControlMapper::enable_midi_control_sequences_training()
{
	midi_control_sequences_training_enabled = true;
	return 0;
}

/**
*   @brief  Disables MIDI control keyboard control-sequences training mode
*   @param  none
*   @return true if enabled
*/
int MidiControlMapper::disable_midi_control_sequences_training()
{
	midi_control_sequences_training_enabled = false;
	return 0;
}

/**
*   @brief  returns true if MIDI control keyboard control-sequences training is enabled
*   @param  none
*   @return 0 if done
*/
bool MidiControlMapper::get_midi_control_sequences_training_state()
{
	return midi_control_sequences_training_enabled;
}

/**
*   @brief  set use of MIDI channel for MIDI control keyboard control-sequences
*   @param  use		true - channel is used; false - channel is ignored
*   @return none
*/
void MidiControlMapper::set_midi_control_sequences_use_channel(bool use)
{
	midi_control_sequences_use_channel = use;
}

/**
*   @brief  get use of MIDI channel for MIDI control keyboard control-sequences state
*   @param  none		
*   @return true - channel is used; false - channel is ignored
*/
bool MidiControlMapper::get_midi_control_sequences_use_channel_state()
{
	return midi_control_sequences_use_channel;
}

/**
*   @brief  update the midi control-sequnce data
*   @param  int		channel		midi channel
*   @param	int		controlNum	midi control-num
*   @return 0 if done
*/
int MidiControlMapper::update_midi_control_sequence_data(int channel, int control_num)
{
	if ((channel < 0) && (channel > 15) && (control_num < 0) && (control_num > 127))
	{
		return -1;
	}
	else
	{
		last_midi_control_channel = channel;
		last_midi_control_num = control_num;
		
		return 0;
	}
}

/**
*   @brief  update the synthesizer control data
*   @param  int		module		synthesizer module (_OSC_1_EVENT, _OSC_2_EVENT..._MOD_SYNTH_EVENT...)
*   @param	int		controlId	module control-num (_OSC_PWM_SYMMETRY,.. _MOD_ADSR_ATTACK...)
*   @return 0 if done
*/
int MidiControlMapper::update_synth_control_data(int module, int control_id)
{
	if ((module < _OSC_1_EVENT) && (module > _MOD_SYNTH_EVENT) && 
		(control_id < 0) && (control_id > 0xffff))
	{
		return -1;
	}
	else
	{
		last_synth_control_module = module;
		last_synth_control_id = control_id;
		
		return 0;
	}
}

/**
*   @brief  Add a new control map pair to the map
*   @param  map				a midiControlsMap_t map
*   @param  map_nc			a midiControlsMapIgnoreChannel_t map (ignore channel)
*   @param	ch				midi control event channel
*   @param	midiControlNum	midi control event control number
*   @param	synthModuleId	synthesizer module id
*   @param	synth
*   @return true if done; false: element already exist
*/
bool MidiControlMapper::add_control_event(midi_controls_map_t *map, midi_controls_map_ignore_channel_t *map_nc, 
										int ch, int midi_control_num, int synth_module_id, int synth_event)
{
	
	std::pair < std::map < midi_control_event_t, synth_control_t >::iterator, bool > ret;	
	std::pair < midi_control_event_t, synth_control_t > control_event((midi_control_event_t{ ch, midi_control_num }), 
																	  (synth_control_t{ synth_module_id, synth_event }));		
	ret = map->insert(control_event);

	std::pair<std::map<int, synth_control_t>::iterator, bool> retnc;
	std::pair<int, synth_control_t> control_event_nc(midi_control_num, (synth_control_t{synth_module_id, synth_event}));
	retnc = map_nc->insert(control_event_nc);
	
	return (ret.second && retnc.second) ;  	
}

/**
*   @brief  Map last midi control event and last synthesizer event and add it to the map.
*   @param  none
*   @return 0 if done; -1: element already exists and replaced -2: non valid last events data
*/
int MidiControlMapper::add_mapped_midi_control_sequence_event()
{
	bool res;
	
	if ((last_midi_control_channel != -1) && (last_midi_control_num != -1) &&
		(last_synth_control_module != -1) && (last_synth_control_id != -1))
	{
		res = add_control_event(&midi_map, &midi_map_nc,
			last_midi_control_channel,
			last_midi_control_num,
			last_synth_control_module,
			last_synth_control_id);
		
		if (res)
		{
			// New inserted
			return 0;
		}
		else
		{
			// exist - replace
			midi_control_event_t control_event;  
			control_event.ch = last_midi_control_channel;
			control_event.control_num = last_midi_control_num;
			
			std::map < midi_control_event_t, synth_control_t >::iterator it;
			it = midi_map.find(control_event);
			if (it != midi_map.end())
			{
				(*it).second.module_id = last_synth_control_module;
				(*it).second.control_id = last_synth_control_id;
			}

			std::map<int, synth_control_t>::iterator it_nc;
			it_nc = midi_map_nc.find(last_midi_control_num);
			if (it_nc != midi_map_nc.end())
			{
				(*it_nc).second.module_id = last_synth_control_module;
				(*it_nc).second.control_id = last_synth_control_id;
			}
			
			return -1;
		}
	}
	
	return -2;
}

/**
*   @brief  Get a mapped synthesizer control sequence
*	@param	event			a pointer to a synthControl_t struct that will hold the returned event or NULL
*	@param  map				a midiControlsMap_t map
*	@param  map_nc			a midiControlsMapIgnoreChannel_t map (ignore channel)
*   @param	ch				midi control event channel
*   @param	midiControlNum	midi control event control number
*   @param	ignoreChannel	when true, ignore midi channel
*   @return 0 if done; -1: event not mapped -2: non valid data
*/
int MidiControlMapper::get_mapped_synth_event(synth_control_t *event,
										   midi_controls_map_t *map,
										   midi_controls_map_ignore_channel_t *map_nc,
										   int ch, int midi_control_num, bool ignore_channel)
{
	if ((event == NULL) ||(map == NULL) || 
		(ch < 0) || (ch > 15) || (midi_control_num < 0) || (midi_control_num > 127))
	{
		// non valid data
		return -2;
	}
	
	midi_control_event_t control_event;  
	control_event.ch = ch;
	control_event.control_num = midi_control_num;
	
	std::map<midi_control_event_t, synth_control_t >::iterator it;
	std::map<int, synth_control_t>::iterator it_nc;

	if (ignore_channel)
	{
		it_nc = midi_map_nc.find(midi_control_num);
		if (it_nc == midi_map_nc.end())
		{
			//event not mapped
			return -1;
		}
		else
		{
			event->module_id = (*it_nc).second.module_id;
			event->control_id = (*it_nc).second.control_id;

			return 0;
		}
	}
	else
	{
		it = midi_map.find(control_event);
		if (it == midi_map.end())
		{
			//event not mapped
			return -1;
		}
		else
		{
			event->module_id = (*it).second.module_id;
			event->control_id = (*it).second.control_id;

			return 0;
		}
	}	
}

/**
*   @brief  Look for a kbd_map_param element in XML string and returns element values
*   @param  path	xmltext		XML string
*   @return a vector of elements of type mapElement_t ; 
*/
std::vector<map_element_t> MidiControlMapper::get_xml_map_param(const std::string &xml_text)
{
	std::vector<map_element_t> map_elements;
	std::string map_element_string;
	std::vector <std::string> int_params_vector;
	map_element_t element;
	int pos = 0, start;
	std::string param_name;
	int int_param_value;
	XML_files *xml_files = new XML_files();

	while (true)
	{
		start = xml_text.find("<kbd_map_param", pos);
		if (start == string::npos)
		{
			// find failed - end
			return map_elements;
		}

		start = xml_text.find(">", start);
		start++;

		pos = xml_text.find("</kbd_map_param", start);
		if (pos == string::npos)
		{
			// find failed
			return map_elements;
		}
		// Get map element parameters string
		map_element_string = xml_text.substr(start, pos - start);
		// break to integer params
		int_params_vector = xml_files->get_int_params(map_element_string);

		// Init element
		element.midi_control_event.ch = -1;
		element.midi_control_event.control_num = -1;
		element.synth_control.module_id = -1;
		element.synth_control.control_id = -1;
		// Get values
		for (auto &elmnt : int_params_vector)
		{
			param_name = xml_files->get_element_name(elmnt);
			int_param_value = xml_files->get_int_element_value(elmnt);

			if (param_name == "channel")
			{
				element.midi_control_event.ch = int_param_value;
			}
			else if (param_name == "control_num")
			{
				element.midi_control_event.control_num = int_param_value;
			}
			else if (param_name == "module_id")
			{
				element.synth_control.module_id = int_param_value;
			}
			else if (param_name == "control_id")
			{
				element.synth_control.control_id = int_param_value;
			}
		}

		map_elements.push_back(element);
	}	
}

/**
*   @brief  Saves a midi keyboard controls map into a file
*   @param  path	map file full path
*   @return 0 if OK; 
*/
int MidiControlMapper::save_midi_keyboard_control_map_file(string path)
{
	int res = 0;
	XML_files *xml_files = new XML_files();
	vector<string> xml_file_string;
	std::string file_name, file_name_no_ext;
	std::string type_str;

	return_val_if_true(path == "", -1); // Bad param

	// Build file headers
	file_name = xml_files->remove_file_extention(xml_files->get_xml_file_name(path));
	return_val_if_true(file_name == "", -2); // Bad path

	file_name_no_ext = xml_files->remove_file_extention(file_name);
	return_val_if_true(file_name_no_ext == "", -2);

	xml_file_string.push_back(xml_files->comment("AdjHeart Modular Synthesizer Kyboard Mapping File"));
	xml_file_string.push_back(xml_files->start_tag("kbd_map", 0, true));

	xml_file_string.push_back(xml_files->string_val_element("string_param", "name", file_name, 4));
	xml_file_string.push_back(xml_files->int_val_element("int_param", "version", version, 4));

	// Go allover mapping pairs
	std::map<midi_control_event_t, synth_control_t>::iterator it = midi_map.begin();
	while (it != midi_map.end())
	{
		xml_file_string.push_back(xml_files->start_tag("kbd_map_param", 4));
		xml_file_string.push_back(xml_files->int_val_element("int_param", "channel", it->first.ch, 8));
		xml_file_string.push_back(xml_files->int_val_element("int_param", "control_num", it->first.control_num, 8));
		xml_file_string.push_back(xml_files->int_val_element("int_param", "module_id", it->second.module_id, 8));
		xml_file_string.push_back(xml_files->int_val_element("int_param", "control_id", it->second.control_id, 8));
		xml_file_string.push_back(xml_files->end_tag("kbd_map_param", 4));
		it++;
	}

	xml_file_string.push_back(xml_files->end_tag("kbd_map", 0));

	res = xml_files->write_xml_file(xml_files->remove_file_extention(path).c_str(), &xml_file_string);

	return res;
}

/**
*   @brief  Loads a midi keyboard controls map from a file
*   @param  path	map file full path
*   @return 0 if OK; 
*/
int MidiControlMapper::load_midi_keyboard_control_map_file(string path)
{
	XML_files *xml_files = new XML_files();
	std::string read_file_data;
	int res = 0, res1;
	std::vector<map_element_t> map_elements;

	return_val_if_true(path == "", -1); // Bad param

	res = xml_files->read_xml_file(xml_files->remove_file_extention(path).c_str(), &read_file_data);
	if (res != 0)
	{
		return -2; // File read error
	}

	if (xml_files->element_exist(read_file_data, "kbd_map"))
	{
		map_elements = get_xml_map_param(read_file_data);
		for (int i = 0; i < map_elements.size(); i++)
		{
			last_midi_control_channel = map_elements.at(i).midi_control_event.ch;
			last_midi_control_num = map_elements.at(i).midi_control_event.control_num;
			last_synth_control_module = map_elements.at(i).synth_control.module_id;
			last_synth_control_id = map_elements.at(i).synth_control.control_id;

			add_mapped_midi_control_sequence_event();
		}
	}
	else
	{
		return -3;	// Not a keyboard mapping file
	}

	return res;
}

/**
*   @brief  Sets the in-focus (GUI) module and control (enable remote setting) 
*   @param  module	module id (-1 indicates no valid selection)
*   @param	control control id (-1 indicates no valid selection)
*   @param	type control type (dial, slider... ; -1 indicates no valid selection) 
*   @return 0 if OK; 
*/
int MidiControlMapper::set_in_focus_module_control(int module, int control, int type)
{
	in_focus_control.control_id = control;
	in_focus_control.module_id = module;
	in_focus_control.type = type;

	return 0;
}

/**
*   @brief  Gets the in-focus (GUI) module Id 
*   @param  none
*   @return module id (-1 if no valid control in focus); 
*/
int MidiControlMapper::get_in_focus_module_id()
{
	return in_focus_control.module_id;
}

/**
*   @brief  Gets the in-focus (GUI) control Id 
*   @param  none
*   @return control id (-1 if no valid control in focus); 
*/
int MidiControlMapper::get_in_focus_control_id()
{
	return in_focus_control.control_id;
}

/**
*   @brief  Gets the in-focus (GUI) control type 
*   @param  none
*   @return control type (-1 if no valid control in focus); 
*/
int MidiControlMapper::get_in_focus_control_type()
{
	return in_focus_control.type;
}
