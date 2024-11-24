#include <jni.h>
#include "Log.h"

static bool setHiddenApiExemptions(JNIEnv *env) {
    jclass vmRuntimeClass = env->FindClass("dalvik/system/VMRuntime");
    if (vmRuntimeClass == nullptr) {
        LOGV("HiddenApiHook",
             "setHiddenApiExemptions failed, can't FindClass dalvik/system/VMRuntime");
        env->ExceptionClear();
        return false;
    }

    jmethodID getRuntimeMethodId = env->GetStaticMethodID(vmRuntimeClass, "getRuntime",
                                                          "()Ldalvik/system/VMRuntime;");
    if (getRuntimeMethodId == nullptr) {
        LOGV("HiddenApiHook", "setApiBlacklistExemptions failed, getRuntimeMethodId is null");
        env->ExceptionClear();
        return false;
    }
    jobject vmRuntime = env->CallStaticObjectMethod(vmRuntimeClass, getRuntimeMethodId);
    if (vmRuntime == nullptr) {
        LOGV("HiddenApiHook", "setApiBlacklistExemptions failed, vmRuntime is null");
        env->ExceptionClear();
        return false;
    }

    jmethodID setHiddenApiExemptionsMethodId = env->GetMethodID(vmRuntimeClass,
                                                                "setHiddenApiExemptions",
                                                                "([Ljava/lang/String;)V");
    if (setHiddenApiExemptionsMethodId == nullptr) {
        LOGV("HiddenApiHook",
             "setApiBlacklistExemptions failed, setHiddenApiExemptionsMethodId is null");
        env->ExceptionClear();
        return false;
    }

    jclass stringClass = env->FindClass("java/lang/String");
    jstring lStr = env->NewStringUTF("L");
    jobjectArray params = env->NewObjectArray(1, stringClass, nullptr);
    env->SetObjectArrayElement(params, 0, lStr);

    env->CallVoidMethod(vmRuntime, setHiddenApiExemptionsMethodId, params);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
    }
    LOGV("HiddenApiHook", "setApiBlacklistExemptions success");

    env->DeleteLocalRef(lStr);
    env->DeleteLocalRef(params);
    env->DeleteLocalRef(vmRuntime);
    return JNI_TRUE;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    setHiddenApiExemptions(env);
    return JNI_VERSION_1_6;
}