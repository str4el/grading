TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += GradingWindow.h \
    Option.h \
    presets.h \
    GradingBuild.h
FORMS += GradingWindow.ui
SOURCES += GradingWindow.cpp \
    Option.cpp \
    main.cpp \
    presets.cpp \
    GradingBuild.cpp
