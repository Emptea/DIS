close all
clear s
s = serialport("COM3",256000,"Timeout",50);
sg_len = 1024;
fft_len = 1024;
Fs = 2e6;
cmd_list = ["cmd0", ...
    "cmd1", ...
    "cmd2", ...
    "cmd3", ...
    "cmd4", ...
    "cmd5", ...
    "cmd7", ...
    "cmd8", ...
    "cmd9", ...
    "ping",];
cmd_names = ["pwr_on_off";...
    "exti_on_off";...
    "set_tim";...
    "set_sg_len";...
    "set_fft_len";...
    "send_res";...
    "start_conv";...
    "send_sg";...
    "send_fft"; ...
    "ping";];

cmd = array2table(reverse(cmd_list));
cmd.Properties.VariableNames = cmd_names;
arg = array2table(uint32([0, 0xFF]),'VariableNames', {'off','on'});

vref = 3.3;
f = Fs/sg_len*(0:(sg_len/2-1));
cfg = struct('poly',0x8005,'init',0xffff,'refin',1,'refout',1,'xorout',0);
%%
send_cmd(s, cmd.ping, arg.on, cfg)
pack = read(s, 10, 'char');
print_ans(pack)
%%
send_cmd(s, cmd.pwr_on_off, arg.on, cfg)
pack = read(s, 10, 'char');
print_ans(pack)
%%
send_cmd(s, cmd.set_sg_len, sg_len, cfg)
pack = read(s, 10, 'char');
print_ans(pack)
%%
send_cmd(s, cmd.set_fft_len, sg_len, cfg)
pack = read(s, 10, 'char'); 
print_ans(pack)
%%
send_cmd(s, cmd.exti_on_off, arg.on, cfg);
fprintf("err ")
pack = read(s, 10, 'char');
print_ans(pack)
fprintf("pulse ")
pack = read(s, 10, 'char');
print_ans(pack)
fprintf("res ")
pack = read(s, 10, 'char');
fprintf("cmd:%s, arg:%.4f\n\n", reverse(pack(1:4)),typecast(uint8(pack(5:8)),'single'))
crc2check = crc16(pack, cfg);
if (crc2check == 0 & pack(1:4) == reverse('rslt'))
    v_max = typecast(uint8(pack(5:8)), 'single')
end
pause(1)
%%
send_cmd(s, cmd.send_sg, sg_len, cfg);
pack = read(s, 10, 'char'); 
print_ans(pack)
sg_pack = read(s, sg_len*2 + 2, 'char');
crc2check = crc16(sg_pack, cfg);
if (crc2check == 0 & pack(1:4) == 'gisp')
    sg = typecast(uint8(sg_pack(1:end-2)), 'uint16');
    volt = sg/double(intmax("uint16"))*vref;
    figure; plot(volt)
    
    Y = fft(sg, fft_len);
    plot_fft(Y, fft_len, Fs)
end

%%
send_cmd(s, cmd.send_fft, fft_len, cfg);
pack = read(s, 10, 'char');
print_ans(pack)
fft_pack = read(s, fft_len*8 + 2, 'char');
crc2check = crc16(pack, cfg);
if (crc2check == 0 & pack(1:4) == 'tffp')
    data = typecast(uint8(fft_pack(1:end-2)), 'single');
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

%%
function send_cmd(s, cmd, arg, cfg)
    arg_uint32 = typecast(uint32(arg),'uint8');
    crc = crc16([char(cmd) arg_uint32], cfg);
    
    pack = [char(cmd) arg_uint32 typecast(uint16(crc), 'uint8')];
    fprintf("cmd pack\ncmd:%s, arg:%d\n", reverse(pack(1:4)),typecast(uint8(pack(5:8)),'uint32'))
    write(s, pack, 'char');
end

%%
function print_ans(pack)
    fprintf("answer\ncmd:%s, arg:%d\n\n", reverse(pack(1:4)),typecast(uint8(pack(5:8)),'uint32'))
end