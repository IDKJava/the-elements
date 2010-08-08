package sand.falling.opengl.network;

import sand.falling.opengl.Menu;
import sand.falling.opengl.R;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.TableRow.LayoutParams;

public class networkengage extends Activity {
	//Declare a couple things for the purpose of dragon slaying
	static public LinearLayout buttonContainer;
    static public TableRow tr;
    static public ImageButton actionButton;
	static public ImageButton ne_close;
	//For the LOL's
	public String bestSynonyms[] = {
		"Awesomest",
		"bang-up",
		"corking",
		"good enough",
		"satisfactory",
		"MostExcellent",
		"Premium"
	};
    
	public void onCreate(Bundle savedInstanceState)
	{		
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.networkengage);
		ne_close = (ImageButton)findViewById(R.id.ne_close);
		//Create a couple extra fun example rows...
        for(int i = 0; i < 6; i++) {
        	//Most values are blank as this is simply filler (for now)
        	add_entity(bestSynonyms[i], "", i, 1);
        }
		//Create a couple generic example rows...
        for(int i = 0; i < 20; i++) {
        	//Most values are blank as this is simply filler (for now)
        	add_entity("", "", i, 0);
        }
        //Get ne_close to go to the Menu activity
        ne_close.setOnClickListener(new OnClickListener()
		{

			public void onClick(View viewParam)
			{
				
			//Start the main app activity
				startActivity(new Intent(networkengage.this, Menu.class));
			
			}
		});
	}	

	
	
	/* Method that adds a row with a given context
	 * entityName defines the text
	 * entityURI (only necessary when taking from server)
	 * id = for the example, will make names a bit different for the rows
	 * actionType would make the button have a different icon depending on its role
	 * e.g. 0 for download file
	 * 		1 for upload file
	 * 		2 for run file
	 * etc
	 */
	public void add_entity(String entityName, String entityURI, int id, int actionType) {
		//DIP representation math
		final float SCALE = getBaseContext().getResources().getDisplayMetrics().density;
		
		/* Find Tablelayout defined in networkengage.xml */
	    TableLayout tl = (TableLayout)findViewById(R.id.entity_container);
	    
	    
        buttonContainer = new LinearLayout(this);
        /* Create a new row to be added. */
        tr = new TableRow(this);
        tr.setGravity(16);
        
        tr.setLayoutParams(new LayoutParams(
                                LayoutParams.FILL_PARENT, 
                                LayoutParams.WRAP_CONTENT));
        tr.setBackgroundResource(0x7f020002);
        tr.setLongClickable(true);
        tr.setClickable(true);
        
        
        /* Create a Textview for the file name */
        TextView filename = new TextView(this);
        if(actionType == 1) {
        	filename.setText(entityName+".txt");
        }
        else {
        	filename.setText("MyBestDestructableExplosion"+id+".txt");
        }
        filename.setLayoutParams(new LayoutParams(
        		(int)(274 * SCALE + 0.5f),
        		(int)(50 * SCALE + 0.5f)));	
        filename.setGravity(16);
        //Makes use of DisplayMetrics to create an automatic dip unit :)
        filename.setPadding((int)(16 * SCALE + 0.5f), 0, 0, 0);
        tr.addView(filename);
        //sets buttonContainer's gravity to right with a 16dip right padding
        buttonContainer.setGravity(17);
        buttonContainer.setPadding(0, 0, (int)(16 * SCALE + 0.5f), 0);
        
        	/* Create a Button to be action invoker */
	        actionButton = new ImageButton(this);
	        actionButton.setBackgroundResource(0x7f02000b);
	        actionButton.setLayoutParams(new LayoutParams(
	                        LayoutParams.FILL_PARENT, 
	                        LayoutParams.WRAP_CONTENT));
	
	        /* Add Button to row. */
	        buttonContainer.addView(actionButton);
	        tr.addView(buttonContainer);
        /* Add row to TableLayout. */
        tl.addView(tr,new TableLayout.LayoutParams(
                        LayoutParams.FILL_PARENT, 
                        LayoutParams.WRAP_CONTENT));
	}
	
	
}
