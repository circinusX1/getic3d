// GeticPlugIns.h: interface for the GeticPlugIns class.
//---------------------------------------------------------------------------------------
#ifndef __GETIC_PLUGINS_H__
#define __GETIC_PLUGINS_H__

//---------------------------------------------------------------------------------------
#ifndef interface
    #define interface struct
#endif //
//---------------------------------------------------------------------------------------
// Structures passed between plugins and main application
// 
struct PlVertex
{
};

struct PlPoly
{
};

struct PlBrush
{
};

struct PlTex
{
};

//---------------------------------------------------------------------------------------
// getic main application interface
interface IPlugIn;
interface GeticEditor
{
    int     AddMenuItem(int , int , UINT , IPlugIn* ) = 0;
    void    SaveDocument()              = 0;
    void    StartNewDocument()          = 0;
    void    AddScene(PlBrush**, int )   = 0;
    void    AddPrefab(PlBrush* )        = 0;
};

//---------------------------------------------------------------------------------------
// allows to load different geometry file formats
interface PlugIn
{
    void            FireCommand(UINT cmdID) = 0;
    const THCAR*    GetMenuItemTexxt()      = 0;
};

//---------------------------------------------------------------------------------------
 

#endif // 
