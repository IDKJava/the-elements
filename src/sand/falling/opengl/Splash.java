package sand.falling.opengl;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;

public class Splash extends Activity
{
	@Override
	protected void onCreate (Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState); //Call the super method
		
		setContentView(R.layout.splash);
		
		new Handler().postDelayed(
				new Runnable()
				{
					@Override
					public void run()
					{
						//Create an Intent to start DemoActivity
						Intent mainIntent = new Intent(Splash.this, DemoActivity.class);
						Splash.this.startActivity(mainIntent);
						Splash.this.finish();
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
