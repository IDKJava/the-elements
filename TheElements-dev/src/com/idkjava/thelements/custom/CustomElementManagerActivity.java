package com.idkjava.thelements.custom;

import com.idkjava.thelements.R;
import com.idkjava.thelements.game.SaveManager;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
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
	List<String> elementList = new ArrayList<String>();
	CustomElementArrayAdapter arrayAdapter;
	
	
	//TODO(MASSIVE,ARMORED): This should display all loaded Custom Elements and allow selection,
	//editing, and creation
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.custom_element_manager_activity);
	}
	@Override
	protected void onResume()
	{
		super.onResume();
		refreshElements();
		
		//Go through and find all the save files and dynamically load them
		int length = elementList.size();
		if(length != 0)
		{
			for(int i = 0; i < length; i++)
			{
				addEntity(elementList.get(i));
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
	private void refreshElements()
	{
		elementList.clear();
		File file = new File("/sdcard/thelements/elements/");
		if(file.exists())
		{
			File[] elementFiles = file.listFiles();
			for(int i = 0; i < elementFiles.length; i++)
			{
				loadCustomElement(elementFiles[i]);
			}
		}
		else
		{
			file.mkdirs();
		}
		//arrayAdapter.notifyDataSetChanged();
		
		// Temporarily load some fake elements
		elementList.add("Hello");
		elementList.add("Element1");
		elementList.add("Element2");
		
		
	}
	
    public void addEntity(String entityName)
    {
    	final String entityNameFinal = entityName;
        //DIP representation math
        //final float SCALE = getBaseContext().getResources().getDisplayMetrics().density;
        
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
		
		//Create a TextView to hold the filename
		TextView elementname = new TextView(this);
		elementname.setText(entityName);
		elementname.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT, 1));
		//filename.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT));
		
		//Makes use of DisplayMetrics to create an automatic dip unit :)
		//filename.setPadding((int)(16 * SCALE + 0.5f), 0, 0, 0);
		//Add the TextView to the TableRow
		buttonContainer.addView(elementname);

		//Set buttonContainer's gravity to right with a 16dip right padding
		//buttonContainer.setPadding(0, 0, (int)(16 * SCALE + 0.5f), 0);
		//buttonContainer.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, 50));
		
        //Create a button to be the action invoker
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
                	SaveManager.loadState(entityNameFinal);
                	finish();
    			}
    		}
        );
        buttonContainer.addView(actionButton);
        
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
					SaveManager.deleteState(entityNameFinal);
					ViewGroup parent = (ViewGroup) v.getParent().getParent();
					parent.removeView((View) v.getParent());
					SaveManager.refresh();
					if(SaveManager.getNumSaves() == 0)
					{
						TableLayout tl = (TableLayout)findViewById(R.id.loads_container);
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
        
        //Add a test click event
        buttonContainer.setOnClickListener
        (
        	new OnClickListener()
	        {
                public void onClick(View viewParam)
                {
                	SaveManager.loadState(entityNameFinal);
                	finish();
                }
	        }
        );
        
        //Add the created row to our TableLayout
        tl.addView(buttonContainer, new LayoutParams(
                LayoutParams.FILL_PARENT, 
                LayoutParams.WRAP_CONTENT));
    }
	
	private void loadCustomElement(File file)
	{
		// TODO: Make a native call
	}
	private void deleteFile(CustomElement customElement)
	{
		File file = new File("/sdcard/thelements/elements/" + customElement.getName());
		if(file.exists())
			file.delete();
		refreshElements();
	}
}