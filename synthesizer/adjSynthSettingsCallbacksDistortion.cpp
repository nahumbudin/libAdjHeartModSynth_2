/**
*	@file		adjSynthSettingsCallbacksEquilizer.cpp
*	@author		Nahum Budin
*	@date		5-Feb-2021
*	@version	1.2
*					1. Code refactoring and notaion.
*
*	@brief		Callback to handle Bands Equilizer settings
*	settings
*
*	History:\n
*
*	version	1.1		25_Dec-2019
*	version 1.0		30_Nov-2019: First version
*
*/

#include "adjSynth.h"

int set_voice_block_distortion_enabled_cb(bool enable, int voice)
{
	if (enable)
	{
		AdjSynth::get_instance()->synth_voice[voice]->dsp_voice->enable_distortion();
	}
	else
	{
		AdjSynth::get_instance()->synth_voice[voice]->dsp_voice->disable_distortion();
	}

	return 0;
}

int set_voice_block_distortion_auto_gain_enabled_cb(bool enable, int voice)
{
	if (enable)
	{
		AdjSynth::get_instance()->synth_voice[voice]->dsp_voice->enable_distortion1_auto_gain();
		AdjSynth::get_instance()->synth_voice[voice]->dsp_voice->enable_distortion2_auto_gain();
	}
	else
	{
		AdjSynth::get_instance()->synth_voice[voice]->dsp_voice->disable_distortion1_auto_gain();
		AdjSynth::get_instance()->synth_voice[voice]->dsp_voice->disable_distortion2_auto_gain();
	}

	return 0;
}


int set_voice_block_distortion_1_drive_cb(int drv, int voice)
{
	AdjSynth::get_instance()->synth_voice[voice]->dsp_voice->distortion1->set_drive((float)drv / 100.f);
	return 0;
}

int set_voice_block_distortion_1_range_cb(int rng, int voice)
{
	float value = ((float)rng / 100.f) * (_DISTORTION_MAX_RANGE - _DISTORTION_MIN_RANGE) + _DISTORTION_MIN_RANGE;

	AdjSynth::get_instance()->synth_voice[voice]->dsp_voice->distortion1->set_range(value);
	return 0;
}

int set_voice_block_distortion_1_blend_cb(int blnd, int voice)
{
	AdjSynth::get_instance()->synth_voice[voice]->dsp_voice->distortion1->set_blend((float)blnd / 100.f);
	return 0;
}


int set_voice_block_distortion_2_drive_cb(int drv, int voice)
{
	AdjSynth::get_instance()->synth_voice[voice]->dsp_voice->distortion2->set_drive((float)drv / 100.f);
	return 0;
}

int set_voice_block_distortion_2_range_cb(int rng, int voice)
{
	float value = ((float)rng / 100.f) * (_DISTORTION_MAX_RANGE - _DISTORTION_MIN_RANGE) + _DISTORTION_MIN_RANGE;

	AdjSynth::get_instance()->synth_voice[voice]->dsp_voice->distortion2->set_range(value);
	return 0;
}

int set_voice_block_distortion_2_blend_cb(int blnd, int voice)
{
	AdjSynth::get_instance()->synth_voice[voice]->dsp_voice->distortion2->set_blend((float)blnd / 100.f);
	return 0;
}
