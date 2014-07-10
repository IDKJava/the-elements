package com.idkjava.thelements.custom;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Hashtable;

import yuku.ambilwarna.AmbilWarnaDialog;
import yuku.ambilwarna.AmbilWarnaDialog.OnAmbilWarnaListener;
import android.app.ProgressDialog;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioGroup;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.Toast;

import com.flurry.android.FlurryAgent;
import com.idkjava.thelements.MainActivity;
import com.idkjava.thelements.R;
import com.idkjava.thelements.ReportingActivity;
import com.idkjava.thelements.game.FileManager;
import com.idkjava.thelements.proto.Messages.Collision;
import com.idkjava.thelements.proto.Messages.CustomElement;
import com.idkjava.thelements.proto.Messages.Special;

public class CustomElementBasicActivity extends ReportingActivity
{
    private static final int COLOR_SQUARE_SIZE = 40;

    private CustomElement.Builder mCustomElementBuilder;
    private String oldFilename;
    private boolean newElement;
    private boolean shouldIgnoreSelection;
    private CustomElementActivity mParent;

    private EditText nameField;
    private Spinner baseElementField;
    private RadioGroup stateField;
    private SeekBar startingTempField;
    private SeekBar lowestTempField;
    private SeekBar highestTempField;
    private Spinner lowerElementField;
    private Spinner higherElementField;
    private SeekBar densityField;
    private SeekBar fallvelField;
    private CheckBox inertiaUnmovableField;
    private SeekBar inertiaNormalField;
    private Button saveButton;

    private LinearLayout mColorArea;
    private ImageView mColorImage;
    private int mChosenColor;
    private AmbilWarnaDialog mColorPickerDialog;

    private ArrayAdapter<CharSequence> elementAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.custom_element_basic_activity);

        // Set the parent activity
        mParent = ((CustomElementActivity) getParent());

        // Select all the fields
        nameField = (EditText) findViewById(R.id.ce_name);
        baseElementField = (Spinner) findViewById(R.id.ce_base_element);
        stateField = (RadioGroup) findViewById(R.id.ce_state);
        startingTempField = (SeekBar) findViewById(R.id.ce_starting_temp);
        lowestTempField = (SeekBar) findViewById(R.id.ce_lowest_temp);
        highestTempField = (SeekBar) findViewById(R.id.ce_highest_temp);
        lowerElementField = (Spinner) findViewById(R.id.ce_lower_element);
        higherElementField = (Spinner) findViewById(R.id.ce_higher_element);
        densityField = (SeekBar) findViewById(R.id.ce_density);
        fallvelField = (SeekBar) findViewById(R.id.ce_fallvel);
        inertiaUnmovableField = (CheckBox) findViewById(R.id.ce_inertia_unmovable);
        inertiaNormalField = (SeekBar) findViewById(R.id.ce_inertia_normal);
        saveButton = (Button) findViewById(R.id.ce_save_button);
        mColorImage = (ImageView) findViewById(R.id.custom_color_image);
        mColorArea = (LinearLayout) findViewById(R.id.color_text_and_image);

        // Make the elements adapter, and assign it to the appropriate field
        elementAdapter = ArrayAdapter.createFromResource(this, R.array.elements_list, android.R.layout.simple_spinner_item);
        elementAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        baseElementField.setAdapter(elementAdapter);
        lowerElementField.setAdapter(elementAdapter);
        higherElementField.setAdapter(elementAdapter);

        // Load data from the parent activity
        mCustomElementBuilder = mParent.mCustomElementBuilder;
        oldFilename = mParent.oldFilename;
        newElement = mParent.newElement;
        shouldIgnoreSelection = false;
        if (!newElement)
        {
            shouldIgnoreSelection = true;
            // Fill in the basic view info
            fillInfo();
            // Also save the special and custom data in the parent activity
            mParent.collisions = CustomElementManager.getCollisionIndexList(mCustomElementBuilder);
            mParent.specials = CustomElementManager.getSpecialsIndexList(mCustomElementBuilder);
            mParent.specialVals = CustomElementManager.getSpecialValsIndexList(mCustomElementBuilder);
        }

        baseElementField.setOnItemSelectedListener(new OnItemSelectedListener ()
        {
            @Override
            public void onItemSelected(AdapterView<?> parent, View v, int pos, long id)
            {
                // Fill in the values with the default for that element
                if (shouldIgnoreSelection)
                {
                    shouldIgnoreSelection = false;
                    return;
                }
                fillInfoFromBase(pos);
            }
            @Override
            public void onNothingSelected(AdapterView<?> parent)
            {
                // TODO: Clear the properties
            }
        });

        inertiaUnmovableField.setOnCheckedChangeListener(new OnCheckedChangeListener()
        {
            @Override
            public void onCheckedChanged(CompoundButton b, boolean checked)
            {
                if (checked)
                {
                    inertiaNormalField.setVisibility(View.GONE);
                }
                else
                {
                    inertiaNormalField.setVisibility(View.VISIBLE);
                }
            }
        });

        saveButton.setOnClickListener(new OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                nameField.setText(nameField.getText().toString().trim());
                Log.d("LOG", "Name field: " + nameField.getText().toString());
                if (nameField.getText().toString().length() <= 0)
                {
                    Log.d("LOG", "Emtpy name field!");
                    Toast.makeText(getApplicationContext(), R.string.ce_empty_name_error, Toast.LENGTH_LONG).show();
                    return;
                }
                ProgressDialog dialog = new ProgressDialog(CustomElementBasicActivity.this);
                dialog.show();
                if(CustomElementBasicActivity.this.saveElement())
                {
                    dialog.dismiss();
                    finish();
                }
                else
                {
                    dialog.dismiss();
                }
            }
        });
    }

    private boolean saveElement()
    {
        writePropertiesToCustom();
        
        CustomElement custom = mCustomElementBuilder.build();
        String filename = CustomElementManager.getFilename(custom);
        if (!filename.equals(oldFilename) && !newElement)
        {
            if (oldFilename != null)
            {
                new File(oldFilename).delete();
            }
        }
        try
        {
            custom.writeTo(new FileOutputStream(filename));
        }
        catch (FileNotFoundException e)
        {
            Toast.makeText(getApplicationContext(), R.string.ce_save_failed, Toast.LENGTH_LONG).show();
            return false;
        }
        catch (IOException e)
        {
            Toast.makeText(getApplicationContext(), R.string.ce_save_failed, Toast.LENGTH_LONG).show();
            return false;
        }
        
        Toast.makeText(getApplicationContext(),
                getResources().getString(R.string.ce_save_success)
                + " " + filename,
                Toast.LENGTH_LONG).show();
        // Log the custom element name
        Hashtable<String, String> params = new Hashtable<String, String>();
        params.put("Name", filename);
        FlurryAgent.logEvent("Element saved", params);
        return true;
    }
    
    public void writePropertiesToCustom()
    {
        // Write all the normal properties to the custom element from the fields in basic
        mCustomElementBuilder.setName(nameField.getText().toString());
        mCustomElementBuilder.setBaseElementIndex(
                (int)baseElementField.getSelectedItemId() + MainActivity.NORMAL_ELEMENT);
        int stateRadioId = stateField.getCheckedRadioButtonId();
        mCustomElementBuilder.setState(getStateValFromId(stateRadioId));
        mCustomElementBuilder.setStartingTemp(startingTempField.getProgress());
        mCustomElementBuilder.setLowestTemp(lowestTempField.getProgress());
        mCustomElementBuilder.setHighestTemp(highestTempField.getProgress());
        mCustomElementBuilder.setLowerElementIndex(
                (int) (lowerElementField.getSelectedItemId() + MainActivity.NORMAL_ELEMENT));
        mCustomElementBuilder.setHigherElementIndex(
                (int) (higherElementField.getSelectedItemId() + MainActivity.NORMAL_ELEMENT));
        mCustomElementBuilder.setRed(Color.red(mChosenColor));
        mCustomElementBuilder.setGreen(Color.green(mChosenColor));
        mCustomElementBuilder.setBlue(Color.blue(mChosenColor));
        mCustomElementBuilder.setDensity(densityField.getProgress());
        mCustomElementBuilder.setFallvel(fallvelField.getProgress());
        if (inertiaUnmovableField.isChecked())
        {
            mCustomElementBuilder.setInertia(255);
        }
        else
        {
            mCustomElementBuilder.setInertia(inertiaNormalField.getProgress());
        }

        // Get the list of collisions from the parent activity (the method of transferring from Advanced)
        ArrayList<Integer> collisions = mParent.collisions;
        // If collisions is null and it's not a new element, then we didn't change anything, so leave it the same
        if (!(collisions == null && !newElement))
        {
            // If collisions is null, then we need to create an arraylist for it
            if (collisions == null)
            {
                collisions = new ArrayList<Integer>();
            }

            // Clear previously set collisions
            mCustomElementBuilder.clearCollision();
            // Walk through the collisions we got from Advanced and set it
            // (use 0 as the default if there was an error transferring)
            int numElements = getResources().getStringArray(R.array.elements_list).length;
            for (int i = 0; i < numElements; i++)
            {
                if (i < collisions.size())
                {
                    mCustomElementBuilder.addCollision(
                            Collision.newBuilder().setType(collisions.get(i)));
                }
                else
                {
                    Log.d("LOG", "Error: collisions array passed to save was not long enough");
                    mCustomElementBuilder.addCollision(
                            Collision.newBuilder().setType(0));
                }
            }
        }
        ArrayList<Integer> specials = mParent.specials;
        ArrayList<Integer> specialVals = mParent.specialVals;
        // If either is null and this is not a new element, then we don't want to bother writing
        if (!((specials == null || specialVals == null) && !newElement))
        {
            // Create the arraylists if needed
            if (specials == null)
            {
                specials = new ArrayList<Integer>();
            }
            if (specialVals == null)
            {
                specialVals = new ArrayList<Integer>();
            }

            // Clear the previous specials in custom
            mCustomElementBuilder.clearSpecial();

            // Walk through the specials, and write both the index and val
            // (using sane defaults and logging if issues occur)
            int numSpecials = MainActivity.MAX_SPECIALS;
            for (int i = 0; i < numSpecials; i++)
            {
                int special;
                int specialVal;
                
                if (i < specials.size())
                {
                    special = specials.get(i);
                }
                else
                {
                    Log.e("TheElements", "Error: specials array passed to save was not long enough");
                    special = -1;
                }
                
                if (i < specialVals.size())
                {
                    specialVal = specialVals.get(i);
                }
                else
                {
                    Log.e("TheElements", "Error: special vals array passed to save was not long enough");
                    specialVal = -1;
                }
                
                mCustomElementBuilder.addSpecial(
                        Special.newBuilder()
                        .setType(special)
                        .setVal(specialVal));
            }
        }
    }

    private void fillInfo()
    {
        nameField.setText(
                mCustomElementBuilder.getName());
        baseElementField.setSelection(CustomElementManager.getElementSelectionFromIndex(
                mCustomElementBuilder.getBaseElementIndex()));
        stateField.check(getStateRadioId(
                mCustomElementBuilder.getState()));
        startingTempField.setProgress(
                mCustomElementBuilder.getStartingTemp());
        lowestTempField.setProgress(
                mCustomElementBuilder.getLowestTemp());
        highestTempField.setProgress(
                mCustomElementBuilder.getHighestTemp());
        lowerElementField.setSelection(CustomElementManager.getElementSelectionFromIndex(
                mCustomElementBuilder.getLowerElementIndex()));
        higherElementField.setSelection(CustomElementManager.getElementSelectionFromIndex(
                mCustomElementBuilder.getHigherElementIndex()));
        final int color = Color.rgb(
                mCustomElementBuilder.getRed(),
                mCustomElementBuilder.getGreen(),
                mCustomElementBuilder.getBlue());
        setElementColorColor(color);
        mColorPickerDialog = makeColorPickerDialog(color);
        mColorArea.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                mColorPickerDialog.show();
            }
        });
        densityField.setProgress(mCustomElementBuilder.getDensity());
        fallvelField.setProgress(mCustomElementBuilder.getFallvel());
        if (mCustomElementBuilder.getInertia() == 255)
        {
            inertiaUnmovableField.setChecked(true);
            inertiaNormalField.setVisibility(View.GONE);
        }
        else
        {
            inertiaUnmovableField.setChecked(false);
            inertiaNormalField.setVisibility(View.VISIBLE);
            inertiaNormalField.setProgress(mCustomElementBuilder.getInertia());
        }
    }
    private void fillInfoFromBase(int pos)
    {
        int elementIndex = pos + MainActivity.NORMAL_ELEMENT;
        String elementInfo = MainActivity.getElementInfo(elementIndex);
        Log.d("LOG", "Element info: " + elementInfo);
        BufferedReader reader = new BufferedReader(new StringReader(elementInfo));

        try
        {
            // Now fill in the actual values
            //nameField.setText(reader.readLine());
            // Ignore the name
            reader.readLine();
            int stateVal = Integer.parseInt(reader.readLine());
            stateField.check(getStateRadioId(stateVal));
            startingTempField.setProgress(Integer.parseInt(reader.readLine()));
            lowestTempField.setProgress(Integer.parseInt(reader.readLine()));
            highestTempField.setProgress(Integer.parseInt(reader.readLine()));
            lowerElementField.setSelection(Integer.parseInt(reader.readLine()) - MainActivity.NORMAL_ELEMENT);
            higherElementField.setSelection(Integer.parseInt(reader.readLine()) - MainActivity.NORMAL_ELEMENT);
            final int color = Color.rgb(Integer.parseInt(reader.readLine()), //red
                        Integer.parseInt(reader.readLine()), //green
                        Integer.parseInt(reader.readLine())); //blue
            setElementColorColor(color);
            mColorPickerDialog = makeColorPickerDialog(color);
            mColorArea.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View v) {
                    mColorPickerDialog.show();
                }
            });


            densityField.setProgress(Integer.parseInt(reader.readLine()));
            fallvelField.setProgress(Integer.parseInt(reader.readLine()));
            int inertia = Integer.parseInt(reader.readLine());
            if (inertia == 255)
            {
                inertiaUnmovableField.setChecked(true);
                inertiaNormalField.setVisibility(View.GONE);
            }
            else
            {
                inertiaUnmovableField.setChecked(false);
                inertiaNormalField.setVisibility(View.VISIBLE);
                inertiaNormalField.setProgress(inertia);
            }

            // Save collisions and specials for the advanced activity
            mParent.collisions = new ArrayList<Integer>();
            String line;
            for (int i = 0; i < MainActivity.NUM_BASE_ELEMENTS-MainActivity.NORMAL_ELEMENT; i++)
            {
                line = reader.readLine();
                if (line == null)
                {
                    return;
                }

                mParent.collisions.add(Integer.parseInt(line));
            }
            mParent.specials = new ArrayList<Integer>();
            mParent.specialVals = new ArrayList<Integer>();
            String[] tempVals;
            for (int i = 0; i < MainActivity.MAX_SPECIALS; i++)
            {
                line = reader.readLine();
                if (line == null)
                {
                    return;
                }

                tempVals = line.split("\\s+", 2);
                if (tempVals.length < 2)
                {
                    return;
                }
                mParent.specials.add(Integer.parseInt(tempVals[0]));
                mParent.specialVals.add(Integer.parseInt(tempVals[1]));
            }
        }
        catch (IOException e)
        {
            // TODO: Maybe do something here?
            e.printStackTrace();
        }
    }

    private int getStateRadioId(int state)
    {
        switch(state)
        {
        case 0:
        {
            return R.id.ce_solid_state;
        }
        case 1:
        {
            return R.id.ce_liquid_state;
        }
        case 2:
        {
            return R.id.ce_gas_state;
        }
        default:
        {
            return 0;
        }
        }
    }
    private int getStateValFromId(int id)
    {
        switch(id)
        {
        case R.id.ce_solid_state:
        {
            return 0;
        }
        case R.id.ce_liquid_state:
        {
            return 1;
        }
        case R.id.ce_gas_state:
        {
            return 2;
        }
        default:
        {
            return 0;
        }
        }
    }

    private AmbilWarnaDialog makeColorPickerDialog(int color) {
        // initialColor is the initially-selected color to be shown in the rectangle on the left of the arrow.
        // for example, 0xff000000 is black, 0xff0000ff is blue. Please be aware of the initial 0xff which is the alpha.
        return new AmbilWarnaDialog(this, color , new OnAmbilWarnaListener() {
                @Override
                public void onOk(AmbilWarnaDialog dialog, int color) {
                    dialog.setColor(color);
                    setElementColorColor(color);
                }

                @Override
                public void onCancel(AmbilWarnaDialog dialog) {

                }
        });
    }

    private void setElementColorColor(int color) {
        ColorDrawable elementColor = new ColorDrawable(color);
        elementColor.setBounds(0, 0, MainActivity.toPx(COLOR_SQUARE_SIZE), MainActivity.toPx(COLOR_SQUARE_SIZE));
        mColorImage.setImageDrawable(elementColor);
        mChosenColor = color;
    }
}
