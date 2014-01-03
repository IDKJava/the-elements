package com.example.theelementsshader;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.AttributeSet;
import android.util.Log;

import com.example.thelementsshader.util.Util;

public class SandView extends GLSurfaceView {

    private SandViewRenderer mRenderer;
    
    public SandView(Context context, AttributeSet attrs) {
        super(context);
        // Enable OpenGL ES 2.0
        setEGLContextClientVersion(2);
        mRenderer = new SandViewRenderer();
        setRenderer(mRenderer);
    }


    protected class SandViewRenderer implements GLSurfaceView.Renderer {

        private int mProg1, mProg2;
        private int[] mFrameBuf = new int[1];
        private int[] mRenderTex = new int[1];
        private int[] mFixedTex = new int[1];
        
        private byte[] mFixedPixels = {
            (byte) 255, 0, 0,
            0, (byte) 255, 0,
            0, 0, (byte) 255,
            (byte) 255, (byte) 255, 0
        };

        private float[] mVertices = {
            -1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f
        };
        private float[] mTexCoords = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f
        };
        private FloatBuffer mVertexBuffer;
        private FloatBuffer mTexCoordBuffer;
        private int mVertexStride = 3 * Float.SIZE / Byte.SIZE;
        private int mTexCoordStride = 2 * Float.SIZE / Byte.SIZE;
        private short[] mQuad = {0, 1, 2, 1, 2, 3};
        private ShortBuffer mQuadBuffer;

        private float[] mMVMatrix = new float[16];
        private float[] mPMatrix = new float[16];
        private float[] mMVPMatrix = new float[16];
        
        private int w, h;
        
        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            Log.i("TheElementsShader", "onSurfaceCreated");
            
            // Vertex buffer
            mVertexBuffer = ByteBuffer.allocateDirect(mVertices.length * Float.SIZE/Byte.SIZE)
                    .order(ByteOrder.nativeOrder()).asFloatBuffer();
            mVertexBuffer.put(mVertices);
            mVertexBuffer.position(0);
            
            // Tex coord buffer
            mTexCoordBuffer = ByteBuffer.allocateDirect(mTexCoords.length * Float.SIZE/Byte.SIZE)
                    .order(ByteOrder.nativeOrder()).asFloatBuffer();
            mTexCoordBuffer.put(mTexCoords);
            mTexCoordBuffer.position(0);
            
            // Triangle index buffer
            mQuadBuffer = ByteBuffer.allocateDirect(mQuad.length * Integer.SIZE/Byte.SIZE)
                    .order(ByteOrder.nativeOrder()).asShortBuffer();
            mQuadBuffer.put(mQuad);
            mQuadBuffer.position(0);
            
            // Render texture
            GLES20.glGenFramebuffers(1, mFrameBuf, 0);
            GLES20.glGenTextures(1, mRenderTex, 0);
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mRenderTex[0]);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S,
                    GLES20.GL_CLAMP_TO_EDGE);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T,
                    GLES20.GL_CLAMP_TO_EDGE);
            int[] buf = new int[480*640];
            IntBuffer texBuffer = ByteBuffer.allocateDirect(buf.length * Integer.SIZE/Byte.SIZE)
                    .order(ByteOrder.nativeOrder()).asIntBuffer();
            GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGB, 640, 480, 0, GLES20.GL_RGB,
                    GLES20.GL_UNSIGNED_SHORT_5_6_5, texBuffer);
            
            // Fixed texture (DEBUG)
            ByteBuffer pixels = ByteBuffer.allocateDirect(4 * 3).order(ByteOrder.nativeOrder());
            pixels.put(mFixedPixels);
            pixels.position(0);
            GLES20.glGenTextures(1, mFixedTex, 0);
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mFixedTex[0]);
            GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGB, 2, 2,
                    0, GLES20.GL_RGB, GLES20.GL_UNSIGNED_BYTE, pixels);
            
            // Two shader programs
            mProg1 = GLES20.glCreateProgram();
            mProg2 = GLES20.glCreateProgram();

            int vertShader1 = loadShader(GLES20.GL_VERTEX_SHADER, R.raw.vert1, "vertShader1");
            int fragShader1 = loadShader(GLES20.GL_FRAGMENT_SHADER, R.raw.frag1, "fragShader1");
            GLES20.glAttachShader(mProg1, vertShader1);
            GLES20.glAttachShader(mProg1, fragShader1);
            linkProgram(mProg1, "mProg1");
            
            int vertShader2 = loadShader(GLES20.GL_VERTEX_SHADER, R.raw.vert2, "vertShader2");
            int fragShader2 = loadShader(GLES20.GL_FRAGMENT_SHADER, R.raw.frag2, "fragShader2");
            GLES20.glAttachShader(mProg2, fragShader2);
            GLES20.glAttachShader(mProg2, vertShader2);
            linkProgram(mProg2, "mProg2");
            
            //DEBUG:
            GLES20.glDisable(GLES20.GL_CULL_FACE);
            
            // Build fixed MV matrix
            Matrix.setLookAtM(mMVMatrix, 0,
                    0, 0, -3, // eye
                    0, 0, 0, // center
                    0, 1, 0); // up
        }
        
        

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            w = width;
            h = height;
            GLES20.glViewport(0, 0, w, h);
            float ratio = (float) width / height;
            // Set MP Matrix
            Matrix.orthoM(mPMatrix, 0, -1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);
            Matrix.multiplyMM(mMVPMatrix, 0, mPMatrix, 0, mMVMatrix, 0);
        }
        
        @Override
        public void onDrawFrame(GL10 gl) {
            //Log.i("TheElementsShader", "onDrawFrame");

            // Bind the frame buffer, so we draw into the render tex first
            GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, mFrameBuf[0]);
            GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0,
                    GLES20.GL_TEXTURE_2D, mRenderTex[0], 0);
            checkGlError("glBindFramebuffer");
            
            GLES20.glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
            
            // PROGRAM 1 -- Drawn into framebuffer tex
            GLES20.glUseProgram(mProg1);
            checkGlError("glUseProgram 1");
            
            GLES20.glEnableVertexAttribArray(GLES20.glGetAttribLocation(mProg1, "position"));
            GLES20.glVertexAttribPointer(GLES20.glGetAttribLocation(mProg1, "position"), 3,
                    GLES20.GL_FLOAT, false, mVertexStride, mVertexBuffer);
            GLES20.glUniformMatrix4fv(GLES20.glGetUniformLocation(mProg1, "uMVPMatrix"),
                    1, false, mMVPMatrix, 0);
            
            GLES20.glDrawElements(GLES20.GL_TRIANGLES, 6, GLES20.GL_UNSIGNED_SHORT, mQuadBuffer);
            checkGlError("glDrawElements");
            GLES20.glDisableVertexAttribArray(GLES20.glGetAttribLocation(mProg1, "position"));

            
            // Now unbind the frame buffer and draw our texture as usual
            GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
            checkGlError("glBindFramebuffer (unbind)");
            GLES20.glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
            
            // PROGRAM 2 -- Drawn onto screen using framebuffer tex
            GLES20.glUseProgram(mProg2);
            checkGlError("glUseProgram 2");
            
            GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mRenderTex[0]);
            //GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mFixedTex[0]);
            // Set filtering
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST);
//            ByteBuffer pixels = ByteBuffer.allocateDirect(4 * 3).order(ByteOrder.nativeOrder());
//            pixels.put(mFixedPixels);
//            pixels.position(0);
//            GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGB, 2, 2,
//                    0, GLES20.GL_RGB, GLES20.GL_UNSIGNED_BYTE, pixels);
            GLES20.glUniform1i(GLES20.glGetUniformLocation(mProg2, "tex"), 0);
            checkGlError("glBindTexture");
            GLES20.glEnableVertexAttribArray(GLES20.glGetAttribLocation(mProg2, "position"));
            GLES20.glVertexAttribPointer(GLES20.glGetAttribLocation(mProg2, "position"), 3,
                    GLES20.GL_FLOAT, false, mVertexStride, mVertexBuffer);
            GLES20.glEnableVertexAttribArray(GLES20.glGetAttribLocation(mProg2, "aTexCoord"));
            GLES20.glVertexAttribPointer(GLES20.glGetAttribLocation(mProg2, "aTexCoord"), 2,
                    GLES20.GL_FLOAT, false, mTexCoordStride, mTexCoordBuffer);
            GLES20.glUniformMatrix4fv(GLES20.glGetUniformLocation(mProg2, "uMVPMatrix"),
                    1, false, mMVPMatrix, 0);
            
            GLES20.glDrawElements(GLES20.GL_TRIANGLES, 6, GLES20.GL_UNSIGNED_SHORT, mQuadBuffer);
            checkGlError("glDrawElements 2");
            GLES20.glDisableVertexAttribArray(GLES20.glGetAttribLocation(mProg2, "position"));
            GLES20.glDisableVertexAttribArray(GLES20.glGetAttribLocation(mProg2, "aTexCoord"));
        }
        
        private int loadShader(int type, int rawRes, String id) {
            int[] compiled = new int[1];
            int shader = GLES20.glCreateShader(type);
            GLES20.glShaderSource(shader, Util.readRawTextFile(getContext(), rawRes));
            GLES20.glCompileShader(shader);
            GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
            if (compiled[0] == 0) {
                Log.e("TheElementsShader", "Could not compile " + id);
                String log = GLES20.glGetShaderInfoLog(shader);
                Log.e("TheElementsShader", log);
                throw new RuntimeException("Error compiling " + id + ": " + log);
            }
            return shader;
        }
        
        private void linkProgram(int prog, String id) {
            int[] linked = new int[1];
            GLES20.glLinkProgram(prog);
            GLES20.glGetProgramiv(prog, GLES20.GL_LINK_STATUS, linked, 0);
            if (linked[0] == 0) {
                Log.e("TheElementsShader", "Could not link " + id);
                String log = GLES20.glGetProgramInfoLog(prog);
                Log.e("TheElementsShader", log);
                throw new RuntimeException("Error linking " + id + ": " + log);
            }
        }
        
        private void checkGlError(String op) {
            int error;
            while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
                Log.e("TheElementsShader", op + ": glError " + error);
                throw new RuntimeException(op + ": glError " + error);
            }
        }
    }
}