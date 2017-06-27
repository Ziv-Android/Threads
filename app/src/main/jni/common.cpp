#include <jni.h>

//
// Created by ziv on 17-6-27.
//
#define EXCEPTION_CLASS "java/lang/RuntimeException"

int throwNewException(JNIEnv *env, const char *msg) {
    // 抛出异常
    jclass exceptionClazz = env->FindClass(EXCEPTION_CLASS);
    return env->ThrowNew(exceptionClazz, msg);
}