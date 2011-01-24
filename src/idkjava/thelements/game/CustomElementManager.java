package idkjava.thelements.game;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;

public class CustomElementManager
{
	public static ArrayList<CharSequence> customElementList;
	public static String[] customElementFiles;
	public static final File customElementsDir = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR);
	
	public static void loadCustomElements()
	{
		//Clear everything first
		customElementList.clear();
		clearCustomElements();
		
		//Create the FilenameFilter to accept only *.ele files
		FilenameFilter filter = new FilenameFilter()
		{
			public boolean accept(File dir, String filename)
			{
				return filename.endsWith(FileManager.ELEMENT_EXT);
			}
		};
		//TODO(UNIMPORTANT): not sure if this creates a small memory leak or not
		customElementFiles = customElementsDir.list(filter);
		
		for(int i = 0; i < customElementFiles.length; i++)
		{
			//TODO: get the custom element name
			//Tell JNI to load the custom element
			loadCustomElement(customElementFiles[i].toCharArray());
		}
	}
	
	public native static void loadCustomElement(char[] filename);
	public native static void clearCustomElements();
	
	static
	{
		System.loadLibrary("libthelements.so");
	}
}
