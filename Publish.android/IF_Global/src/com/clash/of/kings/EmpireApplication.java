package com.clash.of.kings;


import org.hcg.IF.UtilApplication;

import android.support.multidex.MultiDexApplication;
import android.util.Log;

public class EmpireApplication extends MultiDexApplication {
	
	@Override
	public void onCreate() {
		Log.d("EmpireApplication","enter onCreate");
		super.onCreate();
		UtilApplication.onCreate(this);
		UtilCOKApplication.onCreate(this);
	}
}