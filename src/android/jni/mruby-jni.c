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

mrb_state *mrb;
JNIEnv* gEnv;

static void
printstr(mrb_state *mrb, mrb_value obj)
{
  int len;
  char *str;
  jclass cls;
  jmethodID mid;

  if (mrb_string_p(obj)) {
    str = mrb_str_to_cstr(mrb, obj);
    __android_log_print(ANDROID_LOG_DEBUG,"Puts", "arg = %s", str);
    cls = (*gEnv)->FindClass(gEnv, "com/hatenablog/abrakatabura/mruby/Mruby");
    mid = (*gEnv)->GetStaticMethodID(gEnv, cls, "printstr", "(Ljava/lang/String;)V");
    (*gEnv)->CallStaticVoidMethod(gEnv, cls, mid, (*gEnv)->NewStringUTF(gEnv, str));
  }
}

static mrb_value
mrb_printstr(mrb_state *mrb, mrb_value self)
{
  mrb_value argv;

  mrb_get_args(mrb, "o", &argv);
  printstr(mrb, argv);

  return argv;
}

static mrb_value
mrb_gets(mrb_state *mrb, mrb_value self)
{
  jclass cls;
  jmethodID mid;
  mrb_value val;
  jstring inputJStr;
  const char *inputStr;
  const char *emptyStr = "";

  __android_log_print(ANDROID_LOG_DEBUG,"gets", "");
  cls = (*gEnv)->FindClass(gEnv, "com/hatenablog/abrakatabura/mruby/Mruby");
  mid = (*gEnv)->GetStaticMethodID(gEnv, cls, "gets", "()Ljava/lang/String;");
  inputJStr = (jstring)(*gEnv)->CallStaticObjectMethod(gEnv, cls, mid, NULL);
  if(inputJStr == NULL) {
    inputStr = emptyStr;
  } else {
    inputStr = (*gEnv)->GetStringUTFChars(gEnv, inputJStr, 0);
  }
  __android_log_print(ANDROID_LOG_DEBUG,"gets", " inputStr = %s", inputStr);
  return mrb_str_new_cstr(mrb, inputStr);
}

void
Java_com_hatenablog_abrakatabura_mruby_MrubyJni_initialize( JNIEnv* env,
  jobject thiz)
{
  __android_log_print(ANDROID_LOG_DEBUG,"Init", "Java_com_hatenablog_abrakatabura_mruby_MrubyJni_initialize");

  mrb = mrb_open();
  struct RClass *krn;
  krn = mrb->kernel_module;
  mrb_define_method(mrb, krn, "__printstr__", mrb_printstr, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, krn, "gets", mrb_gets, MRB_ARGS_REQ(1));
}



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

  gEnv = env;
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
  buf = (char*)malloc(sizeof(char)*1024);

  if (!mrb) { /* handle error */ }
  //puts("Executing Ruby code from C!");
  //val = mrb_load_string(mrb, "c=Webcam.new;c.capture {|img|;$l=img;};c.snap;a=1+2;\"Hello, MRuby 1+2 = #{$l.length}\"");
  val = mrb_load_string(mrb, jscript);
  // val = mrb_load_string(mrb, "c=Webcam.new;a=1+2;\"Hello, MRuby 1+2 = #{a}\"");
  if (!mrb_string_p(val)) {
    snprintf(buf, 1024, "%s",jscript);
  } else {
    memcpy(str,RSTRING_PTR(val),RSTRING_LEN(val));
    str[RSTRING_LEN(val)]='\0';
    //mrb_close(mrb);
    snprintf(buf, 1024, "%s%s",jscript,str);
  }
  free(str);
  (*env)->ReleaseStringUTFChars(env, script, jscript);

  return (*env)->NewStringUTF(env, buf);
}
