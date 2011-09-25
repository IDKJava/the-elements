package idkjava.thelements.preferences;

import idkjava.thelements.MainActivity;
import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;

public class Preferences
{
	private static SharedPreferences sharedPreferences;

	//Initialize the sharedPreferences based on the context
	public static void initSharedPreferences(Context context)
	{
		sharedPreferences = PreferenceManager.getDefaultSharedPreferences(context);
	}

	//Set the ui state in MainActivity based on sharedPreferences
	public static void loadUIState()
	{
		MainActivity.ui = sharedPreferences.getBoolean("ui_state", true);
	}

	//Set everything except UI based on the preferences that are in existence
	public static void loadPreferences()
	{
		Log.v("TheElements", "setPreferences");
		setBackgroundColor((char) sharedPreferences.getInt("background_red", 0), (char) sharedPreferences.getInt("background_green", 0), (char) sharedPreferences.getInt("background_blue", 0));
		setFlippedState(sharedPreferences.getBoolean("flipped_state", false));
		setAccelState(sharedPreferences.getBoolean("accel_state", false));
		setAtmosphereTemp((char) sharedPreferences.getInt("atmosphere_temp", 100));
		setAtmosphereGravity(sharedPreferences.getFloat("atmosphere_gravity", 1));
		setBorderState(sharedPreferences.getBoolean("border_left", true), sharedPreferences.getBoolean("border_top", true), sharedPreferences.getBoolean("border_right", true), sharedPreferences.getBoolean("border_bottom", true));
	}

	public static void loadScreenState()
	{
		MainActivity.sand_view.setKeepScreenOn(sharedPreferences.getBoolean("screen_state", true));
	}

	//@formatter:off
    public static native void setBorderState(boolean leftBorderState, boolean topBorderState, boolean rightBorderState, boolean bottomBorderState);
	public static native void setFlippedState(boolean flippedState);
	public static native void setAccelState(boolean accelState);
	public static native void setBackgroundColor(char red, char green, char blue);
	public static native void setAtmosphereTemp(char temp);
	public static native void setAtmosphereGravity(float gravity);
	//@formatter:on

	static
	{
		System.loadLibrary("thelements"); // Load the JNI library (libthelements.so)
	}
}
