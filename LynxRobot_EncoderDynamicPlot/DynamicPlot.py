import os
import pprint
import random
import sys
import wx

RedrawRate = 90 # Refresh interval in milisecond
xAxisMinValue = 0
import matplotlib
matplotlib.use('WXAgg')
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import \
    FigureCanvasWxAgg as FigCanvas, \
    NavigationToolbar2WxAgg as NavigationToolbar
import numpy as np
import pylab
from matplotlib.pyplot import *

from ArduinoSerialData import SerialChannel as DataGen


class BoundControlBox(wx.Panel):
    def __init__(self, parent, ID, label, initval):
        wx.Panel.__init__(self, parent, ID)
        
        self.value = initval
        
        box = wx.StaticBox(self, -1, label)
        sizer = wx.StaticBoxSizer(box, wx.VERTICAL)
                
        self.SetSizer(sizer)
        sizer.Fit(self)
    
class GraphFrame(wx.Frame):

    title = 'Lynx Robot - Encoder Values'    

    def __init__(self):
        wx.Frame.__init__(self, None, -1, self.title)
        
        self.datagen = DataGen()
        tmp = self.datagen.next()
        self.data = [tmp[0]]
        self.data2 = [tmp[1]]
        
        self.create_menu()
        self.create_status_bar()
        self.create_main_panel()
        
        self.update_timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.UpdatePlot_Timer, self.update_timer)        
        self.update_timer.Start(RedrawRate)

    def create_menu(self):
        self.menubar = wx.MenuBar()
        
        menu_file = wx.Menu()
        m_expt = menu_file.Append(-1, "&Save plot\tCtrl-S", "Save plot to file")
        self.Bind(wx.EVT_MENU, self.SavePlot, m_expt)
        menu_file.AppendSeparator()
        m_exit = menu_file.Append(-1, "E&xit\tCtrl-X", "Exit")
        self.Bind(wx.EVT_MENU, self.on_exit, m_exit)
                
        self.menubar.Append(menu_file, "&File")
        self.SetMenuBar(self.menubar)

    def create_main_panel(self):
        self.panel = wx.Panel(self)

        self.init_plot()
        self.canvas = FigCanvas(self.panel, -1, self.fig)

        self.vbox = wx.BoxSizer(wx.VERTICAL)
        self.vbox.Add(self.canvas, 1, flag=wx.LEFT | wx.TOP | wx.GROW)        
        self.vbox.Add(wx.BoxSizer(wx.HORIZONTAL), 0, flag=wx.ALIGN_LEFT | wx.TOP)
        
        self.panel.SetSizer(self.vbox)
        self.vbox.Fit(self)
    
    def create_status_bar(self):
        self.statusbar = self.CreateStatusBar()

    def init_plot(self):
        self.dpi = 100
        self.fig = Figure((8.0, 8.0), dpi=self.dpi)

        self.axes = self.fig.add_subplot(111)
        self.axes.set_axis_bgcolor('black')
        self.axes.set_ylabel('Encoder Values', fontsize = 15)
        self.axes.set_xlabel('Reading Step', fontsize = 15)

        pylab.setp(self.axes.get_xticklabels(), fontsize = 8)
        pylab.setp(self.axes.get_yticklabels(), fontsize = 8)

        self.plot_data = self.axes.plot(
            self.data, 
            linewidth = 1,
            color=(0, 1, 0),
            label = "Left Encoder"
            )[0]
        self.plot_data2 = self.axes.plot(
            self.data2, 
            linewidth = 1,
            color = (1, 0, 0),
            label = "Right Encoder"
            )[0]
        self.axes.legend(bbox_to_anchor = (0.5, 1.0), loc = 8, prop = {'size' : 10.0}, borderaxespad = 0.)

    def UpdatePlot(self):
        xmax = max(len(self.data), len(self.data2)) if max(len(self.data), len(self.data2)) > 50 else 50
        xmin = xAxisMinValue
        
        ymax = round(max(self.data + self.data2)) + 10
        ymin = round(min(self.data + self.data2)) - 10

        self.axes.set_xbound(lower = xmin, upper = xmax)
        self.axes.set_ybound(lower = ymin, upper = ymax)
        self.axes.grid(True, color = 'gray')
        
        self.plot_data.set_xdata(np.arange(max(len(self.data), len(self.data2))))
        self.plot_data.set_ydata(np.array(self.data))

        self.plot_data2.set_xdata(np.arange(max(len(self.data), len(self.data2))))
        self.plot_data2.set_ydata(np.array(self.data2))
       
        self.canvas.draw()
    
    def SavePlot(self, event):
        file_choices = "PNG (*.png)|*.png"
        
        dlg = wx.FileDialog(
            self, 
            message = "Save plot as...",
            defaultDir = os.getcwd(),
            defaultFile = "plot.png",
            wildcard = file_choices,
            style = wx.SAVE)
        
        if dlg.ShowModal() == wx.ID_OK:
            path = dlg.GetPath()
            self.canvas.print_figure(path, dpi = self.dpi)
    
    def UpdatePlot_Timer(self, event):
        tmp = self.datagen.next()
        print 'Values Received', tmp
        self.data.append(tmp[0])
        self.data2.append(tmp[1])

        self.UpdatePlot()
    
    def on_exit(self, event):
        self.Destroy()
    
if __name__ == '__main__':
    app = wx.App(False)
    app.frame = GraphFrame()
    app.frame.Show()
    app.MainLoop()