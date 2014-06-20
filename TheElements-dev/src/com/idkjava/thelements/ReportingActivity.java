package com.idkjava.thelements;

import android.app.Activity;

import com.flurry.android.FlurryAgent;
import com.google.analytics.tracking.android.EasyTracker;

public class ReportingActivity extends Activity
{
	@Override
	public void onStart()
	{
		super.onStart();
		// Flurry
		FlurryAgent.onStartSession(this, Globals.flurryAPIKey);
		// Google Analytics
		EasyTracker.getInstance(this).activityStart(this);
	}
	@Override
	public void onStop()
	{
		super.onStop();
		FlurryAgent.onEndSession(this);
		EasyTracker.getInstance(this).activityStop(this);
	}
}
