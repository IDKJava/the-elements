package com.example.theelementsshader;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;

public class SandView extends GLSurfaceView {

    private SandViewRenderer mRenderer;
    private float mTouchX, mTouchY;
    private TouchState mTouchState;
    
    public enum TouchState { DOWN, UP };
    
    public SandView(Context context, AttributeSet attrs) {
        super(context);
        // Enable OpenGL ES 2.0
        setEGLContextClientVersion(2);
        mRenderer = new SandViewRenderer();
        setRenderer(mRenderer);
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent e) {
        mTouchX = e.getX();
        mTouchY = e.getY();
        
        if (e.getAction() == MotionEvent.ACTION_DOWN ||
                e.getAction() == MotionEvent.ACTION_MOVE) {
            mTouchState = TouchState.DOWN;
        }
        else {
            mTouchState = TouchState.UP;
        }
        
        return true;
    }


    protected class SandViewRenderer implements GLSurfaceView.Renderer {

        private int[] mFrameBuf = new int[1];
        private int[] mRenderTex = new int[2];
        // Which render tex holds the current state
        // The other one will be drawn into next
        private int mCurTex = 0;
        
        private byte[] mFixedPixels;
        
        private GLRectangle mSurface1;
        private GLRectangle mSurface2;
        private GLRectangle mTouchRect;

        private float[] mMVMatrix = new float[16];
        private float[] mPMatrix = new float[16];
        private float[] mMVPMatrix = new float[16];
        
        private int w, h;
        
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            Log.i("TheElementsShader", "onSurfaceCreated");
            
            // Create drawn objects
            mSurface1 = new GLRectangle(-1.0f, -1.0f, 1.0f, 1.0f);
            mSurface1.setProg(R.raw.vert_simple, R.raw.frag_redshift, getContext(), "prog1");
            mSurface2 = new GLRectangle(-1.0f, -1.0f, 1.0f, 1.0f);
            mSurface2.setProg(R.raw.vert_simple, R.raw.frag_tex, getContext(), "prog2");
            mTouchRect = new GLRectangle(0.0f, 0.0f, 0.0f, 0.0f);
            mTouchRect.setProg(R.raw.vert_simple, R.raw.frag_single_color, getContext(), "touch rect prog");
            mTouchRect.setColor(1.0f, 0.0f, 0.0f, 1.0f);
            
            // Texture setup
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S,
                    GLES20.GL_CLAMP_TO_EDGE);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T,
                    GLES20.GL_CLAMP_TO_EDGE);
            
            // Render texture
            GLES20.glGenFramebuffers(1, mFrameBuf, 0);
            GLES20.glGenTextures(2, mRenderTex, 0);
            
            //DEBUG:
            //GLES20.glDisable(GLES20.GL_CULL_FACE);
            
            // Build fixed MV matrix
            Matrix.setLookAtM(mMVMatrix, 0,
                    0, 0, 3, // eye
                    0, 0, 0, // center
                    0, 1, 0); // up
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            w = width;
            h = height;
            GLES20.glViewport(0, 0, w, h);
            
            // Set MP Matrix
            Matrix.orthoM(mPMatrix, 0, -1.0f, 1.0f, -1.0f, 1.0f, -5.0f, 5.0f);
            Matrix.multiplyMM(mMVPMatrix, 0, mPMatrix, 0, mMVMatrix, 0);
            
            // Reallocate textures
            // Initialize mRenderTex[0] with the fixed setup
            mFixedPixels = new byte[w*h*3];
            for (int i = 0; i < w; ++i) {
                mFixedPixels[(w*h/2 + i)*3] = (byte) 255;
            }
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mRenderTex[0]);
            ByteBuffer texBuffer = ByteBuffer.allocateDirect(mFixedPixels.length).order(ByteOrder.nativeOrder());
            texBuffer.put(mFixedPixels);
            texBuffer.position(0);
            GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGB, w, h, 0,
                    GLES20.GL_RGB, GLES20.GL_UNSIGNED_BYTE, texBuffer);
            // Initialize mRenderTex[1] with a blank buffer
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mRenderTex[1]);
            byte[] buf = new byte[w*h*3];
            ByteBuffer blankBuffer = ByteBuffer.allocateDirect(buf.length).order(ByteOrder.nativeOrder());
            blankBuffer.put(buf);
            blankBuffer.position(0);
            GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGB, w, h, 0,
                    GLES20.GL_RGB, GLES20.GL_UNSIGNED_BYTE, blankBuffer);
        }
        
        @Override
        public void onDrawFrame(GL10 gl) {
            //Log.i("TheElementsShader", "onDrawFrame");

            // Bind the frame buffer, so we draw into the render tex first
            GLUtils.bindFramebufferColor(mFrameBuf[0], mRenderTex[1-mCurTex]);
            GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
            
            // SURFACE 1 -- Draw from the storage tex into the curTex
            mSurface1.setTex(mRenderTex[mCurTex]);
            mSurface1.draw(mMVPMatrix);
            
            if (mTouchState == TouchState.DOWN) {
                // Put the touch rect around the touch location
                float x = 2*mTouchX/w - 1.0f;
                float y = 1.0f - 2*mTouchY/h; // touch uses top-left as (0,0)
                float rx = 10.0f / w;
                float ry = 10.0f / h;
                mTouchRect.move(y - ry, x - rx, y + ry, x + rx);
                mTouchRect.draw(mMVPMatrix);
            }
            
            // Now unbind the frame buffer and draw our texture as usual
            GLUtils.unbindFramebuffer();
            GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
            
            // SURFACE 2 -- Drawn onto screen using framebuffer tex (curTex)
            mSurface2.setTex(mRenderTex[1-mCurTex]);
            mSurface2.draw(mMVPMatrix);
            
            // Flip textures
            mCurTex = 1 - mCurTex;
        }
    }
}