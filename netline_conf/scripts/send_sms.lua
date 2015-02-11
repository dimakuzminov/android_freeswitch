freeswitch.consoleLog("info","XXXXXXX");
--message:chat_execute("reply","you said:" .. message:getBody() );
api = freeswitch.API();
api:executeString("gsmopen_sendsms gsm01 " .. message:getHeader("to_user") .. " " .. message:getBody() );
