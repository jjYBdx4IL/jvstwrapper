/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package jvst.examples.javafx.fxdelay;

import javax.swing.SwingUtilities;
import jvst.examples.javafx.FXGUIJavaInterop;
import jvst.examples.javafx.FXPluginJavaInterop;
import jvst.examples.jaydlay.JayDLay;

/**
 *
 * @author nix
 *
 */
public class FXDelay extends JayDLay implements FXPluginJavaInterop {

    FXDelayJavaInterop fxgui;

    public FXDelay(long wrapper) {
        super(wrapper);
        //new uid
        this.setUniqueID('j' << 24 | 'f' << 16 | 'x' << 8 | 'D');
    }


    /*
     * override updadateGUI here so that the GUI is updated when
     * the plug changes param values (e.g. loads new program or because of host automation)
     */
    @Override
    protected void updateGUI() {
        if (fxgui!=null) {

            final FXDelay thi = this;
            
            //Avoid Swing threading issues (esp. on Mac): invoke all calls from plug-->gui from the EDT
            SwingUtilities.invokeLater(new Runnable() {
                @Override
                public void run() {
                    fxgui.setDelay(thi.getParameter(PARAM_ID_DELAY));
                    fxgui.setVolume(thi.getParameter(PARAM_ID_OUT));
                    fxgui.setFeedback(thi.getParameter(PARAM_ID_FEEDBACK));
                }
            });
        }
    }

    @Override
    public void setFXGUI(FXGUIJavaInterop fxgui) {
        this.fxgui = (FXDelayJavaInterop)fxgui;
    }
}
