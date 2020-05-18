QT += core quick charts serialport
CONFIG += c++1z
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += $$files("*.cpp") \
    ../common/generated_libs/network_protocol/network_protocol.pb.c \
    ../common/third_party/nanopb/pb_common.c \
    ../common/third_party/nanopb/pb_decode.c \
    ../common/third_party/nanopb/pb_encode.c \
    ../common/libs/framing/framing.cpp \
    ../common/libs/checksum/checksum.cpp
SOURCES += $$files("$$PWD/../common/**/*.c")
HEADERS += $$files("*.h") \
    ../common/generated_libs/network_protocol/network_protocol.pb.h \
    ../common/third_party/nanopb/pb.h \
    ../common/third_party/nanopb/pb_common.h \
    ../common/third_party/nanopb/pb_decode.h \
    ../common/third_party/nanopb/pb_encode.h \
HEADERS += $$files("$$PWD/../common/libs/**/*.h")
INCLUDEPATH += $$PWD/../common/third_party/nanopb
INCLUDEPATH += $$PWD/../common/libs/checksum
INCLUDEPATH += $$PWD/../common/generated_libs/network_protocol
INCLUDEPATH += $$PWD/../common/libs/framing
RESOURCES += qml.qrc images/Logo.png
DISTFILES += images/Logo.png
TRANSLATIONS += ProjectVentilatorGUI_es_GT.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
