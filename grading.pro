TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += GradingWindow.h \
    Option.h \
    GradingSave.h \
    GradingVariableContainer.h \
    GradingVersion.h
FORMS += GradingWindow.ui
SOURCES += GradingWindow.cpp \
    Option.cpp \
    main.cpp \
    GradingVariableContainer.cpp \
    GradingSave.cpp \
    GradingVersion.cpp
