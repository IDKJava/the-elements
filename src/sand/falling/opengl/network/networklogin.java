package sand.falling.opengl.network;

import sand.falling.opengl.DemoActivity;
import sand.falling.opengl.Menu;
import sand.falling.opengl.R;
import sand.falling.opengl.Splash;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

/**
 * Application Name: Generic Login Screen for the Android Platform (back end)
 * Description: This is a generic login screen which catches the username and
 * password values Created on: November 23, 2007 Created by: Pogz Ortile
 * Contact: pogz(at)redhat(dot)polarhome(dot)com Notes: The string values for
 * username and password are assigned to sUserName and sPassword respectively
 * You are free to distribute, modify, and wreck for all I care. GPL ya!
 * */

public class networklogin extends Activity
{
	/** Called when the activity is first created. */

	static public Button launch;
	static public Button register;
	static public Button network_engage;
	static public EditText usernameEditText;
	static public EditText passwordEditText;
	static public String sUserName;
	static public String sPassword;

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		// load up the layout
		setContentView(R.layout.login_layout);

		// get the button resource in the xml file and assign it to a local
		// variable of type Button
		launch = (Button) findViewById(R.id.login_button2);
		register = (Button) findViewById(R.id.register_button);
		network_engage = (Button) findViewById(R.id.engage_test);
		// this gets the resources in the xml file and assigns it to a
		// local variable of type EditText
		usernameEditText = (EditText) findViewById(R.id.txt_username);
		passwordEditText = (EditText) findViewById(R.id.txt_password);

		

		// this is the action listener
		launch.setOnClickListener(new OnClickListener()
		{

			public void onClick(View viewParam)
			{
				
				// the getText() gets the current value of the text box
				// the toString() converts the value to String data type
				// then assigns it to a variable of type String
				sUserName = usernameEditText.getText().toString();
				sPassword = passwordEditText.getText().toString();

				if (usernameEditText == null || passwordEditText == null)
				{
					Toast.makeText(networklogin.this, "Couldn't find the 'txt_username' or 'txt_password'", Toast.LENGTH_SHORT).show();
				}
				else
				{
					// display the username and the password in string format
					Toast.makeText(networklogin.this, "username:" + sUserName + " password:" + sPassword, Toast.LENGTH_SHORT).show();
				}
			}
		}

		); // end of launch.setOnclickListener
		register.setOnClickListener(new OnClickListener()
		{

			public void onClick(View viewParam)
			{
				
				// the getText() gets the current value of the text box
				// the toString() converts the value to String data type
				// then assigns it to a variable of type String
				sUserName = usernameEditText.getText().toString();
				sPassword = passwordEditText.getText().toString();

				// this just catches the error if the program cant locate the
				// GUI stuff
				if (usernameEditText == null || passwordEditText == null)
				{
					Toast.makeText(networklogin.this, "Couldn't find the 'txt_username' or 'txt_password'", Toast.LENGTH_SHORT).show();
				}
				else
				{
					// display the username and the password in string format
					Toast.makeText(networklogin.this, " Register Username:" + sUserName + " Password:" + sPassword, Toast.LENGTH_SHORT).show();
				}
			}
		}

		); // end of register.setOnClickStuff
		network_engage.setOnClickListener(new OnClickListener()
		{

			public void onClick(View viewParam)
			{
				
			//Start the main app activity
				startActivity(new Intent(networklogin.this, networkengage.class));
			
			}
		}

		); // end of register.setOnClickStuff
	}

}
