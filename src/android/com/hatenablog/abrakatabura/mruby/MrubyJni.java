package com.hatenablog.abrakatabura.mruby;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;


public class MrubyJni {
  public static native String mrbLoadString(String s);
  public static native void initialize();
  public native String unimplementedStringFromJNI();
  static {
        System.loadLibrary("mruby-jni");
    }
}
