TEMPLATE = lib
CONFIG += static plugin
include (../../../config.pri)

INTERFACES +=	preferencesdialog.ui \
				prefshortcutwidget.ui \
				prefmultiselectionwidget.ui \ 
				prefselectionwidget.ui \
				prefstringwidget.ui \
				prefnumberwidget.ui \
				prefboolwidget.ui

HEADERS +=		preferencesdialog.h \
				plugin.h \
				prefshortcutwidget.h \
				prefmultiselectionwidget.h \ 
				prefselectionwidget.h \
				prefstringwidget.h \
				prefnumberwidget.h \
				prefboolwidget.h

SOURCES +=		preferencesdialog.cpp \
				plugin.cpp \
				prefshortcutwidget.cpp \
				prefmultiselectionwidget.cpp \ 
				prefselectionwidget.cpp \
				prefstringwidget.cpp \
				prefnumberwidget.cpp \
				prefboolwidget.cpp
