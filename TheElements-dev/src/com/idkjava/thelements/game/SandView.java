package com.idkjava.thelements.game;


import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.FloatMath;
import android.view.MotionEvent;

import com.idkjava.thelements.MainActivity;

public class SandView extends GLSurfaceView
{
  private static final char FINGER_MOVE = 2;
	private static final char FINGER_DOWN = 1;
	private static final char FINGER_UP = 0;

	 //Touch event related variables
	private int m_touchState;
	private final int IDLE = 0;
  private final int TOUCH = 1;
  private final int PINCH = 2;
  private float m_dist0, m_distCurrent;

	private SandViewRenderer mRenderer; //Declare the renderer

	private static boolean m_isDragState = false;

	//Constructor
	public SandView(Context context, AttributeSet attrs)
	{
		super(context, attrs);
		mRenderer = new SandViewRenderer(); //Set up the Renderer for the View
		setEGLConfigChooser(8, 8, 8, 8, // RGBA channel bits
				16, 0); // depth and stencil channel min bits
		setRenderer(mRenderer); //Associate it with this view
	}

	public void setIsDragging(boolean isDrag) {
	  m_isDragState = isDrag;
	  setIsPanMode(isDrag ? (char) 1 : (char) 0);
	}

  // When a touch screen event occurs
  public boolean onTouchEvent(final MotionEvent event)
  {
    if (!m_isDragState) {
      return handleNormalTouch(event);
    }
    else {
      return handlePanTouch(event);
    }
  }

  private boolean handleNormalTouch(final MotionEvent event) {
    // Set the touch state in JNI
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

    // Pass raw mouse state into native code
    setMouseLocation(fingerState, (int) event.getX(), (int) event.getY());

    return true;
  }

  private boolean handlePanTouch(final MotionEvent event) {
    float distx, disty;

    switch (event.getAction() & MotionEvent.ACTION_MASK) {
    case MotionEvent.ACTION_DOWN:
      setMouseLocation(FINGER_DOWN, (int) event.getX(), (int) event.getY());
      m_touchState = TOUCH;
      break;
    case MotionEvent.ACTION_POINTER_DOWN:
      m_touchState = PINCH;
      setPinchActive((char) 1);
      // Get the distance when the second pointer touch
      distx = event.getX(0) - event.getX(1);
      disty = event.getY(0) - event.getY(1);
      m_dist0 = FloatMath.sqrt(distx * distx + disty * disty);

      break;
    case MotionEvent.ACTION_MOVE:
      if (m_touchState == PINCH) {
        // Get the current distance
        distx = event.getX(0) - event.getX(1);
        disty = event.getY(0) - event.getY(1);
        m_distCurrent = FloatMath.sqrt(distx * distx + disty * disty);
        setPinchScale(m_dist0 / m_distCurrent);
      }
      else {
        setMouseLocation(FINGER_MOVE, (int) event.getX(), (int) event.getY());
      }

      break;
    case MotionEvent.ACTION_UP:
      setMouseLocation(FINGER_UP, (int) event.getX(), (int) event.getY());
      m_touchState = IDLE;
      break;
    case MotionEvent.ACTION_POINTER_UP:
      setMouseLocation(FINGER_UP, (int) event.getX(), (int) event.getY());
      setPinchActive((char) 0);
      m_touchState = TOUCH;
      break;
    }

    return true;
  }

	//@formatter:off
	private static native void setMouseLocation(char state, int x, int y);
	private static native void setPinchScale(float scale);
	private static native void setPinchActive(char active);
	private static native void setIsPanMode(char isDrag);
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