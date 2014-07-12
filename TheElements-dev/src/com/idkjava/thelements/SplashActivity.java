package com.idkjava.thelements;

import java.io.File;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.widget.TextView;

import com.idkjava.thelements.game.FileManager;

public class SplashActivity extends ReportingActivity
{
    private TextView mLoadingText; 
    
	@Override
	protected void onCreate (Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState); //Call the super method
		
		setContentView(R.layout.splash_activity);
		mLoadingText = (TextView) findViewById(R.id.loading_text);
		
		// Upgrade files if needed
		maybeUpgradeFiles();
		
		new Handler().postDelayed(
				new Runnable()
				{
					@Override
					public void run()
					{
						//Create an Intent to start MainActivity
						Intent mainIntent = new Intent(SplashActivity.this, MainActivity.class);
						startActivity(mainIntent);
						finish();
					}
				}, 1000);
	}
	
	private void maybeUpgradeFiles()
	{
	    String oldText = mLoadingText.getText().toString();
	    mLoadingText.setText("Upgrading save files...");
	    
	    File elementDir = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR);
	    File[] elementFiles = elementDir.listFiles();
	    if (elementFiles != null)
	    {
	        for (File elementFile : elementFiles)
	        {
	            if (elementFile.getAbsolutePath().endsWith(FileManager.ELEMENT_EXT))
	            {
	                Log.i("TheElements", "Upgrading " + elementFile.getAbsolutePath());
	                mLoadingText.setText("Upgrading " + elementFile.getAbsolutePath());
	                upgradeCustomElement(elementFile.getAbsolutePath());
	            }
	        }
	    }
	    
	    File savesDir = new File(FileManager.ROOT_DIR + FileManager.SAVES_DIR);
	    File[] saveFiles = savesDir.listFiles();
	    if (saveFiles != null)
	    {
	        for (File saveFile : saveFiles)
	        {
	            if (saveFile.getAbsolutePath().endsWith(FileManager.SAVE_EXT))
	            {
	                Log.i("TheElements", "Upgrading " + saveFile.getAbsolutePath());
	                mLoadingText.setText("Upgrading " + saveFile.getAbsolutePath());
	                upgradeSaveFile(saveFile.getAbsolutePath());
	            }
	        }
	    }
	    
	    mLoadingText.setText(oldText);
	}
    
    private static native boolean upgradeCustomElement(String filename);
    private static native boolean upgradeSaveFile(String filename);
    
    static
    {
        System.loadLibrary("stlport_shared");
        try {
            System.loadLibrary("kamcord");
        } catch (UnsatisfiedLinkError e) {
            Log.d("TheElements", "Kamcord not supported");
        }
        System.loadLibrary("protobuf");
        System.loadLibrary("thelements"); // Load the JNI library (libthelements.so)
    }
}
