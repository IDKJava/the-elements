package com.idkjava.thelements.custom;

import com.idkjava.thelements.game.CustomElementManager;
import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;

public class CustomElementActivity extends Activity
{
	//TODO: 1) Depending on the Intent, ACTION_EDIT or ACTION_NEW, set up the UI with
	//either default or loaded data, and allow the user to edit and save
	// 2) Make sure no duplicate names can be entered.
	/*
	 * All elements held in same file, accessed by name.
	 */
	private Intent callingIntent;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		callingIntent = getIntent();
		if(callingIntent.getAction() == Intent.ACTION_VIEW)
		{
			//Get the filename that should be stored in the intent data
			Bundle bundle = getIntent().getExtras();
			String elementName = bundle.getString("name");
			
			//int customElementIndex = CustomElementManager.loadAndGetIndex(filename.toCharArray());
			//if(customElementIndex != -1)
			//{
				//Call getters to fetch the info for the specific element index
			//}
		}
		else
		{
			
		}
	}
	
	//When creating, the elements above and below the state that we point to are going to be
	//indices because if you want to point to an element not yet created there will be problems
	//When loading elements, these indices will temporarily be stored, and then once finished
	//will be resolved to pointers
}
