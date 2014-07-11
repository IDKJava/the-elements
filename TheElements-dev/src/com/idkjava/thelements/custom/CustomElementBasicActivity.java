package com.idkjava.thelements.custom;

import java.io.BufferedReader;
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
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.flurry.android.FlurryAgent;
import com.google.protobuf.ByteString;
import com.google.protobuf.InvalidProtocolBufferException;
import com.idkjava.thelements.MainActivity;
import com.idkjava.thelements.R;
import com.idkjava.thelements.ReportingActivity;
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
    private TextView startingTempText;
    private SeekBar lowestTempField;
    private TextView lowestTempText;
    private SeekBar highestTempField;
    private TextView highestTempText;
    private Spinner lowerElementField;
    private Spinner higherElementField;
    private SeekBar densityField;
    private TextView densityText;
    private SeekBar fallvelField;
    private TextView fallvelText;
    private CheckBox inertiaUnmovableField;
    private LinearLayout inertiaContainer;
    private SeekBar inertiaNormalField;
    private TextView inertiaNormalText;
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
        startingTempText = (TextView) findViewById(R.id.ce_starting_temp_num);
        lowestTempField = (SeekBar) findViewById(R.id.ce_lowest_temp);
        lowestTempText = (TextView) findViewById(R.id.ce_lowest_temp_num);
        highestTempField = (SeekBar) findViewById(R.id.ce_highest_temp);
        highestTempText = (TextView) findViewById(R.id.ce_highest_temp_num);
        lowerElementField = (Spinner) findViewById(R.id.ce_lower_element);
        higherElementField = (Spinner) findViewById(R.id.ce_higher_element);
        densityField = (SeekBar) findViewById(R.id.ce_density);
        densityText = (TextView) findViewById(R.id.ce_density_num);
        fallvelField = (SeekBar) findViewById(R.id.ce_fallvel);
        fallvelText = (TextView) findViewById(R.id.ce_fallvel_num);
        inertiaUnmovableField = (CheckBox) findViewById(R.id.ce_inertia_unmovable);
        inertiaContainer = (LinearLayout) findViewById(R.id.ce_inertia_container);
        inertiaNormalField = (SeekBar) findViewById(R.id.ce_inertia_normal);
        inertiaNormalText = (TextView) findViewById(R.id.ce_inertia_num);
        saveButton = (Button) findViewById(R.id.ce_save_button);
        mColorImage = (ImageView) findViewById(R.id.custom_color_image);
        mColorArea = (LinearLayout) findViewById(R.id.color_text_and_image);

        // Make the elements adapter, and assign it to the appropriate field
        elementAdapter = ArrayAdapter.createFromResource(this, R.array.elements_list, android.R.layout.simple_spinner_item);
        elementAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        baseElementField.setAdapter(elementAdapter);
        lowerElementField.setAdapter(elementAdapter);
        higherElementField.setAdapter(elementAdapter);

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
        
        startingTempField.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                startingTempText.setText(String.valueOf(progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });
        
        lowestTempField.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                lowestTempText.setText(String.valueOf(progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });
        
        highestTempField.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                highestTempText.setText(String.valueOf(progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });
        
        densityField.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                densityText.setText(String.valueOf(progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });
        
        inertiaNormalField.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                inertiaNormalText.setText(String.valueOf(progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });
        
        fallvelField.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                fallvelText.setText(String.valueOf(progress));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        inertiaUnmovableField.setOnCheckedChangeListener(new OnCheckedChangeListener()
        {
            @Override
            public void onCheckedChanged(CompoundButton b, boolean checked)
            {
                if (checked)
                {
                    //TODO(gkanwar): For some reason this does weird stuff with
                    //displaying the inner stuff once it is visible again
                    //inertiaContainer.setVisibility(View.GONE);
                    inertiaNormalField.setVisibility(View.GONE);
                }
                else
                {
                    //inertiaContainer.setVisibility(View.VISIBLE);
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
        
        // Load data from the parent activity
        mCustomElementBuilder = mParent.mCustomElementBuilder;
        oldFilename = mParent.oldFilename;
        newElement = mParent.newElement;
        shouldIgnoreSelection = false;
        if (!newElement)
        {
            shouldIgnoreSelection = true;
            // Fill in all info from the custom element builder
            fillInfo();
        }
    }

    private boolean saveElement()
    {
        writePropertiesToCustom();
        
        if (newElement)
        {
            CustomElementManager.generateUniqueFilename(mCustomElementBuilder);
        }
        CustomElement custom = mCustomElementBuilder.build();
        try
        {
            custom.writeTo(new FileOutputStream(custom.getFilename()));
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
                + " " + custom.getFilename(),
                Toast.LENGTH_LONG).show();
        // Log the custom element name
        Hashtable<String, String> params = new Hashtable<String, String>();
        params.put("Name", custom.getFilename());
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

    // Fill in all info from the current custom element builder
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
        
        // Also save the special and custom data in the parent activity
        mParent.collisions = CustomElementManager.getCollisionIndexList(mCustomElementBuilder);
        mParent.specials = CustomElementManager.getSpecialsIndexList(mCustomElementBuilder);
        mParent.specialVals = CustomElementManager.getSpecialValsIndexList(mCustomElementBuilder);
    }
    
    private void fillInfoFromBase(int pos)
    {
        int elementIndex = pos + MainActivity.NORMAL_ELEMENT;
        ByteString elementInfo = ByteString.copyFrom(MainActivity.getElementInfo(elementIndex));
        CustomElement baseElement;
        try {
            baseElement = CustomElement.parseFrom(elementInfo);
        }
        catch (InvalidProtocolBufferException e) {
            throw new RuntimeException("Could not parse base element proto message");
        }
        
        // Treat name and filename specially, since they should persist
        // Unfortunately, we cannot just use mergeFrom here, since that would just mash
        // the collision lists together.
        String name = mCustomElementBuilder.getName();
        String filename = mCustomElementBuilder.getFilename();
        mCustomElementBuilder = CustomElement.newBuilder(baseElement);
        mCustomElementBuilder.setName(name);
        mCustomElementBuilder.setFilename(filename);
                
        fillInfo();
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
