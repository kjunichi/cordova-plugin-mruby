package com.hatenablog.abrakatabura.mruby;

import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaWebView;
import org.apache.cordova.CallbackContext;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

/**
 * This class echoes a string called from JavaScript.
 */
public class Mruby extends CordovaPlugin {
  private static CordovaWebView stdout;

    @Override
    public void initialize(CordovaInterface cordova, CordovaWebView webView) {
        super.initialize(cordova, webView);
        // your init code here
        stdout = webView;
        MrubyJni.initialize();
    }

    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        if (action.equals("mrbLoadString")) {
            String jniString = MrubyJni.mrbLoadString(args.getString(0));
            String message = ". JNI says: " + jniString;
            Mruby.mrbLoadString(message, callbackContext);
            return true;
        }
        return false;

    }

    private static void mrbLoadString(String message, CallbackContext callbackContext) {
        if (message != null && message.length() > 0) {
            callbackContext.success(message);
        } else {
            callbackContext.error("Expected one non-empty string argument.");
        }
    }
    synchronized public static void printstr(String s) {
      //Log.d("MyApp", "str = " + s);
      final String js = "mruby.__jsPuts(\""+s+"\")";
      //Log.d("MyApp", "js = " + js);
      //cordova.getThreadPool().execute(new Runnable() {
      stdout.getView().post(new Runnable(){
        public void run(){
          //stdout.sendJavascript(js);
          stdout.loadUrl("javascript:" + js);
        }
      });
    }
}
