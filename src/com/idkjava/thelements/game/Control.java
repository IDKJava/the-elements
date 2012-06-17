package com.idkjava.thelements.game;

import com.idkjava.thelements.MainActivity;

import com.idkjava.thelements.R;
import com.idkjava.thelements.custom.CustomElementManagerActivity;

import android.content.Context;
import android.content.Intent;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.SeekBar;

public class Control extends LinearLayout
{
	//The instance of the current activity is stored here and modified through setActvity (call from DemoActivity)
	private MainActivity activity;

	//Two objects in the control area
	private ImageButton element_picker_button;
	private ImageButton custom_elements_button;
	private SeekBar brush_size_slider;

	final CharSequence[] elementslist = {"Sand", "Water", "Plant", "Wall", "Fire", "Ice", "Generator", "Oil", "Magma", "Stone", "C4"};

	//Constructor
	public Control(Context context, AttributeSet attrs)
	{
		super(context, attrs);
	}

	//Sets the current instance of the activity
	public void setActivity(MainActivity act)
	{
		activity = act;
	}

	//Called once the the xml is finished inflating	
	@Override
	protected void onFinishInflate()
	{
		//Define the ImageButton and SeekBar set before using the res ids
		element_picker_button = (ImageButton) findViewById(R.id.element_picker_button);
		custom_elements_button = (ImageButton) findViewById(R.id.custom_elements_button);
		brush_size_slider = (SeekBar) findViewById(R.id.brush_size_slider);

		//Set a click listener for the button which should pop up element picker dialog when clicked
		element_picker_button.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				activity.showDialog(MainActivity.ELEMENT_PICKER); //Run the element picker dialog
			}
		});
		
		// Set a click listener for the button which should open up the CustomElementManagerActivity when clicked
		custom_elements_button.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				activity.startActivity(new Intent(activity, CustomElementManagerActivity.class));
			}
		});

		//Set a change listener for the seekbar
		brush_size_slider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
		{
			@Override
			public void onProgressChanged(SeekBar seekbar, int progress, boolean fromTouch)
			{
				//When it is dragged, set the brush size to 32 * the fraction of the bar dragged
				int p = 32 * progress / seekbar.getMax();
				MainActivity.setBrushSize((char) p);
			}

			//These aren't needed for now
			@Override
			public void onStartTrackingTouch(SeekBar seekbar)
			{}

			@Override
			public void onStopTrackingTouch(SeekBar seekbar)
			{}
		});
		//Start off the progress bar at a brush size of 4
		brush_size_slider.setProgress((int) 4 * brush_size_slider.getMax() / 32);
	}
}
