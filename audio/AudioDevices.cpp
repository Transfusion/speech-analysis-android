//
// Created by clo on 9/12/2019.
//

#include <iostream>
#include "AudioDevices.h"

#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <iostream>
#include <QDebug>

AudioDevices::AudioDevices()
{
    refreshList();
}

bool AudioDevices::refreshList()
{
    inputs.clear();
//    outputs.clear();


    // https://doc.qt.io/qt-5/qandroidjniobject.html#method-signatures

    qInfo() << "ABCDEFG12345";
    qInfo() << QAndroidJniObject::callStaticMethod<jint>(
                     "org/eu/transfusion/formantroidqt/MyJavaClass",
            "fibonacci", "(I)I", 10);


    /* QAndroidJniObject stringArray = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/TestClass",
                                                                              "stringArray"
                                                                              "(Ljava/lang/String;Ljava/lang/String;)[Ljava/lang/String;"
                                                                               string1.object<jstring>(),
                                                                               string2.object<jstring>()); */

    /* QAndroidJniObject::callStaticObjectMethod("org/eu/transfusion/formantroidqt/AudioUtilities",
                                                                                  "fibonacci",
                                                                                  "(I)I",
                                                                                   3);*/

    // AudioDeviceInfo[]	A (possibly zero-length) array of AudioDeviceInfo objects.
    QAndroidJniObject _inputsArray = QAndroidJniObject::callStaticObjectMethod("org/eu/transfusion/formantroidqt/AudioUtilities",
                                                                              "getAllAudioDevices",
                                                                              "(Landroid/content/Context;I)[Lorg/eu/transfusion/formantroidqt/AudioUtilities$AudioDeviceInfoDTO;",
                                                                               QtAndroid::androidContext().object(), 1);
    jobjectArray inputsArray = _inputsArray.object<jobjectArray>();

    QAndroidJniEnvironment qjniEnv;
    int n = qjniEnv->GetArrayLength(inputsArray);
    qInfo() << n;

    std::vector<int> inputAudioDeviceIDs(n);

    for (int i = 0; i < n; ++i) {
        const jobject inputAudioDevice = qjniEnv->GetObjectArrayElement(inputsArray, i);

        // AudioDeviceInfoDTO
        const jclass _class = qjniEnv->GetObjectClass(inputAudioDevice);
        const jfieldID audioDeviceIDFieldID = qjniEnv->GetFieldID(_class, "id", "I");
        const jfieldID audioDeviceTypeFieldID = qjniEnv->GetFieldID(_class, "type", "I");
        const jmethodID humanReadableNameMethodID = qjniEnv->GetMethodID(_class, "getHumanReadableName", "()Ljava/lang/String;");

        const jint inputAudioDeviceID = qjniEnv->GetIntField(inputAudioDevice, audioDeviceIDFieldID);
        inputAudioDeviceIDs.push_back(inputAudioDeviceID);
        const jint inputAudioDeviceType = qjniEnv->GetIntField(inputAudioDevice, audioDeviceTypeFieldID);

        if (defaultInputDeviceId == -1 && inputAudioDeviceType == 15) { // microphone
            defaultInputDeviceId = inputAudioDeviceID;
        }

        const jstring _humanReadableName = (jstring) qjniEnv->CallObjectMethod(inputAudioDevice, humanReadableNameMethodID);
        const char *humanReadableName = qjniEnv->GetStringUTFChars(_humanReadableName, 0);

        qInfo() << "input audio device";
        qInfo() << inputAudioDeviceID;
        qInfo() << humanReadableName;

        inputs.push_back({
            .id = inputAudioDeviceID,
            .name = humanReadableName,
        });
    }

    if (defaultInputDeviceId == -1) {
        // select a random audio device
        defaultInputDeviceId = inputAudioDeviceIDs[rand() % n];
    }

    // https://developer.android.com/reference/android/media/AudioDeviceInfo.html#getChannelCounts() should never be null, adding all audio devices...

//    int numDevices = Pa_GetDeviceCount();
//    if (numDevices < 0) {
//        std::cerr << "ERROR: Pa_CountDevices returned 0x" << std::hex << numDevices << std::dec << std::endl;
//        return false;
//    }

//    const PaDeviceInfo * info;

//    for (int id = 0; id < numDevices; ++id) {
//        info = Pa_GetDeviceInfo(id);

//        if (info->maxInputChannels > 0) {
//            inputs.push_back({
//                .id = id,
//                .name = info->name,
//            });
//        }

//        if (info->maxOutputChannels > 0) {
//            outputs.push_back({
//                .id = id,
//                .name = info->name
//            });
//        }
//    }

    return true;
}

const std::vector<AudioDevice> & AudioDevices::getInputs() const {
    return inputs;
}

//const std::vector<AudioDevice> & AudioDevices::getOutputs() const {
//    return outputs;
//}
