#!/usr/bin/python
# -*- coding: <<encoding>> -*-
#-------------------------------------------------------------------------------
#   <<project>>
# 
#-------------------------------------------------------------------------------

import enum
import wx
import sys
import numpy as np
from numpy import arange, sin, pi, cos
import scipy
from scipy.fft import fft, fftshift
import matplotlib
matplotlib.use('WXAgg')
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg, NavigationToolbar2WxAgg
from matplotlib.figure import Figure
class id(enum.IntEnum):
    BUTTON_plot = wx.ID_HIGHEST + 1
    BUTTON_load = BUTTON_plot + 1
    BUTTON_save = BUTTON_load + 1
    BUTTON_save2 = BUTTON_save + 1
    BUTTON_demodulate = BUTTON_save2 + 1
    SLIDER_FM1 = BUTTON_demodulate + 1
    SLIDER_FM2 = SLIDER_FM1 + 1
    SLIDER_FM3 = SLIDER_FM2 + 1
    LABEL_FM1 = SLIDER_FM3 + 1
    LABEL_FM2 = LABEL_FM1 + 1
    LABEL_FM3 = LABEL_FM2 + 1
    BUTTON_ADD_NOISE = LABEL_FM3 + 1
    NOISE_SLIDER = BUTTON_ADD_NOISE + 1
    MESSAGE_AMPLITUDE_SLIDER = NOISE_SLIDER + 1

class MyFrame(wx.Frame):
    def __init__(self, title):
  
        wx.Frame.__init__(self, None, title=title)
        self.title = "EEE 302 project"
        self.main_sizer = wx.BoxSizer(wx.HORIZONTAL)
        self.controls_sizer = wx.BoxSizer(wx.VERTICAL)
        self.plots_sizer = wx.GridSizer(3, 2, 0, 0)
        self.plot_button = wx.Button(self, id.BUTTON_plot, "Generate Controlled")
        self.load_button = wx.Button(self, id.BUTTON_load, "Load Audio")
        self.m_figure = Figure(dpi=self.GetDPI().GetWidth())
        self.m_axes = self.m_figure.add_subplot(3, 2, 1)
        self.m_canvas = FigureCanvasWxAgg(self, -1, self.m_figure)
        self.mf_figure = Figure(dpi=self.GetDPI().GetWidth())
        self.mf_axes = self.mf_figure.add_subplot(3, 2, 1)
        self.mf_canvas = FigureCanvasWxAgg(self, -1, self.mf_figure)
        self.s_figure = Figure(dpi=self.GetDPI().GetWidth())
        self.s_axes = self.s_figure.add_subplot(3, 2, 1)
        self.s_canvas = FigureCanvasWxAgg(self, -1, self.s_figure)
        self.sf_figure = Figure(dpi=self.GetDPI().GetWidth())
        self.sf_axes = self.sf_figure.add_subplot(3, 2, 1)
        self.sf_canvas = FigureCanvasWxAgg(self, -1, self.sf_figure)
        self.d_figure = Figure(dpi=self.GetDPI().GetWidth())
        self.d_axes = self.d_figure.add_subplot(3, 2, 1)
        self.d_canvas = FigureCanvasWxAgg(self, -1, self.d_figure)
        self.df_figure = Figure(dpi=self.GetDPI().GetWidth())
        self.df_axes = self.df_figure.add_subplot(3, 2, 1)
        self.df_canvas = FigureCanvasWxAgg(self, -1, self.df_figure)



        self.Bind(wx.EVT_CLOSE, self.OnClose)
        self.fm1_slider = wx.Slider(self, id.SLIDER_FM1, 20, 1, 2000)
        self.fm1_label = wx.StaticText(self, wx.ID_ANY, "fm1")
        self.fm2_slider = wx.Slider(self, id.SLIDER_FM2, 200, 1, 2000)
        self.fm2_label = wx.StaticText(self, wx.ID_ANY, "fm2")
        self.fm3_slider = wx.Slider(self, id.SLIDER_FM3, 2000, 1, 2000)
        self.fm3_label = wx.StaticText(self, wx.ID_ANY, "fm3")
        self.noise_slider = wx.Slider(self, id.NOISE_SLIDER, 1, 1, 2000)
        self.noise_slider_label = wx.StaticText(self, wx.ID_ANY, "noise stddev")
        self.message_amplitude_slider = wx.Slider(self, id.MESSAGE_AMPLITUDE_SLIDER, 1, 1, 20)
        self.message_amplitude_label = wx.StaticText(self, wx.ID_ANY, "am")
        self.noise_slider.Label = "stddev of normal/white noise"
        self.fm1 = 20
        self.fm2 = 200
        self.fm3 = 2000
        self.fc = 10000
        self.am = 1
        self.add_noise = 0
        self.stddev = 1000
        self.plots_sizer.Add(self.m_canvas, 1, wx.SHAPED | wx.CENTER, 0)
        self.plots_sizer.Add(self.mf_canvas, 1, wx.SHAPED | wx.CENTER, 0)
        self.plots_sizer.Add(self.s_canvas, 1, wx.SHAPED | wx.CENTER, 0)
        self.plots_sizer.Add(self.sf_canvas, 1, wx.SHAPED | wx.CENTER, 0)
        self.plots_sizer.Add(self.d_canvas, 1, wx.SHAPED | wx.CENTER, 0)
        self.plots_sizer.Add(self.df_canvas, 1, wx.SHAPED | wx.CENTER, 0)
        self.controls_sizer.Add(self.load_button, 0, wx.EXPAND | wx.CENTER, 0)
        self.controls_sizer.Add(self.plot_button, 0, wx.EXPAND | wx.CENTER, 0)
        self.controls_sizer.Add(self.fm1_label, 0, wx.EXPAND | wx.CENTER, 0)
        self.controls_sizer.Add(self.fm1_slider, 0, wx.EXPAND | wx.CENTER, 0)
        self.controls_sizer.Add(self.fm2_label, 0, wx.EXPAND | wx.CENTER, 0)
        self.controls_sizer.Add(self.fm2_slider, 0, wx.EXPAND | wx.CENTER, 0)
        self.controls_sizer.Add(self.fm3_label, 0, wx.EXPAND | wx.CENTER, 0)
        self.controls_sizer.Add(self.fm3_slider, 0, wx.EXPAND | wx.CENTER, 0)
        self.controls_sizer.Add(self.noise_slider_label, 0, wx.EXPAND | wx.CENTER, 0)
        self.controls_sizer.Add(self.noise_slider, 0, wx.EXPAND | wx.CENTER, 0)
        self.controls_sizer.Add(self.message_amplitude_label, 0, wx.EXPAND | wx.CENTER, 0)
        self.controls_sizer.Add(self.message_amplitude_slider, 0, wx.EXPAND | wx.CENTER, 0)
        self.main_sizer.Add(self.controls_sizer, 1, wx.EXPAND | wx.CENTER, 0)
        self.main_sizer.Add(self.plots_sizer, 10, wx.EXPAND | wx.CENTER, 0)
        self.SetSizer(self.main_sizer)
        self.SetAutoLayout(True)
        self.Fit()

        self.Bind(wx.EVT_BUTTON, self.OnPlot, None, id.BUTTON_plot)
        self.Bind(wx.EVT_BUTTON, self.OnAudioLoadButton, None, id.BUTTON_load)
        self.Bind(wx.EVT_SLIDER, self.OnSliderChange, None, id.SLIDER_FM1, id.SLIDER_FM3)
        self.Bind(wx.EVT_SLIDER, self.OnNoiseSliderChange, None, id.NOISE_SLIDER)
        self.Bind(wx.EVT_SLIDER, self.OnMessageAmplitudeSliderChange, None, id.MESSAGE_AMPLITUDE_SLIDER)


 
    def OnClose(self, event):
        self.Destroy()
    
    def OnPlot(self, event):
        self.m_axes.clear()
        self.mf_axes.clear()
        self.s_axes.clear()
        self.sf_axes.clear()
        self.d_axes.clear()
        self.df_axes.clear()

        fc = 10000
        fs = 2*(max(self.fm1, self.fm2, self.fm3) + fc)
        time = np.arange(0, 0.1, 1/fs)

        m = cos(2*pi*self.fm1*time) + cos(2*pi*self.fm2*time) + cos(2*pi*self.fm3*time)
        m = self.am * m

        s = cos(2*pi*fc*time + 2*pi*np.cumsum(m)/fs)


        n = len(time)
        print(n)
        f = np.arange(-n/2, n/2, 1) * (-fs/n)

        self.m_axes.plot(time , m)
        self.Layout()
        self.m_canvas.draw()
        self.m_canvas.Update()
        self.m_axes.relim()
        self.m_axes.autoscale_view()

        mf = fftshift(fft(m))
        self.mf_axes.plot(f , abs(mf))
        self.Layout()
        self.mf_canvas.draw()
        self.mf_canvas.Update()
        self.mf_axes.relim()
        self.mf_axes.autoscale_view()



        s = cos(2*pi*fc*time + 2*pi*np.cumsum(m)/fs)
        # im so tired teacher, i had to look this up
        # https://stackoverflow.com/questions/14058340/adding-noise-to-a-signal-in-python
        noise = np.random.normal(0, self.stddev*0.0000001, n)
        s_noise = s + noise
        self.s_axes.plot(time , s_noise)
        self.s_axes.plot(time , s)
        self.Layout()
        self.s_canvas.draw()
        self.s_canvas.Update()
        self.s_axes.relim()
        self.s_axes.autoscale_view()

        sf = fftshift(fft(s))
        sf_noise = fftshift(fft(sf))
        self.sf_axes.plot(f , sf_noise)
        self.sf_axes.plot(f , sf)
        self.Layout()
        self.sf_canvas.draw()
        self.sf_canvas.Update()
        self.sf_axes.relim()
        self.sf_axes.autoscale_view()

        d = np.diff(np.unwrap(np.angle(scipy.signal.hilbert(s))))/(2*pi)*fs
        d_noise = np.diff(np.unwrap(np.angle(scipy.signal.hilbert(s_noise))))/(2*pi)*fs


        self.d_axes.plot(time[1:], d_noise)
        self.d_axes.plot(time[1:], d)
        self.Layout()
        self.d_canvas.draw()
        self.d_canvas.Update()
        self.d_axes.relim()
        self.d_axes.autoscale_view()

        df = fftshift(fft(d))
        df_noise = fftshift(fft(d_noise))
        self.df_axes.plot(f[1:], df_noise)
        self.df_axes.plot(f[1:], df)
        self.Layout()
        self.df_canvas.draw()
        self.df_canvas.Update()
        self.df_axes.relim()
        self.df_axes.autoscale_view()

        mse = sum(
            (d - d_noise)**2
        )/len(d)

        wx.MessageBox(f"MSE: {mse}")
        save_path = wx.FileSelector("Choose a file to save", '', "export.wav", ".wav", ".wav", wx.FD_SAVE)
        scipy.io.wavfile.write(save_path, fs, m)

    
    def OnSliderChange(self,event):
        match event.GetId():
            case id.SLIDER_FM1:
                self.fm1 = event.GetInt()
                print(self.fm1)
            case id.SLIDER_FM2:
                self.fm2 = event.GetInt()
                print(self.fm2)
            case id.SLIDER_FM3:
                self.fm3 = event.GetInt()
                print(self.fm3)

    def OnNoiseSliderChange(self,event):
        self.stddev = event.GetInt()
        print(self.stddev)

    
    def OnAudioLoadButton(self, event): 
        path = wx.FileSelector("Choose a file to open", wx.EmptyString, wx.EmptyString, ".wav")
        fs, m = scipy.io.wavfile.read(path)
        # 1 channel only
        m = m[:, 0]
        self.m_axes.clear()
        self.mf_axes.clear()
        self.s_axes.clear()
        self.sf_axes.clear()
        self.d_axes.clear()
        self.df_axes.clear()

        fc = 10000
        time = np.arange(0, len(m)/fs, 1/fs)

        s = cos(2*pi*fc*time + 2*pi*np.cumsum(m)/fs)


        n = len(time)
        print(n)
        f = np.arange(-n/2, n/2, 1) * (-fs/n)

        self.m_axes.plot(time , m)
        self.Layout()
        self.m_canvas.draw()
        self.m_canvas.Update()
        self.m_axes.relim()
        self.m_axes.autoscale_view()

        mf = fftshift(fft(m))
        self.mf_axes.plot(f , abs(mf))
        self.Layout()
        self.mf_canvas.draw()
        self.mf_canvas.Update()
        self.mf_axes.relim()
        self.mf_axes.autoscale_view()



        s = cos(2*pi*fc*time + 2*pi*np.cumsum(m)/fs)
        # im so tired teacher, i had to look this up
        # https://stackoverflow.com/questions/14058340/adding-noise-to-a-signal-in-python
        noise = np.random.normal(0, self.stddev*0.0000001, n)
        s_noise = s + noise
        self.s_axes.plot(time , s_noise)
        self.s_axes.plot(time , s)
        self.Layout()
        self.s_canvas.draw()
        self.s_canvas.Update()
        self.s_axes.relim()
        self.s_axes.autoscale_view()

        sf = fftshift(fft(s))
        sf_noise = fftshift(fft(sf))
        self.sf_axes.plot(f , abs(sf_noise))
        self.sf_axes.plot(f , abs(sf))
        self.Layout()
        self.sf_canvas.draw()
        self.sf_canvas.Update()
        self.sf_axes.relim()
        self.sf_axes.autoscale_view()

        d = np.diff(np.unwrap(np.angle(scipy.signal.hilbert(s))))/(2*pi)*fs
        d_noise = np.diff(np.unwrap(np.angle(scipy.signal.hilbert(s_noise))))/(2*pi)*fs


        self.d_axes.plot(time[1:], d_noise)
        self.d_axes.plot(time[1:], d)
        self.Layout()
        self.d_canvas.draw()
        self.d_canvas.Update()
        self.d_axes.relim()
        self.d_axes.autoscale_view()

        df = fftshift(fft(d))
        df_noise = fftshift(fft(d_noise))
        self.df_axes.plot(f[1:], abs(df_noise))
        self.df_axes.plot(f[1:], abs(df))
        self.Layout()
        self.df_canvas.draw()
        self.df_canvas.Update()
        self.df_axes.relim()
        self.df_axes.autoscale_view()

        mse = sum(
            (d - d_noise)**2
        )/len(d)

        wx.MessageBox(f"MSE: {mse}")

        save_path = wx.FileSelector("Choose a file to save", '', "export.wav", ".wav", "*.wav", wx.FD_SAVE)
        scipy.io.wavfile.write(save_path, fs, m)

    
    def OnMessageAmplitudeSliderChange(self,event):
        self.am = event.GetInt()
        print(self.am)


app = wx.App()
top = MyFrame("EEE 302 Project")
top.Show()
app.MainLoop()