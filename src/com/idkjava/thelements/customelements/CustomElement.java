package idkjava.thelements.customelements;

public class CustomElement {
	private String _name;
	private char[] _collisions;
	private char[] _specials;
	
	private boolean _enabled;
	
	public CustomElement(String name, char[] collisions, char[] specials)
	{
		_name = name;
		_collisions = collisions;
		_specials = specials;
	}
	public String getName()
	{
		return _name;
	}
	public char[] getSpecials()
	{
		return _specials;
	}
	public void setSpecials(char[] in)
	{
		_specials = in;
	}
	public char[] getCollisions()
	{
		return _collisions;
	}
	public void setCollisions(char[] in)
	{
		_collisions = in;
	}
	public void toggle()
	{
		if(_enabled)
			_enabled = false;
		else
			_enabled = true;
	}
	public boolean getState()
	{
		return _enabled;
	}
	public void setState(boolean state)
	{
		_enabled = state;
	}
}
