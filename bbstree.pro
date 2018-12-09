QT += widgets

HEADERS += \
        edge.h \
        node.h \
        graphwidget.h \
    bbstree.h

SOURCES += \
        edge.cpp \
        main.cpp \
        node.cpp \
        graphwidget.cpp \
    bbstree.cpp

# install
target.path = ./bbstree
INSTALLS += target
