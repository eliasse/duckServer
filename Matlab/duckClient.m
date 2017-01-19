% This script uses jsonlab to parse JSON data:
% http://iso2mesh.sourceforge.net/cgi-bin/index.cgi?jsonlab/Download
clear, clc
addpath('jsonlab-1.5');

socket = tcpip('192.168.1.190', 12, 'NetworkRole', 'client');
socket.InputBufferSize = 1024;

fopen(socket);

% Create and send a JSON string using jsonlab
time_now = datetime('now');
out_data.SOURCE = 'MATLAB';
out_data.TIME_UNIX = posixtime(time_now);
out_data.NMEA_SENTENCES.NMEA1 = '$WTF1,1,-1,10000,-10000,0.123456789,-0.123456789,*00';
out_data.NMEA_SENTENCES.NMEA2 = '$WTF2,1,-0.123456789,*00';
options.ForceRootName = 0;
out_json = savejson('',out_data, options);

% Put JSON in a 'message'
%out_msg = createMessage('{"SOURCE":"MATLAB","NMEA_SENTENCES":{"NMEA":"$WTF1,1,-1,10000,-10000,0.123456789,-0.123456789,*00","NMEA0":"$WTF2,1,-1,10000,-10000,0.123456789,-0.12345,*00"}}');
out_msg = createMessage(out_json);

% Send the message
fprintf(socket, out_msg);

% Read some data from server
i = 0;
while i < 6
    if (socket.BytesAvailable > 0)
        header = fread(socket, 4);
        header = sprintf('%s', header);
      
        body_len = str2num(header);
        json_string = fread(socket, body_len);
        json = sprintf('%s', json_string);
        
        fprintf('Header:\t%s\n', header);
        fprintf('Body len:\t%s\n', num2str(body_len));
        fprintf('JSON:\t%s\n', json);
        
        i = i + 1;
        data(i) = loadjson(json);
    end
end

fclose(socket);

