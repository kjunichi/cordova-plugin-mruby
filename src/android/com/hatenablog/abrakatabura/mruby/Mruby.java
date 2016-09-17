package com.hatenablog.abrakatabura.mruby;

import org.apache.cordova.CordovaPlugin;
import org.apache.cordova.CallbackContext;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * This class echoes a string called from JavaScript.
 */
public class Mruby extends CordovaPlugin {

    @Override
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        if (action.equals("mrbLoadString")) {
            
            String jniString = MrubyJni.mrbLoadString(args.getString(0));
            String message = ". JNI says: " + jniString;
            this.mrbLoadString(message, callbackContext);
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
}
