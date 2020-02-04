package org.eu.transfusion.formantroidqt;

import android.content.Context;
import android.media.AudioManager;
import android.media.AudioDeviceInfo;

// https://developer.android.com/reference/android/media/AudioDeviceInfo.html#TYPE_BUILTIN_MIC
// javap -s -public -classpath ~/Library/Android/sdk/platforms/android-26/android.jar:/Users/transfusion/build-SpeechAnalysis-Android_for_arm64_v8a_Clang_Qt_5_12_6_for_Android_ARM64_v8a-Debug/android-build/build/intermediates/javac/debug/compileDebugJavaWithJavac/classes org.transfusion.formantroidqt.AudioUtilities

public class AudioUtilities {

    static class AudioDeviceInfoDTO {

        public int id;
        public int type;
        public String productName;
        public String address;

        public AudioDeviceInfoDTO(int id, int type, String productName, String address) {
            this.id = id;
            this.type = type;
            this.productName = productName;
            this.address = address;

            if (this.address != null) {
                this.address = this.address.trim();
            }
            if (this.address.length() == 0) {
                this.address = null;
            }
        }


        public String getHumanReadableName() {
            String s = String.format("Id: %d Type: %d", this.id, this.type);
            if (this.address != null) {
                s += " " + this.address;
            }
            return s;
        }
    }

    // this method will be called from C/C++
    public static int fibonacci(int n) {
        System.out.println("Fibonacci being called!!");

        if (n < 2) return n;
        return fibonacci(n-1) + fibonacci(n-2);
    }

    // https://forum.qt.io/topic/75827/get-context-in-java-code-run-from-c/3
    public static AudioDeviceInfoDTO[] getAllAudioDevices(Context context, int type) {
        System.out.println("getAllAudioDevices being called!");
        AudioManager am = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        AudioDeviceInfo[] devices = am.getDevices(type);

        AudioDeviceInfoDTO[] dtos = new AudioDeviceInfoDTO[devices.length];
        for (int i = 0; i < devices.length; i++) {
            dtos[i] = new AudioDeviceInfoDTO(devices[i].getId(), devices[i].getType(), devices[i].getProductName().toString(), devices[i].getAddress());
        }

        /*for (AudioDeviceInfo device : devices) {
            System.out.println(device.getAddress() + " " + String.valueOf(device.getType()) + " " + String.valueOf(device.getId()) + " " + String.valueOf(device.getProductName()));
        }*/
        return dtos;
    }

}
