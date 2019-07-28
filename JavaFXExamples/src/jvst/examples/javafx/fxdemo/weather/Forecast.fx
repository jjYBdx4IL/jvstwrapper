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
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.HBox;
import javafx.scene.text.Text;
import javafx.scene.text.TextAlignment;

/**
 * @author Sergey A. Malenkov
 */

public class Forecast extends CustomNode {
  public-init var day:  String;
  public-init var low:  String;
  public-init var high: String;
  public-init var text: String;
  public-init var code: String;
  public-init var unit: String;
  public-init var wind: Wind on replace {
    visible = wind != null
  }
  override function create() {
    Group {
      content: [
        wind, 
        RoundRect {
          translateY: Config.OFFSET
          node: Text {
            content: text
            fill: Config.FONT_COLOR
            font: Config.LARGE_FONT
          }
        }
        RoundRect {
          translateY: Config.OFFSET
          fit: false
          node: if (low == "") Text {
            content: "{high}?{unit}"
            fill: Config.FONT_COLOR
            font: Config.LARGE_FONT
          } else HBox {
            content: [
              Text {
                content: "high:\nlow:"
                fill: Config.FONT_COLOR
                font: Config.SMALL_FONT
              }
              Text {
                textAlignment: TextAlignment.RIGHT
                content: "{high}?{unit}\n{low}?{unit}"
                fill: Config.FONT_COLOR
                font: Config.SMALL_FONT
              }
            ]
          }
        }
        ImageView {
          image: Image {
            backgroundLoading: true
            url: "http://l.yimg.com/us.yimg.com/i/us/nws/weather/gr/{code}n.png"
          }
        }
      ]
    }
  }
}
