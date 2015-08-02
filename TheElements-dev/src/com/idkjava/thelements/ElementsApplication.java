package com.idkjava.thelements;

import android.app.Application;
import android.content.SharedPreferences;

import com.idkjava.thelements.money.ProductManager;

/**
 * Custom Application class to handle application-level things.
 */
public class ElementsApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();

        initAppSingletons();
    }

    public static SharedPreferences getPrefs() {
        return sPrefs;
    }

    public static ProductManager getProductManager() {
        return sProductManager;
    }

    private void initAppSingletons() {
        sPrefs = getSharedPreferences(PREFS_NAME, 0);
        sProductManager = new ProductManager(this, sPrefs);
    }

    private static final String PREFS_NAME = "MyPrefsfile";

    public static SharedPreferences sPrefs;
    public static ProductManager sProductManager;
}
