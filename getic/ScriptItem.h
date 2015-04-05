// ScriptItem1.h: interface for the ScriptItem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SCRIPT_ITEM_H__
#define _SCRIPT_ITEM_H__



class Scene;
class SceItem;
class ScriptItem 
{
public:
	ScriptItem(){};
	virtual		~ScriptItem();
    tstring     _name;
	tstring		_script;        
};

#endif // !_SCRIPT_ITEM_H__
