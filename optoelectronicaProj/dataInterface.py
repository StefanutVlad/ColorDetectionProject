import serial
import sys
import _thread

#clasa preluare date despre port
class SerialPort:
    def __init__(self):
        self.comportName = ""
        self.baud = 0
        self.timeout = None
        self.ReceiveCallback = None
        self.isopen = False
        self.receivedMessage = None
        self.serialport = serial.Serial()

    def __del__(self):
        try:
            if self.serialport.is_open():
                self.serialport.close()
        except:
            print("Eroare destructor -> inchidere Port COM ", sys.exc_info()[0] )

    def RegisterReceiveCallback(self,aReceiveCallback):
        self.ReceiveCallback = aReceiveCallback
        try:
            _thread.start_new_thread(self.SerialReadlineThread, ())
        except:
            print("Eroare citire! ", sys.exc_info()[0])

    def SerialReadlineThread(self):
        while True:
            try:
                if self.isopen:
                    self.receivedMessage = self.serialport.readline()
                    if self.receivedMessage != "":
                        self.ReceiveCallback(self.receivedMessage)
            except:
                print("Eroare citire port COM! ", sys.exc_info()[0])

    def IsOpen(self):
        return self.isopen

    def Open(self,portname,baudrate):
        if not self.isopen:
            self.serialport.port = portname
            self.serialport.baudrate = baudrate
            try:
                self.serialport.open()
                self.isopen = True
            except:
                print("Eroare la deschiderea portului COM: ", sys.exc_info()[0])


    def Close(self):
        if self.isopen:
            try:
                self.serialport.close()
                self.isopen = False
            except:
                print("Eroare la inchiderea portului COM: ", sys.exc_info()[0])

    def Send(self,message):
        if self.isopen:
            try:
                newmessage = message.strip()
                newmessage += '\r\n'
                self.serialport.write(newmessage.encode('utf-8'))
            except:
                print("Eroare transmitere date: ", sys.exc_info()[0] )
            else:
                return True
        else:
            return False
