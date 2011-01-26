package idkjava.thelements.game;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;

public class SaveManager
{
	public static ArrayList<CharSequence> saveList;
	public static String[] saveFiles;
	public static final File saveDir = new File(FileManager.ROOT_DIR + FileManager.SAVES_DIR);
	
	public static void loadAtmospheres()
	{
		//Clear everything first
		saveList.clear();
		
		//Create the FilenameFilter to accept only *.sav files
		FilenameFilter filter = new FilenameFilter()
		{
			public boolean accept(File dir, String filename)
			{
				if(filename.matches(FileManager.DEMO_SAVE + FileManager.SAVE_EXT))
				{
					return false;
				}
				return filename.endsWith(FileManager.SAVE_EXT);
			}
		};
		//TODO(UNIMPORTANT): not sure if this creates a small memory leak or not
		saveFiles = saveDir.list(filter);
		
		for(int i = 0; i < saveFiles.length; i++)
		{
			//TODO: get the save name and story in saveList
		}
	}
	
	public static void selectSave(int index)
	{
		loadState(saveFiles[index].toCharArray());
	}
	
	public native static char saveState(char[] saveLoc);
	public native static char loadState(char[] loadLoc);
	
	static
	{
		System.loadLibrary("libthelements.so");
	}
}
