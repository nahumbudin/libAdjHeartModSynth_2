/**
* @file		XMLfiles.h
*	@author		Nahum Budin
*	@date		13-Oct-2019
*	@version	1.0
*
*	@brief	XML files handling (for settings and configuration files).
*
*  Based on Music Open Lab Library.
*	Copyright AdjHeart Nahum Budin Octobe 1, 2018
*
*	History: see h file\n
*
*	version 1.0		17-Oct-2019:
*		First version
*/

#include <string>
#include <stdio.h>
#include "XMLfiles.h"

XML_files::XML_files()
{

}
/**
 *	Returns leading-spaces x indent<tag> and optionally new line
 */
std::string XML_files::start_tag(std::string tag, int indent, bool newline)
{
	std::string spaces;
	
	for (int i = 0; i < indent; i++)
	{
		spaces += " ";
	}
	
	if (newline)
	{
		return spaces + "<" + tag + ">\n";
	}
	else
	{
		return spaces + "<" + tag + ">";
	}
}

/**
 *	Returns leading-spaces x indent</tag> and new line
 */
string XML_files::end_tag(std::string tag, int indent)
{
	std::string spaces;
	
	for (int i = 0; i < indent; i++)
	{
		spaces += " ";
	}
	
	return spaces + "</" + tag + ">\n";
}

/**
 *	Returns leading-spaces x indent<tag>name=int val</tag> and new line
 */
std::string XML_files::int_val_element(std::string tag, std::string name, int val, int indent)
{
	return start_tag(tag, indent) + int_val_attribute(name, val) + end_tag(tag);
}

/**
 *	Returns leading-spaces x indent<tag>name=bool val</tag> and new line
 */
std::string XML_files::bool_val_element(std::string tag, string name, bool val, int indent)
{
	return start_tag(tag, indent) + bool_val_attribute(name, val) + end_tag(tag);
}

/**
 *	Returns leading-spaces x indent<tag>name=float val</tag> and new line
 */
std::string XML_files::float_val_element(std::string tag, std::string name, float val, int indent)
{
	return start_tag(tag, indent) + float_val_attribute(name, val) + end_tag(tag);
}

/**
 *	Returns leading-spaces x indent<tag>name=string val</tag> and new line
 */
std::string XML_files::string_val_element(std::string tag, std::string name, std::string val, int indent)
{
	return start_tag(tag, indent) + string_val_attribute(name, val) + end_tag(tag);
}

/**
 *	Returns name=int val
 */
std::string XML_files::int_val_attribute(std::string name, int val)
{
	return name + "=" + to_string(val);	
}

/**
 *	Returns name=bool val
 */
std::string XML_files::bool_val_attribute(std::string name, bool val)
{
	if (val)
	{
		return name + "=" + "1";
	}
	else
	{
		return name + "=" + "0";
	}
}

/**
 *	Returns name=float val
 */
std::string XML_files::float_val_attribute(std::string name, float val)
{
	return name + "=" + to_string(val);	
}

/**
 *	Returns name=string val
 */
std::string XML_files::string_val_attribute(std::string name, std::string val)
{
	return name + "=" + val;	
}

/**
 *	Returns <!--comment-->
 */
std::string XML_files::comment(std::string comment)
{
	return "<!--" + comment + "-->\n";
}

/**
 *	Scans an XML file text loking for a tag and reurns true if found
 */
bool XML_files::element_exist(const std::string &xml_text, std::string tag)
{
	int pos = 0, start;
	
	while (true)
	{
		start = xml_text.find("<" + tag, pos); 
		if (start == string::npos)
		{
			return false;
		}
		
		start = xml_text.find(">", start);
		start++;

		pos = xml_text.find("</" + tag, start); 
		if (pos == string::npos) 
		{
			return false;
		}
		
		return true;	
	}
}

/**
 *	Returns a vector of std::string elements of a given tag found in xmltext
 */
vector<string> XML_files::get_elements(const std::string &xmltext, std::string tag)
{
	vector<std::string> elements;
	int pos = 0, start;
	
	while (true)
	{
		start = xmltext.find("<" + tag, pos);
		if (start == string::npos)
		{
			return elements;
		}

		start = xmltext.find(">", start);
		start++;

		pos = xmltext.find("</" + tag, start);
		if (pos == string::npos)
		{
			return elements;
		}

		elements.push_back(xmltext.substr(start, pos - start));	
	}
}

/**
 *	Returns a vector of std::string elements of all "int_param" tag
 */
vector<std::string> XML_files::get_int_params(const std::string &xml_text)
{
	return get_elements(xml_text, "int_param");
}

/**
 *	Returns a vector of std::string elements of all "bool_param" tag
 */
vector<std::string> XML_files::get_bool_params(const std::string &xml_text)
{
	return get_elements(xml_text, "bool_param");
}

/**
 *	Returns a vector of std::string elements of all "float_param" tag
 */
vector<std::string> XML_files::get_float_params(const std::string &xml_text)
{
	return get_elements(xml_text, "float_param");
}

/**
 *	Returns a vector of std::string elements of all "string_param" tag
 */
vector<std::string> XML_files::get_string_params(const std::string &xml_text)
{
	return get_elements(xml_text, "string_param");
}

/**
 *	Returns an std::string containing an element name value
 */
std::string XML_files::get_element_name(std::string element)
{
	int pos;
	
	pos = element.find("=", 0); 
	if (pos == string::npos)
	{
		return "\0";
	}
	else
	{
		return element.substr(0, pos);
	}
}

/**
 *	Returns an std::string containing an element value
 */
std::string XML_files::get_element_value(std::string element)
{
	int pos1;
	
	pos1 = element.find("=", 0);
	if (pos1 == string::npos)
	{
		return "\0";
	}
	else 
	{
		return element.substr(pos1 + 1, element.size());
	}
}

/**
 *	Returns an int value contained in an int-element value
 */
int XML_files::get_int_element_value(std::string element)
{
	return stoi(get_element_value(element));
}

/**
 *	Returns a bool value contained in a bool-element value
 */
bool XML_files::get_bool_element_value(std::string element)
{
	int val = stoi(get_element_value(element));
	
	if (val == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 *	Returns a float value contained in a float-element value
 */
float XML_files::get_float_element_value(std::string element)
{
	return stof(get_element_value(element));
}

/**
 *	Returns an std::string value contained in a string-element value
 */
std::string XML_files::get_string_element_value(std::string element)
{
	return get_element_value(element);
}


/**
 *	Read an XML file into an std::string filedata (full path file without .xml)
 */
int XML_files::read_xml_file(const char *full_path, std::string *file_data)
{
	char file_name[256];
	char c;

	if ((full_path == NULL) || (file_data == NULL))
	{
		return -1;
	}

	sprintf(file_name, "%s.xml", full_path);
	
	ifstream xml_file;
	xml_file.open(file_name, std::ifstream::in);
	int err = errno;
	if (!xml_file.is_open())
	{
		fprintf(stderr, "Can't open %s XML patch file for read!\n", full_path);
		return -2;
	}

	while (xml_file.get(c))
	{
		*file_data += c;
	}
	
	xml_file.close();

	return 0;
}
/**
* write a vector<string> XML elements text filedata into an XML file (full path file without .xml)
*/
int XML_files::write_xml_file(const char *full_path, vector<string> *file_data)
{
	char file_name[256];
	int i;
	
	if ((full_path == NULL) || (file_data == NULL))
	{
		return -1;
	}
	
	sprintf(file_name, "%s.xml", full_path);

	ofstream xml_file;
	xml_file.open(file_name, std::ofstream::out);
	int err = errno;
	if (!xml_file.is_open())
	{
		fprintf(stderr, "Can't open %s XML patch file for write!\n", full_path);
		return -2;
	}

	xml_file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	for (i = 0; i < file_data->size(); i++)
	{
		xml_file << file_data->at(i);
	}

	xml_file.close();

	return 0;
}

/**
* Extract file name from full path (file.ext)
*/
std::string XML_files::get_xml_file_name(std::string full_path)
{
	std::string name;
	int pos1, pos2;
	
	if (full_path == "")
	{
		name = "";
	}
	else
	{
		pos2 = full_path.size();
		pos1 = full_path.find_last_of("/", pos2);
		if ((pos2 - pos1) >= 1)
		{
			name = full_path.substr(pos1 + 1, pos2);
		}
		else
		{
			name = "";
		}		
	}	
	
	return name;
}

/**
* Extract file name without the extension or multiple extensions (no more ".")
*/
std::string XML_files::remove_file_extention(std::string file_name)
{
	std::string name = file_name;
	int pos1 = name.size();

	if (file_name == "")
	{
		return "";
	}
	else
	{
		// Remove all .ext
		while (pos1 > 0 )
		{
			pos1 = name.find_last_of(".", name.size());
			if (pos1 > 0)
			{
				name = name.substr(0, pos1);
			}
		}
	}

	return name;
}
 
