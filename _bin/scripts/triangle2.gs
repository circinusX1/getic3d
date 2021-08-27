//============================================================================
// Author: Octavian Marius Chincisan 2006 March - 2006 Sept
// Zalsoft Inc 1999-2006
//============================================================================
dofile("common.gh");        // data constants and scrutures
dofile("render.gh");        // rende plug data constants
dofile("input.gh");         // input keys
dofile("bsp.gh");           // constants for reading BSP file

context<-{

    htex1=null,
    pct=-60.0,
};



function OnStart(sys,syst,sysdata)
{
    sys.SetHudMode(hud.HUD_TRACE);
    sys.SetMode(0);
    sys.Print(" ");
    sys.Print(" ");
    sys.Print("Triangle...Press '~' to hide this");
    context.htex1  = sys.GenTexFile("res/logo0.jpg", 0);
    return 0;
} 

function OnToggleScreen(sys,scrmode,sysdata)
{
    return 0;
} 

function OnEscape(sys,zero, sysdata)
{
    return 0;
} 

function OnPaint(sys,camera, sysdata)
{
    
    TheApp.render.Color4(rand()%255,rand()%255,rand()%255,255);
    TheApp.render.Push(context.htex1);
    TheApp.render.BindTex1(context.htex1);
    
    local inside = -camera.GetFinderDepth();
    local right  = camera.GetFinderWidth()/2.0;
    local down   = -camera.GetFinderHeight()/2.0;
    
    TheApp.render.Translate(V3(right,  down, inside));
    //TheApp.render.Rotate(v);
    TheApp.render.BeginPrim(rndconst.Z_TRIANGLES);
    
    TheApp.render.Texture2(0.0,0.0);
    TheApp.render.Vertex3(-0.6,-0.6,0.0);
    
    TheApp.render.Texture2(1.0,0.0);
    TheApp.render.Vertex3(0.6,-0.6,0.0);
    
    TheApp.render.Texture2(0.5,1.0);
    TheApp.render.Vertex3(0.0,1.2,0.0);
    TheApp.render.End();
    
    TheApp.render.Pop();
    
    return 0; 
} 


function OnRender(sys,camera,sysdata)
{
    local v = V3(0.0, context.pct,0.0);
    context.pct += 0.8;
    
    TheApp.render.Color4(255,255,255,255);
    TheApp.render.Push(context.htex1);
    TheApp.render.BindTex1(context.htex1);
    
    TheApp.render.Translate(V3(0.0,0.0,-60.0));
    //TheApp.render.Rotate(v);
    TheApp.render.BeginPrim(rndconst.Z_TRIANGLES);
    
    TheApp.render.Texture2(0.0,0.0);
    TheApp.render.Vertex3(-16.0,-16.0,0.0);
    
    TheApp.render.Texture2(1.0,0.0);
    TheApp.render.Vertex3(16.0,-16.0,0.0);
    
    TheApp.render.Texture2(0.5,1.0);
    TheApp.render.Vertex3(0.0,32.0,0.0);
    TheApp.render.End();
    
    TheApp.render.Pop();
    
    return 0;
} 

function OnPhysics(sys,syst, sysdata)
{
    return 0;
} 

function OnSpin(sys,syst, sysdata)
{
    return 0;
} 


function OnInput(sys,keystate, keycode)
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

function OnBspRead(btfsectiom, file)
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

function OnExit(sys,zero,sysdata)
{
    return 0;
} 


//-----------------------------------------------------------------------------
//leve this include last in the file//
dofile("gsapp.gh");
