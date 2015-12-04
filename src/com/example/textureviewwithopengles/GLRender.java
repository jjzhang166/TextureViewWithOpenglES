package com.example.textureviewwithopengles;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import android.graphics.SurfaceTexture;
import android.opengl.EGL14;
import android.opengl.EGLConfig;
import android.opengl.EGLContext;
import android.opengl.EGLDisplay;
import android.opengl.EGLSurface;
import android.util.Log;
import android.view.TextureView;

public class GLRender extends Thread implements TextureView.SurfaceTextureListener {
	private SurfaceTexture mSurfaceTexture = null;
	private EGLDisplay mEGLDisplay = EGL14.EGL_NO_DISPLAY;
	private EGLContext mEGLContext = EGL14.EGL_NO_CONTEXT;
	private EGLSurface mEGLSurface = EGL14.EGL_NO_SURFACE;
	private EGLConfig mEGLConfig = null;
	private Lock mutex = null;
	private Condition condition = null;
    private boolean shouldDestroyGLEnvironment=false;
    private GLCallBack glCallBack=null;
	public GLRender() {
		super("GLTextureView Render Thread");
		mutex = new ReentrantLock(false);
		condition = mutex.newCondition();
	}
   
	public void Play()
    {
    	
    	
    }
	public void Pause()
	{
		
		
	}
	public void Stop()
	{
		
		
	}
	
	
	private void initGLEnvironment() {
		if (mEGLDisplay != EGL14.EGL_NO_DISPLAY || mEGLContext != EGL14.EGL_NO_CONTEXT || mEGLSurface != EGL14.EGL_NO_SURFACE) {
			return;
		}
		mutex.lock();
		if (mSurfaceTexture == null) {
			try {
				condition.await();
			} catch (InterruptedException e) {
			}
		}
		mEGLDisplay = EGL14.eglGetDisplay(EGL14.EGL_DEFAULT_DISPLAY);
		int[] version = new int[2];
		EGL14.eglInitialize(mEGLDisplay, version, 0, version, 0);
		int renderableType = EGL14.EGL_OPENGL_ES2_BIT;
		int[] attribList = { EGL14.EGL_RED_SIZE, 8, EGL14.EGL_GREEN_SIZE, 8, EGL14.EGL_BLUE_SIZE, 8, EGL14.EGL_ALPHA_SIZE, 8, EGL14.EGL_DEPTH_SIZE,24, EGL14.EGL_RENDERABLE_TYPE, renderableType, EGL14.EGL_NONE };
		EGLConfig[] configs = new EGLConfig[1];
		int[] numConfigs = new int[1];
		EGL14.eglChooseConfig(mEGLDisplay, attribList, 0, configs, 0, configs.length, numConfigs, 0);
		EGLConfig config = configs[0];
		int[] attrib2_list = { EGL14.EGL_CONTEXT_CLIENT_VERSION, 2, EGL14.EGL_NONE };
		EGLContext context = EGL14.eglCreateContext(mEGLDisplay, config, EGL14.EGL_NO_CONTEXT, attrib2_list, 0);
		mEGLContext = context;
		mEGLConfig = config;
		int[] surfaceAttribs = { EGL14.EGL_NONE };
		mEGLSurface = EGL14.eglCreateWindowSurface(mEGLDisplay, mEGLConfig, mSurfaceTexture, surfaceAttribs, 0);
		EGL14.eglMakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext);
		mutex.unlock();
	}

	@Override
	public void run() {
		while (true) {
			initGLEnvironment();
			glBlock();
			if(shouldDestroyGLEnvironment)
				destroyGLEnvironment();
		try {
			Thread.sleep(33);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		}
	}
    private void initGL()
    {
    	if(glCallBack!=null)
    		glCallBack.initGL();
    }
    private void resizeGL(int width,int height)
    {
    	if(glCallBack!=null)
    		glCallBack.resizeGL(width,height);
    }
    private void paintGL()
    {
    	if(glCallBack!=null)
    		glCallBack.paintGL();
    }
	public void destroyGLEnvironment() {
		mutex.lock();
		EGL14.eglDestroySurface(mEGLDisplay, mEGLSurface);
		mEGLSurface = EGL14.EGL_NO_SURFACE;
		EGL14.eglMakeCurrent(mEGLDisplay, EGL14.EGL_NO_SURFACE, EGL14.EGL_NO_SURFACE, EGL14.EGL_NO_CONTEXT);
		EGL14.eglDestroyContext(mEGLDisplay, mEGLContext);
		EGL14.eglReleaseThread();
		EGL14.eglTerminate(mEGLDisplay);
		mEGLDisplay = EGL14.EGL_NO_DISPLAY;
		mEGLContext = EGL14.EGL_NO_CONTEXT;
		mEGLConfig = null;
		shouldDestroyGLEnvironment=false;
		mutex.unlock();
	}

	@Override
	public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
		// TODO Auto-generated method stub
		Log.i("TextureViewTEST", "surface available");
		mutex.lock();
	    mSurfaceTexture = surface;
		condition.signalAll();
		mutex.unlock();
	}

	@Override
	public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
		Log.i("TextureViewTEST", "surface destroyed");
		mutex.lock();
		shouldDestroyGLEnvironment=true;
		mSurfaceTexture.release();
		mSurfaceTexture=null;
		mutex.unlock();
		return true;
	}

	@Override
	public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
		Log.i("TextureViewTEST", "surface size changed");
	}

	@Override
	public void onSurfaceTextureUpdated(SurfaceTexture surface) {
		Log.i("TextureViewTEST", "surface updated");
	}

	private void glBlock() {
		int[] value = new int[1];
		EGL14.eglQuerySurface(mEGLDisplay, mEGLSurface, EGL14.EGL_WIDTH, value, 0);
		int width = value[0];
		EGL14.eglQuerySurface(mEGLDisplay, mEGLSurface, EGL14.EGL_HEIGHT, value, 0);
		int height = value[0];
        initGL();
        resizeGL(width,height);
		while (true) {
			mutex.lock();
			if(shouldDestroyGLEnvironment)
			{
			  return;
			}
			paintGL();
			EGL14.eglSwapBuffers(mEGLDisplay, mEGLSurface);
			mutex.unlock();
		}
	}
	
	public void setGLCallBack(GLCallBack glCallBack)
	{
		this.glCallBack=glCallBack;
	}
	
    interface GLCallBack
    {
      void initGL();
      void resizeGL(int width,int height);
      void paintGL();
    }
    
}
