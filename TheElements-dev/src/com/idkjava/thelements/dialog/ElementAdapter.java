package com.idkjava.thelements.dialog;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.idkjava.thelements.MainActivity;
import com.idkjava.thelements.R;

import java.util.ArrayList;

/**
 * Definition of the list adapter...uses the View Holder pattern to optimize
 * performance.
 */
public class ElementAdapter extends ArrayAdapter {

    private static final int RESOURCE = R.layout.element_row;
    private static final int COLOR_SQUARE_SIZE = 40;

    private LayoutInflater inflater;

    static class ViewHolder {
        TextView nameTxVw;
    }

    public ElementAdapter(Context context, ArrayList<String> elements) {
        super(context, RESOURCE, elements);
        inflater = LayoutInflater.from(context);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder holder;

        if (convertView == null) {
            // inflate a new view and setup the view holder for future use
            convertView = inflater.inflate(RESOURCE, null);

            holder = new ViewHolder();
            holder.nameTxVw = (TextView) convertView.findViewById(R.id.elementname);
            convertView.setTag(holder);
        } else {
            // view already defined, retrieve view holder
            holder = (ViewHolder) convertView.getTag();
        }

        String name = (String) getItem(position);
        int realElementPosition = position + MainActivity.NORMAL_ELEMENT;
        holder.nameTxVw.setText(name);
        int theColor = Color.rgb(
                MainActivity.getElementRed(realElementPosition),
                MainActivity.getElementGreen(realElementPosition),
                MainActivity.getElementBlue(realElementPosition));
        ColorDrawable elementColor = new ColorDrawable(theColor);

        elementColor.setBounds(0, 0,
                MainActivity.toPx(COLOR_SQUARE_SIZE),
                MainActivity.toPx(COLOR_SQUARE_SIZE));
        holder.nameTxVw.setCompoundDrawables(elementColor, null, null, null);

        return convertView;
    }
}