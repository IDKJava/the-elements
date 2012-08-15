package com.idkjava.thelements.game;


import java.io.File;
import java.io.FilenameFilter;
import java.io.UnsupportedEncodingException;

import com.idkjava.thelements.MainActivity;
import com.idkjava.thelements.R;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

public class SaveManager
{
	public static String[] saveFiles;
	public static final File saveDir = new File(FileManager.ROOT_DIR + FileManager.SAVES_DIR);
	
	public static void refresh(Context c)
	{
		//Filter the files in the directory to exclude the demo and temp saves
		FilenameFilter filter = new FilenameFilter()
		{
			public boolean accept(File dir, String name)
			{
				return !name.contentEquals("temp.sav");
			}
		};
		//Get the array of filenames
		saveFiles = saveDir.list(filter);
		if (saveFiles == null)
		{
			Toast.makeText(c.getApplicationContext(), R.string.sdcard_not_found, Toast.LENGTH_SHORT).show();
		}
		Log.v("TheElements", "SaveManager refreshed, files found: " + saveFiles.length);
		for(int i = 0; i < saveFiles.length; i++)
		{
			Log.v("TheElements", "..." + saveFiles[i]);
		}
	}
	public static int getNumSaves()
	{
		return saveFiles.length;
	}
	public static String getSaveName(int index)
	{
		return saveFiles[index].replace(FileManager.SAVE_EXT, "");
	}
	
	//Overloading these functions -- be careful
	public static boolean saveState(String statename)
	{
		Log.v("TheElements", "saveState() called: " + FileManager.ROOT_DIR + FileManager.SAVES_DIR + statename + FileManager.SAVE_EXT);
		try
		{
			char retVal = saveState((FileManager.ROOT_DIR + FileManager.SAVES_DIR + statename + FileManager.SAVE_EXT).getBytes("ISO-8859-1"));
			Log.v("LOG", "saveState retVal: " + (int)retVal);
			if(retVal == 0)
			{
				return false;
			}
			
			return true;
		}
		catch (UnsupportedEncodingException e)
		{
			//Hopefully this doesn't happen :P
			e.printStackTrace();
		}
		
		return false;
	}
	public static boolean loadState(String statename)
	{
		//Log.v("TheElements", "loadState() called: " + FileManager.ROOT_DIR + FileManager.SAVES_DIR + filename + FileManager.SAVE_EXT);
		try
		{
			char retVal = loadState((FileManager.ROOT_DIR + FileManager.SAVES_DIR + statename + FileManager.SAVE_EXT).getBytes("ISO-8859-1"));
			Log.v("LOG", "loadState retVal: " + (int)retVal);
			if(retVal == 0)
			{
				Log.e("LOG", "loadState failed -- returning false");
				return false;
			}
			MainActivity.last_state_loaded = statename;
			
			return true;
		}
		catch (UnsupportedEncodingException e)
		{
			//Hopefully this doesn't happen :P
			e.printStackTrace();
		}
		
		return false;
	}
	public static void deleteState(String statename)
	{
		String filename = statename + FileManager.SAVE_EXT;
		if(fileExists(filename))
		{
			File file = new File(saveDir, filename);
			file.delete();
		}
	}
	
	public static boolean fileExists(String filename)
	{
		File file = new File(saveDir, filename);
		return file.exists();
	}
	
	public native static char saveState(byte[] saveLoc);
	public native static char loadState(byte[] loadLoc);
	
	static
	{
		System.loadLibrary("thelements");
	}
}
