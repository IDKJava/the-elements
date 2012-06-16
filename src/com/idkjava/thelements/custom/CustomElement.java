package com.idkjava.thelements.custom;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

import com.idkjava.thelements.game.FileManager;

public class CustomElement
{
	private String mFilename; // What the is the filename?
	private int mCopy; // Which numerical tag are we at? (for duplicate element names) -- Not used right now
	private boolean mValid; // Does the file exist, and is it valid?
	private boolean mLoaded; // Have the properties been loaded?
	
	// Properties
	public String name;
	public int baseElementIndex;
	public int state;
	public int startingTemp;
	public int lowestTemp;
	public int highestTemp;
	public int lowerElementIndex;
	public int higherElementIndex;
	public int red;
	public int green;
	public int blue;
	public int density;
	public int fallVel;
	public int inertia;
	public ArrayList<Integer> collisions;
	
	public CustomElement(String filename)
	{
		mFilename = filename;
		mCopy = 0;
		mValid = loadNameFromFile();
		mLoaded = false;
	}
	public String getFilename()
	{
		return mFilename;
	}
	public int getCopy()
	{
		return mCopy;
	}
	public boolean isValid()
	{
		return mValid;
	}
	public boolean isLoaded()
	{
		return mLoaded;
	}
	private boolean loadNameFromFile()
	{
		File fp = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR + mFilename  + FileManager.ELEMENT_EXT);
		try
		{
			BufferedReader br = new BufferedReader(new InputStreamReader(new DataInputStream(new FileInputStream(fp))));
			name = br.readLine();
		}
		catch (FileNotFoundException e)
		{
			e.printStackTrace();
			return false;
		}
		catch (IOException e)
		{
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	public boolean loadPropertiesFromFile()
	{
		File fp = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR + mFilename + FileManager.ELEMENT_EXT);
		try
		{
			BufferedReader br = new BufferedReader(new InputStreamReader(new DataInputStream(new FileInputStream(fp))));
			name = br.readLine();
			baseElementIndex = Integer.parseInt(br.readLine());
			state = Integer.parseInt(br.readLine());
			startingTemp = Integer.parseInt(br.readLine());
			lowestTemp = Integer.parseInt(br.readLine());
			highestTemp = Integer.parseInt(br.readLine());
			lowerElementIndex = Integer.parseInt(br.readLine());
			higherElementIndex = Integer.parseInt(br.readLine());
			red = Integer.parseInt(br.readLine());
			green = Integer.parseInt(br.readLine());
			blue = Integer.parseInt(br.readLine());
			density = Integer.parseInt(br.readLine());
			fallVel = Integer.parseInt(br.readLine());
			inertia = Integer.parseInt(br.readLine());
			String collisionString;
			collisions = new ArrayList<Integer>();
			while((collisionString = br.readLine()) != null)
			{
				collisions.add(Integer.parseInt(collisionString));
			}
		}
		catch (NumberFormatException e)
		{
			e.printStackTrace();
			return false;
		}
		catch (FileNotFoundException e)
		{
			e.printStackTrace();
			return false;
		}
		catch (IOException e)
		{
			e.printStackTrace();
			return false;
		}
		mLoaded = true;
		mValid = true;
		return true;
	}
	
	public boolean writeToFile()
	{
		// Look for an available file location
		File fp = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR + mFilename + FileManager.ELEMENT_EXT);
		/*
		if (!mValid)
		{
			mCopy = 0;
			while(fp.exists())
			{
				mCopy++;
				fp = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR + mFilename + String.valueOf(mCopy) + FileManager.ELEMENT_EXT);
			}
		}
		*/
		
		// Now write our properties to it
		try
		{
			FileWriter writer = new FileWriter(fp);
			BufferedWriter out = new BufferedWriter(writer);
			
			out.write(name);
			out.newLine();
			out.write(String.valueOf(baseElementIndex));
			out.newLine();
			out.write(String.valueOf(state));
			out.newLine();
			out.write(String.valueOf(startingTemp));
			out.newLine();
			out.write(String.valueOf(lowestTemp));
			out.newLine();
			out.write(String.valueOf(highestTemp));
			out.newLine();
			out.write(String.valueOf(lowerElementIndex));
			out.newLine();
			out.write(String.valueOf(higherElementIndex));
			out.newLine();
			out.write(String.valueOf(red));
			out.newLine();
			out.write(String.valueOf(green));
			out.newLine();
			out.write(String.valueOf(blue));
			out.newLine();
			out.write(String.valueOf(density));
			out.newLine();
			out.write(String.valueOf(fallVel));
			out.newLine();
			out.write(String.valueOf(inertia));
			
			// TODO: Advanced
			
			out.close();
		}
		catch (IOException e)
		{
			return false;
		}
		
		mValid = true;
		return true;
	}
}
