#ifndef FUNCS_H
#define FUNCS_H
#include "attributes.h"
string get_type_string(attribute_t* attr);
bool id_is_defined(string id);
bool main_exists();
int string_to_int(std::string str);
type_enum_t get_type_by_name(std::string name);
type_enum_t get_struct_member_type(std::string struct_id, std::string member_id);
type_enum_t get_function_ret_val_by_name(std::string function_name);
#endif // !FUNCS_H

