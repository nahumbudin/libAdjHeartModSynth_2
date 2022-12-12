/**
*	@file		alsaMidiSystemControl.h
*	@author		Nahum Budin
*	@date		6-Mar-2021
*	@version	1.0
*	
*	@brief		Use system "aconnect" commands to scan and control ALSA midi connections.		
*				
*	History:\n
*	
*		version 1.0		
*/

#ifndef _ALSA_MIDI_SYS_CONTROL
#define _ALSA_MIDI_SYS_CONTROL

#define _CLIENTS_TYPE_DATA_INPUT		0
#define _CLIENTS_TYPE_DATA_OUTPUT		1
#define _CLIENTS_TYPE_DATA_CONNECTIONS	2

#define _LINE_TYPE_CLIENT				0
#define _LINE_TYPE_PORT					1
#define _LINE_TYPE_CONNECTIONS			2


#include <string>

const int max_alsa_data_string_length = 2048;
static const int max_num_of_alsa_clients = 16;
static const int max_num_of_alsa_ports = 16;

typedef struct s_alsa_clients_connection
{
	int connected_client;
	int connected_port;
	int connecting_to_client_num;
	int connecting_to_port_num;
} s_alsa_clients_connection_t;

typedef struct s_alsa_text_line
{
	// Line type: client, port, connection
	int type;
	// Each text line refers to a specific client
	int client;
	// Connections text lines refers to specific port
	int port;
	std::string text_line;
} s_alsa_text_line_t;

typedef struct s_alsa_clients_data
{
	// in/out/connections
	int data_type;
	int num_of_clients;
	// Clients numbers
	int clients_num[max_num_of_alsa_clients];
	// Clients names
	std::string clients_name[max_num_of_alsa_clients];
	// Clients types
	std::string clients_type[max_num_of_alsa_clients];
	// Number of ports of each client
	int clients_num_of_ports[max_num_of_alsa_clients];
	// Ports numbers of each input client
	int clients_ports[max_num_of_alsa_clients][max_num_of_alsa_ports];
	// Ports names of each input client
	std::string clients_ports_names[max_num_of_alsa_clients][max_num_of_alsa_ports];
	// Currently processed client index
	int currently_processed_client_index;
	// Currently processed port index
	int currently_processed_port_index;
	// Currently processed connection index
	int currently_processed_connection_index;
	// Holds the clients each client is connected to (used only for input clients)
	std::vector<s_alsa_clients_connection_t> connected_to_list;
	
	
	// Holds the parsed lines data
	std::vector<s_alsa_text_line_t> clients_data_text_lines_list;	
	// Holds the stdout redirected text
	std::string clients_text_data;
} s_alsa_clients_data_t;

class AlsaMidiSysControl
{
public:
	
	static AlsaMidiSysControl *get_instance();
	
	int refresh_alsa_clients_data();
	
	int get_num_of_input_midi_clients();
	int get_num_of_output_midi_clients();
	
	int get_num_of_input_midi_client_ports(int cln);
	int get_num_of_output_midi_client_ports(int cln);
	
	void get_midi_input_client_name_string(int cln, std::string* name);
	void get_midi_output_client_name_string(int cln, std::string* name);
	
	void get_midi_input_client_port_name_string(int cln, int port, std::string* name);
	void get_midi_output_client_port_name_string(int cln, int port, std::string* name);
	
	int connect_midi_clients(int in_client, int in_port, int out_client, int out_port);
	int disconnect_midi_clients(int in_client, int in_port, int out_client, int out_port);
	int disconnect_all_midi_clients();
	
	int get_num_of_midi_clients_connections();
	int get_midi_client_connection_connected_client_num(int con_num);
	int get_midi_client_connection_connected_port_num(int con_num);
	int get_midi_client_connection_connecting_to_client_num(int con_num);
	int get_midi_client_connection_connecting_to_port_num(int con_num);
	
private:
	AlsaMidiSysControl();
	
	int init_clients_data();
	int get_alsa_clients_data();
	
	std::vector<s_alsa_text_line_t> parse_data_text_lines(std::string *data_text);
	int parse_alsa_clients_data_text_lines();
	int parse_text_line_type(s_alsa_text_line_t *text_line_data);
	int parse_text_lines_types(s_alsa_clients_data_t *clients_data);
	int parse_clients_data_text_lines_types();
	
	
	
	int process_client_text_line(std::string *text_line, s_alsa_clients_data_t *clients_data);
	int process_port_text_line(std::string *text_line, s_alsa_clients_data_t *clients_data);
	int process_connecting_to_text_line(std::string *text_line, s_alsa_clients_data_t *clients_data);
	int process_connected_from_text_line(std::string *text_line, s_alsa_clients_data_t *clients_data);
	bool first_non_space_char_is_digit(std::string *text_line);
	int process_text_line(int line_num, s_alsa_clients_data_t *clients_data);
	int parse_alsa_client_data(s_alsa_clients_data_t *clients_data);
	int parse_alsa_clients_data();
		
	s_alsa_clients_data_t input_clients_data;
	s_alsa_clients_data_t output_clients_data;
	s_alsa_clients_data_t clients_connections_data;
	
	static AlsaMidiSysControl *instance;	
};

#endif

