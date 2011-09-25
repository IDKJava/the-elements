package idkjava.thelements;

import idkjava.thelements.R;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;

public class SplashActivity extends Activity
{
	@Override
	protected void onCreate (Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState); //Call the super method
		
		setContentView(R.layout.splash_activity);
		
		new Handler().postDelayed(
				new Runnable()
				{
					@Override
					public void run()
					{
						//Create an Intent to start DemoActivity
						Intent mainIntent = new Intent(SplashActivity.this, MainActivity.class);
						startActivity(mainIntent);
						finish();
					}
				}, 5);
	}
	
	@Override
	protected void onResume()
	{
		super.onResume();
	}
	
	@Override
	protected void onPause()
	{
		super.onPause();
	}
}
