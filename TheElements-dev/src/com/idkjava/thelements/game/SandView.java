package com.idkjava.thelements.game;


import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;

import com.idkjava.thelements.MainActivity;

public class SandView extends GLSurfaceView
{
    private static final char FINGER_MOVE = 2;
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

	//When a touch screen event occurs
	public boolean onTouchEvent(final MotionEvent event)
	{
		//Set the touch state in JNI
	    char fingerState;
		if (event.getAction() == MotionEvent.ACTION_DOWN)
		{
            fingerState = FINGER_DOWN;
		}
		else if (event.getAction() == MotionEvent.ACTION_UP)
		{
		    fingerState = FINGER_UP;
		}
		else
		{
		    fingerState = FINGER_MOVE;
		}

		//Pass raw mouse state into native code
		setMouseLocation(fingerState, (int) event.getX(), (int) event.getY());

		return true;
	}

	//@formatter:off
	private static native void setMouseLocation(char state, int x, int y);
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
	    nativeRender();
	}

	//@formatter:off
	private static native void nativeResize(int width, int height); //Jni resize
	private static native void nativeLoadState(boolean shouldLoadDemo); //Jni load initial state
	private static native void nativeRender();
	//@formatter:on

	static
	{
		System.loadLibrary("thelements");
	}
}