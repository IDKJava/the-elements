package com.idkjava.thelements.error;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Handler;
import android.os.Looper;

/**
 * Error handler using an activity to display an alert dialog.
 * Important: This object must be destroyed in the onDestroy method of
 * the activity to avoid leaking the Activity object.
 */
public class ActivityErrorHandler implements ErrorHandler {

    public ActivityErrorHandler(Activity act) {
        mAct = act;
    }

    @Override
    public void error(String message) {
        final String threadMsg = message;
        // We don't know whether we're being called from the UI thread
        // or not, so post the runnable to the Activity's UI thread.
        mAct.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AlertDialog.Builder b = new AlertDialog.Builder(mAct);
                b.setMessage(threadMsg);
                b.setNeutralButton("OK", null);
                b.create().show();
            }
        });
    }

    @Override
    public void error(int resId) {
        error(mAct.getResources().getString(resId));
    }

    @Override
    public void clear() {
        mAct = null;
    }

    private Activity mAct;
}
