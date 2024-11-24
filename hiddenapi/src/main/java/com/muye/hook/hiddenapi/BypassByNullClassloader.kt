package com.muye.hook.hiddenapi

import android.content.Context
import android.os.Build
import android.util.Base64
import android.util.Log
import dalvik.system.DexFile
import java.io.BufferedOutputStream
import java.io.File
import java.io.FileOutputStream

/**
 * Change the classloader to null to bypass hidden API detection
 */
object BypassHiddenApiByNullClassloader {
    private const val TAG = "BypassByNullClassloader"

    fun bypass(context: Context): Boolean = runCatching {
        if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.O_MR1) {
            return@runCatching true
        }

        if(Build.VERSION.SDK_INT == Build.VERSION_CODES.P) {
            bypassApi28()
            true
        }else{
            bypassApi29(context)
        }
    }.getOrDefault(false)

    private fun bypassApi28(){
        val clazz = Class.forName("com.muye.hook.hiddenapi.SetAllHiddenApiExemptions")
        Class::class.java.getDeclaredField("classLoader").apply {
            isAccessible = true
            set(clazz, null)
        }
        clazz.getDeclaredMethod("invoke").invoke(null)
    }

    private const val DEX_IN_BASE64 = "ZGV4CjAzNQAZF8sMBEiNCPn9PDt+hiX9LlMTme/2rA7wBQAAcAAAAHhWNBIAAAAAAAAAAFAFAAAfAAAAcAAAAAsAAADsAAAABgAAABgBAAAAAAAAAAAAAAcAAABgAQAAAQAAAJgBAAA4BAAAuAEAAK4CAAC2AgAAuQIAAL0CAADCAgAA9wIAAAoDAAAgAwAANAMAAEgDAABfAwAAewMAAJsDAACeAwAAogMAAKUDAAC5AwAAzgMAAOMDAAD8AwAABQQAABgEAAAkBAAANgQAAD4EAABHBAAAVQQAAGQEAAB8BAAAmgQAAJ0EAAAEAAAABQAAAAcAAAAIAAAACQAAAAoAAAAMAAAADgAAAA8AAAAQAAAAEQAAAAIAAAABAAAAkAIAAAMAAAACAAAAmAIAAAMAAAAFAAAAoAIAAAwAAAAGAAAAAAAAAA0AAAAGAAAAqAIAAA4AAAAHAAAAAAAAAAAAAwAAAAAAAAAFABcAAAABAAAAEwAAAAEAAgAUAAAAAgADAAAAAAAFAAEAFwAAAAUABAAaAAAAAAAAAAEAAAACAAAAAAAAAAsAAAAAAAAAOwUAAAAAAAAJAAAAAwABAGACAAA5AAAAEgAaARIAcRACAAEADAEaAhUAIwMIAG4wAwAhAwwCEhNuIAYAMgAjBAkAEgVuMAUAUgQMBBoFGwAjNggAHAcKAE0HBgBuMAMAUQYMBW4gBgA1ACM2CQAjNwoAGggBAE0IBwBNBwYAbjAFAEUGDwMNAQ8AAAABAAAANQABAAEANwABAAEAAQAAAIoCAAAEAAAAcBAEAAAADgALAA55BAEaAgeHAwIXBkt6AwQZA8MDBR0GPNIeBQEFAgUEBQUeAwEeBQAIAA4AAAABAAAAAwAAAAIAAAACAAkAAgAAAAMACAABAAAABwAGPGluaXQ+AAFMAAJMTAADTExMADNMY29tL211eWUvaG9vay9oaWRkZW5hcGkvU2V0QWxsSGlkZGVuQXBpRXhlbXB0aW9uczsAEUxqYXZhL2xhbmcvQ2xhc3M7ABRMamF2YS9sYW5nL0NsYXNzPCo+OwASTGphdmEvbGFuZy9PYmplY3Q7ABJMamF2YS9sYW5nL1N0cmluZzsAFUxqYXZhL2xhbmcvVGhyb3dhYmxlOwAaTGphdmEvbGFuZy9yZWZsZWN0L01ldGhvZDsAHlNldEFsbEhpZGRlbkFwaUV4ZW1wdGlvbnMuamF2YQABVgACVloAAVoAEltMamF2YS9sYW5nL0NsYXNzOwATW0xqYXZhL2xhbmcvT2JqZWN0OwATW0xqYXZhL2xhbmcvU3RyaW5nOwAXZGFsdmlrLnN5c3RlbS5WTVJ1bnRpbWUAB2Zvck5hbWUAEWdldERlY2xhcmVkTWV0aG9kAApnZXRSdW50aW1lABBnZXRSdW50aW1lTWV0aG9kAAZpbnZva2UAB3J1bnRpbWUADHJ1bnRpbWVDbGFzcwANc2V0QWNjZXNzaWJsZQAWc2V0SGlkZGVuQXBpRXhlbXB0aW9ucwAcc2V0SGlkZGVuQXBpRXhlbXB0aW9uc01ldGhvZAABdACbAX5+RDh7ImJhY2tlbmQiOiJkZXgiLCJjb21waWxhdGlvbi1tb2RlIjoiZGVidWciLCJoYXMtY2hlY2tzdW1zIjpmYWxzZSwibWluLWFwaSI6MSwic2hhLTEiOiJmYWNlZGY0MWJiZDI4YjU2M2QxZTllMDljNWY3MmQ3YzVjYTU5OGQ1IiwidmVyc2lvbiI6IjguMi4yLWRldiJ9AAAAAgAAgYAEyAQBCbgDAAAAAAAAAA0AAAAAAAAAAQAAAAAAAAABAAAAHwAAAHAAAAACAAAACwAAAOwAAAADAAAABgAAABgBAAAFAAAABwAAAGABAAAGAAAAAQAAAJgBAAABIAAAAgAAALgBAAADIAAAAgAAAGACAAABEAAABAAAAJACAAACIAAAHwAAAK4CAAAAIAAAAQAAADsFAAADEAAAAQAAAEwFAAAAEAAAAQAAAFAFAAA="

    private fun bypassApi29(context: Context) = runCatching {
        val bytes = Base64.decode(DEX_IN_BASE64, Base64.NO_WRAP)
        val codeCacheDir = context.cacheDir
        if (codeCacheDir == null || !codeCacheDir.exists() || !codeCacheDir.isDirectory) {
            Log.d(TAG, "exemptAllByClassLoader: codeCacheDir is null or not a directory")
            return@runCatching false
        }
        val fixAllForBase64File = File(codeCacheDir, "FixAllForBase64.dex")
        if (fixAllForBase64File.exists() && !fixAllForBase64File.delete()) {
            Log.d(TAG, "exemptAllByClassLoader: delete file failed")
            return@runCatching false
        }

        //Write DEX file
        BufferedOutputStream(FileOutputStream(fixAllForBase64File)).use {
            it.write(bytes)
            it.flush()
        }

        //Before using the dex file, it must be set to read-only
        if (!fixAllForBase64File.setReadOnly()) {
            Log.d(TAG, "exemptAllByClassLoader: setReadOnly failed")
            return@runCatching false
        }

        val dexFile = DexFile(fixAllForBase64File)
        //Reflection call SetAllHiddenApiExertions
        val clazz =
            dexFile.loadClass("com.muye.hook.hiddenapi.SetAllHiddenApiExemptions", null)
                ?: return@runCatching false
        clazz.getDeclaredMethod("invoke").invoke(null)
        dexFile.close()
        true
    }.getOrDefault(false)
}