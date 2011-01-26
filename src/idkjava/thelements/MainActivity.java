package idkjava.thelements;

import idkjava.thelements.game.Control;
import idkjava.thelements.game.FileManager;
import idkjava.thelements.game.MenuBar;
import idkjava.thelements.game.SandView;

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
import android.hardware.SensorManager;
import android.os.Bundle; //import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.Window;

public class MainActivity extends Activity
{
	//Constants for zoom
	public static final boolean ZOOMED_IN = true;
	public static final boolean ZOOMED_OUT = false;
	
	//Constants for dialogue ids
	private static final int INTRO_MESSAGE = 1;
	private static final int ELEMENT_PICKER = 2;
	private static final int BRUSH_SIZE_PICKER = 3;
	
	//Constant for the eraser element
	public static final char ERASER_ELEMENT = 3;
	
	//Request code constants
	public static final int REQUEST_CODE_SELECT_SAVE = 0;
	
	static CharSequence[] elementslist;

	public static boolean play = true;
	public static boolean zoomState = ZOOMED_IN; //Zoomed in or not

	private SensorManager mSensorManager;

	public static final String PREFS_NAME = "MyPrefsfile";
	public static boolean shouldLoadDemo = false;

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
		//Do a temp save
		saveTempState();
		//Set the preferences to indicate paused
		SharedPreferences.Editor editor = getSharedPreferences(PREFS_NAME, 0).edit();
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
		//If our target ui state is not our current one, restart the app
		if (layout_ui != ui)
		{
			System.exit(0);
		}
		SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);
		//TODO: myManager.registerListener(mySensorListener, accSensor, SensorManager.SENSOR_DELAY_GAME);

		//If we're resuming from a pause (not when it starts)
		if (settings.getBoolean("paused", true))
		{
			//Load the save
			loadTempState();
			//Set the preferences to indicate unpaused
			SharedPreferences.Editor editor = settings.edit();
			editor.putBoolean("paused", false);
			editor.commit();
		}
		
		//Set up the file manager for saving and loading
		FileManager.intialize(this);
		
		//First run
		if (settings.getBoolean("firstrun", true))
		{
			shouldLoadDemo = true;
			SharedPreferences.Editor editor = settings.edit();
			editor.putBoolean("firstrun", false);
			editor.commit();

			showDialog(INTRO_MESSAGE); //Pop up intro message
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
					finish();
				}
			}).setNegativeButton(R.string.proceed, new DialogInterface.OnClickListener()
			{
				public void onClick(DialogInterface dialog, int id)
				{
					dialog.cancel();
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
					if (MenuBar.eraserOn)
					{
						MenuBar.setEraserOff();
					}
					setElement((char) item);
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
					setBrushSize((char) java.lang.Math.pow(2, item));
				}
			});
			AlertDialog alert = builder.create(); // Create object
			return alert; // Return handle
		}

		return null; //Default case: return nothing
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
			{
				showDialog(ELEMENT_PICKER);
				return true;
			}
			case R.id.brush_size_picker:
			{
				showDialog(BRUSH_SIZE_PICKER);
				return true;
			}
			case R.id.clear_screen:
			{
				clearScreen();
				return true;
			}
			case R.id.play_pause:
			{
				play = !play;
				setPlayState(play);
				return true;
			}
			case R.id.eraser:
			{
				setElement(ERASER_ELEMENT);
				return true;
			}
			case R.id.toggle_size:
			{
				zoomState = !zoomState;
				setZoomState(zoomState);
				return true;
			}
			case R.id.save:
			{
				saveState();
				return true;
			}
			case R.id.load:
			{
				loadState();
				return true;
			}
			case R.id.load_demo:
			{
				loadDemoState();
				return true;
			}
			case R.id.preferences:
			{
				startActivity(new Intent(MainActivity.this, PreferencesActivity.class));
				return true;
			}
			case R.id.exit:
			{
				System.exit(0);
				return true;
			}
		}
		return false;
	}
	
	//Save the current state
	public boolean saveState()
	{
		//TODO: Call the save activity
		return false;
	}
	public boolean loadState()
	{
		Intent tempIntent = new Intent(this, SaveSelectionActivity.class);
		startActivityForResult(tempIntent, REQUEST_CODE_SELECT_SAVE);
		
		return false;
	}
	
	//Check whether or not the game is zoomed in
	public static boolean zoomedIn()
	{
		return zoomState;
	}

	//JNI Functions
	//Save/load functions
	public static native char saveTempState();
	public static native char loadTempState();
	public static native char loadDemoState();
	public static native char removeTempSave();
	
	//General utility functions
	public static native void clearScreen();
	public static native void setBackgroundColor(char red, char green, char blue);
	public static native void setDimensions(int width, int height);
	
	//Setters
	public static native void setPlayState(boolean playState);
	public static native void setAccelState(boolean accelState);
	public static native void setZoomState(boolean zoomState);
	public static native void setElement(char element);
	public static native void setBrushSize(char brushSize);
	
	//Getters
	public static native char getElement();
	
	//TODO: Acclerometer related
	public static native void setXGravity(float xGravity);
	public static native void setYGravity(float yGravity);
	
	//TODO: Network related
	public static native void setUsername(char[] username);
	public static native void setPassword(char[] password);
	public static native char login();
	public static native char register();
	public static native void viewErr(); //TODO: Figure this out

	static
	{
		System.loadLibrary("thelements"); // Load the JNI library (libthelements.so)
	}
}
