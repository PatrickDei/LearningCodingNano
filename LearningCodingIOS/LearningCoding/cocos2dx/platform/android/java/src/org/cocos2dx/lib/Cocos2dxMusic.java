/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.lib;

import java.io.FileInputStream;
import java.io.IOException;

import com.android.vending.expansion.zipfile.APKExpansionSupport;
import com.android.vending.expansion.zipfile.ZipResourceFile;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.util.Log;

public class Cocos2dxMusic {
	// ===========================================================
	// Constants
	// ===========================================================

	private static final String TAG = Cocos2dxMusic.class.getSimpleName();

	// ===========================================================
	// Fields
	// ===========================================================

	private final Context mContext;
	private MediaPlayer mBackgroundMediaPlayer;
	private float mLeftVolume;
	private float mRightVolume;
	private boolean mPaused;
	private String mCurrentPath;

	
	private static ZipResourceFile zip_resource_file = null;

	// ===========================================================
	// Constructors
	// ===========================================================

	public Cocos2dxMusic(final Context pContext) {
		this.mContext = pContext;
		((Cocos2dxActivity) pContext).setVolumeControlStream(AudioManager.STREAM_MUSIC);
        try {
            //Change the second argument to match with your version code
            zip_resource_file = APKExpansionSupport.getAPKExpansionZipFile(pContext, Cocos2dxHelper.getCodeVersion(), 0);
        } catch ( IOException e ) {
        	Log.e( "Cocos2dxMusic" ,  "Error initialising ZipResourceFile: ", e );
        }

		this.initData();
	}
	

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	// ===========================================================
	// Methods
	// ===========================================================
	
	public boolean isLollipop() 
	{
		Log.d("androidVersion ", String.valueOf(android.os.Build.VERSION.SDK_INT));
	
		if(android.os.Build.VERSION.SDK_INT>=21) {
			return true;
		}
		return false;
	} 
	

	public void preloadBackgroundMusic(final String pPath) {
		if ((this.mCurrentPath == null) || (!this.mCurrentPath.equals(pPath))) {
			// preload new background music

			// release old resource and create a new one
			if (this.mBackgroundMediaPlayer != null) {
				this.mBackgroundMediaPlayer.release();
			}

			this.mBackgroundMediaPlayer = this.createMediaplayer(pPath);

			// record the path
			this.mCurrentPath = pPath;
		}
	}

	public void playBackgroundMusic(final String pPath, final boolean isLoop) {
		if (this.mCurrentPath == null) {
			// it is the first time to play background music or end() was called
			this.mBackgroundMediaPlayer = this.createMediaplayer(pPath);
			this.mCurrentPath = pPath;
		} else {
			if (!this.mCurrentPath.equals(pPath)) {
				// play new background music

				// release old resource and create a new one
				if (this.mBackgroundMediaPlayer != null) {
					this.mBackgroundMediaPlayer.release();
				}
				this.mBackgroundMediaPlayer = this.createMediaplayer(pPath);

				// record the path
				this.mCurrentPath = pPath;
			}
		}

		if (this.mBackgroundMediaPlayer == null) {
			Log.e(Cocos2dxMusic.TAG, "playBackgroundMusic: background media player is null");
		} else {
			// if the music is playing or paused, stop it
			if(!isLollipop())
			{
				this.mBackgroundMediaPlayer.stop();
			}

			this.mBackgroundMediaPlayer.setLooping(isLoop);
			
			Log.v(TAG,"playBackgroundMusic("+pPath+")");
			try {
				
				
				if(!isLollipop())
				{
					this.mBackgroundMediaPlayer.prepare();
				}
				
				this.mBackgroundMediaPlayer.seekTo(0);
				this.mBackgroundMediaPlayer.start();
				
				this.mPaused = false;
			} catch (final Exception e) {
				Log.e(Cocos2dxMusic.TAG, "playBackgroundMusic: error state");
			}
		}
	}

	public void stopBackgroundMusic() {
		try {
			if (this.mBackgroundMediaPlayer != null) {
				this.mBackgroundMediaPlayer.stop();
				
				if(isLollipop())
				{
					this.mBackgroundMediaPlayer.release();
					this.mBackgroundMediaPlayer = null;
					this.mCurrentPath = null;
				}

				// should set the state, if not, the following sequence will be error
				// play -> pause -> stop -> resume
				this.mPaused = false;
			}
		} catch (Exception e) {
			e.printStackTrace();
			// TODO: handle exception
		}
	}

	public void pauseBackgroundMusic() {
	   try {
			if (this.mBackgroundMediaPlayer != null && this.mBackgroundMediaPlayer.isPlaying()) {
				this.mBackgroundMediaPlayer.pause();
				this.mPaused = true;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void resumeBackgroundMusic() {
		try 
		{
			if (this.mBackgroundMediaPlayer != null && !this.mBackgroundMediaPlayer.isPlaying()) {
				this.mBackgroundMediaPlayer.start();
				this.mPaused = false;
			}
		} catch (IllegalStateException e) {
			 e.printStackTrace();
		}
	}

	public void rewindBackgroundMusic() {
		if (this.mBackgroundMediaPlayer != null) {
			if(!isLollipop())
			{
				this.mBackgroundMediaPlayer.stop();
			}
			try {
				if(!isLollipop())
				{
					this.mBackgroundMediaPlayer.prepare();
				}
				else
				{
					this.mBackgroundMediaPlayer.pause();
				}
				this.mBackgroundMediaPlayer.seekTo(0);
				this.mBackgroundMediaPlayer.start();

				this.mPaused = false;
			} catch (final Exception e) {
				Log.e(Cocos2dxMusic.TAG, "rewindBackgroundMusic: error state");
			}
		}
	}

	public boolean isBackgroundMusicPlaying() {
		boolean ret = false;

		if (this.mBackgroundMediaPlayer == null) {
			ret = false;
		} else {
			ret = this.mBackgroundMediaPlayer.isPlaying();
		}

		return ret;
	}

	public void end() {
		if (this.mBackgroundMediaPlayer != null) {
			this.mBackgroundMediaPlayer.release();
		}

		this.initData();
	}

	public float getBackgroundVolume() {
		if (this.mBackgroundMediaPlayer != null) {
			return (this.mLeftVolume + this.mRightVolume) / 2;
		} else {
			return 0.0f;
		}
	}

	public void setBackgroundVolume(float pVolume) {
		if (pVolume < 0.0f) {
			pVolume = 0.0f;
		}

		if (pVolume > 1.0f) {
			pVolume = 1.0f;
		}

		this.mLeftVolume = this.mRightVolume = pVolume;
		if (this.mBackgroundMediaPlayer != null) {
			this.mBackgroundMediaPlayer.setVolume(this.mLeftVolume, this.mRightVolume);
		}
	}

	private void initData() {
		this.mLeftVolume = 0.5f;
		this.mRightVolume = 0.5f;
		this.mBackgroundMediaPlayer = null;
		this.mPaused = false;
		this.mCurrentPath = null;
	}

	/**
	 * create mediaplayer for music
	 * 
	 * @param pPath
	 *            the pPath relative to assets
	 * @return
	 */
	
	private MediaPlayer createMediaplayer(final String pPath) {
		MediaPlayer mediaPlayer = new MediaPlayer();
		AssetFileDescriptor assetFileDescriptor;
		try {
			assetFileDescriptor =  zip_resource_file.getAssetFileDescriptor( "assets/soundEffects" + pPath );
		} catch (Exception e) {
			assetFileDescriptor = null;
		}
	   
		try {
			if (pPath.startsWith("/")) {
				final FileInputStream fis = new FileInputStream(pPath);
				mediaPlayer.setDataSource(fis.getFD());
				fis.close();
			} else {
				if (assetFileDescriptor == null) 
				{
					assetFileDescriptor = this.mContext.getAssets().openFd(pPath);
				}
				mediaPlayer.setDataSource(assetFileDescriptor.getFileDescriptor(), assetFileDescriptor.getStartOffset(), assetFileDescriptor.getLength());
			}

			mediaPlayer.prepare();

			mediaPlayer.setVolume(this.mLeftVolume, this.mRightVolume);
		} catch (final Exception e) {
			mediaPlayer = null;
			Log.e(Cocos2dxMusic.TAG, "error: " + e.getMessage(), e);
		}

		return mediaPlayer;
	}

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}
