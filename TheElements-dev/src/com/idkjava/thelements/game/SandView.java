package com.idkjava.thelements.game;


import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.FloatMath;
import android.util.Log;
import android.view.MotionEvent;

import com.idkjava.thelements.MainActivity;
import com.kamcord.android.Kamcord;

public class SandView extends GLSurfaceView
{
    public enum Tool { BRUSH_TOOL, HAND_TOOL }
    private Tool mTool = Tool.BRUSH_TOOL;

	//Constructor
	public SandView(Context context, AttributeSet attrs)
	{
		super(context, attrs);
		setEGLContextClientVersion(2);
		SandViewRenderer rend = new SandViewRenderer(); //Set up the Renderer for the View
		setEGLConfigChooser(8, 8, 8, 8, // RGBA channel bits
				16, 0); // depth and stencil channel min bits
		setRenderer(rend); //Associate it with this view
	}

	public void setTool(Tool tool) {
      mTool = tool;
	}

  // When a touch screen event occurs
  public boolean onTouchEvent(final MotionEvent event)
  {
    switch (mTool) {
      case BRUSH_TOOL: return handleBrushTouch(event);
      case HAND_TOOL: return handlePanTouch(event);
    }
    return true;
  }

  private boolean handleBrushTouch(final MotionEvent event) {
    // Send the touch events to the native lib
    if (event.getAction() == MotionEvent.ACTION_DOWN) {
        Log.e("TheElements", "Brush down start");
        brushStartLocation((int) event.getX(), (int) event.getY());
        Log.e("TheElements", "Brush down end");
    }
    else if (event.getAction() == MotionEvent.ACTION_UP) {
        Log.e("TheElements", "Brush up start");
        brushEndLocation((int) event.getX(), (int) event.getY());
        Log.e("TheElements", "Brush up end");
    }
    else {
        Log.e("TheElements", "Brush move start");
        brushMoveLocation((int) event.getX(), (int) event.getY());
        Log.e("TheElements", "Brush move end");
    }
    return true;
  }

  private enum HandState { NONE, PAN, PINCH }
  private int mDragStartX, mDragStartY;
  private HandState mHandState;
  private float m_dist0;
  private boolean handlePanTouch(final MotionEvent event) {
    float distx, disty;

    switch (event.getAction() & MotionEvent.ACTION_MASK) {
    case MotionEvent.ACTION_DOWN:
      mHandState = HandState.PAN;
      mDragStartX = (int)event.getX();
      mDragStartY = (int)event.getY();
      break;
    case MotionEvent.ACTION_POINTER_DOWN:
      mHandState = HandState.PINCH;
      // Get the distance when the second pointer touch
      distx = event.getX(0) - event.getX(1);
      disty = event.getY(0) - event.getY(1);
      m_dist0 = FloatMath.sqrt(distx * distx + disty * disty);
      break;
    case MotionEvent.ACTION_MOVE:
      if (mHandState == HandState.PAN) {
        panView(mDragStartX - (int)event.getX(), mDragStartY - (int)event.getY());
        mDragStartX = (int)event.getX();
        mDragStartY = (int)event.getY();
      }
      else if (mHandState == HandState.PINCH){
        // Get the current distance
        distx = event.getX(0) - event.getX(1);
        disty = event.getY(0) - event.getY(1);
        float distCurrent = FloatMath.sqrt(distx * distx + disty * disty);
        setPinchScale(m_dist0 / distCurrent);
      }
      break;
    case MotionEvent.ACTION_UP:
      mHandState = HandState.NONE;
      break;
    case MotionEvent.ACTION_POINTER_UP:
      mHandState = HandState.PAN;
      // Reset the drag position so the pan doesn't go crazy when one finger
      // is lifted. Uses the finger opposite of the one lifted.
      if (event.getActionIndex() == 0) {
          mDragStartX = (int)event.getX(1);
          mDragStartY = (int)event.getY(1);
      }
      else {
          mDragStartX = (int)event.getX(0);
          mDragStartY = (int)event.getY(0);
      }
      commitPinch();
      break;
    }

    return true;
  }

  //@formatter:off
  private static native void brushStartLocation(int x, int y);
  private static native void brushMoveLocation(int x, int y);
  private static native void brushEndLocation(int x, int y);
  private static native void panView(int dx, int dy);
  private static native void setPinchScale(float scale);
  private static native void commitPinch();
  //@formatter:on

  static
  {
      System.loadLibrary("stlport_shared");
      System.loadLibrary("protobuf");
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
      Kamcord.beginDraw();
      nativeRender();
      Kamcord.endDraw();
	}

	//@formatter:off
	private static native void nativeResize(int width, int height); //Jni resize
	private static native void nativeLoadState(boolean shouldLoadDemo); //Jni load initial state
	private static native void nativeRender();
	//@formatter:on

	static
	{
	    
        System.loadLibrary("stlport_shared");
	    System.loadLibrary("protobuf");
		System.loadLibrary("thelements");
	}
}