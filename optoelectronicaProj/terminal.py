
import tkinter as tk
import tkinter.scrolledtext as tkscrolledtext
from tkinter import *
from tkinter import filedialog
import dataInterface
import _thread
import time
import webbrowser
from tkinter import messagebox

# var Globale
serialPort = dataInterface.SerialPort()
logFile = None

root = tk.Tk() # fereastra
root.title( "Date seriale TCS230" )
# Dimensiuni fereastra
screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()
window_width = screen_width/2
window_height = screen_width/3
window_position_x = screen_width/2 - window_width/2
window_position_y = screen_height/2 - window_height/2
root.geometry('%dx%d+%d+%d' % (window_width, window_height, window_position_x, window_position_y))

# text Date Seriale
frame = tk.Frame(root, bg='cyan')
frame.pack(side="bottom", fill='both', expand='no')
textbox = tkscrolledtext.ScrolledText(master=frame, wrap='word', width=180, height=28) #width=caractere, height=linii
textbox.pack(side='bottom', fill='y', expand=True, padx=0, pady=0)
textbox.config(font="bold")

#label Port COM
label_comport = Label(root,width=10,height=2,text="Port-ul COM:")
label_comport.place(x=10,y=26)
label_comport.config(font="bold")

#box Port COM
comport_edit = Entry(root,width=10)
comport_edit.place(x=100,y=36)
comport_edit.config(font="bold")
comport_edit.insert(END,"COM3")

# functi callback date seriale
def OnReceiveSerialData(message):
    str_message = message.decode("utf-8")
    str_m="\n"
    textbox.insert('2.0', str_message)
    textbox.insert('1.0', str_m)

# inregistrare callback cu transmisie seriala
serialPort.RegisterReceiveCallback(OnReceiveSerialData)

def sdterm_main():
    root.after(500, sdterm_main)  # perioada rulare loop 500ms

#
#  Comenzi butoane
#
def OpenCommand():
    if button_openclose.cget("text") == 'Port COM deschis':
        comport = comport_edit.get()
        baudrate = baudrate_edit.get()
        serialPort.Open(comport,baudrate)
        button_openclose.config(text='Port COM inchis')
        textbox.insert('1.0', "Port COM deschis\r\n")
    elif button_openclose.cget("text") == 'Close COM Port':
        textbox.insert('1.0',"STOP log \r\n")
        serialPort.Close()
        button_openclose.config(text='Port COM deschis')
        textbox.insert('1.0',"Port COM inchis\r\n")


def ClearDataCommand():
    textbox.delete('1.0',END)

def SendDataCommand():
    message = senddata_edit.get()
    if serialPort.IsOpen():
        message += '\r\n'
        serialPort.Send(message)
        textbox.insert('2.0',message)
    else:
        textbox.insert('1.0', "Netrimis ->COM inchis\r\n")

def ReplayLogFile():
    try:
      if logFile != None:
        readline = logFile.readline()
        global serialPort
        serialPort.Send(readline)
    except:
      print("Exceptie")

def ReplayLogThread():
    while True:
        time.sleep(1.0)
        global logFile
        if serialPort.IsOpen():
            if logFile != None:
                ReplayLogFile()

def OpenLogFile():
    if not serialPort.IsOpen():
        textbox.insert('1.0', "Port COM deschis\r\n")
    else:
            try:
                root.filename = filedialog.askopenfilename(initialdir="/", title="Select file",
                                                           filetypes=(("log files", "*.log"), ("all files", "*.*")))
                global logFile
                logFile = open(root.filename,'r')
                _thread.start_new_thread(ReplayLogThread, ())

                textbox.insert('1.0', "Trimitere la COM de la : " + root.filename + "\r\n")
            except:
                textbox.insert('1.0', "Exceptie, nu s-a putut deschide\r\n")


#buton deschidere/inchidere port COM
button_openclose = Button(root,text="Port COM deschis",width=20,command=OpenCommand)
button_openclose.config(font="bold")
button_openclose.place(x=210,y=30)

#buton sergrete date-Rx
button_cleardata = Button(root,text="Stergere date Rx",width=20,command=ClearDataCommand)
button_cleardata.config(font="bold")
button_cleardata.place(x=210,y=72)

#buton trimitere date
button_senddata = Button(root,text="Mesaj Trimis",width=20,command=SendDataCommand)
button_senddata.config(font="bold")
button_senddata.place(x=420,y=72)


#
# Label-uri si box-uri
#

#box trimitere date
senddata_edit = Entry(root,width=34)
senddata_edit.place(x=620,y=78)
senddata_edit.config(font="bold")
senddata_edit.insert(END,"Message")

#label Baud Rate
label_baud = Label(root,width=10,height=2,text="Baud Rate:")
label_baud.place(x=10,y=70)
label_baud.config(font="bold")

#box introdurete Baud Rate
baudrate_edit = Entry(root,width=10)
baudrate_edit.place(x=100,y=80)
baudrate_edit.config(font="bold")
baudrate_edit.insert(END,"38400")

#
# MAIN
#
root.after(500, sdterm_main)
root.mainloop()
#