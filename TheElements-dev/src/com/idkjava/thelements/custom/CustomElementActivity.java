package com.idkjava.thelements.custom;

import com.idkjava.thelements.R;
import android.app.TabActivity;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.widget.TabHost;
import android.widget.TabHost.TabSpec;

public class CustomElementActivity extends TabActivity
{
	//TODO: 1) Depending on the Intent, ACTION_EDIT or ACTION_NEW, set up the UI with
	//either default or loaded data, and allow the user to edit and save
	// 2) Make sure no duplicate names can be entered.
	/*
	 * All elements held in same file, accessed by name.
	 */
	private String filename;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		// Set the content view
		setContentView(R.layout.custom_element_activity);
		// Set up some variables
		Intent intent;
		TabSpec spec;
		TabHost tabHost = getTabHost();
		Resources res = getResources();
		// Create the tabs
		intent = new Intent(CustomElementActivity.this, CustomElementBasicActivity.class);
		spec = tabHost.newTabSpec("basic").setIndicator(res.getString(R.string.basic_tab)).setContent(intent);
		tabHost.addTab(spec);
		intent = new Intent(CustomElementActivity.this, CustomElementAdvancedActivity.class);
		spec = tabHost.newTabSpec("advanced").setIndicator(res.getString(R.string.advanced_tab)).setContent(intent);
		tabHost.addTab(spec);
		tabHost.setCurrentTab(0);
		
		filename = getIntent().getStringExtra("filename");
	}
	
	//When creating, the elements above and below the state that we point to are going to be
	//indices because if you want to point to an element not yet created there will be problems
	//When loading elements, these indices will temporarily be stored, and then once finished
	//will be resolved to pointers
}
