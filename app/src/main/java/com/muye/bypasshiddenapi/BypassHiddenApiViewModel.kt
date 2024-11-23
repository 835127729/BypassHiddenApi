package com.muye.bypasshiddenapi

import android.annotation.SuppressLint
import android.content.Context
import android.util.Log
import androidx.lifecycle.ViewModel
import com.muye.hook.hiddenapi.BypassHiddenApi

//import com.muye.hook.hiddenapi.BypassHiddenApi

class BypassHiddenApiViewModel: ViewModel() {

    @SuppressLint("SoonBlockedPrivateApi", "PrivateApi")
    fun callHiddenApi()  = kotlin.runCatching {
        val clazz = Class.forName("android.webkit.WebViewLibraryLoader");
        val loadNativeLibraryMethod = clazz.getDeclaredMethod(
            "loadNativeLibrary",
            ClassLoader::class.java,
            String::class.java
        )
        val result = loadNativeLibraryMethod.invoke(null, null, "webviewchromium") as Int
        Log.d(TAG, "loadNativeLibrary result: $result")
        true
    }.getOrDefault(false)

    fun bypassByModifyAccessFlag() = BypassHiddenApi.bypassByModifyAccessFlag()
    fun bypassByGetMemberActionImpl() = BypassHiddenApi.bypassByGetMemberActionImpl()
    fun bypassByModifyEnforcementPolicy(context: Context) = BypassHiddenApi.bypassByModifyEnforcementPolicy(context)
    fun bypassByNullClassLoader(context:Context) = BypassHiddenApi.bypassByNullClassLoader(context)
    fun bypassByMetaReflect() = BypassHiddenApi.bypassByMetaReflect()
    fun bypassByJniOnLoad() = BypassHiddenApi.bypassByJniOnLoad()
    fun bypassByModifyHiddenApiExemptions() = BypassHiddenApi.bypassByModifyHiddenApiExemptions()
    fun bypassByUnsafeReflect() = BypassHiddenApi.bypassByUnsafeReflect()


    companion object {
        private const val TAG = "BypassHiddenApiViewMode"
    }
}