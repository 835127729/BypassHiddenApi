package com.muye.hook.hiddenapi

import android.os.Build
import sun.misc.Unsafe
import java.lang.reflect.Executable

/**
 * Modify ArtMethod.access flags through inline Hook to bypass interception
 */
object BypassByModifyAccessFlag {

    private val artMethodOffset = lazy {
        runCatching {
            val unsafe = Unsafe::class.java.getDeclaredMethod("getUnsafe").invoke(null) as Unsafe
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q && Build.VERSION.SDK_INT <= Build.VERSION_CODES.S) {
                unsafe.objectFieldOffset(Executable::class.java.getDeclaredField("artMethod"))
            } else {
                -1
            }
        }.getOrDefault(-1)
    }

    fun bypass(): Boolean {
        if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.O_MR1) {
            return true
        }
        return bypassNative(artMethodOffset.value)
    }

    @JvmStatic
    private external fun bypassNative(artMethodOffset: Long):Boolean
}