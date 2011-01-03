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
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorListener;
import android.hardware.SensorManager;
import android.opengl.GLWallpaperService;
import android.os.Handler;
import android.util.Log;
import android.view.MotionEvent;

public class DemoActivity extends GLWallpaperService implements SensorListener
{
	class SandView extends GLEngine
	{
		private int fd; // Set the "finger down" variable

		DemoRenderer mRenderer;
		
		SharedPreferences myPrefs = getSharedPreferences("TheElementsPrefs", 0);

		SandView()
		{
			super();
			//Handle prefs, other initialization
			mRenderer = new DemoRenderer();
			setRenderer(mRenderer);
			setTouchEventsEnabled(true);
		}

		// When finger is held down, flood of events killing framerate, need to
		// put in it's own thread at some point and then use the sleep tactic
		public void onTouchEvent(final MotionEvent event)
		{
			// Gets the touch position
			//Touch down
			if (event.getAction() == MotionEvent.ACTION_DOWN && fd != 1)
			{
				//If the finger state has changed, set the state in jni
				DemoActivity.fd(1);
			}
			//Touch released
			else if (event.getAction() == MotionEvent.ACTION_UP)
			{
				//Tell jni
				DemoActivity.fd(2);
				if(myPrefs.getBoolean("random_pref", true))
				{
					//Randomly set the next element
					do
					{
						random_num = generator.nextInt(25);
					}
					while(random_num == 3 || random_num == 8);
					
					setelement(random_num);
				}
				else
				{
					//Sequentially set the next element
					if(getelement() == 22)
					{
						//Reset to 0 if last element
						setelement(0);
					}
					else if(getelement() == 2)
					{
						//Set element to 4, cuz 3 is gone
						setelement(4);
					}
					else if(getelement() == 7)
					{
						//Set element to 9, cuz 8 is gone
						setelement(9);
					}
					else
					{
						//Otherwise increment by one
						setelement(getelement()+1);
					}
				}
			}

			//If we're on the small size
			if (DemoActivity.size == 0)
			{
				// Both x and y are halved because it needs to be zoomed in
				DemoActivity.mp((int) event.getX() / 2, (int) event.getY() / 2);
			}
			else
			{
				DemoActivity.mp((int) event.getX(), (int) event.getY());
			}
		}
	}

	public static boolean loaddemov = false;

	/*
	 * Not needed any more
	 * static final int maxx = 319; // 319 for g1, 479 for droid
	 * static final int maxy = 414; // 454 for g1, 815 for droid
	 */
	static public boolean play = true;
	private static final int SHAKE_THRESHOLD = 800;
	static public int size = 0;
	/*
	 * Not needed any more
	 * static public int skip = 1;
	 * static public int speed = 1;
	 */

	static
	{
		System.loadLibrary("thelements"); // Load the JNI library
	}

	// JNI functions
	public native static void clearquicksave();
	public native static void fd(int fstate);
	public native static int getelement();
	public native static void jPause();
	public native static int load();
	public native static void loadcustom();
	public native static int loaddemo();
	public native static void mp(int jxm, int jym);
	public native static void nativePause();
	public native static void Play();
	public native static void quickload();
	public native static void quicksave();
	public native static int save();
	public native static void savecustom();
	public native static void sendxg(float xgrav);
	public native static void sendyg(float ygrav);
	public native static void setAccelOnOff(int state);
	public native static void setBackgroundColor(int colorcode);
	public native static void setblue(int blueness);
	public native static void setBrushSize(int jsize);
	public native static void setcollision(int custnumber, int elementnumb, int collisionspot, int collisionnumber);
	public native static void setelement(int element);
	public native static void setexplosiveness(int explosiveness);
	public native static void setFlip(int flipped);
	public native static void setgreen(int greenness);
	public native static void setred(int redness);
	public native static void setup();
	public native static void tester();
	public native static void togglesize();

	Random generator = new Random();

	private float last_x, last_y, last_z;
	private long lastUpdate = -1;
	private int random_num;

	private SensorManager sensorMgr;

	private float x, y, z;

	public DemoActivity()
	{
		super();
	}

	//Required to have this
	public void onAccuracyChanged(int arg0, int arg1) {}

	@Override
	public void onCreate()
	{
		super.onCreate(); // Uses onCreate from the general

		//Load custom elements
		loadcustom();

		//Create the sensor manager
		sensorMgr = (SensorManager) getSystemService(SENSOR_SERVICE);
		//Find out if the accelerometer is supported
		boolean accelSupported = sensorMgr.registerListener(this, SensorManager.SENSOR_ACCELEROMETER, SensorManager.SENSOR_DELAY_GAME);

		//If accel is not supported
		if (!accelSupported)
		{
			//Remove the listener
			sensorMgr.unregisterListener(this, SensorManager.SENSOR_ACCELEROMETER);
		}
		
		Preferences.setPreferences(this);
	}

	@Override
	public Engine onCreateEngine()
	{
		return new SandView();
	}

	public void onSensorChanged(int sensor, float[] values)
	{
		if (sensor == SensorManager.SENSOR_ACCELEROMETER)
		{
			long curTime = System.currentTimeMillis();
			// only allow one update every 100ms.
			if ((curTime - lastUpdate) > 100)
			{
				long diffTime = (curTime - lastUpdate);
				lastUpdate = curTime;

				x = values[SensorManager.DATA_X];
				y = values[SensorManager.DATA_Y];
				z = values[SensorManager.DATA_Z];

				float speed = Math.abs(x + y + z - last_x - last_y - last_z)
						/ diffTime * 10000;
				if (speed > SHAKE_THRESHOLD)
				{
					clearquicksave();
					setup();
				}
				last_x = x;
				last_y = y;
				last_z = z;
			}
		}
	}
}

class DemoRenderer implements GLWallpaperService.Renderer
{
	public void onDrawFrame(GL10 gl)
	{
		nativeRender(); // Actual rendering - everything happens here
	}

	public void onSurfaceChanged(GL10 gl, int w, int h)
	{
		nativeResize(w, h);
		//Load the demo from sdcard on first run
		if (DemoActivity.loaddemov == true)
		{
			DemoActivity.loaddemo();
			DemoActivity.loaddemov = false;
		}
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		nativeInit();
	}

    private static native void nativeInit(); //Jni init
    private static native void nativeResize(int w, int h); //Jni resize
    private static native void nativeRender(); //Jni rendering function - everything happens here
}
