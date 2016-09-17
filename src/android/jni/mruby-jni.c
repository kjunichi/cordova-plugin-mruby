/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <android/log.h>
#include <string.h>
#include <jni.h>
#include <mruby.h>
#include <mruby/string.h>
#include <mruby/compile.h>

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
jstring
Java_com_hatenablog_abrakatabura_mruby_MrubyJni_mrbLoadString( JNIEnv* env,
  jobject thiz, jstring script )
{
  mrb_value val;
  char *str, *buf;
  const char *jscript;
  int len;
  jboolean b;

  //jscript = (*env)->GetStringUTFChars(env,script, &b);
  jscript = (*env)->GetStringUTFChars(env,script, NULL);
  __android_log_print(ANDROID_LOG_DEBUG,"Tag", "jscript = %s",jscript);
#if defined(__arm__)
  #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
   #define ABI "x86"
#elif defined(__x86_64__)
   #define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
   #define ABI "mips64"
#elif defined(__mips__)
   #define ABI "mips"
#elif defined(__aarch64__)
   #define ABI "arm64-v8a"
#else
   #define ABI "unknown"
#endif

  // TODO
  str = (char*)malloc(sizeof(char)*1024);
  mrb_state *mrb = mrb_open();
  if (!mrb) { /* handle error */ }
  //puts("Executing Ruby code from C!");
  //val = mrb_load_string(mrb, "c=Webcam.new;c.capture {|img|;$l=img;};c.snap;a=1+2;\"Hello, MRuby 1+2 = #{$l.length}\"");
  val = mrb_load_string(mrb, jscript);
  // val = mrb_load_string(mrb, "c=Webcam.new;a=1+2;\"Hello, MRuby 1+2 = #{a}\"");

  memcpy(str,RSTRING_PTR(val),RSTRING_LEN(val));
  str[RSTRING_LEN(val)]='\0';
  mrb_close(mrb);
  buf = (char*)malloc(sizeof(char)*1024);
  snprintf(buf, 1024, "%s%s",jscript,str);

  (*env)->ReleaseStringUTFChars(env, script, jscript);
  return (*env)->NewStringUTF(env, buf);
}
