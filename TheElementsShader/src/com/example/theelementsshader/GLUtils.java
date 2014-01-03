package com.example.theelementsshader;

import android.content.Context;
import android.opengl.GLES20;
import android.util.Log;

import com.example.thelementsshader.util.Util;

public class GLUtils {
    public static int loadShader(int type, int rawRes, Context c, String id) {
        int[] compiled = new int[1];
        int shader = GLES20.glCreateShader(type);
        GLES20.glShaderSource(shader, Util.readRawTextFile(c, rawRes));
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
    
    public static void linkProgram(int prog, String id) {
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
    
    public static void checkGlError(String op) {
        int error;
        while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
            Log.e("TheElementsShader", op + ": glError " + error);
            throw new RuntimeException(op + ": glError " + error);
        }
    }
    
    // Bind frameBuf as the current framebuffer and set renderTex as the color attachment
    public static void bindFramebufferColor(int frameBuf, int renderTex) {
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, frameBuf);
        GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0,
                GLES20.GL_TEXTURE_2D, renderTex, 0);
        GLUtils.checkGlError("glBindFramebuffer");
        int r;
        if ((r = GLES20.glCheckFramebufferStatus(GLES20.GL_FRAMEBUFFER))
                != GLES20.GL_FRAMEBUFFER_COMPLETE) {
            Log.e("TheElementsShader", "glBindFrameBuffer 1 failed: " + r);
            throw new RuntimeException("glBindFrameBuffer 1 failed: " + r);
        }
    }

    // Unbind the framebuffer to draw to screen
    public static void unbindFramebuffer() {
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
        GLUtils.checkGlError("glBindFramebuffer (unbind)");
        int r;
        if ((r = GLES20.glCheckFramebufferStatus(GLES20.GL_FRAMEBUFFER))
                != GLES20.GL_FRAMEBUFFER_COMPLETE) {
            Log.e("TheElementsShader", "glBindFrameBuffer 2 failed: " + r);
            throw new RuntimeException("glBindFrameBuffer 2 failed: " + r);
        }
    }
}
