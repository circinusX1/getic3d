<?php
    //reg=%d&ip=%s&port=%d
    if(isset($_GET['reg'] && isset($_GET['ip'] && isset($_GET['port'])
    {
        $register = $_GET['reg'];
        $server   = $_GET['ip']."-".$_GET['port']."<br>";

        if(file_exists("logs/gs.html")) 
        {
            $file = fopen("writable_folder/gs.html", "r");
            $content  = fgets($file,8912);
            fclose($file);
            
            if($register)
            {
                if(!strstr($content,$server))
                {
                    $content+=$server;
                }
            }
            else
            {
                $newcontent = str_replace($server, "", $content);
                $content = $newcontent;
            }
         
            $file2 = fopen("writable_folder/gs.html", "w");
            fputs($file2, $content);
            fclose($file2);
        }        
    }
?>