package com.ziv.threads;

/**
 * Created by ziv on 17-6-26.
 */

class LibraryAPI {
    public native void nativeInit();
    public native void nativeFree();
    public native void nativeWorker(int id, int iterations);
    public native void posixThreads(int threads, int iterations);

    static {
        System.loadLibrary("Threads");
    }
}
