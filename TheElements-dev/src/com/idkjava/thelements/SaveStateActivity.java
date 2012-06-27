package com.idkjava.thelements;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.idkjava.thelements.game.SaveManager;

public class SaveStateActivity extends FlurryActivity
{
	public static Button saveButton;
	public static EditText statename;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.save_state_activity);
		
		saveButton = (Button) findViewById(R.id.save_state_button);
		statename = (EditText) findViewById(R.id.save_state_filename);
		
		saveButton.setOnClickListener
		(
				new OnClickListener()
				{
					public void onClick(View v)
					{
						InputMethodManager imm = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
						imm.hideSoftInputFromWindow(statename.getWindowToken(), 0);
						boolean success = SaveManager.saveState(statename.getText().toString());
						if (!success)
						{
							Toast.makeText(getApplicationContext(), R.string.save_state_failed, Toast.LENGTH_SHORT).show();
						}
						finish();
					}
				}
		);
		
		if(MainActivity.last_state_loaded != null)
		{
			statename.setText(MainActivity.last_state_loaded);
		}
	}
}
