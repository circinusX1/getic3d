//============================================================================
// Author: Octavian Marius Chincisan 2006 March - 2006 Sept
// Zalsoft Inc 1999-2006
//============================================================================
dofile("common.gh");        // data constants and scrutures
dofile("render.gh");        // rende plug data constants
dofile("input.gh");         // input keys
dofile("bsp.gh");           // constants for reading BSP file


function OnStart(sys,syst,sysdata)
{
    sys.SetHudMode(hud.HUD_TRACE|hud.HUD_HELP);
    sys.Print(" ");
    sys.Print(" ");
    sys.Print("Empty Open GL window");
    return 0;
} 

function OnToggleScreen(sys,syst,sysdata)
{
    return 0;
} 

function OnEscape(sys,zero, sysdata)
{
    return 0;
} 
function OnPhysics(sys,syst, sysdata)
{
    return 0;
}

function OnPaint(sys,camera, sysdata)
{
    return 0;
} 
function OnExit(sys,zero,sysdata)
{
    return 0;
} 
function OnRender(sys,camera,sysdata)
{
    return 0;
} 

function OnSpin(sys,syst, sysdata)
{
    return 0;
} 

function OnInput(keystate, keycode)
{
    return 0;
} 
function OnError(sys,errno, desc)
{   
    return 0;
} 

function OnCloseApp(sys,zero,sysdata)
{   
    return 0;
} 

function OnUiCommand(sys,Id, sys)
{
    return 0;
} 

function OnBspRead(sys,btfsectiom, file)
{   
    return 0;
} 

function OnStatusText(sys,unused1, unused2)
{
    return 0;
} 

function OnNetReceive(sys,length, bytes)
{
    return 0;
} 


//-----------------------------------------------------------------------------
//leve this include last in the file//
dofile("gsapp.gh");