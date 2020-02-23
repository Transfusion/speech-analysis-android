QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/eigen
INCLUDEPATH += $$PWD/lib/

# compiling both regular and android in the same project is
# probably too much of a hassle
#unix:!android:INCLUDEPATH += /usr/local/include
#unix:!android:LIBS += -L/usr/local/lib
#unix:!android:LIBS += -lfftw3

SOURCES += \
    analysis/Analyser.cpp \
    analysis/Analyser_mainLoop.cpp \
    analysis/parts/formants.cpp \
    analysis/parts/lpc.cpp \
    analysis/parts/median.cpp \
    analysis/parts/pitch.cpp \
    analysis/parts/preprocess.cpp \
    analysis/parts/resample.cpp \
    analysis/parts/spectrum.cpp \
    audio/AudioCapture.cpp \
    audio/AudioCapture_callbacks.cpp \
    audio/AudioDevices.cpp \
    audio/RingBuffer.cpp \
    gui/canvas/AnalyserCanvas.cpp \
    gui/qt/MainWindow.cpp \
#    lib/FFT/FFT.cpp \
#    lib/Formant/Formant.cpp \
#    lib/LPC/Frame/LPC_Frame.cpp \
#    lib/LPC/Frame/LPC_Frame_auto.cpp \
#    lib/LPC/Frame/LPC_Frame_burg.cpp \
#    lib/LPC/Frame/LPC_Frame_covar.cpp \
#    lib/LPC/Frame/LPC_Frame_huber.cpp \
#    lib/LPC/LPC.cpp \
#    lib/LPC/LPC_filter.cpp \
#    lib/LPC/LPC_huber.cpp \
#    lib/LPC/LPC_huber_stat.cpp \
#    lib/MFCC/MFCC.cpp \
#    lib/Math/Bairstow.cpp \
#    lib/Math/Polynomial.cpp \
#    lib/Math/Viterbi.cpp \
#    lib/Pitch/McLeod/autocorrelation.cpp \
#    lib/Pitch/McLeod/parabolic_interpolation.cpp \
#    lib/Pitch/McLeod/peak_picking.cpp \
#    lib/Pitch/Pitch_AMDF.cpp \
#    lib/Pitch/Pitch_DynWav.cpp \
#    lib/Pitch/Pitch_MPM.cpp \
#    lib/Signal/Filter.cpp \
#    lib/Signal/Resample.cpp \
#    lib/Signal/Window.cpp \
    main.cpp

HEADERS += \
    Exceptions.h \
    analysis/Analyser.h \
    audio/AudioCapture.h \
    audio/AudioDevices.h \
    audio/RingBuffer.h \
    gui/canvas/AnalyserCanvas.h \
    gui/qt/MainWindow.h \
#    lib/FFT/FFT.h \
#    lib/Formant/Formant.h \
#    lib/LPC/Frame/LPC_Frame.h \
#    lib/LPC/LPC.h \
#    lib/LPC/LPC_huber.h \
#    lib/MFCC/MFCC.h \
#    lib/Math/Bairstow.h \
#    lib/Math/Polynomial.h \
#    lib/Math/Viterbi.h \
#    lib/Pitch/McLeod/MPM.h \
#    lib/Pitch/Pitch.h \
#    lib/Signal/Filter.h \
#    lib/Signal/Resample.h \
#    lib/Signal/Window.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Libraries such as fftw3 go here
#STANDALONE_NDK_ROOT = /Users/transfusion/standalone_r20b_toolchain
NDK_ROOT = $$ANDROID_NDK_ROOT

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    INCLUDEPATH += $$PWD/fftw-3.3.8_compiled/aarch64/usr/local/include
    LIBS += -L$$PWD/fftw-3.3.8_compiled/aarch64/usr/local/lib

    INCLUDEPATH += $$PWD/oboe/include
    INCLUDEPATH += $$PWD/oboe/include/oboe
    LIBS += -L$$PWD/oboe_compiled/aarch64/usr/local/lib

#    INCLUDEPATH += $$PWD/lib # libspeech include!
    LIBS += -L$$PWD/libspeech_compiled/aarch64/lib
}

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
#    INCLUDEPATH += $$PWD/fftw-3.3.8_compiled/arm/usr/local/include
#    LIBS += -L$$PWD/fftw-3.3.8_compiled/arm/lib

#    INCLUDEPATH += $$PWD/oboe/include
#    INCLUDEPATH += $$PWD/oboe/include/oboe
#    LIBS += -L$$PWD/oboe_compiled/arm


#    INCLUDEPATH += $$PWD/libspeech_compiled/arm/include
#    LIBS += -L$$PWD/libspeech_compiled/arm/lib

#    LIBS += -L$$STANDALONE_NDK_ROOT/sysroot/usr/lib/arm-linux-androideabi/21

    INCLUDEPATH += $$PWD/fftw-3.3.8_compiled/arm/usr/local/include
    LIBS += -L$$PWD/fftw-3.3.8_compiled/arm/usr/local/lib

    INCLUDEPATH += $$PWD/oboe/include
    INCLUDEPATH += $$PWD/oboe/include/oboe
    LIBS += -L$$PWD/oboe_compiled/arm/usr/local/lib

#    INCLUDEPATH += $$PWD/lib # libspeech include!
    LIBS += -L$$PWD/libspeech_compiled/arm/lib
}

contains(ANDROID_TARGET_ARCH,x86) {
    INCLUDEPATH += $$PWD/fftw-3.3.8_compiled/i686/usr/local/include
    LIBS += -L$$PWD/fftw-3.3.8_compiled/i686/usr/local/lib

    INCLUDEPATH += $$PWD/oboe/include
    INCLUDEPATH += $$PWD/oboe/include/oboe
    LIBS += -L$$PWD/oboe_compiled/i686/usr/local/lib

    LIBS += -L$$PWD/libspeech_compiled/i686/lib
}

contains(ANDROID_TARGET_ARCH,x86_64) {
    INCLUDEPATH += $$PWD/fftw-3.3.8_compiled/x86_64/usr/local/include
    LIBS += -L$$PWD/fftw-3.3.8_compiled/x86_64/usr/local/lib

    INCLUDEPATH += $$PWD/oboe/include
    INCLUDEPATH += $$PWD/oboe/include/oboe
    LIBS += -L$$PWD/oboe_compiled/x86_64/usr/local/lib

    LIBS += -L$$PWD/libspeech_compiled/x86_64/lib
}

android {
    QT += androidextras
    LIBS += -L$$NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/sysroot
    LIBS += -lfftw3
    LIBS += -loboe
    LIBS += -lOpenSLES
    LIBS += -lspeech
    LIBS += -lfftw3
#    LIBS += -lstdc++
}

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/src/org/transfusion/formantroidqt/AudioUtilities.java \
    android/src/org/transfusion/formantroidqt/MyJavaClass.java \
#    lib/CMakeLists.txt

#contains(ANDROID_TARGET_ARCH,arm64-v8a) {
ANDROID_PACKAGE_SOURCE_DIR = \
    $$PWD/android
#}
