build: bright.c
	gcc -o bright bright.c

install:
	cp -i brightener.py /usr/local/bin
	chown root:root bright
	mv -i bright /usr/local/bin/
	chmod u+s bright
	chmod a+x bright
	mv -n bright.desktop "$HOME/.local/share/applications/"
