package com.idkjava.thelements.custom;

import com.idkjava.thelements.R;
import android.app.Activity;
import android.app.ProgressDialog;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.Spinner;

public class CustomElementBasicActivity extends Activity
{
	private CustomElement mCustomElement;
	
	private EditText nameField;
	private Spinner baseElementField;
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
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.custom_element_basic_activity);
		
		// Select all the fields
		nameField = (EditText) findViewById(R.id.ce_name);
		baseElementField = (Spinner) findViewById(R.id.ce_base_element);
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
		
		mCustomElement = ((CustomElementActivity) getParent()).mCustomElement;
		fillInfo();
		
		inertiaUnmovableField.setOnCheckedChangeListener(new OnCheckedChangeListener()
		{
			@Override
			public void onCheckedChanged(CompoundButton b, boolean checked)
			{
				if (checked)
				{
					inertiaNormalField.setVisibility(View.INVISIBLE);
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
				ProgressDialog dialog = new ProgressDialog(CustomElementBasicActivity.this);
				dialog.show();
				CustomElementBasicActivity.this.saveElement();
				dialog.dismiss();
				finish();
			}
		});
	}
	
	private void saveElement()
	{
		// TODO: Open file and save element into it
	}
	
	private void fillInfo()
	{
		CustomElement ce = mCustomElement;
		nameField.setText(ce.name);
		//baseElementField.(ce.baseElementIndex);
		startingTempField.setProgress(ce.startingTemp);
		lowestTempField.setProgress(ce.lowestTemp);
		highestTempField.setProgress(ce.highestTemp);
		//lowerElementField
		//higherElementField
		redField.setProgress(ce.red);
		greenField.setProgress(ce.green);
		blueField.setProgress(ce.blue);
		densityField.setProgress(ce.density);
		fallvelField.setProgress(ce.fallVel);
		if (ce.inertia == 255)
		{
			inertiaUnmovableField.setChecked(true);
			inertiaNormalField.setVisibility(View.INVISIBLE);
		}
		else
		{
			inertiaUnmovableField.setChecked(false);
			inertiaNormalField.setVisibility(View.VISIBLE);
			inertiaNormalField.setProgress(ce.inertia);
		}
	}
}
