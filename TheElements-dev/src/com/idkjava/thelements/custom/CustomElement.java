package com.idkjava.thelements.custom;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStreamReader;

import com.idkjava.thelements.game.FileManager;

public class CustomElement
{
	private String mFilename;
	private String mName;
	private boolean mValid;
	
	public CustomElement(String filename)
	{
		mFilename = filename;
		mValid = loadNameFromFile();
	}
	public String getName()
	{
		return mName;
	}
	public String getFilename()
	{
		return mFilename;
	}
	public boolean isValid()
	{
		return mValid;
	}
	private boolean loadNameFromFile()
	{
		File fp = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR + mFilename);
		try
		{
			BufferedReader br = new BufferedReader(new InputStreamReader(new DataInputStream(new FileInputStream(fp))));
			//TODO: Read in file format and extract name
		}
		catch (FileNotFoundException e)
		{
			e.printStackTrace();
			return false;
		}
		return true;
	}
	
	//TODO:
	public boolean loadPropertiesFromFile()
	{
		File fp = new File(FileManager.ROOT_DIR + FileManager.ELEMENTS_DIR + mFilename);
		try
		{
			BufferedReader br = new BufferedReader(new InputStreamReader(new DataInputStream(new FileInputStream(fp))));
			//TODO: Read in file format and extract name
		}
		catch (FileNotFoundException e)
		{
			e.printStackTrace();
			return false;
		}
		return true;
	}
}
