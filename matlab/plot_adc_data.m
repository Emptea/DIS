close all 
clear s
s = serialport("COM2",256000,"Timeout",50);
sg_len = 32768;
Fs = 2e6;
cmd_send_sg = [0x39 0x64 0x6d 0x63 0x00 0x00 0x00 0x00 0x6a 0x06];
cmd_conv = [0x37 0x64 0x6d 0x63 0x23 0x36];
cmd_exti = [0x32 0x64 0x6d 0x63 0x00 0x00 0x00 0x00 0x2b 0xb5];
vref = 3.3;
f = Fs/sg_len*(0:(sg_len/2-1));

write(s, cmd_exti, 'char')
cmd = read(s, 1, 'uint32');
v_max = read(s, 1, 'single')
crc = read(s, 1, 'uint16');

write(s, cmd_send_sg, 'char')
cmd = read(s, 1, 'uint32');
data = read(s, sg_len,"uint16");
crc = read(s, 1, 'uint16');
volt = data/double(intmax("uint16"))*vref;
figure; plot(volt)

% Y = fft(data, sg_len);
% P2 = abs(Y);
% P1 = P2(1:sg_len/2);
% P1(2:end-1) = 2*P1(2:end-1);
% 
% figure; plot(f,mag2db(P1)) 
% title("Single-Sided Amplitude Spectrum of X(t)")
% xlabel("f (Hz)")
% ylabel("|P1(f)|")
% 
% delete(s)