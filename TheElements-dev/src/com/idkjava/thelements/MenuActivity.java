package com.idkjava.thelements;


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.webkit.WebView;
import android.widget.Button;

import com.flurry.android.FlurryAgent;
import com.idkjava.thelements.custom.CustomElementManagerActivity;
import com.idkjava.thelements.money.ProductManager;
import com.idkjava.thelements.preferences.PreferencesActivity;
import com.winsontan520.wversionmanager.library.WVersionManager;
import com.winsontan520.wversionmanager.library.WVersionManager.WVersionManagerEventListener;

public class MenuActivity extends ReportingActivity {
    public static Button start_game_button;
    public static Button custom_elements_button;
    public static Button store_button;
    public static Button preferences_button;
    public static Button fix_me_button;
    public static Button how_to_play_button;
    public static Button about_button;
    public static boolean loaded = false;

    /** The url to look for the latest update **/
    public static final String update_version_url = "https://raw.githubusercontent.com/IDKJava/the-elements/master/TheElements-dev/assets/latest_version.json";
    public static final int reminder_minutes = 3 * 60 * 24;

    /** 3 days **/

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState); // Call the super method

        requestWindowFeature(Window.FEATURE_NO_TITLE); // Get rid of title bar

        setContentView(R.layout.menu_activity);

        // Define all the objects
        start_game_button = (Button) findViewById(R.id.start_game_button);
        custom_elements_button = (Button) findViewById(R.id.custom_elements_button);
        store_button = (Button) findViewById(R.id.unlock_features_button);
        preferences_button = (Button) findViewById(R.id.preferences_button);
        fix_me_button = (Button) findViewById(R.id.fix_me_button);
        how_to_play_button = (Button) findViewById(R.id.how_to_play_button);
        about_button = (Button) findViewById(R.id.about_button);

        start_game_button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                FlurryAgent.logEvent("Start game button (main menu)");
                // Start the main app activity
                startActivity(new Intent(MenuActivity.this, SplashActivity.class));
            }
        });

        custom_elements_button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                FlurryAgent.logEvent("Custom elements button (main menu)");
                // Start the CustomElementManagerActivity
                startActivity(new Intent(MenuActivity.this, CustomElementManagerActivity.class));
            }
        });

        store_button.setOnClickListener(new OnClickListener() {
           @Override
            public void onClick(View v) {
               FlurryAgent.logEvent("Store button (main menu)");
               startActivity(new Intent(MenuActivity.this, PurchaseActivity.class));
           }
        });

        preferences_button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(MenuActivity.this, PreferencesActivity.class));
            }
        });

        fix_me_button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                FlurryAgent.logEvent("Fix me button (main menu)");
                startActivity(new Intent(MenuActivity.this, FixMeActivity.class));
            }
        });

        how_to_play_button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // Show the instructions
                how_to_play();
            }
        });

        about_button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // Show the about dialog
                showDialog(1);
            }
        });

        WVersionManager versionManager = new WVersionManager(this);

        Resources r = getResources();

        versionManager.setVersionContentUrl(update_version_url);
        versionManager.setUpdateNowLabel(r.getString(R.string.update_now));
        versionManager.setRemindMeLaterLabel(r.getString(R.string.remind_later));
        versionManager.setIgnoreThisVersionLabel(r.getString(R.string.skip_version));
        versionManager.setReminderTimer(reminder_minutes);
        versionManager.setEventListener(new WVersionManagerEventListener() {

            @Override
            public void onUpdateNowClicked() {
                FlurryAgent.logEvent("Update Now Clicked");
            }

            @Override
            public void onSkipVersionClicked() {
                FlurryAgent.logEvent("Skip Version Clicked");
            }

            @Override
            public void onRemindLaterClicked() {
                FlurryAgent.logEvent("Remind Later Clicked");
            }
        });

        versionManager.checkVersion();

    }

    public void how_to_play() {
        StringBuffer data = new StringBuffer();
        try {
            InputStream stream = getResources().openRawResource(R.raw.instructions);
            BufferedReader in = new BufferedReader(new InputStreamReader(stream), 8192);
            while (true) {
                String line = in.readLine();
                if (line == null) {
                    break;
                }
                data.append(line).append("\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }

        WebView how_to_play = new WebView(this);
        how_to_play.setBackgroundColor(0x00000000);
        how_to_play.loadData(data.toString(), "text/html; charset=utf-8", "utf-8");

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setCancelable(true);
        Resources res = getResources();
        builder.setTitle(res.getText(R.string.how_to_play_title));
        builder.setView(how_to_play);
        builder.show();
    }

    protected Dialog onCreateDialog(int id) // This is called when showDialog is
                                            // called
    {
        if (id == 1) // Copyright message
        {
            AlertDialog.Builder builder = new AlertDialog.Builder(this); // Declare
                                                                         // the
                                                                         // object
            Resources res = getResources();
            builder.setMessage(res.getText(R.string.about_text));
            AlertDialog alert = builder.create(); // Create object
            return alert; // Return handle
        }

        return null; // No need to return anything, just formality
    }
}
