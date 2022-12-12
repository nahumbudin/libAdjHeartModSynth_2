/**
*	@file		audioVoice.h
*	@author		Nahum Budin
*	@date		29_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					2. Adding sample-rate and bloc-size settings
*					
*	@version	1.0		11-Nov-2019 (revised version from old libAdjHeartRaspiFlSynthMultiCore_3_1 October 18, 2017)
*
*	@brief		Audio-block polyphonic voice 
*/

#ifndef _AUDIO_VOICE
#define _AUDIO_VOICE

#include "audioBlock.h"
#include "../dsp/dspVoice.h"

#include "../libAdjHeartModSynth_2.h"

#define _SYNTH_VOICE_OUT_1					0
#define _SYNTH_VOICE_OUT_2					1

#define _AMP_AUDIO_IN_CH1					0
#define _AMP_AUDIO_IN_CH2					1

#define _AMP_AUDIO_OUT_L					LEFT
#define _AMP_AUDIO_OUT_R					RIGHT

class AudioVoiceFloat : public AudioBlockFloat
{
public:
	AudioVoiceFloat(
		int stage = 0,
		int samp_rate = _DEFAULT_SAMPLE_RATE,
		int block_size = _DEFAULT_BLOCK_SIZE,
		int voice = 0, 
		int outs = 2, 
		DSP_Voice *dsp_voc = NULL,
		AudioBlockFloat **audio_first_update_ptr = NULL);
	
	int set_sample_rate(int samp_rate);
	int get_sample_rate();
	
	int set_audio_block_size(int size);
	int get_audio_block_size();
	
	void init_poly();
	
	int get_voice_num();
	void set_voice_num(int v_num);
	
	void set_active();
	void set_inactive();
	bool is_voice_active();
	
	void set_wait_for_not_active();
	void reset_wait_for_not_active();
	bool is_voice_wait_for_not_active();
	
	void set_timestamp(uint64_t ts);
	uint64_t get_timestamp();
	
	void set_note(int nt);
	int get_note();
	
	DSP_Voice *get_dsp_voice();

	DSP_Voice *dsp_voice = NULL;
	
	void set_magnitude(float mag);
	float get_magnitude();
	
	virtual void update(void);
	
private:
	
	int voice_num;
	float magnitude;
	bool active, wait_for_not_active;
	int note;
	uint64_t timestamp;
	
	int sample_rate, audio_block_size;
}
;


#endif
