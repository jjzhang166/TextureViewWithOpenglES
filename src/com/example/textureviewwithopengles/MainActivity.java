package com.example.textureviewwithopengles;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.TextureView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.SeekBar;

public class MainActivity extends Activity {
TextureView textureView;
GLRender glRender;
Button playButton;
Button pauseButton;
Button stopButton;
SeekBar progressSeekBar;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		textureView=(TextureView)findViewById(R.id.gltextureView);
		
		playButton=(Button)findViewById(R.id.playButton);
		pauseButton=(Button)findViewById(R.id.pauseButton);
		stopButton=(Button)findViewById(R.id.stopButton);
		progressSeekBar=(SeekBar)findViewById(R.id.progressSeekBar);
		playButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				glRender.Play();
			}
		});
		pauseButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				glRender.Pause();
			}
		});
		stopButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				glRender.Stop();
			}
		});
		
		glRender=new GLRender();
		textureView.setSurfaceTextureListener(glRender);
		glRender.setGLCallBack(new RenderCode());
		glRender.start();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
