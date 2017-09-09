# Provides a GUI of buttons for quick, repeated clicks to adjust monitor brightness.

from Tkinter import *
from subprocess import Popen, PIPE
from threading import Timer
import ImageTk
import re

RED  = 0b1
PLUS = 0b10
MUL  = 0b100

IMG_RED_ADD = 'crops/belmont-red-150.jpg'
IMG_RED_SUB = 'crops/belmont-blue-150.jpg'
IMG_RED_MUL = 'crops/battle-angel-alita.jpg'
IMG_RED_DIV = 'crops/art-gif-yuumei.gif'

IMG_LIGHT_ADD = 'crops/ghost-blade-3.jpg'
IMG_LIGHT_SUB = 'crops/ghost-blade-9.jpg'
IMG_LIGHT_MUL = 'crops/ghost-out-2.jpg'
IMG_LIGHT_DIV = 'crops/ghost-blade-2.jpg'

images = []
do_timer = True

def getImg(basename):
	img = ImageTk.Image.open("/home/markham/Pictures/Clipart/"+basename)
	img.resize((128,128), ImageTk.Image.ANTIALIAS)
	pi = ImageTk.PhotoImage(img)
	return pi

class CP(Frame):
	def __init__(self, parent):
		self.parent = parent
		Frame.__init__(self, parent)
		self.initUI()
		self.parent.bind("q", self.close)
		self.parent.bind("w", self.close)
		self.set_and_get(None)
		self.set_and_get(None, RED)

	def btn_and_lbl(self, frame, text, img, side, command):
		global images
		images.append(getImg(img))
		Label(frame, text=text).pack(side=side, padx=5)
		Button(frame, text=text, image=images[-1], command=command).pack(side=side, padx=5)


	def initUI(self):
		self.pack(fill=BOTH, expand=True)

		imgFrame = Frame(self)
		imgFrame.pack(side=TOP)
		entryFrame = Frame(self)
		entryFrame.pack(side=TOP)
		entryRedFrame = Frame(self)
		entryRedFrame.pack(side=TOP)

		frame = Frame(imgFrame)
		frame.pack(side=LEFT)
		self.btn_and_lbl(frame, 'red + 500', IMG_RED_ADD, TOP, lambda : self.set_and_get(-500, RED|PLUS))
		self.btn_and_lbl(frame, 'red - 500', IMG_RED_SUB, BOTTOM, lambda : self.set_and_get(500, RED|PLUS))

		frame = Frame(imgFrame)
		frame.pack(side=LEFT)
		self.btn_and_lbl(frame, 'red x 2', IMG_RED_MUL, TOP, lambda : self.set_and_get(0.5, RED|MUL))
		self.btn_and_lbl(frame, u'red \xf7 2', IMG_RED_DIV, BOTTOM, lambda : self.set_and_get(2, RED|MUL))

		frame = Frame(imgFrame)
		frame.pack(side=LEFT)
		self.btn_and_lbl(frame, '+5', IMG_LIGHT_ADD, TOP, lambda : self.set_and_get('+5'))
		self.btn_and_lbl(frame, '-5', IMG_LIGHT_SUB, BOTTOM, lambda : self.set_and_get('-5'))

		frame = Frame(imgFrame)
		frame.pack(side=RIGHT)
		self.btn_and_lbl(frame, 'x2', IMG_LIGHT_MUL, TOP, lambda : self.set_and_get('+'))
		self.btn_and_lbl(frame, u'\xf72', IMG_LIGHT_DIV, BOTTOM, lambda : self.set_and_get('-'))

		entryBtn = Button(entryFrame, text="Change", command=(lambda : self.set_and_get(self.entry.get())))
		entryBtn.pack(side=RIGHT, pady=8)
		self.entry = Entry(entryFrame, width=4)
		self.entry.pack(side=RIGHT, pady=8)
		self.entry_red = Label(entryFrame, width=5)
		self.entry_red.pack(side=LEFT, pady=8)
		Button(entryFrame, text='Reset red', command=(lambda : self.set_and_get('-x', RED)), pady=8).pack(side=LEFT)


	def set_and_get(self, arg, mode=0):
		if do_timer:
			if hasattr(self, 'timer') and self.timer:
				self.timer.cancel()
			self.timer = Timer(7, self.close, (self,))
			self.timer.start()
		if mode & RED:
			if mode & PLUS:
				arg = CP.get_redshift() + arg
			elif mode & MUL:
				arg = int(CP.get_redshift() * arg)
			params = ['my_redshift']
			if type(arg) == int:
				params += ['-O', str(arg)]
			elif arg != None:
				params.append(arg)
			p = Popen(params, stdout=PIPE)
			out, err = p.communicate()
			new_val = CP.get_redshift()
			self.entry_red.config(text=(str(new_val)+'K'))
		else:
			params = ['bright', arg] if arg else ['bright']
			p = Popen(params, stdout=PIPE)
			out, err = p.communicate()
			m = re.search('(\d+)\s*\Z', out)
			if m != None:
				self.entry.delete(0, END)
				self.entry.insert(0, m.group(1))

	@staticmethod
	def get_redshift():
		p = Popen(['my_redshift', '-d'], stdout=PIPE)
		out, err = p.communicate()
		m = re.search('Temperature\t(\d+)', out)
		if m != None:
			return int(m.group(1))

	def close(self, evt):
		self.parent.destroy()


if __name__ == '__main__':
	root = Tk()
	root.geometry("676x400")
	root.title("Brighten Your Day")
	app = CP(root)
	root.mainloop()
