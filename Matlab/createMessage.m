function data = createMessage(jsonString)
body_len = length(jsonString);
header = sprintf('%4d', body_len);
disp(header);

data = strcat(header, jsonString);
end