package com.idkjava.thelements;

import android.app.Application;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.idkjava.thelements.game.FileManager;
import com.idkjava.thelements.money.ProductManager;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * Custom Application class to handle application-level things.
 */
public class ElementsApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();

        initAppSingletons();

        // Find files root dir
        FileManager.ROOT_DIR = getFilesDir().getAbsolutePath() + "/";

        // Load textures
        AssetManager am = getAssets();
        Bitmap bhTex;
        Bitmap whTex;
        Bitmap chTex;
        Bitmap ngTex;

        // TODO: Remove code duplication by creating a sprite loader system

        // Black hole
        try {
            bhTex = BitmapFactory.decodeStream(am.open("bh_tex.png"));
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

        // White hole
        try {
            whTex = BitmapFactory.decodeStream(am.open("wh_tex.png"));
        }
        catch (IOException e) {
            throw new RuntimeException("Could not load BH texture.");
        }
        w = whTex.getWidth();
        h = whTex.getHeight();
        size = whTex.getRowBytes()*h;
        buf = ByteBuffer.allocate(size);
        whTex.copyPixelsToBuffer(buf);
        setWHTex(w, h, buf.array());
        whTex.recycle();

        // Curl hole
        try {
            chTex = BitmapFactory.decodeStream(am.open("ch_tex.png"));
        }
        catch (IOException e) {
            throw new RuntimeException("Could not load BH texture.");
        }
        w = chTex.getWidth();
        h = chTex.getHeight();
        size = chTex.getRowBytes()*h;
        buf = ByteBuffer.allocate(size);
        chTex.copyPixelsToBuffer(buf);
        setCHTex(w, h, buf.array());
        chTex.recycle();

        // Null gravity terminal
        try {
            ngTex = BitmapFactory.decodeStream(am.open("ng_tex.png"));
        }
        catch (IOException e) {
            throw new RuntimeException("Could not load BH texture.");
        }
        w = ngTex.getWidth();
        h = ngTex.getHeight();
        size = ngTex.getRowBytes()*h;
        buf = ByteBuffer.allocate(size);
        ngTex.copyPixelsToBuffer(buf);
        setNGTex(w, h, buf.array());
        ngTex.recycle();
    }

    public static SharedPreferences getPrefs() {
        return sPrefs;
    }

    public static ProductManager getProductManager() {
        return sProductManager;
    }

    public static boolean checkOwned(String sku) {
        return sPrefs.getBoolean(sku, false);
    }

    private void initAppSingletons() {
        Log.d("TheElements", "Setting up app singletons: prefs, product manager");
        sPrefs = getSharedPreferences(PREFS_NAME, 0);
        sProductManager = new ProductManager(this, sPrefs);
    }

    private static final String PREFS_NAME = "MyPrefsfile";

    public static SharedPreferences sPrefs;
    public static ProductManager sProductManager;

    // Loading textures
    public static native void setBHTex(int w, int h, byte[] pixels);
    public static native void setWHTex(int w, int h, byte[] pixels);
    public static native void setCHTex(int w, int h, byte[] pixels);
    public static native void setNGTex(int w, int h, byte[] pixels);

    static {
        System.loadLibrary("c++_shared");
        System.loadLibrary("protobuf");
        System.loadLibrary("thelements");
    }
}
