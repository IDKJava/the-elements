package idkjava.thelements;


import idkjava.thelements.R;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.ListPreference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceCategory;
import android.preference.PreferenceManager;
import android.preference.PreferenceScreen;
import android.util.Log;
import android.widget.Toast;


public class PreferencesActivity extends PreferenceActivity implements OnSharedPreferenceChangeListener
{

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        
        setPreferenceScreen(createPreferenceHierarchy());
        getPreferenceScreen().getSharedPreferences().registerOnSharedPreferenceChangeListener(this);
    }


    private PreferenceScreen createPreferenceHierarchy()
    {
    	return null;
    	/*
        // Root
        PreferenceScreen root = getPreferenceManager().createPreferenceScreen(this);
        
        // Inline preferences 
        PreferenceCategory inlinePrefCat = new PreferenceCategory(this);
        inlinePrefCat.setTitle("Look and Feel");
        root.addPreference(inlinePrefCat);
        
       
        
        // List preference
        ListPreference listPref = new ListPreference(this);
        listPref.setEntries(R.array.entries_list_preference);
        listPref.setEntryValues(R.array.entryvalues_list_preference);
        listPref.setDialogTitle("Background");
        listPref.setKey("background_color");
        listPref.setTitle("Background Color");
        listPref.setSummary("Sets the color of the background");
        
        root.addPreference(listPref);

       
        PreferenceCategory launchPrefCat = new PreferenceCategory(this);
        launchPrefCat.setTitle("Other");
        root.addPreference(launchPrefCat);
        
        CheckBoxPreference togglePref = new CheckBoxPreference(this);
        togglePref.setKey("flip_settings");
        togglePref.setTitle("Flip Screen");
        togglePref.setSummary("Flip Screen (Use only if nothing draws on screen)");
        root.addPreference(togglePref);
        
        CheckBoxPreference sleep_on_off = new CheckBoxPreference(this);
        sleep_on_off.setKey("sleep");
        sleep_on_off.setTitle("Sleep On/Off");
        sleep_on_off.setSummary("Allow the screen to sleep during play or not");
        root.addPreference(sleep_on_off);
        
        CheckBoxPreference ui_on_off = new CheckBoxPreference(this);
        ui_on_off.setKey("ui");
        ui_on_off.setTitle("UI On/Off");
        ui_on_off.setSummary("Turn the UI on or off");
        root.addPreference(ui_on_off);
        
        PreferenceCategory game = new PreferenceCategory(this);
        game.setTitle("Game");
        root.addPreference(game);
        
        CheckBoxPreference acceler = new CheckBoxPreference(this);
        acceler.setKey("accel_control");
        acceler.setTitle("Accelerometer Control");
        acceler.setSummary("Turn accelerometer control of gravity on/off ");
        root.addPreference(acceler);
        
        
        final SharedPreferences sharedPrefs =
            PreferenceManager.getDefaultSharedPreferences(this);
        if (!sharedPrefs.contains("background_color"))
        {
        	listPref.setValue("black");
        }
        if (!sharedPrefs.contains("flip_settings"))
        {
        	togglePref.setChecked(false);        	
        }
        if (!sharedPrefs.contains("ui"))
        {
        	ui_on_off.setChecked(true);
        }
        if (!sharedPrefs.contains("sleep"))
        {
        	sleep_on_off.setChecked(true);
        }
        if (!sharedPrefs.contains("accel_control"))
        {
        	acceler.setChecked(false);
        }
        
        return root;
        */
    }
    
    //When something is clicked, this finds it and calls a JNI function to change it in the c code
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key)
    {
    	/*
        // Let's do something when my counter preference value changes
        //Log.v("DemoActivity", "onpref called");
        if (key == "background_color")
        {
        	//Log.v("DemoActivity", "onpref color: " + sharedPreferences.getString(key, "black"));
        	if (sharedPreferences.getString( key, "black").equals("black"))
        	{
        		MainActivity.setBackgroundColor(0);
        	}
        	else
        	{
        		MainActivity.setBackgroundColor(1);
        	}
        }
        else if (key == "flip_settings")
        {
        	Log.v("DemoActivity", "onpref flip");
        	if ( sharedPreferences.getBoolean(key, false) == true)
        	{
        		MainActivity.setFlip(1);
        	}
        	else
        	{
        		MainActivity.setFlip(0);
        	}
        }
        else if (key == "ui")
        {
        	MainActivity.ui = sharedPreferences.getBoolean(key, true);
        	Toast.makeText(this, "UI changed: please restart for effect", Toast.LENGTH_LONG).show();
        }
        else if (key == "sleep")
        {
        	MainActivity.sand_view.setKeepScreenOn(sharedPreferences.getBoolean(key,true));
        }
        else if (key == "accel_control")
        {
        	if ( sharedPreferences.getBoolean(key,false) == true){
        		MainActivity.setAccelOnOff(1);
        	}
        	else{
        		MainActivity.setAccelOnOff(0);
        	}
        }
        */
    }
    //used to set stuff up when the app opens and retrieve the settings
    public static void setpreferences(Context context)
    {
    	/*
        final SharedPreferences sharedPreferences =
            PreferenceManager.getDefaultSharedPreferences(context);
    	if (sharedPreferences.getString("background_color", "black").equals("black"))
    	{
    		MainActivity.setBackgroundColor(0);
    	}
    	else
    	{  
    		MainActivity.setBackgroundColor(1);
    	}
    	if (sharedPreferences.getBoolean("flip_settings", false) == true) //flip screen stuff
    	{
    		MainActivity.setFlip(1);
    	}
    	else
    	{
    		MainActivity.setFlip(0);
    	}
    	if (sharedPreferences.getBoolean("accel_control", false) == true) //flip screen stuff
    	{
    		MainActivity.setAccelOnOff(1);
    	}
    	else
    	{
    		MainActivity.setAccelOnOff(0);
    	}
    	
 
    	
    	//Set the bool variable ui in the Activity
    	MainActivity.ui = sharedPreferences.getBoolean("ui", true);
    	*/
    }
    public static void setScreenOnOff(Context context) //set screen on or off, needs to be seperate because sand_view is created after setPrefs is called
    {
    	/*
    	final SharedPreferences sharedPreferences =
            PreferenceManager.getDefaultSharedPreferences(context);
    	
    	MainActivity.sand_view.setKeepScreenOn(sharedPreferences.getBoolean("screen",true)); //keep screen on or not
    	*/
    }
}
