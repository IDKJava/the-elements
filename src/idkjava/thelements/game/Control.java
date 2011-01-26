package idkjava.thelements.game;

import idkjava.thelements.MainActivity;
import idkjava.thelements.R;
import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.SeekBar;

public class Control extends LinearLayout
{
	//The instance of the current activity is stored here and modified through setActvity (call from DemoActivity)
	private MainActivity activity;
	
	//Two objects in the control area
	private ImageButton control_button;
	private SeekBar brush_size_slider;
	
	final CharSequence[] elementslist = {"Sand", "Water", "Plant", "Wall", "Fire", "Ice", "Generator", "Oil", "Magma", "Stone", "C4"};
	
	//Constructor
	public Control(Context context, AttributeSet attrs)
	{ 
		super(context, attrs);
	}

	//Sets the current instance of the activity
	public void setActivity(MainActivity act)
	{
		activity = act;
	}

	//Called once the the xml is finished inflating	
	@Override
	protected void onFinishInflate()
	{
		//Define the ImageButton and SeekBar set before using the res ids
		control_button = (ImageButton) findViewById(R.id.element_picker_button);
		brush_size_slider = (SeekBar) findViewById(R.id.brush_size_slider);

		//Set a click listener for the button which should pop up element picker dialog when clicked
		control_button.setOnClickListener
			(
					new OnClickListener()
					{
						public void onClick(View v)
						{
							activity.showDialog(2); //Run the element picker dialog
						}
					}
			);
		//Set a palette image for the button
		control_button.setImageResource(R.drawable.palette);

		//Set a change listener for the seekbar
		brush_size_slider.setOnSeekBarChangeListener
			(
					new SeekBar.OnSeekBarChangeListener()
					{
						public void onProgressChanged(SeekBar seekbar, int progress, boolean fromTouch)
						{
							//When it is dragged, set the brush size to 32 * the fraction of the bar dragged
							int p = 32 * progress/seekbar.getMax();
							MainActivity.setBrushSize((char) p);
						}
						//These aren't needed for now
						public void onStartTrackingTouch(SeekBar seekbar) {}
						public void onStopTrackingTouch(SeekBar seekbar) {}
					}
			);
		//Start off the progress bar at a brush size of 4
		brush_size_slider.setProgress((int)4*brush_size_slider.getMax()/32);
	}
}
