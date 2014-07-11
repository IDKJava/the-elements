package com.idkjava.thelements.custom;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

import com.idkjava.thelements.MainActivity;
import com.idkjava.thelements.R;
import com.idkjava.thelements.game.FileManager;
import com.idkjava.thelements.proto.Messages.Collision;
import com.idkjava.thelements.proto.Messages.CustomElement;
import com.idkjava.thelements.proto.Messages.Special;

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
				CustomElement custom;
				try {
				    custom = CustomElement.parseFrom(new FileInputStream(
				            elementDir.getAbsolutePath() + File.separator + elementFiles[i]));
				}
				catch (IOException e) {
				    Log.w("TheElements", "IOException on parsing "
				            + elementDir.getAbsolutePath() + File.separator + elementFiles[i]);
				    continue;
				}

                sCustomElements.add(custom);
			}
		}
	}
	public static ArrayList<CustomElement> getElementList()
	{
		return sCustomElements;
	}
	
	public static void generateUniqueFilename(CustomElement.Builder custom)
	{
	    String name = custom.getName().toLowerCase();
	    File test = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR +
	            name + FileManager.ELEMENT_EXT);
	    if (!test.exists())
	    {
	        custom.setFilename(test.getAbsolutePath());
	    }
	    
	    // Loop through and try adding copy numbers
	    int copy = 1;
	    while (test.exists())
	    {
	        test = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR +
	                name + "(" + copy + ")" + FileManager.ELEMENT_EXT);
	    }
	    custom.setFilename(test.getAbsolutePath());
	}
	
	public static ArrayList<Integer> getCollisionIndexList(CustomElement.Builder custom)
	{
	    ArrayList<Integer> out = new ArrayList<Integer>();
	    for (Collision c : custom.getCollisionList())
	    {
	        out.add((int)c.getType());
	    }
	    return out;
	}
	
	public static ArrayList<Integer> getSpecialsIndexList(CustomElement.Builder custom)
	{
	    ArrayList<Integer> out = new ArrayList<Integer>();
	    for (Special s : custom.getSpecialList())
	    {
	        out.add((int)s.getType());
	    }
	    return out;
	}
	
	public static ArrayList<Integer> getSpecialValsIndexList(CustomElement.Builder custom)
	{
	    ArrayList<Integer> out = new ArrayList<Integer>();
	    for (Special s : custom.getSpecialList())
	    {
	        out.add((int)s.getVal());
	    }
	    return out;
	}
	
	public static int getElementSelectionFromIndex(int index)
	{
	    return index - MainActivity.NORMAL_ELEMENT;
	}
	
	public static int getElementIndexFromSelection(int selection)
	{
	    return selection + MainActivity.NORMAL_ELEMENT;
	}
	
    public static int getCollisionIndexFromPos(int pos)
    {
        // Use this function to do a conversion if ever needed
        return pos;
    }
    public static int getSpecialIndexFromPos(int pos)
    {
        // Use this function to do a conversion if ever needed
        if (pos == 0)
        {
            return -1;
        }
        return pos;
    }
    public static int getSpecialPosFromIndex(int index)
    {
        // Use this function to do a conversion if ever needed
        if (index == -1)
        {
            return 0;
        }
        return index;
    }
}
