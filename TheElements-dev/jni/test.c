// viewport should match texture size
Matrix.frustumM(mProjMatrix, 0, -ratio, ratio, -1, 1, 0.5f, 10);
GLES20.glViewport(0, 0, this.texW, this.texH);
 
// Bind the framebuffer
GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, fb[0]);
 
// specify texture as color attachment
GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0, GLES20.GL_TEXTURE_2D, renderTex[0], 0);
 
// attach render buffer as depth buffer
GLES20.glFramebufferRenderbuffer(GLES20.GL_FRAMEBUFFER, GLES20.GL_DEPTH_ATTACHMENT, GLES20.GL_RENDERBUFFER, depthRb[0]);
 
// check status
int status = GLES20.glCheckFramebufferStatus(GLES20.GL_FRAMEBUFFER);
if (status != GLES20.GL_FRAMEBUFFER_COMPLETE)
    return false;
 
// Clear the texture (buffer) and then render as usual...
GLES20.glClearColor(.0f, .0f, .0f, 1.0f);
GLES20.glClear( GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);
 
....usual rendering continues....

// Bind the default framebuffer (to render to the screen) - indicated by '0'
GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
 
GLES20.glClearColor(.0f, .0f, .0f, 1.0f);
GLES20.glClear( GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);
 
...setup all the matrices as usual....
 
// send the vertex coordinates
_qvb.position(TRIANGLE_VERTICES_DATA_POS_OFFSET);
GLES20.glVertexAttribPointer(GLES20.glGetAttribLocation(_program, "aPosition"), 3, GLES20.GL_FLOAT, false,
        TRIANGLE_VERTICES_DATA_STRIDE_BYTES, _qvb);
GLES20.glEnableVertexAttribArray(GLES20.glGetAttribLocation(_program, "aPosition"));
 
// the normal info (though shouldn't be needed)
_qvb.position(TRIANGLE_VERTICES_DATA_NOR_OFFSET);
GLES20.glVertexAttribPointer(GLES20.glGetAttribLocation(_program, "aNormal"), 3, GLES20.GL_FLOAT, false,
        TRIANGLE_VERTICES_DATA_STRIDE_BYTES, _qvb);
GLES20.glEnableVertexAttribArray(GLES20.glGetAttribLocation(_program, "aNormal"));
 
// bind the framebuffer texture - this is what will be attached to the quad
GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, renderTex[0]);
GLES20.glUniform1i(GLES20.glGetUniformLocation(_program, "texture1"), 0);
 
// enable texturing? [fix - sending float is waste]
GLES20.glUniform1f(GLES20.glGetUniformLocation(_program, "hasTexture"), 1.0f);
 
// texture coordinates
_qvb.position(TRIANGLE_VERTICES_DATA_TEX_OFFSET);
GLES20.glVertexAttribPointer(GLES20.glGetAttribLocation(_program, "textureCoord"), 2, GLES20.GL_FLOAT, false,
        TRIANGLE_VERTICES_DATA_STRIDE_BYTES, _qvb);
GLES20.glEnableVertexAttribArray(GLES20.glGetAttribLocation(_program, "textureCoord"));//GLES20.glEnableVertexAttribArray(shader.maTextureHandle);
 
// Draw with indices
GLES20.glDrawElements(GLES20.GL_TRIANGLES, _quadi.length, GLES20.GL_UNSIGNED_INT, _qib);
