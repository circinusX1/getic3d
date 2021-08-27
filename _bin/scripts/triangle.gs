dofile("common.gh");        // data constants and scrutures
dofile("render.gh");        // rende plug data constants

function OnStart(sys, syst, sysdata)
{
    sys.SetHudMode(hud.HUD_TRACE);
    sys.SetMode(0);
    sys.Print(" ");
    sys.Print(" ");
    sys.Print("Triangle...Press '~' to hide this");
    return 0;
} 

var<-{pct=-60.0,}
function OnDraw(sys,camera,sysdata)
{
    local v = V3(0.0, var.pct,0.0);
    var.pct += 0.8;
    
    TheApp.render.DisableTextures(1);
    TheApp.render.Push();
    TheApp.render.Translate(V3(0.0,0.0,-60.0));
    TheApp.render.Rotate(v);
    TheApp.render.BeginPrim(rndconst.Z_TRIANGLES);
    
    TheApp.render.Color4(255,0,0,255);
    TheApp.render.Vertex3(-16.0,-16.0,0.0);
    
    TheApp.render.Color4(0,255,0,255);
    TheApp.render.Vertex3(16.0,-16.0,0.0);
    
    TheApp.render.Color4(0,0,255,255);
    TheApp.render.Vertex3(0.0,32.0,0.0);
    TheApp.render.End();
    TheApp.render.Pop();
    
    return 0;
} 

function OnAnimate(sys, syst, sysdata)
{
    return spin.NO_CAM_SPIN; // block camera movement
} 

//=============================================================================
TheApp<-{

    [_s.SYS_START]            = OnStart, 
    [_s.SYS_RENDER_3D]        = OnDraw,
    [_s.SYS_ANIMATE]          = OnAnimate,
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


