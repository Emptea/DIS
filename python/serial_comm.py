import serial
import struct
import crcmod
import matplotlib.pyplot as plt
import numpy as np
from collections import namedtuple 
from typing import *

crc16 = crcmod.mkCrcFun(0x18005, rev=True, initCrc=0xFFFF, xorOut=0x0000)
sz_cmd = 4
sz_sg = 128000

cmd_pwr_on = bytearray('cmd0', 'utf-8')
cmd_pwr_off = bytearray('cmd1', 'utf-8')
cmd_start_exti = bytearray('cmd2', 'utf-8')
cmd_set_tim = bytearray('cmd3', 'utf-8')
cmd_set_sg_len = bytearray('cmd4', 'utf-8')
cmd_set_fft_len = bytearray('cmd5', 'utf-8')
cmd_start_conv = bytearray('cmd7', 'utf-8')
cmd_send_fft = bytearray('cmd8', 'utf-8')
cmd_send_sg = bytearray('cmd9', 'utf-8')
cmd_ping = bytearray('ping', 'utf-8')

ser = serial.Serial()
ser.baudrate= 256000
ser.port = 'COM2'
# ser.timeout = 20
ser.open()

def ask(ser, cmd, arg):
    cmd.extend(arg.to_bytes(4,'little'))
    cmd.extend(crc16(cmd).to_bytes(2,'little'))
    ser.write(cmd)
    print(' '.join(format(x, '02x') for x in cmd))
    ans = ser.read(10)
    print(' '.join(format(x, '02x') for x in ans))

def plot(sg):
    x = np.linspace(0, (sz_sg-1)/2e6, sz_sg)

    fig, ax = plt.subplots()
    ax.plot(x, np.multiply(sg, 3.3/65535))
    # ax.plot(x, sg)
    plt.show()


ask(ser, cmd_ping[::-1], 0)
ask(ser, cmd_set_sg_len[::-1], int(32768/2))
# resp = ser.read(10)
# print(' '.join(format(x, '02x') for x in resp))
# ask(ser, cmd_send_sg[::-1], 0)
# ask(ser, cmd_send_sg[::-1])
# pack = bytearray(sz_cmd + 2*sz_sg - 1)
# pack[0:(sz_cmd-1)]= ser.read(sz_cmd)
# rng = 2*int(sz_sg/8000)
# for i in range(rng):
#     pack[(sz_cmd + i*8000):(sz_cmd + (i+1)*8000)]=ser.read(8000)
# crc = ser.read(2)

# print(len(pack))
# sg = struct.unpack_from(f'{sz_sg}H', pack, sz_cmd)
# # print(' '.join(format(x, '02x') for x in pack))
# print(' '.join(format(x, '02x') for x in crc))
# crc_calc = crc16(pack)
# crc_calc = crc_calc.to_bytes(2,'little')
# print(' '.join(format(x, '02x') for x in crc_calc))
# if crc == crc_calc:
#     plot(sg) 
