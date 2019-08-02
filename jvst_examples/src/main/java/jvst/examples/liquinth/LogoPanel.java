
package jvst.examples.liquinth;

import java.awt.*;
import javax.swing.*;
import javax.swing.border.*;

public class LogoPanel extends JPanel {
	public LogoPanel() {
		setLayout( new BorderLayout() );
		setBackground( Color.BLACK );
		setBorder( new BevelBorder( BevelBorder.LOWERED, Color.WHITE, Color.GRAY ) );
		ImageIcon logo = new ImageIcon( getClass().getResource( "liquinth.png" ) );
		add( new JLabel( logo ), BorderLayout.CENTER );
	}
}
