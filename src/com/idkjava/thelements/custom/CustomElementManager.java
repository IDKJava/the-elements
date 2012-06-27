package com.idkjava.thelements.custom;

import java.io.File;
import java.util.ArrayList;

import com.idkjava.thelements.R;
import com.idkjava.thelements.game.FileManager;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

public class CustomElementManager
{
	private static ArrayList<CustomElement> sCustomElements = new ArrayList<CustomElement>();
	private static final File elementDir = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR);
	
	public static void refresh(Context c)
	{
		// Clear the old array
		sCustomElements.clear();
		//Get the array of filenames
		String[] elementFiles = elementDir.list();
		if (elementFiles == null)
		{
			Log.w("TheElements", "Warning: No sdcard found!");
			Toast.makeText(c.getApplicationContext(), R.string.sdcard_not_found, Toast.LENGTH_SHORT).show();
			elementFiles = new String[0];
		}
		Log.v("TheElements", "CustomElementManager refreshed, files found: " + elementFiles.length);
		for(int i = 0; i < elementFiles.length; i++)
		{
			if (elementFiles[i].endsWith(FileManager.ELEMENT_EXT))
			{
				Log.v("TheElements", "..." + elementFiles[i]);
				// Cut off the element extension when saving the filename
				sCustomElements.add(new CustomElement(elementFiles[i].substring(0, elementFiles[i].length()-FileManager.ELEMENT_EXT.length())));
			}
		}
	}
	public static ArrayList<CustomElement> getElementList()
	{
		return sCustomElements;
	}
}
