#include <jni.h>
#include "Log.h"
#include <string>
#include <set>
#include <mutex>

#define __ANDROID_API_U__ 35

class JavaVMExt {
public:
    void *functions;
    void *runtime;
};

// Refer: https://android.googlesource.com/platform/art/+/master/runtime/hidden_api.h
// Hidden API enforcement policy
// This must be kept in sync with ApplicationInfo.ApiEnforcementPolicy in
// frameworks/base/core/java/android/content/pm/ApplicationInfo.java
enum class EnforcementPolicy {
    kNoChecks = 0,
    kJustWarn = 1,  // keep checks enabled, but allow everything (enables logging)
    kDarkGreyAndBlackList = 2,  // ban dark grey & blacklist
    kBlacklistOnly = 3,  // ban blacklist violations only
    kMax = 3,
};

// Refer: https://android.googlesource.com/platform/art/+/master/runtime/experimental_flags.h
// Possible experimental features that might be enabled.
enum class ExperimentalFlags : uint32_t {
    kNone           = 0x0000,
    kMethodHandles  = 0x0004,  // 0b00000100
};

struct PartialRuntimeP {
    // Specifies target SDK version to allow workarounds for certain API levels.
    int32_t target_sdk_version_;

    // Implicit checks flags.
    bool implicit_null_checks_;       // NullPointer checks are implicit.
    bool implicit_so_checks_;         // StackOverflow checks are implicit.
    bool implicit_suspend_checks_;    // Thread suspension checks are implicit.

    // Whether or not the sig chain (and implicitly the fault handler) should be
    // disabled. Tools like dex2oat or patchoat don't need them. This enables
    // building a statically link version of dex2oat.
    bool no_sig_chain_;

    // Force the use of native bridge even if the app ISA matches the runtime ISA.
    bool force_native_bridge_;

    // Whether or not a native bridge has been loaded.
    //
    // The native bridge allows running native code compiled for a foreign ISA. The way it works is,
    // if standard dlopen fails to load native library associated with native activity, it calls to
    // the native bridge to load it and then gets the trampoline for the entry to native activity.
    //
    // The option 'native_bridge_library_filename' specifies the name of the native bridge.
    // When non-empty the native bridge will be loaded from the given file. An empty value means
    // that there's no native bridge.
    bool is_native_bridge_loaded_;

    // Whether we are running under native debugger.
    bool is_native_debuggable_;

    // whether or not any async exceptions have ever been thrown. This is used to speed up the
    // MterpShouldSwitchInterpreters function.
    bool async_exceptions_thrown_;

    // Whether Java code needs to be debuggable.
    bool is_java_debuggable_;

    // The maximum number of failed boots we allow before pruning the dalvik cache
    // and trying again. This option is only inspected when we're running as a
    // zygote.
    uint32_t zygote_max_failed_boots_;

    // Enable experimental opcodes that aren't fully specified yet. The intent is to
    // eventually publish them as public-usable opcodes, but they aren't ready yet.
    //
    // Experimental opcodes should not be used by other production code.
    ExperimentalFlags experimental_flags_;

    // Contains the build fingerprint, if given as a parameter.
    std::string fingerprint_;

    // Oat file manager, keeps track of what oat files are mapsVisitorOpen.
    // OatFileManager* oat_file_manager_;
    void *oat_file_manager_;

    // Whether or not we are on a low RAM device.
    bool is_low_memory_mode_;

    // Whether or not we use MADV_RANDOM on files that are thought to have random access patterns.
    // This is beneficial for low RAM devices since it reduces page cache thrashing.
    bool madvise_random_access_;

    // Whether the application should run in safe mode, that is, interpreter only.
    bool safe_mode_;

    // Whether access checks on hidden API should be performed.
    EnforcementPolicy hidden_api_policy_;
};

// Android R: https://android.googlesource.com/platform/art/+/refs/tags/android-11.0.0_r3/runtime/runtime.h#1182
struct PartialRuntimeR {
    // Specifies target SDK version to allow workarounds for certain API levels.
    uint32_t target_sdk_version_;

    // A set of disabled compat changes for the running app, all other changes are enabled.
    // std::set<uint64_t> disabled_compat_changes_;
    void *disabled_compat_changes_[3];

    // Implicit checks flags.
    bool implicit_null_checks_;       // NullPointer checks are implicit.
    bool implicit_so_checks_;         // StackOverflow checks are implicit.
    bool implicit_suspend_checks_;    // Thread suspension checks are implicit.

    // Whether or not the sig chain (and implicitly the fault handler) should be
    // disabled. Tools like dex2oat don't need them. This enables
    // building a statically link version of dex2oat.
    bool no_sig_chain_;

    // Force the use of native bridge even if the app ISA matches the runtime ISA.
    bool force_native_bridge_;

    // Whether or not a native bridge has been loaded.
    //
    // The native bridge allows running native code compiled for a foreign ISA. The way it works is,
    // if standard dlopen fails to load native library associated with native activity, it calls to
    // the native bridge to load it and then gets the trampoline for the entry to native activity.
    //
    // The option 'native_bridge_library_filename' specifies the name of the native bridge.
    // When non-empty the native bridge will be loaded from the given file. An empty value means
    // that there's no native bridge.
    bool is_native_bridge_loaded_;

    // Whether we are running under native debugger.
    bool is_native_debuggable_;

    // whether or not any async exceptions have ever been thrown. This is used to speed up the
    // MterpShouldSwitchInterpreters function.
    bool async_exceptions_thrown_;

    // Whether anything is going to be using the shadow-frame APIs to force a function to return
    // early. Doing this requires that (1) we be debuggable and (2) that mterp is exited.
    bool non_standard_exits_enabled_;

    // Whether Java code needs to be debuggable.
    bool is_java_debuggable_;

    bool is_profileable_from_shell_ = false;

    // The maximum number of failed boots we allow before pruning the dalvik cache
    // and trying again. This option is only inspected when we're running as a
    // zygote.
    uint32_t zygote_max_failed_boots_;

    // Enable experimental opcodes that aren't fully specified yet. The intent is to
    // eventually publish them as public-usable opcodes, but they aren't ready yet.
    //
    // Experimental opcodes should not be used by other production code.
    ExperimentalFlags experimental_flags_;

    // Contains the build fingerprint, if given as a parameter.
    std::string fingerprint_;

    // Oat file manager, keeps track of what oat files are mapsVisitorOpen.
    // OatFileManager* oat_file_manager_;
    void *oat_file_manager_;

    // Whether or not we are on a low RAM device.
    bool is_low_memory_mode_;

    // Whether or not we use MADV_RANDOM on files that are thought to have random access patterns.
    // This is beneficial for low RAM devices since it reduces page cache thrashing.
    bool madvise_random_access_;

    // Whether the application should run in safe mode, that is, interpreter only.
    bool safe_mode_;

    // Whether access checks on hidden API should be performed.
    EnforcementPolicy hidden_api_policy_;
};

// Record Log contention information, dumpable via SIGQUIT.
#if defined(__linux__)
// To enable lock contention logging, set this to true.
constexpr bool kLogLockContentions = false;
#else
// Keep this false as lock contention logging is supported only with
// futex.
constexpr bool kLogLockContentions = false;
#endif
constexpr size_t kContentionLogSize = 4;
constexpr size_t kContentionLogDataSize = kLogLockContentions ? 1 : 0;

#define EXPORT __attribute__((visibility("default")))

class BaseMutex {
public:
    const char* GetName() const {
        return name_;
    }

    virtual bool IsMutex() const { return false; }
    virtual bool IsReaderWriterMutex() const { return false; }
    virtual bool IsMutatorMutex() const { return false; }
protected:
    virtual ~BaseMutex();
    const char* const name_;
    // A log entry that records contention but makes no guarantee that either tid will be held live.
    struct ContentionLogEntry {
        ContentionLogEntry() : blocked_tid(0), owner_tid(0) {}
        uint64_t blocked_tid;
        uint64_t owner_tid;
        std::atomic<int32_t> count;
    };
    struct ContentionLogData {
        ContentionLogEntry contention_log[kContentionLogSize];
        // The next entry in the contention log to be updated. Value ranges from 0 to
        // kContentionLogSize - 1.
        std::atomic<int32_t> cur_content_log_entry;
        // Number of times the Mutex has been contended.
        std::atomic<int32_t> contention_count;
        // Sum of time waited by all contenders in ns.
        std::atomic<uint64_t> wait_time;
    };
    ContentionLogData contention_log_data_[kContentionLogDataSize];

    const uint8_t level_{0};  // Support for lock hierarchy.
    bool should_respond_to_empty_checkpoint_request_;
};

class EXPORT Mutex : public BaseMutex {
public:
    explicit Mutex(const char* name, uint8_t level, bool recursive = false);
    ~Mutex();
private:
#if defined(__linux__)
    // Low order bit: 0 is unheld, 1 is held.
    // High order bits: Number of waiting contenders.
    std::atomic<int32_t> state_and_contenders_;
    // Exclusive owner.
    std::atomic<pid_t> exclusive_owner_;
#else
    pthread_mutex_t mutex_;
    std::atomic<pid_t> exclusive_owner_;  // Guarded by mutex_. Asynchronous reads are OK.
#endif
    unsigned int recursion_count_;
    const bool recursive_{false};  // Can the lock be recursively held?
    bool enable_monitor_timeout_ = false;
    uint32_t monitor_id_;
};

struct CompatFramework {
    // A set of disabled compat changes for the running app, all other changes are enabled.
    std::set<uint64_t> disabled_compat_changes_;

    // A set of reported compat changes for the running app.
    std::set<uint64_t> reported_compat_changes_;
    Mutex reported_compat_changes_lock_;
};

struct PartialRuntimeS {
// Specifies target SDK version to allow workarounds for certain API levels.
    uint32_t target_sdk_version_;

    // ART counterpart for the compat framework (go/compat-framework).
    CompatFramework compat_framework_;

    // Implicit checks flags.
    bool implicit_null_checks_;       // NullPointer checks are implicit.
    bool implicit_so_checks_;         // StackOverflow checks are implicit.
    bool implicit_suspend_checks_;    // Thread suspension checks are implicit.

    // Whether or not the sig chain (and implicitly the fault handler) should be
    // disabled. Tools like dex2oat don't need them. This enables
    // building a statically link version of dex2oat.
    bool no_sig_chain_;

    // Force the use of native bridge even if the app ISA matches the runtime ISA.
    bool force_native_bridge_;

    // Whether or not a native bridge has been loaded.
    //
    // The native bridge allows running native code compiled for a foreign ISA. The way it works is,
    // if standard dlopen fails to load native library associated with native activity, it calls to
    // the native bridge to load it and then gets the trampoline for the entry to native activity.
    //
    // The option 'native_bridge_library_filename' specifies the name of the native bridge.
    // When non-empty the native bridge will be loaded from the given file. An empty value means
    // that there's no native bridge.
    bool is_native_bridge_loaded_;

    // Whether we are running under native debugger.
    bool is_native_debuggable_;

    // whether or not any async exceptions have ever been thrown. This is used to speed up the
    // MterpShouldSwitchInterpreters function.
    bool async_exceptions_thrown_;

    // Whether anything is going to be using the shadow-frame APIs to force a function to return
    // early. Doing this requires that (1) we be debuggable and (2) that mterp is exited.
    bool non_standard_exits_enabled_;

    // Whether Java code needs to be debuggable.
    bool is_java_debuggable_;

    bool monitor_timeout_enable_;
    uint64_t monitor_timeout_ns_;

    // Whether or not this application can be profiled by the shell user,
    // even when running on a device that is running in user mode.
    bool is_profileable_from_shell_ = false;

    // Whether or not this application can be profiled by system services on a
    // device running in user mode, but not necessarily by the shell user.
    bool is_profileable_ = false;

    // The maximum number of failed boots we allow before pruning the dalvik cache
    // and trying again. This option is only inspected when we're running as a
    // zygote.
    uint32_t zygote_max_failed_boots_;

    // Enable experimental opcodes that aren't fully specified yet. The intent is to
    // eventually publish them as public-usable opcodes, but they aren't ready yet.
    //
    // Experimental opcodes should not be used by other production code.
    ExperimentalFlags experimental_flags_;

    // Contains the build fingerprint, if given as a parameter.
    std::string fingerprint_;

    // Oat file manager, keeps track of what oat files are mapsVisitorOpen.
    void *oat_file_manager_;

    // Whether or not we are on a low RAM device.
    bool is_low_memory_mode_;

    // Whether or not we use MADV_RANDOM on files that are thought to have random access patterns.
    // This is beneficial for low RAM devices since it reduces page cache thrashing.
    bool madvise_random_access_;

    // Limiting size (in bytes) for applying MADV_WILLNEED on vdex files
    // A 0 for this will turn off madvising to MADV_WILLNEED
    size_t madvise_willneed_vdex_filesize_;

    // Limiting size (in bytes) for applying MADV_WILLNEED on odex files
    // A 0 for this will turn off madvising to MADV_WILLNEED
    size_t madvise_willneed_odex_filesize_;

    // Limiting size (in bytes) for applying MADV_WILLNEED on art files
    // A 0 for this will turn off madvising to MADV_WILLNEED
    size_t madvise_willneed_art_filesize_;

    // Whether the application should run in safe mode, that is, interpreter only.
    bool safe_mode_;

    // Whether access checks on hidden API should be performed.
    EnforcementPolicy hidden_api_policy_;
};

enum class RuntimeDebugState {
    // This doesn't support any debug features / method tracing. This is the expected state usually.
    kNonJavaDebuggable,
    // This supports method tracing and a restricted set of debug features (for ex: redefinition
    // isn't supported). We transition to this state when method tracing has started or when the
    // debugger was attached and transition back to NonDebuggable once the tracing has stopped /
    // the debugger agent has detached..
    kJavaDebuggable,
    // The runtime was started as a debuggable runtime. This allows us to support the extended set
    // of debug features (for ex: redefinition). We never transition out of this state.
    kJavaDebuggableAtInit
};

struct PartialRuntimeU {
    // Specifies target SDK version to allow workarounds for certain API levels.
    uint32_t target_sdk_version_;

    // ART counterpart for the compat framework (go/compat-framework).
    CompatFramework compat_framework_;

    // Implicit checks flags.
    bool implicit_null_checks_;       // NullPointer checks are implicit.
    bool implicit_so_checks_;         // StackOverflow checks are implicit.
    bool implicit_suspend_checks_;    // Thread suspension checks are implicit.

    // Whether or not the sig chain (and implicitly the fault handler) should be
    // disabled. Tools like dex2oat don't need them. This enables
    // building a statically link version of dex2oat.
    bool no_sig_chain_;

    // Force the use of native bridge even if the app ISA matches the runtime ISA.
    bool force_native_bridge_;

    // Whether or not a native bridge has been loaded.
    //
    // The native bridge allows running native code compiled for a foreign ISA. The way it works is,
    // if standard dlopen fails to load native library associated with native activity, it calls to
    // the native bridge to load it and then gets the trampoline for the entry to native activity.
    //
    // The option 'native_bridge_library_filename' specifies the name of the native bridge.
    // When non-empty the native bridge will be loaded from the given file. An empty value means
    // that there's no native bridge.
    bool is_native_bridge_loaded_;

    // Whether we are running under native debugger.
    bool is_native_debuggable_;

    // whether or not any async exceptions have ever been thrown. This is used to speed up the
    // MterpShouldSwitchInterpreters function.
    bool async_exceptions_thrown_;

    // Whether anything is going to be using the shadow-frame APIs to force a function to return
    // early. Doing this requires that (1) we be debuggable and (2) that mterp is exited.
    bool non_standard_exits_enabled_;

    // Whether Java code needs to be debuggable.
    RuntimeDebugState runtime_debug_state_;

    bool monitor_timeout_enable_;
    uint64_t monitor_timeout_ns_;

    // Whether or not this application can be profiled by the shell user,
    // even when running on a device that is running in user mode.
    bool is_profileable_from_shell_ = false;

    // Whether or not this application can be profiled by system services on a
    // device running in user mode, but not necessarily by the shell user.
    bool is_profileable_ = false;

    // The maximum number of failed boots we allow before pruning the dalvik cache
    // and trying again. This option is only inspected when we're running as a
    // zygote.
    uint32_t zygote_max_failed_boots_;

    // Enable experimental opcodes that aren't fully specified yet. The intent is to
    // eventually publish them as public-usable opcodes, but they aren't ready yet.
    //
    // Experimental opcodes should not be used by other production code.
    ExperimentalFlags experimental_flags_;

    // Contains the build fingerprint, if given as a parameter.
    std::string fingerprint_;

    // Oat file manager, keeps track of what oat files are mapsVisitorOpen.
    void *oat_file_manager_;

    // Whether or not we are on a low RAM device.
    bool is_low_memory_mode_;

    // Limiting size (in bytes) for applying MADV_WILLNEED on vdex files
    // or uncompressed dex files in APKs.
    // A 0 for this will turn off madvising to MADV_WILLNEED
    size_t madvise_willneed_total_dex_size_;

    // Limiting size (in bytes) for applying MADV_WILLNEED on odex files
    // A 0 for this will turn off madvising to MADV_WILLNEED
    size_t madvise_willneed_odex_filesize_;

    // Limiting size (in bytes) for applying MADV_WILLNEED on art files
    // A 0 for this will turn off madvising to MADV_WILLNEED
    size_t madvise_willneed_art_filesize_;

    // Whether the application should run in safe mode, that is, interpreter only.
    bool safe_mode_;

    // Whether access checks on hidden API should be performed.
    EnforcementPolicy hidden_api_policy_;
};

template<typename Runtime>
static void SetHiddenApiEnforcementPolicy(Runtime *partialRuntime) {
    partialRuntime->hidden_api_policy_ = EnforcementPolicy::kNoChecks;
}

template<typename T>
static int findTargetOffset(uintptr_t base, int startOffset, int endOffset, T target) {
    for (int i = startOffset; i < endOffset; i += sizeof(void *)) {
        T *p = reinterpret_cast<T *>(base + i);
        if (*p == target) {
            return i;
        }
    }
    return -1;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_muye_hook_hiddenapi_BypassByModifyEnforcementPolicy_bypassNative(JNIEnv *env,
                                                                          jobject thiz,
                                                                          jint api_version,
                                                                          jint target_sdk_version) {
    JavaVM *vm;
    env->GetJavaVM(&vm);
    JavaVMExt *javaVMExt = (JavaVMExt *) vm;
    void *runtime = javaVMExt->runtime;


    /**
     * [art/runtime/runtime.h](http://androidxref.com/9.0.0_r3/xref/art/runtime/runtime.h)
     * class Runtime{
     *
     *  893  std::unique_ptr<JavaVMExt> java_vm_;
     *  962  int32_t target_sdk_version_;
     *  996  bool is_java_debuggable_;
     *  1026  hiddenapi::EnforcementPolicy hidden_api_policy_;
     *  1023  bool safe_mode_;
     * }
     * According to the memory layout of Class Runtime, find the pointer to the hidden'api_policy_ variable
     */
    int maxOffset = 500 * sizeof(void *);
    //1、First, locate the Java_VM file_
    int offsetOfVmExt = findTargetOffset(reinterpret_cast<uintptr_t>(runtime), 0, maxOffset, vm);
    if (offsetOfVmExt < 0) {
        return false;
    }

    //2、Find targetSdkVersion
    int targetSdkVersionOffset = findTargetOffset(reinterpret_cast<uintptr_t>(runtime),
                                                  offsetOfVmExt,
                                                  maxOffset,
                                                  target_sdk_version);
    if (targetSdkVersionOffset < 0) {
        return false;
    }

    if (api_version >= __ANDROID_API_U__) {
        auto *partialRuntime = reinterpret_cast<PartialRuntimeU *>(
                reinterpret_cast<uintptr_t>(runtime) + targetSdkVersionOffset);
        SetHiddenApiEnforcementPolicy(partialRuntime);
    } else if (api_version >= __ANDROID_API_S__) {
        auto *partialRuntime = reinterpret_cast<PartialRuntimeS *>(
                reinterpret_cast<uintptr_t>(runtime) + targetSdkVersionOffset);
        SetHiddenApiEnforcementPolicy(partialRuntime);

    } else if (api_version >= __ANDROID_API_R__) {
        auto *partialRuntime = reinterpret_cast<PartialRuntimeR *>(
                reinterpret_cast<uintptr_t>(runtime) + targetSdkVersionOffset);
        SetHiddenApiEnforcementPolicy(partialRuntime);
    } else if (api_version >= __ANDROID_API_P__){
        auto *partialRuntime = reinterpret_cast<PartialRuntimeP *>(
                reinterpret_cast<uintptr_t>(runtime) + targetSdkVersionOffset);
        SetHiddenApiEnforcementPolicy(partialRuntime);
    }
    return true;
}