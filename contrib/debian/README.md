
Debian
====================
This directory contains files used to package geekcashd/geekcash-qt
for Debian-based Linux systems. If you compile geekcashd/geekcash-qt yourself, there are some useful files here.

## geekcash: URI support ##


geekcash-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install geekcash-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your geekcash-qt binary to `/usr/bin`
and the `../../share/pixmaps/bitcoin128.png` to `/usr/share/pixmaps`

geekcash-qt.protocol (KDE)

