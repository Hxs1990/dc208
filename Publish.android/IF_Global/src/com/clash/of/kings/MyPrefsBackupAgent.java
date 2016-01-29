package com.clash.of.kings;

import android.app.backup.BackupAgentHelper;
import android.app.backup.SharedPreferencesBackupHelper;
import android.util.Log;

public class MyPrefsBackupAgent extends BackupAgentHelper {
	// The name of the SharedPreferences file
    static final String PREFS = "user_preferences_new";

    // A key to uniquely identify the set of backup data
    static final String PREFS_BACKUP_KEY = "prefs";

    // Allocate a helper and add it to the backup agent
    @Override
    public void onCreate() {
    	Log.d("MyPrefsBackupAgent start", "MyPrefsBackupAgent start");
        SharedPreferencesBackupHelper helper =
                new SharedPreferencesBackupHelper(this, PREFS);
        addHelper(PREFS_BACKUP_KEY, helper);
    }
}
