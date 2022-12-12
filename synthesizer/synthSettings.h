/**
*	@file		synthSettings.h
*	@author		Nahum Budin
*	@date		29_Jan-2021
*	@version	1.1 
*					1. Code refactoring and notaion.
*					
*	@version	1.0		5-Oct-2019
*
*	@brief		Synthesizer settings.
*/

/**
*	Here we handle the sysnthesizer settings parameters.
*	There is only one settings object (singlton).
*	Each settings parameter is identified by a unique strink key (name).
*	There are 4 types of parameters values: string, integer, float (double) and boolean.
*	The integer and float parameters also include their minimum and maximum values.
*	Each parameter includes a pointer to a set-value callback function.
*	A voice related parameter may also include a pointer to a set-value[voice_num] callback function
*		and a start index and a stop index parameter for the set value above, to enable auto block setting.
*	The parameters are handeled as map that holds the parameter unique key name (string) and the parameter structure.
*	Referencing a parameter is done by its name (key).
*/

#ifndef _MOD_SYNTH_SETTINGS
#define _MOD_SYNTH_SETTINGS

#include <string>
#include <map>
#include <vector>
#include <iterator>
#include <mutex>
#include <bits/stdc++.h> 

using namespace std;

/* Settings operations results */
#define _SETTINGS_OK					0
#define _SETTINGS_FAILED				-1
#define _SETTINGS_KEY_NOT_FOUND			-2
#define _SETTINGS_KEY_FOUND				-3
#define _SETTINGS_BAD_PARAMETERS		-4
#define _SETTINGS_PARAM_OUT_OF_RANGE	-5
#define _SETTINGS_BAD_PATH				-6
#define _SETTINGS_READ_FILE_ERROR		-7

// Bit fields
#define _SET_NONE						0
#define _SET_VALUE						(1 << 1)
#define _SET_MAX_VAL					(1 << 2)
#define _SET_MIN_VAL					(1 << 3)
#define _SET_TYPE						(1 << 4)
#define _SET_CALLBACK					(1 << 5)
#define _EXEC_CALLBACK					(1 << 6)
#define _SET_BLOCK_START_INDEX			(1 << 7)
#define _SET_BLOCK_STOP_INDEX			(1 << 8)
#define _SET_BLOCK_CALLBACK				(1 << 9)
#define _EXEC_BLOCK_CALLBACK			(1 << 10)

#define	_SAVE_FLUID_SETTINGS			(1 << 1)
#define	_SAVE_ADJ_SYNTH_PATCH			(1 << 2)
#define	_SAVE_ADJ_SYNTH_SETTINGS		(1 << 3)
#define	_SAVE_MOD_SYNTH_SETTINGS		(1 << 4)
#define	_SAVE_MOD_SYNTH_PRESET			(1 << 5)
#define _SAVE_MOD_SYNTH_KBD_MAP			(1 << 6)

#define	_READ_FLUID_SETTINGS			_SAVE_FLUID_SETTINGS
#define	_READ_ADJ_SYNTH_PATCH			_SAVE_ADJ_SYNTH_PATCH
#define	_READ_ADJ_SYNTH_SETTINGS		_SAVE_ADJ_SYNTH_SETTINGS
#define	_READ_MOD_SYNTH_SETTINGS		_SAVE_MOD_SYNTH_SETTINGS
#define	_READ_MOD_SYNTH_PRESET			_SAVE_MOD_SYNTH_PRESET
#define _READ_MOD_SYNTH_KBD_MAP			_SAVE_MOD_SYNTH_KBD_MAP

#define _CHECK_MASK(mask, bit) (mask & bit)

#define _PARAM_TYPE_NONE				0
/* Indicates that the parameter is a global settings parameter */
#define _PARAM_TYPE_MOD_SYNTH_SETUP		1
/* Indicates that the parameter is a FluidSynth settings parameter */
#define _PARAM_TYPE_FLUID_SYNTH_SETUP	2
/* Indicates that the parameter is an adjSynth patch settings parameter */
#define _PARAM_TYPE_ADJ_SYNTH_PATCH		3
/* Indicates that the parameter is an adjSynth general settings parameter */
#define _PARAM_TYPE_ADJ_SYNTH_SETUP		4
/* Indicates that the parameter is a modSynth preset settings parameter */
#define _PARAM_TYPE_MOD_SYNTH_PRESET	5


/* Returns settings operations results */
typedef int settings_res_t;

typedef struct _mod_synth_settings_str_param_t mod_synth_settings_str_param_t;
typedef struct _mod_synth_settings_int_param_t mod_synth_settings_int_param_t;
typedef struct _mod_synth_settings_float_param_t mod_synth_settings_float_param_t;
typedef struct _mod_synth_settings_bool_param_t mod_synth_settings_bool_param_t;

/* Callback function - set string parameter action */
typedef settings_res_t(*string_param_update_callback_t)(string value, int prog);
/* Callback function - set integer parameter action */
typedef settings_res_t(*int_param_update_callback_t)(int value, int prog);
/* Callback function - set float (double) parameter action */
typedef settings_res_t(*float_param_update_callback_t)(double value, int prog);
/* Callback function - set boolean parameter action */
typedef settings_res_t(*bool_param_update_callback_t)(bool value, int prog);

/* Block update callbacks - includes an index to a specific bloc-parametr */

/* Callback function - set string parameters-block action */
typedef settings_res_t(*string_block_param_update_callback_t)(string value, int index, int prog);
/* Callback function - set integer parameters-block action */
typedef settings_res_t(*int_block_param_update_callback_t)(int value, int index, int prog);
/* Callback function - set float (double) parameters-block action */
typedef settings_res_t(*float_block_param_update_callback_t)(double value, int index, int prog);
/* Callback function - set boolean parameters-block action */
typedef settings_res_t(*bool_block_param_update_callback_t)(bool value, int inde, int prog);

/* A structure that holds a string value parameter */
struct _mod_synth_settings_str_param_t
{
	std::string key;
	std::string value;
	int type;
	string_param_update_callback_t setup_callback;
	bool callback_set;
	string_block_param_update_callback_t block_setup_callback;
	int block_start_index;
	int block_stop_index;
	bool block_callback_set;
};

/* A structure that holds an integer value parameter */
struct _mod_synth_settings_int_param_t
{
	std::string key;
	int value;
	int max_val;
	int min_val;
	bool limits_set;
	int type;
	int_param_update_callback_t setup_callback;
	bool callback_set;
	int_block_param_update_callback_t block_setup_callback;
	int block_start_index;
	int block_stop_index;
	bool block_callback_set;
};

/* A structure that holds a float (double) value parameter */
struct _mod_synth_settings_float_param_t
{
	std::string key;
	double value;
	double max_val;
	double min_val;
	bool limits_set;
	int type;
	float_param_update_callback_t setup_callback;
	bool callback_set;
	float_block_param_update_callback_t block_setup_callback;
	int block_start_index;
	int block_stop_index;
	bool block_callback_set;
};

/* A structure that holds a boolean value parameter */
struct _mod_synth_settings_bool_param_t
{
	std::string key;
	bool value;
	int type;
	bool_param_update_callback_t setup_callback;
	bool callback_set;
	bool_block_param_update_callback_t block_setup_callback;
	int block_start_index;
	int block_stop_index;
	bool block_callback_set;
};

/* Holds a set of all settings parameters */
typedef struct _params
{
	/* Settings type #_PARAM_TYPE_SETUP, #_PARAM_TYPE_PATCH */
	int settings_type;
	/* Settings version */
	uint32_t version;
	/* Settings name*/
	string name;
	/* String parameters map */
	std::map<std::string, mod_synth_settings_str_param_t> string_parameters_map;
	/* Integer parameters map */
	std::map<std::string, mod_synth_settings_int_param_t> int_parameters_map;
	/* Float (double) parameters map */
	std::map<std::string, mod_synth_settings_float_param_t> float_parameters_map;
	/* Boolean parameters map */
	std::map<std::string, mod_synth_settings_bool_param_t> bool_parameters_map;
} _setting_params_t;



typedef int(*func_ptr_int_settings_parms_ptr_int_t)(_setting_params_t*, int);


class ModSynthSettings
{
public:
	
	ModSynthSettings(_setting_params_t *settings_params);
	~ModSynthSettings();

	_setting_params_t *get_active_settings_parameters();
	
	static void settings_params_deep_copy(_setting_params_t *destination_params, _setting_params_t *source_params);

	uint32_t get_settings_version();

	settings_res_t get_string_param(_setting_params_t *settings = NULL,
		string name = "",
		mod_synth_settings_str_param_t *param = NULL);

	settings_res_t get_int_param(_setting_params_t *settings = NULL,
		string name = "",
		mod_synth_settings_int_param_t *param = NULL);

	settings_res_t get_float_param(_setting_params_t *settings = NULL,
		string name = "",
		mod_synth_settings_float_param_t *param = NULL);

	settings_res_t get_bool_param(_setting_params_t *settings = NULL,
		string name = "",
		mod_synth_settings_bool_param_t *param = NULL);


	settings_res_t set_string_param(_setting_params_t *settings = NULL,
		string name = "",
		string value = "",
		int type = _PARAM_TYPE_NONE,
		string_param_update_callback_t callback = NULL,
		int block_start = -1,
		int block_stop = -1,
		string_block_param_update_callback_t block_callback = NULL,
		uint16_t set_mask = 0,
		int program = 0);

	settings_res_t set_int_param(_setting_params_t *settings = NULL,
		string name = "",
		int value = 0,
		int max_val = INT_MAX,
		int min_val = INT_MIN,
		int type = _PARAM_TYPE_NONE,
		int_param_update_callback_t callback = NULL,
		int block_start = -1,
		int block_stop = -1,
		int_block_param_update_callback_t block_callback = NULL,
		uint16_t set_mask = 0,
		int program = 0);

	settings_res_t set_float_param(_setting_params_t *settings = NULL,
		string name = "",
		double value = 0,
		double max_val = DBL_MAX,
		double min_val = DBL_MIN,
		int type = _PARAM_TYPE_NONE,
		float_param_update_callback_t callback = NULL,
		int block_start = -1,
		int block_stop = -1,
		float_block_param_update_callback_t block_callback = NULL,
		uint16_t set_mask = 0,
		int program = 0);

	settings_res_t set_bool_param(_setting_params_t *settings = NULL,
		string name = "",
		bool value = false,
		int type = _PARAM_TYPE_NONE,
		bool_param_update_callback_t callback = NULL,
		int block_start = -1,
		int block_stop = -1,
		bool_block_param_update_callback_t block_callback = NULL,
		uint16_t set_mask = 0,
		int program = 0);
	
	
	settings_res_t set_string_param_value(_setting_params_t *settings = NULL, 
		string name = "", 
		string value = "", 
		uint16_t set_mask = 0,
		int program = 0);

	settings_res_t set_int_param_value(_setting_params_t *settings = NULL, 
		string name = "", 
		int value = 0, 
		uint16_t set_mask = 0,
		int program = 0);

	settings_res_t set_float_param_value(_setting_params_t *settings = NULL, 
		string name = "", 
		double value = 0, 
		uint16_t set_mask = 0,
		int program = 0);

	settings_res_t set_bool_param_value(_setting_params_t *settings = NULL, 
		string name = "", 
		bool value = false, 
		uint16_t set_mask = 0,
		int program = 0);

	void set_active_sketch(int ask);
	int get_active_sketch();

	settings_res_t write_settings_file(_setting_params_t *params = NULL, 
		uint32_t version = 0,
		string name = "",
		string path = "",
		uint16_t save_mask = 0);

	settings_res_t read_settings_file(_setting_params_t *params = NULL,
		string path = "",
		uint16_t read_mask = 0,
		int channel = 0);

	int look_for_key(std::string key, std::map<std::string, int> params_map);	

	/* Full path for settings XML files */
	static char settings_dir[128];
	/* Full path for sketch_1 XML files */
	static char xml_sketch_1_path[128];
	/* Full path for sketch_2 XML files */
	static char xml_sketch_2_path[128];
	/* Full path for sketch_3 XML files */
	static char xml_sketch_3_path[128];

protected:
	// Holds read xmxl file params
	std::map<std::string, int> int_params_map, bool_params_map, float_params_map, string_params_map;	

private:
	/* Active  parameters */
	_setting_params_t *active_settings_params;
	
	/* Mutex to handle settings opperations */
	static std::mutex settings_manage_mutex;

	ModSynthSettings *mod_synth_settings_instance;
	/* Indicates which sketch is active */
	//	static int activeSketch;

		/* Settings version */
		static uint32_t settings_version;
};


#endif
