package com.example.theelementsshader;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class GLRectangle extends GLShape {
    public GLRectangle(float bottom, float left, float top, float right) {
        // Set vertices
        move(bottom, left, top, right);
        
        // Create arrays
        float[] texCoords = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f
        };
        short[] indices = {0, 1, 2, 2, 1, 3};
        mTexCoords = texCoords;
        mIndices = indices;
        
        // Create the buffers
        mTexCoordBuffer = ByteBuffer.allocateDirect(mTexCoords.length * Float.SIZE/Byte.SIZE)
                .order(ByteOrder.nativeOrder()).asFloatBuffer();
        mTexCoordBuffer.put(mTexCoords);
        mTexCoordBuffer.position(0);
        mIndexBuffer = ByteBuffer.allocateDirect(mIndices.length * Short.SIZE/Byte.SIZE)
                .order(ByteOrder.nativeOrder()).asShortBuffer();
        mIndexBuffer.put(mIndices);
        mIndexBuffer.position(0);
    }
    
    public void move(float bottom, float left, float top, float right) {
        // Update mVertexBuffer
        float[] verts = {
             left, bottom, 0.0f,
             right, bottom, 0.0f,
             left, top, 0.0f,
             right, top, 0.0f
        };
        mVertices = verts;
        mVertexBuffer = ByteBuffer.allocateDirect(mVertices.length * Float.SIZE/Byte.SIZE)
                .order(ByteOrder.nativeOrder()).asFloatBuffer();
        mVertexBuffer.put(mVertices);
        mVertexBuffer.position(0);
    }
}
