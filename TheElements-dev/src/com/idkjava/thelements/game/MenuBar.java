package com.idkjava.thelements.game;

import com.flurry.android.FlurryAgent;
import com.idkjava.thelements.BluTrumpetActivity;
import com.idkjava.thelements.MainActivity;

import com.idkjava.thelements.R;
import android.content.Context;
import android.content.Intent;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.Toast;

public class MenuBar extends LinearLayout
{
	//Used when exit is called because we need the specific instance of the activity to end
	private MainActivity activity;

	private Context context;

	private static ImageButton eraser_button;
	private static ImageButton play_pause_button;
	private static ImageButton save_button;
	private static ImageButton load_button;
	private static ImageButton app_wall_button;
	private static ImageButton clear_button;

	//Used for eraser
	public static boolean eraserOn = false;
	private static char tempElement = 0;

	//Constructor
	public MenuBar(Context context, AttributeSet attrs)
	{
		super(context, attrs);
		this.context = context;
		setGravity(Gravity.CENTER_HORIZONTAL);
	}

	//Used to get specific instance of activity
	public void setActivity(MainActivity act)
	{
		activity = act;
	}

	//Set the eraser to the off position
	public static void setEraserOff()
	{
		eraserOn = false;
		eraser_button.setImageResource(R.drawable.eraser);

		MainActivity.setElement(tempElement);
	}
	//Set the eraser to the off position
	public void setPlayState(boolean state)
	{
		
		if (state)
		{
			play_pause_button.setImageResource(R.drawable.pause);
		}
		else
		{
			play_pause_button.setImageResource(R.drawable.play);
		}
	}	

	//Called when it's finished inflating the XML layout
	@Override
	protected void onFinishInflate()
	{
		//Set up all the variables for the objects
		eraser_button = (ImageButton) findViewById(R.id.eraser_button);
		play_pause_button = (ImageButton) findViewById(R.id.play_pause_button);
		save_button = (ImageButton) findViewById(R.id.save_button);
		load_button = (ImageButton) findViewById(R.id.load_button);
		app_wall_button = (ImageButton) findViewById(R.id.app_wall_button);
		clear_button = (ImageButton) findViewById(R.id.clear_screen_button);

		//Set up the OnClickListener for the eraser button
		eraser_button.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				//If it was on eraser, swap back to regular element
				if (eraserOn)
				{
					MainActivity.setElement(tempElement);

					//Change the button to look unclicked
					eraser_button.setImageResource(R.drawable.eraser);
				}
				//If it is on a normal element, go to eraser and store that element for later
				else
				{
					tempElement = MainActivity.getElement();
					MainActivity.setElement(MainActivity.ERASER_ELEMENT);

					//Change the button to look clicked
					eraser_button.setImageResource(R.drawable.eraser_on);
				}
				eraserOn = !eraserOn;
			}
		});
		if (MainActivity.getElement() == MainActivity.ERASER_ELEMENT) //If the current element is eraser
		{
			//Start off the button to being on
			eraser_button.setImageResource(R.drawable.eraser_on);
		}
		else
		{
			//Start off the eraser in "off" position
			eraser_button.setImageResource(R.drawable.eraser);
		}

		//Set up the OnClickListener for the play/pause button
		play_pause_button.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				MainActivity.play = !MainActivity.play;
				MainActivity.setPlayState(MainActivity.play);

				if (MainActivity.play)
				{
					play_pause_button.setImageResource(R.drawable.pause);
				}
				else
				{
					play_pause_button.setImageResource(R.drawable.play);
				}
			}

		});
		if (MainActivity.play)
		{
			play_pause_button.setImageResource(R.drawable.pause);
		}
		else
		{
			play_pause_button.setImageResource(R.drawable.play);
		}

		//Set up the OnClickListener for the save button
		save_button.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				Log.d("LOG", "Checking sdcard state: " + Environment.getExternalStorageState() + " vs " + Environment.MEDIA_MOUNTED);
				if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED))
				{
					((MainActivity) context).saveState();
				}
				else
				{
					Toast.makeText(context, R.string.sdcard_not_found, Toast.LENGTH_SHORT).show();
				}
			}
		});

		//Set up the OnClickListener for the load button
		load_button.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED))
				{
					((MainActivity) context).loadState();
				
				}
				else
				{
					Toast.makeText(context, R.string.sdcard_not_found, Toast.LENGTH_SHORT).show();
				}
			}
		});

		//Set up the OnClickListener for the app wall button
		app_wall_button.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				FlurryAgent.logEvent("BluTrumpet button (app)");
				((MainActivity) context).startActivity(new Intent(activity, BluTrumpetActivity.class));
			}
		});

		//Set up the OnClickListener for the exit button
		clear_button.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				((MainActivity) context).clearScreen();
			}
		});
	}
}