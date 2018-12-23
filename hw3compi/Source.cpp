#include "funcs.h"
#include "output.hpp"
#include "attributes.h"
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