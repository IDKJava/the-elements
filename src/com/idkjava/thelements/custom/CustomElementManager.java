package com.idkjava.thelements.custom;

import java.io.File;
import java.util.ArrayList;

import com.idkjava.thelements.game.FileManager;

import android.util.Log;

public class CustomElementManager
{
	private static ArrayList<CustomElement> sCustomElements = new ArrayList<CustomElement>();
	private static final File elementDir = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR);
	
	public static void refresh()
	{
		// Clear the old array
		sCustomElements.clear();
		//Get the array of filenames
		String[] elementFiles = elementDir.list();
		Log.v("TheElements", "CustomElementManager refreshed, files found: " + elementFiles.length);
		for(int i = 0; i < elementFiles.length; i++)
		{
			Log.v("TheElements", "..." + elementFiles[i]);
			sCustomElements.add(new CustomElement(elementFiles[i]));
		}
	}
	public static ArrayList<CustomElement> getElementList()
	{
		return sCustomElements;
	}
}
