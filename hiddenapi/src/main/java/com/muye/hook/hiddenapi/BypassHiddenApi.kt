package com.muye.hook.hiddenapi

import android.content.Context
import com.bytedance.shadowhook.ShadowHook
import com.bytedance.shadowhook.ShadowHook.ConfigBuilder

object BypassHiddenApi {
    init {
        ShadowHook.init(ConfigBuilder().run {
            setMode(ShadowHook.Mode.SHARED)
            build()
        })
        System.loadLibrary("hiddenapi")
    }

    fun bypassByModifyAccessFlag() = BypassByModifyAccessFlag.bypass()
    fun bypassByGetMemberActionImpl() = BypassByGetMemberActionImpl.bypass()
    fun bypassByModifyEnforcementPolicy(context: Context) = BypassByModifyEnforcementPolicy.bypass(context)
    fun bypassByNullClassLoader(context: Context) = BypassHiddenApiByNullClassloader.bypass(context)
    fun bypassByMetaReflect() = BypassByMetaReflection.bypass()
    fun bypassByJniOnLoad() = BypassByJniOnLoad.bypass()
    fun bypassByModifyHiddenApiExemptions() = BypassBySetHiddenApiExemptions.bypass()
    fun bypassByUnsafeReflect() = BypassByUnsafeReflect.bypass()
}