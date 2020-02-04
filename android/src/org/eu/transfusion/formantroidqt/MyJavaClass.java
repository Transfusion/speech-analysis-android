/*
* https://www.qtdeveloperdays.com/sites/default/files/BogdanVatra_Extending_Qt_Android_Apps_with_JNI.pdf
*/

package org.eu.transfusion.formantroidqt;


class MyJavaNatives
{
    // declare the native method
    public static native void sendFibonaciResult(int n);
}

public class MyJavaClass
{
    // this method will be called from C/C++
    public static int fibonacci(int n) {
        System.out.println("Fibonacci being called!!");

        if (n < 2) return n;
        return fibonacci(n-1) + fibonacci(n-2);
    }
    public static void compute_fibonacci(int n) {
        // callback the native method with the computed result.
        MyJavaNatives.sendFibonaciResult(fibonacci(n));
    }

}
