/**
* @file		synthSettingsFiles.cpp
*	@author		Nahum Budin
*	@date		7_feb-2021
*	@version	1.1
*					1. Code refactoring and notaion.
*	
*	@brief		Synthesizer settings files handling.
*
*	History 
*				ver 1.0		5-Oct-2019		1st version
*/

#include <stdint.h>

#include "synthSettings.h"
#include "../utils/utils.h"

#include "../utils/XMLfiles.h"

/**
 * Saves settings parameter as an XML file.
 *
 * @param params settings parameters structure
 * @version parameters version
 * @param name parameters set name (for example, patch name)
 * @param path full path of the file
 * @param save_mask bit map to set saved params type #_SAVE_FLUID_SETTINGS, #_SAVE_ADJ_SYNTH_PATCH,
 *													 #_SAVE_ADJ_SYNTH_SETTINGS, #_SAVE_MOD_SYNTH_SETTINGS
 *													 #_SAVE_MOD_SYNTH_PRESET
 * @return #_SETTINGS_OK if saveed sucessfully, #_SETTINGS_FAILED otherwise
 */
settings_res_t ModSynthSettings::write_settings_file(_setting_params_t *params, 
												     uint32_t version, string name,
												     string path, uint16_t save_mask)
{
	settings_res_t res = _SETTINGS_OK;

	// Verify mandatory params
	return_val_if_true(params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(version == 0, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(name == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(save_mask == 0, _SETTINGS_BAD_PARAMETERS);

	XML_files *xml_files = new XML_files();
	vector<string> xml_file_string;
	std::string file_name, file_name_no_ext;
	std::string type_str;
	
	// Set type string
	if ((_CHECK_MASK(save_mask, _SAVE_FLUID_SETTINGS)) && 
		(_CHECK_MASK(save_mask, _SAVE_ADJ_SYNTH_PATCH)) && 
		(_CHECK_MASK(save_mask, _SAVE_ADJ_SYNTH_SETTINGS)) &&
		(_CHECK_MASK(save_mask, _SAVE_MOD_SYNTH_SETTINGS)) &&
		(_CHECK_MASK(save_mask, _SAVE_MOD_SYNTH_PRESET)))
	{
		type_str = "all_params";
	}
	else if _CHECK_MASK(save_mask, _SAVE_FLUID_SETTINGS)
	{
		type_str = "fluid_synth_settings";
	}
	else if _CHECK_MASK(save_mask, _SAVE_ADJ_SYNTH_PATCH)
	{
		type_str = "adj_synth_patch";
	}
	else if _CHECK_MASK(save_mask, _SAVE_ADJ_SYNTH_SETTINGS)
	{
		type_str = "adj_synth_settings";
	}
	else if _CHECK_MASK(save_mask, _SAVE_MOD_SYNTH_SETTINGS)
	{
		type_str = "mod_synth_settings";
	}
	else if _CHECK_MASK(save_mask, _SAVE_MOD_SYNTH_PRESET)
	{
		type_str = "mod_synth_preset";
	}
	else
	{
		type_str = "other";
	}
	
	// Build file headers
	file_name = xml_files->remove_file_extention(xml_files->get_xml_file_name(path));
	return_val_if_true(file_name == "", _SETTINGS_BAD_PATH);

	file_name_no_ext = xml_files->remove_file_extention(file_name);
	return_val_if_true(file_name_no_ext == "", _SETTINGS_BAD_PATH);

	xml_file_string.push_back(xml_files->comment("AdjHeart Modular Synthesizer Parameters File"));
	xml_file_string.push_back(xml_files->start_tag(type_str, 0, true));

	xml_file_string.push_back(xml_files->string_val_element("string_param", "name", file_name, 4));
	xml_file_string.push_back(xml_files->int_val_element("int_param", "version", version, 4));
	
	// Unmark the define below to enable saving all settings parametrs.
	// Leave marked if only settings value parameter should be saved (default)
//#define _SAVE_ALL_PARAMTERS_

	// Go over all string parameters
	for (std::map<std::string, mod_synth_settings_str_param_t>::iterator param = params->string_parameters_map.begin();
		param != params->string_parameters_map.end(); ++param)
	{
		// Verify param context
		if ((_CHECK_MASK(save_mask, _SAVE_FLUID_SETTINGS) && (param->second.type == _PARAM_TYPE_FLUID_SYNTH_SETUP)) ||
			(_CHECK_MASK(save_mask, _SAVE_ADJ_SYNTH_PATCH) && (param->second.type == _PARAM_TYPE_ADJ_SYNTH_PATCH)) ||
			(_CHECK_MASK(save_mask, _SAVE_ADJ_SYNTH_SETTINGS) && (param->second.type == _PARAM_TYPE_ADJ_SYNTH_SETUP)) ||
			(_CHECK_MASK(save_mask, _SAVE_MOD_SYNTH_SETTINGS) && (param->second.type == _PARAM_TYPE_MOD_SYNTH_SETUP)) ||
			(_CHECK_MASK(save_mask, _SAVE_MOD_SYNTH_PRESET) && (param->second.type == _PARAM_TYPE_MOD_SYNTH_PRESET)))
		{
			xml_file_string.push_back(xml_files->string_val_element(
									"string_param", 
									param->first, 
									param->second.value, 4));
#ifdef _SAVE_ALL_PARAMTERS_
			if (param->second.callback_set)
			{
				xml_file_string.push_back(xml_files->bool_val_element("bool_param", param->first + ".callback_set",
										param->second.callback_set, 4));
			}
			
			if (param->second.block_callback_set)
			{
				xml_file_string.push_back(xml_files->bool_val_element("bool_param",
					param->first + ".block_callback_set",
					param->second.block_callback_set,
					4));
				
				xml_file_string.push_back(xml_files->int_val_element("int_param",
					param->first + ".block_start",
					param->second.block_start_index,
					4));

				xml_file_string.push_back(xml_files->int_val_element("int_param",
					param->first + ".block_stop",
					param->second.block_stop_index,
					4));
			}
#endif
		}
	}

	// Go over all integer parameters
	for (std::map<std::string, mod_synth_settings_int_param_t>::iterator param = params->int_parameters_map.begin();
		param != params->int_parameters_map.end(); ++param)
	{
		if ((_CHECK_MASK(save_mask, _SAVE_FLUID_SETTINGS) && (param->second.type == _PARAM_TYPE_FLUID_SYNTH_SETUP)) ||
			(_CHECK_MASK(save_mask, _SAVE_ADJ_SYNTH_PATCH) && (param->second.type == _PARAM_TYPE_ADJ_SYNTH_PATCH)) ||
			(_CHECK_MASK(save_mask, _SAVE_ADJ_SYNTH_SETTINGS) && (param->second.type == _PARAM_TYPE_ADJ_SYNTH_SETUP)) ||
			(_CHECK_MASK(save_mask, _SAVE_MOD_SYNTH_SETTINGS) && (param->second.type == _PARAM_TYPE_MOD_SYNTH_SETUP)) ||
			(_CHECK_MASK(save_mask, _SAVE_MOD_SYNTH_PRESET) && (param->second.type == _PARAM_TYPE_MOD_SYNTH_PRESET)))
		{
			xml_file_string.push_back(xml_files->int_val_element("int_param",
				param->first,
									param->second.value, 4));
#ifdef _SAVE_ALL_PARAMTERS_
			if (param->second.limits_set)
			{
				xml_file_string.push_back(xml_files->int_val_element("int_param",
					param->first + ".max",
										param->second.max_val, 4));

				xml_file_string.push_back(xml_files->int_val_element("int_param",
					param->first + ".min",
										param->second.min_val, 4));

				xml_file_string.push_back(xml_files->bool_val_element("bool_param",
					param->first + ".limits_set",
					param->second.limits_set, 4));
			}

			if (param->second.callback_set)
			{
				xml_file_string.push_back(xml_files->bool_val_element("bool_param",
					param->first + ".callback_set",
					param->second.callback_set, 4));
			}
			
			if (param->second.block_callback_set)
			{
				xml_file_string.push_back(xml_files->bool_val_element("bool_param",
					param->first + ".block_callback_set",
					param->second.block_callback_set,
					4));
				
				xml_file_string.push_back(xml_files->int_val_element("int_param",
					param->first + ".block_start",
					param->second.block_start_index,
					4));

				xml_file_string.push_back(xml_files->int_val_element("int_param",
					param->first + ".block_stop",
					param->second.block_stop_index,
					4));
			}
#endif
		}
	}

	// Go over all float parameters
	for (std::map<std::string, mod_synth_settings_float_param_t>::iterator param = params->float_parameters_map.begin();
		param != params->float_parameters_map.end(); ++param)
	{
		if ((_CHECK_MASK(save_mask, _SAVE_FLUID_SETTINGS) && (param->second.type == _PARAM_TYPE_FLUID_SYNTH_SETUP)) ||
			(_CHECK_MASK(save_mask, _SAVE_ADJ_SYNTH_PATCH) && (param->second.type == _PARAM_TYPE_ADJ_SYNTH_PATCH)) ||
			(_CHECK_MASK(save_mask, _SAVE_ADJ_SYNTH_SETTINGS) && (param->second.type == _PARAM_TYPE_ADJ_SYNTH_SETUP)) ||
			(_CHECK_MASK(save_mask, _SAVE_MOD_SYNTH_SETTINGS) && (param->second.type == _PARAM_TYPE_MOD_SYNTH_SETUP)) ||
			(_CHECK_MASK(save_mask, _SAVE_MOD_SYNTH_PRESET) && (param->second.type == _PARAM_TYPE_MOD_SYNTH_PRESET)))
		{
			xml_file_string.push_back(xml_files->float_val_element("float_param",
				param->first,
									param->second.value, 4));
#ifdef _SAVE_ALL_PARAMTERS_
			if (param->second.limits_set)
			{
				xml_file_string.push_back(xml_files->float_val_element("float_param",
					param->first + ".max",
									param->second.max_val, 4));

				xml_file_string.push_back(xml_files->float_val_element("float_param",
					param->first + ".min",
									param->second.min_val, 4));
				xml_file_string.push_back(xml_files->float_val_element("bool_param",
					param->first + ".limits_set",
									param->second.limits_set, 4));
			}

			if (param->second.callback_set)
			{
				xml_file_string.push_back(xml_files->bool_val_element("bool_param",
					param->first + ".callback_set",
										param->second.callback_set, 4));
			}
			
			if (param->second.block_callback_set)
			{
				xml_file_string.push_back(xml_files->bool_val_element("bool_param",
					param->first + ".block_callback_set",
					param->second.block_callback_set,
					4));
				
				xml_file_string.push_back(xml_files->int_val_element("int_param",
					param->first + ".block_start",
					param->second.block_start_index,
					4));

				xml_file_string.push_back(xml_files->int_val_element("int_param",
					param->first + ".block_stop",
					param->second.block_stop_index,
					4));
			}
#endif
		}
	}

	// Go over all boolean parameters
	for (std::map<std::string, mod_synth_settings_bool_param_t>::iterator param = params->bool_parameters_map.begin();
		param != params->bool_parameters_map.end(); ++param)
	{
		if ((_CHECK_MASK(save_mask, _SAVE_FLUID_SETTINGS) && (param->second.type == _PARAM_TYPE_FLUID_SYNTH_SETUP)) ||
			(_CHECK_MASK(save_mask, _SAVE_ADJ_SYNTH_PATCH) && (param->second.type == _PARAM_TYPE_ADJ_SYNTH_PATCH)) ||
			(_CHECK_MASK(save_mask, _SAVE_ADJ_SYNTH_SETTINGS) && (param->second.type == _PARAM_TYPE_ADJ_SYNTH_SETUP)) ||
			(_CHECK_MASK(save_mask, _SAVE_MOD_SYNTH_SETTINGS) && (param->second.type == _PARAM_TYPE_MOD_SYNTH_SETUP)) ||
			(_CHECK_MASK(save_mask, _SAVE_MOD_SYNTH_PRESET) && (param->second.type == _PARAM_TYPE_MOD_SYNTH_PRESET)))
		{
			xml_file_string.push_back(xml_files->bool_val_element("bool_param",
				param->first,
									param->second.value, 4));
#ifdef _SAVE_ALL_PARAMTERS_
			if (param->second.callback_set)
			{
				xml_file_string.push_back(xml_files->bool_val_element("bool_param",
					param->first + ".callback_set",
										param->second.callback_set, 4));
			}
			
			if (param->second.block_callback_set)
			{
				xml_file_string.push_back(xml_files->bool_val_element("bool_param",
					param->first + ".block_callback_set",
					param->second.block_callback_set,
					4));
				
				xml_file_string.push_back(xml_files->int_val_element("int_param",
					param->first + ".block_start",
					param->second.block_start_index,
					4));

				xml_file_string.push_back(xml_files->int_val_element("int_param",
					param->first + ".block_stop",
					param->second.block_stop_index,
					4));
			}
#endif
		}
	}
	
	xml_file_string.push_back(xml_files->end_tag(type_str, 0));

	res = xml_files->write_xml_file(xml_files->remove_file_extention(path).c_str(), &xml_file_string);

	return res;
}

/**
 * Reads settings parameter from an XML file.
 *
 * @param params settings parameters structure
 * @param path full path of the file
 * @param read_mask bit map to select read params type #_READ_FLUID_SETTINGS, #_READ_ADJ_SYNTH_PATCH,
 *													   #_READ_ADJ_SYNTH_SETTINGS, #_READ_MOD_SYNTH_SETTINGS
 *													   #_READ_MOD_SYNTH_PRESET
 * @param channel	set param of midi channel
 * @return #_SETTINGS_OK if read sucessfully, #_SETTINGS_FAILED otherwise
 */
settings_res_t ModSynthSettings::read_settings_file(_setting_params_t *params,
												  string path, uint16_t read_mask,
												  int channel)
{
	XML_files *xml_files = new XML_files();
	std::string read_file_data;
	settings_res_t res, res_1;
	vector<std::string> int_params, bool_params, float_params, string_params;
	std::string param_name;
	std::string string_param_value;
	int int_param_value;
	float float_param_value;
	bool bool_param_value;
	int id, ivalue, count;
	mod_synth_settings_str_param_t str_param;
	
	// Verify mandatory params
	return_val_if_true(params == NULL && active_settings_params == NULL, _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(path == "", _SETTINGS_BAD_PARAMETERS);
	return_val_if_true(read_mask == 0, _SETTINGS_BAD_PARAMETERS);
	
	res = xml_files->read_xml_file(xml_files->remove_file_extention(path).c_str(), &read_file_data);
	if (res != _SETTINGS_OK)
	{
		return _SETTINGS_READ_FILE_ERROR;
	}
	
	if ((xml_files->element_exist(read_file_data, "fluid_synth_settings") && _CHECK_MASK(read_mask, _READ_FLUID_SETTINGS)) ||
		(xml_files->element_exist(read_file_data, "adj_synth_patch") && _CHECK_MASK(read_mask, _READ_ADJ_SYNTH_PATCH)) ||
		(xml_files->element_exist(read_file_data, "adj_synth_settings") && _CHECK_MASK(read_mask, _READ_ADJ_SYNTH_SETTINGS)) ||
		(xml_files->element_exist(read_file_data, "mod_synth_settings") && _CHECK_MASK(read_mask, _READ_MOD_SYNTH_SETTINGS)) ||
		(xml_files->element_exist(read_file_data, "mod_synth_preset") && _CHECK_MASK(read_mask, _READ_MOD_SYNTH_PRESET)))
	{	
		int_params = xml_files->get_int_params(read_file_data);
		bool_params = xml_files->get_bool_params(read_file_data);
		float_params = xml_files->get_float_params(read_file_data);
		string_params = xml_files->get_string_params(read_file_data);
	}
	else
	{
		return _SETTINGS_READ_FILE_ERROR;
	}
	
	// Parse string parameters
	count = 0;
	for (auto &element : string_params)
	{
		param_name = xml_files->get_element_name(element);
		string_param_value = xml_files->get_string_element_value(element);
		if (!param_name.empty())
		{
			set_string_param_value(params,
				param_name,
				string_param_value,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				channel);
		} 
	}
	
	
	res_1 = get_string_param(params, "name", &str_param);
	if (res_1 == _SETTINGS_KEY_FOUND)
	{
		params->name = str_param.value;
	}

	// Parse integer parameters
	count = 0;
	for (auto &element : int_params)
	{
		param_name = xml_files->get_element_name(element);
		int_param_value = xml_files->get_int_element_value(element);
		if (!param_name.empty())
		{
			set_int_param_value(params,
				param_name,
				int_param_value,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				channel);
		}
	}
	
	// Parse float parameters
	count = 0;
	for (auto &element : float_params)
	{
		param_name = xml_files->get_element_name(element);
		float_param_value = xml_files->get_float_element_value(element);
		if (!param_name.empty())
		{
			set_float_param_value(params,
				param_name,
				float_param_value,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				channel);
		}
	}
	
	// Parse boolean parameters
	count = 0;
	for (auto &element : bool_params)
	{
		param_name = xml_files->get_element_name(element);
		bool_param_value = xml_files->get_bool_element_value(element);
		if (!param_name.empty())
		{
			set_bool_param_value(params,
				param_name,
				bool_param_value,
				_EXEC_CALLBACK | _EXEC_BLOCK_CALLBACK,
				channel);
		}
	}
	
	return res;
}
