/**
* @file		JackConfigurationFile.h
*	@author		Nahum Budin
*	@date		13-Feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*					2. Move from GUI level to lib level
*					
*
*	@brief		Jack Audio configuration handling.
*	
*	History:
*		version 1.0		13-Apr-2020		1st version
*	
*	Example of file:
*	
*	#This is the AdjModSynth JACK Audio configuration file.
*	#Last update: April 13 2020.
*
*	#Do not change the header lines that start with !
*	
*	#Coments lines starts with # and are ignored.
*	
*	!Processes
*	#Current version supports up to 5 processes
*	#JMETERS is a JACK Audio level meters application.
*	jmeters -t din -c 2 L R
*	!Processes_end
*	
*	!Audio
*	#Set the QjackCtl audio connectios using the jack_connect command
*	adjheart_fluid_synth:left jmeters:in-1
*	adjheart_fluid_synth:right jmeters:in-2
*	AdjHeartSynth_out:AdjHeartSynthL_out jmeters:in-1
*	AdjHeartSynth_out:AdjHeartSynthR_out jmeters:in-2
*	adjheart_fluid_synth:left AdjHeartSynth_in:AdjHeartSynthL_in
*	adjheart_fluid_synth:right AdjHeartSynth_in:AdjHeartSynthR_in
*	AdjHeartSynth_out:AdjHeartSynthL_out AdjHeartSynth_in:AdjHeartSynthL_in
*	AdjHeartSynth_out:AdjHeartSynthR_out AdjHeartSynth_in:AdjHeartSynthR_in
*	
*	!Audio_end
*	
*	!Alsa_midi
*	#Set the ALSA MIDI connections using the aconnect command
*	#Novation Impulse
*	24:0 130:0
*	#USB MIDI converter HC35
*	20:0 130:0
*	!Alsa_midi_end
*
*
*/

#ifndef _JACK_CONFIG_FILE
#define _JACK_CONFIG_FILE

#include <string>
#include <vector>

#define _COMMENT_START						'#'

#define _MAX_NUM_OF_PROCESSES				5

#define _PROCESSES_SECTION_SRART_HEADER		"!Processes"
#define _PROCESSES_SECTION_END_HEADER		"!Processes_end"

#define _AUDIO_SECTION_SRART_HEADER			"!Audio"
#define _AUDIO_SECTION_END_HEADER			"!Audio_end"

#define _ALSA_MIDI_SECTION_SRART_HEADER		"!Alsa_midi"
#define _ALSA_MIDI_SECTION_END_HEADER		"!Alsa_midi_end"

class JackConfigurationFile 
{
public:
	JackConfigurationFile(std::string path);
	
	int read_configuration_file();
	int get_config_text_lines();
	int get_section_text_lines(std::vector<std::string> *lines, std::string start_header, std::string end_header);
	
	int execute_configuration();
	
	
private:
	std::string file_path;
	std::string file_data;
	std::vector<std::string> configTextLines;
	
	bool file_is_read;
};

#endif
