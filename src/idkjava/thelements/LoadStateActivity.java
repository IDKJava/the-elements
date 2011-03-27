package idkjava.thelements;

import idkjava.thelements.game.FileManager;
import idkjava.thelements.game.SaveManager;
import android.app.Activity;
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
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.load_state_activity);
		
		//TODO: Go through and find all the save files and dynamically load them
		//Some sample entries to test UI for now
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		addEntity("Hi");
		addEntity("Another saved state");
		
	}
	
    public void addEntity(String entityName)
    {
    	final String entityNameFinal = entityName;
        //DIP representation math
        final float SCALE = getBaseContext().getResources().getDisplayMetrics().density;
        
        //Create a TableLayout object associated with the TableLayout in the .xml file
        TableLayout tl = (TableLayout)findViewById(R.id.loads_container);
        //Create a LinearLayout to contain our button within our row
        buttonContainer = new LinearLayout(this);
		//Create the new TableRow that will be added
		tr = new TableRow(this);
		//Set some properties
		tr.setGravity(Gravity.CENTER_VERTICAL);
		tr.setLayoutParams(new LayoutParams(
		                        LayoutParams.FILL_PARENT, 
		                        LayoutParams.WRAP_CONTENT));
		tr.setBackgroundResource(R.drawable.load_state_tr_bg);
		tr.setLongClickable(true);
		tr.setClickable(true);
		
		//Create a TextView to hold the filename
		TextView filename = new TextView(this);
		filename.setText("Helloasdfkljasdkfasdfasdfasdflajsdklfajs");
		filename.setLayoutParams(new LayoutParams(
		                LayoutParams.FILL_PARENT,
		                LayoutParams.WRAP_CONTENT));
		filename.setGravity(Gravity.CENTER_VERTICAL);
		
		//Makes use of DisplayMetrics to create an automatic dip unit :)
		filename.setPadding((int)(16 * SCALE + 0.5f), 0, 0, 0);
		//Add the TextView to the TableRow
		tr.addView(filename);

		//Set buttonContainer's gravity to right with a 16dip right padding
		buttonContainer.setGravity(Gravity.RIGHT);
		buttonContainer.setPadding(0, 0, (int)(16 * SCALE + 0.5f), 0);

        //Create a button to be the action invoker
        ImageButton actionButton = new ImageButton(this);
        actionButton.setBackgroundResource(R.drawable.load_state_btn);
        actionButton.setLayoutParams(new LayoutParams(
                        LayoutParams.FILL_PARENT, 
                        LayoutParams.WRAP_CONTENT));

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
        tl.addView(tr, new TableLayout.LayoutParams(
                LayoutParams.FILL_PARENT, 
                LayoutParams.WRAP_CONTENT));
    }
}