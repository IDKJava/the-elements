package idkjava.thelements.game;

import idkjava.thelements.MainActivity;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

public class SaveManager
{
	public static ArrayList<CharSequence> saveList;
	public static String[] saveFiles;
	public static final File saveDir = new File(FileManager.ROOT_DIR + FileManager.SAVES_DIR);
	
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
		System.loadLibrary("libthelements.so");
	}
}
