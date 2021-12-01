build: bright.c
	gcc -o bright bright.c

install:
	cmp -s brightener.py /usr/local/bin/brightener.py || sudo cp -i brightener.py /usr/local/bin
	cmp -s bright /usr/local/bin/bright || sudo cp -i bright /usr/local/bin/
	sudo chown root:root /usr/local/bin/bright
	sudo chmod u+s /usr/local/bin/bright
	sudo chmod a+x /usr/local/bin/bright
	cmp -s bright.desktop "${HOME}/.local/share/applications/bright.desktop" || sudo cp -i bright.desktop "${HOME}/.local/share/applications/"
