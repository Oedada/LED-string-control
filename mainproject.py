import kivy
kivy.require("1.0.7")
from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.textinput import TextInput
from kivy.core.window import Window
from kivy.uix.button import Button
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.textinput import TextInput
from kivy.uix.slider import Slider
from kivy.uix.image import Image 
import socket
import time
flagDYN = 0
sumRGB = 0
serverMACAddress = '00:22:06:01:59:AB'  # Put your HC-05 address here
port = 1  # Match the setting on the HC-05 module
s = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
s.connect((serverMACAddress,port))
print("Connected. Type something...")
Window.clearcolor = (1, 1, 1, 1)            
class Light(App):
    
    def build(self):
        layout = FloatLayout()
        self.blue = TextInput(
        size_hint = (.1,.08),
        pos_hint={'x': .7, 'y':.2},
        multiline=False,
        text = ('000')
        )
        self.OFF = Button(
        text = "Выключить ленту",
        size_hint = (.2,.08),
        pos_hint={'x': .01, 'y':.9},
        background_color = [0,.47,.100,1],
        background_normal = '',
        on_press = self.OFFfun   
        )
        self.red = TextInput(
        size_hint = (.1,.08),
        pos_hint={'x': .2, 'y':.2},
        multiline=False,
        text = ('000')
        )
        self.green = TextInput(
        size_hint = (.1,.08),
        pos_hint= {'x': .45, 'y': .2},
        multiline=False,
        text = ('000')
        )
        submit = Button(
        text = 'Отправить RGB покзатели',
        size_hint = (.3,.08),
        pos_hint={'x': 0.35, 'y':.05},
        background_color = [0,.47,.100,1],
        background_normal = '',
        on_press = self.SubmitRGB
        )
        self.RGB = Label(
        markup = True,    
        text = '[color=#FF0000]R[/color][color=#00FF00]G[/color][color=#0000FF]B[/color]',
        size_hint = (.2,.15),
        pos_hint={'x': 0.4, 'y':.25}
        )
        self.REDlb = Label(
        markup = True,    
        text = '[color=#FF0000]Красный[/color]',
        size_hint = (.1,.08),
        pos_hint={'x': .2, 'y':.25},
        )
        self.GREENlb = Label(
        markup = True,    
        text = '[color=#00FF00]Зелёный[/color]',
        size_hint = (.1,.08),
        pos_hint= {'x': .45, 'y': .25}
        )
        self.BLUElb = Label(
        markup = True,    
        text = '[color=#0000FF]Синий[/color]',
        size_hint = (.1,.08),
        pos_hint={'x': .7, 'y':.25}
        )
        self.RainbowStatic = Button(
        text = 'Радуга',
        size_hint = (.1,.08),
        pos_hint={'x': .2, 'y':.05},
        background_color = [0,.47,.100,1],
        background_normal = '',
        on_press = self.RainbowStaticFun
        )
        self.RainbowDynamic = Button(
        text = 'Динамическая радуга',
        size_hint = (.2,.08),
        pos_hint={'x': .65, 'y':.05},
        background_color = [.19,.55,.91,1],
        background_normal = '',
        on_press = self.RainbowDynamicFun
        )
        self.Bright = TextInput(
        size_hint = (.1,.08),
        pos_hint={'x': .8, 'y':.8},
        multiline=False,
        text = "0"
        )
        self.Brightlb = Label(
        markup = True,    
        text = '[color=#3BE7E1]Яркость[/color]',
        size_hint = (.1,.08),
        pos_hint={'x': .8, 'y':.87},
        )
        self.HUEbt = Button(
        size_hint = (.2,.08),
        pos_hint={'x': .01, 'y':.6},    
        background_color = [.19,.55,.91,1],
        background_normal = '',
        on_press = self.HUEfun,
        text = "Submit HUE"  
        )
        self.HUElb = Image(source='HUElb.png',size_hint = (.2,.04),pos_hint = {'center_x':0.5,'center_y':0.5},fit_mode = ("contain"))
        self.HUE = Image(source='HUE.png',size_hint = (.4,.08),pos_hint = {'center_x':0.5,'center_y':0.4},fit_mode = ("contain"))
        self.slideHUE = Slider(min=0, max=255, value=0,value_track_color=[1, 0, 0, 1], size_hint = (.4,.08),pos_hint={'center_x':0.5,'center_y':0.4},border_horizontal = (0,1,0,1))
        self.Brightness = Button(
        text = 'Отправить яркость',
        size_hint = (.2,.08),
        pos_hint={'x': .01, 'y':.7},
        background_color = [.19,.55,.91,1],
        background_normal = '',
        on_press = self.Brightfun
        )
        layout.add_widget(self.OFF)
        layout.add_widget(self.HUEbt)
        layout.add_widget(self.Brightness)
        layout.add_widget(self.Brightlb)
        layout.add_widget(self.Bright)  
        layout.add_widget(self.RainbowDynamic)
        layout.add_widget(self.RainbowStatic)
        layout.add_widget(self.HUElb)
        layout.add_widget(self.HUE)
        layout.add_widget(self.slideHUE)
        layout.add_widget(self.REDlb)
        layout.add_widget(self.BLUElb)
        layout.add_widget(self.GREENlb)
        layout.add_widget(self.RGB)
        layout.add_widget(self.blue)
        layout.add_widget(self.red)
        layout.add_widget(self.green)
        layout.add_widget(submit)
        layout.add_widget(self.label)
        return layout
    def __init__(self):
        super().__init__()
        self.label = Label(text='',size_hint = (.5,1),pos_hint={'x': .5, 'y':.8})



    def RainbowStaticFun(self,*args):
        global flagDYN
        if flagDYN ==1:
            s.send(bytes("Remove", 'UTF-8'))
            time.sleep(2) 
            flagDYN = 0
        s.send(bytes("Rainbowstatic", 'UTF-8'))



    def RainbowDynamicFun(self,*args):
        s.send(bytes("Rainbowdynamic", 'UTF-8'))  
        flagDYN = 1
        print(flagDYN)



    def SubmitRGB(self,obj):
        global flagDYN
        if flagDYN == 1:
            s.send(bytes("Remove", 'UTF-8'))
            time.sleep(2)
            flagDYN = 0  
        s.send(bytes("RGB", 'UTF-8')) 
        sumRGB = str(str(self.red.text)+str(self.green.text)+str(self.blue.text))
        time.sleep(2) 
        s.send(bytes(sumRGB, 'UTF-8'))
        print(sumRGB)
        self.red.text = "000"
        self.green.text = "000"
        self.blue.text = "000"



    def Brightfun(self,*args):
        global flagDYN
        if flagDYN ==1:
            s.send(bytes("Remove", 'UTF-8'))
            time.sleep(2)
            print("HUEok")
            flagDYN = 0
        s.send(bytes("Brightness", 'UTF-8')) 
        time.sleep(2)
        s.send(bytes(self.Bright.text, 'UTF-8'))
        self.Bright.text = "0"



    def HUEfun(self,*args):
        global flagDYN
        if flagDYN == 1:
            s.send(bytes("Remove", 'UTF-8')) 
            time.sleep(1.5)
            flagDYN = 0
        s.send(bytes("HUE", 'UTF-8'))  
        time.sleep(2)
        print(self.slideHUE.value)
        HUEvalue = (self.slideHUE.value)//1
        s.send(bytes(str(HUEvalue), 'UTF-8'))
        


    def OFFfun(self,*args):
        global flagDYN
        if flagDYN == 1:
            s.send(bytes("Remove", 'UTF-8'))
            time.sleep(2)
            flagDYN = 0
        s.send(bytes("Off", 'UTF-8'))       
        
         
      
            

if __name__ == '__main__':
    Light().run()
    