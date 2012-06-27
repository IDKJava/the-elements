package com.idkjava.thelements.custom;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;

import android.app.ProgressDialog;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.Toast;

import com.idkjava.thelements.FlurryActivity;
import com.idkjava.thelements.MainActivity;
import com.idkjava.thelements.R;

public class CustomElementBasicActivity extends FlurryActivity
{
	private CustomElement mCustomElement;
	private boolean newElement;
	private boolean shouldIgnoreSelection;
	private CustomElementActivity mParent;
	
	private EditText nameField;
	private Spinner baseElementField;
	private RadioGroup stateField;
	private SeekBar startingTempField;
	private SeekBar lowestTempField;
	private SeekBar highestTempField;
	private Spinner lowerElementField;
	private Spinner higherElementField;
	private SeekBar redField;
	private SeekBar greenField;
	private SeekBar blueField;
	private SeekBar densityField;
	private SeekBar fallvelField;
	private CheckBox inertiaUnmovableField;
	private SeekBar inertiaNormalField;
	private Button saveButton;
	
	private ArrayAdapter<CharSequence> elementAdapter;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.custom_element_basic_activity);
		
		// Set the parent activity
		mParent = ((CustomElementActivity) getParent());
		
		// Select all the fields
		nameField = (EditText) findViewById(R.id.ce_name);
		baseElementField = (Spinner) findViewById(R.id.ce_base_element);
		stateField = (RadioGroup) findViewById(R.id.ce_state);
		startingTempField = (SeekBar) findViewById(R.id.ce_starting_temp);
		lowestTempField = (SeekBar) findViewById(R.id.ce_lowest_temp);
		highestTempField = (SeekBar) findViewById(R.id.ce_highest_temp);
		lowerElementField = (Spinner) findViewById(R.id.ce_lower_element);
		higherElementField = (Spinner) findViewById(R.id.ce_higher_element);
		redField = (SeekBar) findViewById(R.id.ce_red);
		greenField = (SeekBar) findViewById(R.id.ce_green);
		blueField = (SeekBar) findViewById(R.id.ce_blue);
		densityField = (SeekBar) findViewById(R.id.ce_density);
		fallvelField = (SeekBar) findViewById(R.id.ce_fallvel);
		inertiaUnmovableField = (CheckBox) findViewById(R.id.ce_inertia_unmovable);
		inertiaNormalField = (SeekBar) findViewById(R.id.ce_inertia_normal);
		saveButton = (Button) findViewById(R.id.ce_save_button);
		
		// Make the elements adapter, and assign it to the appropriate field
	    elementAdapter = ArrayAdapter.createFromResource(this, R.array.elements_list, android.R.layout.simple_spinner_item);
	    elementAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
	    baseElementField.setAdapter(elementAdapter);
	    lowerElementField.setAdapter(elementAdapter);
	    higherElementField.setAdapter(elementAdapter);
		
	    // Load data from the parent activity
		mCustomElement = mParent.mCustomElement;
		newElement = mParent.newElement;
		shouldIgnoreSelection = false;
		if (!newElement)
		{
			shouldIgnoreSelection = true;
			// Fill in the basic view info
			fillInfo();
			// Also save the special and custom data in the parent activity
			mParent.collisions = mCustomElement.collisions;
			mParent.specials = mCustomElement.specials;
			mParent.specialVals = mCustomElement.specialVals;
		}
		
		baseElementField.setOnItemSelectedListener(new OnItemSelectedListener ()
		{
			@Override
			public void onItemSelected(AdapterView<?> parent, View v, int pos, long id)
			{
				// Fill in the values with the default for that element
				if (shouldIgnoreSelection)
				{
					shouldIgnoreSelection = false;
					return;
				}
				fillInfoFromBase(pos);
			}
			@Override
			public void onNothingSelected(AdapterView<?> parent)
			{
				// TODO: Clear the properties
			}
		});
		
		inertiaUnmovableField.setOnCheckedChangeListener(new OnCheckedChangeListener()
		{
			@Override
			public void onCheckedChanged(CompoundButton b, boolean checked)
			{
				if (checked)
				{
					inertiaNormalField.setVisibility(View.GONE);
				}
				else
				{
					inertiaNormalField.setVisibility(View.VISIBLE);
				}
			}
		});
		
		saveButton.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				nameField.setText(nameField.getText().toString().trim());
				Log.d("LOG", "Name field: " + nameField.getText().toString());
				if (nameField.getText().toString().length() <= 0)
				{
					Log.d("LOG", "Emtpy name field!");
					Toast.makeText(getApplicationContext(), R.string.ce_empty_name_error, Toast.LENGTH_LONG).show();
					return;
				}
				ProgressDialog dialog = new ProgressDialog(CustomElementBasicActivity.this);
				dialog.show();
				if(CustomElementBasicActivity.this.saveElement())
				{
					dialog.dismiss();
					finish();
				}
				else
				{
					dialog.dismiss();
				}
			}
		});
	}
	
	private boolean saveElement()
	{
		writePropertiesToCustom();
		
		if (mCustomElement.writeToFile())
		{
			Toast.makeText(getApplicationContext(), R.string.ce_save_success, Toast.LENGTH_LONG).show();
			return true;
		}
		else
		{
			Toast.makeText(getApplicationContext(), R.string.ce_save_failed, Toast.LENGTH_LONG).show();
			return false;
		}
	}
	public void writePropertiesToCustom()
	{
		// If this is a new element, we need to set the filename, otherwise we use the previous one
		if (newElement)
		{
			mCustomElement.setFilename(nameField.getText().toString().toLowerCase());
		}
		
		// Write all the normal properties to the custom element from the fields in basic
		CustomElement ce = mCustomElement;
		ce.name = nameField.getText().toString();
		ce.baseElementIndex = (int) baseElementField.getSelectedItemId() + MainActivity.NORMAL_ELEMENT;
		int stateRadioId = stateField.getCheckedRadioButtonId();
		ce.state = getStateValFromId(stateRadioId);
		ce.startingTemp = startingTempField.getProgress();
		ce.lowestTemp = lowestTempField.getProgress();
		ce.highestTemp = highestTempField.getProgress();
		ce.lowerElementIndex = (int) (lowerElementField.getSelectedItemId() + MainActivity.NORMAL_ELEMENT);
		ce.higherElementIndex = (int) (higherElementField.getSelectedItemId() + MainActivity.NORMAL_ELEMENT);
		ce.red = redField.getProgress();
		ce.green = greenField.getProgress();
		ce.blue = blueField.getProgress();
		ce.density = densityField.getProgress();
		ce.fallVel = fallvelField.getProgress();
		if (inertiaUnmovableField.isChecked())
		{
			ce.inertia = 255;
		}
		else
		{
			ce.inertia = inertiaNormalField.getProgress();
		}
		
		// Get the list of collisions from the parent activity (the method of transferring from Advanced)
		ArrayList<Integer> collisions = mParent.collisions;
		// If collisions is null and it's not a new element, then we didn't change anything, so leave it the same
		if (!(collisions == null && !newElement))
		{
			// If collisions is null, then we need to create an arraylist for it
			if (collisions == null)
			{
				collisions = new ArrayList<Integer>();
			}
			
			// Clear the previous collisions
			ce.collisions = new ArrayList<Integer>();
			// Walk through the collisions we got from Advanced and set it
			// (use 0 as the default if there was an error transferring)
			int numElements = getResources().getStringArray(R.array.elements_list).length;
			for (int i = 0; i < numElements; i++)
			{
				if (collisions.size() >= i+1)
				{
					ce.collisions.add(collisions.get(i));
				}
				else
				{
					Log.d("LOG", "Error: collisions array passed to save was not long enough");
					ce.collisions.add(0);
				}
			}
		}
		ArrayList<Integer> specials = mParent.specials;
		ArrayList<Integer> specialVals = mParent.specialVals;
		// If either is null and this is not a new element, then we don't want to bother writing
		if (!((specials == null || specialVals == null) && !newElement))
		{
			// Create the arraylists if needed
			if (specials == null)
			{
				specials = new ArrayList<Integer>();
			}
			if (specialVals == null)
			{
				specialVals = new ArrayList<Integer>();
			}
			
			// Overwrite the previous arraylists in the custom
			ce.specials = new ArrayList<Integer>();
			ce.specialVals = new ArrayList<Integer>();
			
			// Walk through the specials, and write both the index and val
			// (using sane defaults and logging if issues occur)
			int numSpecials = MainActivity.MAX_SPECIALS;
			for (int i = 0; i < numSpecials; i++)
			{
				if (specials.size() >= i+1)
				{
					ce.specials.add(specials.get(i));
				}
				else
				{
					Log.d("LOG", "Error: specials array passed to save was not long enough");
					ce.specials.add(-1);
				}
				
				if (specialVals.size() >= i+1)
				{
					ce.specialVals.add(specialVals.get(i));
				}
				else
				{
					Log.d("LOG", "Error: special vals array passed to save was not long enough");
					ce.specialVals.add(0);
				}
			}
		}
	}
	
	private void fillInfo()
	{
		CustomElement ce = mCustomElement;
		nameField.setText(ce.name);
		baseElementField.setSelection(ce.baseElementIndex - MainActivity.NORMAL_ELEMENT);
		stateField.check(getStateRadioId(ce.state));
		startingTempField.setProgress(ce.startingTemp);
		lowestTempField.setProgress(ce.lowestTemp);
		highestTempField.setProgress(ce.highestTemp);
		lowerElementField.setSelection(ce.lowerElementIndex - MainActivity.NORMAL_ELEMENT);
		higherElementField.setSelection(ce.higherElementIndex - MainActivity.NORMAL_ELEMENT);
		redField.setProgress(ce.red);
		greenField.setProgress(ce.green);
		blueField.setProgress(ce.blue);
		densityField.setProgress(ce.density);
		fallvelField.setProgress(ce.fallVel);
		if (ce.inertia == 255)
		{
			inertiaUnmovableField.setChecked(true);
			inertiaNormalField.setVisibility(View.GONE);
		}
		else
		{
			inertiaUnmovableField.setChecked(false);
			inertiaNormalField.setVisibility(View.VISIBLE);
			inertiaNormalField.setProgress(ce.inertia);
		}
	}
	private void fillInfoFromBase(int pos)
	{
		int elementIndex = pos + MainActivity.NORMAL_ELEMENT;
		String elementInfo = MainActivity.getElementInfo(elementIndex);
		Log.d("LOG", "Element info: " + elementInfo);
		BufferedReader reader = new BufferedReader(new StringReader(elementInfo));
		
		try
		{
			// Now fill in the actual values
			//nameField.setText(reader.readLine());
			// Ignore the name
			reader.readLine();
			int stateVal = Integer.parseInt(reader.readLine());
			stateField.check(getStateRadioId(stateVal));
			startingTempField.setProgress(Integer.parseInt(reader.readLine()));
			lowestTempField.setProgress(Integer.parseInt(reader.readLine()));
			highestTempField.setProgress(Integer.parseInt(reader.readLine()));
			lowerElementField.setSelection(Integer.parseInt(reader.readLine()) - MainActivity.NORMAL_ELEMENT);
			higherElementField.setSelection(Integer.parseInt(reader.readLine()) - MainActivity.NORMAL_ELEMENT);
			redField.setProgress(Integer.parseInt(reader.readLine()));
			greenField.setProgress(Integer.parseInt(reader.readLine()));
			blueField.setProgress(Integer.parseInt(reader.readLine()));
			densityField.setProgress(Integer.parseInt(reader.readLine()));
			fallvelField.setProgress(Integer.parseInt(reader.readLine()));
			int inertia = Integer.parseInt(reader.readLine());
			if (inertia == 255)
			{
				inertiaUnmovableField.setChecked(true);
				inertiaNormalField.setVisibility(View.GONE);
			}
			else
			{
				inertiaUnmovableField.setChecked(false);
				inertiaNormalField.setVisibility(View.VISIBLE);
				inertiaNormalField.setProgress(inertia);
			}
			
			// Save collisions and specials for the advanced activity
			mParent.collisions = new ArrayList<Integer>();
			String line;
			for (int i = 0; i < MainActivity.NUM_BASE_ELEMENTS-MainActivity.NORMAL_ELEMENT; i++)
			{
				line = reader.readLine();
				if (line == null)
				{
					return;
				}
				
				mParent.collisions.add(Integer.parseInt(line));
			}
			mParent.specials = new ArrayList<Integer>();
			mParent.specialVals = new ArrayList<Integer>();
			String[] tempVals;
			for (int i = 0; i < MainActivity.MAX_SPECIALS; i++)
			{
				line = reader.readLine();
				if (line == null)
				{
					return;
				}
				
				tempVals = line.split("\\s+", 2);
				if (tempVals.length < 2)
				{
					return;
				}
				mParent.specials.add(Integer.parseInt(tempVals[0]));
				mParent.specialVals.add(Integer.parseInt(tempVals[1]));
			}
		}
		catch (IOException e)
		{
			// TODO: Maybe do something here?
			e.printStackTrace();
		}
	}
	
	private int getStateRadioId(int state)
	{
		switch(state)
		{
		case 0:
		{
			return R.id.ce_solid_state;
		}
		case 1:
		{
			return R.id.ce_liquid_state;
		}
		case 2:
		{
			return R.id.ce_gas_state;
		}
		default:
		{
			return 0;
		}
		}
	}
	private int getStateValFromId(int id)
	{
		switch(id)
		{
		case R.id.ce_solid_state:
		{
			return 0;
		}
		case R.id.ce_liquid_state:
		{
			return 1;
		}
		case R.id.ce_gas_state:
		{
			return 2;
		}
		default:
		{
			return 0;
		}
		}
	}
}
