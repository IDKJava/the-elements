package com.idkjava.thelements.game;

import java.util.Hashtable;

import android.app.Activity;
import android.content.Context;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.flurry.android.FlurryAgent;
import com.idkjava.thelements.MainActivity;
import com.idkjava.thelements.R;
import com.kamcord.android.Kamcord;

public class MenuBar extends LinearLayout {

    private Activity mAct;
    private SandView m_sandView = null;

    private ImageButton mPlayPauseButton;
    private Button mToolButton;
    private Button mUtilButton;
    private Button mRecordButton;

    // Constructor
    public MenuBar(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public void setActivity(Activity act) { mAct = act; }

    public void setSandView(SandView sandView) {
        m_sandView = sandView;
    }

    // Set the eraser to the off position
    public void setPlayState(boolean state) {

        if (state) {
            mPlayPauseButton.setImageResource(R.drawable.pause);
        } else {
            mPlayPauseButton.setImageResource(R.drawable.play);
        }
    }

    // Called when it's finished inflating the XML layout
    @Override
    protected void onFinishInflate() {

        mPlayPauseButton = (ImageButton) findViewById(R.id.play_pause_button);
        mToolButton = (Button) findViewById(R.id.tool_button);
        mUtilButton = (Button) findViewById(R.id.util_button);
        mRecordButton = (Button) findViewById(R.id.record_button);

        // Set up the OnClickListener for the play/pause button
        mPlayPauseButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                MainActivity.play = !MainActivity.play;
                MainActivity.setPlaying(MainActivity.play);

                if (MainActivity.play) {
                    mPlayPauseButton.setImageResource(R.drawable.pause);
                } else {
                    mPlayPauseButton.setImageResource(R.drawable.play);
                }
            }

        });
        if (MainActivity.play) {
            mPlayPauseButton.setImageResource(R.drawable.pause);
        } else {
            mPlayPauseButton.setImageResource(R.drawable.play);
        }

        mToolButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                mAct.showDialog(MainActivity.TOOL_PICKER);
            }
        });

        mUtilButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                mAct.showDialog(MainActivity.UTIL_DIALOG);
            }
        });

        mRecordButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                mAct.showDialog(MainActivity.RECORD_DIALOG);
            }
        });

        /*
        hand_button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if (hand_button.isSelected()) {
                    hand_button.setSelected(false);
                    m_sandView.setIsDragging(false);
                } else {
                    hand_button.setSelected(true);
                    m_sandView.setIsDragging(true);
                }
            }
        });

        // Set up the OnClickListener for the save button
        save_button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d("LOG",
                        "Checking sdcard state: "
                                + Environment.getExternalStorageState()
                                + " vs " + Environment.MEDIA_MOUNTED);
                if (Environment.getExternalStorageState().equals(
                        Environment.MEDIA_MOUNTED)) {
                    ((MainActivity) context).saveState();
                } else {
                    Toast.makeText(context, R.string.sdcard_not_found,
                            Toast.LENGTH_SHORT).show();
                }
            }
        });

        // Set up the OnClickListener for the load button
        load_button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if (Environment.getExternalStorageState().equals(
                        Environment.MEDIA_MOUNTED)) {
                    ((MainActivity) context).loadState();

                } else {
                    Toast.makeText(context, R.string.sdcard_not_found,
                            Toast.LENGTH_SHORT).show();
                }
            }
        });

        // Set up the OnClickListener for the exit button
        clear_button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                ((MainActivity) context).clearScreen();
            }
        });

        fade_button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if (fade_button.isSelected()) {
                    fade_button.setSelected(false);
                    MainActivity.setFilterMode((char) 0);
                    Hashtable<String, String> params = new Hashtable<String, String>();
                    FlurryAgent.logEvent("Kamcord disable fade", params);
                } else {
                    fade_button.setSelected(true);
                    MainActivity.setFilterMode((char) 1);
                    Hashtable<String, String> params = new Hashtable<String, String>();
                    FlurryAgent.logEvent("Kamcord enable fade", params);
                }
            }
        });
        if (!Kamcord.isWhitelisted()) {
            Hashtable<String, String> params = new Hashtable<String, String>();
            params.put("Device", android.os.Build.DEVICE);
            FlurryAgent.logEvent("Kamcord Disabled", params);
            kamcord_button.setVisibility(View.GONE);
            kamcord_view_button.setVisibility(View.GONE);
        } else {
            kamcord_button.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (kamcord_button.isSelected()) {
                        kamcord_button.setSelected(false);
                        Kamcord.stopRecording();
                        Kamcord.showView();
                        Hashtable<String, String> params = new Hashtable<String, String>();
                        params.put("Device", android.os.Build.DEVICE);
                        FlurryAgent.logEvent("Kamcord End Record", params);
                    } else {
                        kamcord_button.setSelected(true);
                        Kamcord.startRecording();
                        Hashtable<String, String> params = new Hashtable<String, String>();
                        params.put("Device", android.os.Build.DEVICE);
                        FlurryAgent.logEvent("Kamcord Start Record", params);
                    }
                }
            });
            kamcord_view_button.setOnClickListener(new OnClickListener() {
               @Override
               public void onClick(View v) {
                   Hashtable<String, String> params = new Hashtable<String, String>();
                   params.put("Device", android.os.Build.DEVICE);
                   FlurryAgent.logEvent("Kamcord Show View", params);
                   Kamcord.showWatchView();
               }
            });
        }
        */
    }
}
