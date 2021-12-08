QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    yolo.cpp

HEADERS += \
    Header.h \
    mainwindow.h \
    yolo.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
INCLUDEPATH += /usr/local/include/opencv4
LIBS += -L /usr/local/lib \
-lopencv_core \
-lopencv_highgui \
-lopencv_imgproc \
-lopencv_imgcodecs \
-lopencv_video.4.5.3 \
-lopencv_videoio.4.5.3 \
-lopencv_videostab.4.5.3 \
-lopencv_imgcodecs.4.5.3 \
#-lopencv_dnn.4.5.3 \
-lopencv_dnn \
#-lopencv_dnn_superres.4.5.3 \
#-lopencv_dnn_objdetect.4.5.3 \

#unix|win32: LIBS += -lopencv_core.4.5.3
