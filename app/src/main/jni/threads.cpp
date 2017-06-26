#include <com_ziv_threads_LibraryAPI.h>
#include "include/jni_log_util.h"

#include <stdio.h>
#include <unistd.h>

#define EXCEPTION_CLASS "java/lang/RuntimeException"

// 缓存方法ID
static jmethodID nativeMessageMethod = NULL;

/*
 * Class:     com_ziv_threads_LibraryAPI
 * Method:    nativeInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_ziv_threads_LibraryAPI_nativeInit
        (JNIEnv *env, jobject obj) {
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
 * Class:     com_ziv_threads_LibraryAPI
 * Method:    nativeFree
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_ziv_threads_LibraryAPI_nativeFree
        (JNIEnv *, jobject){

}

/*
 * Class:     com_ziv_threads_LibraryAPI
 * Method:    nativeWorker
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_ziv_threads_LibraryAPI_nativeWorker
        (JNIEnv *env, jobject obj, jint id, jint iterations){
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