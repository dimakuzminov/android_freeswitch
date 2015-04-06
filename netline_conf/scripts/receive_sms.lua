freeswitch.consoleLog("warn","RECEIVE SMS\n");
--local event = freeswitch.Event("CUSTOM","SMS:SEND_MESSAGE");
local event = freeswitch.Event("MESSAGE");
event:addHeader("proto","sms");
event:addHeader("dest_proto","sip");
event:addHeader("to_proto","sip");
event:addHeader("from",message:getHeader("from"));
event:addHeader("from_full",message:getHeader("from"));
event:addHeader("login",message:getHeader("login"));
event:addHeader("context","public");
event:addHeader("to","1000@192.168.43.1");
event:addHeader("sip_profile","internal");
event:addHeader("subject",message:getHeader("subject"));
event:addHeader("type","text/plain");
event:addHeader("hint",message:getHeader("hint"));
--event:addHeader("replying","true");
event:addBody(message:getBody());
--event:fire(); -- Does not work
event:chat_execute("send");
