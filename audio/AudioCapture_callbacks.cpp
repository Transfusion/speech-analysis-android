//
// Created by clo on 13/09/2019.
//

#include "AudioCapture.h"
//#include <iostream>
#include <QDebug>

//using namespace Eigen;

oboe::DataCallbackResult AudioCapture::onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) {
//    qInfo() << "Input Audio Frame: " << numFrames;

    /* for (int i = 0; i < numFrames; ++i) {
        float sampleValue = kAmplitude * sinf(mPhase);
        for (int j = 0; j < kChannelCount; j++) {
            floatData[i * kChannelCount + j] = sampleValue;
        }
        mPhase += mPhaseIncrement;
        if (mPhase >= kTwoPi) mPhase -= kTwoPi;
    } */

    oboe::AudioFormat dataFormat = oboeStream->getFormat();
    //... later
    if (dataFormat == oboe::AudioFormat::I16) {
//        qInfo() << "INT16 AUDIO FORMAT";
//        floatData = malloc(sizeof(float) * numFrames);

        oboe::convertPcm16ToFloat((int16_t *) audioData, inputFrames, numFrames);
        Eigen::ArrayXd block = Eigen::Map<const Eigen::ArrayXf>(inputFrames, numFrames).cast<double>();

        audioContext.buffer.writeInto(block);
    } else {
//        qInfo() << "AUDIO FORMAT WITHIN CB " << (int32_t) dataFormat;
//        inputFrames = (float *) audioData;


        auto inPtr = static_cast<const float *>(audioData);
        Eigen::ArrayXd block;

//        auto inPtr = static_cast<const float *>(input);
        block = Eigen::Map<const Eigen::ArrayXf>(inPtr, numFrames).cast<double>();

        audioContext.buffer.writeInto(block);
    }



    return oboe::DataCallbackResult::Continue;
}

//template<typename T, T min, T max>
//static void convertToFloat(ArrayXd & dst, T * src, unsigned long len);

//int AudioCapture::readCallback(const void * input, void * output,
//                               unsigned long frameCount,
//                               const PaStreamCallbackTimeInfo * timeInfo,
//                               PaStreamCallbackFlags statusFlags,
//                               void * userData)
//{
//    auto ctx = static_cast<RecordContext *>(userData);

/* note from transfusion: block is a column vector
 * typedef Array<Scalar, RowsAtCompileTime, ColsAtCompileTime, Options> MyArrayType;
 * https://eigen.tuxfamily.org/dox/group__QuickRefPage.html
Array<double,Dynamic,1> */

//    ArrayXd block;

//    if (ctx->format == paFloat32) {
//        auto inPtr = static_cast<const float *>(input);
//        block = Map<const ArrayXf>(inPtr, frameCount).cast<double>();
//    }
//    else if (ctx->format == paInt32) {
//        auto inPtr = static_cast<const int32_t *>(input);
//        convertToFloat<const int32_t, 0, INT32_MAX>(block, inPtr, frameCount);
//    }
//    else if (ctx->format == paInt16) {
//        auto inPtr = static_cast<const int16_t *>(input);
//        convertToFloat<const int16_t, 0, INT16_MAX>(block, inPtr, frameCount);
//    }
//    else if (ctx->format == paInt8) {
//        auto inPtr = static_cast<const int8_t *>(input);
//        convertToFloat<const int8_t, 0, INT8_MAX>(block, inPtr, frameCount);
//    }
//    else if (ctx->format == paUInt8) {
//        auto inPtr = static_cast<const uint8_t *>(input);
//        convertToFloat<const uint8_t, 128, INT8_MAX>(block, inPtr, frameCount);
//    }

//    ctx->buffer.writeInto(block);

//    return paContinue;

//}

template<typename T, T offset, T max>
void convertToFloat(Eigen::ArrayXd & dst, T * src, unsigned long len)
{
   for (int i = 0; i < len; ++i) {
       dst(i) = static_cast<double>(offset + src[i]) / static_cast<double>(max);
   }
}
