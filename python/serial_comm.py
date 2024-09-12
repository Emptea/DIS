import serial
import time
import crcmod
import matplotlib.pyplot as plt
import numpy as np

crc16 = crcmod.mkCrcFun(0x18005, rev=True, initCrc=0xFFFF, xorOut=0x0000)
sz_cmd = 4

cmd_pwr_on = bytearray('cmd1', 'utf-8')
cmd_pwr_off = bytearray('cmd2', 'utf-8')
cmd_start_exti = bytearray('cmd3', 'utf-8')
cmd_start_conv = bytearray('cmd7', 'utf-8')
cmd_send_sg = bytearray('cmd9', 'utf-8')

ser = serial.Serial()
ser.baudrate= 256000
ser.port = 'COM2'
ser.timeout = 0.05
ser.open()



def ask(ser, cmd, sz_resp):
    cmd.extend(crc16(cmd).to_bytes(2,'little'))
    ser.write(cmd)
    print(' '.join(format(x, '02x') for x in cmd))
    resp=ser.read(sz_resp+2)
    print(' '.join(format(x, '02x') for x in resp))


#ask(ser, cmd_start_exti[::-1], 4)


ask(ser, cmd_start_conv[::-1], 4)
sg = ser.read(1024)
