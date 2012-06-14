package com.idkjava.thelements.custom;

import com.idkjava.thelements.R;
import com.idkjava.thelements.game.FileManager;

import java.io.File;
import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.content.res.Resources;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

/**
 * File format:
 * NAME\n
 * COLLISION ARRAY (space separated)\n
 * SPECIALS ARRAY (space separated)
 */
public class CustomElementManagerActivity extends Activity
{
	private static ImageButton actionButton;
	private static TableRow tr;
	private static LinearLayout buttonContainer;
	private static Resources res;
	
	
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.custom_element_manager_activity);
		res = getResources();
	}
	@Override
	protected void onResume()
	{
		super.onResume();
		CustomElementManager.refresh();
		ArrayList<CustomElement> elements = CustomElementManager.getElementList();
		
		//Go through and find all the save files and dynamically add them
		int length = elements.size();
		if(length != 0)
		{
			for(int i = 0; i < length; i++)
			{
				if (elements.get(i).isValid())
				{
					addEntity(elements.get(i).getName(), elements.get(i).getFilename());
				}
			}
		}
		else
		{
			TableLayout tl = (TableLayout)findViewById(R.id.loads_container);
			tr = new TableRow(this);
			tr.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
			tr.setGravity(Gravity.CENTER);
			
			TextView tv = new TextView(this);
			tv.setText(res.getText(R.string.no_elements));
			
			tr.addView(tv);
			tl.addView(tr);
		}
	}
	
    public void addEntity(String elementName, String filename)
    {
    	final String filenameFinal = filename;
        
        //Create a TableLayout object associated with the TableLayout in the .xml file
        TableLayout tl = (TableLayout)findViewById(R.id.loads_container);
        //Create a LinearLayout to contain our row
        buttonContainer = new LinearLayout(this);
		//Set some properties
		//tr.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT));
		buttonContainer.setBackgroundResource(R.drawable.load_state_tr_bg);
		buttonContainer.setLongClickable(true);
		buttonContainer.setClickable(true);
		buttonContainer.setPadding(25, 10, 25, 10);
		buttonContainer.setGravity(Gravity.RIGHT);
		
		//Create a TextView to hold the element name
		TextView nameTextView = new TextView(this);
		// TODO: Change this to elementName when loading works
		nameTextView.setText(filename);
		nameTextView.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, 1));
		//filename.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT));
		
		//Makes use of DisplayMetrics to create an automatic dip unit :)
		//filename.setPadding((int)(16 * SCALE + 0.5f), 0, 0, 0);
		//Add the TextView to the TableRow
		buttonContainer.addView(nameTextView);

		//Set buttonContainer's gravity to right with a 16dip right padding
		//buttonContainer.setPadding(0, 0, (int)(16 * SCALE + 0.5f), 0);
		//buttonContainer.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, 50));
		
        // Edit element button
        actionButton = new ImageButton(this);
        actionButton.setBackgroundResource(R.drawable.load_state_select);
        actionButton.setLayoutParams(new LayoutParams(
                        LayoutParams.WRAP_CONTENT, 
                        LayoutParams.WRAP_CONTENT));
        actionButton.setOnClickListener
        (
    		new OnClickListener()
    		{
    			public void onClick(View v)
    			{
    				Intent i = new Intent(CustomElementManagerActivity.this, CustomElementActivity.class);
    				i.putExtra("filename", filenameFinal);
    				startActivity(i);
                	finish();
    			}
    		}
        );
        buttonContainer.addView(actionButton);
        
        // Delete button
        actionButton = new ImageButton(this);
        actionButton.setBackgroundResource(R.drawable.load_state_delete);
        actionButton.setLayoutParams(new LayoutParams(
                LayoutParams.WRAP_CONTENT, 
                LayoutParams.WRAP_CONTENT));
		actionButton.setOnClickListener
		(
			new OnClickListener()
			{
				public void onClick(View v)
				{
					// Delete the custom element file
					(new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR + filenameFinal)).delete();
					
					// Update the view accordingly
					ViewGroup parent = (ViewGroup) v.getParent().getParent();
					parent.removeView((View) v.getParent());
					TableLayout tl = (TableLayout)findViewById(R.id.loads_container);
					if(tl.getChildCount() == 0)
					{
						tr = new TableRow(getBaseContext());
						tr.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
						tr.setGravity(Gravity.CENTER);
						
						TextView tv = new TextView(getBaseContext());
						tv.setText(res.getText(R.string.no_elements));
						
						tr.addView(tv);
						tl.addView(tr);
					}
				}
			}
		);
		buttonContainer.addView(actionButton);
        
        // Click on the entire row
        buttonContainer.setOnClickListener
        (
        	new OnClickListener()
	        {
                public void onClick(View viewParam)
                {
    				Intent i = new Intent(CustomElementManagerActivity.this, CustomElementActivity.class);
    				i.putExtra("filename", filenameFinal);
    				startActivity(i);
                	finish();
                }
	        }
        );
        
        // Add the created row to our TableLayout
        tl.addView(buttonContainer, new LayoutParams(
                LayoutParams.FILL_PARENT, 
                LayoutParams.WRAP_CONTENT));
    }
}