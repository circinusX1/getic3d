//============================================================================
// Author: Octavian Marius Chincisan 2006 March - 2006 Sept
// Zalsoft Inc 1999-2006
//============================================================================
dofile("common.gh");        // data constants and scrutures
dofile("render.gh");        // rende plug data constants

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

function OnDrawHud(sys,camera, sysdata)
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


function OnDraw(sys,camera,sysdata)
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


//=============================================================================
TheApp<-{

    [_s.SYS_START]            = OnStart, 
    [_s.SYS_RENDER_3D]        = OnDraw,
    [_s.SYS_RENDER_2D]        = OnDrawHud,
    system					  = null,
    render                    = null,
};


function Main(system)
{
    return "SystemProc";
}

function SystemProc(system, msg, param1, param2)
{
    if (msg in TheApp)
    {
        if(msg==_s.SYS_START)
        {
            TheApp.system = system;
            TheApp.render = system.GetRenderSys();
        }
        return (TheApp[msg])(system,param1, param2);
    }
    return 0;
}


