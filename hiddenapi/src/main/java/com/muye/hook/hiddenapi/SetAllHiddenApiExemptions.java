package com.muye.hook.hiddenapi;

import androidx.annotation.Keep;

import java.lang.reflect.Method;

@Keep
public class SetAllHiddenApiExemptions {
    public static boolean invoke(){
        try{
            Class<?> runtimeClass = Class.forName("dalvik.system.VMRuntime");
            //get VMRuntime instance
            Method getRuntimeMethod = runtimeClass.getDeclaredMethod("getRuntime");
            getRuntimeMethod.setAccessible(true);
            Object runtime = getRuntimeMethod.invoke(null);

            //call VMRuntime.setHiddenApiExemptions("L")
            Method setHiddenApiExemptionsMethod = runtimeClass.getDeclaredMethod("setHiddenApiExemptions", String[].class);
            setHiddenApiExemptionsMethod.setAccessible(true);
            setHiddenApiExemptionsMethod.invoke(runtime, new Object[]{new String[]{"L"}});
            return true;
        }catch (Throwable t){
            return false;
        }
    }
}
