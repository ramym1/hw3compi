#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include<string>
#include<vector>
#include<map>
#include<assert.h>
#include "output.hpp"
using namespace std;

enum type_enum_t {
    TYPE_ENUM_BYTE,
    TYPE_ENUM_BOOL,
    TYPE_ENUM_FUNCTION,
    TYPE_ENUM_INT,
    TYPE_ENUM_STRING,
    TYPE_ENUM_STRUCT_VAR,
    TYPE_ENUM_STRUCT_DECLERATION,
    TYPE_ENUM_VOID
};
extern vector<int> offsets;

class attribute_t {
public:
    type_enum_t type;
    string name;
    int offset;
    int line_num;
    attribute_t(type_enum_t type, string name) :type(type), name(name) {
        offset = offsets.back();
    }
    attribute_t() {}
    virtual int get_size() { return 1; }
};


extern vector<vector<attribute_t*> > tables;

class functionAttribute : public attribute_t {
public:
    vector<attribute_t*> parameters;
    type_enum_t return_type;

    functionAttribute(string name, type_enum_t return_type, vector<attribute_t*> parameters) :
        attribute_t(TYPE_ENUM_FUNCTION, name),
        return_type(return_type),
        parameters(parameters) {}
    functionAttribute() {}
};
class structDeclerationAttribute : public attribute_t {
public:
    vector<attribute_t*> members;
    structDeclerationAttribute(string name, vector<attribute_t*> members) :
        attribute_t(TYPE_ENUM_STRUCT_DECLERATION, name),
        members(members) {}
    structDeclerationAttribute() {}
    int get_size() { return members.size(); }
};
class structVariableAttribute : public attribute_t {
public:
    string struct_name;
    structVariableAttribute(string struct_name, string name) :
        attribute_t(TYPE_ENUM_STRUCT_VAR, name),
        struct_name(struct_name) {}
    structVariableAttribute() {}
    int get_size() {
        for (int i = 0; i < tables.size(); i++)
        {
            for (int j = 0; j < tables[i].size(); j++)
            {
                if (tables[i][j]->name == struct_name)
                {
                    return tables[i][j]->get_size();
                }
            }
        }
        // should check that struct is declared before calling this function
        assert(false);
    }
};

struct expression_t
{
    type_enum_t type;
    string sub_type;
    string belongs_reg; //hw5
    vector<int> true_list;
    vector<int> false_list;
    vector<int> next_list;

    expression_t(type_enum_t type) : 
        type(type),
        sub_type("."),
        belongs_reg(""),
        true_list(vector<int>(0)),
        false_list(vector<int>(0)),
        next_list(vector<int>(0))
    {}
};

struct statement_t {
    vector<int> next_list;

    // List of offsets of jump instruction that were emitted upon encountering "continue;"
    vector<int> continue_list;

    // List of offsets of jump instruction that were emitted upon encountering "break;"
    vector<int> break_list;
    statement_t() :next_list(vector<int>(0)) {}
};

typedef struct
{
    attribute_t* attribute_val;
    vector<attribute_t*>* attributes_list;
    string* string_val;
    type_enum_t type_val;
    expression_t* expression_val;
    vector<expression_t*>* expressions_list;
    statement_t* statement_val;
} STYPE;

#define YYSTYPE STYPE	 // Tell Bison to use STYPE as the stack type
/*

typedef struct
{
string name;
type_enum_t type;
} simpleAttribute;

typedef struct
{
string name;
vector<simpleAttribute> members;
} structAttribute;

typedef struct
{
string name;
type_enum_t return_type;
vector<simpleAttribute> parameters;
} funcAttribute;

typedef union
{
simpleAttribute simpleVal;
structAttribute structVal;
funcAttribute funcVal;
} STYPE;

#define YYSTYPE STYPE	*/ // Tell Bison to use STYPE as the stack type

#endif