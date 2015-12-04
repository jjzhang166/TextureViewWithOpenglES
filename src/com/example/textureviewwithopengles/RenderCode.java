package com.example.textureviewwithopengles;

public class RenderCode implements GLRender.GLCallBack{

	static
	{
		System.loadLibrary("GLRenderCode");
	}
	@Override
	public void initGL() {
		// TODO Auto-generated method stub
		InitGL();
	}

	@Override
	public void resizeGL(int width, int height) {
		// TODO Auto-generated method stub
		ResizeGL(width, height);
	}

	@Override
	public void paintGL() {
		// TODO Auto-generated method stub
		PaintGL();
	}
	public static native void InitGL();
	public static native void ResizeGL(int width,int height);
	public static native void PaintGL();
}
