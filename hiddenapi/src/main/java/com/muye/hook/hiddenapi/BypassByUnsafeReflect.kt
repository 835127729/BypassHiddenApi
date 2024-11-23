package com.muye.hook.hiddenapi

import android.util.Log
import com.muye.unsafe_reflect.UnsafeReflect

/**
 * Modify the memory through Unsafe to obtain the VMRuntime. setHedenApiEmptions pointer and call it
 */
object BypassByUnsafeReflect {

    fun bypass(): Boolean = kotlin.runCatching {
        val clazz = Class.forName("dalvik.system.VMRuntime")
        val vmRuntime = UnsafeReflect.invoke(clazz, null, "getRuntime") ?: return false
        UnsafeReflect.invoke(
            clazz,
            vmRuntime,
            "setHiddenApiExemptions",
            arrayOf(arrayOf<String>()::class.java),
            arrayOf("L")
        )
        true
    }.getOrDefault(false)
}