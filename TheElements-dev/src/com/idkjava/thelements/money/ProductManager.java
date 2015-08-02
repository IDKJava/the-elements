package com.idkjava.thelements.money;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;

import com.idkjava.thelements.BuildConfig;
import com.idkjava.thelements.error.ErrorHandler;
import com.idkjava.thelements.iab.IabHelper;
import com.idkjava.thelements.iab.IabResult;
import com.idkjava.thelements.iab.Inventory;
import com.idkjava.thelements.iab.Purchase;
import com.idkjava.thelements.keys.APIKeys;

/**
 * Class to manage Google Play billing services: initialization, purchasing in-game items,
 * and storing knowledge of in-game purchased settings (currently very insecure).
 * TODO: ProductManager should maintain a queue of async activities to be performed, rather
 * than the currently janky sleeper thread method.
 */
public class ProductManager {
    public static String SKU_GRAVITY_PACK = "gravity_pack"; // includes all gravity tools
    public static String SKU_GRAVITY_BH = "gravity_black_hole"; // black hole
    public static String SKU_GRAVITY_WH = "gravity_white_hole"; // white hole
    public static String SKU_GRAVITY_CH = "gravity_whirlpool"; // whirlpool
    public static String SKU_GRAVITY_NG = "gravity_null_gravity"; // null gravity

    public ProductManager(Context ctx, SharedPreferences prefs) {
        // Only ever use the application context, because this transcends
        // Activity boundaries
        mCtx = ctx.getApplicationContext();
        mPrefs = prefs;
        mHelper = new IabHelper(mCtx, APIKeys.googlePlayPublicKey);
        mHelper.enableDebugLogging(BuildConfig.DEBUG);
        mSetup = false;
        Log.d("TheElements", "Product manager starting helper setup");
        mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
            @Override
            public void onIabSetupFinished(IabResult result) {
                Log.d("TheElements", "Helper setup complete.");
                if (!result.isSuccess()) {
                    makeError("Failed to set up in-app billing: " + result);
                    return;
                }

                if (mHelper == null) return;
                mHelper.queryInventoryAsync(mGotInventoryListener);
            }
        });
    }

    public void bindErrorHandler(ErrorHandler handler) {
        mHandler = handler;
    }
    // This method must be called as the relevant error handler goes out of
    // scope. I.e. for an activity handler, which launches a dialog, when the
    // activity is destroyed the handler should be unbound.
    public void unbindErrorHandler() {
        mHandler = null;
    }

    private IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
        @Override
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
            if (mHelper == null) return;
            if (result.isFailure()) {
                makeError("Failed to get in-app inventory: " + result);
                return;
            }

            mSetup = true;

            // Check items and resolve purchases into in-app settings
            // TODO: Use a more secure item tracking method in the future

            // Gravity pack
            Purchase gravityPurchase = inventory.getPurchase(SKU_GRAVITY_PACK);
            if (gravityPurchase != null && verify(gravityPurchase)) {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_GRAVITY_BH, true);
                editor.putBoolean(SKU_GRAVITY_CH, true);
                editor.putBoolean(SKU_GRAVITY_NG, true);
                editor.putBoolean(SKU_GRAVITY_WH, true);
                editor.commit();
            }

            // Black hole
            Purchase gravityBHPurchase = inventory.getPurchase(SKU_GRAVITY_BH);
            if (gravityBHPurchase != null && verify(gravityBHPurchase)) {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_GRAVITY_BH, true);
                editor.commit();
            }

            // White hole
            Purchase gravityWHPurchase = inventory.getPurchase(SKU_GRAVITY_WH);
            if (gravityWHPurchase != null && verify(gravityWHPurchase)) {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_GRAVITY_WH, true);
                editor.commit();
            }

            // Curl hole
            Purchase gravityCHPurchase = inventory.getPurchase(SKU_GRAVITY_CH);
            if (gravityCHPurchase != null && verify(gravityCHPurchase)) {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_GRAVITY_CH, true);
                editor.commit();
            }

            // Null gravity
            Purchase gravityNGPurchase = inventory.getPurchase(SKU_GRAVITY_NG);
            if (gravityNGPurchase != null && verify(gravityNGPurchase)) {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_GRAVITY_NG, true);
                editor.commit();
            }
        }
    };

    private IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        @Override
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            if (mHelper == null) return;
            if (result.isFailure()) {
                makeError("Error purchasing: " + result);
                return;
            }
            if (!verify(purchase)) {
                makeError("Error purchasing, auth failure.");
                return;
            }

            // Any one-time purchase
            if (purchase.getSku() == SKU_GRAVITY_BH ||
                purchase.getSku() == SKU_GRAVITY_CH ||
                purchase.getSku() == SKU_GRAVITY_NG ||
                purchase.getSku() == SKU_GRAVITY_PACK ||
                purchase.getSku() == SKU_GRAVITY_WH) {
                // Update inventory status asynchronously
                mHelper.queryInventoryAsync(mGotInventoryListener);
            }
            else {
                makeError("Error, unknown purchase type.");
            }
        }
    };

    private boolean verify(Purchase purchase) {
        // If state is cancelled, do not provide the feature
        boolean state = (purchase.getPurchaseState() != 1);

        // TODO: Implement a payload verification scheme
        boolean payload = true;
        return (state && payload);
    }

    public void launchPurchase(Activity act, String sku) {
        final Activity threadAct = act;
        final String threadSku = sku;
        Log.d("TheElements", "launchPurchase");
        new Thread() {
            @Override
            public void run() {
                // Arbitrary request code
                int rc = 10001;
                if (!waitSetup()) return;
                mHelper.launchPurchaseFlow(threadAct, threadSku, rc, mPurchaseFinishedListener);
            }
        }.start();
    }

    public boolean handleActivityResult(int requestCode, int resultCode, Intent data) {
        // Pass through to IabHelper
        Log.d("TheElements", "handleActivityResult");
        if (!waitSetup()) return false;
        return mHelper.handleActivityResult(requestCode, resultCode, data);
    }

    // IMPORTANT: Call this before the wrapping activity closes, to avoid circular references
    // causing leaked objects.
    public void cleanUp() {
        mCtx = null;
        mPrefs = null;
        if (mHelper != null) {
            mHelper.dispose();
        }
        mHelper = null;
    }

    private boolean waitSetup() {
        int tries = 0;
        while (!mSetup) {
            try {
                Thread.sleep(1000);
            }
            catch (InterruptedException e) {}
            tries++;
            if (tries > 30) {
                makeError("Waiting for purchase setup timed out.");
                return false;
            }
        }
        return true;
    }

    private void makeError(String msg) {
        Log.e("TheElements", msg);
        if (mHandler != null) {
            mHandler.error(msg);
        }
    }

    private IabHelper mHelper;
    private boolean mSetup;
    private Context mCtx;
    private SharedPreferences mPrefs;
    private ErrorHandler mHandler;
}
