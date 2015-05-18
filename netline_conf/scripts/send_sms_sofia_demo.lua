freeswitch.consoleLog("warn","SEND SMS\n");
local to_full = message:getHeader("to_user");
freeswitch.consoleLog("info","full To addressing " .. to_full .. "\n");
local to_full_clean = to_full:gsub("sms++", "");
freeswitch.consoleLog("info","full clean To addressing " .. to_full_clean .. "\n");
local to_num = {};
local i = 0;
for w in to_full_clean:gmatch("([^@]*)") do
    to_num[i] = w;
    freeswitch.consoleLog("info", "sub words" .. i .. ":" .. to_num[i] .. "\n");
    i = i + 1;
end
freeswitch.consoleLog("info", to_num[0] .. "\n");
local event = freeswitch.Event("MESSAGE");
event:addHeader("proto","sms");
event:addHeader("dest_proto","sip");
event:addHeader("to_proto","sip");
event:addHeader("from",message:getHeader("from"));
event:addHeader("from_full",message:getHeader("from"));
event:addHeader("login",message:getHeader("login"));
event:addHeader("context","public");
event:addHeader("to",to_num[0] .. "@192.168.43.100");
event:addHeader("sip_profile","external");
event:addHeader("subject",message:getHeader("subject"));
event:addHeader("type","text/plain");
event:addHeader("hint","simbox");
event:addBody(message:getBody());
freeswitch.consoleLog("info",event:serialize());
event:chat_execute("send");
