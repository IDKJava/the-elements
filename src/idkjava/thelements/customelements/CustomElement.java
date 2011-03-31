package idkjava.thelements.customelements;

public class CustomElement {
	private String _name;
	private char[] _reactions;
	private boolean _enabled;
	
	public CustomElement(String name, char[] reactions)
	{
		_name = name;
		_reactions = reactions;
	}
	public String getName()
	{
		return _name;
	}
	public char[] getReactions()
	{
		return _reactions;
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
