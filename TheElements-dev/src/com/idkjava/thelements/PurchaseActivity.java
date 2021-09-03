package com.idkjava.thelements;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import androidx.fragment.app.FragmentActivity;

import com.google.android.gms.analytics.HitBuilders;
import com.idkjava.thelements.error.ActivityErrorHandler;
import com.idkjava.thelements.error.ErrorHandler;
import com.idkjava.thelements.money.ProductManager;


public class PurchaseActivity extends FragmentActivity {

    private static final int OWNED_COLOR = Color.rgb(0, 255, 50);
    private static final int UNOWNED_COLOR = Color.WHITE;

    TextView spaceWorldTitle;
    TextView toolPackTitle;
    TextView cameraToolTitle;
    TextView portalToolTitle;

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
        portalToolTitle = (TextView) findViewById(R.id.portal_tool_title);

        View spaceWorldButton = findViewById(R.id.space_world_button);
        spaceWorldButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (ElementsApplication.checkOwned(ProductManager.SKU_GRAVITY_PACK)) {
                    handler.error(R.string.space_world_owned_error);
                }
                else {
                    ElementsApplication.getTracker().send(
                            new HitBuilders.EventBuilder()
                                    .setCategory("ButtonPress")
                                    .setAction("Purchase Space World")
                                    .build()
                    );
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
                    ElementsApplication.getTracker().send(
                            new HitBuilders.EventBuilder()
                                    .setCategory("ButtonPress")
                                    .setAction("Purchase Tool Pack")
                                    .build()
                    );
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
                    ElementsApplication.getTracker().send(
                            new HitBuilders.EventBuilder()
                                    .setCategory("ButtonPress")
                                    .setAction("Purchase Camera Tool")
                                    .build()
                    );
                    firePurchase(ProductManager.SKU_CAMERA_TOOL, handler);
                }
            }
        });

        View portalToolButton = findViewById(R.id.portal_tool_button);
        portalToolButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (ElementsApplication.checkOwned(ProductManager.SKU_PORTAL_TOOL)) {
                    handler.error(R.string.portal_tool_owned_error);
                }
                else {
                    ElementsApplication.getTracker().send(
                            new HitBuilders.EventBuilder()
                                    .setCategory("ButtonPress")
                                    .setAction("Purchase Portal Tool")
                                    .build()
                    );
                    firePurchase(ProductManager.SKU_PORTAL_TOOL, handler);
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

    private void updateProductUI(String sku, TextView view, int ownedTextRes, int purchaseTextRes) {
        if (ElementsApplication.checkOwned(sku)) {
            view.setText(ownedTextRes);
            view.setTextColor(OWNED_COLOR);
        }
        else {
            view.setText(purchaseTextRes);
            view.append(mProductManager.getPrice(sku));
            view.setTextColor(UNOWNED_COLOR);
        }
    }

    private void refreshUI() {
        updateProductUI(ProductManager.SKU_GRAVITY_PACK, spaceWorldTitle,
                R.string.space_world_owned, R.string.space_world_purchase);
        updateProductUI(ProductManager.SKU_TOOL_PACK, toolPackTitle,
                R.string.tool_pack_owned, R.string.tool_pack_purchase);
        updateProductUI(ProductManager.SKU_CAMERA_TOOL, cameraToolTitle,
                R.string.camera_tool_owned, R.string.camera_tool_purchase);
        updateProductUI(ProductManager.SKU_PORTAL_TOOL, portalToolTitle,
                R.string.portal_tool_owned, R.string.portal_tool_purchase);
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

//    @Override
//    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
//        if (mProductManager == null) return;
//
//        // Need to forward on the activity result to the product manager
//        if (!mProductManager.handleActivityResult(requestCode, resultCode, data)) {
//            super.onActivityResult(requestCode, resultCode, data);
//        }
//
//        mProductManager.refreshInventory(this, new Runnable() {
//            @Override
//            public void run() {
//                refreshUI();
//            }
//        });
//    }

    private ProductManager mProductManager;
    private ErrorHandler mHandler;
}
