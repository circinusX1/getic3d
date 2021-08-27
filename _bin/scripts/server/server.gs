/*
    samples for file server
    ftp://username:password@ftp.xxxxx.com:8000/page.ext
    http://username:password@ftp.xxxxx.com:8000/page.ext
    ftp://ftp.xxxxx.com:8000/page.ext
    http://www.xxxxx.com:8000/page.ext
    
    samples for game server
    gbt://234,34,45,67/servers/server1 (http://www.w3.org/Protocols/Overview.html)  
*/

//-----------------------------------------------------------------------------
// server config options
srv_cfg <-{
    CFG_NOSENDQUEUE              =0x0,  // ->Rec(T1)=====(Proc(Pool))->
                                        //  receive thread and processing pool
                                        //  processing poll sends out data
                                        
    CFG_SENDQUEUE                =0x1,  // ->Rec(T1)=====(Proc(Pool))=====Snd(T1)->
                                        //  separate thread for receive+send
                                        //  coupled with queues into the processing pool
                                        
    CFG_SEPARATETHREADS          =0x3,  // ->Rec(T1)=====(Proc(Pool))=====Snd(T2)->
                                        // 2 threads one for rec one for send and
                                        // the usual processing pool
                                        
    CFG_MULTI_UDP_SRVS           =0x8,  // by compilation not used  (#ifdef _MULTI_UDPS)
        
};

//-----------------------------------------------------------------------------
// print text 'warning' levels
prn_level <-{
    IL  =0x1,   // info level
    WL  =0x3,   // warning level
    EL  =0x7,   // error level
    DL  =0xF,   // debug level
};

//-----------------------------------------------------------------------------
sm <- {
    GM_CREATE    = 100,
    
}

//-----------------------------------------------------------------------------
function main(server)                        
{
    //server.SetProxy("a proxy server",3128);
    server.SetMasterServer("localhost",8080);
    server.SetLocalIP("localhost", 27001); // if you are behind router set the router WAN IP.
    server.SetMapsLocation("http://www.getic.net/levels200/getic_index.ml");
    server.SetWorkingFolder("server_levels"); // should match the one passed DOWN IN CREATE MSG
    server.SetMaxTimePerLevel(1);
    server.SetMaximFrameRate(200);
    server.SetThreadCount(4);   
    server.SetPoolSize(512);    
    server.ConfigServer(srv_cfg.CFG_SENDQUEUE, prn_level.IL);
    
    print("running server version " + server.GetCurrentVersion() +"\n");
    return "ServerProc";
}
            
            
function ServerProc(srvPtr, message, srv, lparam)
{
    switch(message)
    {
        case sm.GM_CREATE:
            srvPtr.Register2Master("www.getic.net", 80, 1);
            local levman = srvPtr.GetLevelManager();
            levman.SetupAddrs("http://www.getic.net/levels200/getic_index.ml",
                              "server_levels",// // should match the one passed UP
                              "no proxy",3128);
            levman.CheckLocally();
            if(0!=levman.Download(1))
            {
                print("Cannot connect \n");
                return -1;
            }
            else if(levman.GetResourceCount("")) 
            {
                return srv.Start();
            }
            return 1; // you 've handeled this message
    }
    return 0; // !handeled
}
                        
