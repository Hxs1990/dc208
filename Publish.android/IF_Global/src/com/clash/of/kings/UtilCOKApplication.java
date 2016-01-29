package com.clash.of.kings;

import org.hcg.IF.UtilApplication;

import android.app.Application;
import android.util.Log;

import com.parse.Parse;
import com.parse.ParseACL;
import com.parse.ParseUser;

public class UtilCOKApplication extends UtilApplication {

	public static void onCreate(Application app) {
		Log.d("COK","UtilDiffApplication onCreate");
		try{
	    	Parse.initialize(app, parseAppId, parseAppKey);
	    	ParseUser.enableAutomaticUser();
	    	ParseACL defaultACL = new ParseACL();
	    	ParseACL.setDefaultACL(defaultACL, true);
		}catch(Exception e){
			e.printStackTrace();
		}catch(Throwable e){
			e.printStackTrace();
		}
	}
}