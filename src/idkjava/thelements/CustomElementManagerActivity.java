package idkjava.thelements;

import android.app.Activity;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.view.Window;

public class CustomElementManagerActivity extends Activity
{
	//TODO(MASSIVE,ARMORED): This should display all loaded Custom Elements and allow selection,
	//editing, and creation
	public void onCreate(Bundle icicle)
	{
		super.onCreate(icicle);
		setContentView(R.layout.custom_element_manager);
		Window window = getWindow();
	    // Eliminates color banding
	    window.setFormat(PixelFormat.RGBA_8888);

		
		
	}
}