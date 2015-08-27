package com.idkjava.thelements.money;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.util.Log;

import com.idkjava.thelements.BuildConfig;
import com.idkjava.thelements.R;
import com.idkjava.thelements.error.ErrorHandler;
import com.idkjava.thelements.iab.IabException;
import com.idkjava.thelements.iab.IabHelper;
import com.idkjava.thelements.iab.IabResult;
import com.idkjava.thelements.iab.Inventory;
import com.idkjava.thelements.iab.Purchase;
import com.idkjava.thelements.keys.APIKeys;

import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

/**
 * Class to manage Google Play billing services: initialization, purchasing in-game items,
 * and storing knowledge of in-game purchased settings (currently very insecure).
 * TODO: ProductManager should maintain a queue of async activities to be performed, rather
 * than the currently janky sleeper thread method.
 */
public class ProductManager {
    public static String SKU_GRAVITY_PACK = "gravity_pack"; // includes all gravity tools
    public static String SKU_TOOL_PACK = "tool_pack"; // includes a large set of extra tools
    public static String SKU_CAMERA_TOOL = "camera_tool"; // a tool to convert pictures to sand

    public String getStr(int resId) {
        return mCtx.getResources().getString(resId);
    }

    public ProductManager(Context ctx, SharedPreferences prefs) {
        // Only ever use the application context, because this transcends
        // Activity boundaries
        mCtx = ctx.getApplicationContext();
        mPrefs = prefs;
        mHelper = new IabHelper(mCtx, APIKeys.googlePlayPublicKey);
        mHelper.enableDebugLogging(BuildConfig.DEBUG);
        mInPurchase = false;
        mExec = Executors.newSingleThreadExecutor();
        mExec.execute(new Runnable() {
            @Override
            public void run() {
                Log.d("TheElements", "Product manager starting helper setup");
                mInSetup = true;
                mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
                    @Override
                    public void onIabSetupFinished(IabResult result) {
                        Log.d("TheElements", "Helper setup complete.");
                        mInSetup = false;
                        if (!result.isSuccess()) {
                            mBillingSupported = false;
                            makeError(getStr(R.string.iab_setup_failed) + result);
                            return;
                        }
                        mBillingSupported = true;

                        if (mHelper == null) return;
                        refreshInventory(null, null);
                    }
                });
                while (mInSetup) {
                    try {
                        Thread.sleep(500);
                    }
                    catch (InterruptedException e) {}
                }
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
                makeError(getStr(R.string.iab_inventory_failed) + result);
                return;
            }

            // Check items and resolve purchases into in-app settings
            // TODO: Use a more secure item tracking method in the future

            // Gravity pack
            Purchase gravityPurchase = inventory.getPurchase(SKU_GRAVITY_PACK);
            if (gravityPurchase != null && verify(gravityPurchase)) {
                Log.d("TheElements", "Gravity pack owned: " + gravityPurchase.getPurchaseState());
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_GRAVITY_PACK, true);
                editor.commit();
            }
            else {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_GRAVITY_PACK, false);
                editor.commit();
            }

            // Tool pack
            Purchase toolPurchase = inventory.getPurchase(SKU_TOOL_PACK);
            if (toolPurchase != null && verify(toolPurchase)) {
                Log.d("TheElements", "Tools pack owned: " + toolPurchase.getPurchaseState());
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_TOOL_PACK, true);
                editor.commit();
            }
            else {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_TOOL_PACK, false);
                editor.commit();
            }

            Purchase cameraPurchase = inventory.getPurchase(SKU_CAMERA_TOOL);
            if (cameraPurchase != null && verify(cameraPurchase)) {
                Log.d("TheElements", "Camera pack owned: " + cameraPurchase.getPurchaseState());
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_CAMERA_TOOL, true);
                editor.commit();
            }
            else {
                SharedPreferences.Editor editor = mPrefs.edit();
                editor.putBoolean(SKU_CAMERA_TOOL, false);
                editor.commit();
            }
        }
    };

    private IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        @Override
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            if (mHelper == null) return;
            if (result.isFailure()) {
                makeError(getStr(R.string.iab_purchase_failed) + result);
                return;
            }
            if (!verify(purchase)) {
                makeError(getStr(R.string.iab_purchase_failed_auth));
                return;
            }

            // Any one-time purchase
            refreshInventory(null, null);
        }
    };

    private boolean verify(Purchase purchase) {
        // If state is cancelled, do not provide the feature
        boolean state = (purchase.getPurchaseState() != 1);

        // TODO: Implement a payload verification scheme
        boolean payload = true;
        return (state && payload);
    }

    public void refreshInventory(final Activity act, final Runnable callback) {
        if (!mBillingSupported) {
            makeError(getStr(R.string.billing_not_supported));
            return;
        }
        mExec.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    Inventory i = mHelper.queryInventory(false, null);
                    IabResult success = new IabResult(
                            IabHelper.BILLING_RESPONSE_RESULT_OK,
                            getStr(R.string.iab_inventory_success));
                    mGotInventoryListener.onQueryInventoryFinished(success, i);
                }
                catch (IabException e) {}
                if (act != null && callback != null) {
                    act.runOnUiThread(callback);
                }
            }
        });
    }

    public void launchPurchase(Activity act, String sku) {
        if (!mBillingSupported) {
            makeError(getStr(R.string.billing_not_supported));
            return;
        }
        final Activity threadAct = act;
        final String threadSku = sku;
        Log.d("TheElements", "launchPurchase");
        mExec.execute(new Runnable() {
            @Override
            public void run() {
                mInPurchase = true;
                // Arbitrary request code
                int rc = 10001;
                mHelper.launchPurchaseFlow(threadAct, threadSku, rc, mPurchaseFinishedListener);
                while (mInPurchase) {
                    try {
                        Thread.sleep(500);
                    }
                    catch (InterruptedException e) {}
                }
            }
        });
    }

    public boolean handleActivityResult(int requestCode, int resultCode, Intent data) {
        // Pass through to IabHelper
        Log.d("TheElements", "handleActivityResult");
        boolean out = mHelper.handleActivityResult(requestCode, resultCode, data);
        mInPurchase = false;
        return out;
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

    // TODO: Deprecate and remove string literal error messages. Move to
    // strings.xml instead.
    private void makeError(String msg) {
        Log.e("TheElements", msg);
        if (mHandler != null) {
            mHandler.error(msg);
        }
    }

    private IabHelper mHelper;
    private boolean mInPurchase;
    private boolean mInSetup;
    private boolean mBillingSupported;
    private Context mCtx;
    private SharedPreferences mPrefs;
    private ErrorHandler mHandler;
    private Executor mExec;
}
