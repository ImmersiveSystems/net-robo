from threading import Thread
import time
import serial
import sys

lastDataReceived = ''
def SerialListener(ser):
    global lastDataReceived
    SerialData = ''
    while True:
        SerialData = SerialData + ser.read(ser.inWaiting())
        if '\n' in SerialData:
            Values = SerialData.split('\n')
            if Values[-1]:           # To avoid losing data if Arduino sends a large number of empty lines
                lastDataReceived = Values[-2]                                            
            SerialData = Values[-1]

class SerialChannel(object):
    def __init__(self, init=50):
        try:
            self.ser = ser = serial.Serial(
                port = 'com4', #'/dev/ttyUSB0', #'com4',
                baudrate = 9600,
                bytesize = serial.EIGHTBITS,
                parity = serial.PARITY_NONE,
                stopbits = serial.STOPBITS_ONE,
                timeout = 0.1,
                xonxoff = 0,
                rtscts = 0,
                interCharTimeout = None
            )
        except serial.serialutil.SerialException:
            #no serial connection
            self.ser = None
        else:
            Thread(target = SerialListener, args = (self.ser,)).start()
        
    def next(self):
        if not self.ser:
            print 'No Serial Connection Detected'
            sys.exit(0)
        encoders = [0.0, 0.0] 
        if lastDataReceived != '':
            raw_line = lastDataReceived.split(' ')
            encoders[0] = float(raw_line[1])
            encoders[1] = float(raw_line[2])
        return encoders #return zeros if no correct value receievd

    def __del__(self):
        if self.ser:
            self.ser.close()

if __name__=='__main__':
    SerialChnl = SerialChannel()
    print SerialChnl.next()