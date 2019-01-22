#include "funcs.h"
#include "output.hpp"
#include "attributes.h"

// hw5: included bp.hpp
#include "bp.hpp"

#include<iostream>
using namespace output;
using namespace std;

static string type_enum_t_to_string(type_enum_t type)
{
    switch (type)
    {
    case TYPE_ENUM_BOOL:
        return "BOOL";
    case TYPE_ENUM_BYTE:
        return "BYTE";
    case TYPE_ENUM_INT:
        return "INT";
    case TYPE_ENUM_STRING:
        return "STRING";
    case TYPE_ENUM_VOID:
        return "VOID";
    default:
        assert(false);
        return "can't convert type!";
    }
}

string get_type_string(attribute_t* attr) {
    structVariableAttribute* struct_var;
    functionAttribute* function;
    vector<string> arg_types;
    string ret_type;
    switch (attr->type)
    {
    case TYPE_ENUM_BOOL: 
        return "BOOL";
    case TYPE_ENUM_BYTE:
        return "BYTE";
    case TYPE_ENUM_INT:
        return "INT";
    case TYPE_ENUM_STRING:
        return "STRING";
    case TYPE_ENUM_STRUCT_VAR:
        struct_var = dynamic_cast<structVariableAttribute*>(attr);
        return "struct " + struct_var->struct_name;
    case TYPE_ENUM_FUNCTION:
    {
        function = dynamic_cast<functionAttribute*>(attr);
        ret_type = type_enum_t_to_string(function->return_type);
        arg_types = vector<string>(0);
        // Store parameters types in the same order they are defined.
        // Note that function->parameters contain these parameters in a reserved order.
        for (int i = function->parameters.size() - 1; i >= 0; i--)
        {
            arg_types.push_back(get_type_string(function->parameters[i]));
        }
        return makeFunctionType(ret_type, arg_types);
    }
    default:
        assert(false);
        return "NO_TYPE";
    }
}

bool id_is_defined(string id)
{
    for (int i = 0; i < tables.size(); i++)
    {
        for (int j = 0; j < tables[i].size(); j++)
        {
            if (tables[i][j]->name == id) return true;
        }
    }
    return false;
}

bool main_exists()
{
    // This function should only be called before reducing the global scope, and therefore there
    // should be only one symbol table
    assert(tables.size() == 1);
    for (int i = 0; i < tables[0].size(); i++)
    {
        if (tables[0][i]->name != "main") continue;
        if (tables[0][i]->type != TYPE_ENUM_FUNCTION) continue;
        
        // There is a "main" function, verify that it has no parameters and type void
        functionAttribute* main_func = dynamic_cast<functionAttribute*>(tables[0][i]);

        if (main_func->return_type != TYPE_ENUM_VOID) continue;

        if (main_func->parameters.size() == 0) return true;
    }
    return false;
}

int string_to_int(string str)
{
    int ans = 0;
    for (int i = 0; i < str.size(); i++) {
        ans *= 10;
        ans += (str[i] - '0');
    }
    return ans;
}

type_enum_t get_type_by_name(string name)
{
    for (int i = 0; i < tables.size(); i++)
    {
        for (int j = 0; j < tables[i].size(); j++)
        {
            if (tables[i][j]->name == name) return tables[i][j]->type;
        }
    }
    // This function should only be called on existing variables
    assert(false);
    return TYPE_ENUM_STRING;
}

static structDeclerationAttribute* get_struct_decleration(std::string struct_name)
{
    for (int i = 0; i < tables.size(); i++)
    {
        for (int j = 0; j < tables[i].size(); j++)
        {
            if (tables[i][j]->name == struct_name)
            {
                structDeclerationAttribute* struct_decleration =
                    dynamic_cast<structDeclerationAttribute*>(tables[i][j]);
                return struct_decleration;
            }
        }
    }
    assert(false);
    return NULL;
}

type_enum_t get_struct_member_type(string struct_id, string member_id)
{
    for (int i = 0; i < tables.size(); i++)
    {
        for (int j = 0; j < tables[i].size(); j++)
        {
            if (tables[i][j]->name == struct_id)
            {
                assert(tables[i][j]->type == TYPE_ENUM_STRUCT_VAR);
                structVariableAttribute* struct_var = dynamic_cast<structVariableAttribute*>(tables[i][j]);
                structDeclerationAttribute* struct_dec = get_struct_decleration(struct_var->struct_name);
                for (int k = 0; k < struct_dec->members.size(); k++)
                {
                    if (struct_dec->members[k]->name == member_id) return struct_dec->members[k]->type;
                }
            }
        }
    }
    assert(false);
    return TYPE_ENUM_STRING;
}

type_enum_t get_function_ret_val_by_name(std::string function_name)
{
    for (int i = 0; i < tables.size(); i++)
    {
        for (int j = 0; j < tables[i].size(); j++)
        {
            if (tables[i][j]->name == function_name)
            {
                assert(tables[i][j]->type == TYPE_ENUM_FUNCTION);
                functionAttribute* func = dynamic_cast<functionAttribute*>(tables[i][j]);
                return func->return_type;
            }
        }
    }
    assert(false);
    return TYPE_ENUM_VOID;
}

bool struct_has_member_with_name(std::string struct_id, std::string member_name)
{
    for (int i = 0; i < tables.size(); i++)
    {
        for (int j = 0; j < tables[i].size(); j++)
        {
            if (tables[i][j]->name == struct_id)
            {
                assert(tables[i][j]->type == TYPE_ENUM_STRUCT_VAR);
                structVariableAttribute* struct_var = dynamic_cast<structVariableAttribute*>(tables[i][j]);
                structDeclerationAttribute* struct_dec = get_struct_decleration(struct_var->struct_name);
                for (int k = 0; k < struct_dec->members.size(); k++)
                {
                    if (struct_dec->members[k]->name == member_name) return true;
                }
            }
        }
    }
    return false;
}

bool type_is_numeric(type_enum_t type)
{
    return (type == TYPE_ENUM_INT || type == TYPE_ENUM_BYTE);
}

bool assign_allowed(expression_t* first, expression_t* second) {
    if (first->type == TYPE_ENUM_INT) return type_is_numeric(second->type);
    if (first->type == second->type &&
        first->type == TYPE_ENUM_STRUCT_VAR)
    {
        return first->sub_type == second->sub_type;
    }
    return first->type == second->type;
}

std::string get_struct_subtype(std::string struct_id)
{
    for (int i = 0; i < tables.size(); i++)
    {
        for (int j = 0; j < tables[i].size(); j++)
        {
            if (tables[i][j]->name == struct_id)
            {
                structVariableAttribute* struct_var = 
                    dynamic_cast<structVariableAttribute*>(tables[i][j]);
                return struct_var->struct_name;
            }
        }
    }
    assert(false);
    return "no name";
}

functionAttribute* get_function(std::string function_id)
{
    for (int i = 0; i < tables.size(); i++)
    {
        for (int j = 0; j < tables[i].size(); j++)
        {
            if (tables[i][j]->name == function_id)
            {
                assert(tables[i][j]->type == TYPE_ENUM_FUNCTION);
                functionAttribute* function = dynamic_cast<functionAttribute*>(tables[i][j]);
                return function;
            }
        }
    }
    assert(false);
    return NULL;
}

bool function_parameters_match(std::string function_id, vector<expression_t*>* parameters)
{
    functionAttribute* function = get_function(function_id);
    if (parameters->size() != function->parameters.size()) return false;
    for (int i = 0; i < parameters->size(); i++)
    {
        expression_t* param_exp = new expression_t(function->parameters[i]->type);
        if (param_exp->type == TYPE_ENUM_STRUCT_VAR)
        {
            structVariableAttribute* struct_var = dynamic_cast<structVariableAttribute*>(function->parameters[i]);
            param_exp->sub_type = struct_var->struct_name;
        }
        if (!assign_allowed(param_exp, (*parameters)[i])) return false;
    }
    return true;
}

/* hw5 */
int emit_conditional_branch_for_relational_operation(
    expression_t left_expression,
    std::string relational_operation,
    expression_t right_expression)
{
    if (relational_operation == "<")
    {
        return CodeBuffer::instance().emit("blt " + left_expression.belongs_reg + "," + right_expression.belongs_reg + ", ");
    }
    else if (relational_operation == "<=")
    {
        return CodeBuffer::instance().emit("ble " + left_expression.belongs_reg + "," + right_expression.belongs_reg + ", ");
    }
    else if (relational_operation == "==")
    {
        return CodeBuffer::instance().emit("beq " + left_expression.belongs_reg + "," + right_expression.belongs_reg + ", ");
    }
    else if (relational_operation == "!=")
    {
        return CodeBuffer::instance().emit("bne " + left_expression.belongs_reg + "," + right_expression.belongs_reg + ", ");
    }
    else if (relational_operation == ">=")
    {
        return CodeBuffer::instance().emit("bge " + left_expression.belongs_reg + "," + right_expression.belongs_reg + ", ");
    }
    else
    {
        // relational_operation is ">"

        return CodeBuffer::instance().emit("bgt " + left_expression.belongs_reg + "," + right_expression.belongs_reg + ", ");
    }
}