# Speech analysis software - Android

[![pipeline status](https://gitlab.com/Transfusion/speech-analysis-android/badges/develop/pipeline.svg)](https://gitlab.com/Transfusion/speech-analysis-android/-/commits/develop)

A port of [Clo Yun-Hee Dufour's software](https://github.com/ichi-rika/speech-analysis) to Android. Also an exercise in porting a Qt5 / PortAudio app to Android with maximal code reuse.

> This project is an attempt at making a software capable of analysing speech signals in real-time. The main focus as of writing this description is on pitch detection and formant analysis.
>
> -- <cite>Clo Yun-Hee Dufour</cite>

![Screenshot of me saying A I U E O into Formantdroid running on the Android AVD](https://i.imgur.com/JsnZ2g2.png)

## Building
You may refer to the commands inside [.gitlab-ci.yml](https://github.com/Transfusion/speech-analysis-android/blob/develop/.gitlab-ci.yml) and [build_native_dependencies.py](https://github.com/Transfusion/speech-analysis-android/blob/develop/build_native_dependencies.py) for an idea. The external native dependencies (fftw, oboe, and the lib folder) should be built first with
```sh
python3 build_native_dependencies.py --libs fftw3 oboe libspeech --sdk /path/to/android-ndk-r21
```
The [lib](https://github.com/Transfusion/speech-analysis-android/tree/develop/lib) folder is built as if it were an external dependency because only until the latest version of Qt does `cmake` support generating multi-ABI .apks, so I went with `qmake`.

I develop by importing the `.pro` file into Qt Creator 4.11.0 (Based on Qt 5.14.0) pointed at Android SDK 26.1.1 and NDK r21 in the Devices section of the Settings menu and hitting the green play button in either Debug or Release mode. I have successfully built on Ubuntu 18.04.4 LTS and macOS 10.14.6 and run this app on aarch64, armv7a, and x86_64 (the AVD) Android devices.

The host dependencies required to start compiling are [here](https://github.com/Transfusion/qt-android-multiarch-docker/blob/master/Dockerfile).


## Issues & Roadmap
Suggestions of any sort are welcome! Such as:
- Improving the usability of this app. I know that the UI looks like it was made for ants on a phone screen.
- Potentially turning this repo into a set of patches which can be auto-applied onto upstream?
- Interested in extending this app with other functionality?

Regardless, I will try to always maintain a branch that has feature parity with upstream by using Meld or a similar diff tool.