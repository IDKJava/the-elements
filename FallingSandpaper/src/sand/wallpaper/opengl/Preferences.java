package sand.wallpaper.opengl;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.ListPreference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceCategory;
import android.preference.PreferenceScreen;

public class Preferences extends PreferenceActivity implements
        OnSharedPreferenceChangeListener {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getPreferenceManager().setSharedPreferencesName(
                "TheElementsWallpaperPrefs");

        // Set the preference screen created by createPreferenceHierarchy()
        setPreferenceScreen(createPreferenceHierarchy());
        getPreferenceScreen().getSharedPreferences()
                .registerOnSharedPreferenceChangeListener(this);
    }

    // Function to set up the preferences
    private PreferenceScreen createPreferenceHierarchy() {
        // Define the root PreferenceScreen
        PreferenceScreen root = getPreferenceManager().createPreferenceScreen(
                this);

        // Look and feel category and add to root
        PreferenceCategory look_feel_cat = new PreferenceCategory(this);
        look_feel_cat.setTitle(R.string.look_feel_title);
        root.addPreference(look_feel_cat);

        // Background color list preference
        ListPreference back_color_pref = new ListPreference(this);
        back_color_pref.setEntries(R.array.entries_list_preference);
        back_color_pref.setEntryValues(R.array.entryvalues_list_preference);
        back_color_pref.setDialogTitle(R.string.back_color_pref_title);
        back_color_pref.setKey("back_color_pref");
        back_color_pref.setTitle(R.string.back_color_pref_title);
        back_color_pref.setSummary(R.string.back_color_pref_summary);
        root.addPreference(back_color_pref);

        // Other category
        PreferenceCategory other_cat = new PreferenceCategory(this);
        other_cat.setTitle(R.string.other_title);
        root.addPreference(other_cat);

        // Flip screen checkbox
        CheckBoxPreference flip_screen_pref = new CheckBoxPreference(this);
        flip_screen_pref.setTitle("Flip Screen");
        flip_screen_pref
                .setSummary("Flip screen (use only if nothing draws on screen)");
        flip_screen_pref.setKey("flip_screen_pref");
        root.addPreference(flip_screen_pref);

        // Game category
        PreferenceCategory game_cat = new PreferenceCategory(this);
        game_cat.setTitle(R.string.game_cat_title);
        root.addPreference(game_cat);

        // Accelerometer checkbox
        CheckBoxPreference accel_pref = new CheckBoxPreference(this);
        accel_pref.setTitle(R.string.accel_pref_title);
        accel_pref.setSummary(R.string.accel_pref_summary);
        accel_pref.setKey("accel_pref");
        root.addPreference(accel_pref);

        // Random checkbox
        CheckBoxPreference random_pref = new CheckBoxPreference(this);
        random_pref.setTitle(R.string.random_pref_title);
        random_pref.setSummary(R.string.random_pref_summary);
        random_pref.setKey("random_pref");
        root.addPreference(random_pref);

        // Create the shared prefs used to access all this
        final SharedPreferences sharedPrefs = getSharedPreferences(
                "TheElementsWallpaperPrefs", 0);

        // Set default values
        if (!sharedPrefs.contains("back_color_pref"))
            back_color_pref.setValue("black");
        if (!sharedPrefs.contains("flip_screen_pref"))
            flip_screen_pref.setChecked(false);
        if (!sharedPrefs.contains("accel_pref"))
            accel_pref.setChecked(false);
        if (!sharedPrefs.contains("random_pref"))
            random_pref.setChecked(true);

        return root;
    }

    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences,
            String key) {

        // Dunno what's with all the nots, just leaving as is
        if (key == "back_color_pref") {
            char color = (sharedPreferences.getString("back_color_pref",
                    "black").equalsIgnoreCase("black")) ? (char) 0 : (char) 255;
            setBackgroundColor(color, color, color);
        } else if (key == "flip_screen_pref") {
            setFlippedState(!(sharedPreferences.getBoolean("flip_screen_pref",
                    false) == false));
        } else if (key == "accel_pref") {
            setAccelState(!(sharedPreferences.getBoolean("accel_pref", false) == false));
        }

    }

    public static void setPreferences(Context context) {
        final SharedPreferences sharedPreferences = context
                .getSharedPreferences("TheElementsWallpaperPrefs", 0);

        char color = (sharedPreferences.getString("back_color_pref", "black")
                .equalsIgnoreCase("black")) ? (char) 0 : (char) 255;

        setBackgroundColor(color, color, color);

        setFlippedState(!(sharedPreferences.getBoolean("flip_screen_pref",
                false) == false));
        setAccelState(!(sharedPreferences.getBoolean("accel_pref", false) == false));
    }

    public static native void setBorderState(boolean leftBorderState,
            boolean topBorderState, boolean rightBorderState,
            boolean bottomBorderState);

    public static native void setFlippedState(boolean flippedState);

    public static native void setAccelState(boolean accelState);

    public static native void setBackgroundColor(char red, char green, char blue);

    public static native void setAtmosphereTemp(char temp);

    public static native void setAtmosphereGravity(float gravity);

    public static native void setZoom(int zoom);

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("protobuf");
        System.loadLibrary("thelements"); // Load the JNI library
                                          // (libthelements.so)
    }
}
