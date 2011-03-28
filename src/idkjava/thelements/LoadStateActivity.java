package idkjava.thelements;

import idkjava.thelements.game.FileManager;
import idkjava.thelements.game.SaveManager;
import android.app.Activity;
import android.content.res.Resources;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

public class LoadStateActivity extends Activity
{
	public static TableRow tr;
	public static LinearLayout buttonContainer;
	public static ImageButton actionButton;
	private static Resources res;
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.load_state_activity);
		
		//Get the associated resources
		res = getResources();
		
		//Refresh the files list
		SaveManager.refresh();
		//Go through and find all the save files and dynamically load them
		int length = SaveManager.getNumSaves();
		if(length != 0)
		{
			for(int i = 0; i < length; i++)
			{
				addEntity(SaveManager.getSaveName(i));
			}
		}
		else
		{
			TableLayout tl = (TableLayout)findViewById(R.id.loads_container);
			tr = new TableRow(this);
			tr.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
			tr.setGravity(Gravity.CENTER);
			
			TextView tv = new TextView(this);
			tv.setText(res.getText(R.string.no_saves));
			
			tr.addView(tv);
			tl.addView(tr);
		}
	}
	
    public void addEntity(String entityName)
    {
    	final String entityNameFinal = entityName;
        //DIP representation math
        //final float SCALE = getBaseContext().getResources().getDisplayMetrics().density;
        
        //Create a TableLayout object associated with the TableLayout in the .xml file
        TableLayout tl = (TableLayout)findViewById(R.id.loads_container);
        //Create a LinearLayout to contain our button within our row
        buttonContainer = new LinearLayout(this);
		//Create the new TableRow that will be added
		tr = new TableRow(this);
		//Set some properties
		//tr.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT));
		tr.setBackgroundResource(R.drawable.load_state_tr_bg);
		tr.setLongClickable(true);
		tr.setClickable(true);
		tr.setPadding(25, 10, 25, 10);
		
		//Create a TextView to hold the filename
		TextView filename = new TextView(this);
		filename.setText(entityName);
		//filename.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.WRAP_CONTENT));
		
		//Makes use of DisplayMetrics to create an automatic dip unit :)
		//filename.setPadding((int)(16 * SCALE + 0.5f), 0, 0, 0);
		//Add the TextView to the TableRow
		tr.addView(filename);

		//Set buttonContainer's gravity to right with a 16dip right padding
		//buttonContainer.setPadding(0, 0, (int)(16 * SCALE + 0.5f), 0);
		//buttonContainer.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, 50));
		buttonContainer.setGravity(Gravity.RIGHT);
		
        //Create a button to be the action invoker
        ImageButton actionButton = new ImageButton(this);
        actionButton.setBackgroundResource(R.drawable.load_state_btn);
        actionButton.setLayoutParams(new LayoutParams(
                        LayoutParams.FILL_PARENT, 
                        LayoutParams.WRAP_CONTENT));
        actionButton.setOnClickListener
        (
    		new OnClickListener()
    		{
    			public void onClick(View v)
    			{
    				//TODO: Something
    			}
    		}
        );

        //Add the button to the TableRow
        buttonContainer.addView(actionButton);
        tr.addView(buttonContainer);
        
        //Add a test click event
        tr.setOnClickListener
        (
        	new OnClickListener()
	        {
                public void onClick(View viewParam)
                {
                	SaveManager.loadState(entityNameFinal+FileManager.SAVE_EXT);
                	finish();
                }
	        }
        );
        
        //Add the created row to our TableLayout
        tl.addView(tr, new LayoutParams(
                LayoutParams.FILL_PARENT, 
                LayoutParams.WRAP_CONTENT));
    }
}