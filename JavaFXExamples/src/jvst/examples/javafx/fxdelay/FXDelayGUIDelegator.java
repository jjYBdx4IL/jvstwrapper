/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jvst.examples.javafx.fxdelay;

import java.awt.BorderLayout;
import javax.swing.JComponent;
import javax.swing.JFrame;
import jvst.examples.javafx.SceneToJComponent;
import jvst.wrapper.VSTPluginAdapter;
import jvst.wrapper.gui.VSTPluginGUIRunner;

import jvst.wrapper.VSTPluginGUIAdapter;


/**
 *
 * @author nix
 *
 * TODO:
 *  - tabs / other fx samples (clock, stopwatch, game)
 * 
 */
public class FXDelayGUIDelegator extends VSTPluginGUIAdapter {

    protected static boolean DEBUG = false;
    protected static final String FX_GUI_CLASS = "jvst.examples.javafx.fxdelay.FXDelayGUI";

    
    public FXDelayGUIDelegator(VSTPluginGUIRunner runner, VSTPluginAdapter plug) {
        super(runner, plug);
        try {
            log("JayDLayFXGUIDelegator <init>");

            //windows properties
            this.setTitle("JayDLayFXGUI");
            this.setSize(290, 150);
            this.setResizable(false);

            //use SceneToJComponent
            JComponent s = SceneToJComponent.loadVSTPluginScene2(FX_GUI_CLASS, plug);

            this.setLayout(new BorderLayout(10,10));
            this.add(s, BorderLayout.CENTER);

            
            //this is needed on the mac only,
            //java guis are handled there in a pretty different way than on win/linux
            //XXX
            if (RUNNING_MAC_X) {
                this.show();
            }
        }
        catch (Exception ex) {
            log("** ERROR: Fatal error when loading JavaFX GUI: " + ex.toString());
            ex.printStackTrace();
        } 
    }


    /*
     * main method for convenient GUI debugging
     */
    public static void main(String[] args) throws Throwable {
        DEBUG = true;

        FXDelayGUIDelegator gui = new FXDelayGUIDelegator(new VSTPluginGUIRunner(), null);
        gui.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        gui.show();
    }
    
}
