#include <com_ziv_threads_MainActivity.h>
#include "include/jni_log_util.h"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define EXCEPTION_CLASS "java/lang/RuntimeException"

// worker线程参数
struct NativeWorkerArgs {
    int id;
    int iternation;
};

// 缓存方法ID
static jmethodID nativeMessageMethod = NULL;

// Java虚拟机接口
static JavaVM *gVm = NULL;
// Java对象的全局引用
static jobject gObj = NULL;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    // 缓存Java虚拟机
    gVm = vm;

    return JNI_VERSION_1_6;
}

/*
 * Class:     com_ziv_threads_MainActivity
 * Method:    nativeInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_ziv_threads_MainActivity_nativeInit
        (JNIEnv *env, jobject obj) {
    if (NULL == gObj) {
        gObj = env->NewGlobalRef(obj);
        if (NULL == gObj) {
            LOGE("GlobalRef obj is failed.");
            return;
        }
    }
    if (NULL == nativeMessageMethod) {
        jclass clazz = env->GetObjectClass(obj);
        nativeMessageMethod = env->GetMethodID(clazz, "onNativeMessage", "(Ljava/lang/String;)V");
        if (NULL == nativeMessageMethod) {
            LOGE("Native message method is not fount.");
            // 抛出异常
            jclass exceptionClazz = env->FindClass(EXCEPTION_CLASS);
            env->ThrowNew(exceptionClazz, "Unable to find method.");
        }
    }
}

/*
 * Class:     com_ziv_threads_MainActivity
 * Method:    nativeFree
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_ziv_threads_MainActivity_nativeFree
        (JNIEnv *env, jobject obj) {
    // 释放全局变量引用
    if (NULL != gObj) {
        env->DeleteGlobalRef(gObj);
        gObj = NULL;
    }
}

/*
 * Class:     com_ziv_threads_MainActivity
 * Method:    nativeWorker
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_ziv_threads_MainActivity_nativeWorker
        (JNIEnv *env, jobject obj, jint id, jint iterations) {
    for (int i = 0; i < iterations; ++i) {
        // 准备消息
        char message[26];
        sprintf(message, "Worker %d: Iteration %d", id, i);
        jstring messageStr = env->NewStringUTF(message);
        // 调用Java方法
        env->CallVoidMethod(obj, nativeMessageMethod, messageStr);
        // 是否产生异常检查
        if (NULL != env->ExceptionOccurred()) {
            break;
        }
        // 睡眠1秒
        sleep(1);
    }
}

/**
 * 原生线程启动程序
 *
 * @param args
 * @return
 */
static void *nativeWorkerThread(void *args) {
    JNIEnv *env = NULL;
    // 将线程附加到Java虚拟机上
    if (JNI_FALSE == gVm->AttachCurrentThread(&env, NULL)) {
        // 获取worker参数
        NativeWorkerArgs *nativeWorkerArgs = (NativeWorkerArgs *) args;

        Java_com_ziv_threads_MainActivity_nativeWorker(env, gObj, nativeWorkerArgs->id,
                                                       nativeWorkerArgs->iternation);

        // 释放原生参数
        delete nativeWorkerArgs;
        // 分离线程和虚拟机
        gVm->DetachCurrentThread();
    }
    // 县城结束后立刻返回
    return (void *) JNI_TRUE;
}

/*
 * Class:     com_ziv_threads_MainActivity
 * Method:    posixThreads
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_ziv_threads_MainActivity_posixThreads
        (JNIEnv *env, jobject obj, jint threads, jint iterations) {
    // 线程句柄
    pthread_t* handles = new pthread_t[threads];
    for (int i = 0; i < threads; ++i) {
        NativeWorkerArgs *nativeWorkerArgs = new NativeWorkerArgs();
        nativeWorkerArgs->id = i;
        nativeWorkerArgs->iternation = iterations;

        // 创建新线程
        int result = pthread_create(&handles[i], NULL, nativeWorkerThread, (void *) nativeWorkerArgs);

        if (0 != result) {
            // 抛出异常
            jclass exceptionClazz = env->FindClass(EXCEPTION_CLASS);
            env->ThrowNew(exceptionClazz, "Unable to find method.");
            return;
        }
    }

    // 等待线程结束
    for (int i = 0; i < threads; ++i) {
        void* result = NULL;
        // 连接每个线程句柄
        if (0 != pthread_join(handles[i], &result)) {
            // 抛出异常
            jclass exceptionClazz = env->FindClass(EXCEPTION_CLASS);
            env->ThrowNew(exceptionClazz, "Unable to join thread.");
        } else {
            // 准备Message
            char message[26];
            sprintf(message, "Worker %d: Iteration %d", i, result);
            jstring messageStr = env->NewStringUTF(message);
            // 调用Java方法
            env->CallVoidMethod(obj, nativeMessageMethod, messageStr);
            // 是否产生异常检查
            if (NULL != env->ExceptionOccurred()) {
                return;
            }
        }
    }
}