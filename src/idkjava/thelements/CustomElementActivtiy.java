package idkjava.thelements;

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
		if(callingIntent.getAction() == Intent.ACTION_EDIT)
		{
			Uri intentData = callingIntent.getData();
			String filename = intentData.getPath();
			if(loadCustomElement(filename.toCharArray()) == 1)
			{
				
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
	
	public native static char loadCustomElement(char[] filename);
	public native static char saveCustomElement(char[] filename);
	
	static
	{
		System.loadLibrary("thelements");
	}
}
