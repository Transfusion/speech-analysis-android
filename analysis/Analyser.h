//
// Created by rika on 16/11/2019.
//

#ifndef SPEECH_ANALYSIS_ANALYSER_H
#define SPEECH_ANALYSIS_ANALYSER_H

#include <QColor>
#include <Eigen/Core>
#include <deque>
#include <thread>
#include <memory>
#include "../audio/AudioCapture.h"
#include "../lib/Formant/Formant.h"
#include "../audio/AudioDevices.h"

struct SpecFrame {
    double fs;
    int nfft;
    Eigen::ArrayXd spec;
};

class Analyser {
public:
    Analyser();

    void startThread();
    void stopThread();

    void toggle();

    void setInputDevice(int id);
    void setOutputDevice(int id);

    void setSpectrum(bool);
    void setFftSize(int);
    void setLinearPredictionOrder(int);
    void setMaximumFrequency(double);
    void setFrameSpace(const std::chrono::duration<double, std::milli> & frameSpace);
    void setWindowSpan(const std::chrono::duration<double> & windowSpan);

    [[nodiscard]] bool isAnalysing() const;
    [[nodiscard]] int getFftSize() const;
    [[nodiscard]] int getLinearPredictionOrder() const;
    [[nodiscard]] double getMaximumFrequency() const;

    [[nodiscard]] const std::chrono::duration<double, std::milli> & getFrameSpace() const;
    [[nodiscard]] const std::chrono::duration<double> & getWindowSpan() const;

    [[nodiscard]] int getFrameCount();

    [[nodiscard]] const SpecFrame & getSpectrumFrame(int iframe);
    [[nodiscard]] const Formant::Frame & getFormantFrame(int iframe);
    [[nodiscard]] double getPitchFrame(int iframe);

    [[nodiscard]] const SpecFrame & getLastSpectrumFrame();
    [[nodiscard]] const Formant::Frame & getLastFormantFrame();
    [[nodiscard]] double getLastPitchFrame();

    void setFrameCallback(std::function<void()> callback);

private:
    void _updateFrameCount();

    void mainLoop();
    void update();
    void applyWindow();
    void analyseSpectrum();
    void analysePitch();
    void resampleAudio(double newFs);
    void applyPreEmphasis();
    void analyseLp();
    void analyseFormantLp();
    void analyseFormantDeep();
    void applyMedianFilters();

    std::mutex audioLock;
    AudioCapture audioCapture;
    AudioDevices audioDevices;

    std::function<void()> newFrameCallback;

    // Parameters.
    std::chrono::duration<double, std::milli> frameSpace;
    std::chrono::duration<double> windowSpan;
    int frameCount;

    bool doAnalyse;
    int nfft;
    double maximumFrequency;
    int lpOrder;

    // Intermediate variables for analysis.
    Eigen::ArrayXd x;
    double fs;
    LPC::Frame lpcFrame;

    std::deque<SpecFrame> spectra;
    Formant::Frames formantTrack;
    std::deque<double> pitchTrack;

    SpecFrame lastSpectrumFrame;
    Formant::Frame lastFormantFrame;
    double lastPitchFrame;

    // Thread-related members
    std::thread thread;
    std::atomic<bool> running;
    std::mutex mutex;
};


#endif //SPEECH_ANALYSIS_ANALYSER_H
