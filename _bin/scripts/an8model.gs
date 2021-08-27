dofile("common.gh");        // data constants and scrutures
dofile("render.gh");        // rende plug data constants
dofile("input.gh");         // input keys
dofile("bsp.gh");           // constants for reading BSP file

context<-{

    htex1=null,
    pct=-60.0,
    an8m = null,
    sys  = null,
    
};



function OnStart(sys,sysdata)
{
    sys.SetHudMode(hud.HUD_TRACE);
    sys.SetMode(0);
    sys.Print(" ");
    sys.Print(" ");
    sys.Print("An8 model view...Press '~' to hide this");
    
    context.sys=sys;
    context.an8m = sys.LoadObject("res/walk.an8","an8loader.dll");
    sys.GetRenderSys().EnableRenderMaterialLighting(0xEEEEEEFF,1);
    return 0;
} 

function OnToggleScreen(scrmode,sysdata)
{
    return 0;
} 

function OnEscape(zero, sysdata)
{
    return 0;
} 

function OnPaint(camera, sysdata)
{
    return 0; 
} 


function OnRender(camera,sysdata)
{
    if(context.an8m!=null)
    {
        context.an8m.Render(sysdata,0);
    }
    return 0;
} 

function OnSpin(camera, sysdata)
{
    if(context.an8m!=null)
    {
        context.an8m.Spin(camera, sysdata);
    }
    return 0;
} 

function OnInput(keystate, keycode)
{
    return 0;
} 
function OnError(errno, desc)
{   
    return 0;
} 

function OnCloseApp(zero,sysdata)
{   
    return 0;
} 

function OnUiCommand(Id, sys)
{
    return 0;
} 

function OnBspRead(btfsectiom, file)
{   
    return 0;
} 

function OnStatusText(unused1, unused2)
{
    return 0;
} 

function OnNetReceive(length, bytes)
{
    return 0;
} 

function OnExit(zero,sysdata)
{
    return 0;
} 


//-----------------------------------------------------------------------------
//leve this include last in the file//
dofile("gsapp.gh");