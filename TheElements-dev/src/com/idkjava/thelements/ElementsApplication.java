package com.idkjava.thelements;

import android.app.Application;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.idkjava.thelements.money.ProductManager;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;

/**
 * Custom Application class to handle application-level things.
 */
public class ElementsApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();

        initAppSingletons();

        // Load textures
        AssetManager am = getAssets();
        Bitmap bhTex;
        try {
            bhTex = BitmapFactory.decodeStream(am.open("bhTex.png"));
            Log.e("TheElements", "Bitmap config: " + bhTex.getConfig());
        }
        catch (IOException e) {
            throw new RuntimeException("Could not load BH texture.");
        }
        int w = bhTex.getWidth();
        int h = bhTex.getHeight();
        int size = bhTex.getRowBytes()*h;
        ByteBuffer buf = ByteBuffer.allocate(size);
        bhTex.copyPixelsToBuffer(buf);
        setBHTex(w, h, buf.array());
        bhTex.recycle();
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

    // Loading textures
    public static native void setBHTex(int w, int h, byte[] pixels);

    static {
        System.loadLibrary("stlport_shared");
        try {
            System.loadLibrary("kamcord");
        } catch (UnsatisfiedLinkError e) {
            Log.d("TheElements", "Kamcord not supported");
        }
        System.loadLibrary("protobuf");
        System.loadLibrary("thelements");
    }
}
