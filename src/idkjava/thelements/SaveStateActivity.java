package idkjava.thelements;

import idkjava.thelements.game.SaveManager;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class SaveStateActivity extends Activity
{
	public static Button saveButton;
	public static EditText statename;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.save_state_activity);
		
		saveButton = (Button) findViewById(R.id.save_state_button);
		statename = (EditText) findViewById(R.id.save_state_filename);
		
		saveButton.setOnClickListener
		(
				new OnClickListener()
				{
					public void onClick(View v)
					{
						SaveManager.saveState(statename.getText().toString());
						finish();
					}
				}
		);
		
		if(MainActivity.last_state_loaded != null)
		{
			statename.setText(MainActivity.last_state_loaded);
		}
	}
}
