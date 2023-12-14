BRIGHTMAX := $(shell cat /sys/class/backlight/intel_backlight/max_brightness)
INSTALLATION_TARGET := /usr/local/bin/,bright

build: bright.c
	gcc -o bright bright.c -D"BRIGHTMAX=${BRIGHTMAX}"

install:
	cmp -s brightener.py /usr/local/bin/brightener.py || sudo cp -i brightener.py /usr/local/bin
	cmp -s bright ${INSTALLATION_TARGET} || sudo cp -i bright ${INSTALLATION_TARGET}
	sudo chown root:root ${INSTALLATION_TARGET}
	sudo chmod u+s ${INSTALLATION_TARGET}
	sudo chmod a+x ${INSTALLATION_TARGET}
	cmp -s bright.desktop "${HOME}/.local/share/applications/bright.desktop" || sudo cp -i bright.desktop "${HOME}/.local/share/applications/"

test: build
	sudo ./bright
	sudo ./bright 56
	sudo ./bright +5
	sudo ./bright +
	sudo ./bright -3
	sudo ./bright -
	sudo ./bright x3
	sudo ./bright /2
	sudo ./bright ++2