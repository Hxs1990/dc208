package com.clash.of.publish;

import it.partytrack.sdk.Track;

import org.nbg.IF.IF;
import org.nbg.stac.empire.publish.AbstractPublishImpl;
import org.nbg.util.Constants;
import org.nbg.util.GameContext;

import android.util.Log;

import com.adjust.sdk.Adjust;
import com.adjust.sdk.AdjustEvent;
import com.google.ads.conversiontracking.AdWordsConversionReporter;
import com.nanigans.android.sdk.NanigansEventManager;
import com.parse.ParseAnalytics;
import com.parse.ParseInstallation;
import com.vk.sdk.VKScope;
import com.vk.sdk.VKSdk;

/**
 * 重构于20150908
 * @author Still4
 *
 */
public class GlobalPublishImpl extends AbstractPublishImpl
{
	
	@Override
	public void initlize() {
		super.initlize();
	}

	@Override
	public String getPublishChannel() {
		return PUBLISH_GLOBAL;
	}
	
	@Override
	public void triggerEventAchievedLevel(int level){
		super.triggerEventAchievedLevel(level);
		if(level == 3){
			Track.event(47736);
		}
		if(level == 6){
			Track.event(47735);
		}
	}

	@Override
	public void triggerEventPurchase(String cost,String itemId){
		super.triggerEventPurchase(cost, itemId);
		
		float itemPrice = Float.parseFloat(cost);
		Track.payment(itemId, itemPrice, "USD", 1);
		
		NanigansEventManager.getInstance().trackPurchase(Float.parseFloat(cost)*100, String.valueOf(cost), 1D);
		
		AdWordsConversionReporter.reportWithConversionId(IF.getInstance().getApplicationContext(),
				"950451480", "VYeNCKfCtF4QmPqaxQM", cost, true);
	}
	
	@Override
	public void trackEvent(String event){
		super.trackEvent(event);
		String eventToken = "";
		if("two_days_login".equals(event))
			eventToken = "9f47vg";
		if("tutorial_over".equals(event))
			eventToken = "hw2r00";
		if("first_pay".equals(event))
			eventToken = "eyxm0i";
	    if("reach_level_4".equals(event))
	    	eventToken = "qumz9u";
	    if("reach_level_5".equals(event))
	    	eventToken = "xzepi1";
	    if("reach_level_6".equals(event))
	    	eventToken = "biwo24";
		if(!"".equals(eventToken)){
			AdjustEvent mevent = new AdjustEvent(eventToken);
			Adjust.trackEvent(mevent);
		}
	}
	
	@Override
	public void queryHistoryPurchase(){
		Log.d("COK", "COK queryPurchaseOrder start");
		IF.getInstance().runOnUiThread(new Runnable() {
			public void run() {
				try {
					if(IF.getInstance().m_payment != null)
						IF.getInstance().m_payment.queryPurchaseOrder();
					Log.d("COK", "COK queryPurchaseOrder finish");
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	
	@Override
	public void loginSNS(String pf){
		Log.d("COK", "loginPlatform "+pf);
		if(Constants.Platform.VK.equals(pf)){
			final String[] sMyScope = new String[]{
	            VKScope.FRIENDS,
	            VKScope.WALL,
	            VKScope.PHOTOS,
	            VKScope.NOHTTPS
			};
			VKSdk.login(IF.getInstance(), sMyScope);
		}
	}
	
	@Override
	public void logoutSNS(String pf){
		Log.d("COK", "logoutPlatform "+pf);
		if(Constants.Platform.VK.equals(pf))
			VKSdk.logout();
	}
	
	@Override 
	public void parseInit(){
		try{
	    	ParseInstallation data = ParseInstallation.getCurrentInstallation();
	    	parseNotifyToken = data.getInstallationId();
	    	data.saveInBackground();
		}catch(Exception e){
			e.printStackTrace();
		}catch(Throwable e){
			e.printStackTrace();
		}
    }
	
	@Override
	public void parseTrackAppOpened(){
		ParseAnalytics.trackAppOpenedInBackground(GameContext.getActivityInstance().getIntent());
	}

}
