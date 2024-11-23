#include <jni.h>
#include <shadowhook.h>
#include "Log.h"
#include <android/api-level.h>
#include <functional>
#include <string>

//art::hiddenapi::Action art::hiddenapi::detail::GetMemberActionImpl<art::ArtMethod>(art::ArtMethod*, art::HiddenApiAccessFlags::ApiList, art::hiddenapi::Action, art::hiddenapi::AccessMethod)
#define Str_GetMemberActionImplApi28 "_ZN3art9hiddenapi6detail19GetMemberActionImplINS_9ArtMethodEEENS0_6ActionEPT_NS_20HiddenApiAccessFlags7ApiListES4_NS0_12AccessMethodE"
//bool art::hiddenapi::detail::ShouldDenyAccessToMemberImpl<art::ArtMethod>(art::ArtMethod*, art::hiddenapi::ApiList, art::hiddenapi::AccessMethod)
#define Str_ShouldDenyAccessToMember29 "_ZN3art9hiddenapi6detail28ShouldDenyAccessToMemberImplINS_9ArtMethodEEEbPT_NS0_7ApiListENS0_12AccessMethodE"

enum Action {
    kAllow,
    kAllowButWarn,
    kAllowButWarnAndToast,
    kDeny
};

enum ApiList {
    kWhitelist = 0,
    kLightGreylist,
    kDarkGreylist,
    kBlacklist,
};

enum AccessMethod {
    kNone,  // internal test that does not correspond to an actual access by app
    kReflection,
    kJNI,
    kLinking,
};

class AccessContext {
};

static Action proxyGetMemberActionImplApi28(void *, ApiList , Action , AccessMethod ) {
    SHADOWHOOK_STACK_SCOPE();
    return kAllow;
}

static bool proxyShouldDenyAccessToMemberImpApi29(void *, ApiList, AccessMethod) {
    SHADOWHOOK_STACK_SCOPE();
    return false;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_muye_hook_hiddenapi_BypassByGetMemberActionImpl_bypassNative(JNIEnv *env,
                                                                      jclass clazz) {
    int apiVersion = android_get_device_api_level();
    void *stub = nullptr;
    if(apiVersion <= __ANDROID_API_O_MR1__){
        return true;
    }else if (apiVersion == __ANDROID_API_P__) {
        stub = shadowhook_hook_sym_name("libart.so", Str_GetMemberActionImplApi28,
                                        (void *) proxyGetMemberActionImplApi28, nullptr);
    } else {
        stub = shadowhook_hook_sym_name("libart.so", Str_ShouldDenyAccessToMember29,
                                        (void *) proxyShouldDenyAccessToMemberImpApi29, nullptr);
    }

    return stub != nullptr;
}