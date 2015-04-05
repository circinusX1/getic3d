dofile("common.gh");        // data constants and scrutures
dofile("render.gh");        // rende plug data constants

tri <-{
    v = null,
}


function OnStart(sys, syst, sysdata)
{
    sys.SetHudMode(hud.HUD_TRACE);
    
    //
    // make a mesh of 3 vertexes
    //
    tri.v = RenderVx(3,0);
    
    local vx = V3(-16.0,-16.0,0.0);
    tri.v.setv(0,vx);
    
    vx = V3(16.0,-16.0,0.0);
    tri.v.setv(1,vx);
    
    vx = V3(0.0,32.0,0.0);
    tri.v.setv(2,vx);
    
    
    
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
        TheApp.render.Color4(255,0,0,255);
        TheApp.render.SetArray(tri.v);
            TheApp.render.DrawArrays(0,3,rndconst.Z_TRIANGLES)
        TheApp.render.ResetArray(tri.v);
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


