package com.idkjava.thelements;

import android.app.Activity;

import com.flurry.android.FlurryAgent;

public class FlurryActivity extends Activity
{
    // Flurry reporting
	@Override
	public void onStart()
	{
		super.onStart();
		FlurryAgent.onStartSession(this, Globals.flurryAPIKey);
	}
	@Override
	public void onStop()
	{
		super.onStop();
		FlurryAgent.onEndSession(this);
	}
}
