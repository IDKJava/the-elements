package idkjava.thelements.preferences;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.TypedArray;
import android.graphics.Typeface;
import android.preference.Preference;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class SeekBarPreference extends Preference implements OnSeekBarChangeListener
{
	public static int MAXIMUM = 255;
	
	private float oldValue = 0;
	private TextView monitorBox;
	
	public SeekBarPreference(Context context)
	{
		super(context);
	}
	public SeekBarPreference(Context context, AttributeSet attrs)
	{
		super(context, attrs);
	}
	public SeekBarPreference(Context context, AttributeSet attrs, int defStyle)
	{
		super(context, attrs, defStyle);
	}
	
	@Override
	protected View onCreateView(ViewGroup parent)
	{
		LinearLayout layout = new LinearLayout(getContext());
		
		LinearLayout.LayoutParams params1 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
		params1.gravity = Gravity.LEFT;
		params1.weight = 1.0f;
		
		LinearLayout.LayoutParams params2 = new LinearLayout.LayoutParams(200, LinearLayout.LayoutParams.WRAP_CONTENT);
		params2.gravity = Gravity.RIGHT;
		
		LinearLayout.LayoutParams params3 = new LinearLayout.LayoutParams(50, LinearLayout.LayoutParams.WRAP_CONTENT);
		params3.gravity = Gravity.CENTER;
		
		
		layout.setPadding(15, 15, 10, 15);
		layout.setOrientation(LinearLayout.HORIZONTAL);
		
		TextView view = new TextView(getContext());
		view.setText(getTitle());
		view.setTextSize(24);
		view.setTypeface(Typeface.SANS_SERIF, Typeface.NORMAL);
		view.setGravity(Gravity.LEFT);
		view.setLayoutParams(params1);
		
		
		SeekBar bar = new SeekBar(getContext());
		bar.setMax(MAXIMUM);
		bar.setProgress((int)oldValue);
		bar.setLayoutParams(params2);
		bar.setOnSeekBarChangeListener(this);
		
		monitorBox = new TextView(getContext());
		monitorBox.setTextSize(12);
		monitorBox.setTypeface(Typeface.MONOSPACE, Typeface.NORMAL);
		monitorBox.setLayoutParams(params3);
		monitorBox.setPadding(5, 5, 0, 0);
		monitorBox.setText(bar.getProgress()+"");
		
		
		layout.addView(view);
		layout.addView(bar);
		layout.addView(monitorBox);
		layout.setId(android.R.id.widget_frame);
		
		
		return layout;
	}
	
	@Override
	public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser)
	{
		//Log.v("TheElements", "onProgressChanged(): " + progress);
	    oldValue = progress;
	    monitorBox.setText(progress+"");
	}

	@Override
	public void onStartTrackingTouch(SeekBar seekBar)
	{
		//Log.v("TheElements", "onStartTrackingTouch()");
	}

	@Override
	public void onStopTrackingTouch(SeekBar seekBar)
	{
		//Log.v("TheElements", "onStopTrackingTouch()");

	    updatePreference((int) oldValue);
	  
	    notifyChanged();
	}
	 
	 
	@Override 
	protected Object onGetDefaultValue(TypedArray ta, int index)
	{
		 int dValue = (int)ta.getInt(index, 50);
		 return validateValue(dValue);
	}
	 

	@Override
	protected void onSetInitialValue(boolean restoreValue, Object defaultValue)
	{
		int temp = restoreValue ? getPersistedInt(50) : (Integer)defaultValue;
	     
		if(!restoreValue)
			persistInt(temp);
	     
	    oldValue = temp;
	}
	 
	 
	private int validateValue(int value)
	{
		if(value > MAXIMUM)
		{
			value = MAXIMUM;
		}
	    else if(value < 0)
	    {
	    	value = 0;
	    }
		
		return value;  
	}
	    
	    
	private void updatePreference(int newValue)
	{
		 SharedPreferences.Editor editor =  getEditor();
		 editor.putInt(getKey(), newValue);
		 editor.commit();
	}
}
