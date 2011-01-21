package idkjava.thelements;

import idkjava.thelements.game.CustomElementManager;
import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;

public class CustomElementActivtiy extends Activity
{
	//TODO: Depending on the Intent, ACTION_EDIT or ACTION_NEW, set up the UI with
	//either default or loaded data, and allow the user to edit and save
	private Intent callingIntent;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		callingIntent = getIntent();
		if(callingIntent.getAction() == Intent.ACTION_VIEW)
		{
			//Get the filename that should be stored in the intent data
			Uri intentData = callingIntent.getData();
			String filename = intentData.getPath();
			int customElementIndex = CustomElementManager.loadAndGetIndex(filename.toCharArray());
			if(customElementIndex != -1)
			{
				//Call getters to fetch the info for the specific element index
			}
		}
		else
		{
			
		}
	}
	
	//When creating, the elements above and below the state that we point to are going to be
	//indices because if you want to point to an element not yet created there will be problems
	//When loading elements, these indices will temporarily be stored, and then once finished
	//will be resolved to pointers
}
