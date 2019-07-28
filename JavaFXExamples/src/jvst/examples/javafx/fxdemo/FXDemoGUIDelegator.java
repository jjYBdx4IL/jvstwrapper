/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package jvst.examples.javafx.fxdemo;

import java.awt.BorderLayout;
import javafx.reflect.FXClassType;
import javafx.reflect.FXLocal;
import javafx.reflect.FXLocal.ObjectValue;
import javax.swing.JComponent;
import jvst.examples.javafx.*;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import jvst.wrapper.VSTPluginAdapter;
import jvst.wrapper.gui.VSTPluginGUIRunner;

import jvst.wrapper.VSTPluginGUIAdapter;

/**
 *
 * @author nix
 *
 * 
 */
public class FXDemoGUIDelegator extends VSTPluginGUIAdapter {

    protected static boolean DEBUG = false;
    protected static final String FX_GUI_CLASS = "jvst.examples.javafx.fxdelay.FXDelayGUI";

    
    public FXDemoGUIDelegator(VSTPluginGUIRunner runner, VSTPluginAdapter plug) {
        super(runner, plug);
        try {
            log("jVST FX Demo GUI <init>");

            //windows properties
            this.setTitle("JavaFX Demo VST GUI");
            this.setSize(600, 400);

            //create tabbed pane
            JTabbedPane tabbedPane = new JTabbedPane();

            //add some more JavaFX stuff --> this is stolen from the sample fx files at javafx.com
            JPanel p;
            p = makeFXPanel("jvst.examples.javafx.fxdemo.AnalogClock");
            tabbedPane.addTab("Analog Clock", p);
            p = makeFXPanel("jvst.examples.javafx.fxdemo.BouncingBall");
            tabbedPane.addTab("Bouncing Ball", p);
            p = makeFXPanel("jvst.examples.javafx.fxdemo.RecursiveTree");
            tabbedPane.addTab("Recursive Tree", p);
            p = makeFXPanel("jvst.examples.javafx.fxdemo.weather.Main");
            tabbedPane.addTab("Weather", p);

            this.getContentPane().add(tabbedPane);

            
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

    private JPanel makeFXPanel(String fxGUIClass) throws Exception {
        JPanel panel = new JPanel(false);
       
        //use SceneToJComponent
        JComponent s = SceneToJComponent.loadVSTPluginNode(fxGUIClass);
        
        //use JXscene
        //JavaInterop fxnode =
        //            (JavaInterop) this.getClass().getClassLoader().loadClass(fxGUIClass).newInstance();
        //JXScene s = new JXScene();
        //s.setScene(fxnode.getScene());
        
        panel.setLayout(new BorderLayout());
        panel.add(s, BorderLayout.CENTER);
        
        return panel;
    }




    /*
     * main method for convenient GUI debugging
     */
    public static void main(String[] args) throws Throwable {
        DEBUG = true;

        FXDemoGUIDelegator gui = new FXDemoGUIDelegator(new VSTPluginGUIRunner(), null);
        gui.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        gui.show();
    }
}
