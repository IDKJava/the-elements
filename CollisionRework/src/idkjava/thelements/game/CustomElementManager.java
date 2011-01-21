package idkjava.thelements.game;

import java.util.ArrayList;

public class CustomElementManager
{
	public static ArrayList<CharSequence> customElementList;
	private static ArrayList<CustomElement> customElements;
	
	public static void addCustomElement(int index)
	{
		CustomElement newCustomElement = new CustomElement();
		newCustomElement.index = index;
		newCustomElement.name = getElementName(index).toString();
		customElements.add(newCustomElement);
	}
	public static void saveCustomElement(CustomElement custom)
	{
		customElements.add(custom);
	}
		
	//TODO: This class should manage the loading, saving, and indexing of custom
	//elements
	
	public native static char[] getElementName(int index);
	
	static
	{
		System.loadLibrary("libthelements.so");
	}
}
