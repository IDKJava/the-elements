package idkjava.thelements.game;

import java.io.File;
import java.io.FilenameFilter;

import android.util.Log;

public class SaveManager
{
	public static String[] saveFiles;
	public static final File saveDir = new File(FileManager.ROOT_DIR + FileManager.SAVES_DIR);
	
	public static void refresh()
	{
		//Filter the files in the directory to exclude the demo and temp saves
		FilenameFilter filter = new FilenameFilter()
		{
			public boolean accept(File dir, String name)
			{
				return !(name.contentEquals("demo.sav") || name.contentEquals("temp.sav"));
			}
		};
		//Get the array of filenames
		saveFiles = saveDir.list(filter);
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
	public static void saveState(String filename)
	{
		saveState(filename.toCharArray());
	}
	public static void loadState(String filename)
	{
		loadState(filename.toCharArray());
	}
	
	public static boolean fileExists(String filename)
	{
		File file = new File(saveDir, filename);
		return file.exists();
	}
	
	public native static char saveState(char[] saveLoc);
	public native static char loadState(char[] loadLoc);
	
	static
	{
		System.loadLibrary("thelements");
	}
}
