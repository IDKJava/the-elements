package com.example.theelementsshader;

import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import android.content.Context;
import android.opengl.GLES20;

public abstract class GLShape {
    protected static final int VERTEX_STRIDE = 3 * Float.SIZE/Byte.SIZE;
    protected static final int TEX_COORD_STRIDE = 2 * Float.SIZE/Byte.SIZE;
    protected float[] mVertices;
    protected float[] mTexCoords;
    protected short[] mIndices;
    protected FloatBuffer mVertexBuffer;
    protected FloatBuffer mTexCoordBuffer;
    protected ShortBuffer mIndexBuffer;
    protected int mTex = -1;
    private float[] mColor = {-1.0f, -1.0f, -1.0f, -1.0f};
    protected int mProg, mFrag, mVert;
    protected String mProgId;
    
    public void setTex(int tex) {
        mTex = tex;
    }
    public void unsetTex() {
        mTex = -1;
    }
    
    public void setColor(float red, float green, float blue, float alpha) {
        mColor[0] = red;
        mColor[1] = green;
        mColor[2] = blue;
        mColor[3] = alpha;
    }
    public void unsetColor() {
        mColor[0] = -1.0f;
        mColor[1] = -1.0f;
        mColor[2] = -1.0f;
        mColor[3] = -1.0f;
    }
    
    public void setProg(int rawVert, int rawFrag, Context c, String id) {
        mProgId = id;
        mProg = GLES20.glCreateProgram();
        mVert = GLUtils.loadShader(GLES20.GL_VERTEX_SHADER, rawVert, c, mProgId + ": vert");
        mFrag = GLUtils.loadShader(GLES20.GL_FRAGMENT_SHADER, rawFrag, c, mProgId + ": frag");
        GLES20.glAttachShader(mProg, mVert);
        GLES20.glAttachShader(mProg, mFrag);
        GLUtils.linkProgram(mProg, mProgId);
    }
    
    // Override to bind additional shader attribs
    protected void onBindShaderAttribs() {}
    // Override to change draw action
    protected void onDrawAction() {
        // Set filtering
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST);
        
        // Draw
        GLES20.glDrawElements(GLES20.GL_TRIANGLES, 6, GLES20.GL_UNSIGNED_SHORT, mIndexBuffer);
        GLUtils.checkGlError("glDrawElements");
    }
    // Override to clean up any additional shader attrib bindings
    protected void onDisableShaderAttribs() {}
    
    public void draw(float[] mvpMatrix) {
        GLES20.glUseProgram(mProg);
        GLUtils.checkGlError("glUseProgram " + mProgId);
        
        // Set up tex if enabled
        if (mTex >= 0) {
            GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mTex);

            int tex = GLES20.glGetUniformLocation(mProg, "uTex");
            GLES20.glUniform1i(tex, 0);
        }
        
        // Set color if enabled
        if (mColor[0] >= 0 && mColor[1] >= 0 && mColor[2] >= 0 && mColor[3] >= 0) {
            int color = GLES20.glGetUniformLocation(mProg, "uColor");
            GLES20.glUniform4fv(color, 1, mColor, 0);
        }
        
        // Set up bindings
        int pos = GLES20.glGetAttribLocation(mProg, "aPos");
        int texcoord = GLES20.glGetAttribLocation(mProg, "aTexCoord");
        int mat = GLES20.glGetUniformLocation(mProg, "uMVPMatrix");
        GLES20.glEnableVertexAttribArray(pos);
        GLES20.glEnableVertexAttribArray(texcoord);
        GLES20.glVertexAttribPointer(pos, 3, GLES20.GL_FLOAT, false, VERTEX_STRIDE, mVertexBuffer);
        GLES20.glVertexAttribPointer(texcoord, 2, GLES20.GL_FLOAT, false, TEX_COORD_STRIDE, mTexCoordBuffer);
        GLES20.glUniformMatrix4fv(mat, 1, false, mvpMatrix, 0);
        onBindShaderAttribs();
        GLUtils.checkGlError("gl bind attribs");
        
        // Draw
        onDrawAction();
        
        // Clean up bindings
        GLES20.glDisableVertexAttribArray(pos);
        GLES20.glDisableVertexAttribArray(texcoord);
        onDisableShaderAttribs();
    }
}