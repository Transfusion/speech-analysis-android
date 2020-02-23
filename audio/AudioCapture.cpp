//
// Created by clo on 12/09/2019.
//

#include <iostream>
#include <QDebug>
#include "AudioCapture.h"
#include "../Exceptions.h"

//static constexpr std::array<int, 2> preferredLayouts = {
//    1, 2
//};

//static constexpr std::array<PaSampleFormat, 5> preferredFormats = {
//    paFloat32,
//    paInt32,
//    paInt16,
//    paInt8,
//    paUInt8,
//};

//static constexpr std::array<double, 5> preferredSampleRates = {
//    48000,
//    44100,
//    32000,
//    22050,
//    16000,
//};

AudioCapture::AudioCapture() {

//    inputStream = nullptr;

    // no direct analogue in Oboe
    /* err = Pa_Initialize();
    if (err != paNoError) {
        throw PaException("Unable to initialise", err);
    } */ // no initialization needed with Oboe

}

AudioCapture::~AudioCapture() {

    closeStream();

    /* err = Pa_Terminate();
    if (err != paNoError) {
        std::cerr << "Unable to terminate: " << Pa_GetErrorText(err) << std::endl;
    } */

}


void AudioCapture::openInputDevice(int id) {
//    oboe::AudioStreamBuilder inputStreamBuilder;

    if (inputFrames != nullptr) {
        delete[](inputFrames);
    }


    if (inputStreamBuilder != nullptr) {
        delete(inputStreamBuilder);
    }

    inputStreamBuilder = new oboe::AudioStreamBuilder();
    inputStreamBuilder->setDirection(oboe::Direction::Input);
    inputStreamBuilder->setDeviceId(id);
    inputStreamBuilder->setChannelCount(1);
    inputStreamBuilder->setPerformanceMode(oboe::PerformanceMode::LowLatency);
    inputStreamBuilder->setCallback(this);
    inputStreamBuilder->setFormat(oboe::AudioFormat::Float);

    oboe::Result result = inputStreamBuilder->openStream(&inputStream);
    if (result == oboe::Result::OK && inputStream != nullptr) {
        sampleRate = getSampleRate();
        audioContext.buffer.setCapacity(BUFFER_SAMPLE_COUNT(sampleRate));
        qInfo() << "RingBuffer size " << BUFFER_SAMPLE_COUNT(sampleRate);

        qInfo() << "Channel Count of input device:" << inputStream->getChannelCount();
        if (inputStream->getFormat() == oboe::AudioFormat::I16) {
            inputFrames = new float[32768]; // highly unlikely to encounter more than this many samples in 1 frame
        }

        audioContext.AudioFormat = inputStream->getFormat();

    }




    /* memset(&parameters, 0, sizeof(parameters));
    parameters.device = id;
    parameters.suggestedLatency = Pa_GetDeviceInfo(id)->defaultLowInputLatency;
    parameters.hostApiSpecificStreamInfo = nullptr;

    for (int channelCount : preferredLayouts) {
        parameters.channelCount = channelCount;

        for (PaSampleFormat format : preferredFormats) {
            parameters.sampleFormat = format;

            for (double preferredSampleRate : preferredSampleRates) {

                err = Pa_IsFormatSupported(&parameters, nullptr, preferredSampleRate);
                if (err == paFormatIsSupported) {
                    sampleRate = preferredSampleRate;
                    goto found;
                }
            }
        }
    }

found:

    audioContext.format = parameters.sampleFormat;
    audioContext.buffer.setCapacity(BUFFER_SAMPLE_COUNT(sampleRate));

    err = Pa_OpenStream(
            &stream,
            &parameters,
            nullptr,
            sampleRate,
            paFramesPerBufferUnspecified,
            paClipOff,
            &readCallback,
            &audioContext);

    if (err != paNoError) {
        throw PaException("Unable to open input stream", err);
    } */
}

//void AudioCapture::openOutputDevice(int id) {

//    memset(&parameters, 0, sizeof(parameters));
//    parameters.device = id;
//    parameters.suggestedLatency = Pa_GetDeviceInfo(id)->defaultLowOutputLatency;
//    parameters.hostApiSpecificStreamInfo = nullptr;

//    for (int channelCount : preferredLayouts) {
//        parameters.channelCount = channelCount;

//        for (PaSampleFormat format : preferredFormats) {
//            parameters.sampleFormat = format;

//            for (double preferredSampleRate : preferredSampleRates) {

//                err = Pa_IsFormatSupported(nullptr, &parameters, preferredSampleRate);
//                if (err == paFormatIsSupported) {
//                    sampleRate = preferredSampleRate;
//                    goto found;
//                }
//            }
//        }
//    }

//found:

//    audioContext.format = parameters.sampleFormat;
//    audioContext.buffer.setCapacity(BUFFER_SAMPLE_COUNT(sampleRate));

//    err = Pa_OpenStream(
//            &stream,
//            nullptr,
//            &parameters,
//            sampleRate,
//            paFramesPerBufferUnspecified,
//            paClipOff,
//            &readCallback,
//            &audioContext);

//    if (err != paNoError) {
//        throw PaException("Unable to open output stream", err);
//    }
//}

void AudioCapture::startStream() {

//    err = Pa_StartStream(stream);
//    if (err != paNoError) {
//        throw PaException("Unable to start stream", err);
//    }

    if (inputStream != nullptr) {
        qDebug() << "inputstream ready";
        if (inputStream->requestStart() != oboe::Result::OK) {
            qDebug() << "Unable to start input stream";
        }
    }

}

void AudioCapture::closeStream() {
    qInfo() << "CLOSE STREAM CALLED";

    if (inputStream != nullptr) {
        oboe::Result res = inputStream->requestStop();
        qInfo() << "Oboe AudioStream close result " << convertToText(res);
//        err = Pa_CloseStream(stream);
//        if (err != paNoError) {
//            std::cerr << "Unable to close stream: " << Pa_GetErrorText(err) << std::endl;
//        }
        inputStream->close();
        delete inputStream;
    }
}


int AudioCapture::getSampleRate() const noexcept {
    const int sr = inputStream->getSampleRate();
//    qDebug() << "get sample rate" << sr;
    return sr;
}

void AudioCapture::readBlock(Eigen::ArrayXd & capture) noexcept {
    capture.conservativeResize(CAPTURE_SAMPLE_COUNT(sampleRate));
    audioContext.buffer.readFrom(capture);
}

