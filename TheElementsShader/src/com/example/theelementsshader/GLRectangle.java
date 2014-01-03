package com.example.theelementsshader;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class GLRectangle extends GLShape {
    public GLRectangle(float bottom, float left, float top, float right) {
        // Create arrays
        float[] verts = {
            bottom, left, 0.0f,
            bottom, right, 0.0f,
            top, left, 0.0f,
            top, right, 0.0f
        };
        float[] texCoords = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f
        };
        short[] indices = {0, 1, 2, 2, 1, 3};
        mVertices = verts;
        mTexCoords = texCoords;
        mIndices = indices;
        
        // Create the buffers
        mVertexBuffer = ByteBuffer.allocateDirect(mVertices.length * Float.SIZE/Byte.SIZE)
                .order(ByteOrder.nativeOrder()).asFloatBuffer();
        mVertexBuffer.put(mVertices);
        mVertexBuffer.position(0);
        mTexCoordBuffer = ByteBuffer.allocateDirect(mTexCoords.length * Float.SIZE/Byte.SIZE)
                .order(ByteOrder.nativeOrder()).asFloatBuffer();
        mTexCoordBuffer.put(mTexCoords);
        mTexCoordBuffer.position(0);
        mIndexBuffer = ByteBuffer.allocateDirect(mIndices.length * Short.SIZE/Byte.SIZE)
                .order(ByteOrder.nativeOrder()).asShortBuffer();
        mIndexBuffer.put(mIndices);
        mIndexBuffer.position(0);
    }
}
