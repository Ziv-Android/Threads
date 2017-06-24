/**
 * 调用android的log功能，输出在控制台
 *
 * 使用说明：
 * 1. 关闭DEBUG开关没有任何日志输出(用于应用上线)
 * 2. 打开DEBUG开关，关闭ALL_LOG开关，仅显示错误信息(用于用户交付)
 * 3. 打开DEBUG和ALL_LOG所有开关，显示所有调试信息(用于开发)
 *
 * Created by ziv on 16-10-24.
 */
#ifndef JNILOG_JNI_LOG_UTIL_H
#define JNILOG_JNI_LOG_UTIL_H

#include <android/log.h>

#define DEBUG
#define ALL_LOG

#ifdef ALL_LOG
#define LOG_TAG "ALL_LOG"
#ifdef DEBUG
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#else
#define LOGI(...) do {} while (0)
#define LOGW(...) do {} while (0)
#define LOGE(...) do {} while (0)
#define LOGD(...) do {} while (0)
#endif

#else
#ifdef DEBUG
#define LOG_TAG "DEBUG"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else
#define LOGI(...) do {} while (0)
#define LOGW(...) do {} while (0)
#define LOGE(...) do {} while (0)
#define LOGD(...) do {} while (0)
#endif

#endif
#endif //JNILOG_JNI_LOG_UTIL_H
