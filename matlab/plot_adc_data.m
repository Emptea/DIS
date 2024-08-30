s = serialport("COM2",115200,"Timeout",5);
sg_len = 1024;
cmd_conv = 3;
cmd_send_sg = 4;
vref = 3.3;

write(s, cmd_conv, 'uint8')
data = read(s,sg_len,"uint16");
volt = data/double(intmax("uint16"))*vref;
figure; plot(data)
figure; plot(volt)

delete(s)