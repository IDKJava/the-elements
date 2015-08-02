package com.idkjava.thelements;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.widget.TextView;

import com.idkjava.thelements.money.ProductManager;


public class PurchaseActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.purchase_activity);

        // Loading intent info, with SKU
        Intent i = getIntent();
        mSku = i.getStringExtra("purchase_sku");

        // Update purchasing text
        TextView purchaseText = (TextView) findViewById(R.id.purchasing_text);
        purchaseText.setText("Purchasing " + mSku);

        // Fire off the purchase workflow
        SharedPreferences prefs = getSharedPreferences(MainActivity.PREFS_NAME, 0);
        ProductManager mProductManager = ProductManager.getInstance(this, prefs);
        mProductManager.launchPurchase(this, mSku);
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
}
