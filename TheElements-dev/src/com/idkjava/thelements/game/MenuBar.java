package com.idkjava.thelements.game;

import java.util.Hashtable;

import android.app.Activity;
import android.content.Context;
import android.graphics.drawable.Drawable;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
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
    private View mToolButton;
    private ImageView mToolButtonIcon;
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

    public void setToolIcon(int toolIcon) {
        mToolButtonIcon.setImageResource(toolIcon);
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
        mToolButton = findViewById(R.id.tool_button);
        mToolButtonIcon = (ImageView) findViewById(R.id.tool_button_icon);
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
    }
}
