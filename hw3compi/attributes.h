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
    attribute_t(type_enum_t type, string name) :type(type), name(name) { 
        offset = offsets.back();
    }
    attribute_t() {}
    virtual int get_size() {return 1; }
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
    structDeclerationAttribute(){}
    int get_size() { return members.size(); }
};
class structVariableAttribute : public attribute_t {
public:
    string struct_name;
    structVariableAttribute(string struct_name, string name) :
        attribute_t(TYPE_ENUM_STRUCT_VAR, name),
        struct_name(struct_name) {}
    structVariableAttribute(){}
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

typedef union
{
    attribute_t* attribute_val;
    vector<attribute_t*>* attributes_list;
    string* string_val;
    type_enum_t type_val;
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