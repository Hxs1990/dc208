package org.nbg.IF;

import android.app.Application;
import android.util.Log;

import com.adjust.sdk.Adjust;
import com.adjust.sdk.AdjustConfig;

public class UtilApplication {

    protected static String parseAppId = "mOzkRMxp0CwRal3O1rOXH9VJK4pIgJApzcY3Vmsj";
    protected static String parseAppKey = "vyXy2kQHrwmCv0sLCxIRFDwpZSZk2qyTirjKgmwh";
    protected static String adjustApptoken = "cbc4rfy5xrad";
	
	public static void onCreate(Application app) {
		Log.d("COK","UtilApplication onCreate");
		String appToken = adjustApptoken;
        String environment = AdjustConfig.ENVIRONMENT_PRODUCTION;
        AdjustConfig config = new AdjustConfig(app, appToken, environment);
        Adjust.onCreate(config);
	}
}