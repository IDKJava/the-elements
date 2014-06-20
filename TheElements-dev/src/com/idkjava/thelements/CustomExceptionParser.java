package com.idkjava.thelements;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;

import com.google.analytics.tracking.android.ExceptionParser;
import com.idkjava.thelements.game.FileManager;

public class CustomExceptionParser implements ExceptionParser {

    @Override
    public String getDescription(String threadName, Throwable t) {
        // Get the string representation of the stack trace
        ByteArrayOutputStream byteOutStream = new ByteArrayOutputStream();
        PrintWriter writer = new PrintWriter(byteOutStream, true);
        t.printStackTrace(writer);
        String stackTrace = byteOutStream.toString();
        
        StringBuilder sb = new StringBuilder();
        sb.append(stackTrace);
        sb.append("\n\n");
        
        // Open the temp.sav file and output it into the string
        try {
            sb.append("temp.sav:\n");
            BufferedReader in = new BufferedReader(new FileReader(FileManager.getTempSaveName()));
            String line = in.readLine();
            while (line != null) {
                sb.append(line);
                line = in.readLine();
            }
        } catch (FileNotFoundException e) {
            sb.append("temp.sav file not found.");
        } catch (IOException e) {
            sb.append("IOException while reading temp.sav");
        }
        
        return sb.toString();
    }
}
