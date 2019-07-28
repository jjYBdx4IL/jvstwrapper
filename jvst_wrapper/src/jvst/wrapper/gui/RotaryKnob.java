/* 
 * jVSTwRapper - The Java way into VST world!
 * 
 * jVSTwRapper is an easy and reliable Java Wrapper for the Steinberg VST interface. 
 * It enables you to develop VST 2.3 compatible audio plugins and virtual instruments 
 * plus user interfaces with the Java Programming Language. 3 Demo Plugins(+src) are included!
 * 
 * Copyright (C) 2006  Daniel Martin [daniel309@users.sourceforge.net] 
 * 					   and many others, see CREDITS.txt
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

package jvst.wrapper.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.awt.geom.Arc2D;

import javax.swing.Action;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import javax.swing.ToolTipManager;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.EventListenerList;


public class RotaryKnob extends JComponent {

	/*
	 * DKnob.java (c) 2000 by Joakim Eriksson
	 * 
	 * DKnob is a component similar to JSlider but with round "user interface",
	 * a knob.
	 * 
	 * Original source is from http://www.dreamfabric.com/java/knob/knob.html
	 */

	public final static int DRAGTYPE_SIMPLE = 1;
	public final static int DRAGTYPE_ROUND = 2;

	private final static long serialVersionUID = -7625797152529235601L;
	private final static float START = 225;
	private final static float LENGTH = 270;
	private final static float PI = (float) Math.PI;
	private final static float START_ANG = (START / 360) * PI * 2;
	private final static float LENGTH_ANG = (LENGTH / 360) * PI * 2;
	private final static float MULTIP = 180 / PI;
	
	private final static Color DEFAULT_FOCUS_COLOR = new Color(0x8080ff);
	private final static Dimension MIN_SIZE = new Dimension(45, 45);
	private final static Dimension PREF_SIZE = new Dimension(60, 60);
	// Set the antialiasing to get the right look!
	private final static RenderingHints AALIAS = new RenderingHints(
			RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
	
	
	private ChangeEvent changeEvent = null;
	private EventListenerList listenerList = new EventListenerList();
	private Arc2D hitArc = new Arc2D.Float(Arc2D.PIE);
	private float ang = (float) START_ANG;
	private float val;
	private int dragpos = -1;
	private float startVal;
	private Color focusColor  = DEFAULT_FOCUS_COLOR;;
	private double lastAng;
	private int dragType = DRAGTYPE_SIMPLE;
	private int shadowSizeX = 1;
	private int shadowSizeY = 1;
	private float dragIncrement = 0.01F;
	private float clickIncrement = 0.01F;
	private int size;
	private int middle;
	
	
	
	public RotaryKnob() {
		setPreferredSize(PREF_SIZE);
		hitArc.setAngleStart(235); // Degrees ??? Radians???
		
		//add listeners		
		addMouseListener(new MouseAdapter() {
			public void mousePressed(MouseEvent me) {
				dragpos = me.getX() + me.getY();
				startVal = val;

				// Fix last angle
				int xpos = middle - me.getX();
				int ypos = middle - me.getY();
				lastAng = Math.atan2(xpos, ypos);

				requestFocus();
			}

			public void mouseClicked(MouseEvent me) {
				hitArc.setAngleExtent(-(LENGTH + 20));
				if (hitArc.contains(me.getX(), me.getY())) {
					hitArc.setAngleExtent(MULTIP * (ang - START_ANG) - 10);
					if (hitArc.contains(me.getX(), me.getY())) {
						decValue();
					} else
						incValue();
				}
			}
		});
		
	
		
		// Let the user control the knob with the mouse
		addMouseMotionListener(new MouseMotionAdapter() {
			public void mouseDragged(MouseEvent me) {
				if (dragType == DRAGTYPE_SIMPLE) {
					float f = dragIncrement
							* (float) ((me.getX() + me.getY()) - dragpos);
					setValue(startVal - f);
				} else if (dragType == DRAGTYPE_ROUND) {
					// Measure relative the middle of the button!
					int xpos = middle - me.getX();
					int ypos = middle - me.getY();
					double ang = Math.atan2(xpos, ypos);
					double diff = lastAng - ang;
					setValue((float) (getValue() + (diff / LENGTH_ANG)));

					lastAng = ang;
				}
				
			}
		});

		// Let the user control the knob with the keyboard
		addKeyListener(new KeyListener() {

			public void keyTyped(KeyEvent e) {
			}

			public void keyReleased(KeyEvent e) {
			}

			public void keyPressed(KeyEvent e) {
				int k = e.getKeyCode();
				if (k == KeyEvent.VK_RIGHT || k == KeyEvent.VK_UP)
					incValue();
				else if (k == KeyEvent.VK_LEFT || k == KeyEvent.VK_DOWN)
					decValue();
			}
		});

		// Handle focus so that the knob gets the correct focus highlighting.
		addFocusListener(new FocusListener() {
			public void focusGained(FocusEvent e) {
				repaint();
			}

			public void focusLost(FocusEvent e) {
				repaint();
			}
		});
	}

	public void setDragType(int type) {
		dragType = type;
	}

	public int getDragType() {
		return dragType;
	}

	public boolean isManagingFocus() {
		return true;
	}

	public boolean isFocusTraversable() {
		return true;
	}

	private void incValue() {
		setValue(val + clickIncrement);
	}

	private void decValue() {
		setValue(val - clickIncrement);
	}

	public float getValue() {
		return val;
	}

	public void setValue(float val) {
		if (val < 0)
			val = 0;
		if (val > 1)
			val = 1;
		this.val = val;
		
		this.setToolTipText(Float.toString(this.val));
		
		ang = START_ANG - (float) LENGTH_ANG * val;
		repaint();
		fireChangeEvent();
	}

	public float getClickIncrement() {
		return clickIncrement;
	}

	public void setClickIncrement(float clickIncrement) {
		if (clickIncrement>=0.5f || clickIncrement<=0f)
			throw new IllegalArgumentException("value must be within the interval ]0,0.5[");
		this.clickIncrement = clickIncrement;
	}

	public float getDragIncrement() {
		return dragIncrement;
	}

	public void setDragIncrement(float dragIncrement) {
		if (dragIncrement>=0.5f || dragIncrement<=0f)
			throw new IllegalArgumentException("value must be within the interval ]0,0.5[");
		this.dragIncrement = dragIncrement;
	}

	public int getShadowSizeX() {
		return shadowSizeX;
	}

	public void setShadowSizeX(int shadowSizeX) {
		this.shadowSizeX = shadowSizeX;
	}

	public int getShadowSizeY() {
		return shadowSizeY;
	}

	public void setShadowSizeY(int shadowSizeY) {
		this.shadowSizeY = shadowSizeY;
	}
	
	public void addChangeListener(ChangeListener cl) {
		listenerList.add(ChangeListener.class, cl);
	}

	public void removeChangeListener(ChangeListener cl) {
		listenerList.remove(ChangeListener.class, cl);
	}

	public Dimension getMinimumSize() {
		return MIN_SIZE;
	}

	protected void fireChangeEvent() {
		// Guaranteed to return a non-null array
		Object[] listeners = listenerList.getListenerList();
		// Process the listeners last to first, notifying
		// those that are interested in this event
		for (int i = listeners.length - 2; i >= 0; i -= 2) {
			if (listeners[i] == ChangeListener.class) {
				// Lazily create the event:
				if (changeEvent == null)
					changeEvent = new ChangeEvent(this);
				((ChangeListener) listeners[i + 1]).stateChanged(changeEvent);
			}
		}
	}

	// Paint the DKnob
	public void paint(Graphics g) {
		int width = getWidth();
		int height = getHeight();

		
		size = Math.min(width, height) - 22;
		//size = Math.min(width, height) - 10; //be closer to the component below!
		middle = 10 + size / 2; //mouse only!

		if (g instanceof Graphics2D) {
			Graphics2D g2d = (Graphics2D) g;
			g2d.setBackground(getParent().getBackground());
			g2d.addRenderingHints(AALIAS);

			// For the size of the "mouse click" area
			hitArc.setFrame(4, 4, size + 12, size + 12);
		}

		// Paint the "markers"
		for (float a2 = START_ANG; a2 >= START_ANG - LENGTH_ANG; a2 = a2
				- (float) (LENGTH_ANG / 10.01)) {
			int x = 10 + size / 2 + (int) ((6 + size / 2) * Math.cos(a2));
			int y = 10 + size / 2 - (int) ((6 + size / 2) * Math.sin(a2));
			g.drawLine(10 + size / 2, 10 + size / 2, x, y);
		}

		// Set the position of the Zero
		//g.drawString("0", 2, size + 10);

		// Paint focus if in focus
		if (hasFocus()) {
			g.setColor(focusColor);
		} else {
			g.setColor(Color.white);
		}

		g.fillOval(10, 10, size, size);
		g.setColor(Color.gray); //TODO: UIManager.getColor("Label.background")
								//einfach mal bei google (search: "UIManager.getColor") 
								//die colors die am besten passen raussuchen
								//dann passt sich der knob auch dem LaF AN!!!
		g.fillOval(14 + shadowSizeX, 14 + shadowSizeY, size - 8, size - 8);

		g.setColor(Color.black);
		g.drawArc(10, 10, size, size, 315, 270);
		g.fillOval(14, 14, size - 8, size - 8);
		g.setColor(Color.white);

		int x = 10 + size / 2 + (int) (size / 2 * Math.cos(ang));
		int y = 10 + size / 2 - (int) (size / 2 * Math.sin(ang));
		g.drawLine(10 + size / 2, 10 + size / 2, x, y);
		g.setColor(Color.gray);
		int s2 = (int) Math.max(size / 6, 6);
		g.drawOval(10 + s2, 10 + s2, size - s2 * 2, size - s2 * 2);

		int dx = (int) (2 * Math.sin(ang));
		int dy = (int) (2 * Math.cos(ang));
		g.drawLine(10 + dx + size / 2, 10 + dy + size / 2, x, y);
		g.drawLine(10 - dx + size / 2, 10 - dy + size / 2, x, y);
	}
	
	
	
	
	
	
	public static void main(String[] args) throws ClassNotFoundException, InstantiationException, IllegalAccessException, UnsupportedLookAndFeelException {
	    JFrame win = new JFrame("DTest!");
	    win.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	    win.getContentPane().setLayout(new BorderLayout());
	    win.setSize(120,140);

	    JPanel panel = new JPanel(new BorderLayout());
	    //panel.setBackground(new Color(200,200,255));
	    win.getContentPane().add(panel, BorderLayout.CENTER);

	    RotaryKnob ts;
	    JLabel jl;
	    panel.add(ts = new RotaryKnob(), BorderLayout.CENTER);
	    panel.add(jl = new JLabel("Volume: 0"), BorderLayout.NORTH);
	    ts.setValue((float)1.0);
	    final JLabel jla = jl;
	    ts.addChangeListener(new ChangeListener() {
		    public void stateChanged(ChangeEvent e) {
		    	RotaryKnob t = (RotaryKnob) e.getSource();
			jla.setText("Volume: " + ((int)(15 * t.getValue())));
		    }
		});
	    win.show();
	}
}
