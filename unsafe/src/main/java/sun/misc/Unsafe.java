package sun.misc;

import java.lang.reflect.Field;

public class Unsafe {
    public long objectFieldOffset(Field var1) {
        return 0;
    }

    public long getLong(Object var1, long var2) {
        return 0;
    }

    public void putLong(Object var1, long var2, long var4) {
    }

    public native void putObject(Object obj, long offset, Object newValue);

    public native Object getObject(Object obj, long offset);

    public void putInt(Object var1, long var2, int var4){}

    public int getInt(long var1) {
        return 0;
    }

    public native Object allocateInstance(Class<?> var1) throws InstantiationException;
}