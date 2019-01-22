#ifndef FUNCS_H
#define FUNCS_H
#include "attributes.h" 
#include<map>
using namespace std;
string get_type_string(attribute_t* attr);
bool id_is_defined(string id);
bool main_exists();
int string_to_int(std::string str);
type_enum_t get_type_by_name(std::string name);
type_enum_t get_struct_member_type(std::string struct_id, std::string member_id);
type_enum_t get_function_ret_val_by_name(std::string function_name);
bool struct_has_member_with_name(std::string struct_id, std::string member_name);
bool assign_allowed(expression_t* first, expression_t* second);
std::string get_struct_subtype(std::string struct_id);
bool function_parameters_match(std::string function_id, vector<expression_t*>* parameters);
functionAttribute* get_function(std::string function_id);
bool type_is_numeric(type_enum_t type);
/****************************  hw5   ****************************************************************/

// Emits the MIPS instruction equivalent to 
// "emit(‘if’ left_expression relational_operation right_expression ‘goto _’);", and returns the
// emitted offset of this instruction
int emit_conditional_branch_for_relational_operation(
    expression_t left_expression,
    std::string relational_operation,
    expression_t right_expression);


#endif // !FUNCS_H




/*
enum type{
INT,
BYTE,
BOOL
};
enum retType{
VOID,

};
*/
/*
class function{
	
private:
	string fun_name;
	retType fun_type;
	map<int,type> params;	
	bool scoop_is_open;	
public:
	function(string name, retType return_type){
		fun_name=name;
		fun_type=return_type;
		scoop_is_open=true;
	}
	void set_scoop_status(bool status){
		scoop_is_open=status;
	}
	bool get_scoop_status(){
		return scoop_is_open;
	}
	int add_param(type param_type){
		params.insert(std::pair<int,type>(params.size(),param_type));
	}
	int get_param_num(){
		return params.size();
	}
	
	
};
*/














	
	
