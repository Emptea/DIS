import serial
import struct
import crcmod
import matplotlib.pyplot as plt
import numpy as np
from collections import namedtuple 
from typing import *

crc16 = crcmod.mkCrcFun(0x18005, rev=True, initCrc=0xFFFF, xorOut=0x0000)
sz_cmd = 4
sz_sg = 1024
format_sg ='I 1024h H'

cmd_pwr_on = bytearray('cmd1', 'utf-8')
cmd_pwr_off = bytearray('cmd2', 'utf-8')
cmd_start_exti = bytearray('cmd3', 'utf-8')
cmd_start_conv = bytearray('cmd7', 'utf-8')
cmd_send_sg = bytearray('cmd9', 'utf-8')

ser = serial.Serial()
ser.baudrate= 256000
ser.port = 'COM2'
# ser.timeout = 0.1
ser.open()

class Packet(NamedTuple):
    cmd: int
    sg: Tuple[int, 1024]
    crc: int


def ask(ser, cmd):
    cmd.extend(crc16(cmd).to_bytes(2,'little'))
    ser.write(cmd)
    print(' '.join(format(x, '02x') for x in cmd))

def plot(sg):
    x = np.linspace(0, sz_sg/2e6, sz_sg)

    fig, ax = plt.subplots()
    ax.plot(x, sg)
    plt.show()

#ask(ser, cmd_start_exti[::-1], 4)
# 
# print(' '.join(format(x, '02x') for x in resp))

ask(ser, cmd_start_conv[::-1])
cmd = ser.read(4)
sg=ser.read(2048)
crc = ser.read(2)
print(len(sg))
sg1 = struct.unpack(f'{sz_sg}H', sg)
print(sg1)
plot(sg1) 

