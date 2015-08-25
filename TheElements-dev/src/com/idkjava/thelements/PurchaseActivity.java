package com.idkjava.thelements;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.idkjava.thelements.error.ActivityErrorHandler;
import com.idkjava.thelements.error.ErrorHandler;
import com.idkjava.thelements.money.ProductManager;


public class PurchaseActivity extends Activity {

    private static final int OWNED_COLOR = Color.rgb(0, 255, 50);

    TextView spaceWorldTitle;
    TextView toolPackTitle;
    TextView cameraToolTitle;

    private void firePurchase(String sku, ErrorHandler handler) {
        // Fire off the purchase workflow
        mProductManager = ElementsApplication.getProductManager();
        mProductManager.bindErrorHandler(handler);
        mProductManager.launchPurchase(this, sku);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.purchase_activity);

        // Create ErrorHandler for this activity
        final ErrorHandler handler = new ActivityErrorHandler(this);
        mHandler = handler;

        spaceWorldTitle = (TextView) findViewById(R.id.space_world_title);
        toolPackTitle = (TextView) findViewById(R.id.tool_pack_title);
        cameraToolTitle = (TextView) findViewById(R.id.camera_tool_title);
        View spaceWorldButton = findViewById(R.id.space_world_button);
        spaceWorldButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (ElementsApplication.checkOwned(ProductManager.SKU_GRAVITY_PACK)) {
                    handler.error(R.string.space_world_owned_error);
                }
                else {
                    firePurchase(ProductManager.SKU_GRAVITY_PACK, handler);
                }
            }
        });
        View toolPackButton = findViewById(R.id.tool_pack_button);
        toolPackButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (ElementsApplication.checkOwned(ProductManager.SKU_TOOL_PACK)) {
                    handler.error(R.string.tool_pack_owned_error);
                }
                else {
                    firePurchase(ProductManager.SKU_TOOL_PACK, handler);
                }
            }
        });
        View cameraToolButton = findViewById(R.id.camera_tool_button);
        cameraToolButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (ElementsApplication.checkOwned(ProductManager.SKU_CAMERA_TOOL)) {
                    handler.error(R.string.camera_tool_owned_error);
                }
                else {
                    firePurchase(ProductManager.SKU_CAMERA_TOOL, handler);
                }
            }
        });

        // Loading intent info, with SKU
        Intent i = getIntent();
        String sku = i.getStringExtra("purchase_sku");

        if (sku != null) {
            if (ElementsApplication.checkOwned(sku)) {
                handler.error(R.string.item_owned_error);
            }
            else {
                firePurchase(sku, mHandler);
            }
        }
    }

    private void refreshUI() {
        if (ElementsApplication.checkOwned(ProductManager.SKU_GRAVITY_PACK)) {
            spaceWorldTitle.setText(R.string.space_world_owned);
            spaceWorldTitle.setTextColor(OWNED_COLOR);
        }
        if (ElementsApplication.checkOwned(ProductManager.SKU_TOOL_PACK)) {
            toolPackTitle.setText(R.string.tool_pack_owned);
            toolPackTitle.setTextColor(OWNED_COLOR);
        }
        if (ElementsApplication.checkOwned(ProductManager.SKU_CAMERA_TOOL)) {
            cameraToolTitle.setText(R.string.camera_tool_owned);
            cameraToolTitle.setTextColor(OWNED_COLOR);
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        mProductManager = ElementsApplication.getProductManager();
        mProductManager.refreshInventory(this, new Runnable() {
            @Override
            public void run() {
                refreshUI();
            }
        });
    }

    @Override
    protected void onDestroy() {
        if (mHandler != null) {
            mHandler.clear();
        }
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

        mProductManager.refreshInventory(this, new Runnable() {
            @Override
            public void run() {
                refreshUI();
            }
        });
    }

    private ProductManager mProductManager;
    private ErrorHandler mHandler;
}
