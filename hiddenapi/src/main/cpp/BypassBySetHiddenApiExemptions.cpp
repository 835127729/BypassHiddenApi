#include <jni.h>
#include <shadowhook.h>
#include "Log.h"

//art::VMRuntime_setHiddenApiExemptions(_JNIEnv*, _jclass*, _jobjectArray*)
#define Str_VMRuntime_setHiddenApiExemptions "_ZN3artL32VMRuntime_setHiddenApiExemptionsEP7_JNIEnvP7_jclassP13_jobjectArray"

typedef void (*vmRuntimeSetHiddenApiExemptions)(JNIEnv *env, jclass, jobjectArray exemptions);

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_muye_hook_hiddenapi_BypassBySetHiddenApiExemptions_bypassNative(
        JNIEnv *env, jclass clazz) {
    void *libart = shadowhook_dlopen("libart.so");
    if (libart == nullptr) {
        return false;
    }
    void *vmRuntimeSetHiddenApiExemptionsPtr = shadowhook_dlsym(libart,Str_VMRuntime_setHiddenApiExemptions);
    if (vmRuntimeSetHiddenApiExemptionsPtr == nullptr) {
        shadowhook_dlclose(libart);
        return false;
    }
    auto vmRuntimeSetHiddenApiExemptionsFunc = (vmRuntimeSetHiddenApiExemptions) vmRuntimeSetHiddenApiExemptionsPtr;
    jobjectArray exemptions = env->NewObjectArray(1, env->FindClass("java/lang/String"), nullptr);
    jstring lStr = env->NewStringUTF("L");
    env->SetObjectArrayElement(exemptions, 0, lStr);
    vmRuntimeSetHiddenApiExemptionsFunc(env, clazz, exemptions);
    env->DeleteLocalRef(lStr);
    env->DeleteLocalRef(exemptions);
    shadowhook_dlclose(libart);
    return true;
}