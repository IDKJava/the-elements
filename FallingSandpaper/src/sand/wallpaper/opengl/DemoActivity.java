/*
 * Element Works: Copyright (C) 2010 IDKJava
 * ----------------------------------------------------------
 * A sandbox type game in which you can play with different elements
 * which interact with each other in unique ways.
 */

package sand.wallpaper.opengl;

import java.util.Random;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.SharedPreferences;
import android.hardware.SensorListener;
import android.hardware.SensorManager;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.view.MotionEvent;

public class DemoActivity extends OpenGLES2WallpaperService implements
        SensorListener {
    class SandView extends GLEngine {
        private int fd; // Set the "finger down" variable

        private static final char FINGER_MOVE = 2;
        private static final char FINGER_DOWN = 1;
        private static final char FINGER_UP = 0;

        public static final char ERASER_ELEMENT = 2;
        public static final char NORMAL_ELEMENT = 3;
        public static final int NUM_BASE_ELEMENTS = 32;

        DemoRenderer mRenderer;

        SharedPreferences myPrefs = getSharedPreferences("TheElementsPrefs", 0);

        SandView() {
            super();
            // Handle prefs, other initialization
            initSurfaceView();
            mRenderer = new DemoRenderer();
            setEGLContextClientVersion(2);
            setEGLConfigChooser(8, 8, 8, 8, // RGBA channel bits
                    16, 0); // depth and stencil channel min bits
            setRenderer(mRenderer);
            setTouchEventsEnabled(true);
        }

        // When finger is held down, flood of events killing framerate, need to
        // put in it's own thread at some point and then use the sleep tactic
        public void onTouchEvent(final MotionEvent event) {
            // Gets the touch position
            // Touch down
            switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN: {
                setMouseLocation(FINGER_DOWN, (int) event.getX(),
                        (int) event.getY());
                break;
            }
            case MotionEvent.ACTION_UP: {
                // Tell jni
                setMouseLocation(FINGER_UP, (int) event.getX(),
                        (int) event.getY());
                if (myPrefs.getBoolean("random_pref", true)) {
                    // Randomly set the next element
                    random_num = generator.nextInt(NUM_BASE_ELEMENTS
                            - NORMAL_ELEMENT)
                            + NORMAL_ELEMENT;
                    setElement((char) random_num);
                } else {
                    // Sequentially set the next element
                    if (getElement() == NUM_BASE_ELEMENTS - 1) {
                        // Reset to 0 if last element
                        setElement((char) NORMAL_ELEMENT);
                    } else {
                        // Otherwise increment by one
                        setElement((char) (getElement() + 1));
                    }
                }
                break;
            }
            case MotionEvent.ACTION_MOVE: {
                setMouseLocation(FINGER_MOVE, (int) event.getX(),
                        (int) event.getY());
                break;
            }
            }
        }
    }

    public static boolean loaddemov = false;

    /*
     * Not needed any more static final int maxx = 319; // 319 for g1, 479 for
     * droid static final int maxy = 414; // 454 for g1, 815 for droid
     */
    static public boolean play = true;
    private static final int SHAKE_THRESHOLD = 800;
    static public int size = 0;
    /*
     * Not needed any more static public int skip = 1; static public int speed =
     * 1;
     */

    static {
        System.loadLibrary("stlport_shared");
        System.loadLibrary("protobuf");
        System.loadLibrary("thelements"); // Load the JNI library
    }

    // @formatter:off
    // JNI Functions
    // Save/load functions
    public static native char saveTempState();

    public static native char loadDemoState();

    // General utility functions
    private static native void nativeInit();

    private static native void nativeRefreshElements();

    public native void clearScreen();

    // Setters
    public static native void setPlayState(boolean playState);

    public static native void setElement(char element);

    public static native void setBrushSize(char brushSize);

    /**
     * Sets a filter rendering mode
     * 
     * @param mode
     *            0 - none 1 - motion blur
     */
    public static native void setFilterMode(char mode);

    // Getters
    public static native char getElement();

    public static native byte[] getElementInfo(int index);

    public static native int getElementRed(int index);

    public static native int getElementGreen(int index);

    public static native int getElementBlue(int index);

    // Accelerometer related
    public static native void setXGravity(float xGravity);

    public static native void setYGravity(float yGravity);

    // @formatter:off
    private static native void setMouseLocation(char state, int x, int y);

    private static native void setPinchScale(float scale);

    private static native void setPinchActive(char active);

    private static native void setIsPanMode(char isDrag);

    // @formatter:on

    Random generator = new Random();

    private float last_x, last_y, last_z;
    private long lastUpdate = -1;
    private int random_num;

    private SensorManager sensorMgr;

    private float x, y, z;

    public DemoActivity() {
        super();
    }

    // Required to have this
    public void onAccuracyChanged(int arg0, int arg1) {
    }

    @Override
    public void onCreate() {
        super.onCreate(); // Uses onCreate from the general

        // Create the sensor manager
        sensorMgr = (SensorManager) getSystemService(SENSOR_SERVICE);
        // Find out if the accelerometer is supported
        boolean accelSupported = sensorMgr.registerListener(this,
                SensorManager.SENSOR_ACCELEROMETER,
                SensorManager.SENSOR_DELAY_GAME);

        // If accel is not supported
        if (!accelSupported) {
            // Remove the listener
            sensorMgr.unregisterListener(this,
                    SensorManager.SENSOR_ACCELEROMETER);
        }

        Preferences.setPreferences(this);
    }

    @Override
    public Engine onCreateEngine() {
        return new SandView();
    }

    public void onSensorChanged(int sensor, float[] values) {
        if (sensor == SensorManager.SENSOR_ACCELEROMETER) {
            long curTime = System.currentTimeMillis();
            // only allow one update every 100ms.
            if ((curTime - lastUpdate) > 100) {
                long diffTime = (curTime - lastUpdate);
                lastUpdate = curTime;

                x = values[SensorManager.DATA_X];
                y = values[SensorManager.DATA_Y];
                z = values[SensorManager.DATA_Z];

                float speed = Math.abs(x + y + z - last_x - last_y - last_z)
                        / diffTime * 10000;
                if (speed > SHAKE_THRESHOLD) {
                    clearScreen();
                }
                last_x = x;
                last_y = y;
                last_z = z;
            }
        }
    }

    @Override
    Renderer getNewRenderer() {
        return new DemoRenderer();
    }
}

class DemoRenderer implements GLSurfaceView.Renderer {
    public void onDrawFrame(GL10 gl) {
        nativeRender(); // Actual rendering - everything happens here
    }

    public void onSurfaceChanged(GL10 gl, int w, int h) {
        nativeResize(w, h);
        // Load the demo from sdcard on first run
        if (DemoActivity.loaddemov == true) {
            // DemoActivity.loaddemo();
            // commented out due to laziness
            DemoActivity.loaddemov = false;
        }
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeInit();
    }

    private static native void nativeInit(); // Jni init

    private static native void nativeResize(int w, int h); // Jni resize

    private static native void nativeRender(); // Jni rendering function -
                                               // everything happens here
}
