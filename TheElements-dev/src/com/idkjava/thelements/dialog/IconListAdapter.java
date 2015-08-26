package com.idkjava.thelements.dialog;

import android.content.Context;
import android.content.res.Resources;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.idkjava.thelements.R;

import java.util.ArrayList;

/**
 * An adapter for a list containing text plus icons. Uses the View Holder
 * pattern to optimize performance.
 */
public class IconListAdapter extends ArrayAdapter {

    private static final int RESOURCE = R.layout.icon_list_row;

    private Resources mResources;
    private LayoutInflater mInflater;

    static class ViewHolder {
        ImageView iconView;
        TextView nameView;
    }

    public IconListAdapter(Context context, ArrayList<IconListItem> elements) {
        super(context, RESOURCE, elements);
        mResources = context.getResources();
        mInflater = LayoutInflater.from(context);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        ViewHolder holder;

        if (convertView == null) {
            // inflate a new view and setup the view holder for future use
            convertView = mInflater.inflate(RESOURCE, null);

            holder = new ViewHolder();
            holder.nameView = (TextView) convertView.findViewById(R.id.item_name);
            holder.iconView = (ImageView) convertView.findViewById(R.id.item_icon);
            convertView.setTag(holder);
        } else {
            // view already defined, retrieve view holder
            holder = (ViewHolder) convertView.getTag();
        }

        IconListItem item = (IconListItem) getItem(position);
        holder.nameView.setText(mResources.getString(item.nameRes));
        holder.iconView.setImageDrawable(mResources.getDrawable(item.iconRes));

        return convertView;
    }

    public void addAll(ArrayList<IconListItem> items) {
        for (IconListItem i : items) {
            this.add(i);
        }
    }
}