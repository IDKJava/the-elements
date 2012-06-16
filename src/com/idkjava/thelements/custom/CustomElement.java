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
	private String mFilename;
	private boolean mValid;
	private boolean mLoaded;
	
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
		mValid = loadNameFromFile();
		mLoaded = false;
	}
	public String getFilename()
	{
		return mFilename;
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
		File fp = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR + mFilename);
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
		File fp = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR + mFilename);
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
		return true;
	}
	
	public boolean writeToFile()
	{
		// Look for an available file location
		File fp = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR + mFilename);
		if (!mLoaded)
		{
			int counter = 0;
			while(fp.exists())
			{
				counter++;
				fp = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR + mFilename + String.valueOf(counter));
			}
			if (counter != 0)
			{
				mFilename = mFilename + String.valueOf(counter);
			}
		}
		
		// Now write our properties to it
		try
		{
			FileWriter writer = new FileWriter(fp);
			BufferedWriter out = new BufferedWriter(writer);
			
			out.write(name);
			out.newLine();
			out.write(baseElementIndex);
			out.newLine();
			out.write(state);
			out.newLine();
			out.write(startingTemp);
			out.newLine();
			out.write(lowestTemp);
			out.newLine();
			out.write(highestTemp);
			out.newLine();
			out.write(lowerElementIndex);
			out.newLine();
			out.write(higherElementIndex);
			out.newLine();
			out.write(red);
			out.newLine();
			out.write(green);
			out.newLine();
			out.write(blue);
			out.newLine();
			out.write(density);
			out.newLine();
			out.write(fallVel);
			out.newLine();
			out.write(inertia);
			
			// TODO: Advanced
			
			out.close();
		}
		catch (IOException e)
		{
			return false;
		}
		
		return true;
	}
}
