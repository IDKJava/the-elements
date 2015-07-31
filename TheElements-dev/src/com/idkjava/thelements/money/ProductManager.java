package com.idkjava.thelements.money;

import android.app.AlertDialog;
import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;

import com.idkjava.thelements.BuildConfig;
import com.idkjava.thelements.iab.IabHelper;
import com.idkjava.thelements.iab.IabResult;
import com.idkjava.thelements.iab.Inventory;
import com.idkjava.thelements.iab.Purchase;
import com.idkjava.thelements.keys.APIKeys;

/**
 * Class to manage Google Play billing services: initialization, purchasing in-game items,
 * and storing knowledge of in-game purchased settings (currently very insecure).
 */
public class ProductManager {
    private String SKU_GRAVITY_PACK = "gravity_pack"; // includes all gravity tools
    private String SKU_GRAVITY_BH = "gravity_black_hole"; // black hole
    private String SKU_GRAVITY_WH = "gravity_white_hole"; // white hole
    private String SKU_GRAVITY_CH = "gravity_whirlpool"; // whirlpool
    private String SKU_GRAVITY_NG = "gravity_null_gravity"; // null gravity

    public ProductManager(Context ctx, SharedPreferences prefs) {
        mCtx = ctx;
        mPrefs = prefs;
        mHelper = new IabHelper(ctx, APIKeys.googlePlayPublicKey);
        mHelper.enableDebugLogging(BuildConfig.DEBUG);
        mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
            @Override
            public void onIabSetupFinished(IabResult result) {
                if (!result.isSuccess()) {
                    makeError("Failed to set up in-app billing: " + result);
                    return;
                }

                if (mHelper == null) return;
                mHelper.queryInventoryAsync(mGotInventoryListener);
            }
        });
    }

    private IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
        @Override
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
            if (mHelper == null) return;
            if (result.isFailure()) {
                makeError("Failed to get in-app inventory: " + result);
                return;
            }

            // Check items and resolve purchases into in-app settings
            // TODO: Use a more secure item tracking method in the future

            // Gravity pack
            Purchase gravityPurchase = inventory.getPurchase(SKU_GRAVITY_PACK);
            if (gravityPurchase != null && verifyPayload(gravityPurchase)) {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_GRAVITY_BH, true);
                editor.putBoolean(SKU_GRAVITY_CH, true);
                editor.putBoolean(SKU_GRAVITY_NG, true);
                editor.putBoolean(SKU_GRAVITY_WH, true);
                editor.commit();
            }

            // Black hole
            Purchase gravityBHPurchase = inventory.getPurchase(SKU_GRAVITY_BH);
            if (gravityBHPurchase != null && verifyPayload(gravityBHPurchase)) {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_GRAVITY_BH, true);
                editor.commit();
            }

            // White hole
            Purchase gravityWHPurchase = inventory.getPurchase(SKU_GRAVITY_WH);
            if (gravityWHPurchase != null && verifyPayload(gravityWHPurchase)) {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_GRAVITY_WH, true);
                editor.commit();
            }

            // Curl hole
            Purchase gravityCHPurchase = inventory.getPurchase(SKU_GRAVITY_CH);
            if (gravityCHPurchase != null && verifyPayload(gravityCHPurchase)) {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_GRAVITY_CH, true);
                editor.commit();
            }

            // Null gravity
            Purchase gravityNGPurchase = inventory.getPurchase(SKU_GRAVITY_NG);
            if (gravityNGPurchase != null && verifyPayload(gravityNGPurchase)) {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_GRAVITY_NG, true);
                editor.commit();
            }
        }
    };

    private boolean verifyPayload(Purchase purchase) {
        // TODO: Implement a payload verification scheme
        return true;
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

    private void makeError(String msg) {
        Log.e("TheElements", msg);
        alert(msg);
    }

    private void alert(String msg) {
        AlertDialog.Builder b = new AlertDialog.Builder(mCtx);
        b.setMessage(msg);
        b.setNeutralButton("OK", null);
        b.create().show();
    }

    private IabHelper mHelper;
    private Context mCtx;
    private SharedPreferences mPrefs;
}
