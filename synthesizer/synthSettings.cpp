/**
* @file		synthSettings.cpp
*	@author		Nahum Budin
*	@date		5-Feb-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					
*	@version	1.0		5-Oct-2019
*	
*	@brief		Synthesizer settings.
*
	
*/

//#include "../headers.h"

#include <mutex>
#include <stdlib.h>
#include <stdint.h>

#include "adjSynth.h"
#include "synthSettings.h"

#include "../libAdjHeartModSynth_2.h"

// Instances of static elements
//ModSynthSettings *ModSynthSettings::mod_synth_settings_instance = NULL;
std::mutex ModSynthSettings::settings_manage_mutex;
//int ModSynthSettings::activeSketch;

uint32_t ModSynthSettings::settings_version = 210122;	// 2021-01-22

/**
 * @brief Creates a settings class object instance
 *
 * @param settings_params a pointer to the synthesizer gsettings parameters object (mandatory parameter)
 * @return none
 */
ModSynthSettings::ModSynthSettings(_setting_params_t *settings_params)
{
	if (settings_params == NULL)
	{
		printf("fatal error - fluid synth - no settings params");
		exit(1);
	}

	active_settings_params = settings_params;
	
//	init_fluid_synth_settings_defaults(active_settings_params);
}

ModSynthSettings::~ModSynthSettings()
{
	active_settings_params->string_parameters_map.clear();
	active_settings_params->int_parameters_map.clear();
	active_settings_params->float_parameters_map.clear();
	active_settings_params->bool_parameters_map.clear();
		
}

/**
  * @brief Get the active settings parameters
  *
  * @param none
  * @return the active settings prameters
  */
_setting_params_t *ModSynthSettings::get_active_settings_parameters()
{
	return active_settings_params;
}

/**
*	@brief	Deep copy settings parameters from given source to destination.
*
*	@param	destinationParams	destination _setting_params_t
*	@param	sourceParams		source _setting_params_t
*	@return void
*/
void ModSynthSettings::settings_params_deep_copy(_setting_params_t *destinationParams, _setting_params_t *sourceParams)
{
	destinationParams = (_setting_params_t*)malloc(sizeof(*sourceParams)) ;
	memcpy(destinationParams, sourceParams, sizeof(*sourceParams));
}

/**
 * @brief Get the settings version.
 *
 * @param none
 * @return settings version
 */
uint32_t ModSynthSettings::get_settings_version() 
{
	return settings_version;
}

/**
 * @brief Get the specified string setting param structure.
 *
 * @param settings all settings object (mandatory parameter)
 * @param name the setting's name (mandatory parameter)
 * @param param pointer to a requested settings param (mandatory parameter)
 * @return parameter #_SETTINGS_OK if the name has been found
 */
settings_res_t ModSynthSettings::get_string_param(_setting_params_t *settings, string name,
													mod_synth_settings_str_param_t *param)
{
	settings_res_t res = _SETTINGS_KEY_NOT_FOUND;
	std::map<std::string, mod_synth_settings_str_param_t>::iterator iter;

	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(param == NULL, _SETTINGS_BAD_PARAMETERS);

	//	settings_manage_mutex.lock();
	
	_setting_params_t *_settings;
	
	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the parameter name
	iter = _settings->string_parameters_map.find(name);
	if (iter != _settings->string_parameters_map.end())
	{
		// Parameter found
		param->key = name;
		param->type = iter->second.type;
		param->value = iter->second.value;
		param->setup_callback = iter->second.setup_callback;
		param->callback_set = iter->second.callback_set;
		param->block_start_index = iter->second.block_start_index;
		param->block_stop_index = iter->second.block_stop_index;
		param->block_setup_callback = iter->second.block_setup_callback;
		param->block_callback_set = iter->second.block_callback_set;
		res = _SETTINGS_KEY_FOUND;
	}

//	settings_manage_mutex.unlock();

	return res;
}

/**
 * @brief Get the specified integer setting param structure.
 *
 * @param settings all settings structure (mandatory parameter)
 * @param name the setting's name (mandatory parameter)
 * @param param pointer to a requested settings param (mandatory parameter)
 * @return parameter #_SETTINGS_OK if the name has been found
 */
settings_res_t ModSynthSettings::get_int_param(_setting_params_t *settings, string name,
												 mod_synth_settings_int_param_t *param)
{
	settings_res_t res = _SETTINGS_KEY_NOT_FOUND;
	std::map<std::string, mod_synth_settings_int_param_t>::iterator iter;

	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(param == NULL, _SETTINGS_BAD_PARAMETERS);

//	settings_manage_mutex.lock();
	
	_setting_params_t *_settings;
	
	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the parameter name
	iter = _settings->int_parameters_map.find(name);
	if (iter != _settings->int_parameters_map.end())
	{
		// Parameter found
		param->key = name;
		param->type = iter->second.type;
		param->value = iter->second.value;
		param->max_val = iter->second.max_val;
		param->min_val = iter->second.min_val;
		param->limits_set = iter->second.limits_set;
		param->setup_callback = iter->second.setup_callback;
		param->callback_set = iter->second.callback_set;
		param->block_start_index = iter->second.block_start_index;
		param->block_stop_index = iter->second.block_stop_index;
		param->block_setup_callback = iter->second.block_setup_callback;
		param->block_callback_set = iter->second.block_callback_set;
		res = _SETTINGS_KEY_FOUND;
	}

//	settings_manage_mutex.unlock();

	return res;
}

/**
 * @brief Get the specified float (double) setting param structure.
 *
 * @param settings all settings structure (mandatory parameter)
 * @param name the setting's name (mandatory parameter)
 * @param param pointer to a requested settings param (mandatory parameter)
 * @return parameter #_SETTINGS_OK if the name has been found
 */
settings_res_t ModSynthSettings::get_float_param(_setting_params_t *settings, string name,
												   mod_synth_settings_float_param_t *param)
{
	settings_res_t res = _SETTINGS_KEY_NOT_FOUND;
	std::map<std::string, mod_synth_settings_float_param_t>::iterator iter;

	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(param == NULL, _SETTINGS_BAD_PARAMETERS);

//	settings_manage_mutex.lock();
	
	_setting_params_t *_settings;
	
	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the parameter name
	iter = _settings->float_parameters_map.find(name);
	if (iter != _settings->float_parameters_map.end())
	{
		// Parameter found
		param->key = name;
		param->type = iter->second.type;
		param->value = iter->second.value;
		param->max_val = iter->second.max_val;
		param->min_val = iter->second.min_val;
		param->limits_set = iter->second.limits_set;
		param->setup_callback = iter->second.setup_callback;
		param->callback_set = iter->second.callback_set;
		param->block_start_index = iter->second.block_start_index;
		param->block_stop_index = iter->second.block_stop_index;
		param->block_setup_callback = iter->second.block_setup_callback;
		param->block_callback_set = iter->second.block_callback_set;
		res = _SETTINGS_KEY_FOUND;
	}

//	settings_manage_mutex.unlock();

	return res;
}

/**
 * @brief Get the specified boolean setting param structure.
 *
 * @param settings all settings structure (mandatory parameter)
 * @param name the setting's name (mandatory parameter)
 * @param param pointer to a requested settings param (mandatory parameter)
 * @return parameter #_SETTINGS_OK if the name has been found
 */
settings_res_t ModSynthSettings::get_bool_param(_setting_params_t *settings, string name,
												  mod_synth_settings_bool_param_t *param)
{
	settings_res_t res = _SETTINGS_KEY_NOT_FOUND;
	std::map<std::string, mod_synth_settings_bool_param_t>::iterator iter;

	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(param == NULL, _SETTINGS_BAD_PARAMETERS);

//	settings_manage_mutex.lock();
	
	_setting_params_t *_settings;
	
	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the parameter name
	iter = _settings->bool_parameters_map.find(name);
	if (iter != _settings->bool_parameters_map.end())
	{
		// Parameter found
		param->key = name;
		param->type = iter->second.type;
		param->value = iter->second.value;
		param->setup_callback = iter->second.setup_callback;
		param->callback_set = iter->second.callback_set;
		param->block_start_index = iter->second.block_start_index;
		param->block_stop_index = iter->second.block_stop_index;
		param->block_setup_callback = iter->second.block_setup_callback;
		param->block_callback_set = iter->second.block_callback_set;
		res = _SETTINGS_KEY_FOUND;
	}

//	settings_manage_mutex.unlock();

	return res;
}

/**
 * @brief	Set the specified string setting param.
 *			If no param with specified name is found a new param is created.
 *
 * @param settings a settings parameters structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new string
 * @param type indicates the parameter type (#_PARAM_TYPE_NONE, #_PARAM_TYPE_GLOABAL_SETUP, 
 *											 #_PARAM_TYPE_FLUID_SYNTH_SETUP, _PARAM_TYPE_ADJ_SYNTH_PATCH)
 * @param callback an update function for the settings
 * @param block_start start index for block settings
 * @param block_stop stop index for block settings
 * @param block_callback a block update function for the settings
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE, #_SET_VALUE | #_SET_MAX_VAL | #_SET_MIN_VA |, #_SET_TYPE | 
 *					#_SET_CALLBACK | #_EXEC_CALLBACK | #_SET_BLOCK_START_INDEX
 *					#_SET_BLOCK_STOP_INDEX | #_SET_BLOCK_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t ModSynthSettings::set_string_param(_setting_params_t *settings, 
													string name,
													string value,
													int type,
													string_param_update_callback_t callback,
													int block_start,
													int block_stop,
													string_block_param_update_callback_t block_callback,
													uint16_t set_mask,
													int prog)
{
	settings_res_t res = _SETTINGS_FAILED;

	mod_synth_settings_str_param_t param;
	mod_synth_settings_str_param_t new_param;

	// Verify mandatory params
	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	
	settings_manage_mutex.lock();
	
	_setting_params_t *_settings;
	
	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the specified param
	res = get_string_param(_settings, name, &param);
	if (res == _SETTINGS_KEY_NOT_FOUND)
	{
		// Its a new parameter - add new
		param.key = name;

		if _CHECK_MASK(set_mask, _SET_VALUE)
		{
			param.value = value;
		}
		else
		{
			param.value = "";
		}

		if _CHECK_MASK(set_mask, _SET_TYPE)
		{
			param.type = type;
		}
		else
		{
			param.type = _PARAM_TYPE_NONE;
		}

		if _CHECK_MASK(set_mask, _SET_CALLBACK)
		{
			param.setup_callback = callback;
			param.callback_set = true;
		}
		else
		{
			param.setup_callback = NULL;
			param.callback_set = false;
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
		{
			param.block_start_index = block_start;	
		}
		else
		{
			param.block_start_index = -1;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
		{
			param.block_stop_index = block_stop;	
		}
		else
		{
			param.block_stop_index = -1;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
		{
			param.block_setup_callback = block_callback;
			param.block_callback_set = true;
		}
		else
		{
			param.block_setup_callback = NULL;
			param.block_callback_set = false;
		}

		_settings->string_parameters_map[name] = param;
	}
	else if (res == _SETTINGS_KEY_FOUND)
	{
		// Parameter already defined - update
		new_param.key = param.key;

		if _CHECK_MASK(set_mask, _SET_VALUE)
		{
			new_param.value = value;
		}
		else
		{
			new_param.value = param.value;
		}

		if _CHECK_MASK(set_mask, _SET_TYPE)
		{
			new_param.type = type;
		}
		else
		{
			new_param.type = param.type;
		}		

		if _CHECK_MASK(set_mask, _SET_CALLBACK)
		{
			// A new callback function - update
			new_param.setup_callback = callback;
			new_param.callback_set = true;
		}
		else
		{
			new_param.setup_callback = param.setup_callback;
			new_param.callback_set = param.callback_set;
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
		{
			new_param.block_start_index = block_start;	
		}
		else
		{
			new_param.block_start_index = param.block_start_index;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
		{
			new_param.block_stop_index = block_stop;	
		}
		else
		{
			new_param.block_stop_index = param.block_stop_index;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
		{
			new_param.block_setup_callback = block_callback;
			new_param.block_callback_set = true;
		}
		else
		{
			new_param.block_setup_callback = param.block_setup_callback;
			new_param.block_callback_set = param.block_callback_set;
		}

		_settings->string_parameters_map[name] = new_param;
	}	

	// Actual update is required?
	if ((_CHECK_MASK(set_mask, _EXEC_CALLBACK)) && 
		(_settings->string_parameters_map[name].setup_callback != NULL))
	{
		// Call update callback
		_settings->string_parameters_map[name].setup_callback(value, prog);
	}
	
	if ((_CHECK_MASK(set_mask, _EXEC_BLOCK_CALLBACK)) && 
		(_settings->string_parameters_map[name].block_setup_callback != NULL))
	{
		// Call block update callback
		if(_settings->string_parameters_map[name].block_start_index >= 0 &&
			(_settings->string_parameters_map[name].block_stop_index >= 
			_settings->string_parameters_map[name].block_start_index))
		{
			for (int i = _settings->string_parameters_map[name].block_start_index;
			     i <= _settings->string_parameters_map[name].block_stop_index; i++)
			{
				_settings->string_parameters_map[name].block_setup_callback(value, i, prog);
			}
		}
	}

	settings_manage_mutex.unlock();

	return _SETTINGS_OK;
}

/**
 * @brief	Set the specified int setting param.
 *			If no param with specified name is found a new param is created.
 *
 * @param settings all settings structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new int value
 * @param max_val the param maximum valid value
 * @param min_val the param minimum valid value
 * @param type indicates the parameter type (#_PARAM_TYPE_NONE, #_PARAM_TYPE_GLOABAL_SETUP, 
 *											 #_PARAM_TYPE_FLUID_SYNTH_SETUP, _PARAM_TYPE_ADJ_SYNTH_PATCH)
 * @param callback an update function for the settings
 * @param block_start start index for block settings
 * @param block_stop stop index for block settings
 * @param block_callback a block update function for the settings											 
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE, #_SET_VALUE | #_SET_MAX_VAL | #_SET_MIN_VA |, #_SET_TYPE | 
 *					#_SET_CALLBACK | #_EXEC_CALLBACK | #_SET_BLOCK_START_INDEX
 *					#_SET_BLOCK_STOP_INDEX | #_SET_BLOCK_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t ModSynthSettings::set_int_param(_setting_params_t *settings,
												string name,
												int value,
												int max_val, int min_val,
												int type,
												int_param_update_callback_t callback,
												int block_start,
												int block_stop,
												int_block_param_update_callback_t block_callback,
												uint16_t set_mask,
												int prog)
{
	settings_res_t res = _SETTINGS_FAILED;
	mod_synth_settings_int_param_t param;
	mod_synth_settings_int_param_t new_param;

		// Verify mandatory params
	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	
	settings_manage_mutex.lock();
	
	_setting_params_t *_settings;
	
	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the specified param
	res = get_int_param(_settings, name, &param);
	if (res == _SETTINGS_KEY_NOT_FOUND)
	{
		// Its a new parameter - add it
		param.key = name;
		
		if _CHECK_MASK(set_mask, _SET_MAX_VAL)
		{
			param.max_val = max_val;
			param.limits_set = true;
		}

		if _CHECK_MASK(set_mask, _SET_MIN_VAL)
		{
			param.min_val = min_val;
			param.limits_set = true;
		}

		if _CHECK_MASK(set_mask, _SET_VALUE)
		{
			if (!param.limits_set)
			{
				param.value = value;
			}
			else
			{
				if ((value < param.min_val) || (value > param.max_val))
				{
					// out of rang - abbort
					settings_manage_mutex.unlock();
					return _SETTINGS_PARAM_OUT_OF_RANGE;
				}
				else
				{
					param.value = value;
				}
			}
		}

		if _CHECK_MASK(set_mask, _SET_TYPE)
		{
			param.type = type;
		}
		else
		{
			param.type = _PARAM_TYPE_NONE;
		}

		if _CHECK_MASK(set_mask, _SET_CALLBACK)
		{
			param.setup_callback = callback;
			param.callback_set = true;
		}
		else
		{
			param.setup_callback = NULL;
			param.callback_set = false;
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
		{
			param.block_start_index = block_start;	
		}
		else
		{
			param.block_start_index = -1;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
		{
			param.block_stop_index = block_stop;	
		}
		else
		{
			param.block_stop_index = -1;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
		{
			param.block_setup_callback = block_callback;
			param.block_callback_set = true;
		}
		else
		{
			param.block_setup_callback = NULL;
			param.block_callback_set = false;
		}

		_settings->int_parameters_map[name] = param;
	}
	else if (res == _SETTINGS_KEY_FOUND)
	{
		// Parameter already defined - update
		new_param.key = param.key;

		if _CHECK_MASK(set_mask, _SET_MAX_VAL)
		{
			new_param.max_val = max_val;
			new_param.limits_set = true;
		}
		else
		{
			new_param.max_val = param.max_val;
			new_param.limits_set = param.limits_set;
		}

		if _CHECK_MASK(set_mask, _SET_MIN_VAL)
		{
			new_param.min_val = min_val;
			new_param.limits_set = true;
		}
		else
		{
			new_param.min_val = param.min_val;
			new_param.limits_set = param.limits_set;
		}

		if _CHECK_MASK(set_mask, _SET_VALUE)
		{
			if (!param.limits_set)
			{
				new_param.value = value;
			}
			else
			{
				if ((value < new_param.min_val) || (value > new_param.max_val))
				{
					// out of rang - abbort
					settings_manage_mutex.unlock();
					return _SETTINGS_PARAM_OUT_OF_RANGE;
				}
				else
				{
					new_param.value = value;
				}
			}
		}
		else
		{
			new_param.value = param.value;
		}

		if _CHECK_MASK(set_mask, _SET_TYPE)
		{
			new_param.type = type;
		}
		else
		{
			new_param.type = param.type;
		}

		if _CHECK_MASK(set_mask, _SET_CALLBACK)
		{
			new_param.setup_callback = callback;
			new_param.callback_set = true;
		}
		else
		{
			new_param.setup_callback = param.setup_callback;
			new_param.callback_set = param.callback_set;
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
		{
			new_param.block_start_index = block_start;	
		}
		else
		{
			new_param.block_start_index = param.block_start_index;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
		{
			new_param.block_stop_index = block_stop;	
		}
		else
		{
			new_param.block_stop_index = param.block_stop_index;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
		{
			new_param.block_setup_callback = block_callback;
			new_param.block_callback_set = true;
		}
		else
		{
			new_param.block_setup_callback = param.block_setup_callback;
			new_param.block_callback_set = param.block_callback_set;
		}
		
		_settings->int_parameters_map[name] = new_param;
	}

	// Actual update is required?
	if ((_CHECK_MASK(set_mask, _EXEC_CALLBACK)) && 
		(_settings->int_parameters_map[name].setup_callback != NULL))
	{
		// Call update callback
 		_settings->int_parameters_map[name].setup_callback(value, prog);
	}
	
	if ((_CHECK_MASK(set_mask, _EXEC_BLOCK_CALLBACK)) && 
		(_settings->int_parameters_map[name].block_setup_callback != NULL))
	{
		// Call block update callback
		if((_settings->int_parameters_map[name].block_start_index >= 0) &&
		    (_settings->int_parameters_map[name].block_stop_index >= 
			 _settings->int_parameters_map[name].block_start_index))
		{
			for (int i = _settings->int_parameters_map[name].block_start_index;
				 i <= _settings->int_parameters_map[name].block_stop_index; i++)
			{
				_settings->int_parameters_map[name].block_setup_callback(value, i, prog);
			}
		}
	}

	settings_manage_mutex.unlock();

	return _SETTINGS_OK;
}

/**
 * Set the specified float (double) setting param.
 *	If no param with specified name is found a new param is created.
 *
 * @param settings all settings structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new float (double) value
 * @param max_val the param maximum valid value
 * @param min_val the param minimum valid value
 * @param type indicates the parameter type (#_PARAM_TYPE_NONE, #_PARAM_TYPE_GLOABAL_SETUP, 
 *											 #_PARAM_TYPE_FLUID_SYNTH_SETUP, _PARAM_TYPE_ADJ_SYNTH_PATCH)
 * @param callback an update function for the settings
 * @param block_start start index for block settings
 * @param block_stop stop index for block settings
 * @param block_callback a block update function for the settings
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE, #_SET_VALUE | #_SET_MAX_VAL | #_SET_MIN_VA |, #_SET_TYPE | 
 *					#_SET_CALLBACK | #_EXEC_CALLBACK | #_SET_BLOCK_START_INDEX
 *					#_SET_BLOCK_STOP_INDEX | #_SET_BLOCK_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t ModSynthSettings::set_float_param(_setting_params_t *settings,
													string name,
													double value,
													double max_val, double min_val,
													int type,
													float_param_update_callback_t callback,
													int block_start,
													int block_stop,
													float_block_param_update_callback_t block_callback,
													uint16_t set_mask,
													int prog)
{
	settings_res_t res = _SETTINGS_FAILED;
	mod_synth_settings_float_param_t param;
	mod_synth_settings_float_param_t new_param;

	// Verify mandatory params
	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	
	settings_manage_mutex.lock();
	
	_setting_params_t *_settings;
	
	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the specified param
	res = get_float_param(_settings, name, &param);
	if (res == _SETTINGS_KEY_NOT_FOUND)
	{
		// Its a new parameter - add it
		param.key = name;

		if _CHECK_MASK(set_mask, _SET_MAX_VAL)
		{
			param.max_val = max_val;
			param.limits_set = true;
		}

		if _CHECK_MASK(set_mask, _SET_MIN_VAL)
		{
			param.min_val = min_val;
			param.limits_set = true;
		}

		if _CHECK_MASK(set_mask, _SET_VALUE)
		{
			if (!param.limits_set)
			{
				param.value = value;
			}
			else
			{
				if ((value < param.min_val) || (value > param.max_val))
				{
					// out of rang - abbort
					settings_manage_mutex.unlock();
					return _SETTINGS_PARAM_OUT_OF_RANGE;
				}
				else
				{
					param.value = value;
				}
			}
		}

		if _CHECK_MASK(set_mask, _SET_TYPE)
		{
			param.type = type;
		}
		else
		{
			param.type = _PARAM_TYPE_NONE;
		}

		if _CHECK_MASK(set_mask, _SET_CALLBACK)
		{
			param.setup_callback = callback;
			param.callback_set = true;
		}
		else
		{
			param.setup_callback = NULL;
			param.callback_set = false;
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
		{
			param.block_start_index = block_start;	
		}
		else
		{
			param.block_start_index = -1;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
		{
			param.block_stop_index = block_stop;	
		}
		else
		{
			param.block_stop_index = -1;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
		{
			param.block_setup_callback = block_callback;
			param.block_callback_set = true;
		}
		else
		{
			param.block_setup_callback = NULL;
			param.block_callback_set = false;
		}

		_settings->float_parameters_map[name] = param;
	}
	else if (res == _SETTINGS_KEY_FOUND)
	{
		// Parameter already defined - update
		new_param.key = name;

		if _CHECK_MASK(set_mask, _SET_MAX_VAL)
		{
			new_param.max_val = max_val;
			new_param.limits_set = true;
		}
		else
		{
			new_param.max_val = param.max_val;
			new_param.limits_set = param.limits_set;
		}

		if _CHECK_MASK(set_mask, _SET_MIN_VAL)
		{
			new_param.min_val = min_val;
			new_param.limits_set = true;
		}
		else
		{
			new_param.min_val = param.min_val;
			new_param.limits_set = param.limits_set;
		}

		if _CHECK_MASK(set_mask, _SET_VALUE)
		{
			if (!param.limits_set)
			{
				new_param.value = value;
			}
			else
			{
				if ((value < new_param.min_val) || (value > new_param.max_val))
				{
					// out of rang - abbort
					settings_manage_mutex.unlock();
					return _SETTINGS_PARAM_OUT_OF_RANGE;
				}
				else
				{
					new_param.value = value;
				}
			}
		}
		else
		{
			new_param.value = param.value;
		}

		if _CHECK_MASK(set_mask, _SET_TYPE)
		{
			new_param.type = type;
		}
		else
		{
			new_param.type = param.type;
		}

		if _CHECK_MASK(set_mask, _SET_CALLBACK)
		{
			new_param.setup_callback = callback;
			new_param.callback_set = true;
		}
		else
		{
			new_param.setup_callback = param.setup_callback;
			new_param.callback_set = param.callback_set;
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
		{
			new_param.block_start_index = block_start;	
		}
		else
		{
			new_param.block_start_index = param.block_start_index;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
		{
			new_param.block_stop_index = block_stop;	
		}
		else
		{
			new_param.block_stop_index = param.block_stop_index;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
		{
			new_param.block_setup_callback = block_callback;
			new_param.block_callback_set = true;
		}
		else
		{
			new_param.block_setup_callback = param.block_setup_callback;
			new_param.block_callback_set = param.block_callback_set;
		}

		_settings->float_parameters_map[name] = new_param;
	}

	// Actual update is required?
	if ((_CHECK_MASK(set_mask, _EXEC_CALLBACK)) && 
		(_settings->float_parameters_map[name].setup_callback != NULL))
	{
		// Call update callback
		_settings->float_parameters_map[name].setup_callback(value, prog);
	}
	
	if ((_CHECK_MASK(set_mask, _EXEC_BLOCK_CALLBACK)) && 
		(_settings->float_parameters_map[name].block_setup_callback != NULL))
	{
		// Call block update callback
		if ((_settings->float_parameters_map[name].block_start_index >= 0) &&
			(_settings->float_parameters_map[name].block_stop_index >= 
			 _settings->float_parameters_map[name].block_start_index))
		{
			for (int i = _settings->float_parameters_map[name].block_start_index;
				 i <= _settings->float_parameters_map[name].block_stop_index; i++)
			{
				_settings->float_parameters_map[name].block_setup_callback(value, i, prog);
			}
		}
	}

	settings_manage_mutex.unlock();

	return _SETTINGS_OK;
}

/**
* Set the specified boolean setting param.
*	If no param with specified name is found a new param is created.
*
* @param settings all settings structure (mandatory parameter)
* @param name the setting's name  (mandatory parameter)
* @param value the param new boolean
* @param type indicates the parameter type (#_PARAM_TYPE_NONE, #_PARAM_TYPE_GLOABAL_SETUP, 
*											 #_PARAM_TYPE_FLUID_SYNTH_SETUP, _PARAM_TYPE_ADJ_SYNTH_PATCH)
* @param callback an update function for the settings
* @param block_start start index for block settings
* @param block_stop stop index for block settings
* @param block_callback a block update function for the settings
* @param _set_mask a bit map that indicates what parameter's fields should be set or update
*					#_SET_NONE, #_SET_VALUE | #_SET_MAX_VAL | #_SET_MIN_VA |, #_SET_TYPE | 
*					#_SET_CALLBACK | #_EXEC_CALLBACK | #_SET_BLOCK_START_INDEX
*					#_SET_BLOCK_STOP_INDEX | #_SET_BLOCK_CALLBACK | #_EXEC_BLOCK_CALLBACK
* @param prog	program number
* @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
*/
settings_res_t ModSynthSettings::set_bool_param(_setting_params_t *settings,
												string name,
												bool value,
												int type,
												bool_param_update_callback_t callback, 
												int block_start,
												int block_stop,
												bool_block_param_update_callback_t block_callback,
												uint16_t set_mask,
												int prog)
{
	settings_res_t res = _SETTINGS_FAILED;
	mod_synth_settings_bool_param_t param;
	mod_synth_settings_bool_param_t new_param;

	// Verify mandatory params
	return_val_if_true(settings == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	
	settings_manage_mutex.lock();
	
	_setting_params_t *_settings;
	
	if (settings == NULL)
	{
		_settings = active_settings_params;
	}
	else
	{
		_settings = settings;
	}

	// Look for the specified param
	res = get_bool_param(_settings, name, &param);
	if (res == _SETTINGS_KEY_NOT_FOUND)
	{
		// Its a new parameter - add new
		param.key = name;
		
		if _CHECK_MASK(set_mask, _SET_VALUE)
		{
			param.value = value;
		}
		else
		{
			param.value = false;
		}

		if _CHECK_MASK(set_mask, _SET_TYPE)
		{
			param.type = type;
		}
		else
		{
			param.type = _PARAM_TYPE_NONE;
		}

		if _CHECK_MASK(set_mask, _SET_CALLBACK)
		{
			param.setup_callback = callback;
			param.callback_set = true;
		}
		else
		{
			param.setup_callback = NULL;
			param.callback_set = false;
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
		{
			param.block_start_index = block_start;	
		}
		else
		{
			param.block_start_index = -1;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
		{
			param.block_stop_index = block_stop;	
		}
		else
		{
			param.block_stop_index = -1;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
		{
			param.block_setup_callback = block_callback;
			param.block_callback_set = true;
		}
		else
		{
			param.block_setup_callback = NULL;
			param.block_callback_set = false;
		}

		_settings->bool_parameters_map[name] = param;
	}
	else if (res == _SETTINGS_KEY_FOUND)
	{
		// Parameter already defined - update
		new_param.key = param.key;

		if _CHECK_MASK(set_mask, _SET_VALUE)
		{
			new_param.value = value;
		}
		else
		{
			new_param.value = param.value;
		}

		if _CHECK_MASK(set_mask, _SET_TYPE)
		{
			new_param.type = type;
		}
		else
		{
			new_param.type = param.type;
		}

		if _CHECK_MASK(set_mask, _SET_CALLBACK)
		{
			// A new callback function - update
			new_param.setup_callback = callback;
			new_param.callback_set = true;
		}
		else
		{
			new_param.setup_callback = param.setup_callback;
			new_param.callback_set = param.callback_set;
		}
		if _CHECK_MASK(set_mask, _SET_BLOCK_START_INDEX)
		{
			new_param.block_start_index = block_start;	
		}
		else
		{
			new_param.block_start_index = param.block_start_index;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_STOP_INDEX)
		{
			new_param.block_stop_index = block_stop;	
		}
		else
		{
			new_param.block_stop_index = param.block_stop_index;	
		}
		
		if _CHECK_MASK(set_mask, _SET_BLOCK_CALLBACK)
		{
			new_param.block_setup_callback = block_callback;
			new_param.block_callback_set = true;
		}
		else
		{
			new_param.block_setup_callback = param.block_setup_callback;
			new_param.block_callback_set = param.block_callback_set;
		}

		_settings->bool_parameters_map[name] = new_param;
	}

	// Actual update is required?
	if ((_CHECK_MASK(set_mask, _EXEC_CALLBACK)) && 
		(_settings->bool_parameters_map[name].setup_callback != NULL))
	{
		// Call update callback
		_settings->bool_parameters_map[name].setup_callback(value, prog);
	}
	
	if ((_CHECK_MASK(set_mask, _EXEC_BLOCK_CALLBACK)) && 
		(_settings->bool_parameters_map[name].block_setup_callback != NULL))
	{
		// Call block update callback
		if((_settings->bool_parameters_map[name].block_start_index >= 0) &&
			(_settings->bool_parameters_map[name].block_stop_index >= 
			_settings->bool_parameters_map[name].block_start_index))
		{
			for (int i = _settings->bool_parameters_map[name].block_start_index;
				 i <= _settings->bool_parameters_map[name].block_stop_index; i++)
			{
				_settings->bool_parameters_map[name].block_setup_callback(value, i, prog);
			}
		}
	}

	settings_manage_mutex.unlock();

	return _SETTINGS_OK;
}

/**
 * @brief	Set the specified string setting param value only.
 *			if one of the set flags #_EXEC_CALLBACK or #_EXEC_BLOCK_CALLBACK is used,
 *			the set value call back is also executed.
 *			If no param with specified name is found operation fails.
 *
 * @param settings a settings parameters structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new string
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE | #_EXEC_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t ModSynthSettings::set_string_param_value(_setting_params_t *settings, 
														string name, 
														string value, 
														uint16_t set_mask,
														int prog)
{
	return set_string_param(settings, name, value, 0, NULL, 0, 0, NULL, set_mask | _SET_VALUE,prog);	
}

/**
 * @brief	Set the specified integer setting param value only.
 *			if one of the set flags #_EXEC_CALLBACK or #_EXEC_BLOCK_CALLBACK is used,
 *			the set value call back is also executed.
 *			If no param with specified name is found operation fails.
 *
 * @param settings a settings parameters structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new string
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE | #_EXEC_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t ModSynthSettings::set_int_param_value(_setting_params_t *settings, 
													 string name, 
													 int value, 
													 uint16_t set_mask,
													 int prog)
{
	return set_int_param(settings, name, value, 0, 0, 0, NULL, 0, 0, NULL, set_mask | _SET_VALUE, prog);
}

/**
 * @brief	Set the specified float setting param value only.
 *			if one of the set flags #_EXEC_CALLBACK or #_EXEC_BLOCK_CALLBACK is used,
 *			the set value call back is also executed.
 *			If no param with specified name is found operation fails.
 *
 * @param settings a settings parameters structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new string
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE | #_EXEC_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t ModSynthSettings::set_float_param_value(_setting_params_t *settings, 
													   string name, 
													   double value, 
													   uint16_t set_mask,
													   int prog)
{
	return set_float_param(settings, name, value, 0, 0, 0, NULL, 0, 0, NULL, set_mask | _SET_VALUE, prog);	
}

/**
 * @brief	Set the specified boolean setting param value only.
 *			if one of the set flags #_EXEC_CALLBACK or #_EXEC_BLOCK_CALLBACK is used,
 *			the set value call back is also executed.
 *			If no param with specified name is found operation fails.
 *
 * @param settings a settings parameters structure (mandatory parameter)
 * @param name the setting's name  (mandatory parameter)
 * @param value the param new string
 * @param _set_mask a bit map that indicates what parameter's fields should be set or update
 *					#_SET_NONE | #_EXEC_CALLBACK | #_EXEC_BLOCK_CALLBACK
 * @param prog	program number
 * @return #_SETTINGS_OK if the callback has been set, #_SETTINGS_FAILED otherwise
 */
settings_res_t ModSynthSettings::set_bool_param_value(_setting_params_t *settings, 
													  string name, 
													  bool value, 
													  uint16_t set_mask,
													  int prog)
{
	return set_bool_param(settings, name, value, 0, NULL, 0, 0, NULL, set_mask | _SET_VALUE, prog);
}


/**
 * Save active settings, sets active sketch number and 
 * copy active sketch settings to active settings.
 *
 * @param ask requested active sketch number (_SKETCH_PROGRM_1-3)
 * @return none
 */
void ModSynthSettings::set_active_sketch(int ask)
{
	if ((ask > _SKETCH_PROGRAM_1) && (ask <= _SKETCH_PROGRAM_3))
	{
		AdjSynth::get_instance()->set_active_sketch(ask);
	}
}

/**
 * Get the active sketch number.
 *
 * @param none
 * @return active sketch number
 */
//int ModSynthSettings::getActiveSketch() { return activeSketch; }



/**
 *	Returns the index of map key value
 */
int ModSynthSettings::look_for_key(std::string key, std::map<std::string, int> paramsMap)
{
	std::map<std::string, int>::iterator it;

	it = paramsMap.find(key);
	if (it != paramsMap.end())
		return it->second;
	else
		return -1;
}



