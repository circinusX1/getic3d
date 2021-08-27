dofile("common.gh");     // data constants and strutures
dofile("render.gh");     // rende plug data constants
dofile("bsp.gh");        // rende plug data constants


myscene<-{
    bsp   = null,
    load  = false,
    rect = null,
    tex  = null,
    pos  = null,
    how  = rndconst.Z_POLYGON|vxt.VX_TX1,
};

function myscene::LoadBulb( w)
{
    pos = Pos();
    pos.pos = w.pos;
    pos.zoom = V3(1.0,1.0,1.0);

    rect = RenderVx(4, vxt.VX_TX1);
    tex = TheApp.system.GenTexFile("res/areyegreen.bmp", 0);
    
    local vx = V3(-16.0,-16.0,0.0);
    local uv = UV(0.0,0.0);
    rect.setv(0,vx);
    rect.setu(0,uv,0);

    vx = V3(16.0,-16.0,0.0);
    uv = UV(1.0,0.0);
    rect.setv(1,vx);    
    rect.setu(1,uv,0);
    
    vx = V3(16.0,16.0,0.0);
    uv = UV(1.0,1.0);
    rect.setv(2,vx);    
    rect.setu(2,uv,0);
        
    vx = V3(-16.0,16.0,0.0);
    uv = UV(0.0,1.0);
    rect.setv(3,vx);    
    rect.setu(3,uv,0);
    
    
}

function myscene::Render()
{
    

    TheApp.render.Blend(blend.DS_BLEND_HALLO);
    TheApp.render.Color4(255,255,255,128);
    
    TheApp.render.BindTex1(tex);
    TheApp.render.Push();    
        TheApp.render.SetTransform(pos,trx.OBJ_BB_SPH);
            TheApp.render.SetArray(rect);
                TheApp.render.DrawArrays(0,4,how);
            TheApp.render.ResetArray(rect);
        TheApp.render.ResetTransform();
    TheApp.render.Pop();
    TheApp.render.UnBlend(blend.DS_BLEND_HALLO);
}


function OnStart(sys, cmdline, sysdata)
{
    
    myscene.bsp =  BeamTree("BspProc");
    if(myscene.bsp.LoadLevel("bspmaps/default.gbt")==0)
    {
        print("bsp loaded");
        myscene.load=true;
        sys.SetMode(sys_states.PLAY_MODE);
    }
    else
    {
        print("cannot load");
        sys.SetMode(sys_states.SET_MODE);
        delete myscene.bsp;
        sys.Stop();
    }
    return 0;
} 

function OnDraw(sys,camera,sysdata)
{
    if(myscene.load==true)
    {
        myscene.bsp.Render(sysdata, rndconst.Z_POLYGON); 
        myscene.Render();
    }
    return 0;
} 

function OnAnimate(sys, camera, sysdata)
{
    if(myscene.load==true)
        myscene.bsp.Animate(camera, sysdata);

    return 0;
} 

function OnClose(sys, camera, sysdata)
{
    if(myscene.load)
    {
        myscene.load=false;
        delete myscene.bsp;
    }
    return 0;
}

//=============================================================================
TheApp<-{

    [_s.SYS_CLOSEAPP]         = OnClose, 
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

function BspProc(bsp, msg, w, l)
{
    switch(msg)
    {
        case _b.BM_CREATED:
            bsp.WantMessages(_bspflag.WANT_BSP_LOAD);
            break;
        case _b.BM_READITEM:
            if(w.type == _item.ITM_LIGTBULB)
            {
                myscene.LoadBulb(w);
            }
            break;
        default:
            print(msg + "\n");
            break;
    }
    return 0;
}
