package com.idkjava.thelements;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.webkit.WebView;
import android.widget.Button;

import com.idkjava.thelements.custom.CustomElementManagerActivity;

public class MenuActivity extends FlurryActivity
{
	public static Button start_game_button;
	public static Button custom_elements_button;
	public static Button blu_trumpet_button;
	public static Button how_to_play_button;
	public static Button about_button;
	public static boolean loaded = false;
	public long stime;
	
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState); //Call the super method
		
		requestWindowFeature(Window.FEATURE_NO_TITLE); //Get rid of title bar
		stime = System.currentTimeMillis();
		
		setContentView(R.layout.menu_activity);
		
		//Define all the objects
		start_game_button = (Button) findViewById(R.id.start_game_button);
		blu_trumpet_button = (Button) findViewById(R.id.blu_trumpet_button);
		custom_elements_button = (Button) findViewById(R.id.custom_elements_button);
		how_to_play_button = (Button) findViewById(R.id.how_to_play_button);
		about_button = (Button) findViewById(R.id.about_button);
		
		
		
		start_game_button.setOnClickListener
		(
			new OnClickListener()
			{
				public void onClick(View v)
				{
					//Start the main app activity
					startActivity(new Intent(MenuActivity.this, SplashActivity.class));
				}
			}
		);
		
		custom_elements_button.setOnClickListener
		(
			new OnClickListener()
			{
				public void onClick(View v)
				{
					//Start the CustomElementManagerActivity
					startActivity(new Intent(MenuActivity.this, CustomElementManagerActivity.class));
				}
			}
		);
		
		blu_trumpet_button.setOnClickListener
		(
			new OnClickListener()
			{
				public void onClick(View v)
				{
					//Start the Blu Trumpet Activity
					startActivity(new Intent(MenuActivity.this, BluTrumpetActivity.class));
					
					//OLD -- Clear quicksave
					//MainActivity.removeTempSave();
					//Toast.makeText(getBaseContext(), "Quicksave file erased", Toast.LENGTH_SHORT).show();
				}
			}
		);
		
		how_to_play_button.setOnClickListener
		(
			new OnClickListener()
			{
				public void onClick(View v)
				{
					//Show the instructions
					how_to_play();
				}
			}
		);
		
		about_button.setOnClickListener
		(
			new OnClickListener()
			{
				public void onClick(View v)
				{
					//Show the about dialog
					showDialog(1);
				}
			}
		);
	}

	@Override
	public void onResume()
	{
		stime = System.currentTimeMillis();
		super.onResume(); //Call the super method
	}
	
	public void how_to_play()
	{
		StringBuffer data = new StringBuffer();
		try
		{
			InputStream stream = getAssets().open("instructions.html");
			BufferedReader in = new BufferedReader(new InputStreamReader(stream), 8192);
			while(true)
			{
				String line = in.readLine();
				if(line == null)
				{
					break;
				}
				data.append(line).append("\n");
			}
		}
		catch (IOException e)
		{
			e.printStackTrace();
			return;
		}
		
		WebView how_to_play = new WebView(this);
		how_to_play.setBackgroundColor(0x00000000);
		how_to_play.loadData(data.toString(), "text/html", "ascii");
		
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setCancelable(true);
		Resources res = getResources();
		builder.setTitle(res.getText(R.string.how_to_play_title));
		builder.setView(how_to_play);
		builder.show();
	}
	
	protected Dialog onCreateDialog(int id)  // This is called when showDialog is called
	{
	   	if (id == 1) //Copyright message
		{
			AlertDialog.Builder builder = new AlertDialog.Builder(this); //Declare the object
			Resources res = getResources();
			builder.setMessage(res.getText(R.string.about_text));		      
			AlertDialog alert = builder.create(); //Create object
			return alert; //Return handle
		}
		
		return null; //No need to return anything, just formality
	}
}
