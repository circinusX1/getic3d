
dofile("common.gh");        // data constants and scrutures
dofile("render.gh");        // rende plug data constants
dofile("input.gh");         // input keys
dofile("bsp.gh");           // constants for reading BSP file 


scene <- {
	maps	= [],               // store all the bsp found in bspmaps/
    menubar = [],
    loaded	= false,            // return error
    bsp		= null,
    splash  = null,
    caption = null,
    system  = null,
};

//-----------------------------------------------------------------
// called before loading any DLL's of firing up the window
function Configure(system, startupData, renderData)
{
//    print("render dll = " + startupData.renderSel);
//    print("input dll = " + startupData.inputSel);
//    print("audio dll = " + startupData.audioSel);
    
    renderData.dmPelsWidth   = 640; 
    renderData.dmPelsHeight  =	480;
    
   return 1; // un-changed 0 1 changed
}

function BspProc(t,m,w,p)
{
    return 0;
}

//-----------------------------------------------------------------
function scene::Initialize(sys)
{
    // creats the Getic logo in the column 80 line 10 up to column 124 and line 40
    // there are 140 cols per 64 lines starting RB of viewport
    system  = sys;
    bsp		= BeamTree("BspProc");       // loaded beam tree (bsp)
    splash  = UiControl();      //
    caption = UiControl();

    
    splash.Create(0, 0, 60, 7, 64);  
    splash.SetFont(1);
    splash.SetImage("logo0.jpg");
    
    // create the top grey caption bar
    caption.Create(0, 0, 60, 128, 64);
    caption.SetFont(3);
    caption.SetText("Bsp Viewer   ...MOGE first glimpse", 0);
    caption.SetColor(0xDDFFDD, 0xFFDDDD);
    caption.SetImage("caption.bmp");
    
    BuildMenu("compiled/");
    sys.SetHudMode(hud.HUD_TRACE|hud.HUD_HELP);
}

// queries master server an buils a menu with all servers
// not stable yet
function scene::ConnToMaster()
{
    /*
    local idx      = 0;
    local uiman    = system.GetUIMan();
    local scrsz    = SIZE();
    local xpos     = 2;
    local menuYpos = uiman.GetScreenGridY(0)-16; // font index(0-3)
    local slot     = 0;
    
    maps.resize(0);
    menubar.resize(0);

    local   levemanager = LevelMan();
    local servers = levemanager.QueryMaster("localhost",8080,"",0);   
    if(servers)
    {
        maps.resize(servers);
        menubar.resize(servers+4); // for top and remote
    }
    
    menubar[slot] = UiControl();
    menubar[slot].Create(0, xpos, menuYpos, xpos+70, menuYpos + 7);
    menubar[slot].SetFont(3);
    menubar[slot].SetText("Curent Folder: " + folder, 0);
    menubar[slot].SetImage("topmenu.bmp");
    menubar[slot].SetColor(0xCCCCCC, 0xCCCCCC);
    slot++;
    menuYpos -= 4;

// makes button for remote downloading
    menuYpos-=3;
    menubar[slot] = UiControl();
    menubar[slot].Create(100, xpos, menuYpos, xpos+40, menuYpos+3);
    menubar[slot].SetFont(1);
    menubar[slot].SetText("> Remote Levels", 0);
    menubar[slot].SetImage("topmenu.bmp");
    menubar[slot].SetColor(0xCCCC00, 0xCCCCCC);
    slot++;
    
    
    // makes button for getting back to bspmaps
    menubar[slot] = UiControl();
    menubar[slot].Create(101, xpos+40, menuYpos, xpos+80, menuYpos+3);
    menubar[slot].SetFont(1);
    menubar[slot].SetText("> Local levels", 0);
    menubar[slot].SetImage("topmenu.bmp");
    menubar[slot].SetColor(0xCCCC00, 0xCCCCCC);
    slot++;
    
       // makes button for getting back to bspmaps
    menubar[slot] = UiControl();
    menubar[slot].Create(102, xpos+80, menuYpos, xpos+120, menuYpos+3);
    menubar[slot].SetFont(1);
    menubar[slot].SetText("> Master Server", 0);
    menubar[slot].SetImage("topmenu.bmp");
    menubar[slot].SetColor(0xCCCC00, 0xCCCCCC);
    slot++;
    
    menuYpos -= 4;

    for( idx=0; idx < servers; idx+=1)
    {
        local server = fb.GetAt(idx);
        if(server != null)
        {
            local srviunfo = levemanager.GetServerAt(idx);
            
            menubar[slot] = UiControl();
            menubar[slot].Create(idx, xpos+(idx), menuYpos, xpos+(idx)+40, menuYpos+3);
            menubar[slot].SetFont(1);
            menubar[slot].SetText("adsfasd", 0);
            menubar[slot].SetImage("freebut.bmp");
            menubar[slot].SetColor(0xDDCC44, 0x22CCCC);

            menuYpos-=3;
            slot++;
        
        }
    }
    */
}

// browses the bspmaps folder for all compiled files
// screen starts at 00 at left bottom corner
function scene::BuildMenu(folder)
{
    local idx      = 0;
    local fb       = FileBrowser();
    local files    = fb.BroseDir(folder,"*.gbt,*.ml");
    local uiman    = system.GetUIMan();
    local scrsz    = SIZE();
    local xpos     = 2;
    local menuYpos = uiman.GetScreenGridY(0)-16; // font index(0-3)
    local slot     = 0;
    
	maps.resize(0);
    menubar.resize(0);
   
    if(files)
    {
        maps.resize(files);
        menubar.resize(files+4); // for top and remote
    }
    
    menubar[slot] = UiControl();
    menubar[slot].Create(0, xpos, menuYpos, xpos+70, menuYpos + 7);
    menubar[slot].SetFont(3);
    menubar[slot].SetText("Curent Folder: " + folder, 0);
    menubar[slot].SetImage("topmenu.bmp");
    menubar[slot].SetColor(0xCCCCCC, 0xCCCCCC);
    slot++;
    menuYpos -= 4;

// makes button for remote downloading
    menuYpos-=3;
    menubar[slot] = UiControl();
    menubar[slot].Create(100, xpos, menuYpos, xpos+40, menuYpos+3);
    menubar[slot].SetFont(1);
    menubar[slot].SetText("> Remote Levels", 0);
    menubar[slot].SetImage("topmenu.bmp");
    menubar[slot].SetColor(0xCCCC00, 0xCCCCCC);
    slot++;
    
    
    // makes button for getting back to bspmaps
    menubar[slot] = UiControl();
    menubar[slot].Create(101, xpos+40, menuYpos, xpos+80, menuYpos+3);
    menubar[slot].SetFont(1);
    menubar[slot].SetText("> Local levels", 0);
    menubar[slot].SetImage("topmenu.bmp");
    menubar[slot].SetColor(0xCCCC00, 0xCCCCCC);
    slot++;
    
       // makes button for getting back to bspmaps
    menubar[slot] = UiControl();
    menubar[slot].Create(102, xpos+80, menuYpos, xpos+120, menuYpos+3);
    menubar[slot].SetFont(1);
    menubar[slot].SetText("> Master Server", 0);
    menubar[slot].SetImage("topmenu.bmp");
    menubar[slot].SetColor(0xCCCC00, 0xCCCCCC);
    slot++;
    
    menuYpos -= 4;

        
    for( idx=0; idx < files; idx+=1)
    {
        local lf = fb.GetAt(idx);
        if(lf == "") break;

        maps[idx] = folder+lf;

        menubar[slot] = UiControl();
        menubar[slot].Create(idx, xpos+(idx), menuYpos, xpos+(idx)+40, menuYpos+3);
        menubar[slot].SetFont(1);
        menubar[slot].SetText(lf, 0);
        menubar[slot].SetImage("freebut.bmp");
        menubar[slot].SetColor(0xCCCC44, 0xCCCCCC);

        menuYpos-=3;
        slot++;
    }
}    
    
//-----------------------------------------------------------------
// checks remote levels
function scene::CheckRemoteLevels(system, url)
{
    local   levemanager = LevelMan();
    // void    Setup(const char* url, const char* locopath,const char* proxy=0, UINT port=0);
    
    levemanager.SetupAddrs(url,"import","",0);
    system.SetMode(sys_states.DOWNLOADING_MODE);

    levemanager.CheckLocally();
    if(0!=levemanager.Download(1))
    {
        system.Print("Cannot connect to '" + url + "'");
    }
    if(levemanager.GetResourceCount(""))
    {
        BuildMenu("import/");
        system.SetMode(sys_states.SET_MODE);
    }
    system.SetMode(sys_states.SET_MODE);
}

//-----------------------------------------------------------------
// Loads the scene 'filename'
function scene::Load(unused, filename)
{
    if( 0 == bsp.LoadLevel(filename) )
    {
        loaded = true;
        print("File loaded: "+filename + "\r\n");
    }
    else
    {
		print("Cannot load: "+filename + "\r\n");
		loaded = false;
    }
		
    // prints a menu in coonsole window too		
	print ("------------Menu------------r\n");
    local m
    for(m=0; m < maps.len(); m+=1)
    {
        local key = (m+1);
        print ("Press " +  key +" to load: " + maps[m] + "\r\n");
    }
    print("press Shift+Escape to exit or '~' to hide this\r\n");
    return loaded;
}

//-----------------------------------------------------------------
// scene Render function
function scene::Render(param)
{
	if(loaded)
		bsp.Render(param, rndconst.Z_POLYGON); 
}

//-----------------------------------------------------------------
// Scene spin function. This is called on constant time frame basis 
// set up at 0.02 secs to update physics and so on.
function scene::Animate(camera, systemdata)
{
    if(loaded)
        bsp.Animate(camera, systemdata);
}

//-----------------------------------------------------------------
function scene::OnCommand(cmdid, system)
{
    switch(cmdid)
    {
        case -2:  // is the id of Mm button  
            system.ToggleFullScreen();
            return 1;
        case 101:
            scene.BuildMenu("bspmaps/");
            return 1;
        case 102:
            //scene.ConnToMaster();
            break;
        case 100: //download levels from www.zalsoft.com/levels200/
            scene.CheckRemoteLevels(system, "http://www.getic.net/levels200/index.php");
            return 1;
        default:
            if(cmdid >=0 && cmdid < maps.len())
            {
                if(scene.Load(null, maps[cmdid]) == true)
                {
                    system.SetMode(sys_states.PLAY_MODE);
                }
                else
                {
                    system.SetMode(sys_states.SET_MODE);
                }
                return 1;
            }
            break;
    }
    return 0;
}

//------------------------------------------------------------------
// deletes all objects before stopping the system
function scene::Destroy(system)
{
    print("\r\nDestroy(system)\r\n");
    loaded = false;
    delete bsp;
    delete splash;
    delete caption;
    delete menubar;
    system.Stop();           
}

function Escape(sys, wP, lP)
{
    return 0; // let default switch between menu and play modes
}
		    
		    
function CloseApp(sys, wP, lP)
{
   scene.Destroy(sys);
}

function ExitApp(sys, wP, lP)
{
    
}
		    
function UiCmd(sys, cmdCode, unused)
{
    return scene.OnCommand(cmdCode, sys);
}


function SInput(sys, isDown, keyCode)
{
    if(keyCode  >= keys.DIK_1 && 
	    keyCode <= keys.DIK_9 && 
	    isDown == 1 )                  // key is down
    {
	    local levIdx = (keyCode-keys.DIK_1) % scene.maps.len();
	    scene.Load(null, scene.maps[levIdx]);
    }
    return 0;
}

function Animate(sys, wP, lP)
{
    scene.Animate(wP,lP);
    return 0;               //return spin@common.gh to block a device from updating 
}

function Paint(sys, wP, lP)
{
    scene.Render(lP);
    return 0;               // return -1 to break the game loop
}

function Start(sys, cmdLine, sysData)
{
    scene.Initialize(sys);   
    return 1;
}

//=============================================================================
function Main(system)
{
    return "SystemProc";
}

TheApp<-{

    [_s.SYS_START]            = Start,  
    [_s.SYS_CONFIGURE]        = Configure,  
    [_s.SYS_ESCAPE]           = Escape,
    [_s.SYS_CLOSEAPP]         = CloseApp,
    [_s.SYS_INPUT]            = SInput,
    [_s.SYS_UICOMMAND]        = UiCmd,
    [_s.SYS_RENDER_3D]        = Paint,
    [_s.SYS_ANIMATE]          = Animate,
    [_s.SYS_EXIT]             = ExitApp,
    system					  = null,
    render                    = null,
};


function SystemProc(system, msg, wP, lP)
{
    if (msg in TheApp)
    {
        if(msg == _s.SYS_START){
            TheApp.system = system;
            TheApp.render = system.GetRenderSys();      
        }
        return (TheApp[msg])(system, wP, lP);
    }
    return 0;
}


