package com.muye.hook.hiddenapi

import android.os.Build
import java.lang.reflect.Method

private object MetaReflectApi28{
    private val getDeclaredMethodMethod = lazy {
        runCatching {
            Class::class.java.getDeclaredMethod(
                "getDeclaredMethod",
                String::class.java,
                arrayOf<Class<*>>()::class.java
            )
        }.getOrNull()
    }

    private fun getMethod(
        targetClass: Class<*>,
        methodName: String,
        parameterTypes: Array<Class<*>>? = null,
    ): Method? = kotlin.runCatching {
        getDeclaredMethodMethod.value?.invoke(
            targetClass,
            methodName,
            parameterTypes
        ) as Method?
    }.getOrNull()

    fun bypass(): Boolean = kotlin.runCatching {
        val vmRuntimeClass = Class.forName("dalvik.system.VMRuntime")
        //1、get vmRuntime instance
        val getRuntimeMethod =  getMethod(vmRuntimeClass, "getRuntime")
            ?: return@runCatching false
        val vmRuntime = getRuntimeMethod.invoke(null) ?: return@runCatching false
        //2、call VMRuntime.setHiddenApiExemptions("L")
        val setHiddenApiExemptionsMethod = getMethod(vmRuntimeClass, "setHiddenApiExemptions",
            arrayOf(arrayOf<String>()::class.java)) ?: return@runCatching false
        setHiddenApiExemptionsMethod.invoke(vmRuntime, arrayOf("L"))
        return true
    }.getOrDefault(false)
}

private object MetaReflectApi30{
    @JvmStatic
    external fun bypassNative(): Boolean
}

internal object BypassByMetaReflection {

    fun bypass(): Boolean = kotlin.runCatching {
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.R){
            MetaReflectApi30.bypassNative()
        }else if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.P){
            MetaReflectApi28.bypass()
        }else {
            true
        }
    }.getOrDefault(false)
}