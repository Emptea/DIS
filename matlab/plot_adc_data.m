close all 
clear s
s = serialport("COM2",256000,"Timeout",50);
sg_len = 32768/2;
fft_len = 32768;
Fs = 2e6;
cmd_send_sg = [0x39 0x64 0x6d 0x63 0x00 0x00 0x00 0x00 0x6a 0x06];
cmd_send_fft = [0x38 0x64 0x6d 0x63 0x00 0x00 0x00 0x00 0xab 0xca];
cmd_conv = [0x37 0x64 0x6d 0x63 0x23 0x36];
cmd_exti = [0x32 0x64 0x6d 0x63 0x00 0x00 0x00 0x00 0x2b 0xb5];
vref = 3.3;
f = Fs/sg_len*(0:(sg_len/2-1));
cfg = struct('poly',0x8005,'init',0xffff,'refin',1,'refout',1,'xorout',0);
%%
write(s, cmd_exti, 'char')
pack = read(s, 10, 'char');
crc2check = crc16(pack, cfg);
if (crc2check == 0 & pack(1:4) == '6dmc')
    v_max = typecast(uint8(pack(5:8)), 'single')
end
%%
write(s, cmd_send_sg, 'char')
pack = read(s, 4 + sg_len*2 + 2, 'char');
crc2check = crc16(pack, cfg);
if (crc2check == 0 & pack(1:4) == '9dmc')
    sg = typecast(uint8(pack(5:end-2)), 'uint16');
    volt = sg/double(intmax("uint16"))*vref;
    figure; plot(volt)

    Y = fft(sg, fft_len);
    plot_fft(Y, fft_len, Fs)
end

%%
write(s, cmd_send_fft, 'char')
pack = read(s, 4 + fft_len*8 + 2, 'char');
crc2check = crc16(pack, cfg);
if (crc2check == 0 & pack(1:4) == '8dmc')
    data = typecast(uint8(pack(5:end-2)), 'single');
    sg_fft = data(1:2:end) + 1i*data(2:2:end);
    plot_fft(sg_fft, fft_len, Fs)
    legend('stm32')
end

delete(s)
%%
function plot_fft(Y, len, Fs)
f = Fs/len*(0:(len/2-1));
P2 = abs(Y);
P1 = P2(1:len/2);
P1(2:end-1) = 2*P1(2:end-1);

figure; plot(f,mag2db(P1)) 
title("Single-Sided Amplitude Spectrum of X(t)")
xlabel("f (Hz)")
ylabel("|P1(f)|")

end