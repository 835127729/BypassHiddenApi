#include <jni.h>
#include <future>
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
        LOGV("HiddenApiHook", "setHiddenApiExemptions failed, getRuntimeMethodId is null");
        env->ExceptionClear();
        return false;
    }
    jobject vmRuntime = env->CallStaticObjectMethod(vmRuntimeClass, getRuntimeMethodId);
    if (vmRuntime == nullptr) {
        LOGV("HiddenApiHook", "setHiddenApiExemptions failed, vmRuntime is null");
        env->ExceptionClear();
        return false;
    }

    jmethodID setHiddenApiExemptionsMethodId = env->GetMethodID(vmRuntimeClass,
                                                                "setHiddenApiExemptions",
                                                                "([Ljava/lang/String;)V");
    if (setHiddenApiExemptionsMethodId == nullptr) {
        LOGV("HiddenApiHook",
             "setHiddenApiExemptions failed, setHiddenApiExemptionsMethodId is null");
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
    LOGV("HiddenApiHook", "setHiddenApiExemptions success");

    env->DeleteLocalRef(lStr);
    env->DeleteLocalRef(params);
    env->DeleteLocalRef(vmRuntime);
    return true;
}

/**
 * 方法5:
 * Android R(11)及以上版本，通过刚attach的native线程，绕过拦截逻辑，调用隐藏API
 */
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_muye_hook_hiddenapi_MetaReflectApi30_bypassNative(JNIEnv *env, jclass clazz) {
    JavaVM *vm;
    env->GetJavaVM(&vm);
    //Start the native thread and pass vm as a parameter
    auto f = std::async(std::launch::async, [&]() ->bool {
        JNIEnv *env = nullptr;
        //1、Attach, so that jni can be called, and the caller at the beginning of the call is null
        vm->AttachCurrentThread(&env, nullptr);
        //2、By using jni and reflecting to call the setHiddenApiExertions method of VMRuntime,
        // all APIs are added to the blacklist
        bool flag = setHiddenApiExemptions(env);
        vm->DetachCurrentThread();
        return flag;
    });
    return f.get();
}
