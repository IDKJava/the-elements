package idkjava.thelements.preferences;

import idkjava.thelements.MainActivity;
import idkjava.thelements.R;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceActivity;
import android.preference.PreferenceManager;
import android.util.Log;


public class PreferencesActivity extends PreferenceActivity //implements OnSharedPreferenceChangeListener
{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        
        addPreferencesFromResource(R.xml.preferences);
    }
    
    //Set everything except UI based on the preferences that are in existence
    public static void setPreferences(Context context)
    {
        final SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(context);
        
        //Log.v("TheElements", "setPreferences");
        setBackgroundColor((char) sharedPreferences.getInt("background_red", 0), (char) sharedPreferences.getInt("background_green", 0), (char) sharedPreferences.getInt("background_blue", 0));
        //Log.v("TheElements", "setBackgroundColor()");
        setFlippedState(sharedPreferences.getBoolean("flipped_state", false));
        //Log.v("TheElements", "setFlipState()");
        setAccelState(sharedPreferences.getBoolean("accel_state", false));
        //Log.v("TheElements", "setAccelState()");
        MainActivity.ui = sharedPreferences.getBoolean("ui_state", true);
        Log.v("TheElements", "set MainActivity.ui: " + sharedPreferences.getBoolean("ui_state", true));
        setBorderState(sharedPreferences.getBoolean("border_left", true),
        		sharedPreferences.getBoolean("border_top", true),
        		sharedPreferences.getBoolean("border_right", true),
        		sharedPreferences.getBoolean("border_bottom", true));
    }
    public static void setScreenState(Context context)
    {
        final SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(context);

        MainActivity.sand_view.setKeepScreenOn(sharedPreferences.getBoolean("screen_state", true));
    }
    
    public static native void setBorderState(boolean leftBorderState, boolean topBorderState, boolean rightBorderState, boolean bottomBorderState);
	public static native void setFlippedState(boolean flippedState);
	public static native void setAccelState(boolean accelState);
	public static native void setBackgroundColor(char red, char green, char blue);
    
    static
    {
    	System.loadLibrary("thelements"); // Load the JNI library (libthelements.so)
    }
}
