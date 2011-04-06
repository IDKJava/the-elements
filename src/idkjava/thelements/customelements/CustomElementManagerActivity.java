package idkjava.thelements.customelements;

import idkjava.thelements.R;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.view.Window;
import android.widget.ListView;

/**
 * File format:
 * NAME\n
 * COLLISION ARRAY (space separated)\n
 * SPECIALS ARRAY (space separated)
 */
public class CustomElementManagerActivity extends Activity
{
	 List<CustomElement> elementList = new ArrayList<CustomElement>();
	//TODO(MASSIVE,ARMORED): This should display all loaded Custom Elements and allow selection,
	//editing, and creation
	public void onCreate(Bundle icicle)
	{
		super.onCreate(icicle);
		setContentView(R.layout.custom_element_manager);
		initElements();
		Window window = getWindow();
	    // Eliminates color banding
	    window.setFormat(PixelFormat.RGBA_8888);
	    
	    ListView elementListView = (ListView) findViewById(R.id.customListView);
	    //Test elements
	    char[] charArray = {4, 5};
	    //CustomElement testElement = new CustomElement("TestElement", charArray, charArray);
	    //writeFile(testElement);
	    initElements();
	    CustomElementArrayAdapter arrayAdapter = new CustomElementArrayAdapter(this, R.layout.custom_element_edit_item, elementList);
	    //TODO: Implement storage of elements, opening of elements, and use of them in the c-backend
		elementListView.setAdapter(arrayAdapter);
		
	}
	private void initElements()
	{
		elementList.clear();
		File file = new File("/sdcard/thelements/elements/");
		if(file.exists())
		{
			File[] elementFiles = file.listFiles();
			for(int i = 0; i < elementFiles.length; i++)
			{
				readFile(elementFiles[i]);
			}
		}
		else
		{
			file.mkdirs();
		}
	}
	private void readFile(File elementFile)//stores a CustomElement to elementList based on the passed file
	{
		byte[] b = new byte[450];
		String fileString;
		try
		{
		FileInputStream fis = new FileInputStream(elementFile);
		fis.read(b);
		fileString = new String(b);
		}
		catch(IOException e){return;}
		
		String[] data = fileString.split("\n");//data[0] = name, data[1] = collision array, data[2] = specials array
		
		String[] collisionsStrings= data[1].split(" ");
		char[] collisions = new char[collisionsStrings.length];
		for(int i = 0; i < collisions.length; i++)
		{
			collisions[i] = collisionsStrings[i].charAt(0);
		}
		String[] specialsStrings= data[2].split(" ");
		char[] specials = new char[specialsStrings.length];
		for(int i = 0; i < collisions.length; i++)
		{
			specials[i] = specialsStrings[i].charAt(0);
		}
		elementList.add(new CustomElement(data[0], collisions, specials));
	}
	private void writeFile(CustomElement customElement)//Writes a save file for the passed CustomElement
	{
		File writeFile = new File("/sdcard/thelements/elements/" + customElement.getName());
		try
		{
			FileWriter writeF = new FileWriter(writeFile);
			String name = customElement.getName();
			String collisions = new String();
			char[] collisionsChars = customElement.getCollisions();
			String specials = new String();
			char[] specialsChars = customElement.getSpecials();
			
			for(int i = 0; i < collisionsChars.length; i++)
			{
				if(i + 1 == collisionsChars.length)
					collisions += collisionsChars[i];
				else
					collisions += collisionsChars[i] + " ";
			}
			for(int i = 0; i < specialsChars.length; i++)
			{
				if(i + 1 == specialsChars.length)
					specials += specialsChars[i];
				else
					specials += specialsChars[i] + " ";
			}
			//add newline chars
			name += "\n";
			collisions += "\n";
			writeF.append(name + collisions + specials);
			writeF.flush();
			writeF.close();
		}
		catch(IOException e){return;}
		
	}
	private void deleteFile(CustomElement customElement)
	{
		File file = new File("/sdcard/thelements/elements/" + customElement.getName());
		if(file.exists())
			file.delete();
	}
}