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
#pragma warning  (disable:4786)
#include "stdafx.h"
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "interpreter.h"
using namespace std;

//---------------------------------------------------------------------------------------
Cit::Cit() {}

//---------------------------------------------------------------------------------------
Cit::~Cit() {

    clear();
    Type::release();
}

//---------------------------------------------------------------------------------------
void Cit::clear()
{
	map<string,const char*>::iterator define_it;
	for(define_it = defines.begin(); define_it != defines.end(); define_it++) 
    {
        char* pc= (char*)define_it->second;
		delete [] pc;
	}
    defines.clear();
	
	// remove global variables
	map<string,const Type*>::iterator variable_it;
	for(variable_it = global_variables.begin(); 
        variable_it != global_variables.end(); 
        variable_it++) 
    {
		delete variable_it->second;
	}
    global_variables.clear();
	
	// remove functions
	map<string,Func*>::iterator function_it;
	for(function_it = functions.begin(); function_it != functions.end(); function_it++) {
		delete [] function_it->second->type;
		delete [] function_it->second->args;
		delete [] function_it->second->body;
		delete function_it->second;
	}
    functions.clear();
	
	// remove extern objects
	map<string,InExternVarBase*>::iterator extern_variable_it;
	for(extern_variable_it = extern_variables.begin(); extern_variable_it != extern_variables.end(); extern_variable_it++) {
		delete extern_variable_it->second;
	}
    extern_variables.clear();
	
	map<string,InFooBase*>::iterator extern_function_it;
	for(extern_function_it = extern_functions.begin(); extern_function_it != extern_functions.end(); extern_function_it++) {
		delete extern_function_it->second;
	}
    extern_functions.clear();
	
	map<string,InExternClassbase*>::iterator extern_class_it;
	for(extern_class_it = extern_classes.begin(); extern_class_it != extern_classes.end(); extern_class_it++) {
		delete extern_class_it->second;
	}
    extern_classes.clear();
}

//---------------------------------------------------------------------------------------
void Cit::Add_script_var(const char *name,InExternVarBase *extern_variable) 
{
	map<string,InExternVarBase*>::iterator it = extern_variables.find(name);
	if(it != extern_variables.end()) throw(error("Add_script_var(): \"%s\" variable already exists\n",name));
	extern_variables[name] = extern_variable;
}

//---------------------------------------------------------------------------------------
void Cit::Add_script_foo(const char *name,InFooBase *extern_function) {
	map<string,InFooBase*>::iterator it = extern_functions.find(name);
	if(it != extern_functions.end()) throw(error("Add_script_foo(): \"%s\" function already exists\n",name));
	extern_functions[name] = extern_function;
}

//---------------------------------------------------------------------------------------
void Cit::Add_script_class(const char *name,InExternClassbase *extern_class) {
	map<string,InExternClassbase*>::iterator it = extern_classes.find(name);
	if(it != extern_classes.end()) throw(error("Add_script_class(): \"%s\" class already exists\n",name));
	extern_classes[name] = extern_class;
}

//---------------------------------------------------------------------------------------
const char *Cit::error(const char *format,...) {
	static char buf[1024];
	va_list arg;
	va_start(arg,format);
#ifdef _WIN32
	_vsnprintf(buf,sizeof(buf),format,arg);
#else
	vsnprintf(buf,sizeof(buf),format,arg);
#endif
	va_end(arg);
	return buf;
}

//---------------------------------------------------------------------------------------
char *Cit::strdup(const char *str) {
	char *ret = new char[strlen(str) + 1];
	strcpy(ret,str);
	return ret;
}

//---------------------------------------------------------------------------------------
char Cit::read_char(const char *src,const char *delimiter) {
	
	const char *s = src;
	
	// skip spaces
	////while(*s && strchr(" \n",*s)) 
    ////    s++;

    while(*s && *s==' ' || *s == '\t' || *s=='\n' ) 
        s++;
	
	if(delimiter) {
		int is_string = 0;
		while(*s) {
			if(*s == '\\') {	// backslash
				s += 2;
			} else if(is_string == 0) {
				if(*s == '"') is_string = 1;
				if(strchr(delimiter,*s)) break;
				s++;
			} else if(is_string == 1) {
				if(*s == '"') is_string = 0;
				s++;
			}
		}
	}
	
	return *s;
}

/*
 */
int Cit::read_name(const char *src,char *dest,int size,const char *symbols) {
	
	const char *s = src;
	
	// skip spaces
	while(*s && strchr(" \n",*s)) s++;
	
	// end of the string
	if(*s == '\0') {
		if(dest) *dest = '\0';
		return 0;
	}
	
	// if zero target
	if(dest == NULL) size = 1000000;
	
	if(symbols) {
		while(*s && strchr(symbols,*s) && size > 1) {
			if(dest) *dest++ = *s++;
			else s++;
			size--;
		}
	} else {
		// letters and digits
		while(*s && (isalpha(*s) || strchr("0123456789_",*s)) && size > 1) {
			if(dest) *dest++ = *s++;
			else s++;
			size--;
		}
	}
	if(dest) *dest = '\0';
	
	if(s == src) {
		throw(error("read_name(): unknown symbol \'%c\'\n",*s));
	}
	
	return s - src;
}

/*
 */
int Cit::read_block(const char *src,char *dest,int size,char from,char to) {
	
	const char *s = src;
	
	// skip spaces
	while(*s && *s == ' ') s++;
	
	// end of the string
	if(*s == '\0') {
		if(dest) *dest = '\0';
		return 0;
	}
	
	int is_string = 0;
	int brackets = 0;
	int counter = 0;
	
	if(*s == from) {
		if(*s == '"') is_string = 1;
		if(*s == '(') brackets++;
		counter++;
		s++;
	}
	
	// if zero target
	if(dest == NULL) size = 1000000;
	
	while(*s && size > 1) {
		if(*s == '\\') {	// backslash
			if(dest) {
				if(from == '"' && to == '"') {
					s++;
					if(*s == 'n') *dest++ = '\n';
					else if(*s == 'r') *dest++ = '\r';
					else if(*s == 't') *dest++ = '\t';
					else if(*s == '"') *dest++ = '\"';
					else if(!strncmp(s,"033",3)) {
						*dest++ = '\033';
						s += 2;
					}
					s++;
				} else {
					*dest++ = *s++;
					*dest++ = *s++;
				}
			} else {
				s += 2;
			}
			size -= 2;
		} else if(is_string == 0) {
			if(*s == '"') {
				is_string = 1;
			} else {
				if(*s == '(') brackets++;
				else if(*s == ')') brackets--;
				if(from && to) {
					if(*s == from) counter++;
					if(*s == to) counter--;
				}
			}
			if(brackets < 0) 
                throw(error("read_block(): some errors with counts of '(' and ')' symbols\n"));
			if(counter < 0) 
                throw(error("read_block(): some errors with counts of '%c' and '%c' symbols\n",from,to));
			if(counter == 0 && brackets == 0 && *s == to) {
				s++;
				break;
			}
			if(dest) *dest++ = *s++;
			else s++;
			size--;
		} else if(is_string == 1) {
			if(*s == '"') {
				is_string = 0;
				if(*s == to) {
					s++;
					break;
				}
			}
			if(dest) *dest++ = *s++;
			else s++;
			size--;
		}
	}
	if(dest) *dest = '\0';
	
	return s - src;
}

/*
 */
int Cit::read_args(const char *src,char **args,int num_args) {
	
	const char *s = src;
	
	if(num_args > 8) throw(error("read_args(): too many arguments maximum is 8\n",num_args));
	
	for(int i = 0; i < num_args; i++) {
		s += read_block(s,args[i],1024,0,',');
		if(*(args[i]) == '\0') throw(error("read_args(): can't read %d argument in \"%s\"\n",i + 1,src));
		if(i == num_args - 1 && *s != '\0') throw(error("read_args(): too many arguments in \"%s\"\n",src));
	}
	
	return s - src;
}

/*****************************************************************************/
/*                                                                           */
/* preprocessor                                                              */
/*                                                                           */
/*****************************************************************************/

/*
 */
const char *Cit::define(const char *name,const char *value) {
	map<string,const char*>::iterator it = defines.find(name);
	if(it == defines.end()) {
		char *s = (value != NULL) ? strdup(value) : NULL;
		defines[name] = s;
		return s;
	}
	return it->second;
}

/*
 */
int Cit::is_define(const char *name) {
	map<string,const char*>::iterator it = defines.find(name);
	if(it == defines.end()) return 0;
	return 1;
}

/*
 */
void Cit::undef(const char *name) {
	map<string,const char*>::iterator it = defines.find(name);
	if(it == defines.end()) return;
	if(it->second) delete [] (char*)it->second;
	defines.erase(it);
}

/*
 */
const char *Cit::preprocessor(const char *name) {
	
	FILE *file = fopen(name,"rb");
	if(!file) return NULL;
	
	char path[1024];
	strncpy(path,name,sizeof(path));
	char *p = path + strlen(path);
	while(p > path && (*p != '/' && *p != '\\')) p--;
	*p = '\0';
	
	fseek(file,0,SEEK_END);
	int size = ftell(file);
	fseek(file,0,SEEK_SET);
	char *data = new char[size + 1];
	data[size] = '\0';
	fread(data,sizeof(char),size,file);
	fclose(file);
	
	char *s = data;		// source string
	char *d = data;		// dest string
	
	while(*s) {
		// skip // comments
		if(*s == '/' && *(s + 1) == '/') {
			while(*s && *s != '\n') s++;
		}
		// skip /* */ comments
		else if(*s == '/' && *(s + 1) == '*') {
			while(*s && (*s != '*' || *(s + 1) != '/')) s++;
			s += 2;
		}
		// skip \r symbols
		else if(*s == '\r') {
			s++;
		}
		// don't change strings
		else if(*s == '"') {
			*d++ = *s++;
			while(*s && (*s != '"' || *(s - 1) == '\\')) *d++ = *s++;
			if(*s) *d++ = *s++;
		}
		// change '\t' to ' '
		else if(*s == '\t') {
			s++;
			*d++ = ' ';
		}
		else {
			*d++ = *s++;
		}
	}
	*d = '\0';
	
	s = data;
	d = data;
	
	int define_depth = 0;
	int define_stack[64];
	int define_ok = 1;
	
	// preprocessor
	while(*s) {
		
		// script
		if(*s == '#' && *(s + 1) == '!') {
			s += read_block(s,NULL,0,0,'\n');
			continue;
		}
		// ifdef
		else if(!strncmp(s,"#ifdef",6)) {
			s += 6;
			char name[128];
			s += read_name(s,name,sizeof(name));
			define_stack[define_depth++] = is_define(name);
			int i;
			for(i = 0; i < define_depth; i++) if(define_stack[i] == 0) break;
			define_ok = (i == define_depth) ? 1 : 0;
			continue;
		}
		// ifndef
		else if(!strncmp(s,"#ifndef",6)) {
			s += 7;
			char name[128];
			s += read_name(s,name,sizeof(name));
			define_stack[define_depth++] = !is_define(name);
			int i;
			for(i = 0; i < define_depth; i++) if(define_stack[i] == 0) break;
			define_ok = (i == define_depth) ? 1 : 0;
			continue;
		}
		// else
		else if(!strncmp(s,"#else",5)) {
			s += 5;
			if(define_depth < 1) {
				delete [] data;
				throw(error("preprocessor(): #else is before #ifdef or #ifndef\n"));
			}
			define_stack[define_depth - 1] = !define_stack[define_depth - 1];
			int i;
			for(i = 0; i < define_depth; i++) if(define_stack[i] == 0) break;
			define_ok = (i == define_depth) ? 1 : 0;
			continue;
		}
		// endif
		else if(!strncmp(s,"#endif",6)) {
			s += 6;
			if(define_depth < 1) {
				delete [] data;
				throw(error("preprocessor(): #endif is before #ifdef or #ifndef\n"));
			}
			define_depth--;
			int i;
			for(i = 0; i < define_depth; i++) if(define_stack[i] == 0) break;
			define_ok = (i == define_depth) ? 1 : 0;
			continue;
		}
		// define
		else if(!strncmp(s,"#define",7)) {
			s += 7;
			char name[128];
			char value[1024];
			s += read_name(s,name,sizeof(name));
			s += read_block(s,value,sizeof(value),0,'\n');
			if(define_ok) {
				define(name,value);
			}
			continue;
		}
		// undef
		else if(!strncmp(s,"#undef",6)) {
			s += 6;
			char name[1024];
			s += read_name(s,name,sizeof(name));
			if(define_ok) {
				undef(name);
			}
			continue;
		}
		// include
		else if(!strncmp(s,"#include",8)) {
			s += 8;
			char name[128];
			char c = read_char(s,"\"<");
			if(c == '"') s += read_block(s,name,sizeof(name),'"','"');
			else if(c == '<') s += read_block(s,name,sizeof(name),'<','>');
			else throw(error("preprocessor(): unknown symbol '%c'\n",c));
			if(define_ok) {
				const char *include = preprocessor(name);
				if(include == NULL) {
					char buf[1024];
					sprintf(buf,"%s/%s",path,name);
					include = preprocessor(buf);
				}
				if(include == NULL) throw(error("preprocessor(): can't find \"%s\" file\n",name));
				
				int include_size = strlen(include);
				char *new_data = new char[size + include_size + 1];
				memset(new_data,0,size + include_size + 1);
				
				memcpy(new_data,data,d - data);
				memcpy(new_data + (d - data),include,include_size);
				memcpy(new_data + (d - data) + include_size,s,size - (s - data) + 1);
				
				s = new_data + (d - data) + include_size;
				d = s;
				
				delete [] (char*)include;
				delete [] data;
				data = new_data;
				
				size += include_size;
			}
			continue;
		}
		// unknown preprocessor token
		else if(*s == '#') {
			char buf[128];
			read_name(s,buf,sizeof(buf));
			delete [] data;
			throw(error("preprocessor(): unknown prepocessor token \"%s\"\n",buf));
		}
		
		if(define_ok == 0) {
			s++;
		}
		else {
			// don't change symbols
			if(*s == '\'' && *(s + 1) == '\\' && *(s + 2) && *(s + 3) == '\'') {
				for(int i = 0; i < 4; i++) *d++ = *s++;
			}
			else if(*s == '\'' && *(s + 2) != '\\' && *(s + 2) == '\'') {
				for(int i = 0; i < 3; i++) *d++ = *s++;
			}
			// don't change strings
			else if(*s == '"') {
				*d++ = *s++;
				while(*s && (*s != '"' || *(s - 1) == '\\')) *d++ = *s++;
				*d++ = *s++;
			}
			// skip double spaces and zero strings
			else if(strchr(" \n",*s) && strchr(" \n",*(d - 1))) {
				s++;
			}
			else {
				
				*d = *s;
				s++;
				
				// replacement of defined words
				if(d > data && strchr(" \n,.=:;()[]{}<>+-*/%!&|^\"'",*d)) {
					char *t = d - 1;
					while(t > data && !strchr(" \n,.=:;()[]{}<>+-*/%!&|^\"'",*t)) {
						t--;
					}
					if(t > data) t++;
					if(t < d) {
						map<string,const char*>::iterator it;
						for(it = defines.begin(); it != defines.end(); it++) {
							if(!strncmp(it->first.c_str(),t,d - t)) {
								const char *define = it->second;
								if(define && define != '\0') {
									int define_size = strlen(define);
									char *new_data = new char[size + define_size + 1];
									memset(new_data,0,size + define_size + 1);
									
									memcpy(new_data,data,t - data);
									memcpy(new_data + (t - data),define,define_size);
									*(new_data + (t - data) + define_size) = *d;
									memcpy(new_data + (t - data) + define_size + 1,s,size - (s - data) + 1);
									
									s = new_data + (t - data) + define_size + 1;
									d = s - 1;
									
									delete [] data;
									data = new_data;
									
									size += define_size;
								} else {
									*t = *d;
									d = t;
								}
								break;
							}
						}
					}
				} // end of the replacement
				
				d++;
			}
		}
	}
	*d = '\0';
	
	if(define_depth != 0) {
		delete [] data;
		throw(error("prepocessor(): depth of stack isn't equal to zero\n"));
	}
	
	// skip spaces
	s = data;
	d = data;
	while(*s) {
		if(*s == '"') {
			*d++ = *s++;
			while(*s && (*s != '"' || *(s - 1) == '\\')) *d++ = *s++;
			if(*s) *d++ = *s++;
		} else {
			if(*s == ' ' && (strchr("*/%+-!=<>&|^{}[]();,\"\n",*(d - 1)) || strchr("*/%+-!=<>&|^{}[]();,\"\n",*(s + 1)))) s++;
			else *d++ = *s++;
		}
	}
	*d = '\0';
	
	return data;
}

/*****************************************************************************/
/*                                                                           */
/* object recognition                                                        */
/*                                                                           */
/*****************************************************************************/

/*
 */
int Cit::is_builtin_type(const char *name) {
    if(name[0]=='v')
    {
        if(!strcmp(name,"void")) return 1;
    	if(!strcmp(name,"vec3")) return 1;
	    if(!strcmp(name,"vec4")) return 1;
    }
    else if(name[0]=='i')
    {
		if(!strcmp(name,"int")) return 1;
    }
    else if(name[0]=='h')
    {
        if(!strcmp(name,"handle")) return 1;
    }
    else if(name[0]=='f')
    {
	    if(!strcmp(name,"float")) return 1;
    }
    else if(name[0]=='m')
    {
    	if(!strcmp(name,"mat4")) return 1;
    }
    else if(name[0]=='s')
    {
	    if(!strcmp(name,"string")) return 1;
    }
	return 0;
}

/*
 */
int Cit::is_variable(const char *name,Func *function) {
	
	map<string,const Type*>::iterator it;
	
	// find in function namespace
	if(function) {
		it = function->variables.find(name);
		if(it != function->variables.end()) return 1;
	}
	
	// find in global namespace
	it = global_variables.find(name);
	if(it != global_variables.end()) return 1;
	
	return 0;
}

/*
 */
int Cit::is_function(const char *name) {
	
	// user defined functions
	map<string,Func*>::iterator function_it = functions.find(name);
	if(function_it != functions.end()) return 1;
	
	// extern functions
	map<string,InFooBase*>::iterator extern_function_it = extern_functions.find(name);
	if(extern_function_it != extern_functions.end()) return 1;
	
	return 0;
}

/*
 */
int Cit::is_extern_variable(const char *name) {
	map<string,InExternVarBase*>::iterator it = extern_variables.find(name);
	if(it != extern_variables.end()) return 1;
	return 0;
}

/*
 */
int Cit::is_extern_class(const char *name) {
	map<string,InExternClassbase*>::iterator it = extern_classes.find(name);
	if(it != extern_classes.end()) return 1;
	return 0;
}

/*
 */
int Cit::is_extern_class_object(const char *name) {
	map<string,InExternClassbase*>::iterator it;
	for(it = extern_classes.begin(); it != extern_classes.end(); it++) {
		if(it->second->is_object(name)) return 1;
	}
	return 0;
}

/*****************************************************************************/
/*                                                                           */
/* variables                                                                 */
/*                                                                           */
/*****************************************************************************/

/* add variables
 */
void Cit::add_variable(const char *type,const char *args,Func *function) {
	
	int t = 0;
	if(!strcmp(type,"int")) t = Type::Int;
    else if(!strcmp(type,"handle")) t = Type::Int;
	else if(!strcmp(type,"float")) t = Type::Float;
	else if(!strcmp(type,"vec3")) t = Type::Vec3;
	else if(!strcmp(type,"vec4")) t = Type::Vec4;
	else if(!strcmp(type,"mat4")) t = Type::Mat4;
	else if(!strcmp(type,"string")) t = Type::String;
	else throw(error("add_variable(): unknown variable type \"%s\"\n",type));
	
	while(1) {
		char name[1024];
		args += read_block(args,name,sizeof(name),0,',');
		
		const Type *v = NULL;
		
		// array
		if(strchr(name,'[')) {
			char exp[1024];
			char *s = strchr(name,'[');
			if(s == NULL) throw(error("add_variable(): can't find '[' symbol\n"));
			read_block(s,exp,sizeof(exp),'[',']');
			*s = '\0';
			v = &Type::makeArray(t,(int)expression(exp,function));
		}
		// assignment
		else if(strchr(name,'=')) {
			char exp[1024];
			char *s = strchr(name,'=');
			if(s == NULL) throw(error("add_variable(): can't find '=' symbol\n"));
			*s = '\0';
			strcpy(exp,s + 1);
			
			if(t == Type::Int) v = &Type::makeInt(expression(exp,function));
			else if(t == Type::Float) v = &Type::makeFloat(expression(exp,function));
			else if(t == Type::Vec3) v = &Type::makeVec3(expression(exp,function));
			else if(t == Type::Vec4) v = &Type::makeVec4(expression(exp,function));
			else if(t == Type::Mat4) v = &Type::makeMat4(expression(exp,function));
			else if(t == Type::String) v = &Type::makeString(expression(exp,function));
		}
		// dummy constructor
		else {
			if(t == Type::Int) v = &Type::makeInt(0);
			else if(t == Type::Float) v = &Type::makeFloat(0.0f);
			else if(t == Type::Vec3) v = &Type::makeVec3(vec3());
			else if(t == Type::Vec4) v = &Type::makeVec4(vec4());
			else if(t == Type::Mat4) v = &Type::makeMat4(mat4());
			else if(t == Type::String) v = &Type::makeString("");
		}
		
		Type **var = get_variable(name,function);
		if(var) {
			if(function) {
				Type **global_var = get_variable(name,NULL);
				if(global_var == NULL || *global_var != *var) {
					function->old_variables[name].push_back(v);		// store variable
					*var = (Type*)v;	// override varaible
				} else {
					function->variables[name] = v;
				}
			} else {
				::delete *var;	// global variables uses standart memory managment
				*var = (Type*)&Type::makeExtern(*v);
			}
		} else {
			if(function) function->variables[name] = v;
			else global_variables[name] = &Type::makeExtern(*v);
		}
		
		if(*args == '\0') break;
	}
}

/* remove variables
 */
void Cit::remove_variable(const char *args,Func *function) {
	
	if(function == NULL) throw(error("remove_variable(): can't remove global variable\n"));
	
	while(1) {
		char name[1024];
		args += read_block(args,name,sizeof(name),0,',');
		
		if(strchr(name,'=')) {
			char exp[1024];
			char *s = strchr(name,'=');
			*s = '\0';
			strcpy(exp,s + 1);
		}
		
		map<string,const Type*>::iterator it;
		it = function->variables.find(name);
		if(it != function->variables.end()) {
			function->variables.erase(it);
			
			// restore overrided variables
			map<string,vector<const Type*> >::iterator old_it;
			old_it = function->old_variables.find(name);
			if(old_it != function->old_variables.end()) {
				function->variables[name] = *(old_it->second.end() - 1);
				old_it->second.pop_back();
				if(old_it->second.size() == 0) {
					function->old_variables.erase(old_it);
				}
			}
		}
		
		if(*args == '\0') break;
	}
}

/* get variable by name
 */
Type **Cit::get_variable(const char *name,Func *function) {
	
	map<string,const Type*>::iterator it;
	
	// find in function namespace
	if(function) {
		it = function->variables.find(name);
		if(it != function->variables.end()) return (Type**)&it->second;
	}
	
	// find in global namespace
	it = global_variables.find(name);
	if(it != global_variables.end()) return (Type**)&it->second;
	
	return NULL;
}

/*
 */
const Type *Cit::get_extern_variable(const char *name,Func *function) {
	
	map<string,InExternVarBase*>::iterator it = extern_variables.find(name);
	if(it != extern_variables.end()) return it->second->get_variable();
	
	return NULL;
}

/*****************************************************************************/
/*                                                                           */
/* functions                                                                 */
/*                                                                           */
/*****************************************************************************/

/* Run function
 */
const Type *Cit::run_function(const char *name,const char *args,Func *function) {
	
	// find in user defined functions
	map<string,Func*>::iterator function_it = functions.find(name);
	if(function_it != functions.end()) {
		
		Func *func = function_it->second;
		
		// set parameters
		if(args && function) {
			const char *s = func->args;
			while(1) {
				char buf[1024];
				char exp[1024];
				s += read_block(s,buf,sizeof(buf),0,',');
				args += read_block(args,exp,sizeof(exp),0,',');
				
				if(buf[0] == '\0' && exp[0] == '\0') break;
				if(buf[0] == '\0' || exp[0] == '\0') throw(error("run_function(): bad arguments for \"%s\" function\n",name));
				
				char type[1024];
				char name[1024];
				int length = read_name(buf,type,sizeof(type));
				read_name(buf + length,name,sizeof(name));
				
				if(!strcmp(type,"int")) func->variables[name] = &Type::makeInt(expression(exp,function));
                else if(!strcmp(type,"handle")) func->variables[name] = &Type::makeInt(expression(exp,function));
				else if(!strcmp(type,"float")) func->variables[name] = &Type::makeFloat(expression(exp,function));
				else if(!strcmp(type,"vec3")) func->variables[name] = &Type::makeVec3(expression(exp,function));
				else if(!strcmp(type,"vec4")) func->variables[name] = &Type::makeVec4(expression(exp,function));
				else if(!strcmp(type,"mat4")) func->variables[name] = &Type::makeMat4(expression(exp,function));
				else if(!strcmp(type,"string")) func->variables[name] = &Type::makeString(expression(exp,function));
				else if(is_extern_class(type)) {
					InExternClassbase *extern_class = get_extern_class_by_type(type);
					extern_class->assignment(name,expression(exp,function));
				}
				else throw(error("run_function(): unknown argument type \"%s\"\n",type));
			}
		}
		
		// parse body
		parse(func->body,func);
		
		// delete varibales
		map<string,const Type*>::iterator variable_it;
		for(variable_it = func->variables.begin(); variable_it != func->variables.end(); variable_it++) {
			delete variable_it->second;
		}
		func->variables.clear();
		
		return return_value;
	}
	
	// extern functions
	map<string,InFooBase*>::iterator extern_function_it = extern_functions.find(name);
	if(extern_function_it != extern_functions.end()) {
		
		InFooBase *func = extern_function_it->second;
		
		return_value = NULL;
		
		func->Run(this,args,function);
		
		const Type *ret = return_value;
		
		return_value = NULL;
		
		return ret;
	}
	
	// builtin types
	if(!strcmp(name,"int")) return &Type::makeInt((int)expression(args,function));
    if(!strcmp(name,"handle")) return &Type::makeInt((int)expression(args,function));
	if(!strcmp(name,"float")) return &Type::makeFloat((float)expression(args,function));
	if(!strcmp(name,"vec3")) {
		char buf[3][1024];
		char *a[3] = { buf[0], buf[1], buf[2] };
		read_args(args,a,sizeof(a) / sizeof(a[0]));
		return &Type::makeVec3(vec3(expression(a[0],function),expression(a[1],function),expression(a[2],function)));
	}
	if(!strcmp(name,"vec4")) {
		char buf[4][1024];
		char *a[4] = { buf[0], buf[1], buf[2], buf[3] };
		read_args(args,a,sizeof(a) / sizeof(a[0]));
		return &Type::makeVec4(vec4(expression(a[0],function),expression(a[1],function),expression(a[2],function),expression(a[3],function)));
	}
	if(!strcmp(name,"mat4")) {
		char arg_0[1024];
		char arg_1[1024];
		char arg_2[1024];
		char arg_3[1024];
		args += read_block(args,arg_0,sizeof(arg_0),0,',');
		args += read_block(args,arg_1,sizeof(arg_1),0,',');
		args += read_block(args,arg_2,sizeof(arg_2),0,',');
		if(read_char(args) != '\0') {
			args += read_block(args,arg_3,sizeof(arg_3),0,',');
			return &Type::makeMat4(mat4(expression(arg_0,function),expression(arg_1,function),expression(arg_2,function),expression(arg_3,function)));
		} else {
			return &Type::makeMat4(mat4(expression(arg_0,function),expression(arg_1,function),expression(arg_2,function)));
		}
	}
	if(!strcmp(name,"string")) return &Type::makeString((const char*)expression(args,function));
	
	// builtin functions
	if(!strcmp(name,"sin")) return &sin(expression(args,function));
	if(!strcmp(name,"asin")) return &asin(expression(args,function));
	if(!strcmp(name,"cos")) return &cos(expression(args,function));
	if(!strcmp(name,"acos")) return &acos(expression(args,function));
	if(!strcmp(name,"tan")) return &tan(expression(args,function));
	if(!strcmp(name,"atan")) return &atan(expression(args,function));
	
	if(!strcmp(name,"log")) return &log(expression(args,function));
	if(!strcmp(name,"exp")) return &exp(expression(args,function));
	
	if(!strcmp(name,"sqrt")) return &sqrt(expression(args,function));
	if(!strcmp(name,"rsq")) return &rsq(expression(args,function));
	if(!strcmp(name,"rcp")) return &rcp(expression(args,function));
	
	if(!strcmp(name,"fabs")) return &fabs(expression(args,function));
	if(!strcmp(name,"saturate")) return &saturate(expression(args,function));
	
	if(!strcmp(name,"pow")) {
		char buf[2][1024];
		char *a[2] = { buf[0], buf[1] };
		read_args(args,a,sizeof(a) / sizeof(a[0]));
		return &pow(expression(a[0],function),(float)expression(a[1],function));
	}
	
	//if(!strcmp(name,"length")) return &length(expression(args,function));
	//if(!strcmp(name,"normalize")) return &normalize(expression(args,function));
    /*
	if(!strcmp(name,"dot")) {
		char buf[2][1024];
		char *a[2] = { buf[0], buf[1] };
		read_args(args,a,sizeof(a) / sizeof(a[0]));
		return &dot(expression(a[0],function),expression(a[1],function));
	}
	if(!strcmp(name,"cross")) {
		char buf[2][1024];
		char *a[2] = { buf[0], buf[1] };
		read_args(args,a,sizeof(a) / sizeof(a[0]));
		return &cross(expression(a[0],function),expression(a[1],function));
	}
	
	if(!strcmp(name,"rotate")) {
		char buf[2][1024];
		char *a[2] = { buf[0], buf[1] };
		read_args(args,a,sizeof(a) / sizeof(a[0]));
		return &rotate(expression(a[0],function),expression(a[1],function));
	}
	
	if(!strcmp(name,"rotation")) return &rotation(expression(args,function));
	if(!strcmp(name,"transpose")) return &transpose(expression(args,function));
	if(!strcmp(name,"inverse")) return &inverse(expression(args,function));
	if(!strcmp(name,"scale")) return &scale(expression(args,function));
	if(!strcmp(name,"translate")) return &translate(expression(args,function));
	if(!strcmp(name,"reflect")) return &reflect(expression(args,function));
	if(!strcmp(name,"perspective")) {
		char buf[4][1024];
		char *a[4] = { buf[0], buf[1], buf[2], buf[3] };
		read_args(args,a,sizeof(a) / sizeof(a[0]));
		return &perspective(expression(a[0],function),expression(a[1],function),expression(a[2],function),expression(a[3],function));
	}
	if(!strcmp(name,"look_at")) {
		char buf[3][1024];
		char *a[3] = { buf[0], buf[1], buf[3] };
		read_args(args,a,sizeof(a) / sizeof(a[0]));
		return &look_at(expression(a[0],function),expression(a[1],function),expression(a[2],function));
	}
	*/
	if(!strcmp(name,"strstr")) {
		char buf[2][1024];
		char *a[2] = { buf[0], buf[1] };
		read_args(args,a,sizeof(a) / sizeof(a[0]));
		return &strstr(expression(a[0],function),expression(a[1],function));
	}
	if(!strcmp(name,"strlen")) return &strlen(expression(args,function));
	
	if(!strcmp(name,"c_array")) return &c_array(expression(args,function));
	
	if(!strcmp(name,"rand")) return &Type::makeFloat(((float)rand() / (float)RAND_MAX - 0.5f) * 2.0f * (float)expression(args,function));
	
	throw(error("run_function(): unknown function \"%s\"\n",name));
}

/*****************************************************************************/
/*                                                                           */
/* extern classes                                                            */
/*                                                                           */
/*****************************************************************************/

/*
 */
InExternClassbase *Cit::get_extern_class_by_type(const char *name) {
	map<string,InExternClassbase*>::iterator it = extern_classes.find(name);
	if(it != extern_classes.end()) return it->second;
	throw(error("get_extern_class_by_name(): can't find \"%s\" extern class\n",name));
}

/*
 */
InExternClassbase *Cit::get_extern_class_by_name(const char *name) {
	map<string,InExternClassbase*>::iterator it;
	for(it = extern_classes.begin(); it != extern_classes.end(); it++) {
		if(it->second->is_object(name)) return it->second;
	}
	throw(error("get_extern_class_by_name(): can't find extern class for \"%s\"\n",name));
}

/*
 */
const char *Cit::get_extern_class_type_by_name(const char *name) {
	static char type[1024];
	map<string,InExternClassbase*>::iterator it;
	for(it = extern_classes.begin(); it != extern_classes.end(); it++) {
		if(it->second->is_object(name)) {
			strncpy(type,it->first.c_str(),sizeof(type));
			return type;
		}
	}
	throw(error("get_extern_class_type_by_name(): can't find extern class for \"%s\"\n",name));
}

/*
 */
const Type *Cit::run_extern_class_function(const char *name,const char *func,const char *args,Func *function) {
	
	return_value = NULL;
	
	InExternClassbase *extern_class = get_extern_class_by_name(name);
	extern_class->Run(this,name,func,args,function);
	
	const Type *ret = return_value;
	return_value = NULL;
	
	return ret;
}

/*****************************************************************************/
/*                                                                           */
/* evaluate expressions                                                      */
/*                                                                           */
/*****************************************************************************/

/*
 */
int Cit::priority(int op) {
	if(strchr("*/%",op)) return 5;
	if(strchr("&|",op)) return 4;
	if(strchr("+-",op)) return 3;
	if(strchr("<>=!",op)) return 2;
	if(strchr("AO",op)) return 1;
	return 0;
}

/* evaluate expression
 */
const Type &Cit::expression(const char *src,Func *function) {
	
	struct Stack {
		int op;
		const Type *var;
	};
	
	Stack *stack = new Stack[1024];
	int stack_depth = 0;
	
	int *stack_op = new int[1024];
	int stack_op_depth = 0;
	
	int unary_op = 0;
	
	const char *s = src;
	while(*s) {
		if(strchr(" \t\n",*s)) {
			s++;
		}
		else if(*s == '(') {
			stack_op[stack_op_depth++] = *s++;
			unary_op = 0;
		}
		else if(*s == ')') {
			while(stack_op_depth > 0 && stack_op[stack_op_depth - 1] != '(') {
				stack[stack_depth++].op = stack_op[stack_op_depth - 1];
				stack_op_depth--;
			}
			if(stack_op_depth > 0) stack_op_depth--;
			s++;
		}
		// binary operations
		else if(!strncmp("==",s,2)) {
			while(stack_op_depth > 0 && priority('=') <= priority(stack_op[stack_op_depth - 1])) {
				stack[stack_depth++].op = stack_op[--stack_op_depth];
			}
			stack_op[stack_op_depth++] = '=';
			s += 2;
		}
		else if(!strncmp("!=",s,2)) {
			while(stack_op_depth > 0 && priority('!') <= priority(stack_op[stack_op_depth - 1])) {
				stack[stack_depth++].op = stack_op[--stack_op_depth];
			}
			stack_op[stack_op_depth++] = '!';
			s += 2;
		}
		else if(!strncmp("&&",s,2)) {
			while(stack_op_depth > 0 && priority('A') <= priority(stack_op[stack_op_depth - 1])) {
				stack[stack_depth++].op = stack_op[--stack_op_depth];
			}
			stack_op[stack_op_depth++] = 'A';
			s += 2;
		}
		else if(!strncmp("||",s,2)) {
			while(stack_op_depth > 0 && priority('O') <= priority(stack_op[stack_op_depth - 1])) {
				stack[stack_depth++].op = stack_op[--stack_op_depth];
			}
			stack_op[stack_op_depth++] = 'O';
			s += 2;
		}
		else if(strchr("+-!*/%&|<>",*s)) {
			if(unary_op == 0) {
				if(*s == '+') unary_op = -1;
				else if(*s == '-') unary_op = '-';
				else if(*s == '!') unary_op = '!';
				else throw(error("expression(): bad unary operation '%c'\n",*s));
				s++;
			} else {
				while(stack_op_depth > 0 && priority(*s) <= priority(stack_op[stack_op_depth - 1])) {
					stack[stack_depth++].op = stack_op[--stack_op_depth];
				}
				stack_op[stack_op_depth++] = *s;
				if(strchr("<>",*s)) unary_op = 0;
				s++;
			}
		}
		// digits
		else if(strchr("1234567890.",*s)) {
			char buf[1024];
			s += read_name(s,buf,sizeof(buf),"1234567890.fFxXaAbBcCdDeEfF");
			
			stack[stack_depth].op = 'v';
			if(strchr(buf,'.')) {
				stack[stack_depth++].var = &Type::makeFloat((float)atof(buf));
			} else {
				int value;
				if(buf[0] == '0' && (buf[1] == 'x' || buf[1] == 'X')) sscanf(buf + 2,"%x",&value);
				else value = atoi(buf);
				stack[stack_depth++].var = &Type::makeInt(value);
			}
			
			if(unary_op == '-') stack[stack_depth - 1].var = &(*stack[stack_depth - 1].var * Vint(-1));
			else if(unary_op == '!') stack[stack_depth - 1].var = &(Type::makeInt(!(int)*stack[stack_depth - 1].var));
			unary_op = -1;
		}
		// chars
		else if(*s == '\'' && *(s + 2) == '\'') {
			stack[stack_depth].op = 'v';
			stack[stack_depth++].var = &Type::makeInt(*(s + 1));
			s += 3;
		}
		// strings
		else if(*s == '"') {
			char buf[1024];
			s += read_block(s,buf,sizeof(buf),'"','"');
			
			stack[stack_depth].op = 'v';
			stack[stack_depth++].var = &Type::makeString(buf);
			unary_op = -1;
		}
		// functions and variables
		else if(isalpha(*s) || *s=='_') {
			char name[1024];
			s += read_name(s,name,sizeof(name));
			
			stack[stack_depth].op = 'v';
			
			// array
			int index = -1;
			if(*s == '[') {
				char buf[1024];
				s += read_block(s,buf,sizeof(buf),'[',']');
				index = (int)expression(buf,function);
				if(index < 0) throw(error("parse(): bad array index %d\n",index));
			}
			
			// function
			if(*s == '(') {
				char args[1024];
				s += read_block(s,args,sizeof(args),'(',')');
				const Type *var = run_function(name,args,function);
				if(var == NULL) throw(error("expression(): \"%s\" function return NULL\n",name));
				stack[stack_depth++].var = var;
			}
			// class function
			else if(*s == '.') {
				if(is_extern_class_object(name)) {
					char func[128];
					char args[1024];
					s += read_name(s + 1,func,sizeof(func)) + 1;
					s += read_block(s,args,sizeof(args),'(',')');
					const Type *var = run_extern_class_function(name,func,args,function);
					if(var == NULL) throw(error("expression(): \"%s.%s\" function return NULL\n",name,func));
					stack[stack_depth++].var = var;
				} else {	// swizzle
					const Type *var = NULL;
					Type **variable = get_variable(name,function);
					if(variable) var = *variable;
					if(var == NULL) {
						var = get_extern_variable(name,function);
						if(var == NULL) throw(error("expression(): \"%s\" isn't class object or variable\n",name));
					}
					if(index >= 0) var = *(var->get(index));
					s++;
					float value = 0;
					if(var->type == Type::Vec3) {
						vec3 v = *var;
						if(*s == 'x') value = v.x;
						else if(*s == 'y') value = v.y;
						else if(*s == 'z') value = v.z;
						else throw(error("expression(): bad swizzle '%c' for vec3 type\n",*s));
						stack[stack_depth++].var = &Type::makeFloat(value);
					} else if(var->type == Type::Vec4) {
						vec4 v = *var;
						if(*s == 'x') value = v.x;
						else if(*s == 'y') value = v.y;
						else if(*s == 'z') value = v.z;
						else if(*s == 'w') value = v.w;
						else throw(error("expression(): bad swizzle '%c' for vec4 type\n",*s));
						stack[stack_depth++].var = &Type::makeFloat(value);
					} else {
						throw(error("expression(): can't use swizzle with \"%s\" variable\n",name));
					}
					s++;
				}
			}
			// variable
			else {
				if(is_variable(name,function)) {
					Type **var = get_variable(name,function);
					if(index >= 0) var = (*var)->get(index);
					stack[stack_depth++].var = *var;
				} else if(is_extern_variable(name)) {
					stack[stack_depth++].var = get_extern_variable(name,function);
				} else if(is_extern_class_object(name)) {
					InExternClassbase *extern_class = get_extern_class_by_name(name);
					stack[stack_depth++].var = &Type::makePointer(extern_class->get_object(name),extern_class->get_type_info());
				} else {
					throw(error("expression(): can't find \"%s\" variable\n",name));
				}
			}
			
			if(unary_op == '-') stack[stack_depth - 1].var = &(*stack[stack_depth - 1].var * Vint(-1));
			else if(unary_op == '!') stack[stack_depth - 1].var = &(Type::makeInt(!(int)*stack[stack_depth - 1].var));
			unary_op = -1;
		}
		else {
			delete [] stack_op;
			delete [] stack;
			throw(error("expression(): unknown token \"%c\" in \"%s\" expression\n",*s,src));
		}
	}
	
	// add operations to the end
	while(stack_op_depth) {
		stack[stack_depth++].op = stack_op[stack_op_depth - 1];
		stack_op_depth--;
	}
	
	// calculate value
	for(int tries = 0; tries < 1024; tries++) {
	
		int arg_0 = -1;
		int arg_1 = -1;
		int depth = 0;
		
		for(int i = 0; i < stack_depth; i++) {
			if(stack[i].op == 0) continue;
			depth++;
			
			if(arg_0 != -1 && arg_1 != -1 && strchr("*/%+-&|<>=!AO",stack[i].op)) {
				if(stack[i].op == '*') stack[i].var = &(*stack[arg_0].var * *stack[arg_1].var);
				else if(stack[i].op == '/') stack[i].var = &(*stack[arg_0].var / *stack[arg_1].var);
				else if(stack[i].op == '%') stack[i].var = &Type::makeInt((int)*stack[arg_0].var % (int)*stack[arg_1].var);
				else if(stack[i].op == '+') stack[i].var = &(*stack[arg_0].var + *stack[arg_1].var);
				else if(stack[i].op == '-') stack[i].var = &(*stack[arg_0].var - *stack[arg_1].var);
				else if(stack[i].op == '&') stack[i].var = &Type::makeInt((int)*stack[arg_0].var & (int)*stack[arg_1].var);
				else if(stack[i].op == '|') stack[i].var = &Type::makeInt((int)*stack[arg_0].var | (int)*stack[arg_1].var);
				else if(stack[i].op == '<') stack[i].var = &Type::makeInt((float)*stack[arg_0].var < (float)*stack[arg_1].var);
				else if(stack[i].op == '>') stack[i].var = &Type::makeInt((float)*stack[arg_0].var > (float)*stack[arg_1].var);
				else if(stack[i].op == '=') stack[i].var = &(*stack[arg_0].var == *stack[arg_1].var);
				else if(stack[i].op == '!') stack[i].var = &(*stack[arg_0].var != *stack[arg_1].var);
				else if(stack[i].op == 'A') stack[i].var = &Type::makeInt((int)*stack[arg_0].var && (int)*stack[arg_1].var);
				else if(stack[i].op == 'O') stack[i].var = &Type::makeInt((int)*stack[arg_0].var || (int)*stack[arg_1].var);
				stack[i].op = 'v';
				stack[arg_0].op = 0;
				stack[arg_1].op = 0;
				depth = 100;
				break;
			}
			else if(stack[i].op == 'v') {
				if(arg_0 == -1) { arg_0 = i; }
				else if(arg_1 == -1) { arg_1 = i; }
				else { arg_0 = arg_1; arg_1 = i; }
			}
			else {
				throw(error("expression(): unknown operation '%c' in \"%s\" expression\n",stack[i].op,src));
			}
		}
		
		if(depth == 1) {
			const Type *v = stack[arg_0].var;
			delete [] stack_op;
			delete [] stack;
			return *v;
		}
	}
	
	delete [] stack_op;
	delete [] stack;
	throw(error("expression(): can't parse \"%s\" expression\n",src));
}

/*****************************************************************************/
/*                                                                           */
/* Load_file                                                                      */
/*                                                                           */
/*****************************************************************************/

/*
 */
void Cit::setDefine(const char *name,const char *value) {
	define(name,value);
}

/*
 */
int Cit::Load_file(const char *name) {
	
	line[0] = '\0';
	
	try {
		
		const char *src = preprocessor(name);
		if(src == NULL) return 0;
		
		const char *s = src;
		while(*s) {
			
			while(*s && strchr(" \n;",*s)) s++;
			
			// read current line
			read_block(s,line,sizeof(line),0,';');
			
			char type[128];
			s += read_name(s,type,sizeof(type));
			if(type[0] == '\0') break;
			
			if(is_builtin_type(type) || is_extern_class(type)) {
				
				char d = read_char(s,";{");
				
				// global variables
				if(d == ';') {
					if(is_builtin_type(type)) {	// builtin type
						char args[1024];
						s += read_block(s,args,sizeof(args),0,';');
						add_variable(type,args,NULL);
					} else {	// extern class
						char name[128];
						char func[128];
						char args[1024];
						s += read_name(s,name,sizeof(name));
						char c = read_char(s);
						if(c == ';') {
							// null constructor
							InExternClassbase *extern_class = get_extern_class_by_type(type);
							extern_class->null_constructor(name);
						} else if(c == '=') {
							s += read_name(s,NULL,0,"=");
							s += read_name(s,func,sizeof(func));
							// class constructor
							if(!strcmp(func,"new")) {
								s += read_name(s,func,sizeof(func));
								if(strcmp(func,type)) throw(error("parse(): can't convert \"%s\" to \"%s\" in assignment\n",func,type));
								s += read_block(s,args,sizeof(args),'(',')');
								InExternClassbase *extern_class = get_extern_class_by_type(type);
								extern_class->constructor(this,name,args,NULL);
							}
							// class assignment
							else {
								s += read_block(s,args,sizeof(args),0,';');
								InExternClassbase *extern_class = get_extern_class_by_type(type);
								extern_class->assignment(name,expression(args,NULL));
							}
						} else {
							throw(error("parse(): unknown token '%c'\n",c));
						}
					}
				}
				// function
				else if(d == '{') {
					char name[128];
					char args[1024];
					char *body = new char[strlen(s) + 1];
					
					s += read_name(s,name,sizeof(name));
					s += read_block(s,args,sizeof(args),'(',')');
					s += read_block(s,body,strlen(s) + 1,'{','}');
					
					Func *function = new Func;
					function->type = strdup(type);
					function->args = strdup(args);
					function->body = body;
					
					functions[name] = function;
				}
				else {
					throw(error("Load_file(): can't find ';' or '{' symbol after \"%s\"\n",type));
				}
			}
			else {
				throw(error("Load_file(): unknown token \"%s\"\n",type));
			}
		}
		
		delete [] (char*)src;
	}
	
	catch(const char *error) {
        _lasterror = error;
        _lasterror += line;
		Type::release();
		return 0;
	}
	
	Type::release();
	
	return 1;
}

/*****************************************************************************/
/*                                                                           */
/* parse                                                                     */
/*                                                                           */
/*****************************************************************************/

/*
 */
int Cit::parse_if(const char *src,Func *function) {
	
	const char *s = src;
	
	char buf[1024];
	s += read_block(s,buf,sizeof(buf),'(',')');
	
	if((int)expression(buf,function)) {
		if(read_char(s,";{") == ';') {
			s += read_block(s,buf,sizeof(buf),0,';');
			parse(buf,function);
		} else {
			if(read_char(s) == '{') s += read_name(s,NULL,0,"{");
			s += parse(s,function);
		}
		while(1) {	// skiping all next else/if blocks
			char c = read_char(s);
			if(c != 'e') break;
			read_name(s,buf,sizeof(buf));
			if(strcmp(buf,"else")) break;
			s += read_name(s,NULL,0);
			c = read_char(s);
			if(c == 'i') read_name(s,buf,sizeof(buf));
			if(!strcmp(buf,"if")) {
				read_name(s,NULL,0);
				s += read_block(s,buf,sizeof(buf),'(',')');
				c = read_char(s,";{");
				if(c == ';') s += read_block(s,NULL,0,0,';');
				else s += read_block(s,NULL,0,'{','}');
			} else {
				if(read_char(s,";{") == ';') s += read_block(s,NULL,0,0,';');
				else s += read_block(s,NULL,0,'{','}');
				break;
			}
		}
	} else {
		if(read_char(s,";{") == ';') s += read_block(s,NULL,0,0,';');
		else s += read_block(s,NULL,0,'{','}');
	}
	
	return s - src;
}

/*
 */
int Cit::parse_for(const char *src,Func *function) {
	
	const char *s = src;
	
	char args[1024];
	char buf[1024];
	char condition[1024];
	char addition[1024];
	
	s += read_block(s,args,sizeof(args),'(',')');
	const char *a = args;
	
	a += read_block(a,buf,sizeof(buf),0,';');
	parse(buf,function);
	
	a += read_block(a,condition,sizeof(condition),0,';');
	a += read_block(a,addition,sizeof(addition),0,';');
	
	const char *begin = s;
	while((int)expression(condition,function)) {
		s = begin;
		if(read_char(s,";{") == ';') {
			s += read_block(s,buf,sizeof(buf),0,';');
			parse(buf,function);
		} else {
			s += read_name(s,NULL,0,"{");
			s += parse(s,function);
		}
		if(break_flag) {
			s = begin;
			if(read_char(s,";{") == ';') s += read_block(s,NULL,0,0,';');
			else s += read_block(s,NULL,0,'{','}');
			break;
		}
		parse(addition,function);
	}
	
	return s - src;
}

/*
 */
int Cit::parse_while(const char *src,Func *function) {
	
	const char *s = src;
	
	char buf[1024];
	char condition[1024];
	
	s += read_block(s,condition,sizeof(condition),'(',')');
	
	const char *begin = s;
	while((int)expression(condition,function)) {
		s = begin;
		if(read_char(s,";{") == ';') {
			s += read_block(s,buf,sizeof(buf),0,';');
			parse(buf,function);
		} else {
			s += read_name(s,NULL,0,"{");
			s += parse(s,function);
		}
		if(break_flag) {
			s = begin;
			if(read_char(s,";{") == ';') s += read_block(s,NULL,0,0,';');
			else s += read_block(s,NULL,0,'{','}');
			break;
		}
	}
	
	return s - src;
}

/*
 */
int Cit::parse(const char *src,Func *function) {
	
	const char *s = src;
	
	break_flag = 0;
	return_value = NULL;
	
	vector<string> variables;
	
	while(*s) {
		
		while(*s && strchr(" \n",*s)) s++;
		
		if(*s == ';') {
			s++;
			continue;
		} else if(*s == '{') {
			s++;
			s += parse(s,function);
		} else if(*s == '}') {
			s++;
			for(int i = 0; i < (int)variables.size(); i++) {
				remove_variable(variables[i].c_str(),function);
			}
			break;
		}
		
		while(*s && strchr(" \n",*s)) s++;
		
		// read current line
		read_block(s,line,sizeof(line),0,';');
		
		// read first token
		char buf[128];
		s += read_name(s,buf,sizeof(buf));
		if(buf[0] == '\0') break;
		
		// new variable
		if(is_builtin_type(buf)) {
			char args[1024];
			s += read_block(s,args,sizeof(args),0,';');
			add_variable(buf,args,function);
			variables.push_back(args);
		}
		// swizzle or operator=
		else if(is_variable(buf,function)) {
			char exp[1024];
			s += read_block(s,exp,sizeof(exp),0,';');
			const char *e = exp;
			// array
			int index = -1;
			if(*e == '[') {
				char index_buf[1024];
				e += read_block(e,index_buf,sizeof(index_buf),'[',']');
				index = (int)expression(index_buf,function);
				if(index < 0) throw(error("parse(): bad array index %d\n",index));
			}
			// swizzle
			if(*e == '.') {
				Type **var = get_variable(buf,function);
				if(var == NULL) throw(error("parse(): can't find \"%s\" variable\n",buf));
				if(index >= 0) var = (*var)->get(index);
				e++;
				float *value = NULL;
				if((*var)->type == Type::Vec3) {
					Vvec3 *v = reinterpret_cast<Vvec3*>(*var);
					if(*e == 'x') value = &v->value.x;
					else if(*e == 'y') value = &v->value.y;
					else if(*e == 'z') value = &v->value.z;
					else throw(error("parse(): bad swizzle '%c' for vec3 type\n",*e));
				} else if((*var)->type == Type::Vec4) {
					Vvec4 *v = reinterpret_cast<Vvec4*>(*var);
					if(*e == 'x') value = &v->value.x;
					else if(*e == 'y') value = &v->value.y;
					else if(*e == 'z') value = &v->value.z;
					else if(*e == 'w') value = &v->value.w;
					else throw(error("parse(): bad swizzle '%c' for vec4 type\n",*e));
				} else {
					throw(error("expression(): can't use swizzle with \"%s\" variable\n",buf));
				}
				e++;
				if(!strncmp(e,"=",1)) *value = expression(e + 1,function);
				else if(!strncmp(e,"++",2)) (*value) += 1.0f;
				else if(!strncmp(e,"--",2)) (*value) -= 1.0f;
				else if(!strncmp(e,"*=",2)) *value *= (float)expression(e + 2,function);
				else if(!strncmp(e,"/=",2)) *value /= (float)expression(e + 2,function);
				else if(!strncmp(e,"+=",2)) *value += (float)expression(e + 2,function);
				else if(!strncmp(e,"-=",2)) *value -= (float)expression(e + 2,function);
				else throw(error("parse(): unknown operation \"%s\"\n",e));
			}
			// operator=
			else if(strchr("=*/+-",*e)) {
				Type **var = get_variable(buf,NULL);
				Type *global_var = (var != NULL) ? *var : NULL;
				var = get_variable(buf,function);
				if(var == NULL) throw(error("parse(): can't find \"%s\" variable\n",buf));
				if(index < 0 && (*var)->type == Type::Array) throw(error("parse(): can't access to array without []\n"));
				if(index >= 0) var = (*var)->get(index);
				if(global_var != *var) global_var = NULL;
				if(!strncmp(e,"=",1)) *var = (Type*)&expression(e + 1,function);
				else if(!strncmp(e,"++",2)) *var = (Type*)&(**var + Type::makeInt(1));
				else if(!strncmp(e,"--",2)) *var = (Type*)&(**var - Type::makeInt(1));
				else if(!strncmp(e,"*=",2)) *var = (Type*)&(**var * expression(e + 2,function));
				else if(!strncmp(e,"/=",2)) *var = (Type*)&(**var / expression(e + 2,function));
				else if(!strncmp(e,"+=",2)) *var = (Type*)&(**var + expression(e + 2,function));
				else if(!strncmp(e,"-=",2)) *var = (Type*)&(**var - expression(e + 2,function));
				else throw(error("parse(): unknown operation \"%s\"\n",e));
				// remove global variable and recreate it
				if(global_var) {
					::delete global_var;
					*var = (Type*)&Type::makeExtern(**var);
				}
			} else {
				throw(error("parse(): unknown token '%c'\n",*e));
			}
		}
		// extern variable
		else if(is_extern_variable(buf)) {
			throw(error("parse(): \"%s\" is read only variable\n",buf));
		}
		// function
		else if(is_function(buf)) {
			char args[1024];
			s += read_block(s,args,sizeof(args),'(',')');
			run_function(buf,args,function);
		}
		// class constructor or class appropriate
		else if(is_extern_class(buf)) {
			char name[128];
			char func[128];
			char args[1024];
			s += read_name(s,name,sizeof(name));
			char c = read_char(s);
			if(c == ';') {
				// null constructor
				InExternClassbase *extern_class = get_extern_class_by_type(buf);
				extern_class->null_constructor(name);
			} else if(c == '=') {
				s += read_name(s,NULL,0,"=");
				s += read_name(s,func,sizeof(func));
				// class constructor
				if(!strcmp(func,"new")) {
					s += read_name(s,func,sizeof(func));
					if(strcmp(func,buf)) throw(error("parse(): can't convert \"%s\" to \"%s\" in assignment\n",func,buf));
					s += read_block(s,args,sizeof(args),'(',')');
					InExternClassbase *extern_class = get_extern_class_by_type(buf);
					extern_class->constructor(this,name,args,function);
				}
				// class assignment
				else {
					s += read_block(s,args,sizeof(args),0,';');
					InExternClassbase *extern_class = get_extern_class_by_type(buf);
					extern_class->assignment(name,expression(args,function));
				}
			} else {
				throw(error("parse(): unknown token '%c'\n",c));
			}
		}
		// class function or class appropriate
		else if(is_extern_class_object(buf)) {
			char func[128];
			char args[1024];
			char c = read_char(s);
			// class function
			if(c == '.') {
				s += read_name(s + 1,func,sizeof(func)) + 1;
				s += read_block(s,args,sizeof(args),'(',')');
				InExternClassbase *extern_class = get_extern_class_by_name(buf);
				extern_class->Run(this,buf,func,args,function);
			}
			// class constructor or class ret function
			else if(c == '=') {
				s += read_name(s,NULL,0,"=");
				read_name(s,func,sizeof(func));
				// constructor
				if(!strcmp(func,"new")) {
					s += read_name(s,func,sizeof(func));	// new
					s += read_name(s,func,sizeof(func));	// class constructor
					if(strcmp(func,get_extern_class_type_by_name(buf))) throw(error("parse(): can't convert \"%s\" to \"%s\" in assignment\n",func,get_extern_class_type_by_name(buf)));
					s += read_block(s,args,sizeof(args),'(',')');
					InExternClassbase *extern_class = get_extern_class_by_name(buf);
					extern_class->constructor(this,buf,args,function);
				}
				// class assignment
				else {
					s += read_block(s,args,sizeof(args),0,';');
					InExternClassbase *extern_class = get_extern_class_by_name(buf);
					extern_class->assignment(buf,expression(args,function));
				}
			}
			else {
				throw(error("parse(): unknown token '%c'\n",*s));
			}
		}
		// class destructor
		else if(!strcmp(buf,"delete")) {
			char name[128];
			s += read_name(s,name,sizeof(name));
			if(is_extern_class_object(name) == 0) throw(error("parse(): can't apply operator delete to \"%\" variable\n",name));
			InExternClassbase *extern_class = get_extern_class_by_name(name);
			extern_class->destructor(name);
		}
		// if
		else if(!strcmp(buf,"if")) {
			s += parse_if(s,function);
		}
		// else
		else if(!strcmp(buf,"else")) {
			// nothing
		}
		// for
		else if(!strcmp(buf,"for")) {
			s += parse_for(s,function);
		}
		// while
		else if(!strcmp(buf,"while")) {
			s += parse_while(s,function);
		}
		// break
		else if(!strcmp(buf,"break")) {
			break_flag = 1;
			break;
		}
		// continue
		else if(!strcmp(buf,"continue")) {
			break;
		}
		// return
		else if(!strcmp(buf,"return")) {
			char exp[1024];
			s += read_block(s,exp,sizeof(exp),0,';');
			return_value = (Type*)&(expression(exp,function));
			break;
		}
		else {
			throw(error("parse(): unknown token \"%s\"\n",buf));
		}
	}
	
	return s - src;
}

/*****************************************************************************/
/*                                                                           */
/* Run                                                                       */
/*                                                                           */
/*****************************************************************************/

/*
 */
