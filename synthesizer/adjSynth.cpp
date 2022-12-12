/**
*	@file		adjSynth.cpp
*	@author		Nahum Budin
*	@date		4-Feb--2021
*	@version	1.2 
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and bloc-size settings
*					3. Adding start/stop audio.
*
*	@brief		A collection of 4 synthesizers: Additive, Karplus String, PAD and Morphed Sine Oscilator (MSO)
*	
*	History:\n
*	
*	version	1.1		18-Jan-2021: Adding audio parameters settings
*	version 1.0		15_Nov-2019: First version
*/

#include <mutex>

#include <omp.h>

#include "adjSynth.h"
#include "../utils/utils.h"
#include "../audio/audioBlock.h"
#include "../audio/audioBandEqualizer.h"
#include "../jack/jackAudioClients.h"
#include "audioPoliphonyMixer.h"
#include "../midi/midiStream.h"

#include "../libAdjHeartModSynth_2.h"

AdjSynth *AdjSynth::adj_synth = NULL;
SynthVoice *AdjSynth::synth_voice[_SYNTH_MAX_NUM_OF_VOICES] = {NULL};
AdjSynthPolyphony *AdjSynth::synth_polyphony = NULL;

int AdjSynth::num_of_cores = 1;
int AdjSynth::num_of_core_voices = 1;

// Mutex to handle note on / off opperations
pthread_mutex_t voice_manage_mutex;
// Mutex to handle busy/not-busy voices marking
pthread_mutex_t voice_busy_mutex;
// Mutex to controll audio memory blocks allocation
pthread_mutex_t voice_mem_blocks_allocation_control_mutex;

// Callback that is initiated by the AudioManager audio-update thread
void callback_audio_voice_update(int voice_num)
{
	if (AdjSynth::synth_voice[voice_num]->audio_voice->is_voice_active() ||
		AdjSynth::synth_voice[voice_num]->audio_voice->is_voice_wait_for_not_active())
	{
		AdjSynth::synth_voice[voice_num]->update_all();
	}
}

// Callback that is initiated by the AudioManager audio-update thread.
void callback_audio_update_cycle_end_tasks(int param)
{
	AudioBlockFloat *p;
	for (p = *AdjSynth::get_instance()->audio_poly_mixer->audio_first_update; p; p = p->audio_next_update)
	{
		p->update();
	}
}

// Callback that is initiated by SynthProg
int set_patch_settings_default_params_callback_wrapper(_setting_params_t *params, int prog)
{
	return AdjSynth::get_instance()->set_default_patch_parameters(params, prog);
}

// Call back intiated by dspVoice when voice ends (energy decayed to zero)
//void callback_voice_end(int voice)
//{

//}

AdjSynth::AdjSynth()
{
	int voice;
	
	adj_synth = this;

	// Create settings managers
	adj_synth_settings_manager = new ModSynthSettings(&active_adj_synth_settings_params);
	//adj_synth_general_settings_manager = new ModSynthSettings(&active_adj_synth_general_settings_params);
	
	num_of_voices = _SYNTH_MAX_NUM_OF_VOICES;
	num_of_programs = _SYNTH_MAX_NUM_OF_PROGRAMS;
	utilization = 0;
	hammond_percussion_on = false;
	hammond_percussion_slow = false;
	hammond_percussion_soft = false; 
	hammond_ercussion_3_rd = false; 
	active_sketch = _SKETCH_PROGRAM_1;

	// Allocate audio blocks data memory pool
//	AllocateAudioMemoryBlocksFloatPool(_MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE, audio_block_size); // moved down after setting sample-rate and block size

	// Allocate midi blocks data memory pool
	allocate_midi_stream_messages_memory_pool(_MAX_MIDI_STREAM_MESSAGES_POOL_SIZE);
	allocate_raw_data_mssgs_memory_pool(_MAX_RAWDATA_MSSGS_POOL_SIZE);
	
	// Get number of cpu cores
	num_of_cores = omp_get_num_procs();
	if (num_of_cores < 1)
	{
		num_of_cores = 1;
	}

	// Number of voices per core
	num_of_core_voices = mod_synth_get_synthesizer_num_of_polyphonic_voices() / num_of_cores;

	// Init mutexes
	pthread_mutex_init(&voice_manage_mutex, NULL);
	pthread_mutex_init(&voice_busy_mutex, NULL);
	pthread_mutex_init(&voice_mem_blocks_allocation_control_mutex, NULL);

	set_sample_rate(_DEFAULT_SAMPLE_RATE);
	set_audio_block_size(_DEFAULT_BLOCK_SIZE);

	// Allocate audio blocks data memory pool
	AllocateAudioMemoryBlocksFloatPool(_MAX_AUDIO_BLOCKS_MESSAGES_POOL_SIZE, audio_block_size);
	
	// Create a Keyboard instance
	kbd1 = new SynthKeyboard(sample_rate, audio_block_size);
	// Do not change with patch
	poly_mode = _KBD_POLY_MODE_REUSE;  // _KBD_POLY_MODE_FIFO;
	play_mode = _PLAY_MODE_POLY;
	midi_mapping_mode = _MIDI_MAPPING_MODE_SKETCH;
	
	synth_polyphony = AdjSynthPolyphony::get_instance();	// new AdjSynthPolyphony();
	
	// Don't change the order (stage - TODO:)
	audio_manager = AudioManager::get_instance();
	audio_manager->set_sample_rate(sample_rate);
	audio_manager->set_audio_block_size(audio_block_size);
	audio_manager->set_period_time_us(((unsigned long)audio_block_size*1000000)/sample_rate + 0.5);	
	
	audio_manager->register_callback_audio_voice_update(&callback_audio_voice_update);
	audio_manager->register_callback_audio_update_cycle_end_tasks(&callback_audio_update_cycle_end_tasks);
	

	program_wavetable = new Wavetable();
	program_wavetable->size = _PAD_DEFAULT_WAVETABLE_SIZE;
	program_wavetable->samples = (float*)malloc(_PAD_DEFAULT_WAVETABLE_SIZE * sizeof(float));
	synth_pad_creator = new SynthPADcreator(program_wavetable, program_wavetable->size, sample_rate);
	program_wavetable->base_freq =
		synth_pad_creator->set_base_frequency(program_wavetable, _PAD_DEFAULT_BASE_NOTE);

	mso_wtab = new DSP_MorphingSinusOscWTAB(sample_rate);
	mso_wtab->calc_segments_lengths(&mso_wtab->base_segment_lengths, &mso_wtab->base_segment_positions);
	mso_wtab->calc_wtab(mso_wtab->base_waveform_tab, &mso_wtab->base_segment_lengths, &mso_wtab->base_segment_positions);
	mso_wtab->calc_segments_lengths(&mso_wtab->morphed_segment_lengths, &mso_wtab->morphed_segment_positions);
	mso_wtab->calc_wtab(mso_wtab->morphed_waveform_tab, &mso_wtab->morphed_segment_lengths, &mso_wtab->morphed_segment_positions);
		
	// TODO:
	audio_poly_mixer = AudioPolyMixerFloat::get_instance(
				_AUDIO_STAGE_0, audio_block_size, _DEFAULT_MIDI_MAPPING_MODE, 
				mod_synth_get_synthesizer_num_of_polyphonic_voices(), 
				&audio_common_first_update);

	audio_poly_mixer->set_active();

	audio_equalizer = new AudioBandEqualizer(_AUDIO_STAGE_0, audio_block_size, &audio_common_first_update);

	audio_reverb = new AudioReverb(_AUDIO_STAGE_0, sample_rate, audio_block_size, &audio_common_first_update);

	audio_out = new AudioOutputFloat(_AUDIO_STAGE_0, audio_block_size,
		audio_manager->audio_block_stereo_float_shared_memory_outputs,
		&audio_common_first_update);

	connection_mixer_out_L = audio_manager->connections_manager->get_audio_connection();
	connection_mixer_out_R = audio_manager->connections_manager->get_audio_connection();	
	
	connection_equilizer_out_L = audio_manager->connections_manager->get_audio_connection();
	connection_equilizer_out_R = audio_manager->connections_manager->get_audio_connection();
	
	connection_reverb_out_L = audio_manager->connections_manager->get_audio_connection();
	connection_reverb_out_R = audio_manager->connections_manager->get_audio_connection();
	
	connection_mixer_send_L = audio_manager->connections_manager->get_audio_connection();
	connection_mixer_send_R = audio_manager->connections_manager->get_audio_connection();
	
	connection_mixer_out_L->connect(audio_poly_mixer, _LEFT, audio_equalizer, _LEFT);
	connection_mixer_out_R->connect(audio_poly_mixer, _RIGHT, audio_equalizer, _RIGHT);
	
	connection_mixer_send_L->connect(audio_poly_mixer, _SEND_LEFT, audio_reverb, _LEFT);
	connection_mixer_send_R->connect(audio_poly_mixer, _SEND_RIGHT, audio_reverb, _RIGHT);
	
	connection_reverb_out_L->connect(audio_reverb, _LEFT, audio_equalizer, _SEND_LEFT);
	connection_reverb_out_R->connect(audio_reverb, _RIGHT, audio_equalizer, _SEND_RIGHT);
	
	connection_equilizer_out_L->connect(audio_equalizer, _LEFT, audio_out, _LEFT);
	connection_equilizer_out_R->connect(audio_equalizer, _RIGHT, audio_out, _RIGHT);
	
	set_audio_driver_type(_DEFAULT_AUDIO_DRIVER);
	set_sample_rate(_DEFAULT_SAMPLE_RATE);
	set_audio_block_size(_DEFAULT_BLOCK_SIZE);
	
	//	startCheackCpuUtilizationThread();	
}

AdjSynth::~AdjSynth()
{
	
}

/**
*   @brief  retruns the single AdjSynth instance
*   @param  none
*   @return the single modular synth instance
*/
AdjSynth *AdjSynth::get_instance()
{
	if (adj_synth == NULL)
	{
		adj_synth = new AdjSynth();
	}
	
	return adj_synth;
}

/**
*	@brief	Sets the sample-rate
*	@param	sample  rate: _SAMPLE_RATE_44 (44100Hz) or _SAMPLE_RATE_48 (48000Hz)
*					if non of the above, sample rate is set to _DEFAULT_SAMPLE_RATE (44100).
*					Also sets WTAB funfemental and maxfrequencies based on sample rate.
*					
*	@return set sample-rate
*/
int AdjSynth::set_sample_rate(int samp_rate)
{
	if (!is_valid_sample_rate(samp_rate))
	{
		sample_rate = _DEFAULT_SAMPLE_RATE;
	}
	else
	{
		sample_rate = samp_rate;
	}

	// TODO: reallocate audio blocks.

	if (kbd1)
	{
		kbd1->set_sample_rate(sample_rate);
	}

	if (audio_manager)
	{
		audio_manager->set_period_time_us(((unsigned long)audio_block_size * 1000000) / sample_rate + 0.5);
	}

	if (mso_wtab)
	{
		mso_wtab->set_sample_rate(sample_rate);
	}

	if (audio_reverb)
	{
		audio_reverb->set_sample_rate(sample_rate);
	}

	return sample_rate;
}

/**
*	@brief	Returns the set sample-rate
*	@param	none
*	@return set sample-rate
*/	
int AdjSynth::get_sample_rate() { return sample_rate; }

/**
*   @brief  sets the audio block size 
*   @param  int size: _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024
*   @return size if OK; -1 param out of range
*/
int AdjSynth::set_audio_block_size(int size)
{	
	if (is_valid_audio_block_size(size))
	{
		audio_block_size = size;

		if (kbd1)
		{
			kbd1->set_audio_block_size(audio_block_size);
		}

		if (audio_manager)
		{
			audio_manager->set_period_time_us(((unsigned long)audio_block_size * 1000000) / sample_rate + 0.5);
		}

		if (audio_poly_mixer)
		{
			audio_poly_mixer->set_audio_block_size(audio_block_size);
		}

		if (audio_equalizer)
		{
			audio_equalizer->set_audio_block_size(audio_block_size);
		}

		if (audio_reverb)
		{
			audio_reverb->set_audio_block_size(audio_block_size);
		}

		if (audio_out)
		{
			audio_out->set_audio_block_size(audio_block_size);
		}

		return audio_block_size;
	}
	else
	{
		return -1;
	}
}
	
/**
*   @brief  retruns the audio block size  
*   @param  none
*   @return buffer size
*/	
int AdjSynth::get_audio_block_size() { return audio_block_size; }

/**
*	@brief	Sets the audio driver type
*			Settings will be effective only after the next call to start_audio().
*	@param	driver  _AUDIO_JACK, _AUDIO_ALSA (default: _DEFAULT_AUDIO)
*	@return set audio-driver
*/
int AdjSynth::set_audio_driver_type(int driver)
{
	if (is_valid_audio_driver(driver))
	{
		audio_driver_type = driver;
	}
	else
	{
		audio_driver_type = _DEFAULT_AUDIO_DRIVER;
	}
	
	return audio_driver_type;
}

/**
*	@brief	Returns the audio-driver type
*	@param	none
*	@return audio driver type
*/	
int AdjSynth::get_audio_driver_type() { return audio_driver_type; }

/**
*   @brief  Starts the audio service.
*			Must be called after intializtion.
*   @param  none
*   @return 0 if done
*/
int AdjSynth::start_audio(int driver, int samp_rate, int block_size)
{
	int res = 0;
	
	stop_audio();
	
	set_audio_driver_type(driver);
	set_sample_rate(samp_rate);
	set_audio_block_size(block_size);
	
	return audio_manager->start_audio_service(driver, sample_rate, audio_block_size);
}

/**
*   @brief  Stops the audio service.
*   @param  none
*   @return 0 if done
*/
int AdjSynth::stop_audio()
{
	return audio_manager->stop_audio_service();
}

/**
*   @brief  retruns a pointer to an original main dsp voice 
*   @param  voice voice number
*   @return  pointer to an original main dsp voice if OK; NULL if param out of range
*/	
DSP_Voice *AdjSynth::get_original_main_dsp_voices(int voice)
{
	if ((voice >= 0) && (voice < mod_synth_get_synthesizer_num_of_polyphonic_voices()))
	{	
		return original_main_dsp_voices[voice];
	}
	else
	{
		return NULL;
	}
}

/**
*   @brief  Set synthvoice instances to point at program[0] voices //Create and initialize the synth voice instances
*   @param  none
*   @return void
*/
void AdjSynth::init_synth_voices()
{
	for (int voice = 0; voice < mod_synth_get_synthesizer_num_of_polyphonic_voices(); voice++)
	{
		synth_voice[voice] = new SynthVoice(voice, active_sketch, sample_rate, audio_block_size, 
											 &active_adj_synth_patch_params, mso_wtab, program_wavetable,
											audio_manager);
		
		//		originalMainDspVoices[voice] = synth_voice[voice]->dspVoice;
				
//		synth_voice[voice] = synth_program[0]->synth_voices[voice];		
		
		
		// Assign the program voice to the original voice
		synth_voice[voice]->assign_dsp_voice(synth_program[active_sketch]->synth_voices[voice]->dsp_voice);
		// Assingn LUTs
		synth_voice[voice]->mso_wtab = synth_program[active_sketch]->mso_wtab;
		synth_voice[voice]->pad_wavetable = synth_program[active_sketch]->program_wavetable;
	}
}

/**
*   @brief  Create and initialize the synth programs instances
*   @param  none
*   @return void
*/
void AdjSynth::init_synth_programs()
{
	// Program[0] is the active non MIDIping mode program used for editting patches, etc.
	// Program[1] to Program[16] are MIDI-mapping mode programs used each for a MIDI channel 1-16 
	for(int program = 0 ; program < mod_synth_get_synthesizer_num_of_programs(); program++)
	{		
		synth_program[program] = new SynthProgram(
				sample_rate,
				audio_block_size,			
				num_of_voices,								
				0,						// 1st voice num'
				_PAD_DEFAULT_WAVETABLE_SIZE,
				audio_manager);
		
//		set_default_patch_parameters(&synth_program[program]->active_patch_params, program);

//		synth_program[program]->register_set_patch_settings_default_params_callback_ptr(&set_patch_settings_default_params_callback_wrapper);
//		synth_program[program]->activate_set_patch_settings_default_params_callback(
//			&synth_program[program]->active_patch_params,
//			program);
	}
}

/**
*   @brief   set the adj synth master volume
*   @param  vol	volume level 0-100
*   @return void
*/
void AdjSynth::set_master_volume(int vol)
{
	if ((vol >= 0) && (vol <= 100))
	{
		master_volume = vol;
		audio_out->set_master_volume((float)vol / 100.f);
	}
}

/**
*   @brief  get the adj synth master volume
*   @param	none 
*   @return volume level 0-100
*/
int AdjSynth::get_master_volume()
{
	return master_volume;
}

/**
*   @brief  Init AdjSynth settings parameters (keyboard,equilizer, reverb)
*   @param  none
*   @return void
*/
int AdjSynth::init_synth_settings_params(_setting_params_t *settings_params)
{
	if (settings_params == NULL)
	{
		return -1;
	}
	
	int res = 0;

	_mod_synth_settings_bool_param_t bool_param;
	_mod_synth_settings_int_param_t int_param;

	res = adj_synth_settings_manager->get_bool_param(settings_params, "adjsynth.reverb3m.enable_state", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		if (bool_param.value)
		{
			audio_reverb->rev3m_enable();
		}
		else
		{
			audio_reverb->rev3m_disable();
		}
	}

	res |= adj_synth_settings_manager->get_bool_param(settings_params, "adjsynth.reverb.enable_state", &bool_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		if (bool_param.value)
		{
			audio_reverb->rev_enable();
		}
		else
		{
			audio_reverb->rev_disable();
		}
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.reverb3m.preset", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		sf_presetreverb(&audio_reverb->rv3m, _AUDIO_SAMPLE_RATE, (sf_reverb_preset)int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.reverb.room_size", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_reverb->reverb->set_room_size(int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.reverb.damp", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_reverb->reverb->set_damp(int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.reverb.wet", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_reverb->reverb->set_wet(int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.reverb.dry", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_reverb->reverb->set_dry(int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.reverb.mode", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_reverb->reverb->set_mode(int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_31_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_31_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_31_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_62_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_62_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_62_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_125_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_125_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_125_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_250_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_250_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_250_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_500_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_500_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_500_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_1k_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_1000_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_1000_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_2k_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_2000_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_2000_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_4k_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_4000_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_4000_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_8k_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_8000_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_8000_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.band_16k_level", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_band_level(_band_16000_hz, int_param.value);
		audio_equalizer->band_equalizer_R->set_band_level(_band_16000_hz, int_param.value);
	}

	res |= adj_synth_settings_manager->get_int_param(settings_params, "adjsynth.equilizer.preset", &int_param);
	if (res == _SETTINGS_KEY_FOUND)
	{
		audio_equalizer->band_equalizer_L->set_preset(int_param.value);
		audio_equalizer->band_equalizer_R->set_preset(int_param.value);
	}
	
	// TODO: Mixer
	
	return res;	
}

/**
*   @brief  Retruns the actual max num of voices
*   @param  none
*   @return the actual max num of voices
*/
int AdjSynth::get_num_of_voices() { return num_of_voices; }

/**
*   @brief  Retruns the actual max num of Midi Programs
*   @param  none
*   @return the actual max num of Midi Programs
*/
int AdjSynth::get_num_of_programs() { return num_of_programs; }

/**
*   @brief  Sets the active sketch num
*   @param  apt	the active sketch num _SKETCH_PROGRAM_1 to _SKETCH_PROGRAM_3
*   @return void
*/
void AdjSynth::set_active_sketch(int ask)
{
	if ((ask >= _SKETCH_PROGRAM_1) && (ask <= _SKETCH_PROGRAM_3))
	{
		active_sketch = ask;
	}
}

/**
*   @brief  Retruns the active sketch num
*   @param  none
*   @return the active sketch num _SKETCH_1 to _SKETCH_3
*/
int AdjSynth::get_active_sketch() { return active_sketch; }

/**
*   @brief  Copy source sketch params to a destination sketch params
*   @param  int srcsk	sorce sketch num _SKETCH_PROGRAM_1 to _SKETCH_PROGRAM_3
*   @param  int destsk	destination sketch num _SKETCH_PROGRAM_1 to _SKETCH_PROGRAM_3
*   @return 0 if done
*/
int AdjSynth::copy_sketch(int srcsk, int destsk)
{
	if ((srcsk == destsk) || (srcsk < _SKETCH_PROGRAM_1) || (srcsk > _SKETCH_PROGRAM_3) ||
		(destsk < _SKETCH_PROGRAM_1) || (destsk > _SKETCH_PROGRAM_3))
	{
		return -1;
	}
	else
	{
		synth_program[destsk]->set_program_patch_params(&synth_program[srcsk]->active_patch_params);
	}

	return 0;
}

/**
*   @brief  Initilize the voices polyphonic state and paramters
*   @param  none
*   @return void
*/
void AdjSynth::init_poly()
{
	for (int voice = 0; voice < mod_synth_get_synthesizer_num_of_polyphonic_voices(); voice++)
	{
		if (synth_voice[voice] != NULL)
		{	
			synth_voice[voice]->audio_voice->init_poly();
		}
		mark_voice_not_busy_callback(voice);

		for (int core = 0; core < mod_synth_get_number_of_cores(); core++)
		{	
			synth_polyphony->clear_busy_core_voices_count(core);
		}

		for (int program = 0; program < mod_synth_get_synthesizer_num_of_programs(); program++)
		{
			if (synth_program[program]->synth_voices[voice] != NULL)
			{	
				synth_program[program]->synth_voices[voice]->dsp_voice->not_in_use();
			}
		}
	}
}

/**
*   @brief  Start the jack out thread processing and connect to the Jackserver
*   @param	none
*   @return void
*/
void AdjSynth::init_jack()
{
	int res;
	sleep(1);
	res = initialize_jack_server_interface();
//	res |= intilize_jack_server_connection_out("AdjHeartSynth_out", "default");
//	res |= intilize_jack_server_connection_in("AdjHeartSynth_in", "default");
}

/**
*   @brief  Start the audio update process.
*   @param	none
*   @return void
*/
//void AdjSynth::start_audio()
//{
//#ifdef _USE_ALSA
//	audioManager->startAudioService(_AUDIO_ALSA);
//#else
//	audio_manager->start_audio_service(_AUDIO_JACK);
//#endif
//}

/**
*   @brief  Set MIDI mapping mode.
*			In non mapping mode, MIDI channel is ignored.
*			Non mapping mode enabled editing and creating patches.
*			In mapping mode, patches can only be loaded into each program/channel.
*   @param	mod	_MIDI_MAPPING_MODE_NON_MAPPING or _MIDI_MAPPING_MODE_MAPPING
*   @return void
*/
void AdjSynth::set_midi_mapping_mode(int mod)
{
	if (mod == _MIDI_MAPPING_MODE_SKETCH)
	{
		midi_mapping_mode = _MIDI_MAPPING_MODE_SKETCH;
		
		pthread_mutex_lock(&voice_manage_mutex);

		//		for (int voice = 0; voice < _SYNTH_MAX_NUM_OF_VOICES; voice++)
		//		{
					// Assign all voices their original (non-mapung mode) params.
					//			synth_voice[voice]->assignDspVoice(originalMainDspVoices[voice]);
					//			synth_voice[voice]->msoLUT = synth_voice[voice]->dspVoice->original_msoLUT;
					//			synth_voice[voice]->padWavetable = synth_voice[voice]->dspVoice->original_padWavetable;

					// Set all voices settings to point at porogram[0] active patch settings
					//			init_synth_voices();
					//		}
		
					//		init_synth_voices();

		init_poly();

		//	synth_program[0]->set_num_of_voices(num_of_voices);

		pthread_mutex_unlock(&voice_manage_mutex);
	}
	else if (mod == _MIDI_MAPPING_MODE_MAPPING)
	{
		midi_mapping_mode = _MIDI_MAPPING_MODE_MAPPING;
	}

	audio_poly_mixer->set_midi_maping_mode(midi_mapping_mode);
}

int AdjSynth::get_midi_mapping_mode() { return midi_mapping_mode; }

/**
*   @brief  Set the settings handling object and the params structure
*   @param	settings		a pointer to a ModSynthSettings settings handling object
*   @param	patch_param		a pointer to a _setting_params_t params structure holding patch params
*   @param	settings_params	a pointer to a _setting_params_t params structure holding settings params
*   @return 0 if done
*/
int AdjSynth::set_settings_params(ModSynthSettings *settings, 
	//								  /*_setting_params_t *patch_params,*/ 
									  _setting_params_t *settings_params)
{
	return_val_if_true(settings_params == NULL || settings == NULL, _SETTINGS_BAD_PARAMETERS);

	adj_synth_settings_manager = settings;
	active_adj_synth_settings_params = *settings_params;

	active_adj_synth_settings_params.name = "default_settings";
	active_adj_synth_settings_params.settings_type = _PARAM_TYPE_ADJ_SYNTH_SETUP;
	active_adj_synth_settings_params.version = settings->get_settings_version();
	
	
	// Amp level, pan send mixer settings not patch
	
	return 0;
}

/**
*   @brief  Gets a program patch name
*   @param  prog	program num (1-16)
*   @return program patch name
*/
std::string AdjSynth::get_program_patch_name(int prog)
{
	if ((prog >= 0) && (prog <= 16))
	{
		return synth_program[prog]->active_patch_params.name;
	}
	else
	{
		return string("--------");
	}
}

/**
*   @brief  retruns a pointer to the active patch params struct
*   @param  none
*   @return a pointer to the active patch params struct
*/
_setting_params_t *AdjSynth::get_active_patch_params()
{
	return &synth_program[active_sketch]->active_patch_params;
}

/**
*   @brief  retruns a pointer to the active settings params struct
*   @param  none
*   @return a pointer to the active settings params struct
*/
_setting_params_t *AdjSynth::get_active_settings_params()
{
	return &active_adj_synth_settings_params;
}

/**
*   @brief  retruns a pointer to the active general settings params struct
*   @param  none
*   @return a pointer to the active general settings params struct
*/
//_setting_params_t *AdjSynth::get_active_general_settings_params()
//{
//	return &active_adj_synth_general_settings_params;
//}

/**
*   @brief  Set the patch parameters to their default values
*   @param	params	a _setting_params_t parameters struct
*   @return 0 if done
*/
int AdjSynth::set_default_patch_parameters(_setting_params_t *params, int prog)
{
	int res = 0;
	
	return_val_if_true(params == NULL, _SETTINGS_BAD_PARAMETERS);
	
	res = set_default_patch_parameters_vco(params, prog);
	res |= set_default_patch_parameters_noise(params, prog);
	res |= set_default_patch_parameters_kps(params, prog);
	res |= set_default_patch_parameters_mso(params, prog);
	res |= set_default_patch_parameters_pad(params, prog);
	res |= set_default_patch_parameters_amp(params, prog);
	res |= set_default_patch_parameters_distortion(params, prog);
	res |= set_default_patch_parameters_modulators(params, prog);

	res |= set_default_patch_parameters_filter(params, prog);
	
	return res;
}

/**
*   @brief  Set the settings parameters to their default values
*   @param	params	a _setting_params_t parameters struct
*   @return 0 if done
*/
int AdjSynth::set_default_settings_parameters(_setting_params_t *params)
{	
	int res = 0;
	
	res = set_default_settings_parameters_equalizer(params);
	res |= set_default_settings_parameters_reverb(params);
	res |= set_default_settings_parameters_keyboard(params);
	res |= set_default_settings_parameters_mixer(params);

	//	// TODO: GLobal part of modsynth
	//	res |= set_default_settings_parameters_mixer(params);
	//	res |= set_default_settings_parameters_keyboard(params);
	
		return res;
}

/**
*   @brief  Set the general settings parameters to their default values
*   @param	params	a _setting_params_t parameters struct
*   @return 0 if done
*/
/*
int AdjSynth::set_default_general_settings_parameters(_setting_params_t *params)
{
	int res = 0;

	res = set_default_settings_parameters_audio(params);

	//	// TODO: GLobal part of modsynth
	//	res |= set_default_settings_parameters_mixer(params);
	//	res |= set_default_settings_parameters_keyboard(params);

	return res;
}
*/

// UI updates callbacks initiation

void AdjSynth::mark_voice_not_busy_callback(int vnum)
{
	callback_mark_voice_not_busy(vnum);
}

void AdjSynth::mark_voice_busy_callback(int vnum)
{
	callback_mark_voice_busy(vnum);
}

void AdjSynth::set_utilization_callback(int util)
{
	
}

int AdjSynth::get_utilization_callback()
{
	return utilization; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< utilization not set
}


/**
*   @brief  Play note on 
*			Allocate free voice
*			In portamento mode use momophonic voice 0 only.
*			In non mapping mode assign selected free voice with selected program params
*   @param	channel	MIDI channel: 0-15 patc1-3: 16-18
*	@param	byte2	note midi num
*	@param	byte3	note velocity (if set to 0, note off will be executed).
*	@param	voc		voice (NA).
*   @return void
*/
void  AdjSynth::midi_play_note_on(uint8_t channel, uint8_t byte2, uint8_t byte3, int voc)
{
	int voice, core, scaledMagnitude, prog = 0;
	bool reused = false;
	SynthVoice *prog_voice = NULL;
	
	if (midi_mapping_mode == _MIDI_MAPPING_MODE_MAPPING)
	{
		prog = channel;
	}
	else
	{
		prog = active_sketch;
	}

	pthread_mutex_lock(&voice_manage_mutex);

	if (byte3 == 0)
	{
		pthread_mutex_unlock(&voice_manage_mutex);
		midi_play_note_off(channel, byte2, byte3);
		return;
	}

	voice = -1;

	if (kbd1->portamento_is_enabled()) // TODO mobe portamento to programs?
		{
			// TODO: Polyphonic portament?
			synth_voice[_SYNTH_VOICE_1]->audio_voice->set_active();
			synth_voice[_SYNTH_VOICE_1]->audio_voice->reset_wait_for_not_active();
			mark_voice_busy_callback(_SYNTH_VOICE_1);
			voice = _SYNTH_VOICE_1;
		}
	else
	{
		if (poly_mode == _KBD_POLY_MODE_REUSE)
		{
			voice = synth_polyphony->get_reused_note((int)byte2, channel);   // TODO: program = 0
			if(voice > -1)
			{
				reused = true;
			}
		}
	}

	if (voice < 0)
	{
		// Not found yet
		core = synth_polyphony->get_less_busy_core();
		// Look for a free voice
		voice = synth_polyphony->get_free_voice(core);

		if ((voice < 0) && (poly_mode == _KBD_POLY_MODE_FIFO))
		{
			// Not found yet - get the oldest active
			voice = synth_polyphony->get_oldest_voice();
		}
	}

	if ((voice > -1) && !reused)
	{
		/*	moved up	
		 *	if (midi_mapping_mode == _MIDI_MAPPING_MODE_MAPPING)
				{
					prog = channel;
			}
			else
			{
				prog = active_sketch;
		}
*/		
		// Voice found and if not reused, get a free voice from mapped program		
		prog_voice = synth_program[prog]->get_free_voice();
		
		if (!prog_voice)
			voice = -1;
		else
		{
				// Assign the program voice to the free voice
			synth_voice[voice]->assign_dsp_voice(prog_voice->dsp_voice);
			// Assingn LUTs
			synth_voice[voice]->mso_wtab = synth_program[prog]->mso_wtab;			
			synth_voice[voice]->pad_wavetable = synth_program[prog]->program_wavetable;

			synth_voice[voice]->allocated_to_program_num = prog_voice->allocated_to_program_num;
			synth_voice[voice]->allocated_to_program_voice_num = prog_voice->voice_num;
			audio_poly_mixer->preserve_gain_pan(voice);
			audio_poly_mixer->set_voice_gain_1_ptr(voice, prog);
			audio_poly_mixer->set_voice_gain_2_ptr(voice, prog);
			audio_poly_mixer->set_voice_pan_1_ptr(voice, prog);
			audio_poly_mixer->set_voice_pan_2_ptr(voice, prog);
			audio_poly_mixer->set_voice_send_1_ptr(voice, prog);
			audio_poly_mixer->set_voice_send_2_ptr(voice, prog);

			fprintf(stderr,
				"midi_play_note_on: %i voice: %i program: %i allocated to prog %i\n", 
				byte2,
				prog_voice->voice_num,
				prog,
				prog_voice->allocated_to_program_num);
		}
	}
_voice_is_on:
	if ((voice > -1) && (synth_voice[voice] != NULL))
	{
		if (!reused)
		{
			core = voice / num_of_core_voices;
			pthread_mutex_lock(&voice_busy_mutex);
			synth_polyphony->inc_busy_core_voices_count(core);
			pthread_mutex_unlock(&voice_busy_mutex);
			mark_voice_busy_callback(voice);
			printf("Bussy %i %i %i %i\n",
				synth_polyphony->get_busy_core_voices_count(0),
				synth_polyphony->get_busy_core_voices_count(1),
				synth_polyphony->get_busy_core_voices_count(2),
				synth_polyphony->get_busy_core_voices_count(3));
		}
		
		synth_polyphony->activate_resource(voice, (int)byte2, prog);
		//		kbd1->voices[voice].note = byte2;
		synth_voice[voice]->audio_voice->set_note(byte2);
		fprintf(stderr, "On voice %i\n", voice);

		//		if (sequencer1->mainTrack->recording)
		//		{
		//			sequencer1->addEvent((int)byte2, channel, (int)byte3, true, SynthSequencer::getInstance()->mainTrack);
		//		}
		
		kbd1->midi_play_note_on(channel, byte2, byte3);

		if (kbd1->get_split_point() == _KBD_SPLIT_POINT_NONE)
		{
			scaledMagnitude = kbd1->get_scaled_velocity(byte3);
		}
		else if ((int)byte2 >= 12 * (kbd1->get_split_point() + 3))
		{
			scaledMagnitude = kbd1->get_scaled_velocity(byte3);
		}
		else
		{
			scaledMagnitude = kbd1->get_low_scaled_velocity(byte3);
		}

		synth_voice[voice]->dsp_voice->set_voice_frequency(kbd1->get_note_frequency());

		synth_voice[voice]->dsp_voice->filter1->set_kbd_freq(kbd1->get_note_frequency());
		synth_voice[voice]->dsp_voice->filter2->set_kbd_freq(kbd1->get_note_frequency());

		synth_voice[voice]->audio_voice->set_magnitude((float)scaledMagnitude / 127);
		synth_voice[voice]->dsp_voice->karplus1->note_on(byte2, (float)scaledMagnitude / 127);


		//		fprintf(stderr, "%i %f\n", voice, kbd1->getNoteFrequency());

		//		fprintf(stderr, "freq %f\n", kbd1->getNoteFrequency());

		synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr1);
		synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr2);
		synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr3);
		synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr4);
		synth_voice[voice]->dsp_voice->adsr_note_on(synth_voice[voice]->dsp_voice->adsr5);

		//		fprintf(stderr, "\nsynth on %i %i ", byte2, voice);
	}
	else
		fprintf(stderr, "note %i on not found  ", byte2);

	pthread_mutex_unlock(&voice_manage_mutex);
}

/**
*   @brief  Play note off
*			Restore original voice params
*   @param	channel	MIDI channel 0-15
*	@param	byte2	note midi num
*	@param	byte3	note velocity (usually 0).
*	@param	voc		voice (NA).
*   @return void
*/
void  AdjSynth::midi_play_note_off(uint8_t channel, uint8_t byte2, uint8_t byte3, int voc)
{
	int voice = -1, program = 0;

	if (midi_mapping_mode == _MIDI_MAPPING_MODE_MAPPING)
	{
		program = channel;
	}
	else
	{
		program = active_sketch;
	}

	pthread_mutex_lock(&voice_manage_mutex);

	//	while ((voice  -1) /*&& (program < _SYNTH_NUM_OF_PROGRAMS)*/)
	//	{
			// look for the voice number of the voice that is part of the
			// provided program and plays the provided note
			voice = synth_polyphony->get_voice_note(byte2, program);
	//		program++;
	//	}

		if(voice != -1)
	{		
		synth_voice[voice]->dsp_voice->adsr_note_off(synth_voice[voice]->dsp_voice->adsr1);
		synth_voice[voice]->dsp_voice->adsr_note_off(synth_voice[voice]->dsp_voice->adsr2);
		synth_voice[voice]->dsp_voice->adsr_note_off(synth_voice[voice]->dsp_voice->adsr3);
		synth_voice[voice]->dsp_voice->adsr_note_off(synth_voice[voice]->dsp_voice->adsr4);
		synth_voice[voice]->dsp_voice->adsr_note_off(synth_voice[voice]->dsp_voice->adsr5);
		synth_voice[voice]->dsp_voice->karplus1->note_off();
		synth_polyphony->free_voice(voice, true);   // go to pending untill env is zero
		fprintf(stderr, "midi_play_note_off  %i voice: %i prog: %i\n", byte2, voice, program);

		//synthVoice[voice]->assignDspVoice(originalMainDspVoices[voice]);

	}
	else
	{
		fprintf(stderr, "midi_play_note_off %i not found program: %i\n ", byte2, program);
	}
	
	//	if (sequencer1->mainTrack->recording)
	//	{
	//		sequencer1->addEvent((int)byte2, channel, (int)byte3, false, SynthSequencer::getInstance()->mainTrack);
	//	}

		kbd1->midi_play_note_off(channel, byte2, byte3);

	pthread_mutex_unlock(&voice_manage_mutex);
}




