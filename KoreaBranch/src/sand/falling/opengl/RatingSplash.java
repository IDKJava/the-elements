package sand.falling.opengl;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Window;

public class RatingSplash extends Activity
{
	@Override
	protected void onCreate (Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState); //Call the super method

		requestWindowFeature(Window.FEATURE_NO_TITLE); //Get rid of title bar
		setContentView(R.layout.ratingsplash);
		
		Thread ratingSplashThread = new Thread()
		{
			@Override
			public void run()
			{
				try
				{
					int waited = 0;
					while(waited < 5000)
					{
						sleep(100);
						waited+=100;
					}
				}
				catch(InterruptedException e)
				{
					//Do nothing
				}
				finally
				{
					finish();
					Intent i = new Intent(RatingSplash.this, Menu.class);
					startActivity(i);
				}
			}
		};
		ratingSplashThread.start();
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
