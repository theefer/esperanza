TEMPLATE = lib
CONFIG += static
include (../../../config.pri)

HEADERS += abstract_page.h ftwmanager.h welcome.h \
		   finish.h page.h ftwdialog.h \
		   gui.h server.h media.h

SOURCES += ftwmanager.cpp page.cpp \
		   gui.cpp server.cpp media.cpp

INTERFACES += welcome.ui page.ui finish.ui \
			  ftwdialog.ui gui.ui server.ui \
			  media.ui


