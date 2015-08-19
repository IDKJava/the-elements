package com.idkjava.thelements.dialog;

/**
 * A class containing a text item and a corresponding iconRes resource.
 */
public class IconListItem {
    public int iconRes;
    public int nameRes;
    public boolean locked;
    public String sku;

    public IconListItem(int nameRes, int iconRes) {
        this.nameRes = nameRes;
        this.iconRes = iconRes;
        this.locked = false;
        this.sku = null;
    }
}
