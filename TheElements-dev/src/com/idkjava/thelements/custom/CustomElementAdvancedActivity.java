package com.idkjava.thelements.custom;

import com.idkjava.thelements.MainActivity;
import com.idkjava.thelements.R;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;

import java.util.ArrayList;

public class CustomElementAdvancedActivity extends Activity
{
	private CustomElement mCustomElement;
	private boolean newElement;
	//private Button saveButton;
	private ArrayList<Spinner> spinnerList;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.custom_element_advanced_activity);
		//saveButton = (Button) findViewById(R.id.ce_advanced_save_button);
		
	    // Load data from the parent activity
		mCustomElement = ((CustomElementActivity) getParent()).mCustomElement;
		newElement = ((CustomElementActivity) getParent()).newElement;
		
		// Create an arraylist to save the spinners we make
		spinnerList = new ArrayList<Spinner>();
		
		
		// Collisions list
		String[] elementsList = getResources().getStringArray(R.array.elements_list);
		LinearLayout layout = (LinearLayout) findViewById(R.id.ce_collisions_layout);
		TextView label;
		Spinner spinner;
		ArrayAdapter<CharSequence> collisions = ArrayAdapter.createFromResource(this,
											R.array.collisions_list,
											android.R.layout.simple_spinner_item);
		collisions.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		
		for (int i = 0; i < elementsList.length; i++)
		{
			String element = elementsList[i];
			label = new TextView(this);
			label.setText(element);
			spinner = new Spinner(this);
			spinner.setAdapter(collisions);
			if (!newElement)
			{
				if (mCustomElement.collisions.size() >= i+1)
				{
					spinner.setSelection(mCustomElement.collisions.get(i));
				}
			}
			spinnerList.add(spinner);
			layout.addView(label);
			layout.addView(spinner);
		}
		
		// Specials list -- Implement in the future
		/*
		int maxSpecials = MainActivity.getMaxSpecials();
		ArrayAdapter<CharSequence> specials = ArrayAdapter.createFromResource(this,
											R.array.specials_list,
											android.R.layout.simple_spinner_item);
		specials.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		layout = (LinearLayout) findViewById(R.id.ce_specials_layout);
		LinearLayout container;
		SeekBar specialValBar;
		for (int i = 0; i < maxSpecials; i++)
		{
			container = new LinearLayout(this);
			container.setOrientation(LinearLayout.VERTICAL);
			spinner = new Spinner(this);
			spinner.setAdapter(specials);
			specialValBar = new SeekBar(this);
			specialValBar.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
			spinner.setOnItemSelectedListener(new OnItemSelectedListener()
			{
				@Override
				public void onItemSelected(AdapterView<?> parent, View v, int pos, long id)
				{
					if (pos == 0)
					{
						((LinearLayout)parent.getParent()).getChildAt(1).setVisibility(View.GONE);
					}
					else
					{
						((LinearLayout)parent.getParent()).getChildAt(1).setVisibility(View.VISIBLE);
						// TODO: Conditionally set the progress bar max
					}
				}
				@Override
				public void onNothingSelected(AdapterView<?> parent)
				{
					((LinearLayout)parent.getParent()).getChildAt(1).setVisibility(View.GONE);
				}
			});
			container.addView(spinner);
			container.addView(specialValBar);
			layout.addView(container);
		}
		*/
		
		/* The save button is too complicated for now -- finish this up later
		saveButton.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				for (int i = 0; i < spinnerList.size(); i++)
				{
					Spinner s = spinnerList.get(i);
					int collision = s.getSelectedItemPosition();
					
					mCustomElement = 
				}
			}
		});
		*/
	}
	
	@Override
	protected void onPause()
	{
		super.onPause();
		
		// Write the collisions to the parent activity
		((CustomElementActivity) getParent()).collisions = new ArrayList<Integer>();
		ArrayList<Integer> collisions = ((CustomElementActivity) getParent()).collisions;
		int numSpinners = spinnerList.size();
		for (int i = 0; i < numSpinners; i++)
		{
			collisions.add(spinnerList.get(i).getSelectedItemPosition());
		}
	}
}
