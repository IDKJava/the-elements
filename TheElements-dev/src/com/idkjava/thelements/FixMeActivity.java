package com.idkjava.thelements;

import java.io.File;

import com.idkjava.thelements.game.FileManager;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class FixMeActivity extends Activity
{
	private TextView logOutput;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.fix_me_activity);
		
		logOutput = (TextView) findViewById(R.id.fix_me_textview);
		
		// Thread this later if it becomes processor intensive
		fixMe();
	}
	
	// Core function -- does all the fixing
	private void fixMe()
	{
		// Try to fix crashing by moving temp.sav to temp.sav.bak
		logOutput.setText(logOutput.getText() + "\n\n" + getResources().getString(R.string.fix_me_temp_sav));
		
		File file = new File(FileManager.ROOT_DIR + FileManager.SAVES_DIR + "temp" + FileManager.SAVE_EXT);
		boolean success = file.renameTo(new File(FileManager.ROOT_DIR + FileManager.SAVES_DIR + "temp" + FileManager.SAVE_EXT + FileManager.BACKUP_EXT));
		if (success)
		{
			logOutput.setText(logOutput.getText() + "\n" + getResources().getString(R.string.fix_me_temp_sav_succeeded));
		}
		else
		{
			logOutput.setText(logOutput.getText() + "\n" + getResources().getString(R.string.fix_me_temp_sav_failed));
		}
	
		// Finally
		logOutput.setText(logOutput.getText() + "\n\n" + getResources().getString(R.string.fix_me_done));
	}
}
