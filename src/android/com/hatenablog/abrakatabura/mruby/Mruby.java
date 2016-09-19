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
  private static CordovaWebView stdout;
  private static CallbackContext ctx;
  private static boolean waitInput;
  private static String inputString;
  private static CordovaInterface _cordova;
  private static String _jniString;

  private static void setResult(String r) {
    _jniString = r;
  }

    @Override
    public void initialize(CordovaInterface cordova, CordovaWebView webView) {
        super.initialize(cordova, webView);
        // your init code here
        stdout = webView;
        _cordova = cordova;
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
                String jniString = MrubyJni.mrbLoadString(script);
                Mruby.setResult(jniString);
                String message = ". JNI says: " + _jniString;
                Mruby.mrbLoadString(message, cb);
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

    private static void mrbLoadString(String message, CallbackContext callbackContext) {
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
      //final String js = "mruby.__jsDoneGets(mruby.__jsGets())";
      final String js = "console.log(mruby.__jsGets())";
      //PluginResult dataResult = new PluginResult(PluginResult.Status.OK, js);
      //dataResult.setKeepCallback(true);
      //ctx.sendPluginResult(dataResult);
      //stdout.getView().evaluateJavascript(js,null);
      //_cordova.getThreadPool().execute(new Runnable() {
      stdout.getView().post(new Runnable() {
      //Runnable runnable = new Runnable() {
        public void run(){
          //stdout.sendJavascript(js);
          Log.d("Mruby", "gets/exec js = " + js);
          stdout.loadUrl("javascript:" + js);
          Log.d("Mruby", "gets/exec js done");
        }
      //};
      });
      //_cordova.getActivity().runOnUiThread(runnable);
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
      //cordova.getThreadPool().execute(new Runnable() {
      stdout.getView().post(new Runnable(){
        public void run(){
          //stdout.sendJavascript(js);
          stdout.loadUrl("javascript:" + js);
        }
      });
      //stdout.getView().evaluateJavascript(js,null);
    }
}
