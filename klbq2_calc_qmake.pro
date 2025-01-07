QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp \
    httpclient.cpp

HEADERS += \
    widget.h \
    httpclient.h

FORMS += \
    widget.ui

TRANSLATIONS += \
    klbq2_calc_qmake_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

ANDROID_EXTRA_LIBS += \
    D:/ASDK-OPENSSL/ssl_3/arm64-v8a/libcrypto_3.so \
    D:/ASDK-OPENSSL/ssl_3/arm64-v8a/libssl_3.so

# Default rules for deployment.
android: include(D:/ASDK-OPENSSL/openssl.pri)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    reso.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/res/xml/qtprovider_paths.xml

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
