build: bright.c
	gcc -o bright bright.c

install:
	chown root bright
	chmod u+s bright
	mv bright /usr/local/bin/
