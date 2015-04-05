/* Cit
 *
 * Copyright (C) 2003-2004, Alexander Zaprjagaev <frustum@frustum.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;
#include "variable.h"

class InExternVarBase;
class InFooBase;
class InExternClassbase;


class Cit {
public:

	Cit();
	~Cit();
	void clear();
	void Add_script_var(const char *name,InExternVarBase *extern_variable);
	void Add_script_foo(const char *name,InFooBase *extren_function);
	void Add_script_class(const char *name,InExternClassbase *extern_class);
	void setDefine(const char *name,const char *value = NULL);
	int Load_file(const char *name);
    struct Func;
	//int Run(const char *name, const char *args = 0,Func *function = 0);
    template <class T> int Run(T& rv, const char *name, const char *args = 0,Func *function = 0) 
    {
	    int ret = 0;
	    
	    line[0] = '\0';
        _lasterror.empty();
	    
	    try {
		    run_function(name,args,function);
		    if(return_value) {
			    rv = *return_value;
		    }
	    }
	    
	    catch(const char *error) {
		    _lasterror = error;
            _lasterror += line;
		    Type::release();
		    return 0;
	    }
	    
	    Type::release();
	    
	    return ret;
    }

    int Run(char* rv, const char *name, const char *args = 0,Func *function = 0) 
    {
	    int ret = 0;
	    
	    line[0] = '\0';
        _lasterror.empty();
	    
	    try {
		    run_function(name,args,function);
		    if(return_value) 
            {
                ::strcpy(rv,(const char*)*return_value);
		    }
	    }
	    
	    catch(const char *error) {
		    _lasterror = error;
            _lasterror += line;
		    Type::release();
		    return 0;
	    }
	    
	    Type::release();
	    
	    return ret;
    }

	
	
	static const char *error(const char *format,...);
	char *strdup(const char *src);
	
	char read_char(const char *src,const char *delimiter = NULL);
	int  read_name(const char *src,char *dest,int size,const char *symbols = NULL);
	int  read_block(const char *src,char *dest,int size,char from,char to);
	int  read_args(const char *str,char **args,int num_args);
	
	// preprocessor
	const char *define(const char *name,const char *value = NULL);
	int   is_define(const char *name);
	void  undef(const char *name);
	const char *preprocessor(const char *name);
	
	// object recognition
	int is_builtin_type(const char *name);
	int is_variable(const char *name,Func *function);
	int is_function(const char *name);
	int is_extern_variable(const char *name);
	int is_extern_class(const char *name);
	int is_extern_class_object(const char *name);
	
	// variables
	void add_variable(const char *type,const char *args,Func *function);
	void remove_variable(const char *args,Func *function);
	Type **get_variable(const char *name,Func *function);
	const Type *get_extern_variable(const char *name,Func *function);
	
	// functions
	const Type *run_function(const char *name,const char *args,Func *function);
	
	// extern classes
	InExternClassbase *get_extern_class_by_type(const char *name);
	InExternClassbase *get_extern_class_by_name(const char *name);
	const char *get_extern_class_type_by_name(const char *name);
	const Type *run_extern_class_function(const char *name,const char *func,const char *args,Func *function);
	
	// evaluate expressions
	int priority(int op);
	const Type &expression(const char *src,Func *function);
	
	// parser
	int parse_if(const char *src,Func *function);
	int parse_for(const char *src,Func *function);
	int parse_while(const char *src,Func *function);
	int parse(const char *src,Func *function);
	
	// defines
	std::map<std::string,const char*> defines;
	
	struct Func {
		char *type;
		char *args;
		char *body;
		std::map<std::string,const Type*> variables;
		std::map<std::string,std::vector<const Type*> > old_variables;
	};
	
	// user defined objects
	std::map<std::string,const Type*> global_variables;
	std::map<std::string,Func*> functions;
	
	// extern objects
	std::map<std::string,InExternVarBase*> extern_variables;
	std::map<std::string,InFooBase*> extern_functions;
	std::map<std::string,InExternClassbase*> extern_classes;

    string      _lasterror;
	int         break_flag;
	Type*   return_value;
	char        line[1024];
};


/*****************************************************************************/
/*                                                                           */
/* Extern Type                                                           */
/*                                                                           */
/*****************************************************************************/

/*
 */
class InExternVarBase {
public:
	InExternVarBase() { }
	virtual ~InExternVarBase() { }
	
	virtual const Type *get_variable() = 0;
};

/*
 */
template <class type> class InExtVar : public InExternVarBase {
public:
	InExtVar(type *variable) : variable(variable) { }
	virtual ~InExtVar() { }
	
	virtual const Type *get_variable() {
		return &Type::make(*variable);
	}
	
	type *variable;
};

/*****************************************************************************/
/*                                                                           */
/* Extern Functions                                                          */
/*                                                                           */
/*****************************************************************************/

/*
 */
class InFooBase {
public:
	InFooBase() { }
	virtual ~InFooBase() { }
	virtual void Run(Cit *interpreter,const char *args,Cit::Func *function = NULL) = 0;
};

/*
 */
template <class ret_type> class InFoo0 : public InFooBase {
public:
	InFoo0(ret_type (*func)()) : func(func) { }
	virtual ~InFoo0() { }
	
	virtual void Run(Cit *interpreter,const char *args,Cit::Func *function = NULL) {
		interpreter->return_value = (Type*)&Type::make(func());
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (*func)();
};

template <> class InFoo0<void> : public InFooBase {
public:
	InFoo0(void (*func)()) : func(func) { }
	virtual ~InFoo0() { }
	
	virtual void Run(Cit *interpreter,const char *args,Cit::Func *function = NULL) {
		func();
	}
	
	void (*func)();
};

/*
 */
template <class ret_type,class arg_0_type> 
class InFoo1 : public InFooBase {
public:
	InFoo1(ret_type (*func)(arg_0_type)) : func(func) { }
	virtual ~InFoo1() { }
	
	virtual void Run(Cit *interpreter,
                     const char *args,
                     Cit::Func *function = 0) 
    {
		VariableToType<arg_0_type>  arg_0(interpreter->expression(args,function));
		
        interpreter->return_value = (Type*)&Type::make(func(arg_0.value));
		
        interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (*func)(arg_0_type);
};

template <class ret_type,class arg_0_type,class arg_1_type> class InFoo2 : public InFooBase {
public:
	InFoo2(ret_type (*func)(arg_0_type,arg_1_type)) : func(func) { }
	virtual ~InFoo2() { }
	
	virtual void Run(Cit *interpreter,const char *args,Cit::Func *function = NULL) {
		char buf[2][1024];
		char *a[2] = { buf[0], buf[1] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		interpreter->return_value = (Type*)&Type::make(func(arg_0.value,arg_1.value));
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (*func)(arg_0_type,arg_1_type);
};
template <class ret_type,class arg_0_type,class arg_1_type,class arg_2_type> class InFoo3 : public InFooBase {
public:
	InFoo3(ret_type (*func)(arg_0_type,arg_1_type,arg_2_type)) : func(func) { }
	virtual ~InFoo3() { }
	
	virtual void Run(Cit *interpreter,const char *args,Cit::Func *function = NULL) {
		char buf[3][1024];
		char *a[3] = { buf[0], buf[1], buf[2] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		VariableToType<arg_2_type> arg_2(interpreter->expression(a[2],function));
		interpreter->return_value = (Type*)&Type::make(func(arg_0.value,arg_1.value,arg_2.value));
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (*func)(arg_0_type,arg_1_type,arg_2_type);
};

template <class ret_type,class arg_0_type,class arg_1_type,class arg_2_type,class arg_3_type> class InFoo4 : public InFooBase {
public:
	InFoo4(ret_type (*func)(arg_0_type,arg_1_type,arg_2_type,arg_3_type)) : func(func) { }
	virtual ~InFoo4() { }
	
	virtual void Run(Cit *interpreter,const char *args,Cit::Func *function = NULL) {
		char buf[4][1024];
		char *a[4] = { buf[0], buf[1], buf[2], buf[3] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		VariableToType<arg_2_type> arg_2(interpreter->expression(a[2],function));
		VariableToType<arg_3_type> arg_3(interpreter->expression(a[3],function));
		interpreter->return_value = (Type*)&Type::make(func(arg_0.value,arg_1.value,arg_2.value,arg_3.value));
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (*func)(arg_0_type,arg_1_type,arg_2_type,arg_3_type);
};

template <class ret_type,class arg_0_type,class arg_1_type,class arg_2_type,class arg_3_type,class arg_4_type> class InFoo5 : public InFooBase {
public:
	InFoo5(ret_type (*func)(arg_0_type,arg_1_type,arg_2_type,arg_3_type,arg_4_type)) : func(func) { }
	virtual ~InFoo5() { }
	
	virtual void Run(Cit *interpreter,const char *args,Cit::Func *function = NULL) {
		char buf[5][1024];
		char *a[5] = { buf[0], buf[1], buf[2], buf[3], buf[4] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		VariableToType<arg_2_type> arg_2(interpreter->expression(a[2],function));
		VariableToType<arg_3_type> arg_3(interpreter->expression(a[3],function));
		VariableToType<arg_4_type> arg_4(interpreter->expression(a[4],function));
		interpreter->return_value = (Type*)&Type::make(func(arg_0.value,arg_1.value,arg_2.value,arg_3.value,arg_4.value));
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (*func)(arg_0_type,arg_1_type,arg_2_type,arg_3_type,arg_4_type);
};

template <class ret_type,class arg_0_type,class arg_1_type,class arg_2_type,class arg_3_type,class arg_4_type,class arg_5_type> class InFoo6 : public InFooBase {
public:
	InFoo6(ret_type (*func)(arg_0_type,arg_1_type,arg_2_type,arg_3_type,arg_4_type,arg_5_type)) : func(func) { }
	virtual ~InFoo6() { }
	
	virtual void Run(Cit *interpreter,const char *args,Cit::Func *function = NULL) {
		char buf[6][1024];
		char *a[6] = { buf[0], buf[1], buf[2], buf[3], buf[4], buf[5] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		VariableToType<arg_2_type> arg_2(interpreter->expression(a[2],function));
		VariableToType<arg_3_type> arg_3(interpreter->expression(a[3],function));
		VariableToType<arg_4_type> arg_4(interpreter->expression(a[4],function));
		VariableToType<arg_5_type> arg_5(interpreter->expression(a[5],function));
		interpreter->return_value = (Type*)&Type::make(func(arg_0.value,arg_1.value,arg_2.value,arg_3.value,arg_4.value,arg_5.value));
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (*func)(arg_0_type,arg_1_type,arg_2_type,arg_3_type,arg_4_type,arg_5_type);
};

/* extern class functions
 */
template <class type> class InterpreterExternClassFunctionBase {
public:
	InterpreterExternClassFunctionBase() { }
	virtual ~InterpreterExternClassFunctionBase() { }
	
	virtual void Run(Cit *interpreter,type *object,const char *args,Cit::Func *function = NULL) = 0;
};

/*
 */
template <class type,class ret_type> class InterpreterExternClassFunction0 : public InterpreterExternClassFunctionBase<type> {
public:
	InterpreterExternClassFunction0(ret_type (type::*func)()) : func(func) { }
	~InterpreterExternClassFunction0() { }
	
	virtual void Run(Cit *interpreter,type *object,const char *args,Cit::Func *function = NULL) {
		interpreter->return_value = (Type*)&Type::make((object->*func)());
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (type::*func)();
};
template <class type,class ret_type,class arg_0_type> class InterpreterExternClassFunction1 : public InterpreterExternClassFunctionBase<type> {
public:
	InterpreterExternClassFunction1(ret_type (type::*func)(arg_0_type)) : func(func) { }
	~InterpreterExternClassFunction1() { }
	
	virtual void Run(Cit *interpreter,type *object,const char *args,Cit::Func *function = NULL) {
		VariableToType<arg_0_type> arg_0(interpreter->expression(args,function));
		interpreter->return_value = (Type*)&Type::make((object->*func)(arg_0.value));
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (type::*func)(arg_0_type);
};
template <class type,class ret_type,class arg_0_type,class arg_1_type> class InterpreterExternClassFunction2 : public InterpreterExternClassFunctionBase<type> {
public:
	InterpreterExternClassFunction2(ret_type (type::*func)(arg_0_type,arg_1_type)) : func(func) { }
	~InterpreterExternClassFunction2() { }
	
	virtual void Run(Cit *interpreter,type *object,const char *args,Cit::Func *function = NULL) {
		char buf[2][1024];
		char *a[2] = { buf[0], buf[1] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		interpreter->return_value = (Type*)&Type::make((object->*func)(arg_0.value,arg_1.value));
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (type::*func)(arg_0_type,arg_1_type);
};

template <class type,class ret_type,class arg_0_type,class arg_1_type,class arg_2_type> class InterpreterExternClassFunction3 : public InterpreterExternClassFunctionBase<type> {
public:
	InterpreterExternClassFunction3(ret_type (type::*func)(arg_0_type,arg_1_type,arg_2_type)) : func(func) { }
	~InterpreterExternClassFunction3() { }
	
	virtual void Run(Cit *interpreter,type *object,const char *args,Cit::Func *function = NULL) {
		char buf[3][1024];
		char *a[3] = { buf[0], buf[1], buf[2] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		VariableToType<arg_2_type> arg_2(interpreter->expression(a[2],function));
		interpreter->return_value = (Type*)&Type::make((object->*func)(arg_0.value,arg_1.value,arg_2.value));
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (type::*func)(arg_0_type,arg_1_type,arg_2_type);
};

template <class type,class ret_type,class arg_0_type,class arg_1_type,class arg_2_type,class arg_3_type> class InterpreterExternClassFunction4 : public InterpreterExternClassFunctionBase<type> {
public:
	InterpreterExternClassFunction4(ret_type (type::*func)(arg_0_type,arg_1_type,arg_2_type,arg_3_type)) : func(func) { }
	~InterpreterExternClassFunction4() { }
	
	virtual void Run(Cit *interpreter,type *object,const char *args,Cit::Func *function = NULL) {
		char buf[4][1024];
		char *a[4] = { buf[0], buf[1], buf[2], buf[3] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		VariableToType<arg_2_type> arg_2(interpreter->expression(a[2],function));
		VariableToType<arg_3_type> arg_3(interpreter->expression(a[3],function));
		interpreter->return_value = (Type*)&Type::make((object->*func)(arg_0.value,arg_1.value,arg_2.value,arg_3.value));
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (type::*func)(arg_0_type,arg_1_type,arg_2_type,arg_3_type);
};

template <class type,class ret_type,class arg_0_type,class arg_1_type,class arg_2_type,class arg_3_type,class arg_4_type> class InterpreterExternClassFunction5 : public InterpreterExternClassFunctionBase<type> {
public:
	InterpreterExternClassFunction5(ret_type (type::*func)(arg_0_type,arg_1_type,arg_2_type,arg_3_type,arg_4_type)) : func(func) { }
	~InterpreterExternClassFunction5() { }
	
	virtual void Run(Cit *interpreter,type *object,const char *args,Cit::Func *function = NULL) {
		char buf[5][1024];
		char *a[5] = { buf[0], buf[1], buf[2], buf[3], buf[4] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		VariableToType<arg_2_type> arg_2(interpreter->expression(a[2],function));
		VariableToType<arg_3_type> arg_3(interpreter->expression(a[3],function));
		VariableToType<arg_4_type> arg_4(interpreter->expression(a[4],function));
		interpreter->return_value = (Type*)&Type::make((object->*func)(arg_0.value,arg_1.value,arg_2.value,arg_3.value,arg_4.value));
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (type::*func)(arg_0_type,arg_1_type,arg_2_type,arg_3_type,arg_4_type);
};

template <class type,class ret_type,class arg_0_type,class arg_1_type,class arg_2_type,class arg_3_type,class arg_4_type,class arg_5_type> class InterpreterExternClassFunction6 : public InterpreterExternClassFunctionBase<type> {
public:
	InterpreterExternClassFunction6(ret_type (type::*func)(arg_0_type,arg_1_type,arg_2_type,arg_3_type,arg_4_type,arg_5_type)) : func(func) { }
	~InterpreterExternClassFunction6() { }
	
	virtual void Run(Cit *interpreter,type *object,const char *args,Cit::Func *function = NULL) {
		char buf[6][1024];
		char *a[6] = { buf[0], buf[1], buf[2], buf[3], buf[4], buf[5] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		VariableToType<arg_2_type> arg_2(interpreter->expression(a[2],function));
		VariableToType<arg_3_type> arg_3(interpreter->expression(a[3],function));
		VariableToType<arg_4_type> arg_4(interpreter->expression(a[4],function));
		VariableToType<arg_5_type> arg_5(interpreter->expression(a[5],function));
		interpreter->return_value = (Type*)&Type::make((object->*func)(arg_0.value,arg_1.value,arg_2.value,arg_3.value,arg_4.value,arg_5.value));
		interpreter->return_value->setTypeInfo(typeid(ret_type));
	}
	
	ret_type (type::*func)(arg_0_type,arg_1_type,arg_2_type,arg_3_type,arg_4_type,arg_5_type);
};

class InExternClassbase {
public:
	InExternClassbase() { }
	virtual ~InExternClassbase() { }
	
	virtual int is_object(const char *name) = 0;
	virtual const void* get_object(const char *name) = 0;
	
	virtual const std::type_info &get_type_info() = 0;
	
	virtual void null_constructor(const char *name) = 0;
	virtual void constructor(Cit *interpreter,const char *name,const char *args,Cit::Func *function = NULL) = 0;
	virtual void assignment(const char *name,const Type &variable) = 0;
	
	virtual void destructor(const char *name) = 0;
	
	virtual void Run(Cit *i,const char *name,const char *func,const char *args,Cit::Func *function = NULL) = 0;
};

/*
 */
template <class type> class InterpreterExternClass : public InExternClassbase {
public:
	InterpreterExternClass() { }
	virtual ~InterpreterExternClass() 
    {
        clear();
	}

    void clear()
    {
		typename std::map<std::string,type*>::iterator object_it;
		for(object_it = objects.begin(); object_it != objects.end(); object_it++) {
			if(object_it->second) destructor(object_it->first.c_str());
		}
		for(int i = 0; i < (int)unnamed_objects.size(); i++) {
			if(unnamed_objects[i]) delete unnamed_objects[i];
		}
		typename std::map<std::string,InterpreterExternClassFunctionBase<type>*>::iterator function_it;
		for(function_it = functions.begin(); function_it != functions.end(); function_it++) {
			delete function_it->second;
		}
    }
	
	// add class memeber function
	void addClassFunction(const char *name,InterpreterExternClassFunctionBase<type> *function) {
		typename std::map<std::string,InterpreterExternClassFunctionBase<type>*>::iterator it = functions.find(name);
		if(it == functions.end()) {
			functions[name] = function;
			return;
		}
		fprintf(stderr,"InterpreterExternClass::addClassFunction(): \"%s\" function already exists\n",name);
	}
	
	// check names
	virtual int is_object(const char *name) {
		typename std::map<std::string,type*>::iterator it = objects.find(name);
		if(it != objects.end()) return 1;
		return 0;
	}
	
	// get object
	virtual const void* get_object(const char *name) {
		typename std::map<std::string,type*>::iterator it = objects.find(name);
		if(it != objects.end()) return (const void*)it->second;
		return NULL;
	}
	
	// get type info
	virtual const std::type_info &get_type_info() {
		return typeid(type*);
	}
	
	// add object
	void add_object(const char *name,type *object) {
		typename std::map<std::string,type*>::iterator it = objects.find(name);
		if(it == objects.end()) {
			objects[name] = object;
			return;
		}
		if(it->second) unnamed_objects.push_back(it->second);
		it->second = object;
	}
	
	// null class constructor
	virtual void null_constructor(const char *name) {
		add_object(name,NULL);
	}
	
	// class constructor
	virtual void constructor(Cit *interpreter,const char *name,const char *args,Cit::Func *function = NULL) = 0;
	
	// class assignment
	virtual void assignment(const char *name,const Type &variable) {
		VariableToType<type*> ptr(variable);
		add_object(name,(type*)ptr.value);
	}
	
	// class destructor
	virtual void destructor(const char *name) {
		typename std::map<std::string,type*>::iterator it = objects.find(name);
		if(it == objects.end()) throw(Cit::error("InterpreterExternClass::destructor(): can't find \"%s\" object\n",name));
		if(it->second == NULL) throw(Cit::error("InterpreterExternClass::destructor(): can't delete \"%s\" object is NULL\n",name));
		// remove all identical objects
		typename std::map<std::string,type*>::iterator object_it;
		for(object_it = objects.begin(); object_it != objects.end(); object_it++) {
			if(object_it != it && object_it->second == it->second) {
				object_it->second = NULL;
			}
		}
		for(int i = 0; i < (int)unnamed_objects.size(); i++) {
			if(unnamed_objects[i] == it->second) unnamed_objects[i] = NULL;
		}
		delete it->second;
		it->second = NULL;
	}
	
	// Run member function
	virtual void Run(Cit *interpreter,const char *name,const char *func,const char *args,Cit::Func *function = NULL) {
		typename std::map<std::string,type*>::iterator object_it = objects.find(name);
		if(object_it == objects.end()) throw(Cit::error("InterpreterExternClass::Run(): can't find \"%s\" object\n",name));
		
		if(object_it->second == NULL) throw(Cit::error("InterpreterExternClass::Run(): can't Run \"%s\" function because \"%s\" is NULL object\n",func,name));
		
		typename std::map<std::string,InterpreterExternClassFunctionBase<type>*>::iterator function_it = functions.find(func);
		if(function_it == functions.end()) throw(Cit::error("InterpreterExternClass::Run(): can't find \"%s\" function for \"%s\" object\n",func,name));
		
		function_it->second->Run(interpreter,object_it->second,args,function);
	}
	
	std::map<std::string,type*> objects;
	std::vector<type*> unnamed_objects;
	std::map<std::string,InterpreterExternClassFunctionBase<type>*> functions;
};

/*
 */
template <class type> class InterpreterExternClass0 : public InterpreterExternClass<type> {
public:
	InterpreterExternClass0() { }
	virtual ~InterpreterExternClass0() { }
	
	virtual void constructor(Cit *interpreter,const char *name,const char *args,Cit::Func *function = NULL) {
		add_object(name,new type());
	}
};

template <class type,class arg_0_type> class InterpreterExternClass1 : public InterpreterExternClass<type> {
public:
	InterpreterExternClass1() { }
	virtual ~InterpreterExternClass1() { }
	
	virtual void constructor(Cit *interpreter,const char *name,const char *args,Cit::Func *function = NULL) {
		VariableToType<arg_0_type> arg_0(interpreter->expression(args,function));
		add_object(name,new type(arg_0.value));
	}
};

template <class type,class arg_0_type,class arg_1_type> class InterpreterExternClass2 : public InterpreterExternClass<type> {
public:
	InterpreterExternClass2() { }
	virtual ~InterpreterExternClass2() { }
	
	virtual void constructor(Cit *interpreter,const char *name,const char *args,Cit::Func *function = NULL) {
		char buf[2][1024];
		char *a[2] = { buf[0], buf[1] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		add_object(name,new type(arg_0.value,arg_1.value));
	}
};

template <class type,class arg_0_type,class arg_1_type,class arg_2_type> class InterpreterExternClass3 : public InterpreterExternClass<type> {
public:
	InterpreterExternClass3() { }
	virtual ~InterpreterExternClass3() { }
	
	virtual void constructor(Cit *interpreter,const char *name,const char *args,Cit::Func *function = NULL) {
		char buf[3][1024];
		char *a[3] = { buf[0], buf[1], buf[2] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		VariableToType<arg_2_type> arg_2(interpreter->expression(a[2],function));
		add_object(name,new type(arg_0.value,arg_1.value,arg_2.value));
	}
};

template <class type,class arg_0_type,class arg_1_type,class arg_2_type,class arg_3_type> class InterpreterExternClass4 : public InterpreterExternClass<type> {
public:
	InterpreterExternClass4() { }
	virtual ~InterpreterExternClass4() { }
	
	virtual void constructor(Cit *interpreter,const char *name,const char *args,Cit::Func *function = NULL) {
		char buf[4][1024];
		char *a[4] = { buf[0], buf[1], buf[2], buf[3] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		VariableToType<arg_2_type> arg_2(interpreter->expression(a[2],function));
		VariableToType<arg_3_type> arg_3(interpreter->expression(a[3],function));
		add_object(name,new type(arg_0.value,arg_1.value,arg_2.value,arg_3.value));
	}
};

template <class type,class arg_0_type,class arg_1_type,class arg_2_type,class arg_3_type,class arg_4_type> class InterpreterExternClass5 : public InterpreterExternClass<type> {
public:
	InterpreterExternClass5() { }
	virtual ~InterpreterExternClass5() { }
	
	virtual void constructor(Cit *interpreter,const char *name,const char *args,Cit::Func *function = NULL) {
		char buf[5][1024];
		char *a[5] = { buf[0], buf[1], buf[2], buf[3], buf[4] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		VariableToType<arg_2_type> arg_2(interpreter->expression(a[2],function));
		VariableToType<arg_3_type> arg_3(interpreter->expression(a[3],function));
		VariableToType<arg_4_type> arg_4(interpreter->expression(a[4],function));
		add_object(name,new type(arg_0.value,arg_1.value,arg_2.value,arg_3.value,arg_4.value));
	}
};

template <class type,class arg_0_type,class arg_1_type,class arg_2_type,class arg_3_type,class arg_4_type,class arg_5_type> class InterpreterExternClass6 : public InterpreterExternClass<type> {
public:
	InterpreterExternClass6() { }
	virtual ~InterpreterExternClass6() { }
	
	virtual void constructor(Cit *interpreter,const char *name,const char *args,Cit::Func *function = NULL) {
		char buf[6][1024];
		char *a[6] = { buf[0], buf[1], buf[2], buf[3], buf[4], buf[5] };
		interpreter->read_args(args,a,sizeof(a) / sizeof(a[0]));
		VariableToType<arg_0_type> arg_0(interpreter->expression(a[0],function));
		VariableToType<arg_1_type> arg_1(interpreter->expression(a[1],function));
		VariableToType<arg_2_type> arg_2(interpreter->expression(a[2],function));
		VariableToType<arg_3_type> arg_3(interpreter->expression(a[3],function));
		VariableToType<arg_4_type> arg_4(interpreter->expression(a[4],function));
		VariableToType<arg_5_type> arg_5(interpreter->expression(a[5],function));
		add_object(name,new type(arg_0.value,arg_1.value,arg_2.value,arg_3.value,arg_4.value,arg_5.value));
	}
};

#endif /* __INTERPRETER_H__ */
