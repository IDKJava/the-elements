package idkjava.thelements.game;

import java.util.ArrayList;

public class CustomElementManager
{
	public static ArrayList<CharSequence> customElementList;
	
	public static void reloadCustomElements()
	{
		customElementList.clear();
		int numLoaded = loadCustomElements();
		for(int i = 0; i < numLoaded; i++)
		{
			customElementList.add(getCustomElementName(i).toString());
		}
	}
	
	public native static char[] getCustomElementName(int index);
	public native static int loadCustomElements();
	
	static
	{
		System.loadLibrary("libthelements.so");
	}
}
