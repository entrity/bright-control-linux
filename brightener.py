# Provides a GUI of buttons for quick, repeated clicks to adjust monitor brightness.

from Tkinter import *
from subprocess import Popen, PIPE
from threading import Timer
import ImageTk
import re

def getImg(basename):
	img = ImageTk.Image.open("/home/markham/Pictures/Clipart/"+basename)
	img.resize((150,150), ImageTk.Image.ANTIALIAS)
	pi = ImageTk.PhotoImage(img)
	return pi

class CP(Frame):
	def __init__(self, parent):
		self.parent = parent
		Frame.__init__(self, parent)
		self.initUI()
		self.parent.bind("<Control-q>", self.close)
		self.parent.bind("<Control-w>", self.close)
		self.set_and_get(None)

	def initUI(self):
		self.pack(fill=BOTH, expand=True)

		self.imgBelmont = getImg("crops/belmont-red-128x128.jpg")
		self.imgBelmon2 = getImg("crops/belmont-cross-128x128.jpg")
		self.imgGhost = getImg("crops/ghost-blade-128x128.jpg")
		self.imgGrave = getImg("grave-angel.png")

		imgFrame = Frame(self)
		imgFrame.pack(side=TOP)
		entryFrame = Frame(self)
		entryFrame.pack(side=BOTTOM)

		frame1 = Frame(imgFrame)
		frame1.pack(side=LEFT)
		up5Btn = Button(frame1, text="+5", image=self.imgBelmont, command=(lambda : self.set_and_get('+5')))
		up5Btn.pack(side=TOP, padx=5)
		dn5Btn = Button(frame1, text="-5", image=self.imgBelmon2, command=(lambda : self.set_and_get('-5')))
		dn5Btn.pack(side=BOTTOM, padx=5)

		frame2 = Frame(imgFrame)
		frame2.pack(side=RIGHT)

		up2Btn = Button(frame2, text="x2", image=self.imgGhost, command=(lambda : self.set_and_get('+')))
		up2Btn.pack(side=TOP, padx=5)
		dn2Btn = Button(frame2, text="/2", image=self.imgGrave, command=(lambda : self.set_and_get('-')))
		dn2Btn.pack(side=BOTTOM, padx=5)

		self.entry = Entry(entryFrame, width=4)
		self.entry.pack(side=LEFT, pady=8)
		entryBtn = Button(entryFrame, text="Change", command=(lambda : self.set_and_get(self.entry.get())))
		entryBtn.pack(side=RIGHT, pady=8)

	def set_and_get(self, arg):
		if hasattr(self, 'timer') and self.timer:
			self.timer.cancel()
		self.timer = Timer(7, self.close, (self,))
		self.timer.start()
		params = ['bright', arg] if arg else ['bright']
		p = Popen(params, stdout=PIPE)
		out, err = p.communicate()
		m = re.search('(\d+)\s*\Z', out)
		if m != None:
			self.entry.delete(0, END)
			self.entry.insert(0, m.group(1))

	def close(self, evt):
		self.parent.destroy()


if __name__ == '__main__':
	root = Tk()
	root.geometry("276x310")
	root.title("Brighten Your Day")
	app = CP(root)
	root.mainloop()
