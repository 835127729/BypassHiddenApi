package com.muye.hook.hiddenapi

import android.content.Context
import android.os.Build

/**
 * By directly modifying VMRuntime. midden.api_policy_ to EnforcementPolicy:: kNoChecks in memory,
 * bypassing hidden API checks
 */
object BypassByModifyEnforcementPolicy {

    fun bypass(context: Context): Boolean {
        if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.O_MR1) {
            return true
        }
        return bypassNative(
            Build.VERSION.SDK_INT,
            context.applicationInfo.targetSdkVersion
        )
    }

    private external fun bypassNative(apiVersion: Int, targetSdkVersion: Int): Boolean
}