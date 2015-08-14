package com.idkjava.thelements;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

/**
 * Activity to offer the user the choice of world to start in. Displays locked worlds differently,
 * and links to the buy page if the user selects them.
 */
public class SelectWorldActivity extends Activity {
    View earthWorldButton;
    View spaceWorldButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.select_world_activity);

        earthWorldButton = findViewById(R.id.earth_world_button);
        earthWorldButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent mainIntent = new Intent(SelectWorldActivity.this, MainActivity.class);
                mainIntent.putExtra("world", MainActivity.WORLD_EARTH);
                startActivity(mainIntent);
                finish();
            }
        });
        spaceWorldButton = findViewById(R.id.space_world_button);
        spaceWorldButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent mainIntent = new Intent(SelectWorldActivity.this, MainActivity.class);
                mainIntent.putExtra("world", MainActivity.WORLD_SPACE);
                startActivity(mainIntent);
                finish();
            }
        });
    }
}
