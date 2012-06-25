package com.idkjava.thelements.custom;

import java.util.ArrayList;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;

import com.idkjava.thelements.FlurryActivity;
import com.idkjava.thelements.MainActivity;
import com.idkjava.thelements.R;

public class CustomElementAdvancedActivity extends FlurryActivity
{
	private CustomElementActivity mParent;
	private CustomElement mCustomElement;
	private boolean newElement;
	//private Button saveButton;
	private ArrayList<Spinner> collisionsSpinnerList;
	private String[] elementsList;
	private ArrayList<Spinner> specialsSpinnerList;
	private ArrayList<LinearLayout> specialsLayoutList;
	
	private int numListenerSkips;
	
	private int maxSpecials;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.custom_element_advanced_activity);
		//saveButton = (Button) findViewById(R.id.ce_advanced_save_button);
		
		// Save a pointer to the parent activity
		mParent = ((CustomElementActivity) getParent());
		
	    // Load data from the parent activity
		mCustomElement = mParent.mCustomElement;
		newElement = mParent.newElement;
		
		// Create an arraylist to save the spinners we make
		collisionsSpinnerList = new ArrayList<Spinner>();
		specialsSpinnerList = new ArrayList<Spinner>();
		specialsLayoutList = new ArrayList<LinearLayout>();
		
		// Collisions list
		elementsList = getResources().getStringArray(R.array.elements_list);
		LinearLayout collisionsLayout = (LinearLayout) findViewById(R.id.ce_collisions_layout);
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
			collisionsSpinnerList.add(spinner);
			collisionsLayout.addView(label);
			collisionsLayout.addView(spinner);
		}
		
		// Specials list
		maxSpecials = MainActivity.MAX_SPECIALS;
		ArrayAdapter<CharSequence> specials = ArrayAdapter.createFromResource(this,
											R.array.specials_list,
											android.R.layout.simple_spinner_item);
		specials.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		LinearLayout specialsLayout = (LinearLayout) findViewById(R.id.ce_specials_layout);
		LinearLayout container;
		// We want to skip maxSpecials listener calls for initialization
		numListenerSkips = maxSpecials;
		OnItemSelectedListener spinnerListener = new OnItemSelectedListener()
		{
			@Override
			public void onItemSelected(AdapterView<?> parent, View v, int pos, long id)
			{
				// This is a total hack
				if (numListenerSkips > 0)
				{
					numListenerSkips--;
					return;
				}
				Log.d("LOG", "Special selected: " + pos);
				if (pos == 0)
				{
					// Remove the existing view
					View child = ((LinearLayout)parent.getParent()).getChildAt(1);
					if (child != null)
					{
						((LinearLayout)parent.getParent()).removeView(child);
					}
				}
				else
				{
					// Remove the existing view
					View child = ((LinearLayout)parent.getParent()).getChildAt(1);
					if (child != null)
					{
						((LinearLayout)parent.getParent()).removeView(child);
					}
					
					// Add a new view based on the special val
					child = getSpecialValView(pos);
					if (child != null)
					{
						LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
						lp.setMargins(30, 0, 30, 10);
						child.setLayoutParams(lp);
						((LinearLayout)parent.getParent()).addView(child);
					}
				}
			}
			@Override
			public void onNothingSelected(AdapterView<?> parent)
			{
				// Remove the existing view
				View child = ((LinearLayout)parent.getParent()).getChildAt(1);
				if (child != null)
				{
					((LinearLayout)parent.getParent()).removeView(child);
				}
			}
		};
		for (int i = 0; i < maxSpecials; i++)
		{
			container = new LinearLayout(this);
			container.setOrientation(LinearLayout.VERTICAL);
			spinner = new Spinner(this);
			spinner.setAdapter(specials);
			spinner.setOnItemSelectedListener(spinnerListener);
			container.addView(spinner);
			specialsLayout.addView(container);
			specialsLayoutList.add(container);
			specialsSpinnerList.add(spinner);
		}
		
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
	protected void onResume()
	{
		super.onResume();
		
		// Initialize the collision values
		for (int i = 0; i < elementsList.length; i++)
		{
			if (mParent.collisions.size() >= i+1)
			{
				Log.d("LOG", "Initializing collision from parent saved values: " + mParent.collisions.get(i));
				collisionsSpinnerList.get(i).setSelection(mParent.collisions.get(i));
			}
		}
		// Initialize the special values
		for (int i = 0; i < maxSpecials; i++)
		{
			if (mParent.specials.size() >= i+1)
			{
				Log.d("LOG", "Initializing special from parent saved values: " + mParent.specials.get(i) + ", " + mParent.specialVals.get(i));
				specialsSpinnerList.get(i).setSelection(CustomElement.getSpecialPosFromIndex(mParent.specials.get(i)));
				setSpecialVal(CustomElement.getSpecialPosFromIndex(mParent.specials.get(i)), specialsLayoutList.get(i), mParent.specialVals.get(i));
			}
		}
	}
	
	@Override
	protected void onPause()
	{
		super.onPause();
		
		// Write the collisions to the parent activity
		mParent.collisions = new ArrayList<Integer>();
		ArrayList<Integer> collisions = mParent.collisions;
		int numSpinners = collisionsSpinnerList.size();
		for (int i = 0; i < numSpinners; i++)
		{
			collisions.add(collisionsSpinnerList.get(i).getSelectedItemPosition());
		}
		
		// Write the specials to parent activity
		ArrayList<Integer> specials = new ArrayList<Integer>();
		ArrayList<Integer> specialVals = new ArrayList<Integer>();
		mParent.specials = specials;
		mParent.specialVals = specialVals;
		for (int i = 0; i < maxSpecials; i++)
		{			LinearLayout specialContainer = specialsLayoutList.get(i);
			int special = ((Spinner) specialContainer.getChildAt(0)).getSelectedItemPosition();
			int specialVal = getSpecialVal(special, specialContainer);
			specials.add(special);
			specialVals.add(specialVal);
		}
	}
	
	private View getSpecialValView(int special)
	{
		switch(special)
		{
		// No special
		case 0:
		{
			// No view
			return null;
		}
		// Spawn
		case 1:
		{
			// Element spinner
			Spinner spinner = new Spinner(this);
			ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this, R.array.elements_list, android.R.layout.simple_spinner_item);
			adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
			spinner.setAdapter(adapter);
			return spinner;
		}
		// Break
		case 2:
		{
			// SeekBar
			SeekBar seekbar = new SeekBar(this);
			seekbar.setMax(30);
			return seekbar;
		}
		// Grow
		case 3:
		{
			// Element spinner
			Spinner spinner = new Spinner(this);
			ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this, R.array.elements_list, android.R.layout.simple_spinner_item);
			adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
			spinner.setAdapter(adapter);
			return spinner;
		}
		// Heat
		case 4:
		{
			// TextView that says no value
			TextView tv = new TextView(this);
			tv.setText(getResources().getString(R.string.no_special_val));
			return tv;
		}
		// Explode
		case 5:
		{
			// SeekBar
			SeekBar seekbar = new SeekBar(this);
			seekbar.setMax(30);
			return seekbar;
		}
		// Life
		case 6:
		{
			// SeekBar
			SeekBar seekbar = new SeekBar(this);
			seekbar.setMax(100);
			return seekbar;
		}
		// Hmm, something went wrong...
		default:
		{
			Log.d("LOG", "Unrecognized special selected");
			return null;
		}
		}
	}
	
	private int getSpecialVal(int special, LinearLayout container)
	{
		switch(special)
		{
		// No special
		case 0:
		{
			// No special val
			return 0;
		}
		// Spawn
		case 1:
		{
			// The view is a Spinner
			return ((Spinner) container.getChildAt(1)).getSelectedItemPosition() + MainActivity.NORMAL_ELEMENT;
		}
		// Break
		case 2:
		{
			// The view is a SeekBar
			return ((SeekBar) container.getChildAt(1)).getProgress();
		}
		// Grow
		case 3:
		{
			// The view is a Spinner
			return ((Spinner) container.getChildAt(1)).getSelectedItemPosition() + MainActivity.NORMAL_ELEMENT;
		}
		// Heat
		case 4:
		{
			// No special val
			return 0;
		}
		// Explode
		case 5:
		{
			// The view is a SeekBar
			return ((SeekBar) container.getChildAt(1)).getProgress();
		}
		// Life
		case 6:
		{
			// The view is a SeekBar
			return ((SeekBar) container.getChildAt(1)).getProgress();
		}
		// Hmm, something went wrong...
		default:
		{
			Log.d("LOG", "Unrecognized special selected");
			return 0;
		}
		}
	}
	private void setSpecialVal(int special, LinearLayout container, int val)
	{
		// Assumes the container doesn't contain any child yet
		
		switch(special)
		{
		// No special
		case 0:
		{
			// No special val
			return;
		}
		// Spawn
		case 1:
		{
			// The view is a Spinner
			clearContainerChild(container);
			Spinner child = (Spinner) getSpecialValView(special);
			if (child == null)
			{
				Log.d("LOG", "Error: No child existed");
				return;
			}
			LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
			lp.setMargins(30, 0, 30, 10);
			child.setLayoutParams(lp);
			container.addView(child);
			
			child.setSelection(val - MainActivity.NORMAL_ELEMENT);
			return;
		}
		// Break
		case 2:
		{
			// The view is a SeekBar
			clearContainerChild(container);
			SeekBar child = (SeekBar) getSpecialValView(special);
			if (child == null)
			{
				Log.d("LOG", "Error: No child existed");
				return;
			}
			LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
			lp.setMargins(30, 0, 30, 10);
			child.setLayoutParams(lp);
			container.addView(child);
			
			child.setProgress(val);
			return;
		}
		// Grow
		case 3:
		{
			// The view is a Spinner
			clearContainerChild(container);
			Spinner child = (Spinner) getSpecialValView(special);
			if (child == null)
			{
				Log.d("LOG", "Error: No child existed");
				return;
			}
			LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
			lp.setMargins(30, 0, 30, 10);
			child.setLayoutParams(lp);
			container.addView(child);

			child.setSelection(val - MainActivity.NORMAL_ELEMENT);
			return;
		}
		// Heat
		case 4:
		{
			clearContainerChild(container);
			TextView child = (TextView) getSpecialValView(special);
			if (child == null)
			{
				Log.d("LOG", "Error: No child existed");
				return;
			}
			LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
			lp.setMargins(30, 0, 30, 10);
			child.setLayoutParams(lp);
			container.addView(child);

			// No special val
			return;
		}
		// Explode
		case 5:
		{
			// The view is a SeekBar
			clearContainerChild(container);
			SeekBar child = (SeekBar) getSpecialValView(special);
			if (child == null)
			{
				Log.d("LOG", "Error: No child existed");
				return;
			}
			LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
			lp.setMargins(30, 0, 30, 10);
			child.setLayoutParams(lp);
			container.addView(child);
			

			child.setProgress(val);
			return;
		}
		// Life
		case 6:
		{
			// The view is a SeekBar
			clearContainerChild(container);
			SeekBar child = (SeekBar) getSpecialValView(special);
			if (child == null)
			{
				Log.d("LOG", "Error: No child existed");
				return;
			}
			LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
			lp.setMargins(30, 0, 30, 10);
			child.setLayoutParams(lp);
			container.addView(child);
			

			child.setProgress(val);
			return;
		}
		// Hmm, something went wrong...
		default:
		{
			Log.d("LOG", "Unrecognized special selected");
			return;
		}
		}
	}
	private void clearContainerChild(LinearLayout container)
	{
		View child = container.getChildAt(1);
		if (child != null)
		{
			container.removeView(child);
		}
	}
}
