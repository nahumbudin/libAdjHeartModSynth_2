/**
* @file		libAdjHeartModSynth_1.h
*	@author		Nahum Budin
*	@date		23-Jan-2021
*	@version	2.0
*		1. Code refactoring
*		2. Adding local ALSA midi device/clientss handling (not through jack connection kit)
*		3. Adding local JACK clients handling (not through jack connection kit)
*		4. Seperating JACK auto connection of audio and midi
*		5. Adding start/stop audio
*			
*
*	History:\n
*	
*	@version	1.3		9-Jan-2021
*		1. Add support in setting in focus controls using external knob/dial using sysex commands.
*		2. Add support in controloing JACK params and operation
*	
*	@version	1.2
*		1. Add support in Serial Port MIDI input.
*		2. Add ALSA client to support serial port input stream..*
*	
*	@version	1.1:	15-May-2020
*		1. Add JackAudio input client to drive signal level and output waveform display.
*		2. Activating the Pass all mode of the filter.
*		3. Add MIDI control-sequnces training to enable mapping of external midi control-keyboard\n
*		   controls to modSynth settings and parameters changing (e.g. kbd knob changes filter-freq)
*	
*	version 1.0:	5-Oct-2019		
*		First version (based on libAdjHeartRaspiFlSynthMultiCore_3.h 
*		version 3.8		8-Feb-2019)
*
*	@brief		A raspberry Pi multi-core polyphonic music synthesizer library api.
*
*	\mainpage AdjHeart Polyphonic Synthesizer
*
*	This library was writen to run over a Raspberry Pi 3B or higher version (currentlly 4).\n
*	The librarry implements a modular synthesizer (ModSynth) that comprises 2 main parts:\n
*	1. FluidSynth - an open source SoundFont base synthesizer.\n
*	2. AdjHeart Synthesizer (AdjSynth), as described hereafter.\n
*	It uses  OpenMP to distribute the processing acrros the Pi 4 cores.\n
*	It implements the DSP functions using floating point arithmetics.\n
*	It implements a JackAudio client.\n
*	It implements MIDI interfaces using alsaMIDI.\n
*	It implements muliple Bluetooth SPP conections server serving as MIDI input channel.\n
*	It was developed on VisualGDB addon for MS VisualStudio using MS Build.\n
*	For best performanve: IT MUST BE COMPILED IN MAXIMUM-SPEED (-o3) OPTIMIZTION MODE!!!!
*
*	Each of the polyphonic voices implements:
*		- 2 VCOs (VCO1 provides multiple harmonics mixer to enable Organ like sounds).
*		- 2 Filters (LPF, HPF, BPF)
*		- 1 Noise generator (Multi colored)
*		- 1 Karplus Strong String sound generator
*		- 1 Morphed Sinuse Oscilator
*		- 1 PAD synthersizer (IFFT wavetable based generator).
*		- Otput Volume/Pan mixer
*		- Distortion
*		- 5 ADSR enevelope generators
*		- 5 LFOs with delayed trigger capability
*
*	It provides common control and processing modules:
*		- 10 Bands graphic equilizer
*		- Reverbration (Freeverb and modified Freeverb 3)
*		- Split keyboard and sensitivity control.
*		- Portamento (unisonic)
*		- Amp-mixer with Pan control and modulation for main + 16 midi channels
*		
*	It provides the user with edditing, management and configuration capabilities
*		- Full control and edditing mode for creating AdjSynth sound patches.
*		- 3 Sketches for incremental patch development.
*		- Saving and loading patches XML files.
*		- MIDI mapping mode that ebables setting each MIDI channel features, including synthesizer type,\n
*		AdjSynth patch / MIDI SoundFont selection, level, pan and send setting.
*		- Saving /Loading mapping mode settings (described above) as XML preset files.
*		- Quick select between 5 preloaded preset files.
*		- Text based configuration files for setting automatic audio and MIDI connections.
*		
*	It provides a full API supporting all features incluuding callback functions for updates and events initiation.
*	
*/

#ifndef _LIB_ADJ_SYNTH_2_0
#define _LIB_ADJ_SYNTH_2_0

#include <stdint.h>
#include <vector>
#include <string>

//#ifdef __cplusplus
//extern "C" {
//#endif

#define _SYNTH_MAX_NUM_OF_VOICES					48
#if (_SYNTH_MAX_NUM_OF_CORE_VOICES > 64)
	Error - must be no more than 64
#endif

#define _SYNTH_MAX_NUM_OF_CORES						8
#if (_SYNTH_MAX_NUM_OF_CORES > 8)
		Error -
		must be no more than 8
#endif
		
#define _NUM_OF_PRESETS								5
	
#define	_NUM_OF_HARMONIES							9
		

#define _AUDIO_JACK									0
#define _AUDIO_ALSA									1
#define _DEFAULT_AUDIO_DRIVER						_AUDIO_JACK
	
#define _SAMPLE_RATE_44								44100
#define _SAMPLE_RATE_48								48000
#define _DEFAULT_SAMPLE_RATE						_SAMPLE_RATE_44
	
#define _AUDIO_BLOCK_SIZE_256						256
#define _AUDIO_BLOCK_SIZE_512						512
#define _AUDIO_BLOCK_SIZE_1024						1024
#define _DEFAULT_BLOCK_SIZE							_AUDIO_BLOCK_SIZE_512
		
#define _DEFAULT_ALSA_AUDIO_DEVICE					{ "plughw:1,0" }
#define _DEFAULT_ALSA_PERIOD_TIME_USEC				10000		
#define _DEFAULT_ALSA_AUD_PERIOD_TIME_USEC			((_DEFAULT_BLOCK_SIZE*1000000)/_DEFAULT_SAMPLE_RATE + 0.5)
		
#define _DEFAULT_JACK_AUD_PERIOD_TIME_USEC			((_DEFAULT_BLOCK_SIZE*1000000)/_DEFAULT_SAMPLE_RATE + 0.5)
		
#define _JACK_MODE_APP_CONTROL						0	// application sets JACK params
#define _JACK_MODE_SERVER_CONTROL					1	// JACK server sets application params
#define _DEFAULT_JACK_MODE							_JACK_MODE_SERVER_CONTROL
	
#define _JACK_AUTO_START_DIS						false
#define _JACK_AUTO_START_EN							true
#define _DEFAULT_JACK_AUTO_START					_JACK_AUTO_START_EN

#define _JACK_AUTO_CONNECT_AUDIO_DIS				false
#define _JACK_AUTO_CONNECT_AUDIO_EN					true
#define _DEFAULT_JACK_AUTO_CONNECT_AUDIO			_JACK_AUTO_CONNECT_AUDIO_EN
	
#define _JACK_AUTO_CONNECT_MIDI_DIS					false
#define _JACK_AUTO_CONNECT_MIDI_EN					true
#define _DEFAULT_JACK_AUTO_CONNECT_MIDI				_JACK_AUTO_CONNECT_MIDI_DIS
		
#define _LEFT										0
#define _RIGHT										1		
#define _SEND_LEFT									2
#define _SEND_RIGHT									3
		
#define _MAX_NUM_OF_JACK_AUDIO_OUT_PORTS			2
#define _MAX_NUM_OF_JACK_AUDIO_IN_PORTS				2
#define _MAX_NUM_OF_JACK_MIDI_IN_PORTS				4
		
#define _DEFAULT_MASTER_VOLUME						80
#define _DEFAULT_FLUID_SYNTH_VOLUME					80

#define _FLUID_CHORUS_MOD_SINE						0            
#define _FLUID_CHORUS_MOD_TRIANGLE					1

#define _FLUID_DEFAULT_SOUNDFONT_FILE				"/usr/share/sounds/sf2/FluidR3_GM.sf2"
#define _FLUID_DEFAULT_SOUNDFONT_DIR				"/usr/share/sounds/sf2"
#define _ADJ_DEFAULT_PATCHES_DIR					"/home/pi/AdjHeartModSynth_2/Settings"			
		
#define _MAX_NUM_OF_VOICES							_SYNTH_MAX_NUM_OF_VOICES

#define _PROGRAM_0									0
#define _PROGRAM_1									1
#define _PROGRAM_2									2
#define _PROGRAM_3									3
#define _PROGRAM_4									4
#define _PROGRAM_5									5
#define _PROGRAM_6									6
#define _PROGRAM_7									7
#define _PROGRAM_8									8
#define _PROGRAM_9									9
#define _PROGRAM_10									10
#define _PROGRAM_11									11
#define _PROGRAM_12									12
#define _PROGRAM_13									13
#define _PROGRAM_14									14
#define _PROGRAM_15									15
#define _PROGRAM_16									16		// Scetch 1
#define _PROGRAM_17									17
#define _PROGRAM_18									18

#define _SYNTH_MAX_NUM_OF_PROGRAMS		_PROGRAM_18 + 1
#if (_SYNTH_MAX_NUM_OF_PROGRAMS > _PROGRAM_18 + 1)
	Error -
	must be no more than PROGRAM_18 + 1
#endif
		
#define _SYNTH_NUM_OF_PROGRAMS						_SYNTH_MAX_NUM_OF_PROGRAMS

#define _MIDI_MIXER_CHAN_1							_PROGRAM_0
#define _MIDI_MIXER_CHAN_2							_PROGRAM_1
#define _MIDI_MIXER_CHAN_3							_PROGRAM_2
#define _MIDI_MIXER_CHAN_4							_PROGRAM_3
#define _MIDI_MIXER_CHAN_5							_PROGRAM_4
#define _MIDI_MIXER_CHAN_6							_PROGRAM_5
#define _MIDI_MIXER_CHAN_7							_PROGRAM_6
#define _MIDI_MIXER_CHAN_8							_PROGRAM_7
#define _MIDI_MIXER_CHAN_9							_PROGRAM_8
#define _MIDI_MIXER_CHAN_10							_PROGRAM_9
#define _MIDI_MIXER_CHAN_11							_PROGRAM_10
#define _MIDI_MIXER_CHAN_12							_PROGRAM_11
#define _MIDI_MIXER_CHAN_13							_PROGRAM_12
#define _MIDI_MIXER_CHAN_14							_PROGRAM_13
#define _MIDI_MIXER_CHAN_15							_PROGRAM_14
#define _MIDI_MIXER_CHAN_16							_PROGRAM_15
#define _SKETCH_PROGRAM_1							_PROGRAM_16
#define _SKETCH_PROGRAM_2							_PROGRAM_17
#define _SKETCH_PROGRAM_3							_PROGRAM_18

#define _MAX_NUM_OF_MIDI_DEVICES					16
#define	_MAX_NUM_OF_MIDI_DEVICE_PORTS				16
#define _MIDI_DEVICE_INPUT							0		// an input midi device (synth)
#define _MIDI_DEVICE_OUTPUT							1		// an output midi device (keyboard)

#define _MAX_NUM_OF_JACK_CLIENTS					32
#define _MAX_NUM_OF_JACK_CLIENT_PORTS				16
#define _JACK_CLIENT_INPUT							0 
#define _JACK_CLIENT_OUTPUT							1

#define _MIDI_CHAN_ASSIGNED_SYNTH_NONE				0
#define _MIDI_CHAN_ASSIGNED_SYNTH_FLUID				1
#define _MIDI_CHAN_ASSIGNED_SYNTH_ADJ				2	
	
#define _OSC_DETUNE_MAX_OCTAVE						6
#define _OSC_DETUNE_MIN_OCTAVE						-6
#define _OSC_DETUNE_MAX_SEMITONES					11
#define _OSC_DETUNE_MIN_SEMITONES					-11
#define _OSC_DETUNE_MAX_CENTS						7.5f
#define _OSC_DETUNE_MIN_CENTS						-7.5f
#define _OSC_DETUNE_CENTS_FACTORIAL					0.25f
	
#define _MOD_LFO_MIN_FREQ							0.1f
#define _MOD_LFO_MAX_FREQ							10.0f
	
	
#define _DISTORTION_MAX_DRIVE						1.f
#define _DISTORTION_MIN_DRIVE						0.f
#define _DISTORTION_MAX_RANGE						100.f
#define _DISTORTION_MIN_RANGE						1.f
#define _DISTORTION_MAX_BLEND						1.f
#define _DISTORTION_MIN_BLEND						0.f
		
#define _TONE_ON_DELAY_NONE							0
#define _TONE_ON_DELAY_500MS						400
#define _TONE_ON_DELAY_1000MS						800
#define _TONE_ON_DELAY_1500MS						1200
#define _TONE_ON_DELAY_2000MS						1600

#define _NUM_OF_UNISON_LEVEL_SLIDERS				9
		
#define _CHANGE_COMMAND_SOURCE_NONE					-1
#define _CHANGE_COMMAND_SOURCE_GUI					0
#define _CHANGE_COMMAND_SOURCE_PATCH				1		
	
#define _NUM_OF_LFOS								5
#define _NUM_OF_LFO_DELAY_OPTIONS					5
#define _NUM_OF_ENVS								5
	
#define _LFO_NONE									0
#define _LFO_1										1
#define _LFO_2										2
#define _LFO_3										3
#define _LFO_4										4
#define _LFO_5										5

#define _LFO_1_DELAYED_500MS						10
#define _LFO_2_DELAYED_500MS						11
#define _LFO_3_DELAYED_500MS						12
#define _LFO_4_DELAYED_500MS						13
#define _LFO_5_DELAYED_500MS						14

#define _LFO_1_DELAYED_1000MS						20
#define _LFO_2_DELAYED_1000MS						21
#define _LFO_3_DELAYED_1000MS						22
#define _LFO_4_DELAYED_1000MS						23
#define _LFO_5_DELAYED_1000MS						24

#define _LFO_1_DELAYED_1500MS						30
#define _LFO_2_DELAYED_1500MS						31
#define _LFO_3_DELAYED_1500MS						32
#define _LFO_4_DELAYED_1500MS						33
#define _LFO_5_DELAYED_1500MS						34

#define _LFO_1_DELAYED_2000MS						40
#define _LFO_2_DELAYED_2000MS						41
#define _LFO_3_DELAYED_2000MS						42
#define _LFO_4_DELAYED_2000MS						43
#define _LFO_5_DELAYED_2000MS						44
	
#define _ENV_NONE									0
#define _ENV_1										1
#define _ENV_2										2
#define _ENV_3										3
#define _ENV_4										4
#define _ENV_5										5

#define _NUM_OF_VCOS								2
	
#define _CONTROL_TYPE_DIAL							10
#define _CONTROL_TYPE_SLIDER						11
#define _CONTROL_TYPE_LIST							12
#define _CONTROL_TYPE_BUTTON						13
#define _CONTROL_TYPE_CHECK							14
		
#define _OSC_1_EVENT								0
#define _OSC_2_EVENT								1

#define _LFO_1_EVENT								2
#define _LFO_2_EVENT								3
#define _LFO_3_EVENT								4
#define _LFO_4_EVENT								5
#define _LFO_5_EVENT								6
#define _ENV_1_EVENT								7
#define _ENV_2_EVENT								8
#define _ENV_3_EVENT								9
#define _ENV_4_EVENT								10
#define _ENV_5_EVENT								11

#define _ENABLE										((bool)true)
#define _DISABLE									((bool)false)
		
#define _FILTER_1_EVENT								13
#define _FILTER_2_EVENT								14

#define _AMP_CH1_EVENT								15
#define _AMP_CH2_EVENT								16

#define _KBD_1_EVENT								20

#define _NOISE_1_EVENT								25

#define _KARPLUS_1_EVENT							30

#define _REVERB_EVENT								35

#define _DISTORTION_1_EVENT							40
#define _DISTORTION_2_EVENT							41

#define _BAND_EQUALIZER_EVENT						45

#define _MSO_1_EVENT								50

#define _MIDI_MAPPING_EVENT							55

#define _PAD_1_EVENT								60

#define _SEQUENCER_1_EVENT							65

#define _RECORDER_1_EVENT							70

#define	_MIDI_MIXER_1_EVENT							75
	
#define _FLUID_SYNTH_1_EVENT						80
	
#define _MOD_SYNTH_EVENT							85

#define _AUDIO_EVENT_1								90
		
#define SYNC_ON										true
#define SYNC_OFF									false
#define TRACK_ON									true
#define TRACK_OFF									false

#define _OSC_PARAM_WAVEFORM							110		
	
#define _OSC_WAVEFORM_SINE							0
#define _OSC_WAVEFORM_SQUARE						1
#define _OSC_WAVEFORM_PULSE							2
#define _OSC_WAVEFORM_TRIANGLE						3
#define _OSC_WAVEFORM_SAMPHOLD						4
		
#define _OSC_PWM_SYMMETRY							120
#define _OSC_DETUNE_OCTAVE							130
#define _OSC_DETUNE_SEMITONES						131
#define _OSC_DETUNE_CENTS							132
#define _OSC_FILTER_SEND_1							140
#define _OSC_FILTER_SEND_2							150
#define _OSC_UNISON_MODE							160	

#define _OSC_UNISON_MODE_12345678					0
#define _OSC_UNISON_MODE_HAMMOND					1
#define _OSC_UNISON_MODE_OCTAVES					2
#define _OSC_UNISON_MODE_C_CHORD					3
#define _OSC_UNISON_MODE_Cm_CHORD					4
#define _OSC_UNISON_MODE_C7_CHORD					5
#define _OSC_UNISON_MODE_Cm7_CHORD					6

#define _OSC_UNISON_LEVEL_1							170
#define _OSC_UNISON_LEVEL_2							171
#define _OSC_UNISON_LEVEL_3							172
#define _OSC_UNISON_LEVEL_4							173
#define _OSC_UNISON_LEVEL_5							174
#define _OSC_UNISON_LEVEL_6							175
#define _OSC_UNISON_LEVEL_7							176
#define _OSC_UNISON_LEVEL_8							177
#define _OSC_UNISON_LEVEL_9							181
#define _OSC_UNISON_DISTORTION						178
#define _OSC_UNISON_DETUNE							179
#define _OSC_SYNC									180
#define _OSC_FIX_TONE								190
#define _OSC_TRACK_KBD								200
#define _OSC_1_UNISON_SQUARE						210
#define _OSC_FREQ_MOD_LFO							220
#define _OSC_FREQ_MOD_LFO_LEVEL						221
#define _OSC_FREQ_MOD_ENV							222
#define _OSC_FREQ_MOD_ENV_LEVEL						223
#define _OSC_PWM_MOD_LFO							224
#define _OSC_PWM_MOD_LFO_LEVEL						225
#define _OSC_PWM_MOD_ENV							226
#define _OSC_PWM_MOD_ENV_LEVEL						227
#define _OSC_AMP_MOD_LFO							228
#define _OSC_AMP_MOD_LFO_LEVEL						229
#define _OSC_AMP_MOD_ENV							230
#define _OSC_AMP_MOD_ENV_LEVEL						231
#define _OSC_ENABLE									232
#define _OSC_HAMMOND_PERCUSION_MODE					240
		
#define _HAMMOND_PERCUSION_MODE_OFF					0
#define _HAMMOND_PERCUSION_MODE_2ND_SOFT_SLOW		1
#define _HAMMOND_PERCUSION_MODE_2ND_SOFT_FAST		2
#define _HAMMOND_PERCUSION_MODE_2ND_NORM_SLOW		3
#define _HAMMOND_PERCUSION_MODE_2ND_NORM_FAST		4
#define _HAMMOND_PERCUSION_MODE_3RD_SOFT_SLOW		5
#define _HAMMOND_PERCUSION_MODE_3RD_SOFT_FAST		6
#define _HAMMOND_PERCUSION_MODE_3RD_NORM_SLOW		7
#define _HAMMOND_PERCUSION_MODE_3RD_NORM_FAST		8
		
#define _MOD_ADSR_ATTACK							300
#define _MOD_ADSR_DECAY								301
#define _MOD_ADSR_SUSTAIN							302
#define _MOD_ADSR_RELEASE							303
#define _MOD_LFO_WAVEFORM							310
#define _MOD_LFO_RATE								311
#define _MOD_LFO_SYMMETRY							312
		
#define _FILTER_FREQ								400
#define _FILTER_OCT									401
#define _FILTER_Q									402
#define _FILTER_KBD_FREQ							403
#define _FILTER_KBD_TRACK							404
#define _FILTER_BAND								405
#define _FILTER_2_TRACK_FILT_1						406
#define _FILTER_FREQ_MOD_LFO						407
#define _FILTER_FREQ_MOD_LFO_LEVEL					408
#define _FILTER_FREQ_MOD_ENV						409
#define _FILTER_FREQ_MOD_ENV_LEVEL					410
	
#define _FILTER_BAND_LPF							0
#define _FILTER_BAND_HPF							1
#define _FILTER_BAND_BPF							2
#define _FILTER_BAND_PASS_ALL						3

#define _AMP_LEVEL									500
#define _AMP_PAN									501
#define _AMP_ENV_MOD_ENABLE							502
#define _AMP_PAN_MOD_LFO							503
#define _AMP_PAN_MOD_LFO_LEVEL						504
#define _AMP_FIXED_LEVELS							505
#define _AMP_SEND									506

#define _AMP_FIXED_LEVELS_DISABLE					0
#define _AMP_FIXED_LEVELS_ENABLE					1
		
#define _MIXER_CHAN_1_LEVEL							510
#define _MIXER_CHAN_2_LEVEL							511
#define _MIXER_CHAN_3_LEVEL							512
#define _MIXER_CHAN_4_LEVEL							513
#define _MIXER_CHAN_5_LEVEL							514
#define _MIXER_CHAN_6_LEVEL							515
#define _MIXER_CHAN_7_LEVEL							516
#define _MIXER_CHAN_8_LEVEL							517
#define _MIXER_CHAN_9_LEVEL							518
#define _MIXER_CHAN_10_LEVEL						519
#define _MIXER_CHAN_11_LEVEL						520
#define _MIXER_CHAN_12_LEVEL						521
#define _MIXER_CHAN_13_LEVEL						522
#define _MIXER_CHAN_14_LEVEL						523
#define _MIXER_CHAN_15_LEVEL						524
#define _MIXER_CHAN_16_LEVEL						525
#define _MIXER_ALL_LEVEL							526

#define _MIXER_CHAN_1_PAN							530
#define _MIXER_CHAN_2_PAN							531
#define _MIXER_CHAN_3_PAN							532
#define _MIXER_CHAN_4_PAN							533
#define _MIXER_CHAN_5_PAN							534
#define _MIXER_CHAN_6_PAN							535
#define _MIXER_CHAN_7_PAN							536
#define _MIXER_CHAN_8_PAN							537
#define _MIXER_CHAN_9_PAN							538
#define _MIXER_CHAN_10_PAN							539
#define _MIXER_CHAN_11_PAN							540
#define _MIXER_CHAN_12_PAN							541
#define _MIXER_CHAN_13_PAN							542
#define _MIXER_CHAN_14_PAN							543
#define _MIXER_CHAN_15_PAN							544
#define _MIXER_CHAN_16_PAN							545
#define _MIXER_ALL_PAN								546
	
#define _MIXER_CHAN_1_SEND							550
#define _MIXER_CHAN_2_SEND							551
#define _MIXER_CHAN_3_SEND							552
#define _MIXER_CHAN_4_SEND							553
#define _MIXER_CHAN_5_SEND							554
#define _MIXER_CHAN_6_SEND							555
#define _MIXER_CHAN_7_SEND							556
#define _MIXER_CHAN_8_SEND							557
#define _MIXER_CHAN_9_SEND							558
#define _MIXER_CHAN_10_SEND							559
#define _MIXER_CHAN_11_SEND							560
#define _MIXER_CHAN_12_SEND							561
#define _MIXER_CHAN_13_SEND							562
#define _MIXER_CHAN_14_SEND							563
#define _MIXER_CHAN_15_SEND							564
#define _MIXER_CHAN_16_SEND							565
#define _MIXER_ALL_SEND								566
		
#define _KBD_PORTAMENTO_LEVEL						600
#define _KBD_PORTAMENTO_ENABLE						601
#define _KBD_SENSITIVITY_LEVEL						602
#define _KBD_POLY_MODE_LIMIT						603
#define _KBD_POLY_MODE_FIFO							604
#define _KBD_POLY_MODE_REUSE						605
#define _KBD_LOW_SENSITIVITY_LEVEL					606
#define _KBD_SPLIT_POINT							607

#define _KBD_SPLIT_POINT_NONE						0
#define _KBD_SPLIT_POINT_C2							1
#define _KBD_SPLIT_POINT_C3							2
#define _KBD_SPLIT_POINT_C4							3
#define _KBD_SPLIT_POINT_C5							4
		
#define _NOISE_COLOR								700		
	
#define _WHITE_NOISE								0
#define _PINK_NOISE									1
#define _BROWN_NOISE								2
		
#define _NOISE_SEND_1								701
#define _NOISE_SEND_2								702
#define _NOISE_AMP_MOD_LFO							703
#define _NOISE_AMP_MOD_LFO_LEVEL					704
#define _NOISE_AMP_MOD_ENV							705
#define _NOISE_AMP_MOD_ENV_LEVEL					706
#define _NOISE_ENABLE								707
		
#define _KARPLUS_STRONG_EXCITATION_WAVEFORM			800	
	
#define _KARPLUS_STRONG_EXCITATION_WHITE_NOISE		0
#define _KARPLUS_STRONG_EXCITATION_PINK_NOISE		1
#define _KARPLUS_STRONG_EXCITATION_BROWN_NOISE		2
#define _KARPLUS_STRONG_EXCITATION_SINECHIRP		3
#define _KARPLUS_STRONG_EXCITATION_DECAYEDSINE		4
#define _KARPLUS_STRONG_EXCITATION_SAWTOOTH_WAVE	5
#define _KARPLUS_STRONG_EXCITATION_SQUARE_WAVE		6
		
#define _KARPLUS_STRONG_STRING_DUMP_CALC_MODE		801	
	
#define _KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT	0
#define _KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC	1
		
#define _KARPLUS_STRONG_STRING_DAMPING				802
#define _KARPLUS_STRONG_STRING_DAMPING_VARIATION	803
#define _KARPLUS_STRONG_PLUCK_DAMPING				804
#define _KARPLUS_STRONG_PLUCK_DAMPING_VARIATION		805
#define _KARPLUS_STRONG_ON_DECAY					806
#define _KARPLUS_STRONG_OFF_DECAY					807
#define _KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS 808
#define _KARPLUS_STRONG_ENABLE						809

#define _KARPLUS_STRONG_SEND_1						812
#define _KARPLUS_STRONG_SEND_2						813

#define _SYNTH_VOICE_1								0

#define _REVERB_ROOM_SIZE							900
#define _REVERB_ENABLE								901
#define _REVERB_DAMP								902
#define _REVERB_WET									903
#define _REVERB_DRY									904
#define _REVERB_WIDTH								905
#define _REVERB_MODE								906
#define _REVERB_PRESET								907
#define _REVERB3M_ENABLE							908

#define _REVERP_PRESET_CUSTOM						0
		
#define _PLAY_MODE									1000
#define _PLAY_MODE_POLY								0
#define _PLAY_MODE_SOLO								1
#define _PLAY_MODE_MIDI								2

#define _MODULATOR_PREV_DESTINATION					-5

#define _NO_ACTION									0
#define _DESTINATION_IN_USE							1
#define _DISCONNECTED								2
#define _CONNECTED									3
#define _CONNECTION_ERROR							-1
		
#define _ENABLE_DISTORTION							1100
#define _DISTORTION_AUTO_GAIN						1101
#define _DISTORTION_DRIVE							1102
#define _DISTORTION_RANGE							1103
#define _DISTORTION_BLEND							1104

#define _BAND_EQUALIZER_BAND_31_LEVEL				1200
#define _BAND_EQUALIZER_BAND_62_LEVEL				1201
#define _BAND_EQUALIZER_BAND_125_LEVEL				1202
#define _BAND_EQUALIZER_BAND_250_LEVEL				1203
#define _BAND_EQUALIZER_BAND_500_LEVEL				1204
#define _BAND_EQUALIZER_BAND_1K_LEVEL				1205
#define _BAND_EQUALIZER_BAND_2K_LEVEL				1206
#define _BAND_EQUALIZER_BAND_4K_LEVEL				1207
#define _BAND_EQUALIZER_BAND_8K_LEVEL				1208
#define _BAND_EQUALIZER_BAND_16K_LEVEL				1209

#define _BAND_EQUALIZER_PRESET						1220		// 0?
#define _BAND_EQUALIZER_SET_ALL_ZERO				1221		// 1?
		
#define _MSO_SEGMENT_A_POSITION						1250
#define _MSO_SEGMENT_B_POSITION						1251
#define _MSO_SEGMENT_C_POSITION						1252
#define _MSO_SEGMENT_D_POSITION						1253
#define _MSO_SEGMENT_E_POSITION						1254
#define _MSO_SEGMENT_F_POSITION						1255

#define _MSO_SYMETRY								1260

#define _MSO_CALC_BASE_LUT							1270
#define _MSO_CALC_MORPHED_LUT						1271

#define _MSO_DETUNE_OCTAVE							1280
#define _MSO_DETUNE_SEMITONES						1281
#define _MSO_DETUNE_CENTS							1282
#define _MSO_FILTER_SEND_1							1283
#define _MSO_FILTER_SEND_2							1284
#define _MSO_ENABLE									1285

#define _MSO_FREQ_MOD_LFO							1290
#define _MSO_FREQ_MOD_LFO_LEVEL						1291
#define _MSO_FREQ_MOD_ENV							1292
#define _MSO_FREQ_MOD_ENV_LEVEL						1293
#define _MSO_PWM_MOD_LFO							1294
#define _MSO_PWM_MOD_LFO_LEVEL						1295
#define _MSO_PWM_MOD_ENV							1296
#define _MSO_PWM_MOD_ENV_LEVEL						1297
#define _MSO_AMP_MOD_LFO							1298
#define _MSO_AMP_MOD_LFO_LEVEL						1299
#define _MSO_AMP_MOD_ENV							1300
#define _MSO_AMP_MOD_ENV_LEVEL						1301
		
#define _MIDI_MAPPING_MODE							1400

#define _MIDI_MAPPING_MODE_SKETCH					0
#define _MIDI_MAPPING_MODE_MAPPING					1
#define _DEFAULT_MIDI_MAPPING_MODE					_MIDI_MAPPING_MODE_SKETCH
		
#define _PAD_DETUNE_OCTAVE							1500
#define _PAD_DETUNE_SEMITONES						1501
#define _PAD_DETUNE_CENTS							1502
#define _PAD_FILTER_SEND_1							1503
#define _PAD_FILTER_SEND_2							1504
#define _PAD_ENABLE									1505

#define _PAD_FREQ_MOD_LFO							1506
#define _PAD_FREQ_MOD_LFO_LEVEL						1507
#define _PAD_FREQ_MOD_ENV							1508
#define _PAD_FREQ_MOD_ENV_LEVEL						1509
#define _PAD_AMP_MOD_LFO							1510
#define _PAD_AMP_MOD_LFO_LEVEL						1511
#define _PAD_AMP_MOD_ENV							1512
#define _PAD_AMP_MOD_ENV_LEVEL						1513
		
#define _PAD_HARMONY_LEVEL_1						1520
#define _PAD_HARMONY_LEVEL_2						1521
#define _PAD_HARMONY_LEVEL_3						1522
#define _PAD_HARMONY_LEVEL_4						1523
#define _PAD_HARMONY_LEVEL_5						1524
#define _PAD_HARMONY_LEVEL_6						1525
#define _PAD_HARMONY_LEVEL_7						1526
#define _PAD_HARMONY_LEVEL_8						1527
#define _PAD_HARMONY_LEVEL_9						1528
#define _PAD_HARMONY_LEVEL_10						1529

#define _PAD_QUALITY								1530

#define _PAD_QUALITY_32K							0
#define _PAD_QUALITY_64K							1
#define _PAD_QUALITY_128K							2
#define _PAD_QUALITY_256K							3
#define _PAD_QUALITY_512K							4
#define _PAD_QUALITY_1024K							5

#define _PAD_SHAPE									1531		
		
#define _PAD_SHAPE_RECTANGULAR						0
#define _PAD_SHAPE_GAUSSIAN							1
#define _PAD_SHAPE_DOUBLE_EXP						2
		
#define _PAD_BASE_NOTE								1532

#define _PAD_BASE_NOTE_C2							0
#define _PAD_BASE_NOTE_G2							1
#define _PAD_BASE_NOTE_C3							2
#define _PAD_BASE_NOTE_G3							3
#define _PAD_BASE_NOTE_C4							4
#define _PAD_BASE_NOTE_G4							5
#define _PAD_BASE_NOTE_C5							6
#define _PAD_BASE_NOTE_G5							7
#define _PAD_BASE_NOTE_C6							8
#define _PAD_BASE_NOTE_G6							9
		
#define _PAD_BASE_WIDTH								1533
#define _PAD_GENERATE								1534
#define _PAD_DETUNE									1535

#define _PAD_SHAPE_CUTOFF							1540		
		
#define _PAD_SHAPE_CUTOFF_FULL						0
#define _PAD_SHAPE_CUTOFF_UPPER						1
#define _PAD_SHAPE_CUTOFF_LOWER						2
		
#define _SEQUENCER_START_RECORDING					1600
#define _SEQUENCER_STOP_RECORDING					1601

#define _RECORDING_START							1700
#define _RECORDING_STOP								1701
#define _RECORDING_CLEAR							1702		
				
#define _MESSAGE_JACK_SERV_OUTPUT_NOT_RUNNING		1800
#define _MESSAGE_JACK_SERV_OUTPUT_RUNNING			1801
#define _MESSAGE_JACK_SERV_INPUT_NOT_RUNNING		1802
#define _MESSAGE_JACK_SERV_INPUT_RUNNING			1803
		
#define _MASTER_VOLUME								1900
	
#define _FLUID_SYNTH_VOLUME							2000
#define _FLUID_SYNTH_REVERB_ROOM_SIZE				2010
#define _FLUID_SYNTH_REVERB_DAMP					2011
#define _FLUID_SYNTH_REVERB_WIDTH					2012
#define _FLUID_SYNTH_REVERB_LEVEL					2013
#define _FLUID_SYNTH_REVERB_ENABLE					2014
#define _FLUID_SYNTH_CHORUS_NUMBER					2020
#define _FLUID_SYNTH_CHORUS_LEVEL					2021
#define _FLUID_SYNTH_CHORUS_SPEED					2022
#define _FLUID_SYNTH_CHORUS_DEPTH					2023
#define _FLUID_SYNTH_CHORUS_WAVEFORM				2024
#define _FLUID_SYNTH_CHORUS_ENABLE					2025
#define _FLUID_SYNTH_ENABLE							2026

#define _AUDIO_DRIVER								2110
#define _AUDIO_SAMPLE_RATE							2120
#define _AUDIO_BLOCK_SIZE							2130

#define	_AUDIO_JACK_MODE							2200
#define _AUDIO_JACK_AUTO_START						2130
#define _AUDIO_JACK_AUTO_CONNECT					2140		

	
	
typedef enum 
{
	_SF_REVERB_PRESET_DEFAULT,
	_SF_REVERB_PRESET_SMALLHALL1,
	_SF_REVERB_PRESET_SMALLHALL2,
	_SF_REVERB_PRESET_MEDIUMHALL1,
	_SF_REVERB_PRESET_MEDIUMHALL2,
	_SF_REVERB_PRESET_LARGEHALL1,
	_SF_REVERB_PRESET_LARGEHALL2,
	_SF_REVERB_PRESET_SMALLROOM1,
	_SF_REVERB_PRESET_SMALLROOM2,
	_SF_REVERB_PRESET_MEDIUMROOM1,
	_SF_REVERB_PRESET_MEDIUMROOM2,
	_SF_REVERB_PRESET_LARGEROOM1,
	_SF_REVERB_PRESET_LARGEROOM2,
	_SF_REVERB_PRESET_MEDIUMER1,
	_SF_REVERB_PRESET_MEDIUMER2,
	_SF_REVERB_PRESET_PLATEHIGH,
	_SF_REVERB_PRESET_PLATELOW,
	_SF_REVERB_PRESET_LONGREVERB1,
	_SF_REVERB_PRESET_LONGREVERB2
} sf_reverb_preset;
	
using namespace std;

/* used for holding soundfont presets */
typedef struct _soundfont_presets_data
{
	int soundFontId;
	char *presetname;
	int banknum;
	int program;
} _soundfont_presets_data_t;
	
	
	// Callback functions pointers
/* void foo(void) function pointer */
typedef void(*func_ptr_void_void_t)(void);
/* void foo(int) function pointer */
typedef void(*func_ptr_void_int_t)(int);
/* void foo(int, int) function pointer */
typedef void(*func_ptr_void_int_int_t)(int, int);
/* void foo(int, bool) function pointer */
typedef void(*func_ptr_void_int_bool_t)(int, bool);
/* void foo(int, int, int) function pointer */
typedef void(*func_ptr_void_int_int_int_t)(int, int, int);
/* void foo(float, float, int) function pointer */
typedef void(*func_ptr_void_float_ptr_float_ptr_int_t)(float*, float*, int);
/* void foo(uint32_t) function pointer */
typedef void(*func_ptr_void_uint32_t)(uint32_t);
/* void foo(string) function pointer */
typedef void(*func_ptr_void_string_t)(std::string msg);
/* bool foo(int) function pointer */
typedef bool(*func_ptr_bool_int_t)(int);

	
/**
*   @brief  Initializes the synthesizer engine.
*			Must be called first before any other call.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init();

/**
*   @brief  Starts the audio service.
*			Must be called after intializtion.
*   @param  none
*   @return 0 if done
*/
int mod_synth_start_audio();

/**
*   @brief  Stops the audio service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_stop_audio();

/**
*   @brief  Initializes the Bluetooth services.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init_bt_services();

/**
*   @brief  Deinitializes the Bluetooth services.
*			terminate al BT related threads
*   @param  none
*   @return 0 if done
*/
int mod_synth_deinit_bt_services();

/**
*   @brief  Initializes the Serial Port services.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init_serial_port_services(int portNum = 1, int baud = 115200, const char *mode = "8N1", int flowCtl = 0);  // 0=FLOW_CONTROL_DISABLED

/**
*   @brief  Deinitializes the Serial Port services.
*   @param  none
*   @return 0 if done
*/
int mod_synth_deinit_serial_port_services();

/**
*   @brief  Initializes the MIDI streaming service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_init_midi_services();

/**
*   @brief  Deinitializes the MIDI streaming service.
*   @param  none
*   @return 0 if done
*/
int mod_synth_deinit_midi_services();

/**
*   @brief `Enable bt MIDI input direct (non ALSA) streaming.
*   @param  none
*   @return void
*/
void mod_synth_connect_bt_to_midi();

/**
*   @brief `Disable bt MIDI input direct (non ALSA) streaming.
*   @param  none
*   @return void
*/
void mod_synth_disconnect_bt_to_midi();

/**
*   @brief  Closing and cleanup when application goes down.
*			Must be called whenever terminating the application.
*   @param  none
*   @return void
*/
void mod_synth_on_exit();	
	
/**
*   @brief  Returns the synthesizer maximum number of polyiphonic voices.
*   @param  none
*   @return int	maximum number of polyiphonic voices.
*/
int mod_synth_get_synthesizer_max_num_of_polyphonic_voices();
/**
*   @brief  Returns the synthesizer enabled number of polyiphonic voices.
*   @param  none
*   @return int	enabled number of polyiphonic voices.
*/
int mod_synth_get_synthesizer_num_of_polyphonic_voices();
/**
*   @brief  Returns the synthesizer maximum number of programs.
*   @param  none
*   @return int	maximum number of programs.
*/
int mod_synth_get_synthesizer_max_num_of_programs();
/**
*   @brief  Returns the synthesizer enabled number of programs.
*   @param  none
*   @return int	enabled number of programs.
*/
int mod_synth_get_synthesizer_num_of_programs();
/**
*   @brief  Returns the number of the CPU cores.
*   @param  none
*   @return int	number of the CPU cores (1, 2, 3, 4...).
*/
int mod_synth_get_number_of_cores();

/**
*   @brief  Returns the total (all cores) CPU utilization.
*   @param  none
*   @return int	the total (all cores) CPU utilization in precetages (0 to 100).
*/
int mod_synth_get_cpu_utilization();
	
/**
*   @brief  Returns the audio driver type.
*   @param  none
*   @return int	_AUDIO_ALSA or _AUDIO_JACK.
*/
int mod_synth_get_audio_driver();

/**
*   @brief  Setss the audio driver type.
*			Settings will be effective only after the next call to start_audio().
*   @param  driver	_AUDIO_ALSA or _AUDIO_JACK
*   @return int	_AUDIO_ALSA or _AUDIO_JACK.
*/
int mod_synth_set_audio_driver(int driver);

/**
*   @brief  Returns the JACK mode of operation auto (parameters are retrived
*			from JACK server), or manual (application sets JACK servers parameters).
*   @param  none
*   @return int	_JACK_MODE_MANUAL or _JACK_MODE_AUTO.
*/
int mod_synth_get_jack_mode();

/**
*   @brief  Set the JACK mode of operation auto (parameters are retrived
*			from JACK server), or manual (application sets JACK servers parameters).
*   @param  int	_JACK_MODE_MANUAL or _JACK_MODE_AUTO
*   @return void
*/
void mod_synth_set_jack_mode(int mode);

/**
*   @brief  Returns the audio sample-rate.
*   @param  none
*   @return int	_SAMPLE_RATE_44 (44100KHz) or _SAMPLE_RATE_48 (48KHz).
*/
int mod_synth_get_audio_sample_rate();

/**
*   @brief  Sets the audio sample-rate.
*			Settings will be effective only after the next call to start_audio().
*   @param  int	_SAMPLE_RATE_44 (44100KHz/default) or _SAMPLE_RATE_48 (48KHz).
*   @return sample-rate.
*/
int mod_synth_set_audio_sample_rate(int rate);

/**
*   @brief  Returns the audio block-size.
*   @param  none
*   @return int	_AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024.
*/
int mod_synth_get_audio_block_size();

/**
*   @brief  Sets the audio block-size.
*			Settings will be effective only after the next call to start_audio().
*   @param  size _AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512 (default), _AUDIO_BLOCK_SIZE_1024
*   @return set bloc-size	_AUDIO_BLOCK_SIZE_256, _AUDIO_BLOCK_SIZE_512, _AUDIO_BLOCK_SIZE_1024.
*/
int mod_synth_set_audio_block_size(int size);

/**
*   @brief  Sets the JACK auto-start state.
*   @param  state true for auto start enable.
*   @return auto start state
*/
bool mod_synth_set_jack_auto_start_state(bool state);

/**
*   @brief  Returns the JACK auto-start state.
*   @param  none.
*   @return bool _JACK_AUTO_START_DIS, _JACK_AUTO_START_EN.
*/
bool mod_synth_get_jack_auto_start_state();

/**
*   @brief  Sets the JACK auto-connect state (audio only).
*   @param  state true for auto connect enable.
*   @return auto connect state
*/
bool mod_synth_set_jack_auto_connect_audio_state(bool state);

/**
*   @brief  Returns the JACK audio auto-connect state.
*   @param  none.
*   @return bool _JACK_AUTO_CONNECT_AUDIO_DIS, _JACK_AUTO_CONNECT_AUDIO_EN.
*/
bool mod_synth_get_jack_auto_connect_audio_state();

/**
*   @brief  Returns the JACK midi auto-connect state.
*   @param  none.
*   @return bool _JACK_AUTO_CONNECT_MIDI_DIS, _JACK_AUTO_CONNECT_MIDI_EN.
*/
bool mod_synth_get_jack_auto_connect_midi_state();


/**
*	@brief	Refresh all JACK clients and connections connections data
*	@param	none
*	@return 0
*/
int mod_synth_refresh_jack_clients_data();

/**
*   @brief  Returns the number of detected input JACK clientes.
*   @param  none.
*   @return int the number of detected input JACK clients.
*/
int mod_synth_get_num_of_input_jack_clients();

/**
*   @brief  Returns the number of detected output JACK clientes.
*   @param  none.
*   @return int the number of detected output JACK clients.
*/
int mod_synth_get_num_of_output_jack_clients();

/**
*   @brief  Returns the number of JACK input client ports.
*   @param  cln client index.
*   @return int the number of an JACK input clients ports; -1 if param out of range.
*/
int mod_synth_get_num_of_jack_input_client_ports(int cln);

/**
*   @brief  Returns the number of JACK output client ports.
*   @param  cln client index.
*   @return int the number of an JACK output clients ports; -1 if param out of range.
*/
int mod_synth_get_num_of_jack_output_client_ports(int cln);
	
/**
*   @brief  Returns the name of JACK input client.
*   @param  cln client index.
*   @return int the number of an JACK input client name; "" if param out of range.
*/
std::string mod_synth_get_jack_input_client_name(int cln);

/**
*   @brief  Returns the name of JACK output client.
*   @param  cln client index.
*   @return int the number of an JACK output client name; "" if param out of range.
*/
std::string mod_synth_get_jack_output_client_name(int cln);
	
/**
*   @brief  Returns the name of JACK input client port.
*   @param  cln client index.
*   @param	prt port index
*   @return int the number of an JACK input client name; "" if param out of range.
*/
std::string mod_synth_get_jack_input_client_port_name(int cln, int prt);

/**
*   @brief  Returns the name of JACK output client port.
*   @param  cln client index.
*   @param	prt port index
*   @return int the number of an JACK output client name; "" if param out of range.
*/
std::string mod_synth_get_jack_output_client_port_name(int cln, int prt);

/**
*   @brief  Returns the number of active jack connections.
*   @param  none
*   @return int the number of active jack connectionse.
*/
int mod_synth_get_num_of_jack_clients_connections();

/**
*   @brief  Returns the connection input client name.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connection input client name 
*/
std::string mod_synth_get_jack_connection_in_client_name(int con_num);

/**
*   @brief  Returns the connection input client port name.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connection input client port name 
*/
std::string mod_synth_get_jack_connection_in_client_port_name(int con_num);

/**
*   @brief  Returns the connection output client name.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connection output client name. 
*/
std::string mod_synth_get_jack_connection_out_client_name(int con_num);

/**
*   @brief  Returns the connection output client port name.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connection output client port name. 
*/
std::string mod_synth_get_jack_connection_out_client_port_name(int con_num);

/**
*   @brief  Connects Jack input and output clients/portse.
*   @param  in_client_name			input client name
*   @param  in_client_port name		input client port name
*   @param  out_client_name			output client name
*   @param  out_client_port name	output client port name
*	@return 0 if done; -1 otherwise 
*/
int mod_synth_connect_jack_connection(
	std::string in_client_name,
	std::string in_client_port_name,
	std::string out_client_name,
	std::string out_client_port_name);

/**
*   @brief  Disonnects Jack input and output clients/portse.
*   @param  in_client_name			input client name
*   @param  in_client_port name		input client port name
*   @param  out_client_name			output client name
*   @param  out_client_port name	output client port name
*	@return 0 if done; -1 otherwise 
*/
int mod_synth_disconnect_jack_connection(
	std::string in_client_name,
	std::string in_client_port_name,
	std::string out_client_name,
	std::string out_client_port_name);

/**
*   @brief  Disonnects all Jack active connections.
*   @param  none
*	@return 0 if done; -1 otherwise 
*/
int mod_synth_disconnect_all_jack_connections();





/**
*   @brief  Sets the active sketch num
*   @param  ask	the active sketch num _SKETCH_1 to _SKETCH_3
*   @return void
*/
void mod_synth_set_active_sketch(int ask);

/**
*   @brief  Retruns the active sketch num
*   @param  none
*   @return the active sketch num _SKETCH_1 to _SKETCH_3
*/
int mod_synth_get_active_sketch();

/**
*   @brief  Copy source sketch params to a destination sketch params
*   @param  int srcsk	sorce sketch num _SKETCH_PROGRAM_1 to _SKETCH_PROGRAM_3
*   @param  int destsk	destination sketch num _SKETCH_PROGRAM__1 to _SKETCH_PROGRAM_3
*   @return 0 if done
*/
int mod_synth_copy_sketch(int srcsk, int destsk);

/**
*   @brief  Enables MIDI control keyboard control-sequences training mode
*   @param  none
*   @return 0 if done
*/
int mod_synth_enable_midi_control_sequences_training();

/**
*   @brief  Disables MIDI control keyboard control-sequences training mode
*   @param  none
*   @return 0 if done.
*/
int mod_synth_disable_midi_control_sequences_training();

/**
*   @brief  returns true if MIDI control keyboard control-sequences training is enabled
*   @param  none
*   @return true if enabled
*/
bool mod_synth_get_midi_control_sequences_training_enabled();
	
/**
*   @brief  set use of MIDI channel for MIDI control keyboard control-sequences
*   @param  use		true - channel is used; false - channel is ignored
*   @return none
*/
void mod_synth_set_midi_control_sequences_use_channel(bool use);

/**
*   @brief  get use of MIDI channel for MIDI control keyboard control-sequences state
*   @param  none		
*   @return true - channel is used; false - channel is ignored
*/
bool mod_synth_get_midi_control_sequences_use_channel_state();

/**
*   @brief  Initiates a midi control sequence map event
*   @param  none
*   @return 0 if done; -1: element already exists and replaced -2: non valid last events data
*/
int mod_synth_map_control_sequence_event();

/**
*   @brief  Saves a midi keyboard controls map into a file
*   @param  path	map file full path
*   @return 0 if OK; 
*/
int mod_synth_save_midi_keyboard_control_map_file(string path);

/**
*   @brief  Loadss a midi keyboard controls map from a file
*   @param  path	map file full path
*   @return 0 if OK; 
*/
int mod_synth_load_midi_keyboard_control_map_file(string path);

/**
*   @brief  Sets the in-focus (GUI) module and control (enable remote setting) 
*   @param  module	module id (-1 indicates no valid selection)
*   @param	control control id (-1 indicates no valid selection)
*   @param	type control type (dial, slider... ; -1 indicates no valid selection) 
*   @return 0 if OK; 
*/
int mod_synth_set_in_focus_module_control(int module, int control, int type);

/**
*   @brief  Gets the in-focus (GUI) module Id 
*   @param  none
*   @return module id (-1 if no valid control in focus); 
*/
int mod_synth_get_in_focus_module_id();

/**
*   @brief  Gets the in-focus (GUI) control Id 
*   @param  none
*   @return control id (-1 if no valid control in focus); 
*/
int mod_synth_get_in_focus_control_id();

/**
*   @brief  Gets the in-focus (GUI) control type 
*   @param  none
*   @return control type (-1 if no valid control in focus); 
*/
int mod_synth_get_in_focus_control_type();

/**
*   @brief  Scan and collect MIDI active ports info
*   @param  none
*   @return 0 if OK; -1 if failed
*/
int mod_synth_scan_midi_ports();

/**
*   @brief  Connect a MIDI device.
*   @param  int dev  the device index on the detected midi-devices list.
*	@parm	in_out	_MIDI_DEVICE_INPUT or _MIDI_DEVICE_OUTPUT
*	If dev = num of devices + 1, midi-devices scan is initiated.
*   @return connection number if connected; 
*			-1 if params out of range; 
*			-2 if no free connection is available
*/
int mod_synth_connect_midi_device(int dev, int in_out);

/**
*   @brief  Disconnect a MIDI device.
*   @param  int dev  the device index on the detected midi-devices list.
*	@parm	in_out	_MIDI_DEVICE_INPUT or _MIDI_DEVICE_OUTPUT
*	If dev = num of devices + 1, midi-devices scan is initiated.
*   @return 0 if disconnected; 
*			-1 if params out of range
*			-2 if device is not connected
*			-3 if device connection was not found
*/
int mod_synth_disconnect_midi_device(int dev, int in_out);

/**
*   @brief  Refresh ALSA midi clients info and connections status
*   @param  None.
*   @return 0 if done; 
*/
int mod_synth_refresh_alsa_midi_clients_data();

/**
*   @brief  Connect an input MIDI client to a MIDI output client.
*   @param  in_client	the input client index on the detected midi-clients list.
*   @param  in_port		the input client port index on the detected midi-clients list.
*	@param  out_client	the output client index on the detected midi-clients list.
*   @param  out_port	the output client port index on the detected midi-clients list.
*   @return 0 if done; -1 if params out of range; 
*/
int mod_synth_connect_midi_clients(int in_client, int in_port, int out_client, int out_port);

/**
*   @brief  Disconnect an input MIDI client from a MIDI output client.
*   @param  in_client	the input client index on the detected midi-clients list.
*   @param  in_port		the input client port index on the detected midi-clients list.
*	@param  out_client	the output client index on the detected midi-clients list.
*   @param  out_port	the output client port index on the detected midi-clients list.
*   @return 0 if done; -1 if params out of range; 
*/
int mod_synth_disconnect_midi_clients(int in_client, int in_port, int out_client, int out_port);

/**
*   @brief  Disconnect all connected midi clients connections.
*   @param  none.
*	@return 0
*/
int mod_synth_disconnect_all_midi_clients();

/**
*   @brief  Returns the number of detected input MIDI clients.
*   @param  none.
*   @return int the number of detected input MIDI clients.
*/
int mod_synth_get_num_of_input_midi_clients();

/**
*   @brief  Returns the number of detected output MIDI clients.
*   @param  none.
*   @return int the number of detected output MIDI clients.
*/
int mod_synth_get_num_of_output_midi_clients();

/**
*   @brief  Returns the number of an input MIDI client ports.
*   @param  cln input device index.
*   @return int the number of an input MIDI client ports; ; -1 if param out of range.
*/
int mod_synth_get_num_of_input_midi_client_ports(int cln);

/**
*   @brief  Returns the number of an output MIDI client ports.
*   @param  cln output device index.
*   @return int the number of an output MIDI client ports; ; -1 if param out of range.
*/
int mod_synth_get_num_of_output_midi_client_ports(int cln);

/**
*   @brief  Returns the selected MIDI input client name string.
*   @param  int cln input client id.
*   @param  name std::string* pointer to the selected MIDI input client name string.
*	@return void
*/
void mod_synth_get_midi_input_client_name_string(int cln, std::string *name);

/**
*   @brief  Returns the selected MIDI output client name string.
*   @param  int cln output client id.
*   @param  name std::string* pointer to the selected MIDI output client name string.
*	@return void
*/
void mod_synth_get_midi_output_client_name_string(int cln, std::string* name);

/**
*   @brief  Returns the selected MIDI input client port name string.
*   @param  int cln input device id.
*   @param	int port port number
*   @param  name std::string* pointer to the selected MIDI input client port name string.
*	@return void
*/
void mod_synth_get_midi_input_client_port_name_string(int cln, int port, std::string* name);

/**
*   @brief  Returns the selected MIDI output client port name string.
*   @param  int cln output client id.
*   @param	int port port number
*   @param  name std::string* pointer to the selected MIDI output client port name string.
*	@return void
*/
void mod_synth_get_midi_output_client_port_name_string(int cln, int port, std::string* name);

/**
*   @brief  Returns the number of midi clients (connected) connections.
*   @param  none.
*	@return the number of midi clients (connected) connections
*/
int mod_synth_get_num_of_midi_clients_connections();

/**
*   @brief  Returns the connected (src) client number.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connected-client (kbd) client number 
*/
int mod_synth_get_midi_client_connection_connected_client_num(int con_num);

/**
*   @brief  Returns the connected (src) port number.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connected-client (kbd) port number 
*/
int mod_synth_get_midi_client_connection_connected_port_num(int con_num);

/**
*   @brief  Returns the connecting to (target) client number.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connecting to (target) client number. 
*/
int mod_synth_get_midi_client_connection_connecting_to_client_num(int con_num);

/**
*   @brief  Returns the connecting to (target) port number.
*   @param  con_num		connection num (0 to num_of_connections-1).
*	@return the connecting to (target) port number. 
*/
int mod_synth_get_midi_client_connection_connecting_to_port_num(int con_num);



/**
*   @brief  Returns the selected MIDI device name.
*   @param  int dev device id.
*   @param  name char* pointer to the selected MIDI device name.
*	@return void
*/
void mod_synth_get_midi_device_name(int dev, char* name);
/**
*   @brief  Returns the selected MIDI device sub-name.
*   @param  int dev device id.
*   @param  sub_char* pointer to the selected MIDI device sub-name.
*	@return void
*/
void mod_synth_get_midi_device_sub_name(int dev, char* sub_name);


/**
*   @brief  Sets the master volume level
*   @param  vol (volum 0-100)
*   @return void
*/
void mod_synth_set_master_volume(int vol);

/**
*   @brief  Sets the Fluid synth volume level
*   @param  volume (0-100)
*   @return void
*/
void mod_synth_set_fluid_synth_volume(int vol);

/**
*   @brief  Sets the Fluid synth reverb room size
*   @param  room size (0-120)
*   @return void
*/
void mod_synth_set_fluid_synth_reverb_room_size(int val);

/**
*   @brief  Sets the Fluid synth reverb damp
*   @param  damp level (0-100)
*   @return void
*/
void mod_synth_set_fluid_synth_reverb_damp(int val);

/**
*   @brief  Sets the Fluid synth reverb width
*   @param  width level (0-100)
*   @return void
*/
void mod_synth_set_fluid_synth_reverb_width(int val);

/**
*   @brief  Sets the Fluid synth reverb level
*   @param  reverb level (0-100)
*   @return void
*/
void mod_synth_set_fluid_synth_reverb_level(int val);

/**
*   @brief  Sets the Fluid synth chorus number of stages
*   @param  number of stages (0-99)
*   @return void
*/
void mod_synth_set_fluid_synth_chorus_number(int val);

/**
*   @brief  Sets the Fluid synth chorus level
*   @param  chorus level (0-99)
*   @return void
*/
void mod_synth_set_fluid_synth_chorus_level(int val);

/**
*   @brief  Sets the Fluid synth chorus speed (Hz)
*   @param  chorus level (30-500)
*   @return void
*/
void mod_synth_set_fluid_synth_chorus_speed(int val);

/**
*   @brief  Sets the Fluid synth chorus depth
*   @param  depth (0-210)
*   @return void
*/
void mod_synth_set_fluid_synth_chorus_depth(int val);

/**
*   @brief  Sets the Fluid synth waveform
*   @param  waveform _FLUID_CHORUS_MOD_SINE (0) or _FLUID_CHORUS_MOD_TRIANGLE (1)
*   @return void
*/
void mod_synth_set_fluid_synth_chorus_waveform(int val);

/**
*   @brief  Enable the Fluid synth reverb effect
*   @param  none
*   @return void
*/
void mod_synth_set_fluid_synth_enable_reverb();

/**
*   @brief  Disable the Fluid synth reverb effect
*   @param  none
*   @return void
*/
void mod_synth_set_fluid_synth_disable_reverb();

/**
*   @brief  Enable the Fluid synth chorus effect
*   @param  none
*   @return void
*/
void mod_synth_set_fluid_synth_enable_chorus();

/**
*   @brief  Disable the Fluid synth chorus effect
*   @param  none
*   @return void
*/
void mod_synth_set_fluid_synth_disable_chorus();

/**
*   @brief  Select a SoundFont file for the fluid synth
*   @param  path full path of the sound font file
*   @return void
*/
void mod_synth_set_fluid_synth_soundfont(string path);

/**
*   @brief  Select an instrument on a MIDI channel by SoundFont ID, bank and program numbers.
*   @param  chan MIDI channel 0-15
*	@param	sfid ID of loaded sondfone
*	@param	bank bank number
*	@param	program MIDI program number
*   @return 0 on successes -1 otherwise
*/
int mod_synth_set_fluid_synth_program_select(int chan,
	unsigned int sfid, 
	unsigned int bank,
	unsigned int program);

/**
*   @brief  Set the midi channel assigned synthesizer.
*   @param  chan	midi channel number 0-15
*	@parm	synth	associated id
*   @return void
*/
void mod_synth_set_midi_channel_synth(int chan, int synth);

/**
*   @brief  Get the midi channel assigned synthesizer.
*   @param  chan	midi channel number 0-15
*   @return channel associated synthesizer id
*/
int mod_synth_get_midi_channel_synth(int chan);

/**
*   @brief Initiates an audio related event with integer value.
*   All available parameters values are defined in defs.h 
*   @param int aouid target audio : _AUDIO_EVENT_1 
*   @param int eventid specific event code :\n 
*		_AUDIO_JACK_MODE, _AUDIO_SAMPLE_RATE, _AUDIO_BLOCK_SIZE\n 
*	@param int val event parameter value(must be used with the relevant event id) :\n 
*		_AUDIO_JACK_MODE : _JACK_MODE_MANUAL, _JACK_MODE_AUTO\n
*		_AUDIO_JACK_SAMPLE_RATE : _JACK_SAMPLE_RATE_44, _JACK_SAMPLE_RATE_48\n
*		_AUDIO_JACK_BLOCK_SIZE : _JACK_BLOCK_SIZE_256, _JACK_BLOCK_SIZE_512, _JACK_BLOCK_SIZE_1024\n
*	@param  _setting_params_t *params: global parameters structure\n
*	@param int prog program num: n/a
*/
void mod_synth_audio_event_int(int audid, int eventid, int val);

/**
*   @brief Initiates an audio related event with boolean value.
*   All available parameters values are defined in defs.h 
*   @param int aouid target audio : _AUDIO_EVENT_1 
*   @param int eventid specific event code :\n 
*		_AUDIO_JACK_AUTO_START, _AUDIO_JACK_AUTO_CONNECT\n 
*	@param int val event parameter value(must be used with the relevant event id) :\n 
*		_AUDIO_JACK_AUTO_START : _JACK_AUTO_START_DIS, _JACK_AUTO_START_EN\n
*		_AUDIO_JACK_AUTO_CONNECTT : _JACK_AUTO_CONNECT_DIS, _JACK_AUTO_CONNECT_EN\n
*	@param  _setting_params_t *params: global parameters structure\n
*	@param int prog program num: n/a
*/
void mod_synth_audio_event_bool(int audid, int eventid, bool val);

/**
*   @brief  Initiates a VCO related event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int vcoid	target VCO: _OSC_1_EVENT or _OSC_2_EVENT
*	@param	int eventid	specific event code:\n
*				_OSC_PARAM_WAVEFORM, _OSC_PWM_SYMMETRY\n
*				_OSC_DETUNE_OCTAVE, _OSC_DETUNE_SEMITONES, _OSC_DETUNE_CENTS\n
*				_OSC_FILTER_SEND_1, _OSC_FILTER_SEND_2\n
*				_OSC_UNISON_MODE, _OSC_UNISON_DISTORTION, _OSC_UNISON_DETUNE  (valid only for OSC 1)\n
*				_OSC_UNISON_LEVEL_1, _OSC_UNISON_LEVEL_2, _OSC_UNISON_LEVEL_3 (valid only for OSC 1)\n
*				_OSC_UNISON_LEVEL_4, _OSC_UNISON_LEVEL_5, _OSC_UNISON_LEVEL_6 (valid only for OSC 1)\n
*				_OSC_UNISON_LEVEL_7, _OSC_UNISON_LEVEL_8, _OSC_UNISON_LEVEL_9 (valid only for OSC 1)\n
*				_OSC_HAMMOND_PERCUSION_MODE									  (valid only for OSC 1)\n
*				_OSC_FREQ_MOD_LFO, _OSC_FREQ_MOD_LFO_LEVEL\n
*				_OSC_PWM_MOD_LFO, _OSC_PWM_MOD_LFO_LEVEL\n
*				_OSC_AMP_MOD_LFO, _OSC_AMP_MOD_LFO_LEVEL\n
*				_OSC_FREQ_MOD_ENV, _OSC_FREQ_MOD_ENV_LEVEL\n
*				_OSC_PWM_MOD_ENV, _OSC_PWM_MOD_ENV_LEVEL\n
*				_OSC_AMP_MOD_ENV, _OSC_AMP_MOD_ENV_LEVEL\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_OSC_PARAM_WAVEFORM:\n
\verbatim
				_OSC_WAVEFORM_SINE, _OSC_WAVEFORM_SQUARE, _OSC_WAVEFORM_PULSE
				_OSC_WAVEFORM_TRIANGLE, _OSC_WAVEFORM_SAMPHOLD
\endverbatim
*				_OSC_PWM_SYMMETRY: 5-95\n
*				_OSC_DETUNE_OCTAVE: 0 to (getOscDetuneMaxOctave() - getOscDetuneMinOctave() + 1); 0->min octave-detune\n
*				_OSC_DETUNE_SEMITONES: 0 to (getOscDetuneMaxSemitone() - getOscDetuneMinSemitone() + 1); 0->min semitone-detune\n
*				_OSC_DETUNE_CENTS: 0 to (getOscDetuneMinCents() - getOscDetuneMaxCents() + 1); 0->min cents-detune in 0.25 steps\n
*				_OSC_FILTER_SEND_1, _OSC_FILTER_SEND_2: 0-100\n
*				_OSC_UNISON_MODE:\n
\verbatim
	_OSC_UNISON_MODE_12345678, _OSC_UNISON_MODE_HAMMOND, _OSC_UNISON_MODE_OCTAVES
	_OSC_UNISON_MODE_C_CHORD, _OSC_UNISON_MODE_Cm_CHORD
	_OSC_UNISON_MODE_C7_CHORD, _OSC_UNISON_MODE_Cm7_CHORD
\endverbatim
*				_OSC_UNISON_DISTORTION, _OSC_UNISON_DETUNE: 0-99\n
*				_OSC_UNISON_LEVEL_1 - _OSC_UNISON_LEVEL_9: 0-100\n
*				_OSC_HAMMOND_PERCUSION_MODE:\n
\verbatim
	_HAMMOND_PERCUSION_MODE_OFF, _HAMMOND_PERCUSION_MODE_2ND_SOFT_SLOW,
	_HAMMOND_PERCUSION_MODE_2ND_SOFT_FAST, _HAMMOND_PERCUSION_MODE_2ND_NORM_SLOW,
	_HAMMOND_PERCUSION_MODE_2ND_NORM_FAST, _HAMMOND_PERCUSION_MODE_3RD_SOFT_SLOW,
	_HAMMOND_PERCUSION_MODE_3RD_SOFT_FAST, _HAMMOND_PERCUSION_MODE_3RD_NORM_SLOW,
	_HAMMOND_PERCUSION_MODE_3RD_NORM_FAST
\endverbatim
*				_OSC_FREQ_MOD_LFO, _OSC_PWM_MOD_LFO, _OSC_AMP_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				_OSC_FREQ_MOD_LFO_LEVEL, _OSC_PWM_MOD_LFO_LEVEL, _OSC_AMP_MOD_LFO_LEVEL: 0-100\n
*				_OSC_FREQ_MOD_ENV, _OSC_PWM_MOD_ENV, _OSC_AMP_MOD_ENV:\n
\verbatim
	_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, _ENV_4, _ENV_5
\endverbatim
*				_OSC_FREQ_MOD_ENV_LEVEL, _OSC_PWM_MOD_ENV_LEVEL, _OSC_AMP_MOD_ENV_LEVEL: 0-100\n
*
*   @return void
*/
void mod_synth_vco_event(int vcoid, int eventid, int val);

/**
*   @brief  Initiates a VCO related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int vcoid	target VCO: _OSC_1_EVENT or _OSC_2_EVENT
*	@param	int eventid	specific event code:\n
*				_OSC_1_UNISON_SQUARE				(valid only for OSC 1)\n
*				_OSC_SYNC, _OSC_FIX_TONE			(valid only for OSC 2)
*
*	@param	bool val event parameter value  true or false (enable/disable)
*   @return void
*/
void mod_synth_vco_event_bool(int vcoid, int eventid, bool val);

/**
*   @brief  Initiates a Morphed Sinus Oscilator related event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int msoid	target mso : _MSO_1_EVENT
*	@param	int eventid	specific event code:\n
*				_MSO_SEGMENT_A_POSITION, _MSO_SEGMENT_B_POSITION, _MSO_SEGMENT_C_POSITION\n
*				_MSO_SEGMENT_D_POSITION, _MSO_SEGMENT_E_POSITION, _MSO_SEGMENT_F_POSITION
*				_MSO_SYMETRY

*				_NOISE_AMP_MOD_LFO_LEVEL, _NOISE_AMP_MOD_ENV_LEVEL
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_MSO_SEGMENT_x_POSITION : 0 - _MSO_WAVEFORM_LENGTH - 1 (511)
*				_MSO_SYMETRY : 0 - 100

*				MSO_SEND_1, _NOISE_SEND_2: 0-100\n
*				MSO_AMP_MOD_LFO:\n
\verbatim
	_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
	_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
	_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
	_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
	_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
	_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
	_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
	_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
\endverbatim
*				MSO_AMP_MOD_LFO_LEVEL: 0-100\n

*				MSO_AMP_MOD_ENV:\n
\verbatim
_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, ENV_4, ENV_5
\endverbatim
*				_MSO_AMP_MOD_ENV_LEVEL: 0-100\n
*
*	@param	bool val event parameter value  true or false (enable/disable)
*   @return void
*/
void mod_synth_mso_event(int msoid, int eventid, int val);

/**
	*   @brief  Initiates a Noise generator related event with integer value (affects all voices).
	*			All available parameters values are defined in defs.h
	*   @param  int noiseid	target noise generator: _NOISE_1_EVENT
	*	@param	int eventid	specific event code:\n
	*				_NOISE_COLOR, _NOISE_SEND_1, _NOISE_SEND_2\n
	*				_NOISE_AMP_MOD_LFO, _NOISE_AMP_MOD_ENV
	*				_NOISE_AMP_MOD_LFO_LEVEL, _NOISE_AMP_MOD_ENV_LEVEL
	*	@param	int val event parameter value (must be used with the relevant event id):\n
	*				_NOISE_COLOR\n
	\verbatim
		_WHITE_NOISE, _PINK_NOISE, _BROWN_NOISE
	\endverbatim
	*				_NOISE_SEND_1, _NOISE_SEND_2: 0-100\n
	*				NOISE_AMP_MOD_LFO:\n
	\verbatim
			_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
			_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
			_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
			_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
			_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
			_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
			_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
			_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
	\endverbatim
	*				NOISE_AMP_MOD_LFO_LEVEL: 0-100\n
	*				NOISE_AMP_MOD_ENV:\n
	\verbatim
		_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, _ENV_4, _ENV_5
	\endverbatim
	*				_NOISE_AMP_MOD_ENV_LEVEL: 0-100\n		
	*
	*	@param	bool val event parameter value  true or false (enable/disable)
	*   @return void
	*/
void mod_synth_noise_event(int noiseid, int eventid, int val);

/**
	*   @brief  Initiates a PAD related event with integer value (affects all voices).
	*			http://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htm
	*			All available parameters values are defined in defs.h
	*   @param  int padid	target PAD: _PAD_1_EVENT
	*	@param	int eventid	specific event code:\n 
	*				_PAD_DETUNE_OCTAVE, _PAD_DETUNE_SEMITONES, _PAD_DETUNE_CENTS\n 
	*				_PAD_HARMONY_LEVEL_1, _PAD_HARMONY_LEVEL_2, _PAD_HARMONY_LEVEL_3\n
	*				_PAD_HARMONY_LEVEL_4, _PAD_HARMONY_LEVEL_5, _PAD_HARMONY_LEVEL_6\n
	*				_PAD_HARMONY_LEVEL_7, _PAD_HARMONY_LEVEL_8, _PAD_DETUNE\n
	*				_PAD_FREQ_MOD_LFO, _PAD_FREQ_MOD_LFO_LEVEL\n
	*				_PAD_AMP_MOD_LFO, _PAD_AMP_MOD_LFO_LEVEL\n
	*				_PAD_QUALITY, _PAD_SHAPE\n
	*				_PAD_BASE_NOTE, _PAD_BASE_WIDTH\n
	*				_PAD_GENERATE, _PAD_ENABLE\n
	*	@param	int val event parameter value (must be used with the relevant event id):\n
	*				_PAD_DETUNE_OCTAVE: 0 to (getOscDetuneMaxOctave() - getOscDetuneMinOctave() + 1); 0->min octave-detune\n
	*				_PAD_DETUNE_SEMITONES: 0 to (getOscDetuneMaxSemitone() - getOscDetuneMinSemitone() + 1); 0->min semitone-detune\n
	*				_PAD_DETUNE_CENTS: 0 to (getOscDetuneMinCents() - getOscDetuneMaxCents() + 1); 0->min cents-detune in 0.25 steps\n
	*				_PAD_FILTER_SEND_1, _PAD_FILTER_SEND_2: 0-100\n
	*				_PAD_FREQ_MOD_LFO, _PAD_AMP_MOD_LFO:\n
	\verbatim
		_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
		_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
		_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
		_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
		_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
		_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
		_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
		_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
	\endverbatim
	*				_PAD_FREQ_MOD_LFO_LEVEL, _PAD_AMP_MOD_LFO_LEVEL: 0-100\n
	*				_PAD_FREQ_MOD_ENV, _PAD_AMP_MOD_ENV:\n
	\verbatim
	_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, ENV_4, _ENV_5
	\endverbatim
	*				_PAD_FREQ_MOD_ENV_LEVEL, _PAD_AMP_MOD_ENV_LEVEL: 0-100\n
	*				_PAD_QUALITY:\n 
	\verbatim
		_PAD_QUALITY_32K, _PAD_QUALITY_64K, _PAD_QUALITY_128K
		_PAD_QUALITY_256K, _PAD_QUALITY_512K, _PAD_QUALITY_1024K
	\endverbatim
	*				_PAD_SHAPE:\n
	\verbatim
	_PAD_SHAPE_RECTANGULAR, _PAD_SHAPE_GAUSSIAN, _PAD_SHAPE_DOUBLE_EXP
	\endverbatim
	*				_PAD_BASE_NOTE:\n
	\verbatim
	_PAD_BASE_NOTE_C2, _PAD_BASE_NOTE_G2, _PAD_BASE_NOTE_C3, _PAD_BASE_NOTE_G3
	_PAD_BASE_NOTE_C4, _PAD_BASE_NOTE_G4, _PAD_BASE_NOTE_C5, _PAD_BASE_NOTE_G5
	_PAD_BASE_NOTE_C6, _PAD_BASE_NOTE_G6
	\endverbatim
	*				_PAD_BASE_WIDTH: 0-100\n
	*				_PAD_GENERATE: (any value)
	*				_PAD_DETUNE: 0-100\n
	*				_PAD_ENABLE: false, true
	*
	*   @return void
	*/
void mod_synth_pad_event(int padid, int eventid, int val);

/**
*   @brief  Initiates a Karplus-Strong-String generator related event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int karpid	target generator: _KARPLUS_1_EVENT
*	@param	int eventid	specific event code:\n
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM\n
*				_KARPLUS_STRONG_STRING_DUMP_CALC_MODE\n
*				_KARPLUS_STRONG_STRING_DAMPING, _KARPLUS_STRONG_STRING_DAMPING_VARIATION\n
*				_KARPLUS_STRONG_ON_DECAY, _KARPLUS_STRONG_OFF_DECAY\n
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS\n
*				_KARPLUS_STRONG_SEND_1, _KARPLUS_STRONG_SEND_2
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM:\n
\verbatim
	_KARPLUS_STRONG_EXCITATION_WHITE_NOISE, _KARPLUS_STRONG_EXCITATION_PINK_NOISE
	_KARPLUS_STRONG_EXCITATION_BROWN_NOISE, _KARPLUS_STRONG_EXCITATION_SINECHIRP
	_KARPLUS_STRONG_EXCITATION_DECAYEDSINE, _KARPLUS_STRONG_EXCITATION_SAWTOOTH_WAVE
	_KARPLUS_STRONG_EXCITATION_SQUARE_WAVE\n
\endverbatim
*				_KARPLUS_STRONG_STRING_DUMP_CALC_MODE:\n
\verbatim
	_KARPLUS_STRONG_STRING_DAMPING_CALC_DIRECT, _KARPLUS_STRONG_STRING_DAMPING_CALC_MAGIC
\endverbatim
*				_KARPLUS_STRONG_STRING_DAMPING: 0-100;\n 
\verbatim
	0->getKarplusStrongMinStringDamping() 
	100 -> getKarplusStrongMaxStringDamping()
\endverbatim
*				_KARPLUS_STRONG_STRING_DAMPING_VARIATION: 0-100\n
\verbatim
	0 -> getKarplusStrongMinStringDampingVariations() 
	100 -> getKarplusStrongMaxStringDampingVariations()
\endverbatim
*				_KARPLUS_STRONG_ON_DECAY, _KARPLUS_STRONG_OFF_DECAY: 0-100:\n 
\verbatim
	0 -> getKarplusStrongMinDecay()  
	100 -> getKarplusStrongMaxDecay()
\endverbatim
*				_KARPLUS_STRONG_EXCITATION_WAVEFORM_VARIATIONS: 0-100\n
*				_KARPLUS_STRONG_SEND_1, _KARPLUS_STRONG_SEND_2: 0-100\n
*
*   @return void
*/
void mod_synth_karplus_event(int karpid, int eventid, int val);

/**
	*   @brief  Initiates a filter related event with integer value (affects all voices).
	*			All available parameters values are defined in defs.h
	*   @param  int filtid	target filter: _FILTER_1_EVENT, _FILTER_2_EVENT
	*	@param	int eventid	specific event code:\n
	*				_FILTER_FREQ, _FILTER_OCT, _FILTER_Q\n
	*				_FILTER_KBD_TRACK, _FILTER_BAND\n
	*				_FILTER_FREQ_MOD_LFO, _FILTER_FREQ_MOD_LFO_LEVEL\n
	*				_FILTER_FREQ_MOD_ENV, _FILTER_FREQ_MOD_ENV_LEVEL\n
	*	@param	int val event parameter value (must be used with the relevant event id):\n
	*				_FILTER_FREQ: 0-100; 0->getFilterMinCenterFreq(); 100-> getFilterMaxCenterFreq()\n
	*				_FILTER_OCT: 0-100; 0->0.f; 100->6.9999f\n
	*				_FILTER_Q: 0-100: 0->getFilterMinQ();  100->getFilterMaxQ()\n
	*				_FILTER_KBD_TRACK: 0-100\n				
	*				_FILTER_BAND:\n
	\verbatim
	_FILTER_BAND_LPF, _FILTER_BAND_HPF, _FILTER_BAND_BPF, _FILTER_BAND_PASS_ALL\n
	\endverbatim
	*				FILTER_FREQ_MOD_LFO:\n
	\verbatim
		_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
		_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
		_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
		_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
		_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
		_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
		_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
		_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
	\endverbatim
	*				_FILTER_FREQ_MOD_LFO_LEVEL: 0-100\n
	*				_FILTER_FREQ_MOD_ENV:\n
	\verbatim
		_ENV_NONE, _ENV_1, _ENV_2, _ENV_3, _ENV_4, _ENV_5
	\endverbatim
	*				_FILTER_FREQ_MOD_ENV_LEVEL: 0-100\n
	*
	*   @return void
	*/
void mod_synth_filter_event(int filtid, int eventid, int val);

/**
	*   @brief  Initiates an amplifier-mixer related event with integer value (affects all voices).
	*			All available parameters values are defined in defs.h
	*   @param  int ampid	target amp-mixer: _AMP_CH1_EVENT, _AMP_CH2_EVENT
	*	@param	int eventid	specific event code:\n
	*				_AMP_LEVEL, _AMP_PAN, _AMP_PAN_MOD_LFO, _AMP_PAN_MOD_LFO_LEVEL\n
	*	@param	int val event parameter value (must be used with the relevant event id):\n
	*				_AMP_LEVEL: 0-100;\n
	*				_AMP_PAN: 0-100; 0 -> Left   50 -> Mid   100- -> Right\n	
	*				_AMP_PAN_MOD_LFO:\n
	\verbatim
		_LFO_NONE, _LFO_1, _LFO_2, _LFO_3, _LFO_4, _LFO_5,
		_LFO_1_DELAYED_500MS, _LFO_2_DELAYED_500MS, _LFO_3_DELAYED_500MS, 
		_LFO_4_DELAYED_500MS, _LFO_5_DELAYED_500MS,_LFO_1_DELAYED_1000MS, 
		_LFO_2_DELAYED_1000MS, LFO_3_DELAYED_1000MS, _LFO_3_DELAYED_1000MS, 
		_LFO_4_DELAYED_1000MS, _LFO_5_DELAYED_1000MS, _LFO_1_DELAYED_1500MS, 
		_LFO_2_DELAYED_1500MS, _LFO_3_DELAYED_1500MS, _LFO_4_DELAYED_1500MS,
		_LFO_5_DELAYED_1500MS, _LFO_1_DELAYED_2000MS, _LFO_2_DELAYED_2000MS,
		_LFO_3_DELAYED_2000MS, _LFO_4_DELAYED_2000MS, _LFO_5_DELAYED_2000MS
	\endverbatim
	*				_AMP_PAN_MOD_LFO_LEVEL: 0-100\n
	*
	*   @return void
	*/
void mod_synth_amp_event(int ampid, int eventid, int val);

/**
*   @brief  Initiates a midi-mixer related event with integer value (affects all voices).
*   @param  int mixid	target amp-mixer: _MIDI_MIXER_1_EVENT
*	@param	int eventid	specific event code:\n
*				_MIXER_CHAN_n_LEVEL, _MIXER_CHAN_n_PAN, _MIXER_CHAN_n_SEND (n: 1-16)\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				LEVEL, SEND: 0-100;\n
*				PAN: 0-100; 0 -> Left   50 -> Mid   100- -> Right\n*
*   @return void
*/
void mod_synth_midi_mixer_event(int mixid, int eventid, int val);

/**
*   @brief  Initiates a reverbration effect related event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int rvbid	target reverbration: _REVERB_EVENT
*	@param	int eventid	specific event code:\n
*				_REVERB_ROOM_SIZE, _REVERB_DAMP, _REVERB_WET, _REVERB_DRY\n
*				_REVERB_WIDTH, _REVERB_MODE, _REVERB_PRESET
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_REVERB_ROOM_SIZE: 10-98\n
*				_REVERB_DAMP: 0-100\n
*				_REVERB_WET: 0-100\n
*				_REVERB_DRY: 0-100\n
*				_REVERB_WIDTH: 1-100\n
*				_REVERB_MODE: 0-50
*				_REVERB_PRESET: enum sf_reverb_preset (defined in defs.h)\n
*
*   @return void
*/
void mod_synth_reverb_event(int rvbid, int eventid, int val);
/**
*   @brief  Initiates a reverbration effect related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int rvbid	target reverbration: _REVERB_EVENT
*	@param	int eventid	specific event code:\n
*				_REVERB_ENABLE, _REVERB3M_ENABLE\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_REVERB_ENABLE, _REVERB3M_ENABLE: true - enabled; false - disabled\n
*
*   @return void
*/
void mod_synth_reverb_event_bool(int rvbid, int eventid, bool val);
/**
*   @brief  Initiates a distortion effect related event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int distid	target distortion: _DISTORTION_1_EVENT, _DISTORTION_2_EVENT
*	@param	int eventid	specific event code:\n
*				_DISTORTION_DRIVE, _DISTORTION_RANGE, _DISTORTION_BLEND\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_DISTORTION_DRIVE: 0-100\n
*				_DISTORTION_RANGE: 0-100\n
*				_DISTORTION_BLEND: 0-100\n
*
*   @return void
*/
void mod_synth_distortion_event(int distid, int eventid, int val);

/**
*   @brief  Initiates a distortion effect related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int distid	target distortion: _DISTORTION_1_EVENT, _DISTORTION_2_EVENT
*	@param	int eventid	specific event code:\n
*				_DISTORTION_ENABLE, _DISTORTION_AUTO_GAIN\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_DISTORTION_ENABLE, _DISTORTION_AUTO_GAIN: true - enabled; false - disabled\n
*
*   @return void
*/
void mod_synth_distortion_event_bool(int distid, int eventid, bool val);
/**
*   @brief  Initiates a 10 bands equalizer related event with integer value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int beqid	target equilizer: _BAND_EQUALIZER_EVENT
*	@param	int eventid	specific event code:\n
*				_BAND_EQUALIZER_BAND_31_LEVEL, _BAND_EQUAILIZER_BAND_62_LEVEL\n
*				_BAND_EQUALIZER_BAND_125_LEVEL, _BAND_EQUALIZER_BAND_250_LEVEL\n
*				_BAND_EQUALIZER_BAND_500_LEVEL, _BAND_EQUALIZER_BAND_1K_LEVEL\n
*				_BAND_EQUALIZER_BAND_2K_LEVEL, _BAND_EQUALIZER_BAND_4K_LEVEL\n
*				_BAND_EQUALIZER_BAND_8K_LEVEL, _BAND_EQUALIZER_BAND_16K_LEVEL\n
*				_BAND_EQUALIZER_PRESET, _BAND_EQUALIZER_EVENT\n
*	@param	int val event parameter value (must be used with the relevant event id):\n
*				_BAND_EQUALIZER_BAND_xxx_LEVEL: 0-40; 0 -> -20db   40 -> +20db\n
*				_BAND_EQUALIZER_PRESET: not implemented\n
*				_BAND_EQUALIZER_EVENT: 1;
*
*   @return void
*/
void mod_synth_band_equalizer_event(int beqid, int eventid, int val);
/**
*   @brief  Initiates a 10 bands equalizer related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int distid	target equilizer: _BAND_EQUALIZER_EVENT
*	@param	int eventid	specific event code: not defind
*	@param	int val event parameter value (must be used with the relevant event id): not defind
	*
*   @return void
*/
void mod_synth_band_equalizer_event_bool(int beqid, int eventid, bool val);


/**
	*   @brief  Initiates a keyboard related event with integer value (affects all voices).
	*			All available parameters values are defined in defs.h
	*   @param  int kbdid	target keyboard: _KBD_1_EVENT
	*	@param	int eventid	specific event code:\n
	*				_KBD_PORTAMENTO_LEVEL, _KBD_SENSITIVITY_LEVEL, _KBD_LOW_SENSITIVITY_LEVEL\n
	*				_KBD_SPLIT_POINT\n
	*	@param	int val event parameter value (must be used with the relevant event id):\n
	*				_KBD_PORTAMENTO_LEVEL: 0-100; 0 -> 0   100 -> getPortamentoMaxTimeSec()\n
	*				_KBD_SENSITIVITY_LEVEL: upper/all kbd split 0-100; 0 -> no sensitivity; 100 -> full velocity range\n
	*				_KBD_LOW_SENSITIVITY_LEVEL: lower kbd split 0-100; 0 -> no sensitivity; 100 -> full velocity range\n
	*				_KBD_SPLIT_POINT: 
	\verbatim
		_KBD_SPLIT_POINT_NONE, _KBD_SPLIT_POINT_C2, _KBD_SPLIT_POINT_C3
		_KBD_SPLIT_POINT_C4, _KBD_SPLIT_POINT_C5
	\endverbatim
	*
	*   @return void
	*/
void mod_synth_keyboard_event(int kbdid, int eventid, int val);
/**
*   @brief  Initiates a keyboard related event with boolean value (affects all voices).
*			All available parameters values are defined in defs.h
*   @param  int kbdid	target keyboard: _KBD_1_EVENT
*	@param	int eventid	specific event code:\n
*				_KBD_PORTAMENTO_ENABLE, _KBD_POLY_MODE_LIMIT, _KBD_POLY_MODE_FIFO, _KBD_POLY_MODE_REUSE\n
*	@param	bool val event parameter value (must be used with the relevant event id):\n
*				_KBD_PORTAMENTO_ENABLE: true - enabled; false - disabled\n
*				_KBD_POLY_MODE_LIMIT, _KBD_POLY_MODE_FIFO, _KBD_POLY_MODE_REUSE: only one of; true - enabled; false - disabled\n
*
*   @return void
*/
void mod_synth_keyboard_event_bool(int kbdid, int eventid, bool val);



/**
	*   @brief  Initiates a modulator (LFO/ADSR) related event with integer value (affects all voices).
	*			All available parameters values are defined in defs.h
	*   @param  int beqid	target modulator:
	\verbatim
		_LFO_1_EVENT, _LFO_2_EVENT, _LFO_3_EVENT, _LFO_4_EVENT, _LFO_5_EVENT
		_ENV_1_EVENT, _ENV_2_EVENT, _ENV_3_EVENT, _ENV_4_EVENT, _ENV_5_EVENT
	\endverbatim
	*	@param	int modid	specific event code:\n
	*				_MOD_ADSR_ATTACK, _MOD_ADSR_DECAY, _MOD_ADSR_SUSTAIN, _MOD_ADSR_RELEASE\n
	*				_MOD_LFO_WAVEFORM, _MOD_LFO_RATE, _MOD_LFO_SYMMETRY\n
	*	@param	int val event parameter value (must be used with the relevant event id):\n
	*				_MOD_ADSR_ATTACK: 0-100; 0 -> 0   100 -> getAdsrMaxAttackTimeSec()\n
	*				_MOD_ADSR_DECAY: 0-100; 0 -> 0   100 -> getAdsrMaxDecayTimeSec()\n
	*				_MOD_ADSR_SUSTAIN: 0-100; 0 -> 0   100 -> getAdsrMaxSustainTimeSec()\n
	*				_MOD_ADSR_RELEASE: 0-100; 0 -> 0   100 -> getAdsrMaxReleaseTimeSec()\n
	*				_MOD_LFO_WAVEFORM:\n
	\verbatim
		_OSC_WAVEFORM_SINE, _OSC_WAVEFORM_SQUARE, _OSC_WAVEFORM_PULSE
		_OSC_WAVEFORM_TRIANGLE, _OSC_WAVEFORM_SAMPHOLD
	\endverbatim
	*				_MOD_LFO_RATE: 0-100; 0 -> getLFOminFrequency(); 100 -> getLFOmaxFrequency()\n
	*				_MOD_LFO_SYMMETRY: 5-95
	*
	*   @return void
	*/
void mod_synth_modulator_event(int modid, int eventid, int val);

/**
	*   @brief  Enables OSC1.
	*   @param  none
	*   @return void
	*/
void mod_synth_enable_osc1();
/**
*   @brief  Disables OSC1.
*   @param  none
*   @return void
*/
void mod_synth_disable_osc1();
/**
*   @brief  Enables OSC2.
*   @param  none
*   @return void
*/
void mod_synth_enable_osc2();
/**
*   @brief  Disables OSC2.
*   @param  none
*   @return void
*/
void mod_synth_disable_osc2();
/**
	*   @brief  Enables Noise.
	*   @param  none
	*   @return void
	*/
void mod_synth_enable_noise();
/**
*   @brief  Disables Noise.
*   @param  none
*   @return void
*/
void mod_synth_disable_noise();
/**
*   @brief  Enables Karplus.
*   @param  none
*   @return void
*/
void mod_synth_enable_karplus();
/**
*   @brief  Disables Karplus.
*   @param  none
*   @return void
*/
void mod_synth_disable_karplus();
/**
*   @brief  Enables Drawbars.
*   @param  none
*   @return void
*/
void mod_synth_enable_drawbars();
/**
*   @brief  Disables Drawbars.
*   @param  none
*   @return void
*/
void mod_synth_disable_drawbars();
/**
*   @brief  Enables Morphed Sinus Osc.
*   @param  none
*   @return void
*/
void mod_synth_enable_morphsin();
/**
*   @brief  Disables Morphed Sinus Osc.
*   @param  none
*   @return void
*/
void mod_synth_disable_morphsin();
/**
*   @brief  Enables PDAsynthesizer.
*   @param  none
*   @return void
*/
void mod_synth_enable_pad_synth();
/**
*   @brief  Disables PDAsynthesizer.
*   @param  none
*   @return void
*/
void mod_synth_disable_pad_synth();

/**
*   @brief  Activates the main MSO WTAB update process.
*   @param  None.
*   @return void
*/
void mod_synth_update_mso_main_wtab();

/**
*   @brief  Returns a pointer to the PAD synth base harmony shape vector.
*   @param  None.
*   @return float*  a pointer to the PAD synth base harmony shape vector
*/
float *mod_synth_get_pad_base_harmony_profile();
/**
*   @brief  Returns the PAD synth base harmony shape vector number of elements.
*   @param  None.
*   @return int  the PAD synth base harmony shape vector  number of elements.
*/
int mod_synth_get_pad_base_harmony_profile_size();
/**
*   @brief  Returns a pointer to the PAD synth spectrum vector.
*   @param  None.
*   @return float*  a pointer to the PAD synth spectrum vector
*/
float *mod_synth_get_pad_spectrum();
/**
*   @brief  Returns the PAD synth spectrum vector number of elements.
*   @param  None.
*   @return int  the PAD synth spectrum vector  number of elements.
*/
int mod_synth_get_pad_spectrum_size();

// Parameters Range	

/**
*   @brief  Returns the maximum value of oscilator octave detune.
*   @param  none
*   @return int	the maximum value of oscilator octave detune.
*/
int mod_synth_get_osc_detune_max_octave();
/**
*   @brief  Returns the minimum value of oscilator octave detune.
*   @param  none
*   @return int	the minimum value of oscilator octave detune.
*/
int mod_synth_get_osc_detune_min_octave();
/**
*   @brief  Returns the maximum value of oscilator semitones detune.
*   @param  none
*   @return int	the maximum value of oscilator semitones detune.
*/
int mod_synth_get_osc_detune_max_semitones();
/**
*   @brief  Returns the minimum value of oscilator semitones detune.
*   @param  none
*   @return int	the maximum value of oscilator Semitones detune.
*/
int mod_synth_get_osc_detune_min_semitones();
/**
*   @brief  Returns the maximum value of oscilator cents detune.
*   @param  none
*   @return float	the maximum value of oscilator cents detune.
*/
float mod_synth_get_osc_detune_max_cents();
/**
*   @brief  Returns the minimum value of oscilator cents detune.
*   @param  none
*   @return float	the minimum value of oscilator cents detune.
*/
float mod_synth_get_osc_detune_min_cents();
/**
*   @brief  Returns the number of OSC1 drawbars.
*   @param  none
*   @return int	 the number of OSC1 drawbars.
*/
int mod_synth_get_osc1_num_of_drawbars();
/**
*   @brief  Returns the maximum value of ADSR attack time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR attack time in seconds.
*/
float mod_synth_get_adsr_max_attack_time_sec();
/**
*   @brief  Returns the maximum value of ADSR fecay time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR decay time in seconds.
*/
float mod_synth_get_adsr_max_decay_time_sec();
/**
*   @brief  Returns the maximum value of ADSR release time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR release time in seconds.
*/
float mod_synth_get_adsr_max_release_time_sec();
/**
*   @brief  Returns the maximum value of ADSR forced-release time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR forced-release time in seconds.
*/
float mod_synth_get_adsr_max_forced_release_time_sec();

/**
*   @brief  Returns the minimum value of filter frequency in Hz.
*   @param  none
*   @return float	the minimum value of filter frequency in Hz.
*/
float mod_synth_get_filter_min_center_freq();
/**
*   @brief  Returns the maximum value of filter frequency in Hz.
*   @param  none
*   @return float	the maximum value of filter frequency in Hz.
*/
float mod_synth_get_filter_max_center_freq();
/**
*   @brief  Returns the minimum value of filter Q.
*   @param  none
*   @return float	the minimum value of filter Q.
*/
float mod_synth_get_filter_min_Q();
/**
*   @brief  Returns the maximum value of filter Q.
*   @param  none
*   @return float	the maximum value of filter Q.
*/
float mod_synth_get_filter_max_Q();
/**
*   @brief  Returns the maximum value of the Karplus-Strong generator String Damping factor.
*   @param  none
*   @return float	the maximum value of the Karplus-Strong generator String Damping factor.
*/
float mod_synth_get_karplus_strong_max_string_damping();
/**
*   @brief  Returns the minimum value of the Karplus-Strong generator String Damping factor.
*   @param  none
*   @return float	the minimum value of the Karplus-Strong generator String Damping factor.
*/
float mod_synth_get_karplus_strong_min_string_damping();
/**
*   @brief  Returns the maximum value of the Karplus-Strong generator Pluck Damping factor.
*   @param  none
*   @return float	the maximum value of the Karplus-Strong generator Pluck Damping factor.
*/
float mod_synth_get_karplus_strong_max_pluck_damping();
/**
*   @brief  Returns the minimum value of the Karplus-Strong generator Pluck Damping factor.
*   @param  none
*   @return float	the minimum value of the Karplus-Strong generator Pluck Damping factor.
*/
float mod_synth_get_karplus_strong_min_pluck_damping();
/**
*   @brief  Returns the maximum value of the Karplus-Strong generator decay time.
*   @param  none
*   @return float	the maximum value of the Karplus-Strong generator decay time.
*/
float mod_synth_get_karplus_strong_max_decay();
/**
*   @brief  Returns the minimum value of the Karplus-Strong generator decay time.
*   @param  none
*   @return float	the minimum value of the Karplus-Strong generator decay time.
*/
float mod_synth_get_karplus_strong_min_decay();
/**
*   @brief  Returns the maximum value of the Karplus-Strong String damping variations factor.
*   @param  none
*   @return float	the maximum value of the Karplus-Strong String damping variations factor.
*/
float mod_synth_get_karplus_strong_max_pluck_damping_variations();
/**
*   @brief  Returns the minimum value of the Karplus-Strong Pluck damping variations factor.
*   @param  none
*   @return float	the minimum value of the Karplus-Strong Pluck damping variations factor.
*/
float mod_synth_get_karplus_strong_min_pluck_damping_variations();

/**
*   @brief  Returns the maximum value of the Karplus-Strong Pluck damping variations factor.
*   @param  none
*   @return float	the maximum value of the Karplus-Strong Pluck damping variations factor.
*/
float mod_synth_get_karplus_strong_max_string_damping_variations();
/**
*   @brief  Returns the minimum value of the Karplus-Strong String damping variations factor.
*   @param  none
*   @return float	the minimum value of the Karplus-Strong String damping variations factor.
*/
float mod_synth_get_karplus_strong_min_string_damping_variations();

/**
*   @brief  Returns the synthesizer maximum number of polyiphonic voices.
*   @param  none
*   @return int	maximum number of polyiphonic voices.
*/
int mod_synth_get_synthesizer_max_num_of_polyphonic_voices();

/**
*   @brief  Returns the synthesizer enabled number of programs.
*   @param  none
*   @return int	enabled number of programs.
*/
int mod_synth_get_synthesizer_max_num_of_programs();

/**
	*   @brief  Returns the minimum value of filter frequency in Hz.
	*   @param  none
	*   @return float	the minimum value of filter frequency in Hz.
	*/
float mod_synth_get_filter_min_center_freq();
/**
*   @brief  Returns the maximum value of filter frequency in Hz.
*   @param  none
*   @return float	the maximum value of filter frequency in Hz.
*/
float mod_synth_get_filter_max_center_freq();
/**
*   @brief  Returns the minimum value of filter Q.
*   @param  none
*   @return float	the minimum value of filter Q.
*/
float mod_synth_get_filter_min_Q();
/**
*   @brief  Returns the maximum value of filter Q.
*   @param  none
*   @return float	the maximum value of filter Q.
*/
float mod_synth_get_filter_max_Q();
/**
	*   @brief  Returns the maximum value of ADSR attack time in seconds.
	*   @param  none
	*   @return float	the maximum value of ADSR attack time in seconds.
	*/
float mod_synth_get_adsr_max_attack_time_sec();
/**
*   @brief  Returns the maximum value of ADSR fecay time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR decay time in seconds.
*/
float mod_synth_get_adsr_max_decay_time_sec();
/**
*   @brief  Returns the maximum value of ADSR release time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR release time in seconds.
*/
float mod_synth_get_adsr_max_release_time_sec();
/**
*   @brief  Returns the maximum value of ADSR forced-release time in seconds.
*   @param  none
*   @return float	the maximum value of ADSR forced-release time in seconds.
*/
float mod_synth_get_adsr_max_forced_release_time_sec();

/**
*   @brief  Returns the minimum value of the LFO freequency in Hz.
*   @param  none
*   @return float	the minimum value of the LFO freequency in Hz.
*/
float mod_synth_get_lfo_min_frequency();
/**
*   @brief  Returns the maximum value of the LFO freequency in Hz.
*   @param  none
*   @return float	the maximum value of the LFO freequency in Hz.
*/
float mod_synth_get_lfo_max_frequency();

/**
*   @brief  Returns the MSO lookup table length.
*   @param  none
*   @return int	the the MSO lokkup table length.
*/
int mod_synth_get_mso_table_length();
/**
*   @brief  Returns a pointer to the the MSO lookup table.
*   @param  none
*   @return float*	ta pointer to the the MSO lookup table.
*/
float *mod_synth_get_mso_morphed_lookup_table();

/**
*   @brief  Returns the maximum value of the keyboard portamento time in seconds.
*   @param  none
*   @return float	the minimum value of the keyboard portamento time in seconds.
*/
float mod_synth_get_portamento_max_time_sec();

/**
*   @brief  Returns the minimum value of the LFO freequency in Hz.
*   @param  none
*   @return float	the minimum value of the LFO freequency in Hz.
*/
float mod_synth_get_lfo_min_frequency();
/**
*   @brief  Returns the maximum value of the LFO freequency in Hz.
*   @param  none
*   @return float	the maximum value of the LFO freequency in Hz.
*/
float mod_synth_get_lfo_max_frequency();


// Active parameters values

	/**
	*   @brief  Returns the the active patch OSC1 enable state.
	*   @param  none
	*   @return bool	 the active patch OSC1 enable state.
	*/
bool mod_synth_get_active_osc1_enable_state();
/**
*   @brief  Returns the the active patch OSC2 enable state.
*   @param  none
*   @return bool	 the active patch OSC2 enable state.
*/
bool mod_synth_get_active_osc2_enable_state();
/**
*   @brief  Returns the the active patch Noise enable state.
*   @param  none
*   @return bool	 the active patch Noise enable state.
*/
bool mod_synth_get_active_noise_enable_state();
/**
*   @brief  Returns the the active patch Karplus enable state.
*   @param  none
*   @return bool	 the active patch Karplus enable state.
*/
bool mod_synth_get_active_karplus_enable_state();
/**
*   @brief  Returns the the active patch MSO enable state.
*   @param  none
*   @return bool	 the active patch MSO enable state.
*/
bool mod_synth_get_active_mso_enable_state();
/**
*   @brief  Returns the the active patch PAD enable state.
*   @param  none
*   @return bool	 the active patch PAD enable state.
*/
bool mod_synth_get_active_pad_enable_state();

/**
*   @brief  Returns the value of the active patch OSC1 waveform.
*   @param  none
*   @return int	 the value of the active patch OSC1 waveform (see defs.h).
*/
int mod_synth_get_active_osc1_waveform();
/**
*   @brief  Returns the value of the active patch OSC1 pwm %.
*   @param  none
*   @return int	 the value of the active patch OSC1 pwm %.
*/
int mod_synth_get_active_osc1_pwm_percents();
/**
*   @brief  Returns the value of the active patch OSC1 detune octave value.
*   @param  none
*   @return int	 the value of the active patch OSC1 detune octave value\n.
*	[octave detune = return value + getOscDetuneMinOctave()]
*/
int mod_synth_get_active_osc1_detune_octave();
/**
*   @brief  Returns the value of the active patch OSC1 detune semitones value.
*   @param  none
*   @return int	 the value of the active patch OSC1 detune semitones value\n.
*	[semitones detune = return value + getOscDetuneMinSenitone()]
*/
int mod_synth_get_active_osc1_detune_semitones();
/**
*   @brief  Returns the value of the active patch OSC1 detune cents value.
*   @param  none
*   @return int	 the value of the active patch OSC1 detune cents value\n.
*	[cents detune = (return value + getOscDetuneMinCents()) * 0.25]
*/
int mod_synth_get_active_osc1_detune_cents();
/**
*   @brief  Returns the value of the active patch OSC1 send level to filter 1.
*   @param  none
*   @return int	 the value of the active patch OSC1 send level to filter 1.
*/
int mod_synth_get_active_osc1_send_filter_1();
/**
*   @brief  Returns the value of the active patch OSC1 send level to filter 2.
*   @param  none
*   @return int	 the value of the active patch OSC1 send level to filter 2.
*/
int mod_synth_get_active_osc1_send_filter_2();
/**
*   @brief  Returns the value of the active patch OSC1 unison mode.
*   @param  none
*   @return int	 the value of the active patch OSC1 unison mode (see defs.h).
*/
int mod_synth_get_active_osc1_unison_mode();
/**
*   @brief  Returns the value of the active patch OSC1 hammond-organ mode.
*   @param  none
*   @return int	 the value of the active patch OSC1 hammond-organ mode (see defs.h).
*/
int mod_synth_get_active_osc1_hammond_mode();
/**
*   @brief  Returns the value of the active patch OSC1 unison harmony level.
*   @param  int harnum		0-7 (0-8 in Hamond unisom mode)
*   @return int	 the value of the active patch OSC1 hammond-organ mode (see defs.h).
*/
int mod_synth_get_active_osc1_harmonies_levels(int harnum);
/**
*   @brief  Returns the value of the active patch OSC1 harmonies detune level.
*   @param  none
*   @return int	 the value of the active patch OSC1 harmonies detune level.
*/
int mod_synth_get_active_osc1_harmonies_detune_factor();
/**
*   @brief  Returns the value of the active patch OSC1 harmonies distortion level.
*   @param  none
*   @return int	 the value of the active patch OSC1 harmonies distortion level.
*/
int mod_synth_get_active_osc1_harmonies_dist_factor();

/**
*   @brief  Returns the value of the active patch OSC1 frequency modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch OSC1 frequency modulation LFO number.
*/
int mod_synth_get_active_osc1_freq_mod_lfo();
/**
*   @brief  Returns the value of the active patch OSC1 frequency modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch OSC1 frequency modulation LFO level.
*/
int mod_synth_get_active_osc1_freq_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch OSC1 frequency modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch OSC1 frequency modulation ENV number.
*/
int mod_synth_get_active_osc1_freq_mod_env();
/**
*   @brief  Returns the value of the active patch OSC1 frequency modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch OSC1 frequency modulation ENV level.
*/
int mod_synth_get_active_osc1_freq_mod_env_level();
/**
*   @brief  Returns the value of the active patch OSC1 pwm modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch OSC1 pwm modulation LFO number.
*/
int mod_synth_get_active_osc1_pwm_mod_lfo();
/**
*   @brief  Returns the value of the active patch OSC1 pwm modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch OSC1 pwm modulation LFO level.
*/
int mod_synth_get_active_osc1_pwm_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch OSC1 pwm modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch OSC1 pwm modulation ENV number.
*/
int mod_synth_get_active_osc1_pwm_mod_env();
/**
*   @brief  Returns the value of the active patch OSC1 pwm modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch OSC1 pwm modulation ENV level.
*/
int mod_synth_get_active_osc1_pwm_mod_env_level();
/**
*   @brief  Returns the value of the active patch OSC1 amplitude modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch OSC1 amplitude modulation LFO number.
*/
int mod_synth_get_active_osc1_amp_mod_lfo();
/**
*   @brief  Returns the value of the active patch OSC1 amplitude modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch OSC1 amplitude modulation LFO level.
*/
int mod_synth_get_active_osc1_amp_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch OSC1 amplitude modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch OSC1 amplitude modulation ENV number.
*/
int mod_synth_get_active_osc1_amp_mod_env();
/**
*   @brief  Returns the value of the active patch OSC1 amplitude modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch OSC1 amplitude modulation ENV level.
*/
int mod_synth_get_active_osc1_amp_mod_env_level();

/**
*   @brief  Returns the value of the active patch OSC2 waveform.
*   @param  none
*   @return int	 the value of the active patch OSC2 waveform (see defs.h).
*/
int mod_synth_get_active_osc2_waveform();
/**
*   @brief  Returns the value of the active patch OSC2 pwm %.
*   @param  none
*   @return int	 the value of the active patch OSC2 pwm %.
*/
int mod_synth_get_active_osc2_pwm_percents();
/**
*   @brief  Returns the value of the active patch OSC2 detune octave value.
*   @param  none
*   @return int	 the value of the active patch OSC2 detune octave value\n.
*	[octave detune = return value + getOscDetuneMinOctave()]
*/
int mod_synth_get_active_osc2_detune_octave();
/**
*   @brief  Returns the value of the active patch OSC2 detune semitones value.
*   @param  none
*   @return int	 the value of the active patch OSC2 detune semitones value\n.
*	[semitones detune = return value + getOscDetuneMinSenitone()]
*/
int mod_synth_get_active_osc2_detune_semitones();
/**
*   @brief  Returns the value of the active patch OSC2 detune cents value.
*   @param  none
*   @return int	 the value of the active patch OSC2 detune cents value\n.
*	[cents detune = (return value + getOscDetuneMinCents()) * 0.25]
*/
int mod_synth_get_active_osc2_detune_cents();
/**
*   @brief  Returns the value of the active patch OSC2 send level to filter 1.
*   @param  none
*   @return int	 the value of the active patch OSC2 send level to filter 1.
*/
int mod_synth_get_active_osc2_send_filter_1();
/**
*   @brief  Returns the value of the active patch OSC2 send level to filter 2.
*   @param  none
*   @return int	 the value of the active patch OSC2 send level to filter 2.
*/
int mod_synth_get_active_osc2_send_filter_2();
/**
*   @brief  Returns the value of the active patch OSC2 sync on OSC1 state.
*   @param  none
*   @return bool	 the value of the active patch OSC2 sync on OSC1 state.
*/
bool mod_synth_get_active_osc2_sync_is_on_state();
/**
*   @brief  Returns the value of the active patch OSC2 fixed tone state (Not implemented).
*   @param  none
*   @return bool	 the value of the active patch OSC2 ixed tone state.
*/
bool mod_synth_get_active_osc2_fix_tone_is_on_state();

/**
*   @brief  Returns the value of the active patch OSC2 frequency modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch OSC2 frequency modulation LFO number.
*/
int mod_synth_get_active_osc2_freq_mod_lfo();
/**
*   @brief  Returns the value of the active patch OSC1 frequency modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch OSC1 frequency modulation LFO level.
*/
int mod_synth_get_active_osc2_freq_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch OSC2 frequency modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch OSC2 frequency modulation ENV number.
*/
int mod_synth_get_active_osc2_freq_mod_env();
/**
*   @brief  Returns the value of the active patch OSC2 frequency modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch OSC2 frequency modulation ENV level.
*/
int mod_synth_get_active_osc2_freq_mod_env_level();
/**
*   @brief  Returns the value of the active patch OSC2 pwm modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch OSC2 pwm modulation LFO number.
*/
int mod_synth_get_active_osc2_pwm_mod_lfo();
/**
*   @brief  Returns the value of the active patch OSC2 pwm modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch OSC2 pwm modulation LFO level.
*/
int mod_synth_get_active_osc2_pwm_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch OSC2 pwm modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch OSC2 pwm modulation ENV number.
*/
int mod_synth_get_active_osc2_pwm_mod_env();
/**
*   @brief  Returns the value of the active patch OSC2 pwm modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch OSC2 pwm modulation ENV level.
*/
int mod_synth_get_active_osc2_pwm_mod_env_level();
/**
*   @brief  Returns the value of the active patch OSC2 amplitude modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch OSC2 amplitude modulation LFO number.
*/
int mod_synth_get_active_osc2_amp_mod_lfo();
/**
*   @brief  Returns the value of the active patch OSC2 amplitude modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch OSC2 amplitude modulation LFO level.
*/
int mod_synth_get_active_osc2_amp_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch OSC2 amplitude modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch OSC2 amplitude modulation ENV number.
*/
int mod_synth_get_active_osc2_amp_mod_env();
/**
*   @brief  Returns the value of the active patch OSC2 amplitude modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch OSC2 amplitude modulation ENV level.
*/
int mod_synth_get_active_osc2_amp_mod_env_level();

/**
*   @brief  Returns the value of the active patch Noise color.
*   @param  none
*   @return int	 the value of the active patch  Noise color (see defs.h).
*/
int mod_synth_get_active_noise_color();
/**
*   @brief  Returns the value of the active patch Noise send filter 1 level.
*   @param  none
*   @return int	 the value of the active patch  Noise send filter 1 level.
*/
int mod_synth_get_active_noise_send_filter_1();
/**
*   @brief  Returns the value of the active patch Noise send filter 2 level.
*   @param  none
*   @return int	 the value of the active patch  Noise send filter 2 level.
*/
int mod_synth_get_active_noise_send_filter_2();

/**
*   @brief  Returns the value of the active patch Noise amp modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch  Noise amp modulation LFO number.
*/
int mod_synth_get_active_noise_amp_mod_lfo();
/**
*   @brief  Returns the value of the active patch Noise amp modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch  Noise amp modulation LFO level.
*/
int mod_synth_get_active_noise_amp_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch Noise amp modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch  Noise amp modulation ENV number.
*/
int mod_synth_get_active_noise_amp_mod_env();
/**
*   @brief  Returns the value of the active patch Noise amp modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch  Noise amp modulation ENV level.
*/
int mod_synth_get_active_noise_amp_mod_env_level();

/**
	*   @brief  Returns the value of the active patch ADSR 1 attack time.
	*   @param  none
	*   @return int	 the value of the active patch ADSR 1 attack time.
	*	[time = getAdsrMaxAttackTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
	*/
int mod_synth_get_active_env_mod_1_attack();
/**
*   @brief  Returns the value of the active patch ADSR 1 decay time.
*   @param  none
*   @return int	 the value of the active patch ADSR 1 decay time.
*	[time = getAdsrMaxDecayTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_1_decay();
/**
*   @brief  Returns the value of the active patch ADSR 1 sustain level.
*   @param  none
*   @return int	 the value of the active patch ADSR 1 sustain level.
*/
int mod_synth_get_active_env_mod_1_sustain();
/**
*   @brief  Returns the value of the active patch ADSR 1 release time.
*   @param  none
*   @return int	 the value of the active patch ADSR 1 release time.
*	[time = getAdsrMaxReleaseTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_1_release();
	
/**
*   @brief  Returns the value of the active patch ADSR 2 attack time.
*   @param  none
*   @return int	 the value of the active patch ADSR 2 attack time.
*	[time = getAdsrMaxAttackTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_2_attack();
/**
*   @brief  Returns the value of the active patch ADSR 2 decay time.
*   @param  none
*   @return int	 the value of the active patch ADSR 2 decay time.
*	[time = getAdsrMaxDecayTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_2_decay();
/**
*   @brief  Returns the value of the active patch ADSR 2 sustain level.
*   @param  none
*   @return int	 the value of the active patch ADSR 2 sustain level.
*/
int mod_synth_get_active_env_mod_2_sustain();
/**
*   @brief  Returns the value of the active patch ADSR 2 release time.
*   @param  none
*   @return int	 the value of the active patch ADSR 2 release time.
*	[time = getAdsrMaxReleaseTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_2_release();
	
/**
*   @brief  Returns the value of the active patch ADSR 3 attack time.
*   @param  none
*   @return int	 the value of the active patch ADSR 3 attack time.
*	[time = getAdsrMaxAttackTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_3_attack();
/**
*   @brief  Returns the value of the active patch ADSR 3 decay time.
*   @param  none
*   @return int	 the value of the active patch ADSR 3 decay time.
*	[time = getAdsrMaxDecayTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_3_decay();
/**
*   @brief  Returns the value of the active patch ADSR 3 sustain level.
*   @param  none
*   @return int	 the value of the active patch ADSR 3 sustain level.
*/
int mod_synth_get_active_env_mod_3_sustain();
/**
*   @brief  Returns the value of the active patch ADSR 3 release time.
*   @param  none
*   @return int	 the value of the active patch ADSR 3 release time.
*	[time = getAdsrMaxReleaseTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_3_release();

/**
*   @brief  Returns the value of the active patch ADSR 4 attack time.
*   @param  none
*   @return int	 the value of the active patch ADSR 4 attack time.
*	[time = getAdsrMaxAttackTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_4_attack();
/**
*   @brief  Returns the value of the active patch ADSR 4 decay time.
*   @param  none
*   @return int	 the value of the active patch ADSR 4 decay time.
*	[time = getAdsrMaxDecayTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_4_decay();
/**
*   @brief  Returns the value of the active patch ADSR 4 sustain level.
*   @param  none
*   @return int	 the value of the active patch ADSR 4 sustain level.
*/
int mod_synth_get_active_env_mod_4_sustain();
/**
*   @brief  Returns the value of the active patch ADSR 4 release time.
*   @param  none
*   @return int	 the value of the active patch ADSR 4 release time.
*	[time = getAdsrMaxReleaseTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_4_release();

/**
*   @brief  Returns the value of the active patch ADSR 5 attack time.
*   @param  none
*   @return int	 the value of the active patch ADSR 5 attack time.
*	[time = getAdsrMaxAttackTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_5_attack();
/**
*   @brief  Returns the value of the active patch ADSR 5 decay time.
*   @param  none
*   @return int	 the value of the active patch ADSR 5 decay time.
*	[time = getAdsrMaxDecayTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_5_decay();
/**
*   @brief  Returns the value of the active patch ADSR 5 sustain level.
*   @param  none
*   @return int	 the value of the active patch ADSR 5 sustain level.
*/
int mod_synth_get_active_env_mod_5_sustain();
/**
*   @brief  Returns the value of the active patch ADSR 5 release time.
*   @param  none
*   @return int	 the value of the active patch ADSR 5 release time.
*	[time = getAdsrMaxReleaseTimeSec()*pow(10.0, ret value / 50.0) / 100.0)]
*/
int mod_synth_get_active_env_mod_5_release();

/**
*   @brief  Returns the value of the active patch LFO 1 waveform.
*   @param  none
*   @return int	 the value of the active patch LFO 1 waveform (see defs.h).
*/
int mod_synth_get_active_lfo_mod_1_waveform();
/**
*   @brief  Returns the value of the active patch LFO 1 symetry in %.
*   @param  none
*   @return int	 the value of the active patch LFO 1 symetry in %.
*/
int mod_synth_get_active_lfo_mod_1_symmetry();
/**
*   @brief  Returns the value of the active patch LFO 1 rate Hz.
*   @param  none
*   @return int	 the value of the active patch LFO 1 rate Hz.
*	[rate Hz = getLFOminFrequency() + (getLFOmaxFrequency() - getLFOminFrequency())*pow(10, (double)ret value / 50.0) / 100.0
*/
int mod_synth_get_active_lfo_mod_1_rate();
	
/**
*   @brief  Returns the value of the active patch LFO 2 waveform.
*   @param  none
*   @return int	 the value of the active patch LFO 2 waveform (see defs.h).
*/
int mod_synth_get_active_lfo_mod_2_waveform();
/**
*   @brief  Returns the value of the active patch LFO 2 symetry in %.
*   @param  none
*   @return int	 the value of the active patch LFO 2 symetry in %.
*/
int mod_synth_get_active_lfo_mod_2_symmetry();
/**
*   @brief  Returns the value of the active patch LFO 2 rate Hz.
*   @param  none
*   @return int	 the value of the active patch LFO 2 rate Hz.
*	[rate Hz = getLFOminFrequency() + (getLFOmaxFrequency() - getLFOminFrequency())*pow(10, (double)ret value / 50.0) / 100.0
*/
int mod_synth_get_active_lfo_mod_2_rate();
	
/**
*   @brief  Returns the value of the active patch LFO 3 waveform.
*   @param  none
*   @return int	 the value of the active patch LFO 1 waveform (see defs.h).
*/
int mod_synth_get_active_lfo_mod_3_waveform();
/**
*   @brief  Returns the value of the active patch LFO 3 symetry in %.
*   @param  none
*   @return int	 the value of the active patch LFO 3 symetry in %.
*/
int mod_synth_get_active_lfo_mod_3_symmetry();
/**
*   @brief  Returns the value of the active patch LFO 3 rate Hz.
*   @param  none
*   @return int	 the value of the active patch LFO 3 rate Hz.
*	[rate Hz = getLFOminFrequency() + (getLFOmaxFrequency() - getLFOminFrequency())*pow(10, (double)ret value / 50.0) / 100.0
*/
int mod_synth_get_active_lfo_mod_3_rate();

/**
*   @brief  Returns the value of the active patch LFO 4 waveform.
*   @param  none
*   @return int	 the value of the active patch LFO 4 waveform (see defs.h).
*/
int mod_synth_get_active_lfo_mod_4_waveform();
/**
*   @brief  Returns the value of the active patch LFO 4 symetry in %.
*   @param  none
*   @return int	 the value of the active patch LFO 4 symetry in %.
*/
int mod_synth_get_active_lfo_mod_4_symmetry();
/**
*   @brief  Returns the value of the active patch LFO 4 rate Hz.
*   @param  none
*   @return int	 the value of the active patch LFO 4 rate Hz.
*	[rate Hz = getLFOminFrequency() + (getLFOmaxFrequency() - getLFOminFrequency())*pow(10, (double)ret value / 50.0) / 100.0
*/
int mod_synth_get_active_lfo_mod_4_rate();

/**
*   @brief  Returns the value of the active patch LFO 5 waveform.
*   @param  none
*   @return int	 the value of the active patch LFO 5 waveform (see defs.h).
*/
int mod_synth_get_active_lfo_mod_5_waveform();
/**
*   @brief  Returns the value of the active patch LFO 5 symetry in %.
*   @param  none
*   @return int	 the value of the active patch LFO 5 symetry in %.
*/
int mod_synth_get_active_lfo_mod_5_symmetry();
/**
*   @brief  Returns the value of the active patch LFO 5 rate Hz.
*   @param  none
*   @return int	 the value of the active patch LFO 5 rate Hz.
*	[rate Hz = getLFOminFrequency() + (getLFOmaxFrequency() - getLFOminFrequency())*pow(10, (double)ret value / 50.0) / 100.0
*/
int mod_synth_get_active_lfo_mod_5_rate();

/**
*   @brief  Returns an active patch MSO lookup table segment point.
*   @param  int  requested point (0-5)
*   @return int	the an active patch MSO lookup table segment point.
*/
int mod_synth_get_active_mso_table_segment_point(int point);
/**
*   @brief  Returns an active patch MSO symetry value.
*   @param  none
*   @return int	the an active patch MSO symetry value.
*/
int mod_synth_get_active_mso_symetry();
/**
*   @brief  Returns the value of the active patch MSO pwm %.
*   @param  none
*   @return int	 the value of the active patch MSO pwm %.
*/
int mod_synth_get_active_mso_pwm_percents();
/**
*   @brief  Returns the value of the active patch MSO detune octave value.
*   @param  none
*   @return int	 the value of the active patch MSO detune octave value\n.
*	[octave detune = return value + getOscDetuneMinOctave()]
*/
int mod_synth_get_active_mso_detune_octave();
/**
*   @brief  Returns the value of the active patch MSO detune semitones value.
*   @param  none
*   @return int	 the value of the active patch MSO detune semitones value\n.
*	[semitones detune = return value + getOscDetuneMinSenitone()]
*/
int mod_synth_get_active_mso_detune_semitones();
/**
*   @brief  Returns the value of the active patch MSO detune cents value.
*   @param  none
*   @return int	 the value of the active patch MSO detune cents value\n.
*	[cents detune = (return value + getOscDetuneMinCents()) * 0.25]
*/
int mod_synth_get_active_mso_detune_cents();
/**
*   @brief  Returns the value of the active patch MSO send level to filter 1.
*   @param  none
*   @return int	 the value of the active patch MSO send level to filter 1.
*/
int mod_synth_get_active_mso_send_filter_1();
/**
*   @brief  Returns the value of the active patch MSO send level to filter 2.
*   @param  none
*   @return int	 the value of the active patch MSO send level to filter 2.
*/
int mod_synth_get_active_mso_send_filter_2();
/**
*   @brief  Returns the value of the active patch MSO sync on OSC1 state.
*   @param  none
*   @return bool	 the value of the active patch MSO sync on OSC1 state.
*/
//bool getActiveMso1SyncIsOnState();
/**
*   @brief  Returns the value of the active patch MSO frequency modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch MSO frequency modulation LFO number.
*/
int mod_synth_get_active_mso_freq_mod_lfo();
/**
*   @brief  Returns the value of the active patch MSO frequency modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch MSO frequency modulation LFO level.
*/
int mod_synth_get_active_mso_freq_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch MSO frequency modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch MSO frequency modulation ENV number.
*/
int mod_synth_get_active_mso_freq_mod_env();
/**
*   @brief  Returns the value of the active patch MSO frequency modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch MSO frequency modulation ENV level.
*/
int mod_synth_get_active_mso_freq_mod_env_level();
/**
*   @brief  Returns the value of the active patch MSO pwm modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch MSO pwm modulation LFO number.
*/
int mod_synth_get_active_mso_pwm_mod_lfo();
/**
*   @brief  Returns the value of the active patch MSO pwm modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch MSO pwm modulation LFO level.
*/
int mod_synth_get_active_mso_pwm_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch MSO pwm modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch MSO pwm modulation ENV number.
*/
int mod_synth_get_active_mso_pwm_mod_env();
/**
*   @brief  Returns the value of the active patch MSO pwm modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch MSO pwm modulation ENV level.
*/
int mod_synth_get_active_mso_pwm_mod_env_level();
/**
*   @brief  Returns the value of the active patch MSO amplitude modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch MSO amplitude modulation LFO number.
*/
int mod_synth_get_active_mso_amp_mod_lfo();
/**
*   @brief  Returns the value of the active patch MSO amplitude modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch MSO amplitude modulation LFO level.
*/
int mod_synth_get_active_mso_amp_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch MSO amplitude modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch MSO amplitude modulation ENV number.
*/
int mod_synth_get_active_mso_amp_mod_env();
/**
*   @brief  Returns the value of the active patch MSO amplitude modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch MSO amplitude modulation ENV level.
*/
int mod_synth_get_active_mso_amp_mod_env_level();

/**
	*   @brief  Returns the value of the active patch Karplus-strong generator excitation waveform.
	*   @param  none
	*   @return int	 the value of the active patch Karplus-strong generator excitation waveform (see defs.h).
	*/
int mod_synth_get_active_karplus_excitation_waveform_type();

/**
	*   @brief  Returns the value of the active patch Karplus-strong generator excitation waveform variations level.
	*   @param  none
	*   @return int	 the value of the active patch Karplus-strong generator excitation waveform variations level(see defs.h).
	*/
int mod_synth_get_active_karplus_excitation_waveform_variations();

/**
*   @brief  Returns the value of the active patch Karplus-strong generator string damping calculation mode.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator string damping calculation mode (see defs.h).
*/
int mod_synth_get_active_karplus_string_damping_calculation_mode();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator string damping.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator string damping.\n
*	[damping = getKarplusStrongMinStringDamping()+ret value/100.0f*(getKarplusStrongMinStringDamping()-getKarplusStrongMaxStringDamping())]
*/
int mod_synth_get_active_karplus_string_damping();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator pluck damping.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator pluck damping.\n
*	[damping = getKarplusStrongMinPluckDamping()+ret value/100.0f*(getKarplusStrongMinPluckDamping()-getKarplusStrongMaxPluckDamping())]
*/
int mod_synth_get_active_karplus_pluck_damping();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator note-on decay factor.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator note-on decay factor.\n
*	[decay = getKarplusStrongMinStringDecay()+ret value/100.0f*(getKarplusStrongMinStringDecay()-getKarplusStrongMaxStringDecay())]
*/
int mod_synth_get_active_karplus_string_on_decay();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator note-off decay factor.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator note-off decay factor.\n
*	[decay = getKarplusStrongMinStringDecay()+ret value/100.0f*(getKarplusStrongMinStringDecay()-getKarplusStrongMaxStringDecay())]
*/
int mod_synth_get_active_karplus_string_off_decay();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator note-on string damping variations factor.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator note-on string damping variations factor.\n
*	[string damping variation factor = \n
*	getKarplusStrongMinStringDampingVariations()+ret value/100.0f*(getKarplusStrongMaxStringDampingVariations()-getKarplusStrongMinStringDampingVariations())]
*/
int mod_synth_get_active_karplus_string_damping_variation();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator note-on pluck damping variations factor.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator note-on pluck damping variations factor.\n
*	[pluckdamping variation factor = \n
*	getKarplusStrongMinPluckDampingVariations()+ret value/100.0f*(getKarplusStrongMaxPluckDampingVariations()-getKarplusStrongMinPluckDampingVariations())]
*/
int mod_synth_get_active_karplus_pluck_damping_variation();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator send filter 1 level.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator send filter 1 level.
*/
int mod_synth_get_active_karplus_send_filter_1();
/**
*   @brief  Returns the value of the active patch Karplus-strong generator send filter 2 level.
*   @param  none
*   @return int	 the value of the active patch Karplus-strong generator send filter 2 level.
*/
int mod_synth_get_active_karplus_send_filter_2();

/**
	*   @brief  Returns the value of the active patch PAD detune octave value.
	*   @param  none
	*   @return int	 the value of the active patch PAD detune octave value\n.
	*	[octave detune = return value + getOscDetuneMinOctave()]
	*/
int mod_synth_get_active_pad_detune_octave();
/**
*   @brief  Returns the value of the active patch PAD detune semitones value.
*   @param  none
*   @return int	 the value of the active patch PAD detune semitones value\n.
*	[semitones detune = return value + getOscDetuneMinSenitone()]
*/
int mod_synth_get_active_pad_detune_semitones();
/**
*   @brief  Returns the value of the active patch PAD detune cents value.
*   @param  none
*   @return int	 the value of the active patch PAD detune cents value\n.
*	[cents detune = (return value + getOscDetuneMinCents()) * 0.25]
*/
int mod_synth_get_active_pad_detune_cents();
/**
*   @brief  Returns the value of the active patch PAD send level to filter 1.
*   @param  none
*   @return int	 the value of the active patch PAD send level to filter 1.
*/
int mod_synth_get_active_pad_send_filter_1();
/**
*   @brief  Returns the value of the active patch PAD send level to filter 2.
*   @param  none
*   @return int	 the value of the active patch PAD send level to filter 2.
*/
int mod_synth_get_active_pad_send_filter_2();
/**
*   @brief  Returns the value of the active patch PAD frequency modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch PAD frequency modulation LFO number.
*/
int mod_synth_get_active_pad_freq_mod_lfo();
/**
*   @brief  Returns the value of the active patch PAD frequency modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch PAD frequency modulation LFO level.
*/
int mod_synth_get_active_pad_freq_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch PAD frequency modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch PAD frequency modulation ENV number.
*/
int mod_synth_get_active_pad_freq_mod_env();
/**
*   @brief  Returns the value of the active patch PAD frequency modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch PAD frequency modulation ENV level.
*/
int mod_synth_get_active_pad_freq_mod_env_level();
/**
*   @brief  Returns the value of the active patch PAD amplitude modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch PAD amplitude modulation LFO number.
*/
int mod_synth_get_active_pad_amp_mod_lfo();
/**
*   @brief  Returns the value of the active patch PAD amplitude modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch PAD amplitude modulation LFO level.
*/
int mod_synth_get_active_pad_amp_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch PAD amplitude modulation ENV number.
*   @param  none
*   @return int	 the value of the active patch PAD amplitude modulation ENV number.
*/
int mod_synth_get_active_pad_amp_mod_env();
/**
*   @brief  Returns the value of the active patch PAD amplitude modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch PAD amplitude modulation ENV level.
*/
int mod_synth_get_active_pad_amp_mod_env_level();
/**
*   @brief  Returns the value of the active patch PAD quality factor.
*   @param  none
*   @return int	 the value of the active patch PAD quality factor.
*/
int mod_synth_get_active_pad_quality();
/**
*   @brief  Returns the value of the active patch PAD shape.
*   @param  none
*   @return int	 the value of the active patch PAD shape.
*/
int mod_synth_get_active_pad_shape();
/**
*   @brief  Returns the value of the active patch PAD shape-cutoff mode.
*   @param  none
*   @return int	 the value of the active patch PAD shape-cutoff mode.
*/
int mod_synth_get_active_pad_shape_cutoff();
/**
*   @brief  Returns the value of the active patch PAD base note.
*   @param  none
*   @return int	 the value of the active patch PAD base note.
*/
int mod_synth_get_active_pad_base_note();
/**
*   @brief  Returns the value of the active patch PAD base width.
*   @param  none
*   @return int	 the value of the active patch PAD abase width.
*/
int mod_synth_get_active_pad_base_width();
/**
*   @brief  Returns the value of the active patch PAD harmony level.
*   @param  int harmony: 0-7
*   @return int	 the value of the active patch PAD harmony level.
*/
int mod_synth_get_active_pad_harmony_level(int harmony);
/**
*   @brief  Returns the value of the active PAD harmonic-detune level.
*   @param  none
*   @return int	 the value of the active patch PAD harmonic-detune level.
*/
int mod_synth_get_active_pad_harmony_detune();

/**
*   @brief  Returns the value of the active patch filter 1 frequency.
*   @param  none
*   @return int	 the value of the active patch filter 1 frequency.
*	[freq Hz = getFilterMinCenterFreq() + (getFilterMaxCenterFreq() - getFilterMinCenterFreq()) * ret value / 100.0f]
*/
int mod_synth_get_active_filter_1_freq();
/**
*   @brief  Returns the value of the active patch filter 1 Octave.
*   @param  none
*   @return int	 the value of the active patch filter 1 octave.
*	[octave = (float)ret value * 6.9999f / 100.0f]
*/
int mod_synth_get_active_filter_1_oct();
/**
*   @brief  Returns the value of the active patch filter 1 Q.
*   @param  none
*   @return int	 the value of the active patch filter 1 Q.
*	[Q = getFilterMinQ() + (getFilterMaxCenterQ() - getFilterMinCenterQ()) * ret value / 100.0f]
*/
int mod_synth_get_active_filter_1_q();
/**
*   @brief  Returns the value of the active patch filter 1 keyboard tracking.
*   @param  none
*   @return int	 the value of the active patch filter 1 keyboard tracking.
*/
int mod_synth_get_active_filter_1_kbd_track();
/**
*   @brief  Returns the value of the active patch filter 1  band.
*   @param  none
*   @return int	 the value of the active patch filter 1 band (see defs.h).
*/
int mod_synth_get_active_filter_1_band();

/**
*   @brief  Returns the value of the active patch filter 1 frequency modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch filter 1 frequency modulation LFO number.
*/
int mod_synth_get_active_filter_1_Freq_mod_lfo();
/**
*   @brief  Returns the value of the active patch filter 1 frequency modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch filter 1 frequency modulation LFO level.
*/
int mod_synth_get_active_filter_1_Freq_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch filter 1 frequency modulation ENV.
*   @param  none
*   @return int	 the value of the active patch filter 1 frequency modulation ENV.
*/
int mod_synth_get_active_filter_1_Freq_mod_env();
/**
*   @brief  Returns the value of the active patch filter 1 frequency modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch filter 1 frequency modulation ENV level.
*/
int mod_synth_get_active_filter_1_Freq_mod_env_level();

/**
*   @brief  Returns the value of the active patch filter 2 frequency.
*   @param  none
*   @return int	 the value of the active patch filter 2 frequency.
*	[freq Hz = getFilterMinCenterFreq() + (getFilterMaxCenterFreq() - getFilterMinCenterFreq()) * ret value / 100.0f]
*/
int mod_synth_get_active_filter_2_freq();
/**
*   @brief  Returns the value of the active patch filter 2 Octave.
*   @param  none
*   @return int	 the value of the active patch filter 2 octave.
*	[octave = (float)ret value * 6.9999f / 100.0f]
*/
int mod_synth_get_active_filter_2_oct();
/**
*   @brief  Returns the value of the active patch filter 2 Q.
*   @param  none
*   @return int	 the value of the active patch filter 2 Q.
*	[Q = getFilterMinQ() + (getFilterMaxCenterQ() - getFilterMinCenterQ()) * ret value / 100.0f]
*/
int mod_synth_get_active_filter_2_q();
/**
*   @brief  Returns the value of the active patch filter 2 keyboard tracking.
*   @param  none
*   @return int	 the value of the active patch filter 2 keyboard tracking.
*/
int mod_synth_get_active_filter_2_kbd_track();
/**
*   @brief  Returns the value of the active patch filter 2  band.
*   @param  none
*   @return int	 the value of the active patch filter 2 band (see defs.h).
*/
int mod_synth_get_active_filter_2_band();
/**
*   @brief  Returns the state of the active patch filter 2  tracking filter 1.
*   @param  none
*   @return int	 the state of the active patch filter 2  tracking filter 1.
*/
bool mod_synth_get_active_filter_2_Trac_filter_1_state();

/**
*   @brief  Returns the value of the active patch filter 2 frequency modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch filter 2 frequency modulation LFO number.
*/
int mod_synth_get_active_filter_2_Freq_mod_lfo();
/**
*   @brief  Returns the value of the active patch filter 2 frequency modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch filter 2 frequency modulation LFO level.
*/
int mod_synth_get_active_filter_2_Freq_mod_lfo_level();
/**
*   @brief  Returns the value of the active patch filter 2 frequency modulation ENV.
*   @param  none
*   @return int	 the value of the active patch filter 2 frequency modulation ENV.
*/
int mod_synth_get_active_filter_2_Freq_mod_env();
/**
*   @brief  Returns the value of the active patch filter 2 frequency modulation ENV level.
*   @param  none
*   @return int	 the value of the active patch filter 2 frequency modulation ENV level.
*/
int mod_synth_get_active_filter_2_Freq_mod_env_level();

/**
	*   @brief  Returns the value of the active patch AMP 1 level.
	*   @param  none
	*   @return int	 the value of the active patch AMP 1 level.
	*/
int mod_synth_get_active_ch_1_level();
/**
*   @brief  Returns the value of the active patch AMP 1 pan.
*   @param  none
*   @return int	 the value of the active patch AMP 1 pan.
*	[ 0 -> Left  50 -> mid   100 -> Right]
*/
int mod_synth_get_active_ch_1_pan();
/**
*   @brief  Returns the value of the active patch AMP1 pan modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch  AMP1 pan modulation LFO number.
*/
int mod_synth_get_active_ch_1_pan_mod_lfo();
/**
*   @brief  Returns the value of the active patch AMP 2 pan modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch  AMP 2 pan modulation LFO level.
*/
int mod_synth_get_active_ch_1_pan_mod_lfo_level();	
/**
*   @brief  Returns the value of the active  AMP 1 send.
*   @param  none
*   @return int	 the value of the active h AMP 1 sendn. 0-100
*/
int mod_synth_get_active_ch_1_send();

/**
*   @brief  Returns the value of the active patch AMP 2 level.
*   @param  none
*   @return int	 the value of the active patch AMP 2 level.
*/
int mod_synth_get_active_ch_2_level();
/**
*   @brief  Returns the value of the active patch AMP 2 pan.
*   @param  none
*   @return int	 the value of the active patch AMP 2 pan.
*	[ 0 -> Left  50 -> mid   100 -> Right]
*/
int mod_synth_get_active_ch_2_pan();
/**
*   @brief  Returns the value of the active patch AMP 2 pan modulation LFO number.
*   @param  none
*   @return int	 the value of the active patch  AMP 2 pan modulation LFO number.
*/
int mod_synth_get_active_ch_2_pan_mod_lfo();
/**
*   @brief  Returns the value of the active patch AMP 1 pan modulation LFO level.
*   @param  none
*   @return int	 the value of the active patch  AMP 1 pan modulation LFO level.
*/
int mod_synth_get_active_ch_2_pan_mod_lfo_level();
/**
	*   @brief  Returns the value of the active  AMP 2 send.
	*   @param  none
	*   @return int	 the value of the active h AMP 2 send.0-100
	*/
int mod_synth_get_active_ch_2_send();

/**
*   @brief  Returns the value of the active patch reverbration enable state.
*   @param  none
*   @return bool	 the value of the active patch reverbration enable state.
*/
bool mod_synth_get_active_reverb_enable_state();
/**
*   @brief  Returns the value of the active patch reverbration room size value.
*   @param  none
*   @return int	 the value of the active patch reverbration room size value.
*/
int mod_synth_get_active_reverb_room_size();
/**
*   @brief  Returns the value of the active patch reverbration damp value.
*   @param  none
*   @return int	 the value of the active patch reverbration damp value.
*/
int mod_synth_get_active_reverb_damp();
/**
*   @brief  Returns the value of the active patch reverbration wet value.
*   @param  none
*   @return int	 the value of the active patch reverbration wet value.
*/
int mod_synth_get_active_reverb_wet();
/**
*   @brief  Returns the value of the active patch reverbration dry value.
*   @param  none
*   @return int	 the value of the active patch reverbration dry value.
*/
int mod_synth_get_active_reverb_dry();
/**
*   @brief  Returns the value of the active patch reverbration width value.
*   @param  none
*   @return int	 the value of the active patch reverbration width value.
*/
int mod_synth_get_active_reverb_width();
/**
*   @brief  Returns the value of the active patch reverbration mode value.
*   @param  none
*   @return int	 the value of the active patch reverbration mode value.
*/
int mod_synth_get_active_reverb_mode();

/**
*   @brief  Returns the value of the active patch 3M reverbration enable state.
*   @param  none
*   @return bool	 the value of the active patch 3M reverbration enable state.
*/
bool mod_synth_get_active_reverb_3m_enable_state();
/**
*   @brief  Returns the value of the active patch 3M reverbration preset.
*   @param  none
*   @return int	 the value of the active patch 3M reverbration preset (see defs.h).
*/
int mod_synth_get_active_reverb_3m_preset();
/**
*   @brief  Returns the value of the active patch distortion selected preset (not implemented).
*   @param  none
*   @return int	 the value of the active patch distortion preset.
*/
int mod_synth_get_active_reverb_3m_preset();

/**
*   @brief  Returns the value of the active patch distortion enable state.
*   @param  none
*   @return bool	 the value of the active patch distortion enable state.
*/
bool mod_synth_get_active_distortion_enable_state();
/**
*   @brief  Returns the value of the active patch distortion auto gain enable state.
*   @param  none
*   @return bool	 the value of the active patch distortion auto gain enable state.
*/
bool mod_synth_get_active_distortion_auto_gain_state();
/**
*   @brief  Returns the value of the active patch distortion 1 drive level.
*   @param  none
*   @return int	 the value of the active patch distortion 1 drive level.
*/
int mod_synth_get_active_distortion_1_drive();
/**
*   @brief  Returns the value of the active patch distortion 1 range level.
*   @param  none
*   @return int	 the value of the active patch distortion 1 range level.
*/
int mod_synth_get_active_distortion_1_range();
/**
*   @brief  Returns the value of the active patch distortion 1 blend level.
*   @param  none
*   @return int	 the value of the active patch distortion 1 blend level.
*/
int mod_synth_get_active_distortion_1_blend();
/**
*   @brief  Returns the value of the active patch distortion 2 drive level.
*   @param  none
*   @return int	 the value of the active patch distortion 2 drive level.
*/
int mod_synth_get_active_distortion_2_drive();
/**
*   @brief  Returns the value of the active patch distortion 2 range level.
*   @param  none
*   @return int	 the value of the active patch distortion 2 range level.
*/
int mod_synth_get_active_distortion_2_range();
/**
*   @brief  Returns the value of the active patch distortion 2 blend level.
*   @param  none
*   @return int	 the value of the active patch distortion 2 blend level.
*/
int mod_synth_get_active_distortion_2_blend();

/**
*   @brief  Returns the value of the active patch band-equilizer 31Hz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 31Hz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band31_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 62Hz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 62Hz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band62_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 125Hz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 125Hz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band125_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 250Hz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 250Hz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band250_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 500Hz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 500Hz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band500_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 1KHz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 1KHz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band1k_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 2KHz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 2KHz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band2k_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 4KHz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 4KHz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band4k_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 8KHz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 8KHz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band8k_level();
/**
*   @brief  Returns the value of the active patch band-equilizer 16KHz band level.
*   @param  none
*   @return int	 the value of the active patch band-equilizer 16KHz band level.\n
*	(0 -> -20db    40 -> +20db)
*/
int mod_synth_get_active_equilizer_band16k_level();
/**
*   @brief  Returns the value of the active patch band-equilizer selected preset (not implemented).
*   @param  none
*   @return int	 the value of the active patch band-equilizer selected preset.
*/
int mod_synth_get_active_equilizer_preset();

/**
*   @brief  Returns the value of the active Midi mixer setting parameters.
*   @param  paramid	parameter id: _MIXER_CHAN_n_LEVEL, _MIXER_CHAN_n_PAN, _MIXER_CHAN_n_SEND (n:1-16)
*   @return int	 the value of the active patch band-equilizer selected preset.
*/
int mod_synth_get_active_midi_mixer_setting_param(int paramid);

/**
*   @brief  Returns the value of the active patch portamento time.
*   @param  none
*   @return int	 the value of the active patch portamento time.
*/
int mod_synth_get_active_keyboard_portamento_level();
/**
*   @brief  Returns the value of the active patch portamento enable state.
*   @param  none
*   @return bool	 the value of the active patch portamento enable state.
*/
bool mod_synth_get_active_keyboard_portamento_enabled_state();
	
/**
*   @brief  Returns the value of the active patch keyboard sensitivity.\n
*	Affects all keyboard if no keyboard-split is set.\n
*	Affects only upper keyboard-split when keyboard-split is set.
*   @param  none
*   @return int	 the value of the active patch keyboard sensitivity.
*/
int mod_synth_get_active_keyboard_sensitivity();
/**
*   @brief  Returns the value of the active patch low-keyboard sensitivity.\n
*	Has no effect when keyboard-split is not set.\n
*	Affects only lower keyboard-split when keyboard-split is set.
*   @param  none
*   @return int	 the value of the active patch lower-keyboard sensitivity.
*/
int mod_synth_get_active_keyboard_sensitivity_low();
/**
*   @brief  Returns the value of the active patch keyboard split point.\n
*   @param  none
*   @return int	 the value of the active patch keyboard split point.
*/
int mod_synth_get_active_keyboard_split_point();
/**
*   @brief  Returns the value of the active patch keyboard poly mode.
*   @param  none
*   @return bool	 the value of the active patch keyboard poly mode (see defs.h).
*/


int mod_synth_get_active_keyboard_poly_mode();
/**
*   @brief  Returns the value of the active midi mapping mode.
*   @param  none
*   @return bool	 the value of the active midi mapping mode (see defs.h).
*/
int mod_synth_get_active_midi_mapping_mode();

/**
*   @brief  set the active midi mapping mode.
*   @param  mode	midi mapping mode _MIDI_MAPPING_MODE_NON_MAPPING or _MIDI_MAPPING_MODE_MAPPING
*   @return void
*/
void mod_synth_set_active_midi_mapping_mode(int mode);

/**
*   @brief  Save the active FluidSynth settings parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_save_fluid_synth_settings_file(string path);

/**
*   @brief  Open settings parameters XML file and set it as the FluidSynth active settings
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_open_fluid_synth_settings_file(std::string path);

/**
*   @brief  Save the active AdjSynth patch parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_save_adj_synth_patch_file(string path);

/**
*   @brief  Open settings parameters XML file and set it as the AdjSynth active patch parameters
*   @param  path settings XML file full path
*	@param	channel	midi channel (1-16, 0 for active patch).
*   @return 0 if done
*/
int mod_synth_open_adj_synth_patch_file(std::string path, int channel);

/**
*   @brief  Gets a program patch name
*   @param  prog	program num (1-16)
*   @return program patch name
*/
std::string mod_synth_get_program_patch_name(int prog);

/**
*   @brief  Save the active AdjSynth settingd parameters as XML file
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_save_adj_synth_settings_file(string path);

/**
*   @brief  Open settings parameters XML file and set it as the AdjSynth active settings parameters
*   @param  path settings XML file full path
*   @return 0 if done
*/
int mod_synth_open_adj_synth_settings_file(std::string path);

/**
*   @brief  Save the active AdjSynth general settingd parameters as XML file
*   @param  path general settings XML file full path
*   @return 0 if done
*/
int mod_synth_save_mod_synth_general_settings_file(string path);

/**
*   @brief  Open general settings parameters XML file and set it as the AdjSynth active general settings parameters
*   @param  path general settings XML file full path
*   @return 0 if done
*/
int mod_synth_open_mod_synth_general_settings_file(std::string path);

/**
*   @brief  Save the active ModSynth preset parameters as XML file
*   @param  path preset XML file full path
*   @return 0 if done
*/
int mod_synth_save_mod_synth_preset_file(string path);

/**
*   @brief  Open and load preset parameters XML file and set it as the selected ModSynth preset parameters
*   @param  path preset XML file full path
*   @param	presetNum selected preset id (0 to _MAX_NUM_OF_PRESETS)
*   @return 0 if done
*/
int mod_synth_open_mod_synth_preset_file(std::string path, int presetNum);

/**
*   @brief  Set the active ModSynth preset 
*   @param  presetNum	selected preset (0 to _MAX_NUM_OF_PRESETS)
*   @return none
*/
void mod_synth_set_mod_synth_active_preset_num(int presetNum);

/**
*   @brief  Get the active ModSynth preset 
*   @param  None
*   @return 0 if done
*/
int mod_synth_get_mod_synth_active_preset_num();

/**
*   @brief  Get the preset summary string
*   @param  presetNum	selected preset (0 to _MAX_NUM_OF_PRESETS)
*   @return summary string text
*/
string *mod_synth_get_preset_summary_string(int presetNum);

/**
*   @brief  Return the current master volume set value
*   @param  non
*   @return the a pointer to current master volume set value (0-100)
*/
int mod_synth_get_active_master_volume();
		
/**
*  @brief  Return the current fluid synth volume set value
*  @param  non
*  @return the the current fluid synth volume set value
*/
int mod_synth_get_active_fluid_synth_volume();

/**
*   @brief  Return the current fluid synth reverb room size
*   @param  non
*   @return the the current fluid synth reverb room size
*/
int mod_synth_get_active_fluid_synth_reverb_room_size();

/**
*   @brief  Return the current fluid synth reverb damp
*   @param  non
*   @return the the current fluid synth reverb damp level
*/
int mod_synth_get_active_fluid_synth_reverb_damp();

/**
*   @brief  Return the current fluid synth reverb width
*   @param  non
*   @return the the current fluid synth reverb width level
*/
int mod_synth_get_active_fluid_synth_reverb_width();

/**
*   @brief  Return the current fluid synth reverb level
*   @param  non
*   @return the the current fluid synth level
*/
int mod_synth_get_active_fluid_synth_reverb_level();

/**
*   @brief  Return the current fluid synth chorus number
*   @param  non
*   @return the the current fluid synth chorus number
*/
int mod_synth_get_active_fluid_synth_chorus_number();

/**
*   @brief  Return the current fluid synth chorus level
*   @param  non
*   @return the the current fluid synth chorus level
*/
int mod_synth_get_active_fluid_synth_chorus_level();

/**
*   @brief  Return the current fluid synth chorus speed
*   @param  non
*   @return the the current fluid synth chorus speed
*/
int mod_synth_get_active_fluid_synth_chorus_speed();

/**
*   @brief  Return the current fluid synth chorus depth
*   @param  non
*   @return the the current fluid synth chorus depth
*/
int mod_synth_get_active_fluid_synth_chorus_depth();

/**
*   @brief  Return the current fluid synth chorus modulation waveform
*   @param  non
*   @return the the current fluid synth chorus modulation waveform 0-sine; 1-triangle
*/
int mod_synth_get_active_fluid_synth_chorus_waveform();

/**
*   @brief  Return the current fluid synth reverb activation state
*   @param  non
*   @return the the current fluid synth reverb activation state
*/
bool mod_synth_get_active_fluid_synth_reverb_activation_state();

/**
*   @brief  Return the current fluid synth chorus activation state
*   @param  non
*   @return the the current fluid synth chorus activation state
*/
bool mod_synth_get_active_fluid_synth_chorus_activation_state();

/**
*   @brief  Get the current SoundFont file used by the fluid synth
*   @param  none
*   @return path the string - full path of the sound font file
*/
std::string mod_synth_get_fluid_synth_soundfont();

/**
*   @brief  Get the current fluid synth channel sfont id
*   @param  int chan
*   @return sfont id
*/
int mod_synth_get_fluid_synth_channel_sfont_id(int chan);

/**
*   @brief  Get the current fluid synth channel bank
*   @param  int chan
*   @return bank
*/
int mod_synth_get_fluid_synth_channel_bank(int chan);

/**
*   @brief  Get the current fluid synth channel program
*   @param  int chan
*   @return program
*/
int mod_synth_get_fluid_synth_channel_program(int chan);

/**
*   @brief  Get the current fluid synth channel preset name
*   @param  int chan
*   @return preset name
*/
std::string mod_synth_get_fluid_synth_channel_preset_name(int chan);

/**
*   @brief  Get the current fluid synth channel preset soundfont name
*   @param  int chan
*   @return soundfont name
*/
std::string mod_synth_get_fluid_synth_channel_preset_soundfont_name(int chan);

/**
*   @brief  Get the name of the soundfont id
*   @param  int sfid
*   @return  soundfont name
*/
std::string mod_synth_get_fluid_synth_soundfont_id_name(int sfid);

/**
*   @brief  Get the number of loaded (on th stuck) fluid soundfont files
*   @param  none
*   @return number of soundfonts
*/
int mod_synth_get_num_of_fluid_synth_loaded_soundfonts_files();

/**
*   @brief  Get the presets of the specified fluid soundfont file.
*   @param  sfnum soundfont num - note, this is the number of the soundfont 
*			on the stuck (starting from 0), not the soundfont Id.
*	@param	presets a pointer to a _soundfont_presets_data_t vector to return
*			the specified sound font presets data
*   @return 0 if OK 
*/
int mod_synth_get_fluid_synth_soundfonts_presets(int sfnum, std::vector<_soundfont_presets_data_t> *presets);

/**
*   @brief  Get the presets of all fluid soundfont files.
*	@param	presets a pointer to a _soundfont_presets_data_t vector to return
*			the specified sound font presets data
*   @return 0 if OK
*/
int mod_synth_get_fluid_synth_loaded_soundfonts_presets(std::vector<_soundfont_presets_data_t> *presets);
	
/**
*   @brief  Activate fluid synth panic cleanup
*   @param  none
*   @return none
*/
void mod_synth_panic_action();

// Utils

	/**
	*   @brief  Returns a logaritmic scaled value. (Used for logarithmic based parameters).
	*   @param  float  min  minimum value of logarithmic range.
	*   @param  float  max  maximum value of logarithmic range.
	*   @param  float  base logarithmic base.
	*   @param  int    input value
	*   @return float  logVal = (float)(min + (max - min)*pow((double)base, (double)in / 50.0) / 100.0).
	*/
	float mod_synth_log_scale_100_float(float min, float max, float base, int val);
/**
*   @brief  Returns a logaritmic scaled value. (Used for logarithmic based parameters).
*   @param  int  min  minimum value of logarithmic range.
*   @param  int  max  maximum value of logarithmic range.
*   @param  int  base logarithmic base.
*   @param  int  input value
*   @return int  logVal = (float)(min + (max - min)*pow((double)base, (double)in / 50.0) / 100.0).
*/
int mod_synth_log_scale_100_int(int min, int max, int base, int val);
	
	
// Callback Functions Registration

/**
*   @brief  Register a callback function that initiates a full GUI update.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(void) )
*   @return void
*/
void register_callback_update_ui(func_ptr_void_void_t ptr);

/**
	*   @brief  Register a callback function that updates the detected MIDI devices list.
	*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(void) )
	*   @return void
	*/
void register_callback_update_midi_devices_combo(func_ptr_void_void_t ptr);

/**
*   @brief  Register a callback function that clears a voice-busy GUI indication.
*   @param  funcPtrVoidInt ptr  a pointer to the callback function ( void func(int) )\n
*	Callback int parameter indicates the voice number.
*   @return void
*/
void register_callback_mark_voice_not_busy(func_ptr_void_int_t ptr);
/**
*   @brief  Register a callback function that sets a voice-busy GUI indication.
*   @param  funcPtrVoidInt ptr  a pointer to the callback function ( void func(int) )\n
*	Callback int parameter indicates the voice number.
*   @return void
*/
void register_callback_mark_voice_busy(func_ptr_void_int_t ptr);

/**
*   @brief  Register a callback function that updates the UI FluidSynth settings name text display.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void func(void) )
*   @return void
*/
void register_callback_update_fluid_synth_settings_text(func_ptr_void_string_t ptr);

/**
*   @brief  Register a callback function that initiates updating the status text.
*   @param  funcPtrVoidString ptr  a pointer to the callback function ( void foo(string) )
*   @return void
*/
void register_callback_update_ui_status_text(func_ptr_void_string_t ptr);

/**
*   @brief  Register a callback function that initiates a GUI update of a utiliztion bar.
*   @param  funcPtrVoidInt ptr  a pointer to the callback function ( void func(int) )\n
*	Callback int parameter indicates the utilization %.
*   @return void
*/	
void register_callback_update_utilization_bar(func_ptr_void_int_t ptr);

/**
*   @brief  Register a callback function that initiates a GUI update of a Left-signal strength bar.
*   @param  funcPtrVoidInt ptr  a pointer to the callback function ( void func(int) )\n
*	Callback int parameter indicates the Left-signal strength value %.
*   @return void
*/
void register_callback_update_left_level(func_ptr_void_int_t ptr);
/**
*   @brief  Register a callback function that initiates a GUI update of a Right-signal strength bar.
*   @param  funcPtrVoidInt ptr  a pointer to the callback function ( void func(int) )\n
*	Callback int parameter indicates the Right-signal strength value %.
*   @return void
*/
void register_callback_update_right_level(func_ptr_void_int_t ptr);

/**
*   @brief  Register a callback function that set the BT connection ui indication to active.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void foo(void) )
*   @return void
*/
void register_callback_update_ui_bt_connection_status_active(func_ptr_void_void_t ptr);

/**
*   @brief  Register a callback function that set the BT connection ui indication to not active.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void foo(void) )
*   @return void
*/
void register_callback_update_ui_bt_connection_status_not_active(func_ptr_void_void_t ptr);

/**
*   @brief  Register a callback function that set the JACK connection ui indication to active.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void foo(void) )
*   @return void
*/
void register_callback_update_ui_jack_connection_status_active(func_ptr_void_void_t ptr);

/**
*   @brief  Register a callback function that set the JACK connection ui indication to not active.
*   @param  funcPtrVoidVoid ptr  a pointer to the callback function ( void foo(void) )
*   @return void
*/
void register_callback_update_ui_jack_connection_status_not_active(func_ptr_void_void_t ptr);

/**
*   @brief  Register a callback function that is activated when a midi change program event is detected.
*   @param  funcPtrVoidIntInt ptr  a pointer to the callback function ( void foo(int chan, int prog) )
*   @return void
*/	
void register_callback_midi_program_change_event(func_ptr_void_int_int_t ptr);

/**
*   @brief  Register a callback function that initiates a GUI update of signals oscilooscope display.
*   @param  funcPtrVoidFloatFloatInt ptr  a pointer to the callback function ( void func(float*, float*, int) )\n
*	Callback float* ptr to left signal buffer, float* ptr to right signal buffer, int bufeers length.
*   @return void
*/	
void register_callback_update_signal_display(func_ptr_void_float_ptr_float_ptr_int_t ptr);

/**
*   @brief  Register a callback function that initiate a message popup.
*   @param  funcPtrVoidint ptr  a pointer to the callback function ( void func(int) )\n
*	Callback param: message id.
*   @return void
*/
void register_callback_message_id(func_ptr_void_int_t ptr);
	
/**
*   @brief  Register a callback function that initiates a GUI update of the OSC1 Unison mode.
*   @param  funcPtrVoidInt ptr  a pointer to the callback function ( void func(int) )\n
*	Callback int parameter indicates the unison mode, that provides this mode labels.
*   @return void
*/
void register_set_osc1_unison_mode_callback(func_ptr_void_int_t ptr);

/**
*   @brief  Register a callback function that initiates a GUI update of the midi\n
*	control-sequnces data.
*   @param  funcPtrVoidIntIntInt ptr  a pointer to the callback function ( void func(int, int, int) )\n
*   @return void
*/
void register_update_midi_control_sequnces_data_callback(func_ptr_void_int_int_int_t ptr);

/**
*   @brief  Register a callback function that initiates a GUI update of the synthesizer\n
*	controls data (vcos/filters/amp...  knobs/sliders/pushbuttons...).
*   @param  funcPtrVoidIntIntInt ptr  a pointer to the callback function ( void func(int, int, int) )\n
*   @return void
*/
void register_update_synth_controls_data_callback(func_ptr_void_int_int_int_t ptr);

/**
*   @brief  Register a callback function that initiates a GUI update of a synthesizer module\n
*   @param  funcPtrVoidIntIntInt ptr  a pointer to the callback function ( void func(int) )\n
*   @return void
*/
void register_initiate_update_gui_callback(func_ptr_void_int_t ptr);
	
	
// Callback Functions Activation	
	
void callback_update_ui();

void callback_update_midi_devices_combo();

void callback_mark_voice_not_busy(int voice);
void callback_mark_voice_busy(int voice);
void callback_update_ui_status_text(string txt);
void callback_update_fluid_synth_settings_text(string txt);
void callback_update_utilization_bar(int util);
void callback_update_left_level(int lev);
void callback_update_right_level(int lev);
void callback_update_ui_bt_connection_status_active();
void callback_update_ui_bt_connection_status_not_active();
void callback_update_ui_jack_connection_status_active();
void callback_update_ui_jack_connection_status_not_active();
void callback_midi_program_change_event(int chan, int prog);
void callback_midi_program_change_event(int chan, int prog);
void callback_update_recording_time_display(uint32_t rt);	
void callback_update_signal_display(float *sigL, float *sigR, int size);
void callback_message_id(int);
void callback_set_osc1_unison_mode(int mode);	
void callback_update_midi_control_sequnces_data(int ch, int control_num, int val);
void callback_update_synth_controls_data(int module_id, int control_id, int val);
void callback_initiate_update_gui(int module_id);
	
	



//#ifdef __cplusplus
//}
//#endif

#endif

