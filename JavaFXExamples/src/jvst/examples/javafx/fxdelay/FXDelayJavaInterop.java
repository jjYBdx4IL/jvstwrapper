/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jvst.examples.javafx.fxdelay;

import jvst.examples.javafx.FXGUIJavaInterop;

/**
 *
 * @author nix
 */
public interface FXDelayJavaInterop extends FXGUIJavaInterop {
    
    public void setVolume(float v);
    public void setFeedback(float f);
    public void setDelay(float d);
}
