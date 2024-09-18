close all 

s = serialport("COM2",256000,"Timeout",10);
sg_len = 128000;
Fs = 2e6;
cmd_send_sg = [0x39 0x64 0x6d 0x63 0x21 0xde];
cmd_conv = [0x33 0x64 0x6d 0x63 0x22 0x06];
vref = 3.3;

write(s, cmd_conv, 'char')
cmd = read(s, 1, 'uint32');
data = read(s,sg_len,"uint16")-32768;
crc = read(s, 1, 'uint16');

volt = data/double(intmax("uint16"))*vref;
figure; plot(data)
figure; plot(volt)

Y = fft(volt);
P2 = abs(Y/sg_len);
P1 = P2(1:sg_len/2+1);
P1(2:end-1) = 2*P1(2:end-1);

f = Fs/sg_len*(0:(sg_len/2));
plot(f,mag2db(P1),"LineWidth",3) 
title("Single-Sided Amplitude Spectrum of X(t)")
xlabel("f (Hz)")
ylabel("|P1(f)|")

delete(s)