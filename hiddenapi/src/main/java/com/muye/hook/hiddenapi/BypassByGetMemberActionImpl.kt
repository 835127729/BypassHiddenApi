package com.muye.hook.hiddenapi

/**
 * By using the Inline Hook core method, it always returns non interception
 */
object BypassByGetMemberActionImpl {
    fun bypass(): Boolean = bypassNative()

    @JvmStatic
    private external fun bypassNative(): Boolean
}