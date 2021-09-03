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

import androidx.fragment.app.FragmentActivity;

import com.idkjava.thelements.MainActivity;
import com.idkjava.thelements.R;

public class CustomElementAdvancedActivity extends FragmentActivity
{
	private CustomElementActivity mParent;
	//private Button saveButton;
	private ArrayList<Spinner> collisionsSpinnerList;
	private String[] elementsList;
	private ArrayList<Spinner> specialsSpinnerList;
	private ArrayList<LinearLayout> specialsLayoutList;
		
	private int maxSpecials;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.custom_element_advanced_activity);
		//saveButton = (Button) findViewById(R.id.ce_advanced_save_button);
		
		// Save a pointer to the parent activity
		mParent = ((CustomElementActivity) getParent());
		
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
											R.layout.multiline_spinner_dropdown_item);
		
		for (int i = 0; i < elementsList.length; i++)
		{
			String element = elementsList[i];
			label = new TextView(this);
			label.setText(element);
			spinner = new Spinner(this, Spinner.MODE_DIALOG);
			spinner.setAdapter(collisions);
			collisionsSpinnerList.add(spinner);
			collisionsLayout.addView(label);
			collisionsLayout.addView(spinner);
		}
		
		// Specials list
		maxSpecials = MainActivity.MAX_SPECIALS;
		ArrayAdapter<CharSequence> specials = ArrayAdapter.createFromResource(this,
											R.array.specials_list,
											R.layout.multiline_spinner_dropdown_item);
		LinearLayout specialsLayout = (LinearLayout) findViewById(R.id.ce_specials_layout);
		LinearLayout container;
		// We want to skip maxSpecials listener calls for initialization
		//numListenerSkips = maxSpecials;
		OnItemSelectedListener spinnerListener = new OnItemSelectedListener()
		{
			@Override
			public void onItemSelected(AdapterView<?> parent, View v, int pos, long id)
			{
				// This is a total hack
				/*
				if (numListenerSkips > 0)
				{
					numListenerSkips--;
					return;
				}
				*/
				Log.d("LOG", "Special selected: " + pos);
				// Get the child LinearLayout
				LinearLayout child = (LinearLayout) ((LinearLayout)parent.getParent()).getChildAt(1);
				// Special case for pos == 0 -- hide the LinearLayout
				if (pos == 0)
				{
					if (child != null)
					{
						child.setVisibility(View.GONE);
					}
					
					return;
				}
				// Pos != 0, do normal set up and display
				if (child == null)
				{
					child = new LinearLayout(CustomElementAdvancedActivity.this);
					child.setOrientation(LinearLayout.VERTICAL);
					LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
					lp.setMargins(30, 0, 30, 10);
					child.setLayoutParams(lp);
					((LinearLayout)parent.getParent()).addView(child);
					
					TextView tv = new TextView(CustomElementAdvancedActivity.this);
					tv.setText(getSpecialValText(pos));
					View valView = getSpecialValView(pos);
					child.addView(tv);
					if (valView != null)
					{
						child.addView(valView);
					}
				}
				
				// Get the children of the child view, set them to the appropriate values
				TextView textView = null;
				View specialValView = null;
				try
				{
					textView = (TextView) child.getChildAt(0);
					if (textView == null)
					{
						textView = new TextView(CustomElementAdvancedActivity.this);
						child.addView(textView);
					}
					textView.setText(getSpecialValText(pos));
					specialValView = child.getChildAt(1);
					if (specialValView != null)
					{
						child.removeViewAt(1);
					}
					specialValView = getSpecialValView(pos);
					if (specialValView != null)
					{
						child.addView(specialValView);
					}
				}
				catch (ClassCastException e)
				{
					Log.d("LOG", "The subview for a special is messed up");
					child.removeAllViews();
					textView = new TextView(CustomElementAdvancedActivity.this);
					child.addView(textView);
					textView.setText(getSpecialValText(pos));
					specialValView = getSpecialValView(pos);
					if (specialValView != null)
					{
						child.addView(specialValView);
					}
				}

				// If we're selecting the special initially, we already have a special val that we can initialize to
				int i = ((LinearLayout)parent.getParent().getParent()).indexOfChild((View) parent.getParent());
				Log.d("LOG", "Checking special val: " + pos + ", " + i);
				if (pos != 0 && i < mParent.specials.size() && pos == CustomElementManager.getSpecialPosFromIndex(mParent.specials.get(i)))
				{
					Log.d("LOG", "..." + CustomElementManager.getSpecialPosFromIndex(mParent.specials.get(i)));
					Log.d("LOG", "Initializing special val: " + pos);
					setSpecialVal(CustomElementManager.getSpecialPosFromIndex(mParent.specials.get(i)), child, mParent.specialVals.get(i));
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
			spinner = new Spinner(this, Spinner.MODE_DIALOG);
			spinner.setAdapter(specials);
			spinner.setOnItemSelectedListener(spinnerListener);
			container.addView(spinner);
			specialsLayout.addView(container);
			specialsLayoutList.add(container);
			specialsSpinnerList.add(spinner);
		}
	}
	
	@Override
	protected void onResume()
	{
		super.onResume();
		
		// Initialize the collision values
		for (int i = 0; i < elementsList.length; i++)
		{
			if (mParent.collisions != null && i < mParent.collisions.size())
			{
				Log.d("LOG", "Initializing collision from parent saved values: " + mParent.collisions.get(i));
				collisionsSpinnerList.get(i).setSelection(mParent.collisions.get(i));
			}
		}
		// Initialize the special values
		for (int i = 0; i < maxSpecials; i++)
		{
			if (mParent.specials != null && i < mParent.specials.size())
			{
				Log.d("LOG", "Initializing special from parent saved values: " + mParent.specials.get(i) + ", " + mParent.specialVals.get(i));
				specialsSpinnerList.get(i).setSelection(CustomElementManager.getSpecialPosFromIndex(mParent.specials.get(i)));
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
		{
			LinearLayout specialContainer = specialsLayoutList.get(i);
			int special = ((Spinner) specialContainer.getChildAt(0)).getSelectedItemPosition();
			int specialVal = getSpecialVal(special, (LinearLayout) specialContainer.getChildAt(1));
			specials.add(special);
			specialVals.add(specialVal);
		}
	}
	
	// Get the view associated with setting the value for a particular special
	private View getSpecialValView(int special)
	{
		switch(special)
		{
		// No special, Conductive, Conductable
		case 0:
		case 11:
		case 12:
		{
			// No view
			return null;
		}
		// Break
		case 2:
		{
			// SeekBar
			SeekBar seekbar = new SeekBar(this);
			seekbar.setMax(30);
			return seekbar;
		}
		// Spawn, Grow, Tunnel
		case 1:
		case 3:
		case 9:
		{
			// Element spinner
			Spinner spinner = new Spinner(this, Spinner.MODE_DIALOG);
			ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
					this, R.array.elements_list, R.layout.multiline_spinner_dropdown_item);
			spinner.setAdapter(adapter);
			return spinner;
		}
		// Explode, Trail
		case 5:
		case 13:
		{
			// SeekBar
			SeekBar seekbar = new SeekBar(this);
			seekbar.setMax(30);
			return seekbar;
		}
		// Life, Wander, Jump, Heat, Burn -- seekbar probability
		case 4:
		case 6:
		case 7:
		case 8:
		case 10:
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
	// Get the description text for the special
	private String getSpecialValText(int special)
	{
		return getResources().getStringArray(R.array.specials_description_list)[special];
	}
	
	// Get the special val based on the special
	private int getSpecialVal(int special, LinearLayout container)
	{
		switch(special)
		{
		// No special, Conductive, Conductable
		case 0:
		case 11:
		case 12:
		{
			// No special val
			return -1;
		}
		// Spawn, Grow, Tunnel
		case 1:
		case 3:
		case 9:
		{
			// The view is a Spinner
			Spinner child = (Spinner) container.getChildAt(1);
			if (child != null)
			{
				return child.getSelectedItemPosition() + MainActivity.NORMAL_ELEMENT;
			}
			else
			{
				// Use sand as the default, for lack of a better choice
				Log.e("TheElements", "Spinner not defined in special");
				return MainActivity.NORMAL_ELEMENT;
			}
		}
		// Break, Explode, Life, Wander, Jump, Burn, Heat, Trail
		case 2:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 10:
		case 13:
		{
			// The view is a SeekBar
			SeekBar child = (SeekBar) container.getChildAt(1);
			if (child != null)
			{
				return child.getProgress();
			}
			else
			{
				// Default to 0, for lack of a better choice.
				Log.e("TheElements", "SeekBar not defined in special");
				return 0;
			}
		}
		// Hmm, something went wrong...
		default:
		{
			Log.d("LOG", "Unrecognized special selected");
			return -1;
		}
		}
	}
	// Set the special val in the container based on the special
	private void setSpecialVal(int special, LinearLayout container, int val)
	{
		switch(special)
		{
		// No special, Conductive, Conductable
		case 0:
		case 11:
		case 12:
		{
			// No special val
			return;
		}
		// Spawn, Grow, Tunnel
		case 1:
		case 3:
		case 9:
		{
			// The view is a Spinner
			Spinner child = (Spinner) container.getChildAt(1);
			if (child == null)
			{
				Log.e("LOG", "Error: no child existed");
				return;
			}
			child.setSelection(val - MainActivity.NORMAL_ELEMENT);
			return;
		}

		// Break, Explode, Life, Wander, Jump, Burn, Heat, Trail
		case 2:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 10:
		case 13:
		{
			SeekBar child = (SeekBar) container.getChildAt(1);
			if (child == null)
			{
				Log.e("LOG", "Error: No child existed");
				return;
			}
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
}
