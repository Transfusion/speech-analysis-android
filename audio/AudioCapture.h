//
// Created by clo on 12/09/2019.
//

#ifndef SPEECH_ANALYSIS_AUDIOCAPTURE_H
#define SPEECH_ANALYSIS_AUDIOCAPTURE_H

//#include <portaudio.h>
#include <Oboe.h>
#include <Eigen/Core>
#include "RingBuffer.h"

#define CAPTURE_DURATION 35.0
#define CAPTURE_SAMPLE_COUNT(sampleRate) ((CAPTURE_DURATION * sampleRate) / 1000)

#define BUFFER_SAMPLE_COUNT(sampleRate) (CAPTURE_SAMPLE_COUNT(sampleRate))

struct RecordContext {
    RingBuffer buffer;
//    PaSampleFormat format;
    oboe::AudioFormat AudioFormat; // https://developer.android.com/reference/android/media/AudioFormat.html#ENCODING_PCM_16BIT
};

class AudioCapture: public oboe::AudioStreamCallback {
public:
    AudioCapture();
    ~AudioCapture();

    void openInputDevice(int id);
    void openOutputDevice(int id);
    void startStream();
    void closeStream();

    /* static int readCallback(const void * input, void * output,
                     unsigned long frameCount,
                     const PaStreamCallbackTimeInfo * timeInfo,
                     PaStreamCallbackFlags statusFlags,
                     void * userData); */

    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override;

    [[nodiscard]]
    int getSampleRate() const noexcept;

    void readBlock(Eigen::ArrayXd & capture) noexcept;

private:
    // PaError err;
    // PaStream * stream;

    // PaStreamParameters parameters;

    // instead of storing the parameters we store the AudioStreamBuilder?
    oboe::AudioStreamBuilder * inputStreamBuilder = nullptr;
    float * inputFrames = nullptr;

    // the active input device stream
    oboe::AudioStream * inputStream = nullptr;
    double sampleRate;

    // Ring buffer
    struct RecordContext audioContext;

};

#endif //SPEECH_ANALYSIS_AUDIOCAPTURE_H
