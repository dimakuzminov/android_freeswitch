freeswitch.consoleLog("info","RECEIVE SMS" .. message:serialize() .. "\n");
--local event = freeswitch.Event("CUSTOM","SMS:SEND_MESSAGE");
local event = freeswitch.Event("MESSAGE");
event:addHeader("proto","sms");
event:addHeader("dest_proto","sip");
event:addHeader("to_proto","sip");
event:addHeader("from",message:getHeader("from"));
event:addHeader("from_full",message:getHeader("from"));
--event:addHeader("login","sip:mod_sofia@192.168.43.1:5060");
event:addHeader("context","public");
event:addHeader("to","1000@192.168.43.1");
event:addHeader("to_user","1000");
event:addHeader("to_host","192.168.43.1");
event:addHeader("sip_profile","internal");
event:addHeader("subject",message:getHeader("subject"));
event:addHeader("type","text/plain");
event:addHeader("hint","simbox");
--event:addHeader("replying","true");
event:addBody(message:getBody());
--event:fire(); -- Does not work
freeswitch.consoleLog("info","RECEIVE SMS" .. event:serialize() .. "\n");
event:chat_execute("send");
