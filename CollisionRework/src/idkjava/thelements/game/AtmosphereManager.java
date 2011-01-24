package idkjava.thelements.game;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;

public class AtmosphereManager
{
	public static ArrayList<CharSequence> atmosphereList;
	public static String[] atmosphereFiles;
	public static final File atmosphereDir = new File(FileManager.ROOT_DIR + FileManager.ATMOSPHERES_DIR);
	
	public static void loadAtmospheres()
	{
		//Clear everything first
		atmosphereList.clear();
		
		//Create the FilenameFilter to accept only *.atm files
		FilenameFilter filter = new FilenameFilter()
		{
			public boolean accept(File dir, String filename)
			{
				return filename.endsWith(FileManager.ATMOSPHERE_EXT);
			}
		};
		//TODO(UNIMPORTANT): not sure if this creates a small memory leak or not
		atmosphereFiles = atmosphereDir.list(filter);
		
		for(int i = 0; i < atmosphereFiles.length; i++)
		{
			//TODO: get the atmosphere name
		}
	}
	
	public static void selectAtmosphere(int index)
	{
		loadAtmosphere(atmosphereFiles[index].toCharArray());
	}
	
	public native static void loadAtmosphere(char[] filename);
	
	static
	{
		System.loadLibrary("libthelements.so");
	}
}
