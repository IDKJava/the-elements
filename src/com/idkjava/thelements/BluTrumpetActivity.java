package com.idkjava.thelements;

import android.os.Bundle;
import android.view.ViewGroup;

import com.blutrumpet.sdk.BluTrumpet;

public class BluTrumpetActivity extends FlurryActivity
{
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.blu_trumpet_activity);
		
		ViewGroup container = ((ViewGroup) findViewById(R.id.blu_trumpet_container));
		container.addView(BluTrumpet.getAppWall());
	}
}
