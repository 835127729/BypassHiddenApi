#include <jni.h>
#include "Log.h"
#include <shadowhook.h>
#include <functional>

#if defined(__LP64__)
// art::ObjPtr<art::mirror::Method> art::mirror::Class::GetDeclaredMethodInternal<(art::PointerSize)8, false>(art::Thread*, art::ObjPtr<art::mirror::Class>, art::ObjPtr<art::mirror::String>, art::ObjPtr<art::mirror::ObjectArray<art::mirror::Class> >)
#define Str_GetDeclaredMethodInternalApi28 "_ZN3art6mirror5Class25GetDeclaredMethodInternalILNS_11PointerSizeE8ELb0EEENS_6ObjPtrINS0_6MethodEEEPNS_6ThreadENS4_IS1_EENS4_INS0_6StringEEENS4_INS0_11ObjectArrayIS1_EEEE"
// art::ObjPtr<art::mirror::Method> art::mirror::Class::GetDeclaredMethodInternal<(art::PointerSize)8, false>(art::Thread*, art::ObjPtr<art::mirror::Class>, art::ObjPtr<art::mirror::String>, art::ObjPtr<art::mirror::ObjectArray<art::mirror::Class> >)
#define Str_GetDeclaredMethodInternalApi29 "_ZN3art6mirror5Class25GetDeclaredMethodInternalILNS_11PointerSizeE8ELb0EEENS_6ObjPtrINS0_6MethodEEEPNS_6ThreadENS4_IS1_EENS4_INS0_6StringEEENS4_INS0_11ObjectArrayIS1_EEEE"
// art::ObjPtr<art::mirror::Method> art::mirror::Class::GetDeclaredMethodInternal<(art::PointerSize)8>(art::Thread*, art::ObjPtr<art::mirror::Class>, art::ObjPtr<art::mirror::String>, art::ObjPtr<art::mirror::ObjectArray<art::mirror::Class> >, std::__1::function<art::hiddenapi::AccessContext ()> const&)
#define Str_GetDeclaredMethodInternalApi31 "_ZN3art6mirror5Class25GetDeclaredMethodInternalILNS_11PointerSizeE8EEENS_6ObjPtrINS0_6MethodEEEPNS_6ThreadENS4_IS1_EENS4_INS0_6StringEEENS4_INS0_11ObjectArrayIS1_EEEERKNSt3__18functionIFNS_9hiddenapi13AccessContextEvEEE"
#else
// art::ObjPtr<art::mirror::Method> art::mirror::Class::GetDeclaredMethodInternal<(art::PointerSize)4, false>(art::Thread*, art::ObjPtr<art::mirror::Class>, art::ObjPtr<art::mirror::String>, art::ObjPtr<art::mirror::ObjectArray<art::mirror::Class> >)
#define Str_GetDeclaredMethodInternalApi28 "_ZN3art6mirror5Class25GetDeclaredMethodInternalILNS_11PointerSizeE4ELb0EEENS_6ObjPtrINS0_6MethodEEEPNS_6ThreadENS4_IS1_EENS4_INS0_6StringEEENS4_INS0_11ObjectArrayIS1_EEEE"
// art::ObjPtr<art::mirror::Method> art::mirror::Class::GetDeclaredMethodInternal<(art::PointerSize)4, false>(art::Thread*, art::ObjPtr<art::mirror::Class>, art::ObjPtr<art::mirror::String>, art::ObjPtr<art::mirror::ObjectArray<art::mirror::Class> >)
#define Str_GetDeclaredMethodInternalApi29 "_ZN3art6mirror5Class25GetDeclaredMethodInternalILNS_11PointerSizeE4ELb0EEENS_6ObjPtrINS0_6MethodEEEPNS_6ThreadENS4_IS1_EENS4_INS0_6StringEEENS4_INS0_11ObjectArrayIS1_EEEE"
// art::ObjPtr<art::mirror::Method> art::mirror::Class::GetDeclaredMethodInternal<(art::PointerSize)4>(art::Thread*, art::ObjPtr<art::mirror::Class>, art::ObjPtr<art::mirror::String>, art::ObjPtr<art::mirror::ObjectArray<art::mirror::Class> >, std::__1::function<art::hiddenapi::AccessContext ()> const&)
#define Str_GetDeclaredMethodInternalApi31 "_ZN3art6mirror5Class25GetDeclaredMethodInternalILNS_11PointerSizeE4EEENS_6ObjPtrINS0_6MethodEEEPNS_6ThreadENS4_IS1_EENS4_INS0_6StringEEENS4_INS0_11ObjectArrayIS1_EEEERKNSt3__18functionIFNS_9hiddenapi13AccessContextEvEEE"
#endif
// bool art::hiddenapi::ShouldDenyAccessToMember<art::ArtMethod>(art::ArtMethod*, std::__1::function<art::hiddenapi::AccessContext ()> const&, art::hiddenapi::AccessMethod)
#define Str_ShouldDenyAccessToMemberApi32 "_ZN3art9hiddenapi24ShouldDenyAccessToMemberINS_9ArtMethodEEEbPT_RKNSt3__18functionIFNS0_13AccessContextEvEEENS0_12AccessMethodE"
//art::mirror::Executable::GetArtMethod()
#define Str_GetArtMethod "_ZN3art6mirror10Executable12GetArtMethodEv"

static int64_t gArtMethodOffset = 0;
struct ObjPtr {
    uintptr_t reference_;
};

class AccessContext {
};

enum AccessMethod {
    kNone,  // internal test that does not correspond to an actual access by app
    kReflection,
    kJNI,
    kLinking,
};

enum Action {
    kAllow,
    kAllowButWarn,
    kAllowButWarnAndToast,
    kDeny
};

typedef void *(*GetArtMethod)(void *exec);

static constexpr uint32_t kAccPublicApi = 0x10000000;

class ArtMethod{
public:
    uint32_t declaring_class_;
    std::atomic<uint32_t> access_flags_;
};

static void modifyAccessFlags(void* artMethodPtr){
    if(artMethodPtr == nullptr){
        return;
    }
    auto artMethod = (ArtMethod*)artMethodPtr;
    /**
     * class ArtMethod{
     * protected:
     *  GcRoot<mirror::Class> declaring_class_;
     *  std::atomic<std::uint32_t> access_flags_;
     *  ...
     * }
     */
     if(android_get_device_api_level() == __ANDROID_API_P__) {
         artMethod->access_flags_ &= 0x9FFFFFFF;
     }else{
         artMethod->access_flags_ |= kAccPublicApi;
     }
}

static void* CallGetArtMethod(void* method){
    void *libart = shadowhook_dlopen("libart.so");
    if (libart == nullptr) {
        return nullptr;
    }
    void *getArtMethodPtr = shadowhook_dlsym(libart, Str_GetArtMethod);
    shadowhook_dlclose(libart);
    if(getArtMethodPtr == nullptr){
        return nullptr;
    }
    return ((GetArtMethod)getArtMethodPtr)(method);
}

static ObjPtr proxyGetDeclaredMethodInternalApi28(void *self, ObjPtr klass, ObjPtr name, ObjPtr args) {
    SHADOWHOOK_STACK_SCOPE();
    ObjPtr res = SHADOWHOOK_CALL_PREV(proxyGetDeclaredMethodInternalApi28, self, klass, name, args);
    if (res.reference_ == 0) {
        return res;
    }
    auto *artMethod = CallGetArtMethod((void *) res.reference_);
    if(artMethod == nullptr){
        return res;
    }
    modifyAccessFlags(artMethod);
    return res;
}

static ObjPtr proxyGetDeclaredMethodInternalApi29(void *self, ObjPtr klass, ObjPtr name, ObjPtr args,
                                                  std::function<AccessContext> const &fn_get_access_context) {
    SHADOWHOOK_STACK_SCOPE();
    ObjPtr res = SHADOWHOOK_CALL_PREV(proxyGetDeclaredMethodInternalApi29, self, klass, name, args,
                                      fn_get_access_context);
    if (res.reference_ == 0) {
        return res;
    }
    auto *artMethodPtr = (uintptr_t *) ((uintptr_t) res.reference_ + gArtMethodOffset);
    auto artMethod = (void*)*artMethodPtr;
    modifyAccessFlags(artMethod);
    return res;
}

static bool proxyShouldDenyAccessToMemberApi32(void *artMethod, std::function<AccessContext> const & context, AccessMethod accessMethod) {
    SHADOWHOOK_STACK_SCOPE();
    modifyAccessFlags((ArtMethod*)artMethod);
    return SHADOWHOOK_CALL_PREV(proxyShouldDenyAccessToMemberApi32, artMethod, context, accessMethod);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_muye_hook_hiddenapi_BypassByModifyAccessFlag_bypassNative(JNIEnv *env,
                                                                   jclass _,
                                                                   jlong artMethodOffset) {
    void *stub = nullptr;
    int apiVersion = android_get_device_api_level();
    if(apiVersion <= __ANDROID_API_O_MR1__){
        return true;
    }else if (apiVersion == __ANDROID_API_P__) {
        stub = shadowhook_hook_sym_name("libart.so", Str_GetDeclaredMethodInternalApi28,
                                        (void *) proxyGetDeclaredMethodInternalApi28, nullptr);
    } else if (apiVersion <= __ANDROID_API_S__) {
        if(artMethodOffset == -1){
            return false;
        }
        gArtMethodOffset = artMethodOffset;
        if(apiVersion <= __ANDROID_API_R__){
            stub = shadowhook_hook_sym_name("libart.so", Str_GetDeclaredMethodInternalApi29,
                                            (void *) proxyGetDeclaredMethodInternalApi29, nullptr);
        }else{
            stub = shadowhook_hook_sym_name("libart.so", Str_GetDeclaredMethodInternalApi31,
                                            (void *) proxyGetDeclaredMethodInternalApi29, nullptr);
        }
    } else {
        stub = shadowhook_hook_sym_name("libart.so", Str_ShouldDenyAccessToMemberApi32,
                                        (void *) proxyShouldDenyAccessToMemberApi32, nullptr);
    }

    return stub != nullptr;
}