package sand.falling.opengl;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.hardware.Sensor;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;

public class SandView extends GLSurfaceView
{
	private static int fd; //Set the "finger down" variable
	
    public SandView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
    	mRenderer = new DemoRenderer(); //Set up the Renderer for the View
        setRenderer(mRenderer); //Set it as the main
        Log.v("DemoActivity", "SandView Constructor");
        
    }

    //When finger is held down, flood of events killing framerate, need to put in it's own thread at some point
    //and then use the sleep tactic
    public boolean onTouchEvent(final MotionEvent event) //Finger down
    {
    	
    	// Gets the mouse position
    	if (event.getAction() == MotionEvent.ACTION_DOWN)
    	{
			if (fd != 1) //if it has changed
			{
				MainActivity.fd(1); //sets the finger state in jni
    		}
    		else
    		{
    			fd = 1;
    		}
    	}
    	else if (event.getAction() == MotionEvent.ACTION_UP)
    	{
    		MainActivity.fd(2);
    	}
    	
    	if(MainActivity.size == 0)
    	{
    		//Both x and y are halved because it needs to be zoomed in
    		MainActivity.mp((int)event.getX()/2, (int)event.getY()/2); //sets the mouse position in jdk
    	}
    	else
    	{
    		//Not zoomed in
    		MainActivity.mp((int)event.getX(), (int)event.getY()); //sets the mouse position in jdk		
    	}
    	
    	return true;
    }

    DemoRenderer mRenderer; //Declare the renderer
}

class DemoRenderer implements GLSurfaceView.Renderer
{
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        nativeInit();
    }

    public void onSurfaceChanged(GL10 gl, int w, int h)
    {
        //gl.glViewport(0, 0, w, h);
        nativeResize(w, h);
        if (MainActivity.loaddemov == true) //loads the demo from the sdcard on first run.
        {
        	MainActivity.loaddemo();
        	MainActivity.loaddemov = false;
        }
    }

    public void onDrawFrame(GL10 gl)
    {
		nativeRender(); //Actual rendering - everything happens here
    }

    private static native void nativeInit(); //Jni init
    private static native void nativeResize(int w, int h); //Jni resize
    private static native void nativeRender(); //Jni rendering function - everything happens here
}