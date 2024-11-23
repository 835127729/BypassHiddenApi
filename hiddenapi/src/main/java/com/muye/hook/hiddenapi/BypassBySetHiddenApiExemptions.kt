package com.muye.hook.hiddenapi

/**
 * Directly find the VMRuntime_SetHiddenApiExertionExemptions() through dlsym and call it
 */
object BypassBySetHiddenApiExemptions {

    fun bypass(): Boolean = bypassNative()

    @JvmStatic
    private external fun bypassNative(): Boolean
}