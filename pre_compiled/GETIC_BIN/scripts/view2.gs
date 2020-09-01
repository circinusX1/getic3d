
dofile("common.gh");        // data constants and scrutures
dofile("render.gh");        // rende plug data constants
dofile("input.gh");         // input keys
dofile("bsp.gh");           // constants for reading BSP file 


//-----------------------------------------------------------------
// define a dummy scene for our bsp viewer
scene <- {
	maps	= [],               // store all the bsp found in bspmaps/
    menubar = [],
    bsp		= BeamTree(),       // loaded beam tree (bsp)
    loaded	= false,            // return error
    splash  = UiControl(),      //
    caption = UiControl(),
    system  = null,
};

//-----------------------------------------------------------------
// called before main
function Configure(config)
{
    
}

function OnStart, 
function OnToggleScreen,
function OnEscape,
function OnPaint,
function OnExit,
function OnRender,
function OnSpin,
function OnBspRead,
function OnStatusText,
function OnNetReceive,
function OnPhysics,

//-----------------------------------------------------------------
function scene::Initialize(sys)
{
    // creats the Getic logo in the column 80 line 10 up to column 124 and line 40
    // there are 140 cols per 64 lines starting RB of viewport
    system = sys;
    
    
    
    splash.Create(0, 0, 60, 7, 64);  
    splash.SetFont(1);
    splash.SetImage("logo0.jpg");
    
    // create the top grey caption bar
    caption.Create(0, 0, 60, 128, 64);
    caption.SetFont(3);
    caption.SetText("Bsp Viewer   ...MOGE first glimpse", 0);
    caption.SetColor(0xDDFFDD, 0xFFDDDD);
    caption.SetImage("caption.bmp");
    
    BuildMenu("bspmaps/");
    
    sys.SetHudMode(hud.HUD_TRACE|hud.HUD_HELP);
    sys.Print("This script is available in /scripts");
    sys.Print("This demo is just a simple BSP viewer");
    sys.Print("MOGE exported methods are in system/export.cpp");
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
function scene::CheckRemoteLevels(url)
{
    local   levemanager = LevelMan();
    // void    Setup(const char* url, const char* locopath,const char* proxy=0, UINT port=0);
    
    levemanager.SetupAddrs(url,"import","",0);
    levemanager.CheckLocally();
    if(0!=levemanager.Download(1))
    {
        system.Print("Cannot connect to '" + url + "'");
    }
    if(levemanager.GetResourceCount(""))
    {
        BuildMenu("import/");
    }
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
function scene::Physics(camera, systemdata)
{
    if(loaded)
        bsp.Animate(camera, systemdata);

}

//-----------------------------------------------------------------
// called from SystemProc to process buttons commands. 
// menubar[k].Create(k.. k is the command id that the button fires up
// when clicked (! pressed) (click -> means down and up)
function scene::OnCommand(cmdid, system)
{
    switch(cmdid)
    {
        case -1:   // created default by the uiman system engine
                   // is the id of X close button  
            Destroy(system);
            return 1;
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
            scene.CheckRemoteLevels("http://www.zalsoft.com/levels200/getic_index.ml");
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
    //maps.resize(0);
    //menubar.resize(0);
    
    system.Stop();           
}


//------------------------------------------------------------------
// MOGE MAIN PROCEDURE FUNCTION. MOGE accepts squirell scripts
// as well dll plugins that implements the systemproc or a mix
// of both. param1 and param2 are specific to the msg received.
// see common.gh for pre-pre-pre beta MOGE suported messages
function SystemProc(psystem, msg, param1, param2)
{
    if(set.debug)
    {
        trace("system: "+ psystem+ typeof(psystem)+" \r\n");
        trace("message: "+ msg+ typeof(msg)+" \r\n");
        trace("param1: "+ param1+ typeof(param1)+" \r\n");
        trace("param2: " + param2+ typeof(param2)+" \r\n\r\n");
    }
    switch(msg)
    {
        case _s.SYS_START:
			//print("Please select a map to view !");
			//param1.SetHudMode(hud.HUD_TRACE);
			
            return 1; // return 1 to avoid default implementation
        case _s.SYS_ESCAPE:
            return 0; // return 0 to leave default implementation
                      // here is the switch to setting screen  
		case _s.SYS_CLOSEAPP: // sent by shift escape or X button
		    scene.Destroy(param1);
            return 1;
		case _s.SYS_UICOMMAND:
		    return scene.OnCommand(param1, psystem);
        case _s.SYS_INPUT: // loads as well a map. 
			if(param2  >= keys.DIK_1 && 
			   param2  <= keys.DIK_9 && 
			   param1==1 )                  // key is down
			{
				local levIdx = (param2-keys.DIK_1) % scene.maps.len();
				scene.Load(null, scene.maps[levIdx]);
			}
            return 0;
        case _s.SYS_PHYSICS: 
            scene.Physics(param1,param2);
            return 1;
        case _s.SYS_RENDER:
            scene.Render(param2);
            return 1;
        default:
            break;
    }
    return 0;           
}

dofile("gsapp.gh")