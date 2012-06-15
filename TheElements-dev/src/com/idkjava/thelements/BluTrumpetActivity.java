package com.idkjava.thelements;

import com.blutrumpet.sdk.BluTrumpet;

import android.app.Activity;
import android.os.Bundle;
import android.view.ViewGroup;

public class BluTrumpetActivity extends Activity
{
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.blu_trumpet_activity);
		
		//Initialize Blu Trumpet
		if (!BluTrumpet.isInitialized())
		{
			 BluTrumpet.initWithAppId("2fbef62a-23e6-33f5-acbd-7071d4740f21", this);
		}
		
		ViewGroup container = ((ViewGroup) findViewById(R.id.blu_trumpet_container));
		container.addView(BluTrumpet.getAppWall());
	}
}
