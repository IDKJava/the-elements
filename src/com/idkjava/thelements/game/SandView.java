package com.idkjava.thelements.game;


import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.idkjava.thelements.MainActivity;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;

public class SandView extends GLSurfaceView
{
	private static final char FINGER_DOWN = 1;
	private static final char FINGER_UP = 0;

	private SandViewRenderer mRenderer; //Declare the renderer

	//Constructor
	public SandView(Context context, AttributeSet attrs)
	{
		super(context, attrs);
		mRenderer = new SandViewRenderer(); //Set up the Renderer for the View
		setEGLConfigChooser(false); //Set the EGLConfigChooser
		setRenderer(mRenderer); //Associate it with this view
	}
	@Override
	public void onResume()
	{
		super.onResume();
	}
	//When a touch screen event occurs
	public boolean onTouchEvent(final MotionEvent event)
	{
		//Set the touch state in JNI
		if (event.getAction() == MotionEvent.ACTION_DOWN)
		{
			setFingerState(FINGER_DOWN);
		}
		else if (event.getAction() == MotionEvent.ACTION_UP)
		{
			setFingerState(FINGER_UP);
		}

		//Set the touch position in JNI
		if (MainActivity.isZoomedIn())
		{
			//Both x and y are halved because it needs to be zoomed in
			setMouseLocation((int) event.getX() / 2, (int) event.getY() / 2);
		}
		else
		{
			//X and y are normal, because it is zoomed out
			setMouseLocation((int) event.getX(), (int) event.getY());
		}

		return true;
	}

	//@formatter:off
	private static native void setFingerState(char state);
	private static native void setMouseLocation(int x, int y);
	//@formatter:on

	static
	{
		System.loadLibrary("thelements");
	}
}

class SandViewRenderer implements GLSurfaceView.Renderer
{
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		//Do nothing
	}

	public void onSurfaceChanged(GL10 gl, int w, int h)
	{
		nativeResize(w, h);
		nativeLoadState(MainActivity.shouldLoadDemo);

		MainActivity.shouldLoadDemo = false;
	}

	public void onDrawFrame(GL10 gl)
	{
		nativeRender(); //Actual rendering - everything happens here
	}

	//@formatter:off
	private static native void nativeResize(int width, int height); //Jni resize
	private static native void nativeRender(); //Jni rendering function - everything happens here
	private static native void nativeLoadState(boolean shouldLoadDemo); //Jni load initial state
	//@formatter:on

	static
	{
		System.loadLibrary("thelements");
	}
}