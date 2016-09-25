package com.hatenablog.abrakatabura.mruby;

import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CordovaInterface;
import org.apache.cordova.CordovaWebView;
import org.apache.cordova.CallbackContext;
import org.apache.cordova.PluginResult;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

/**
 * This class echoes a string called from JavaScript.
 */
public class Mruby extends CordovaPlugin {
  private static CordovaWebView sWebView;
  private static boolean waitInput;
  private static String inputString;
  //private static String _jniString;

  // private static void setResult(String r) {
  //   _jniString = r;
  // }

    @Override
    public void initialize(CordovaInterface cordova, CordovaWebView webView) {
        super.initialize(cordova, webView);

        sWebView = webView;
        MrubyJni.initialize();
    }

    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        final CallbackContext cb = callbackContext;
        if (action.equals("mrbLoadString")) {
          final String script = args.getString(0);
            Thread th = new Thread(new Runnable(){
              @Override
              public void run() {
                //String jniString = 
                //setResult(jniString);
                String message = ". JNI says: " + MrubyJni.mrbLoadString(script);;
                mrbLoadString(message, cb);
              }
            });
            th.start();


            return true;
        }
        if (action.equals("_doneGets")) {
          Log.d("Mruby", "_doneGets");
          inputString = args.getString(0);
          callbackContext.success("message: "+ inputString);
          waitInput = false;
          return true;
        }
        return false;

    }

    private void mrbLoadString(String message, CallbackContext callbackContext) {
        if (message != null && message.length() > 0) {
            callbackContext.success(message);
        } else {
            callbackContext.error("Expected one non-empty string argument.");
        }
    }
    public static String gets() {
      Log.d("Mruby", "gets");
      waitInput = true;
      inputString = "dummy";
      final String js = "console.log(mruby.__jsGets())";
      sWebView.getView().post(new Runnable() {
        public void run(){
          Log.d("Mruby", "gets/exec js = " + js);
          sWebView.loadUrl("javascript:" + js);
          Log.d("Mruby", "gets/exec js done");
        }
      });
      Log.d("Mruby", "gets/wait");
      while (waitInput) {
        try {
          Thread.sleep(200);
        } catch (InterruptedException e) {
          Thread.currentThread().interrupt();
        }
      }
      Log.d("Mruby", "gets/rtn = "+inputString);
      return inputString;
    }
    
    public static void printstr(String s) {
      //Log.d("MyApp", "str = " + s);
      final String js = "mruby.__jsPuts(\""+s+"\")";
      //Log.d("MyApp", "js = " + js);
      sWebView.getView().post(new Runnable(){
        public void run(){
          sWebView.loadUrl("javascript:" + js);
        }
      });
    }
}
