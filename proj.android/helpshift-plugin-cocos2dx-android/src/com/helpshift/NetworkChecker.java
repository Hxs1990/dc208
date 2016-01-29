package com.helpshift;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.InetAddress;

import android.util.Log;

public class NetworkChecker implements Runnable {
	
	private String theUser = null;
	private String theHost = null;
	
	public NetworkChecker(String strUuid, String strHost) {
		theUser = strUuid;
		theHost = strHost;
	}

	@Override
	public void run() {
		doPing();
	}
	
	public void stop() {
	}
	
	public void doPing() {
		try {
			InetAddress theInetAddress = InetAddress.getByName(theHost);
			StringBuilder commandLine = new StringBuilder("ping");
			// interval
			commandLine.append(" -i").append(1);
			// deadline 等待每次回复的超时时间(毫秒)
			commandLine.append(" -w").append(70);
			// count
			commandLine.append(" -c").append(5);
			// packetsize
			commandLine.append(" -s").append(64).append(" ");
			commandLine.append(theInetAddress.getHostAddress());
			Log.d("debug", commandLine.toString());
			Process localProcess = Runtime.getRuntime().exec(commandLine.toString());
			
			BufferedReader resultReader = new BufferedReader(new InputStreamReader(localProcess.getInputStream()), 1024);
			StringBuilder sb = new StringBuilder();
			String str1 = resultReader.readLine();
			while (str1 != null) {
				Log.d("debug", str1);
				str1 = str1.trim();
				if (str1.length() > 0) {
					sb.append(str1).append(";");
				}
				str1 = resultReader.readLine();
			}
			
			localProcess.waitFor();
			resultReader.close();
			localProcess.getInputStream().close();
			localProcess.getOutputStream().close();
//			sb.append("\r\n\t");
			sb.append(theUser);
			if( sb.toString() != null )
			{
				com.helpshift.support.Log.d("ping", sb.toString());
				Log.e("[ping]" + theUser, sb.toString());
			}
		} catch (Exception e) {
			if( e.getMessage() != null )
				Log.e("debug", e.getMessage());
		}
	}

}
