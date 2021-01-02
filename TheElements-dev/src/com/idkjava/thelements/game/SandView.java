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
    public enum Tool {
        BRUSH_TOOL,
        ERASE_TOOL,
        HAND_TOOL,
        BH_TOOL,
        WH_TOOL,
        CH_TOOL,
        NG_TOOL,
        REMOVE_GRAV_TOOL,
        RECT_TOOL,
        CIRCLE_TOOL,
        LINE_TOOL,
        SLINGSHOT_TOOL,
        SPRAY_TOOL,
        TRIANGLE_TOOL,
        DASHED_LINE_TOOL,
        PORTAL_TOOL
    }
    private Tool mTool = Tool.BRUSH_TOOL;

    // Integer constants reflecting drag render in native
    // NOTE: Must match macros in native
    private static final int RT_RECT = 0;
    private static final int RT_LINE = 1;
    private static final int RT_CIRCLE = 2;
    private static final int RT_TRI = 3;

    // Boolean constants for brush props
    private static final int BRUSH_NORMAL = 0;
    private static final int BRUSH_SPRAY = 1;

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

        // Initialize tool rendering properties in native
        switch (mTool) {
            case BRUSH_TOOL:
            case ERASE_TOOL:
                setBrushProps(BRUSH_NORMAL);
                break;
            case SPRAY_TOOL:
                setBrushProps(BRUSH_SPRAY);
                break;
            case NG_TOOL:
            case RECT_TOOL:
                setDragProps(RT_RECT);
                break;
            case TRIANGLE_TOOL:
                setDragProps(RT_TRI);
                break;
            case CIRCLE_TOOL:
                setDragProps(RT_CIRCLE);
                break;
            case LINE_TOOL:
            case DASHED_LINE_TOOL:
            case SLINGSHOT_TOOL:
            case PORTAL_TOOL:
                setDragProps(RT_LINE);
                break;
        }
	}

    public boolean isDrawTool() {
        switch (mTool) {
            case BRUSH_TOOL:
            case SPRAY_TOOL:
            case RECT_TOOL:
            case TRIANGLE_TOOL:
            case CIRCLE_TOOL:
            case LINE_TOOL:
            case DASHED_LINE_TOOL:
            case SLINGSHOT_TOOL:
                return true;
            default:
                return false;
        }
    }

  // When a touch screen event occurs
  public boolean onTouchEvent(final MotionEvent event)
  {
      switch (mTool) {
          case BRUSH_TOOL:
          case ERASE_TOOL:
          case SPRAY_TOOL:
              return handleBrushTouch(event);
          case HAND_TOOL:
              return handlePanTouch(event);
          case BH_TOOL:
          case WH_TOOL:
          case CH_TOOL:
          case REMOVE_GRAV_TOOL:
              return handlePointTouch(event);
          case NG_TOOL:
          case RECT_TOOL:
          case TRIANGLE_TOOL:
          case CIRCLE_TOOL:
          case LINE_TOOL:
          case SLINGSHOT_TOOL:
          case DASHED_LINE_TOOL:
          case PORTAL_TOOL:
              return handleRectTouch(event);
          default:
              throw new RuntimeException("Unknown tool selected!");
      }
  }

  private boolean handleBrushTouch(final MotionEvent event) {
    // Send the touch events to the native lib
    if (event.getAction() == MotionEvent.ACTION_DOWN) {
        brushStartLocation((int) event.getX(), (int) event.getY());
    }
    else if (event.getAction() == MotionEvent.ACTION_UP) {
        brushEndLocation((int) event.getX(), (int) event.getY());
    }
    else {
        brushMoveLocation((int) event.getX(), (int) event.getY());
    }
    return true;
  }

    private boolean handlePointTouch(final MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            // TODO: Send a temp black hole signal to native,
            // so we render without the gravity
        }
        else if (event.getAction() == MotionEvent.ACTION_UP) {
            // Final placement of the hole
            switch (mTool) {
                case BH_TOOL:
                    makeBlackHole((int) event.getX(), (int) event.getY());
                    break;
                case WH_TOOL:
                    makeWhiteHole((int) event.getX(), (int) event.getY());
                    break;
                case CH_TOOL:
                    makeCurlHole((int) event.getX(), (int) event.getY());
                    break;
                case REMOVE_GRAV_TOOL:
                    removeGravObject((int) event.getX(), (int) event.getY());
                    break;
                default:
                    throw new RuntimeException("Invalid handler for tool: " + mTool);
            }
        }
        else {
            // TODO: Send a move temp signal to native.
        }
        return true;
    }

    // Handles all point-to-point drag type tools
    private int mRectStartX, mRectStartY;
    private boolean handleRectTouch(final MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            mRectStartX = (int)event.getX();
            mRectStartY = (int)event.getY();
            rectStart(mRectStartX, mRectStartY);
        }
        else if (event.getAction() == MotionEvent.ACTION_UP) {
            rectEnd();
            int ex = (int)event.getX();
            int ey = (int)event.getY();
            switch (mTool) {
                case NG_TOOL:
                    makeNullGravity(mRectStartX, mRectStartY, ex, ey);
                    break;
                case RECT_TOOL:
                    drawRect(mRectStartX, mRectStartY, ex, ey);
                    break;
                case TRIANGLE_TOOL:
                    drawTri(mRectStartX, mRectStartY, ex, ey);
                    break;
                case CIRCLE_TOOL:
                    drawCircle(mRectStartX, mRectStartY, ex, ey);
                    break;
                case LINE_TOOL:
                    drawLine(mRectStartX, mRectStartY, ex, ey);
                    break;
                case SLINGSHOT_TOOL:
                    drawSlingshot(mRectStartX, mRectStartY, ex, ey);
                    break;
                case DASHED_LINE_TOOL:
                    drawLineDashed(mRectStartX, mRectStartY, ex, ey);
                    break;
                case PORTAL_TOOL:
                    addPortal(mRectStartX, mRectStartY, ex, ey);
                    break;
                default:
                    throw new RuntimeException("Invalid handler for tool: " + mTool);
            }
        }
        else {
            rectMove((int)event.getX(), (int)event.getY());
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
      m_dist0 = (float) Math.sqrt(distx * distx + disty * disty);
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
        float distCurrent = (float) Math.sqrt(distx * distx + disty * disty);
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
  private static native void setBrushProps(int brushType);
  private static native void setDragProps(int renderType);
  private static native void rectStart(int x, int y);
  private static native void rectMove(int x, int y);
  private static native void rectEnd();
  private static native void brushStartLocation(int x, int y);
  private static native void brushMoveLocation(int x, int y);
  private static native void brushEndLocation(int x, int y);
  private static native boolean makeBlackHole(int x, int y);
  private static native boolean makeWhiteHole(int x, int y);
  private static native boolean makeCurlHole(int x, int y);
  private static native boolean makeNullGravity(int sx, int sy, int ex, int ey);
  private static native void drawRect(int sx, int sy, int ex, int ey);
    private static native void drawTri(int sx, int sy, int ex, int ey);
    private static native void drawCircle(int sx, int sy, int ex, int ey);
    private static native void drawLine(int sx, int sy, int ex, int ey);
    private static native void drawLineDashed(int sx, int sy, int ex, int ey);
    private static native void drawSlingshot(int sx, int sy, int ex, int ey);
    private static native void addPortal(int sx, int sy, int ex, int ey);
  private static native void removeGravObject(int x, int y);
  private static native void panView(int dx, int dy);
  private static native void setPinchScale(float scale);
  private static native void commitPinch();
  //@formatter:on

  static
  {
      System.loadLibrary("c++_shared");
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
        if (MainActivity.shouldLoadPhoto) {
            MainActivity.setGameToPhoto();
        }
        else {
            nativeLoadState(MainActivity.shouldLoadDemo);
            MainActivity.shouldLoadDemo = false;
        }
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
	    
        System.loadLibrary("c++_shared");
	    System.loadLibrary("protobuf");
		System.loadLibrary("thelements");
	}
}