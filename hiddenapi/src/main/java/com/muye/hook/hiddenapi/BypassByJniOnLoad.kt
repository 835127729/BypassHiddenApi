package com.muye.hook.hiddenapi

object BypassByJniOnLoad {
    fun bypass(): Boolean = kotlin.runCatching {
        System.loadLibrary("bypass_by_jni_on_load")
        true
    }.getOrDefault(false)
}