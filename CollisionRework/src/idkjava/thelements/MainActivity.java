package idkjava.thelements;

import idkjava.thelements.game.Control;
import idkjava.thelements.game.CustomElementManager;
import idkjava.thelements.game.FileManager;
import idkjava.thelements.game.MenuBar;
import idkjava.thelements.game.SandView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.List;


import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle; //import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.Window;

public class MainActivity extends Activity
{
	//Constants for zoom
	public static final int ZOOMED_IN = 0;
	public static final int ZOOMED_OUT = 1;
	
	//Constants for dialogue ids
	private static final int INTRO_MESSAGE = 1;
	private static final int ELEMENT_PICKER = 2;
	private static final int BRUSH_SIZE_PICKER = 3;
	
	//Constant for the eraser element
	public static final int ERASER_ELEMENT = 3;
	
	static CharSequence[] elementslist;

	public static boolean play = true;
	public static int size = ZOOMED_IN; //Zoomed in or not

	private SensorManager mSensorManager;

	public static final String PREFS_NAME = "MyPrefsfile";
	public static boolean loaddemov = false;

	public static boolean ui;
	public static boolean layout_ui;

	public static MenuBar menu_bar;
	public static Control control;
	public static SandView sand_view;

	private SensorManager myManager;
	private List<Sensor> sensors;
	private Sensor accSensor;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState); //Uses onCreate from the general Activity

		requestWindowFeature(Window.FEATURE_NO_TITLE); //Get rid of title bar

		PreferencesActivity.setpreferences(this);

		//Set Sensor + Manager
		myManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
		accSensor = myManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

		//Set the layout based on the settings
		if (ui)
		{
			setContentView(R.layout.ui);
		}
		else
		{
			setContentView(R.layout.non_ui);
		}

		//Sync on startup (layout_ui is not changed on preference changed for smoothness)
		layout_ui = ui;

		//Need to do this otherwise it gives a nullpointerexception
		menu_bar = new MenuBar(this, null);
		sand_view = new SandView(this, null);
		control = new Control(this, null);

		//Set the new view and control box and menu bar to the stuff defined in layout
		menu_bar = (MenuBar) findViewById(R.id.menu_bar);
		sand_view = (SandView) findViewById(R.id.sand_view);
		control = (Control) findViewById(R.id.control);

		PreferencesActivity.setScreenOnOff(this); //Finds out to keep screen on or off
		
		//Set up the elements list
		Resources res = getResources();
		elementslist = res.getTextArray(R.array.elements_list);
		
		//Load the custom elements
		//CustomElementManager.reloadCustomElements();
		
		//Add custom elements to the elements list
	}

	/*
	private final SensorEventListener mySensorListener = new SensorEventListener()
	{
		public void onSensorChanged(SensorEvent event)
		{
			sendXGrav(event.values[0]);
			sendYGrav(event.values[1]);
		}

		public void onAccuracyChanged(Sensor sensor, int accuracy)
		{
		}
	};
	*/
	@Override
	protected void onPause()
	{
		//QuickSave
		tempSave();
		//Set the preferences to indicate paused
		SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);
		SharedPreferences.Editor editor = settings.edit();
		editor.putBoolean("paused", true);
		editor.commit();
		//Use the normal onPause
		super.onPause();
		//Call onPause for the view
		sand_view.onPause();
	}

	@Override
	protected void onResume()
	{
		if (layout_ui != ui)
		{
			System.exit(0);
		}
		SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);
		//myManager.registerListener(mySensorListener, accSensor, SensorManager.SENSOR_DELAY_GAME);

		//If we're resuming from a pause (not when it starts)
		if (settings.getBoolean("paused", true))
		{
			//Load the save
			//tempLoad();
			//Set the preferencWhen a tes to indicate unpaused
			SharedPreferences.Editor editor = settings.edit();
			editor.putBoolean("paused", false);
			editor.commit();
		}
		
		FileManager.intialize(this);
		
		//If it's the first run, tell it to load the demo and unset firstrun
		if (settings.getBoolean("firstrun", true))
		{
			loaddemov = true;
			SharedPreferences.Editor editor = settings.edit();
			editor.putBoolean("firstrun", false);
			editor.commit();

			showDialog(1); //Pop up intro message
		}

		if (layout_ui)
		{
			//This is where I set the activity for Control so that I can call showDialog() from it
			control.setActivity(this);
			//Set instance of activity for MenuBar also
			menu_bar.setActivity(this);
		}
		
		//Use the super onResume as well
		super.onResume();
		//Call onResume() for view too
		sand_view.onResume();
	}

	protected Dialog onCreateDialog(int id) //This is called when showDialog is called
	{
		if (id == INTRO_MESSAGE) // The first dialog - the intro message
		{
			AlertDialog.Builder builder = new AlertDialog.Builder(this);
			builder.setMessage(R.string.app_intro).setCancelable(false).setPositiveButton(R.string.exit, new DialogInterface.OnClickListener()
			{
				public void onClick(DialogInterface dialog, int id)
				{
					MainActivity.this.finish(); // Exit button
					// closes
					// program
				}
			}).setNegativeButton(R.string.proceed, new DialogInterface.OnClickListener()
			{
				public void onClick(DialogInterface dialog, int id)
				{
					dialog.cancel(); // Proceed closes the
					// message
				}
			});
			AlertDialog alert = builder.create(); // Actually create the message
			return alert; // Return the object created
		}
		else if (id == ELEMENT_PICKER) // Element picker
		{
			AlertDialog.Builder builder = new AlertDialog.Builder(this); // Create a new one
			builder.setTitle(R.string.element_picker); // Set the title
			builder.setItems(elementslist, new DialogInterface.OnClickListener() //Create the list
			{
				public void onClick(DialogInterface dialog, int item)
				{
					if (MenuBar.eraser_on)
					{
							MenuBar.setEraserOff();
					}

					if (item == 0) // Sand
					{
						setElement(0);
					}
					else if (item == 1) // Water
					{
						setElement(1);
					}
					else if (item == 2) // Plant
					{
						setElement(4);
					}
					else if (item == 3) // Wall
					{
						setElement(2);
					}
					else if (item == 4) // Fire
					{
						setElement(5);
					}
					else if (item == 5) // Ice
					{
						setElement(6);
					}
					else if (item == 6)// Generator
					{
						setElement(7);
					}
					else if (item == 7)// Oil
					{
						setElement(9);
					}
					else if (item == 8)// Magma
					{
						setElement(10);
					}
					else if (item == 9)// Stone
					{
						setElement(11);
					}
					else if (item == 10)// C4
					{
						setElement(12);
					}
					else if (item == 11)// C4++
					{
						setElement(13);
					}
					else if (item == 12)// Fuse
					{
						setElement(14);
					}
					else if (item == 13)// Destructible wall
					{
						setElement(15);
					}
					else if (item == 14)// Drag
					{
						setElement(16);
					}
					else if (item == 15)// Acid
					{
						setElement(17);
					}
					else if (item == 16)// Steam
					{
						setElement(18);
					}
					else if (item == 17)// Salt
					{
						setElement(19);
					}
					else if (item == 18)// Salt-water
					{
						setElement(20);
					}
					else if (item == 19)// Glass
					{
						setElement(21);
					}
					else if (item == 20)// Custom 1
					{
						setElement(22);
					}
					else if (item == 21)// Mud
					{
						setElement(23);
					}
					else if (item == 22)// Custom 3
					{
						setElement(24);
					}
				}
			});
			AlertDialog alert = builder.create(); // Create the dialog

			return alert; // Return handle
		}
		else if (id == BRUSH_SIZE_PICKER)
		{
			AlertDialog.Builder builder = new AlertDialog.Builder(this); // Declare the object
			builder.setTitle(R.string.brush_size_picker);
			builder.setItems(R.array.brush_size_list, new DialogInterface.OnClickListener()
			{
				public void onClick(DialogInterface dialog, int item)
				{
					setBrushSize((int) java.lang.Math.pow(2, item));
				}
			});
			AlertDialog alert = builder.create(); // Create object
			return alert; // Return handle
		}

		return null; // No need to return anything, just formality
	}

	public boolean onPrepareOptionsMenu(Menu menu) // Pops up when you press Menu
	{
		// Create an inflater to inflate the menu already defined in res/menu/options_menu.xml
		// This seems to be a bit faster at loading the menu, and easier to modify
		MenuInflater inflater = getMenuInflater();
		if (layout_ui)
		{
			menu.clear();
			inflater.inflate(R.menu.options_menu_small, menu);
		}
		else
		{
			menu.clear();
			inflater.inflate(R.menu.options_menu_large, menu);
		}

		return true;
	}

	public boolean onOptionsItemSelected(MenuItem item)
	{
		switch (item.getItemId())
		{
			case R.id.element_picker:
	
				showDialog(2);
	
				return true;
			case R.id.brush_size_picker:
	
				showDialog(3);
	
				return true;
			case R.id.clear_screen:
	
				clearScreen();
	
				return true;
			case R.id.play_pause:
	
				if (play)
				{
					pause();
				}
				else
				{
					play();
				}
				play = !play;
				return true;
			case R.id.eraser:
	
				setElement(3);
				return true;
			case R.id.toggle_size:
	
				if (size == 1)
				{
					size = 0;
				}
				else
				{
					size = 1;
				}
				toggleSize();
				return true;
			case R.id.save:
	
				save();
				return true;
			case R.id.load:
	
				load();
				return true;
			case R.id.load_demo:
	
				loadDemo();
				return true;
			case R.id.preferences:
	
				startActivity(new Intent(MainActivity.this, PreferencesActivity.class));
				return true;
			case R.id.exit:
	
				System.exit(0);
				return true;
		}
		return false;
	}
	
	//Check whether or not the game is zoomed in
	public static boolean zoomedIn()
	{
		return size == ZOOMED_IN;
	}

	//JNI functions
	public native static int save();
	public native static int loadDemo();
	public native static int load();
	public native static void clearScreen(); //Set up arrays and such
	public native static void tester();
	public native static void play(); // Jni play
	public native static void pause(); // Jni pause
	public native static int  getPlayState(); //Get the play state
	public native static void toggleSize(); // Jni toggle size
	public native static void tempSave();
	public native static void tempLoad();
	public native static void setBackgroundColor(int colorcode);
	public native static void setFlip(int flipped);
	public native static void setElement(int element);
	public native static void setBrushSize(int jsize);
	public native static int  getElement();
	public native static char removeTempSave();
	public native static void sendYGrav(float ygrav);
	public native static void sendXGrav(float xgrav);
	public native static void setDimensions(int width, int height);
	public native static void setAccelOnOff(int state);
	public native static void setCollision(int custnumber, int elementnumb, int collisionspot, int collisionnumber);
	public native static void setUsername( char[] username);
	public native static void setPassword ( char[] password);
	public native static boolean login();
	public native static boolean register();

	static
	{
		System.loadLibrary("thelements"); // Load the JNI library (libthelements.so)
	}
}
