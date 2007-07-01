TEMPLATE = lib
CONFIG += static plugin
include (../../../config.pri)

# this code was copied from Psi (http://psi-im.org)
HEADERS += $$PWD/globalshortcutmanager.h $$PWD/globalshortcuttrigger.h
SOURCES += $$PWD/globalshortcutmanager.cpp
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD


unix:!mac {
	SOURCES += $$PWD/globalshortcutmanager_x11.cpp
}
win32: {
	SOURCES += $$PWD/globalshortcutmanager_win.cpp
}
mac: {
	SOURCES += $$PWD/globalshortcutmanager_mac.cpp
}
