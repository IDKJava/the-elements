package com.idkjava.thelements.money;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.sax.Element;
import android.util.Log;

import androidx.annotation.NonNull;

import com.android.billingclient.api.AcknowledgePurchaseParams;
import com.android.billingclient.api.AcknowledgePurchaseResponseListener;
import com.android.billingclient.api.BillingClient;
import com.android.billingclient.api.BillingClient.BillingResponseCode;
import com.android.billingclient.api.BillingClientStateListener;
import com.android.billingclient.api.BillingFlowParams;
import com.android.billingclient.api.BillingResult;
import com.android.billingclient.api.Purchase;
import com.android.billingclient.api.PurchasesResponseListener;
import com.android.billingclient.api.PurchasesUpdatedListener;

import com.android.billingclient.api.SkuDetails;
import com.android.billingclient.api.SkuDetailsParams;
import com.android.billingclient.api.SkuDetailsResponseListener;
import com.idkjava.thelements.BuildConfig;
import com.idkjava.thelements.ElementsApplication;
import com.idkjava.thelements.R;
import com.idkjava.thelements.error.ErrorHandler;
import com.idkjava.thelements.keys.APIKeys;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

/**
 * Class to manage Google Play billing services: initialization, purchasing in-game items,
 * and storing knowledge of in-game purchased settings (currently very insecure).
 * TODO: ProductManager should maintain a queue of async activities to be performed, rather
 * than the currently janky sleeper thread method.
 */
public class ProductManager implements PurchasesUpdatedListener, AcknowledgePurchaseResponseListener {
    public static String SKU_GRAVITY_PACK = "gravity_pack"; // includes all gravity tools
    public static String SKU_TOOL_PACK = "tool_pack"; // includes a large set of extra tools
    public static String SKU_CAMERA_TOOL = "camera_tool"; // a tool to convert pictures to sand
    public static String SKU_PORTAL_TOOL = "portals"; // a tool to make portals!
    public static ArrayList<String> ALL_SKUS = new ArrayList<String>(Arrays.asList(
            SKU_GRAVITY_PACK,
            SKU_TOOL_PACK,
            SKU_CAMERA_TOOL,
            SKU_PORTAL_TOOL
    ));
    public Map<String, SkuDetails> mSkuDetails = new HashMap<String, SkuDetails>();

    public String getStr(int resId) {
        return mCtx.getResources().getString(resId);
    }

    public ProductManager(Context ctx, SharedPreferences prefs) {
        // Only ever use the application context, because this transcends
        // Activity boundaries
        mCtx = ctx.getApplicationContext();
        mPrefs = prefs;
        // APIKeys.googlePlayPublicKey
        mInPurchase = false;
        mBillingSupported = true;
//        mExec = Executors.newSingleThreadExecutor();
//        mExec.execute(new Runnable() {
//            @Override
//            public void run() {
//                Log.d("TheElements", "Product manager starting helper setup");
//                mInSetup = true;
//                mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
//                    @Override
//                    public void onIabSetupFinished(IabResult result) {
//                        Log.d("TheElements", "Helper setup complete.");
//                        mInSetup = false;
//                        if (!result.isSuccess()) {
//                            mBillingSupported = false;
//                            makeError(getStr(R.string.iab_setup_failed) + result);
//                            return;
//                        }
//                        mBillingSupported = true;
//
//                        if (mHelper == null) return;
//                        refreshInventory(null, null);
//                    }
//                });
//                while (mInSetup) {
//                    try {
//                        Thread.sleep(500);
//                    }
//                    catch (InterruptedException e) {}
//                }
//            }
//        });

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

    public boolean grantEntitlement(String sku) {
        if (!ALL_SKUS.contains(sku)) return false;

        SharedPreferences.Editor editor = mPrefs.edit();
        editor.putBoolean(sku, true);
        editor.commit();
        return true;
    }

    @Override
    public void onAcknowledgePurchaseResponse(BillingResult billingResult) {
        // TODO: Handle this?
    }

    @Override
    public void onPurchasesUpdated(BillingResult billingResult, List<Purchase> purchases) {
        if (billingResult.getResponseCode() == BillingResponseCode.OK &&
            purchases != null) {
            for (Purchase purchase : purchases) {
                int state = purchase.getPurchaseState();
                if (state == Purchase.PurchaseState.PURCHASED) {
                    if (!purchase.isAcknowledged()) {
                        // TODO: Purchase verification using a payload system?
                        boolean success = true;
                        for (String sku : purchase.getSkus()) {
                            if (!grantEntitlement(sku)) {
                                success = false;
                                break;
                            }
                        }
                        if (!success) continue;

                        AcknowledgePurchaseParams acknowledgePurchaseParams =
                                AcknowledgePurchaseParams.newBuilder()
                                        .setPurchaseToken(purchase.getPurchaseToken())
                                        .build();
                        ElementsApplication.sBilling.acknowledgePurchase(acknowledgePurchaseParams, this);
                    }
                } else { // unsupported purchase state
                    Log.d("TheElements", "Got purchase in unsupported state: " + purchase);
                }
            }
        }
        else {
            handleBillingResponse(billingResult.getResponseCode());
        }
    }

    private void handleSkuDetails(List<SkuDetails> skuDetails) {
        Log.d("TheElements", "handleSkuDetails");
        // Add all sku details to the map
        for (SkuDetails details : skuDetails) {
            Log.d("TheElements", "... checking SKU " + details.getSku());
            if (!ALL_SKUS.contains(details.getSku())) continue;
            Log.d("TheElements", "... in list, adding details.");
            mSkuDetails.put(details.getSku(), details);
        }
    }

//    private IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
//        @Override
//        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
//            if (mHelper == null) return;
//            if (result.isFailure()) {
//                makeError(getStr(R.string.iab_inventory_failed) + result);
//                return;
//            }
//
//            // Add all sku details to the map
//            for (String sku : ALL_SKUS) {
//                mSkuDetails.put(sku, inventory.getSkuDetails(sku));
//            }
//
//            // Check items and resolve purchases into in-app settings
//            // TODO: Use a more secure item tracking method in the future
//
//            // Gravity pack
//            Purchase gravityPurchase = inventory.getPurchase(SKU_GRAVITY_PACK);
//            if (gravityPurchase != null && verify(gravityPurchase)) {
//                Log.d("TheElements", "Gravity pack owned: " + gravityPurchase.getPurchaseState());
//                SharedPreferences.Editor editor = mPrefs.edit();
//                editor.putBoolean(SKU_GRAVITY_PACK, true);
//                editor.commit();
//            }
//            else {
//                SharedPreferences.Editor editor = mPrefs.edit();
//                editor.putBoolean(SKU_GRAVITY_PACK, false);
//                editor.commit();
//            }
//
//            // Tool pack
//            Purchase toolPurchase = inventory.getPurchase(SKU_TOOL_PACK);
//            if (toolPurchase != null && verify(toolPurchase)) {
//                Log.d("TheElements", "Tools pack owned: " + toolPurchase.getPurchaseState());
//                SharedPreferences.Editor editor = mPrefs.edit();
//                editor.putBoolean(SKU_TOOL_PACK, true);
//                editor.commit();
//            }
//            else {
//                SharedPreferences.Editor editor = mPrefs.edit();
//                editor.putBoolean(SKU_TOOL_PACK, false);
//                editor.commit();
//            }
//
//            Purchase cameraPurchase = inventory.getPurchase(SKU_CAMERA_TOOL);
//            if (cameraPurchase != null && verify(cameraPurchase)) {
//                Log.d("TheElements", "Camera pack owned: " + cameraPurchase.getPurchaseState());
//                SharedPreferences.Editor editor = mPrefs.edit();
//                editor.putBoolean(SKU_CAMERA_TOOL, true);
//                editor.commit();
//            }
//            else {
//                SharedPreferences.Editor editor = mPrefs.edit();
//                editor.putBoolean(SKU_CAMERA_TOOL, false);
//                editor.commit();
//            }
//
//            Purchase portalPurchase = inventory.getPurchase(SKU_PORTAL_TOOL);
//            if (portalPurchase != null && verify(portalPurchase)) {
//                Log.d("TheElements", "Portal tool owned: " + portalPurchase.getPurchaseState());
//                SharedPreferences.Editor editor = mPrefs.edit();
//                editor.putBoolean(SKU_PORTAL_TOOL, true);
//                editor.commit();
//            }
//            else {
//                SharedPreferences.Editor editor = mPrefs.edit();
//                editor.putBoolean(SKU_PORTAL_TOOL, false);
//                editor.commit();
//            }
//
//        }
//    };

//    private IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
//        @Override
//        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
//            if (mHelper == null) return;
//            if (result.isFailure()) {
//                makeError(getStr(R.string.iab_purchase_failed) + result);
//                return;
//            }
//            if (!verify(purchase)) {
//                makeError(getStr(R.string.iab_purchase_failed_auth));
//                return;
//            }
//
//            // Any one-time purchase
//            refreshInventory(null, null);
//        }
//    };

    public void tryStartBillingConnection() {
        ElementsApplication.sBilling.startConnection(new BillingClientStateListener() {
            @Override
            public void onBillingServiceDisconnected() {
                mBillingSupported = false;
            }

            @Override
            public void onBillingSetupFinished(@NonNull BillingResult billingResult) {
                if (billingResult.getResponseCode() == BillingResponseCode.OK) {
                    mBillingSupported = true;
                }
            }
        });
    }

    // TODO: Probably add a "stop billing connection" if we eventually tie this to an
    // activity lifecycle rather than the whole appliation.

    public void refreshInventory(final Activity act, final Runnable callback) {
        if (!mBillingSupported) {
            makeError(getStr(R.string.billing_not_supported));
            return;
        }
        SkuDetailsParams.Builder params = SkuDetailsParams.newBuilder();
        params.setSkusList(ALL_SKUS).setType(BillingClient.SkuType.INAPP);
        ElementsApplication.sBilling.querySkuDetailsAsync(params.build(),
                new SkuDetailsResponseListener() {
                    @Override
                    public void onSkuDetailsResponse(@NonNull BillingResult billingResult,
                                                     List<SkuDetails> skuDetailsList) {
                        Log.d("TheElements", "onSkuDetailsResponse, result: " + billingResult.toString());
                        if (billingResult.getResponseCode() == BillingResponseCode.OK &&
                            skuDetailsList != null) {
                            handleSkuDetails(skuDetailsList);
                            ElementsApplication.sBilling.queryPurchasesAsync(
                                    BillingClient.SkuType.INAPP,
                                    new PurchasesResponseListener() {
                                        @Override
                                        public void onQueryPurchasesResponse(BillingResult billingResult, List<Purchase> purchases) {
                                            ProductManager.this.onPurchasesUpdated(billingResult, purchases);
                                            if (act != null && callback != null) {
                                                act.runOnUiThread(callback);
                                            }
                                        }
                                    });
                        }
                        else {
                            Log.d("TheElements", "onSkuDetailsResponse bad billingResult");
                            handleBillingResponse(billingResult.getResponseCode());
                        }
                    }
                });
//        mExec.execute(new Runnable() {
//            @Override
//            public void run() {
//                try {
//                    Inventory i = mHelper.queryInventory(true, ALL_SKUS);
//                    IabResult success = new IabResult(
//                            IabHelper.BILLING_RESPONSE_RESULT_OK,
//                            getStr(R.string.iab_inventory_success));
//                    mGotInventoryListener.onQueryInventoryFinished(success, i);
//                }
//                catch (IabException e) {}
//                if (act != null && callback != null) {
//                    act.runOnUiThread(callback);
//                }
//            }
//        });
    }

    public void handleBillingResponse(int code) {
        if (code == BillingResponseCode.OK) {
            return;
        }
        else if (code == BillingResponseCode.BILLING_UNAVAILABLE ||
                 code == BillingResponseCode.SERVICE_DISCONNECTED ||
                 code == BillingResponseCode.SERVICE_TIMEOUT ||
                 code == BillingResponseCode.SERVICE_UNAVAILABLE) {
            makeError(getStr(R.string.billing_unavailable));
        }
        else if (code == BillingResponseCode.DEVELOPER_ERROR ||
                 code == BillingResponseCode.ERROR ||
                 code == BillingResponseCode.FEATURE_NOT_SUPPORTED) {
            makeError(getStr(R.string.billing_error));
        }
        else if (code == BillingResponseCode.ITEM_ALREADY_OWNED) {
            makeError(getStr(R.string.billing_item_owned));
        }
        else if (code == BillingResponseCode.ITEM_UNAVAILABLE) {
            makeError(getStr(R.string.billing_item_unavailable));
        }
        else if (code == BillingResponseCode.USER_CANCELED) {
            makeError(getStr(R.string.billing_user_cancelled));
        }
        else {
            makeError(getStr(R.string.billing_error));
        }
    }

    public void launchPurchase(Activity act, String sku) {
        if (!mBillingSupported) {
            makeError(getStr(R.string.billing_not_supported));
            return;
        }
        if (!mSkuDetails.containsKey(sku)) {
            makeError(getStr(R.string.billing_product_not_supported));
            return;
        }
//        final Activity threadAct = act;
//        final String threadSku = sku;
        Log.d("TheElements", "launchPurchase");
        BillingFlowParams billingFlowParams = BillingFlowParams.newBuilder()
                .setSkuDetails(Objects.requireNonNull(mSkuDetails.get(sku)))
                .build();
        int responseCode = ElementsApplication.sBilling.launchBillingFlow(act, billingFlowParams).getResponseCode();
        handleBillingResponse(responseCode);
//        mExec.execute(new Runnable() {
//            @Override
//            public void run() {
//                mInPurchase = true;
//                // Arbitrary request code
//                int rc = 10001;
//                mHelper.launchPurchaseFlow(threadAct, threadSku, rc, mPurchaseFinishedListener);
//                while (mInPurchase) {
//                    try {
//                        Thread.sleep(500);
//                    }
//                    catch (InterruptedException e) {}
//                }
//            }
//        });
    }

//    public boolean handleActivityResult(int requestCode, int resultCode, Intent data) {
//        // Pass through to IabHelper
//        Log.d("TheElements", "handleActivityResult");
//        boolean out = mHelper.handleActivityResult(requestCode, resultCode, data);
//        mInPurchase = false;
//        return out;
//    }

    public String getPrice(String sku) {
        SkuDetails details = mSkuDetails.get(sku);
        if (details == null) {
            return "";
        }
        else {
            return details.getPrice();
        }
    }

    // IMPORTANT: Call this before the wrapping activity closes, to avoid circular references
    // causing leaked objects.
    public void cleanUp() {
        mCtx = null;
        mPrefs = null;
    }

    // TODO: Deprecate and remove string literal error messages. Move to
    // strings.xml instead.
    private void makeError(String msg) {
        Log.e("TheElements", msg);
        if (mHandler != null) {
            mHandler.error(msg);
        }
    }

    private boolean mInPurchase;
    private boolean mInSetup;
    private boolean mBillingSupported;
    private Context mCtx;
    private SharedPreferences mPrefs;
    private ErrorHandler mHandler;
    private Executor mExec;
}
