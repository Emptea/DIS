import serial
import time
import crcmod

crc16 = crcmod.mkCrcFun(0x18005, rev=True, initCrc=0xFFFF, xorOut=0x0000)
sz_cmd = 4
cmd2send = bytearray('ping', 'utf-8')

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

ask(ser, cmd2send[::-1], 4)