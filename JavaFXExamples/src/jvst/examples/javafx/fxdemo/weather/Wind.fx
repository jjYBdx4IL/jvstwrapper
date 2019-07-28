/* 
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * Copyright 2008 Sun Microsystems, Inc. All rights reserved. Use is subject to license terms. 
 * 
 * This file is available and licensed under the following license:
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   * Neither the name of Sun Microsystems nor the names of its contributors 
 *     may be used to endorse or promote products derived from this software 
 *     without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package jvst.examples.javafx.fxdemo.weather;

import javafx.scene.CustomNode;
import javafx.scene.Group;
import javafx.scene.paint.LinearGradient;
import javafx.scene.paint.Stop;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Polygon;
import javafx.scene.text.Text;
import java.lang.Math;

/**
 * @author Sergey A. Malenkov
 */

public class Wind extends CustomNode {
  public-init var angle: String;
  public-init var speed: String;
  public-init var unit:  String;

  override function create() {
    def a = java.lang.Integer.parseInt(angle);
    def r = 3.5 * Config.SMALL_FONT.size;
    def x = r * Math.cos(Math.PI / 5);
    def y = r * Math.sin(Math.PI / 5);
    def text = Text {
      content: "{speed} {unit}"
      fill: Config.FONT_COLOR
      font: Config.SMALL_FONT
    }
    Group {
      translateX: bind scene.width - r - Config.SPACE
      translateY: r + Config.SPACE_DOUBLE
      rotate: a
      content: [
        Circle {
          radius: r
          strokeWidth: 2
          stroke: Config.LIGHT_COLOR
          fill: null
        }
        Polygon {
          points: [
            -x, -y,
             0,  r,
             x, -y
          ]
          stroke: Config.DARK_COLOR
          fill: LinearGradient {
            endX: 0
            stops: [
              Stop { offset: 0   color: Config.DARK_COLOR }
              Stop { offset: 1   color: Config.LIGHT_COLOR }
            ]
          }
        }
        Group {
          rotate: if ((90 <= a) and (a < 270))  then 180  else 0
          translateX: bind - text.boundsInLocal.width / 2
          translateY: bind - text.boundsInLocal.minY - y
          content: text
        }
      ]
    }
  }
}
