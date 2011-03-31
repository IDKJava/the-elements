package idkjava.thelements.customelements;

import idkjava.thelements.R;

import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ToggleButton;

public class CustomElementArrayAdapter extends ArrayAdapter {
	private int _fieldId = 0;
	private LayoutInflater  _inflater;
	private int _resource;
	private List<CustomElement> _objects;
	private Context _context;
	
	public CustomElementArrayAdapter(Context context, int resource, List<CustomElement> objects)
	{
		super(context, resource, objects);
		_inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		_resource = resource;
		_objects = objects;
		_context = context;
	}
	
	public View getView(final int position, View convertView, ViewGroup parent)
	{
		final int pos = position;
		final List<CustomElement> _finalObjects = _objects;
		convertView = _inflater.inflate(R.layout.custom_element, parent, false);
	    convertView.setBackgroundResource(android.R.drawable.menuitem_background);
	    
		final ToggleButton elementToggle = (ToggleButton) convertView.findViewById(R.id.customToggle);
		elementToggle.setChecked(_objects.get(pos).getState());
		final Button editButton = (Button) convertView.findViewById(R.id.editButton);
		
		
	    convertView.setOnClickListener(new OnClickListener(){
	    
		@Override
		public void onClick(View v)
		{
			if(v == editButton)
				//Start edit activity
			if(v == elementToggle)
				_finalObjects.get(pos).toggle();
		}

		});
		
		
		
		
		return convertView;
	}
	public int getViewTypeCount()
	{
		return 3;
	}
}

