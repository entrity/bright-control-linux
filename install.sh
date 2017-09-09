sudo cp -i brightener.py /usr/local/bin
gcc -o bright bright.c
sudo mv -i bright /usr/local/bin/
sudo chown root:root /usr/local/bin/bright
sudo chmod +s /usr/local/bin/bright
sudo chmod a+x /usr/local/bin/bright
mv -n bright.desktop "$HOME/.local/share/applications/"
