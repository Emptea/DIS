close all 

s = serialport("COM2",115200,"Timeout",5);
sg_len = 64000;
cmd_conv = 'cmd3';
cmd_send_sg = 4;
vref = 3.3;

write(s, 'cmd5', 'char')
data = read(s,sg_len,"uint16");
volt = data/double(intmax("uint16"))*vref;
figure; plot(data)
figure; plot(volt)

delete(s)