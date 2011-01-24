package idkjava.thelements.game;

import idkjava.thelements.R;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.Context;

public class FileManager
{
	//File and directory constants
	public static final String ROOT_DIR = "/sdcard/thelements/";
	public static final String SAVES_DIR = "saves/";
	public static final String ELEMENTS_DIR = "elements/";
	public static final String ATMOSPHERES_DIR = "atmospheres/";
	public static final String SAVE_EXT = ".sav";
	public static final String ELEMENT_EXT = ".ele";
	public static final String ATMOSPHERE_EXT = ".atm";
	public static final String DEMO_SAVE = "demo";
	
	public static void intialize(Context context)
	{		
		try
		{
			//Try to create the folders
			(new File(ROOT_DIR)).mkdir();
			(new File(ROOT_DIR + SAVES_DIR)).mkdir();
			(new File(ROOT_DIR + ELEMENTS_DIR)).mkdir();
			(new File(ROOT_DIR + ATMOSPHERES_DIR)).mkdir();

			if(!(new File(ROOT_DIR + SAVES_DIR + DEMO_SAVE + SAVE_EXT)).exists())
			{
				//Set the input stream to the demo save resource
				InputStream in = context.getResources().openRawResource(R.raw.demo);
				//Set the output stream to the demo save file location
				OutputStream out = new FileOutputStream(ROOT_DIR + SAVES_DIR + DEMO_SAVE);
				//Create a 256 byte buffer
				byte[] buf = new byte[256];
				//Read in 256 byte chunks from in and output to out
				int len;
				while ((len = in.read(buf)) != -1)
				{
					out.write(buf, 0, len);
				}
				//Close both streams
				in.close();
				out.close();
			}
		}
		catch (FileNotFoundException e)
		{
			//FileNotFoundException is normal, ignore it
			e.printStackTrace();
		}
		catch (IOException e)
		{
			//IOException is also fine, ignore
			e.printStackTrace();
		}
	}
}
