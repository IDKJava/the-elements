package com.idkjava.thelements.dialog;

/**
 * A class containing a text item and a corresponding iconRes resource.
 */
public class IconListItem {
    public int iconRes;
    public int nameRes;

    public IconListItem(int nameRes, int iconRes) {
        this.nameRes = nameRes;
        this.iconRes = iconRes;
    }
}
