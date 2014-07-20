package com.idkjava.thelements;

import java.util.ArrayList;
import java.util.List;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.webkit.WebView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.flurry.android.FlurryAgent;
import com.google.analytics.tracking.android.EasyTracker;
import com.google.analytics.tracking.android.ExceptionReporter;
import com.google.analytics.tracking.android.GAServiceManager;
import com.google.analytics.tracking.android.Tracker;
import com.idkjava.thelements.custom.CustomElementManager;
import com.idkjava.thelements.custom.CustomElementManagerActivity;
import com.idkjava.thelements.game.Control;
import com.idkjava.thelements.game.FileManager;
import com.idkjava.thelements.game.MenuBar;
import com.idkjava.thelements.game.SandView;
import com.idkjava.thelements.game.SaveManager;
import com.idkjava.thelements.keys.APIKeys;
import com.idkjava.thelements.preferences.Preferences;
import com.idkjava.thelements.preferences.PreferencesActivity;
import com.idkjava.thelements.proto.Messages.CustomElement;
import com.kamcord.android.Kamcord;
import com.pollfish.constants.Position;
import com.pollfish.main.PollFish;

public class MainActivity extends ReportingActivity implements DialogInterface.OnCancelListener
{
    //Constants for dialogue ids
    private static final int INTRO_MESSAGE = 1;
    public static final int ELEMENT_PICKER = 2;
    private static final int BRUSH_SIZE_PICKER = 3;
    private static final int RATE_PROMPT = 4;

    //Constants for elements
    public static final char ERASER_ELEMENT = 2;
    public static final char NORMAL_ELEMENT = 3;
    public static final int NUM_BASE_ELEMENTS = 32;

    //Constants for intents
    public static final char SAVE_STATE_ACTIVITY = 0;

    //Request code constants
    public static final int REQUEST_CODE_SELECT_SAVE = 0;
        
    //Constants for specials, collisions
    public static final int MAX_SPECIALS = 6;
    public static final int NUM_COLLISIONS = 12;

    static CharSequence[] baseElementsList;
    static ArrayList<String> elementsList;
    
    private static final int COLOR_SQUARE_SIZE = 40;

    public static boolean play;

    private SensorManager mSensorManager;

    public static final String PREFS_NAME = "MyPrefsfile";
    public static boolean shouldLoadDemo = false;

    public static MenuBar menu_bar;
    public static Control control;
    public static SandView sand_view;
    
    private ElementAdapter mElementAdapter;
        
    public static String last_state_loaded = null;

    private SensorManager myManager;
    private List<Sensor> sensors;
    private Sensor accSensor;

    private static float mDPI; 

    private Tracker mTracker;

    @Override
    protected void onCreate(Bundle savedInstanceState) 
    {
        //Uses onCreate from the general Activity
        super.onCreate(savedInstanceState);

        //Set up EasyTracker custom error reporting
        EasyTracker curTracker = EasyTracker.getInstance(this);
        ExceptionReporter handler = new ExceptionReporter(
                curTracker, GAServiceManager.getInstance(),
                Thread.getDefaultUncaughtExceptionHandler(), this);
        handler.setExceptionParser(new CustomExceptionParser());
        Thread.setDefaultUncaughtExceptionHandler(handler);

        //Init the shared preferences and set the ui state
        Preferences.initSharedPreferences(this);
        //Init kamcord recording framework
        Kamcord.initKeyAndSecret(APIKeys.kamcordAPIKey, APIKeys.kamcordAPISecret, "TheElements");
        Kamcord.initActivity(this);
        //Set Sensor + Manager
        myManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        accSensor = myManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

        setUpViews();

        elementsList = new ArrayList<String>();

        //Start unpaused
        play = true;
        menu_bar.setPlayState(play);

        // Get DPI from screen -- TODO: Sometimes this lies, add custom function to do this with hardcoded values
        DisplayMetrics dm = new DisplayMetrics();
        ((WindowManager) this.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getMetrics(dm);
        mDPI = dm.densityDpi;
    }

    private final SensorEventListener mySensorListener = new SensorEventListener()
    {
        @Override
        public void onSensorChanged(SensorEvent event)
        {
            setXGravity(event.values[0]);
            setYGravity(event.values[1]);
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy)
        {}
    };

    @Override
    protected void onPause()
    {
        // Log.v("TheElements", "MainActivity.onPause()");
        //Use the normal onPause
        super.onPause();
        //Call onPause for the view
        sand_view.onPause();
                
        //Do a temp save
        saveTempState();
        //Set the preferences to indicate paused
        SharedPreferences.Editor editor = getSharedPreferences(PREFS_NAME, 0).edit();
        editor.putBoolean("paused", true);
        editor.commit();

    }

    @Override
    protected void onResume()
    {
        //Use the super onResume
        super.onResume();
        PollFish.init(this, APIKeys.pollfishAPIKey , Position.BOTTOM_RIGHT, 0);
                
        //Load the settings shared preferences which deals with if we're resuming from pause or not
        SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);

        //Load the regular preferences into JNI
        Preferences.loadPreferences();

        //Register the accelerometer listener
        myManager.registerListener(mySensorListener, accSensor, SensorManager.SENSOR_DELAY_GAME);
                
        //Set up the elements list
        nativeRefreshElements();
        Resources res = getResources();
        baseElementsList = res.getTextArray(R.array.elements_list);
        elementsList.clear();
                
        // Add the base elements
        for (int i = 0; i < baseElementsList.length; i++)
        {
            elementsList.add(baseElementsList[i].toString());
        }
                
        // Load the custom elements list
        try
        {
            CustomElementManager.refresh(getApplicationContext());
            for (CustomElement c : CustomElementManager.getElementList())
            {
                elementsList.add(c.getName());
            }
            baseElementsList = elementsList.toArray(new CharSequence[elementsList.size()]);
        }
        //Catch any exceptions
        catch (Exception e)
        {
            Log.e("TheElements", "Error: " + e.getMessage());
        }
        
        // Refresh elements list
        if (mElementAdapter != null) {
        	mElementAdapter.notifyDataSetChanged();
        }

        //Set up the file manager for saving and loading
        FileManager.intialize(this);

        //If we're resuming from a pause (not when it starts)
        if (settings.getBoolean("paused", false))
        {
            //Set the preferences to indicate unpaused
            SharedPreferences.Editor editor = settings.edit();
            editor.putBoolean("paused", false);
            editor.commit();
        }
        else if (settings.getBoolean("firstrun", true))
        {
            //Indicate that the demo should be loaded by nativeLoadState()
            shouldLoadDemo = true;
            //Unset firstrun
            SharedPreferences.Editor editor = settings.edit();
            editor.putBoolean("firstrun", false);
            editor.putLong("date_firstlaunch", System.currentTimeMillis());
            editor.commit();

            //Also show the intro message
            showDialog(INTRO_MESSAGE);
                        
            //Finally, delete the temp save, in case there were save format changes
            SaveManager.deleteState("temp");
        }
        
        // Should we prompt for a rating?
        SharedPreferences.Editor editor = settings.edit();
        long launchCount = settings.getLong("launch_count", 0) + 1;
        editor.putLong("launch_count", launchCount);

        long dateFirstLaunch = settings.getLong("date_firstlaunch", System.currentTimeMillis());
        // Write back date first launch in case we missed the first run and didn't
        // set the property properly.
        editor.putLong("date_firstlaunch", dateFirstLaunch);
        boolean madeAnElement = settings.getBoolean("made_element", false);
        long age = System.currentTimeMillis() - dateFirstLaunch;
        boolean shownRatePrompt = settings.getBoolean("shown_rate_prompt", false);
        if (age > 3 * 24 * 60 * 60 * 1000 && // 3 days -> millis
            launchCount > 5 &&
            madeAnElement &&
            !shownRatePrompt)
        {
            FlurryAgent.logEvent("shown_rate_prompt");
            editor.putBoolean("shown_rate_prompt", true);
            new Handler().postDelayed(new Runnable() {
               @Override
               public void run() {
                   showDialog(RATE_PROMPT);
               }
            }, 5000);
        }
        editor.commit();
        
        //Set the activity for Control so that we can call showDialog() from it
        control.setActivity(this);

        //Call onResume() for view too
        // Log.v("TheElements", "sand_view.onResume()");
        sand_view.onResume();
        // Log.v("TheElements", "sand_view.onResume() done");
    }

    protected Dialog onCreateDialog(int id) //This is called when showDialog is called
    {
        if (id == INTRO_MESSAGE) // The first dialog - the intro message
        {
            AlertDialog.Builder builder = new AlertDialog.Builder(this);
            WebView wv = new WebView(this);
            wv.loadData(getResources().getString(R.string.app_intro), "text/html", "utf-8");
            wv.setBackgroundColor(Color.BLACK);
            builder.setView(wv).setCancelable(false).setPositiveButton(R.string.proceed, new DialogInterface.OnClickListener()
            {
                public void onClick(DialogInterface dialog, int id)
                {
                    dialog.cancel();
                }
            });
            return builder.create();
        }
        else if (id == ELEMENT_PICKER) // Element picker
        {
            AlertDialog.Builder builder = new AlertDialog.Builder(this); // Create a new one

            mElementAdapter = new ElementAdapter(this, elementsList);

            builder.setTitle(R.string.element_picker); // Set the title
            builder.setOnCancelListener(this);
            builder.setAdapter(mElementAdapter, new OnClickListener() {
                public void onClick(DialogInterface dialog, int item)
                {
                	//lol this code is kinda bad, these methods probably shouldn't be static  
                    if (Control.eraserOn)
                    {
                        Control.setEraserOff();
                    }
                    setElement((char) (item + NORMAL_ELEMENT));
                    setPlaying(play);
                    dialog.dismiss();
                }
            });

            return builder.create();
        }
        else if (id == BRUSH_SIZE_PICKER)
        {
            AlertDialog.Builder builder = new AlertDialog.Builder(this); // Declare the object
            builder.setTitle(R.string.brush_size_picker);
            builder.setOnCancelListener(this);
            builder.setItems(R.array.brush_size_list, new DialogInterface.OnClickListener()
            {
                public void onClick(DialogInterface dialog, int item)
                {
                    if (item == 0)
                    {
                        setBrushSize((char) 0);
                    }
                    else
                    {
                        setBrushSize((char) java.lang.Math.pow(2, item - 1));
                    }
                    setPlaying(play);
                }
            });
            return builder.create();
        }
        else if (id == RATE_PROMPT)
        {
            final Dialog rateDialog = new Dialog(this);
            Window window = rateDialog.getWindow();
            window.requestFeature(Window.FEATURE_NO_TITLE);
            window.setContentView(R.layout.rate_dialog);
            // Make this dialog not focused, so it doesn't interrupt play
            window.setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
                    WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL);
            window.clearFlags(WindowManager.LayoutParams.FLAG_DIM_BEHIND);
            window.setLayout(ViewGroup.LayoutParams.MATCH_PARENT,
                    ViewGroup.LayoutParams.WRAP_CONTENT);
            // Plain gray background without margin
            window.setBackgroundDrawableResource(R.drawable.dialog_background);
            // Bump this dialog below the menu bar
            window.setGravity(Gravity.TOP);
            WindowManager.LayoutParams params = window.getAttributes();
            // Leave a 45dip margin on top for the menu bar
            params.y = (int) (getResources().getDisplayMetrics().density * 45 + 0.5f);
            params.windowAnimations = R.style.RateDialogAnimations;
            window.setAttributes(params);

            // Dialog content
            TextView tv = (TextView)rateDialog.findViewById(R.id.message);
            tv.setText(R.string.rate_prompt);
            Button positiveButton = (Button)rateDialog.findViewById(R.id.button1);
            positiveButton.setText("Rate");
            positiveButton.setOnClickListener(new Button.OnClickListener() {
                @Override
                public void onClick(View v) {
                    try {
                        MainActivity.this.startActivity(new Intent(
                            Intent.ACTION_VIEW, Uri.parse(
                            "market://details?id=com.idkjava.thelements")));
                    }
                    catch (ActivityNotFoundException e) {
                        Log.e("TheElements", "No google play found");
                        Toast.makeText(MainActivity.this,
                                R.string.couldnt_launch_market, Toast.LENGTH_LONG).show();
                    }
                    rateDialog.dismiss();
                }
            });
            Button neutralButton = (Button)rateDialog.findViewById(R.id.button2);
            neutralButton.setText("Feedback");
            neutralButton.setOnClickListener(new Button.OnClickListener() {
               @Override
               public void onClick(View v) {
                   MainActivity.this.startActivity(new Intent(
                               Intent.ACTION_SENDTO, Uri.parse(
                               "mailto:idkjava@gmail.com")));
                   rateDialog.dismiss();
               }
            });
            Button negativeButton = (Button)rateDialog.findViewById(R.id.button3);
            negativeButton.setText("Not now");
            negativeButton.setOnClickListener(new Button.OnClickListener() {
                @Override
                public void onClick(View v) {
                    rateDialog.dismiss();
                }
            });
            return rateDialog;
        }

        return null; //Default case: return nothing
    }

    // Used by the Element Picker and Brush Size Picker dialogs to reset play state
    // if the dialog is cancelled, instead of a selection being made.
    @Override
    public void onCancel(DialogInterface dialog)
    {
        // Reset native play state to whatever the ui believes the play state is
        setPlaying(play);
    }

    public boolean onPrepareOptionsMenu(Menu menu) // Pops up when you press Menu
    {
        // Create an inflater to inflate the menu already defined in res/menu/options_menu.xml
        // This seems to be a bit faster at loading the menu, and easier to modify
        MenuInflater inflater = getMenuInflater();
        menu.clear();
        inflater.inflate(R.menu.options_menu_small, menu);

        return true;
    }

    public boolean onOptionsItemSelected(MenuItem item)
    {
        switch (item.getItemId())
        {
        case R.id.element_picker:
        {
            setPlaying(false);
            showDialog(ELEMENT_PICKER);
            return true;
        }
        case R.id.brush_size_picker:
        {
            setPlaying(false);
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
            setPlaying(play);
            return true;
        }
        case R.id.eraser:
        {
            setElement(ERASER_ELEMENT);
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
        case R.id.custom_element_editor:
        {
            startActivity(new Intent(MainActivity.this, CustomElementManagerActivity.class));
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

    //Set up the views based on the state of ui
    private void setUpViews()
    {
        nativeInit();
                
        //Set the content view based on this variable
        setContentView(R.layout.main_activity_ui);

        //Set the new view and control box and menu bar to the stuff defined in layout
        menu_bar = (MenuBar) findViewById(R.id.menu_bar);
        sand_view = (SandView) findViewById(R.id.sand_view);
        control = (Control) findViewById(R.id.control);

        menu_bar.setSandView(sand_view);

        //Set the screen state for sand_view now that it's defined
        Preferences.loadScreenState();
    }

    //Trigger the SaveStateActivity
    public void saveState()
    {
        Intent tempIntent = new Intent(this, SaveStateActivity.class);
        startActivity(tempIntent);
    }

    //Trigger the LoadStateActivity
    public void loadState()
    {
        Intent tempIntent = new Intent(this, LoadStateActivity.class);
        startActivity(tempIntent);
    }
        
        
    /**
     * Definition of the list adapter...uses the View Holder pattern to
     * optimize performance.
     */
    private static class ElementAdapter extends ArrayAdapter {

        private static final int RESOURCE = R.layout.row;
        private LayoutInflater inflater;

        static class ViewHolder {
            TextView nameTxVw;
        }

        public ElementAdapter(Context context, ArrayList<String> elements)
        {
            super(context, RESOURCE, elements);
            inflater = LayoutInflater.from(context);
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent)
        {
            ViewHolder holder;

            if ( convertView == null ) {
                // inflate a new view and setup the view holder for future use
                convertView = inflater.inflate( RESOURCE, null );

                holder = new ViewHolder();
                holder.nameTxVw =
                    (TextView) convertView.findViewById(R.id.elementname);
                convertView.setTag( holder );
            }  else {
                // view already defined, retrieve view holder
                holder = (ViewHolder) convertView.getTag();
            }

            String name = (String) getItem(position);
            int realElementPosition = position + NORMAL_ELEMENT;
            holder.nameTxVw.setText(name);
            int theColor = Color.rgb(getElementRed(realElementPosition), 
                                     getElementGreen(realElementPosition), getElementBlue(realElementPosition));
            ColorDrawable elementColor = new ColorDrawable(theColor);
                
            elementColor.setBounds(0, 0, toPx(COLOR_SQUARE_SIZE), toPx(COLOR_SQUARE_SIZE));
            holder.nameTxVw.setCompoundDrawables( elementColor, null, null, null );

            return convertView;
        }
    }

    //Converts dp to pixels
    public static int toPx(int dp) {
        return (int)((dp*mDPI)/160f);
    }

    public static void setPlaying(boolean playState) {
        if ( Kamcord.isPaused() || Kamcord.isRecording()) {
            if (playState) {
                Kamcord.resumeRecording();
            }
            else {
                Kamcord.pauseRecording();
            }
        }
        setPlayState(playState);
    }

    //@formatter:off
    //JNI Functions
    //Save/load functions
    public static native char saveTempState();
    public static native char loadDemoState();
        
    //General utility functions
    private static native void nativeInit();
    private static native void nativeRefreshElements();
    public native void clearScreen();
        
    //Setters
    public static native void setPlayState(boolean playState);
    public static native void setElement(char element);
    public static native void setBrushSize(char brushSize);
    /**
     * Sets a filter rendering mode
     * @param mode 0 - none
     *              1 - motion blur
     */
    public static native void setFilterMode(char mode);

        
    //Getters
    public static native char getElement();
    public static native byte[] getElementInfo(int index);
    public static native int getElementRed(int index);
    public static native int getElementGreen(int index);
    public static native int getElementBlue(int index);
        
    //Accelerometer related
    public static native void setXGravity(float xGravity);
    public static native void setYGravity(float yGravity);
    //@formatter:on

    static
    {
        System.loadLibrary("stlport_shared");
    	try {
    		System.loadLibrary("kamcord");
    	} catch (UnsatisfiedLinkError e) {
    		Log.d("TheElements", "Kamcord not supported");
    	}
    	System.loadLibrary("protobuf");
        System.loadLibrary("thelements"); // Load the JNI library (libthelements.so)
    }
}
