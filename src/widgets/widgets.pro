TARGET = widgets
TEMPLATE = lib
CONFIG += static
include (../../config.pri)

INTERFACES =	prefshortcutwidget.ui \
				prefmultiselectionwidget.ui \ 
				prefselectionwidget.ui \
				prefstringwidget.ui \
				prefnumberwidget.ui \
				prefboolwidget.ui

HEADERS =		prefshortcutwidget.h \
				prefmultiselectionwidget.h \ 
				prefselectionwidget.h \
				prefstringwidget.h \
				prefnumberwidget.h \
				prefboolwidget.h

SOURCES =		prefshortcutwidget.cpp \
				prefmultiselectionwidget.cpp \ 
				prefselectionwidget.cpp \
				prefstringwidget.cpp \
				prefnumberwidget.cpp \
				prefboolwidget.cpp
