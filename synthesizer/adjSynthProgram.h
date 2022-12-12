/**
*	@file		adjSynthProgram.h
*	@author		Nahum Budin
*	@date		4-Feb-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and audio block-size handling
*					
*	@version	1.0		15-Nov-2019 
*
*	@brief		Handle AdjSynth MIDI programs
*				A MIDI program is a adjSyntg preset (voice).
*/

#ifndef _ADJ_SYNTH_PROGRAM
#define _ADJ_SYNTH_PROGRAM

#include <stdint.h>

#include "synthSettings.h"
#include "adjSynthPADcreator.h"
#include "adjSynthVoice.h"
#include "../libAdjHeartModSynth_2.h"
#include "../audio/audioManager.h"
#include "../dsp/dspMorphedSineOsc.h"

/*
typedef struct voice_resource
{
	voice_resource()
		: id(-1)
		, used(false)
		, pending(false)
		, note(-1)
		, timestamp(0) {}
	int id;
	bool used;
	bool pending;
	int note;
	uint64_t timestamp;
} voice_resource_t;
*/

class SynthProgram
{
public:

	SynthProgram(
		int samp_rate = _DEFAULT_SAMPLE_RATE,
		int block_size = _DEFAULT_BLOCK_SIZE,
		int voices = 2,
		int first_v_index = 0,
		int wt_size = _PAD_DEFAULT_WAVETABLE_SIZE,
		AudioManager *aud_mng = NULL );

	~SynthProgram();
	
	void register_set_patch_settings_default_params_callback_ptr(func_ptr_int_settings_parms_ptr_int_t ptr);
	int activate_set_patch_settings_default_params_callback(_setting_params_t* params, int prog);
	
	
	void register_mark_voice_bussy_callback_ptr(func_ptr_void_int_t ptr);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();

	void set_num_of_voices(int nov);
	int get_num_of_voices();

	void set_program_patch_params(_setting_params_t *patch_params);

	void set_portamento_time(float porta);
	void set_portamento_time(int porta);
	float get_portamento_level();

	void enable_portamento();
	void disable_portamento();
	bool portamento_is_enabled();

	float get_note_frequency();
	void update_actual_frequency();

	SynthVoice *get_free_voice();
	void free_voice(int voice);

	SynthVoice *synth_voices[_SYNTH_MAX_NUM_OF_VOICES] = { NULL };

	ModSynthSettings *settings_manager = NULL; 
	_setting_params_t active_patch_params, prev_active_patch_params_x;  

	DSP_MorphingSinusOscWTAB *mso_wtab = NULL;

	// synthPAD wavetable
	SynthPADcreator *synth_pad_creator = NULL;
	Wavetable *program_wavetable = NULL;
	

private:
	
	AudioManager *audio_manager = NULL;
	int prog_num;
	// Global program number identifier allocator
	static int prog_numbers;
	// Maximum number of polyphonic voices assigned to this program
	int num_of_voices;
	// Indicates 1st voice index out of all synthesizer voices. 
	// e.g. firstVoiceIndex = 10 and numOfVoices = 12 => program voices: 10 to 21.
	int first_voice_index;

	bool portamento_enabled;
	// Portamento gliding time
	float portamento_time;
	// Portamento frequency glide factor (1-iRrefreshRate/portaTime/iSAMPLE_RATE) */
	float porata_div = 1.0f;
	// Holds previous note num 
	int prev_note_ind;
	// Indicates if this Note is higher (1) or lower (-1) than the previous note 
	int note_diff;
	//  Note frequency 
	float note_freq = 440.0f;
	// Actual frequency when deviating (e.g. portamento glide) 
	float actual_freq = 440.0f;

	struct timeval start_time;
	
	int sample_rate, audio_block_size;
	
	func_ptr_int_settings_parms_ptr_int_t set_patch_settings_default_params_callback_ptr = NULL;
	func_ptr_void_int_t mark_voice_bussy_callback_ptr = NULL;
};

#endif

