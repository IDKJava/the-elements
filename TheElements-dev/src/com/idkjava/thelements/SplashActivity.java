package com.idkjava.thelements;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import androidx.fragment.app.FragmentActivity;

import com.idkjava.thelements.game.FileManager;

public class SplashActivity extends FragmentActivity
{
    private TextView mLoadingText; 
    
	@Override
	protected void onCreate (Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState); //Call the super method
		
		setContentView(R.layout.splash_activity);
		mLoadingText = (TextView) findViewById(R.id.loading_text);
		new Thread()
		{
		    @Override
		    public void run()
		    {
		        maybeUpgradeFiles();
		        
		        try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {}
		        
                //Create an Intent to start MainActivity
                Intent mainIntent = new Intent(SplashActivity.this, SelectWorldActivity.class);
                startActivity(mainIntent);
                finish();
		    }
		}.start();
	}

	private String getStr(int resId) {
		return getResources().getString(resId);
	}
	
	private void setTextOnUiThread(final String text) {
	    runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mLoadingText.setText(text);
			}
		});
	}

	private void copy(File src, File dst) throws IOException {
		InputStream in = new FileInputStream(src);
		OutputStream out = new FileOutputStream(dst);

		// Transfer bytes from in to out
		byte[] buf = new byte[1024];
		int len;
		while ((len = in.read(buf)) > 0) {
			out.write(buf, 0, len);
		}
		in.close();
		out.close();
	}

	private void maybeUpgradeFiles()
	{
	    String oldText = mLoadingText.getText().toString();
	    setTextOnUiThread(getStr(R.string.upgrading_save_files));
	    try {
            Thread.sleep(100);
        } catch (InterruptedException e) {}

		FileManager.ROOT_DIR = getFilesDir().getAbsolutePath() + "/";
		Log.d("TheElements", "Got internal storage path: " + FileManager.ROOT_DIR);

		// Check directories exist
		File rootDir = new File(FileManager.ROOT_DIR);
		File eltDir = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR);
		eltDir.mkdirs();
		File saveDir = new File(FileManager.ROOT_DIR + FileManager.SAVES_DIR);
		saveDir.mkdirs();
		Log.d("TheElements", "Root exists: " + rootDir.exists());
		Log.d("TheElements", "Elt exists: " + eltDir.exists());
		Log.d("TheElements", "Elt can write: " + eltDir.canWrite());
		Log.d("TheElements", "Saves exists: " + saveDir.exists());

	    File elementDir = new File(FileManager.OLD_ROOT_DIR + FileManager.ELEMENTS_DIR);
	    File[] elementFiles = elementDir.listFiles();
	    if (elementFiles != null) {
			for (File elementFile : elementFiles) {
				if (elementFile.getAbsolutePath().endsWith(FileManager.ELEMENT_EXT)) {
					Log.i("TheElements", "Upgrading " + elementFile.getAbsolutePath());
					setTextOnUiThread(getStr(R.string.upgrading) + elementFile.getAbsolutePath());
					upgradeCustomElement(elementFile.getAbsolutePath());
					File elementFileBak = new File(elementFile.getAbsolutePath() + FileManager.BACKUP_EXT);
					elementFile.renameTo(elementFileBak);
					try {
						Thread.sleep(100);
					} catch (InterruptedException e) {
					}
				}
			}
		}

		// TODO: Test upgrade
		// Move elements from fixed external storage to internal storage
		elementFiles = elementDir.listFiles();
		if (elementFiles != null) {
			String newEltDir = FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR;
			for (File elementFile : elementFiles) {
				if (elementFile.getAbsolutePath().endsWith(FileManager.ELEMENT2_EXT)) {
					File n = new File(newEltDir + elementFile.getName());
					try {
						copy(elementFile, n);
						elementFile.delete();
					}
					catch (IOException e) {
						Log.e("TheElements", "Move failed: " + elementFile.getAbsolutePath()
								+ " to: " + n.getAbsolutePath());
					}
				}
			}
	    }
	    
	    File savesDir = new File(FileManager.OLD_ROOT_DIR + FileManager.SAVES_DIR);
	    File[] saveFiles = savesDir.listFiles();
	    if (saveFiles != null)
	    {
	        for (File saveFile : saveFiles)
	        {
	            if (saveFile.getAbsolutePath().endsWith(FileManager.SAVE_EXT))
	            {
	                Log.i("TheElements", "Upgrading " + saveFile.getAbsolutePath());
	                setTextOnUiThread(getStr(R.string.upgrading) + saveFile.getAbsolutePath());
	                upgradeSaveFile(saveFile.getAbsolutePath());
	                File saveFileBak = new File(saveFile.getAbsolutePath() + FileManager.BACKUP_EXT);
	                saveFile.renameTo(saveFileBak);
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {}
	            }
	        }
	    }

		// Move saves from fixed external storage to internal storage
		saveFiles = savesDir.listFiles();
		if (saveFiles != null) {
			String newSaveDir = FileManager.ROOT_DIR + FileManager.SAVES_DIR;
			for (File saveFile : saveFiles) {
				Log.d("TheElements", "Moving old save: " + saveFile.getAbsolutePath());
				if (saveFile.getAbsolutePath().endsWith(FileManager.SAVE2_EXT)) {
					File n = new File(newSaveDir + saveFile.getName());
					try {
						copy(saveFile, n);
						saveFile.delete();
					}
					catch (IOException e) {
						Log.e("TheElements", "Move failed: " + saveFile.getAbsolutePath()
								+ " to: " + n.getAbsolutePath());
					}
				}
			}
		}

		// Set native internal storage root
		Log.d("TheElements", "Set root dir: " + FileManager.ROOT_DIR);
		setRootDir(FileManager.ROOT_DIR);
	    
	    setTextOnUiThread(oldText);
	}
    
    private static native boolean upgradeCustomElement(String filename);
    private static native boolean upgradeSaveFile(String filename);
	private static native void setRootDir(String newRoot);
    
    static
    {
        System.loadLibrary("thelements"); // Load the JNI library (libthelements.so)
    }
}
