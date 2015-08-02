package com.idkjava.thelements;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.idkjava.thelements.error.ActivityErrorHandler;
import com.idkjava.thelements.error.ErrorHandler;
import com.idkjava.thelements.money.ProductManager;


public class PurchaseActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.purchase_activity);

        // Create ErrorHandler for this activity
        ErrorHandler mHandler = new ActivityErrorHandler(this);

        // Loading intent info, with SKU
        Intent i = getIntent();
        mSku = i.getStringExtra("purchase_sku");

        // Update purchasing text
        TextView purchaseText = (TextView) findViewById(R.id.purchasing_text);
        purchaseText.setText("Purchasing " + mSku);

        SharedPreferences prefs = ElementsApplication.getPrefs();
        TextView gravityState = (TextView) findViewById(R.id.purchasing_state);
        gravityState.setText("Gravity state: " +
                prefs.getBoolean(ProductManager.SKU_GRAVITY_PACK, false));
        // Fire off the purchase workflow
        ProductManager mProductManager = ElementsApplication.getProductManager();
        mProductManager.bindErrorHandler(mHandler);
        mProductManager.launchPurchase(this, mSku);
    }

    @Override
    protected void onDestroy() {
        mHandler = null;
        if (mProductManager != null) {
            mProductManager.unbindErrorHandler();
        }
        mProductManager = null;
        super.onDestroy();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (mProductManager == null) return;

        // Need to forward on the activity result to the product manager
        if (!mProductManager.handleActivityResult(requestCode, resultCode, data)) {
            super.onActivityResult(requestCode, resultCode, data);
        }

        TextView purchaseText = (TextView) findViewById(R.id.purchasing_text);
        purchaseText.setText("Purchase complete: " + mSku);
    }

    private String mSku;
    private ProductManager mProductManager;
    private ErrorHandler mHandler;
}
