package sand.falling.opengl.custom;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import sand.falling.opengl.DemoActivity;
import sand.falling.opengl.R;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.AdapterView.OnItemSelectedListener;

public class CustomMaker extends Activity
{

	private final int TSpinner = 20;

	ArrayAdapter<CharSequence> sandcoladp = null;
	ArrayAdapter<CharSequence> adapter = null;
	Spinner sandcollis = null;
	Spinner watercollis = null;
	Spinner wallcollis = null;
	Spinner plantcollis = null;
	Spinner firecollis = null;
	Spinner oilcollis = null;
	Spinner icecollis = null;
	Spinner magmacollis = null;
	Spinner stonecollis = null;
	Spinner C4collis = null;
	Spinner C4pluscollis = null;
	Spinner fusecollis = null;
	Spinner destwallcollis = null;
	Spinner acidcollis = null;
	Spinner steamcollis = null;
	Spinner saltcollis = null;
	Spinner saltwatercollis = null;
	Spinner glasscollis = null;
	Spinner mudcollis = null;
	Spinner customelecollis = null;

	private final int TSeekBar = 5;
	private SeekBar explo_slider;
	private SeekBar red_1_slider;
	private SeekBar blue_1_slider;
	private SeekBar green_1_slider;
	private SeekBar density_slider;

	private int[] incustom = new int[TSpinner + TSeekBar];
	int[] inFile = new int[TSpinner + TSeekBar];

	@Override
	public void onCreate(Bundle savedInstanceState)
	{

		FileInputStream fis;
		DataInputStream dis;

		try
		{

			File file = new File("/sdcard/elementworks/customeelementsave.txt");
			fis = new FileInputStream(file);
			dis = new DataInputStream(fis);
			for (int i = 0; i < (TSpinner + TSeekBar); ++i)
			{

				try{
					inFile[i] = dis.readInt();
				}catch ( EOFException e){

					e.printStackTrace();
					inFile[i] = 0;

				}

			}

		} catch (IOException e)
		{
			e.printStackTrace();
			for (int i = 0; i < (TSpinner + TSeekBar); ++i)
			{
				inFile[i] = 0;
			}
		}


		super.onCreate(savedInstanceState);
		setContentView(R.layout.custom);

		explo_slider = (SeekBar) findViewById(R.id.explo_slider);
		red_1_slider = (SeekBar) findViewById(R.id.red_1_slider);
		blue_1_slider = (SeekBar) findViewById(R.id.blue_1_slider);
		green_1_slider = (SeekBar) findViewById(R.id.green_1_slider);
		density_slider = (SeekBar) findViewById(R.id.density_slider);

		// set up the Spinners
		sandcollis = (Spinner) findViewById(R.id.sandcol);
		sandcoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		sandcoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		sandcollis.setAdapter(sandcoladp);
		sandcollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		sandcollis.setSelection(inFile[4]);

		watercollis = (Spinner) findViewById(R.id.watercol);
		ArrayAdapter<CharSequence> watercoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		watercoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		watercollis.setAdapter(watercoladp);
		watercollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		watercollis.setSelection(inFile[5]);

		wallcollis = (Spinner) findViewById(R.id.wallcol);
		ArrayAdapter<CharSequence> wallcoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		wallcoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		wallcollis.setAdapter(wallcoladp);
		wallcollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		wallcollis.setSelection(inFile[6]);

		plantcollis = (Spinner) findViewById(R.id.plantcol);
		ArrayAdapter<CharSequence> plantcoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		plantcoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		plantcollis.setAdapter(plantcoladp);
		plantcollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		plantcollis.setSelection(inFile[7]);

		firecollis = (Spinner) findViewById(R.id.firecol);
		ArrayAdapter<CharSequence> firecoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		firecoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		firecollis.setAdapter(firecoladp);
		firecollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		firecollis.setSelection(inFile[8]);

		icecollis = (Spinner) findViewById(R.id.icecol);
		ArrayAdapter<CharSequence> icecoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		icecoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		icecollis.setAdapter(icecoladp);
		icecollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		icecollis.setSelection(inFile[9]);

		oilcollis = (Spinner) findViewById(R.id.oilcol);
		ArrayAdapter<CharSequence> oilcoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		oilcoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		oilcollis.setAdapter(oilcoladp);
		oilcollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		oilcollis.setSelection(inFile[10]);

		magmacollis = (Spinner) findViewById(R.id.magmacol);
		ArrayAdapter<CharSequence> magmacoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		magmacoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		magmacollis.setAdapter(magmacoladp);
		magmacollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		magmacollis.setSelection(inFile[11]);

		stonecollis = (Spinner) findViewById(R.id.stonecol);
		ArrayAdapter<CharSequence> stonecoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		stonecoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		stonecollis.setAdapter(stonecoladp);
		stonecollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		stonecollis.setSelection(inFile[12]);

		C4collis = (Spinner) findViewById(R.id.C4col);
		ArrayAdapter<CharSequence> C4coladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		C4coladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		C4collis.setAdapter(C4coladp);
		C4collis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		C4collis.setSelection(inFile[13]);

		C4pluscollis = (Spinner) findViewById(R.id.C4pluscol);
		ArrayAdapter<CharSequence> C4pluscoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		C4pluscoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		C4pluscollis.setAdapter(C4pluscoladp);
		C4pluscollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		C4pluscollis.setSelection(inFile[14]);

		fusecollis = (Spinner) findViewById(R.id.fusecol);
		ArrayAdapter<CharSequence> fusecoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		fusecoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		fusecollis.setAdapter(fusecoladp);
		fusecollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		fusecollis.setSelection(inFile[15]);

		destwallcollis = (Spinner) findViewById(R.id.destructablewallcol);
		ArrayAdapter<CharSequence> destwallcoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		destwallcoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		destwallcollis.setAdapter(destwallcoladp);
		destwallcollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		destwallcollis.setSelection(inFile[16]);

		acidcollis = (Spinner) findViewById(R.id.acidcol);
		ArrayAdapter<CharSequence> acidcoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		acidcoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		acidcollis.setAdapter(acidcoladp);
		acidcollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		acidcollis.setSelection(inFile[17]);

		steamcollis = (Spinner) findViewById(R.id.steamcol);
		ArrayAdapter<CharSequence> steamcoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		steamcoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		steamcollis.setAdapter(steamcoladp);
		steamcollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		steamcollis.setSelection(inFile[18]);

		saltcollis = (Spinner) findViewById(R.id.saltcol);
		ArrayAdapter<CharSequence> saltcoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		saltcoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		saltcollis.setAdapter(saltcoladp);
		saltcollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		saltcollis.setSelection(inFile[19]);

		saltwatercollis = (Spinner) findViewById(R.id.saltwatercol);
		ArrayAdapter<CharSequence> saltwatercoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		saltwatercoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		saltwatercollis.setAdapter(saltwatercoladp);
		saltwatercollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		saltwatercollis.setSelection(inFile[20]);

		glasscollis = (Spinner) findViewById(R.id.glasscol);
		ArrayAdapter<CharSequence> glasscoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		glasscoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		glasscollis.setAdapter(glasscoladp);
		glasscollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		glasscollis.setSelection(inFile[21]);

		mudcollis = (Spinner) findViewById(R.id.mudcol);
		ArrayAdapter<CharSequence> mudcoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		mudcoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		mudcollis.setAdapter(mudcoladp);
		mudcollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		mudcollis.setSelection(inFile[23]);

		customelecollis = (Spinner) findViewById(R.id.customelecol);
		ArrayAdapter<CharSequence> customelecoladp = ArrayAdapter.createFromResource(this, R.array.reaction_array, android.R.layout.simple_spinner_item);
		customelecoladp.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		customelecollis.setAdapter(customelecoladp);
		customelecollis.setOnItemSelectedListener(new MyOnItemSelectedListener());
		customelecollis.setSelection(inFile[22]);

		explo_slider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
		{
			public void onProgressChanged(SeekBar seekbar, int progress, boolean fromTouch)
			{
				// When it is dragged, set the explosion value to 100 * the fraction of the bar dragged
				int p = 100 * progress / seekbar.getMax();
				DemoActivity.setexplosiveness(p);
				DemoActivity.savecustom();
				inFile[0] = progress;
				savecustomdata();
			}

			// These aren't needed for now
			public void onStartTrackingTouch(SeekBar seekbar)
			{
			}

			public void onStopTrackingTouch(SeekBar seekbar)
			{
			}
		});
		// Start off the progress bar at the value in the file
		explo_slider.setProgress(inFile[0]);
		red_1_slider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
		{
			public void onProgressChanged(SeekBar seekbar, int progress, boolean fromTouch)
			{
				// When it is dragged, set the red value to 255 * the fraction of the bar dragged
				int p = 255 * progress / seekbar.getMax();
				DemoActivity.setred(p);
				DemoActivity.savecustom();
				inFile[1] = progress;
				savecustomdata();
			}

			// These aren't needed for now
			public void onStartTrackingTouch(SeekBar seekbar)
			{
			}

			public void onStopTrackingTouch(SeekBar seekbar)
			{
			}
		});
		// Start off the progress bar at the saved value
		red_1_slider.setProgress(inFile[1]);

		green_1_slider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
		{
			public void onProgressChanged(SeekBar seekbar, int progress, boolean fromTouch)
			{
				// When it is dragged, set the green value to 255 * the fraction of the bar dragged
				int p = 255 * progress / seekbar.getMax();
				DemoActivity.setgreen(p);
				DemoActivity.savecustom();
				inFile[2] = progress;
				savecustomdata();
			}

			// These aren't needed for now
			public void onStartTrackingTouch(SeekBar seekbar)
			{
			}

			public void onStopTrackingTouch(SeekBar seekbar)
			{
			}
		});
		// Start off the progress bar at the value in the file
		green_1_slider.setProgress(inFile[2]);
		blue_1_slider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
		{
			public void onProgressChanged(SeekBar seekbar, int progress, boolean fromTouch)
			{
				// When it is dragged, set the blue value to 255 * the fraction of the bar dragged
				int p = 255 * progress / seekbar.getMax();
				DemoActivity.setblue(p);
				DemoActivity.savecustom();
				inFile[3] = progress;
				savecustomdata();
			}

			// These aren't needed for now
			public void onStartTrackingTouch(SeekBar seekbar)
			{
			}

			public void onStopTrackingTouch(SeekBar seekbar)
			{
			}
		});
		// Start off the progress bar at the value in the file
		blue_1_slider.setProgress(inFile[3]);
		density_slider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
		{
			public void onProgressChanged(SeekBar seekbar, int progress, boolean fromTouch)
			{
				// When it is dragged, set the blue value to 255 * the fraction of the bar dragged
				int p = 100 * progress / seekbar.getMax();
				DemoActivity.setdensity(p);
				DemoActivity.savecustom();
				inFile[4] = progress;
				savecustomdata();
			}

			// These aren't needed for now
			public void onStartTrackingTouch(SeekBar seekbar)
			{
			}

			public void onStopTrackingTouch(SeekBar seekbar)
			{
			}
		});
		// Start off the progress bar at the value in the file
		density_slider.setProgress(inFile[4]);
		DemoActivity.savecustom();
	}

	public class MyOnItemSelectedListener implements OnItemSelectedListener
	{

		public void onItemSelected(AdapterView<?> parent, View view, int pos, long id)
		{

			int ffileindex = 4; // temp var, initialized to 4 to start b.c
			// that's where the first collis is in array
			int rfileindex = 0; // one we'll actually use
			// pos starts at 0
			if (parent.equals(sandcollis))
			{
				rfileindex = ffileindex;
				collisioner(0, pos);
			}
			ffileindex++;
			if (parent.equals(watercollis))
			{
				rfileindex = ffileindex;
				collisioner(1, pos);
			}
			ffileindex++;
			if (parent.equals(wallcollis))
			{
				rfileindex = ffileindex;
				collisioner(2, pos);
			}
			ffileindex++;
			if (parent.equals(plantcollis))
			{
				rfileindex = ffileindex;
				collisioner(4, pos);
			}
			ffileindex++;
			if (parent.equals(firecollis))
			{
				rfileindex = ffileindex;
				collisioner(5, pos);
			}
			ffileindex++;
			if (parent.equals(icecollis))
			{
				rfileindex = ffileindex;
				collisioner(6, pos);
			}
			ffileindex++;
			if (parent.equals(oilcollis))
			{
				rfileindex = ffileindex;
				collisioner(9, pos);
			}
			ffileindex++;
			if (parent.equals(magmacollis))
			{
				rfileindex = ffileindex;
				collisioner(10, pos);
			}
			ffileindex++;
			if (parent.equals(stonecollis))
			{
				rfileindex = ffileindex;
				collisioner(11, pos);
			}
			ffileindex++;
			if (parent.equals(C4collis))
			{
				rfileindex = ffileindex;
				collisioner(12, pos);
			}
			ffileindex++;
			if (parent.equals(C4pluscollis))
			{
				rfileindex = ffileindex;
				collisioner(13, pos);
			}
			ffileindex++;
			if (parent.equals(fusecollis))
			{
				rfileindex = ffileindex;
				collisioner(14, pos);
			}
			ffileindex++;
			if (parent.equals(destwallcollis))
			{
				rfileindex = ffileindex;
				collisioner(15, pos);
			}
			ffileindex++;
			if (parent.equals(acidcollis))
			{
				rfileindex = ffileindex;
				collisioner(17, pos);
			}
			ffileindex++;
			if (parent.equals(steamcollis))
			{
				rfileindex = ffileindex;
				collisioner(18, pos);
			}
			ffileindex++;
			if (parent.equals(saltcollis))
			{
				rfileindex = ffileindex;
				collisioner(19, pos);
			}
			ffileindex++;
			if (parent.equals(saltwatercollis))
			{
				rfileindex = ffileindex;
				collisioner(20, pos);
			}
			ffileindex++;
			if (parent.equals(glasscollis))
			{
				rfileindex = ffileindex;
				collisioner(21, pos);
			}
			ffileindex++;
			if (parent.equals(customelecollis))
			{
				rfileindex = ffileindex;
				collisioner(22, pos);
			}
			ffileindex++;
			if (parent.equals(mudcollis))
			{
				rfileindex = ffileindex;
				collisioner(23, pos);
			}
			ffileindex++;

			inFile[rfileindex] = pos;
			savecustomdata();

		}

		public void onNothingSelected(AdapterView parent)
		{
			// Do nothing.
		}

		public void collisioner(int elent, int pos)
		{ //TODO: change this massive thing to a switch statement

			// setcollision(int custnumber, int elementnumb, int collisionspot,
			// int collisionnumber);
			// which custom element, the element the custom should be like, the
			// element colliding with, collision type
			if (pos == 0)
			{
				DemoActivity.setcollision(1, 0, elent, 0);
			}
			else if (pos == 1)
			{
				DemoActivity.setcollision(1, 0, elent, 1);
			}
			else if (pos == 2)
			{
				DemoActivity.setcollision(1, 24, elent, 1);
			}
			else if (pos == 3)
			{
				DemoActivity.setcollision(1, 24, elent, 3);
			}
			else if (pos == 4)
			{
				DemoActivity.setcollision(1, 1, elent, 4);
			}
			else if (pos == 5)
			{
				DemoActivity.setcollision(1, 4, elent, 4);
			}
			else if (pos == 6)
			{
				DemoActivity.setcollision(1, 5, elent, 5);
			}
			else if (pos == 7)
			{
				DemoActivity.setcollision(1, 0, elent, 5);
			}
			else if (pos == 8)
			{
				DemoActivity.setcollision(1, 5, elent, 7);
			}
			else if (pos == 9)
			{
				DemoActivity.setcollision(1, 1, elent, 8);
			}
			else if (pos == 10)
			{
				DemoActivity.setcollision(1, 6, elent, 8);
			}
			else if (pos == 11)
			{
				DemoActivity.setcollision(1, 5, elent, 9);
			}
			else if (pos == 12)
			{
				DemoActivity.setcollision(1, 6, elent, 9);
			}
			else if (pos == 13)
			{
				DemoActivity.setcollision(1, 10, elent, 10);
			}
			else if (pos == 14)
			{
				DemoActivity.setcollision(1, 6, elent, 10);
			}
			else if (pos == 15)
			{
				DemoActivity.setcollision(1, 9, elent, 11);
			}
			else if (pos == 16)
			{
				DemoActivity.setcollision(1, 10, elent, 11);
			}
			else if (pos == 17)
			{
				DemoActivity.setcollision(1, 10, elent, 12);
			}
			else if (pos == 18)
			{
				DemoActivity.setcollision(1, 1, elent, 12);
			}
			else if (pos == 19)
			{
				DemoActivity.setcollision(1, 11, elent, 13);
			}
			else if (pos == 20)
			{
				DemoActivity.setcollision(1, 10, elent, 13);
			}
			else if (pos == 21)
			{
				DemoActivity.setcollision(1, 11, elent, 14);
			}
			else if (pos == 22)
			{
				DemoActivity.setcollision(1, 10, elent, 15);
			}
			else if (pos == 23)
			{
				DemoActivity.setcollision(1, 12, elent, 15);
			}
			else if (pos == 24)
			{
				DemoActivity.setcollision(1, 10, elent, 17);
			}
			else if (pos == 25)
			{
				DemoActivity.setcollision(1, 15, elent, 17);
			}
			else if (pos == 26)
			{
				DemoActivity.setcollision(1, 17, elent, 18);
			}
			else if (pos == 27)
			{
				DemoActivity.setcollision(1, 0, elent, 18);
			}
			else if (pos == 28)
			{
				DemoActivity.setcollision(1, 17, elent, 19);
			}
			else if (pos == 29)
			{
				DemoActivity.setcollision(1, 1, elent, 19);
			}
			else if (pos == 30)
			{
				DemoActivity.setcollision(1, 18, elent, 20);
			}
			else if (pos == 31)
			{
				DemoActivity.setcollision(1, 1, elent, 20);
			}
			else if (pos == 32)
			{
				DemoActivity.setcollision(1, 18, elent, 21);
			}
			else if (pos == 33)
			{
				DemoActivity.setcollision(1, 18, elent, 22);
			}
			else if (pos == 34)
			{
				DemoActivity.setcollision(1, 0, elent, 22);
			}
			else if (pos == 35)
			{
				DemoActivity.setcollision(1, 1, elent, 23);
			}
			else if (pos == 36)
			{
				DemoActivity.setcollision(1, 5, elent, 23);
			}
			else if (pos == 37)
			{
				DemoActivity.setcollision(1, 19, elent, 24);
			}
			else if (pos == 38)
			{
				DemoActivity.setcollision(1, 1, elent, 24);
			}
			else if (pos == 39)
			{
				DemoActivity.setcollision(1, 19, elent, 25);
			}
			else if (pos == 40)
			{
				DemoActivity.setcollision(1, 6, elent, 25);
			}
			else if (pos == 41)
			{
				DemoActivity.setcollision(1, 20, elent, 26);
			}
			else if (pos == 42)
			{
				DemoActivity.setcollision(1, 4, elent, 26);
			}
			else if (pos == 43)
			{
				DemoActivity.setcollision(1, 0, elent, 27);
			}
			else if (pos == 44)
			{
				DemoActivity.setcollision(1, 10, elent, 27);
			}
			else if (pos == 45)
			{
				DemoActivity.setcollision(1, 4, elent, 6);
			}
			else if (pos == 46)
			{
				DemoActivity.setcollision(1, 5, elent, 6);
			}
			else if (pos == 47)
			{
				DemoActivity.setcollision(1, 1, elent, 28);
			}
			else if (pos == 48)
			{
				DemoActivity.setcollision(1, 0, elent, 28);
			}
			else if (pos == 49)
			{
				DemoActivity.setcollision(1, 23, elent, 29);
			}
			else if (pos == 50)
			{
				DemoActivity.setcollision(1, 5, elent, 29);
			}
			else if (pos == 51)
			{
				DemoActivity.setcollision(1, 23, elent, 30);
			}
			else if (pos == 52)
			{
				DemoActivity.setcollision(1, 10, elent, 30);
			}

			DemoActivity.savecustom();

		}

	}

	public void savecustomdata()
	{ // used to save the custom element data to a
		// file

		FileOutputStream fos;
		DataOutputStream dos;

		try
		{

			File file = new File("/sdcard/elementworks/customeelementsave.txt");
			fos = new FileOutputStream(file);
			dos = new DataOutputStream(fos);
			for (int i = 0; i < (TSpinner + TSeekBar); ++i)
			{
				dos.writeInt(inFile[i]);
			}

		} catch (IOException e)
		{
			e.printStackTrace();
		}

	}

}
