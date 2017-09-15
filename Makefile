build: bright.c
	gcc -o bright bright.c

install:
	cmp -s brightener.py /usr/local/bin/brightener.py || cp -i brightener.py /usr/local/bin
	chown root:root bright
	cmp -s bright /usr/local/bin/bright || cp -i bright /usr/local/bin/
	chmod u+s bright
	chmod a+x bright
	cmp -s bright.desktop "${HOME}/.local/share/applications/bright.desktop" || cp -i bright.desktop "${HOME}/.local/share/applications/"
