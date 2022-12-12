/**
*	@file		LibAPI_getModulatorsParams.cpp
*	@author		Nahum Budin
*	@date		9-Feb-2021
*	@version	1.1
*
*	@brief		Get active LFOs and ADSRs settings parameters
*
*	History: 
*				ver 1.0	17-jan-2020		1st version
*
*/


#include "../libAdjHeartModSynth_2.h"
#include "LibAPI_settingsManager.h"
#include "../synthesizer/modSynth.h" // why AdjSynth unrecognized?

int res_mod;
mod_synth_settings_int_param_t int_param_mod;

int mod_synth_get_active_env_mod_1_attack()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_1.attack", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_1_decay()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_1.decay", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_1_sustain()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_1.sustain", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_1_release()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_1.release", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}


int mod_synth_get_active_env_mod_2_attack()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_2.attack", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_2_decay()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_2.decay", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_2_sustain()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_2.sustain", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_2_release()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_2.release", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_3_attack()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_3.attack", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_3_decay()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_3.decay", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_3_sustain()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_3.sustain", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_3_release()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_3.release", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_4_attack()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_4.attack", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_4_decay()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_4.decay", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_4_sustain()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_4.sustain", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_4_release()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_4.release", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_5_attack()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_5.attack", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_5_decay()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_5.decay", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_5_sustain()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_5.sustain", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_env_mod_5_release()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.env_5.release", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}


int mod_synth_get_active_lfo_mod_1_waveform()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_1.waveform", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_1_symmetry()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_1.symmetry", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_1_rate()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_1.rate", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}


int mod_synth_get_active_lfo_mod_2_waveform()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_2.waveform", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_2_symmetry()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_2.symmetry", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_2_rate()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_2.rate", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_3_waveform()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_3.waveform", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_3_symmetry()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_3.symmetry", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_3_rate()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_3.rate", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_4_waveform()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_4.waveform", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_4_symmetry()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_4.symmetry", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_4_rate()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_4.rate", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_5_waveform()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_5.waveform", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_5_symmetry()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_5.symmetry", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}

int mod_synth_get_active_lfo_mod_5_rate()
{
	res_mod = settings_manager->get_int_param(AdjSynth::get_instance()->get_active_patch_params(),
		"adjsynth.lfo_5.rate", &int_param_mod);
	if (res_mod == _SETTINGS_KEY_FOUND)
	{
		return int_param_mod.value;
	}
	else
	{
		return 0;
	}
}
