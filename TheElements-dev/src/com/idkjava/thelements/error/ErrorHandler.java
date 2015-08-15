package com.idkjava.thelements.error;

/**
 * Interface to handle error messages.
 */
public interface ErrorHandler {
    // Fire an error
    void error(String message);
    void error(int resId);
    // Clear any held objects
    void clear();
}