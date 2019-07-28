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

import javafx.data.pull.PullParser;
import javafx.data.xml.QName;
import javafx.io.http.HttpRequest;
import javafx.scene.Cursor;
import javafx.scene.CustomNode;
import javafx.scene.Group;
import javafx.scene.text.Text;

/**
 * @author Sergey A. Malenkov
 */

public class Weather extends CustomNode {
  public var celcius = true;
  public var code: String;

  public var location: String;
  public var forecast: Forecast[];

  var index = 0;
  var newForecast = bind forecast[index] on replace oldForecast {
    oldForecast.visible = false;
    newForecast.visible = true;
  }
  var url = bind if (celcius)
            then "http://weather.yahooapis.com/forecastrss?p={code}&u=c"
            else "http://weather.yahooapis.com/forecastrss?p={code}&u=f" on replace {
    if (url.length() == 55) {
      location = "Loading...";
      delete forecast;
      index = 0;
      HttpRequest {
        location: url
        onDone: function() {
          if (forecast.isEmpty()) 
            location = "Loading error"
        }
        onInput: function(input) {
          try {
            PullParser {
              var wind: Wind;
              var temp: String;
              var speed: String;
              input: input
              onEvent: function(event) {
                if ((event.type == PullParser.START_ELEMENT) and (event.qname.prefix == "yweather")) {
                  if (event.qname.name == "location") {
                    location = event.getAttributeValue(QName{name:"city"});
                    def region = event.getAttributeValue(QName{name:"region"});
                    if (0 < region.length()) {
                      location = "{location}, {region}"
                    }
                  }
                  else if (event.qname.name == "units") {
                    temp  = event.getAttributeValue(QName{name:"temperature"});
                    speed = event.getAttributeValue(QName{name:"speed"})
                  }
                  else if (event.qname.name == "wind") {
                    wind = Wind {
                      angle: event.getAttributeValue(QName{name:"direction"})
                      speed: event.getAttributeValue(QName{name:"speed"})
                      unit:  speed
                    }
                  }
                  else if (event.qname.name == "condition") {
                    insert Forecast {
                      day: "Now"
                      high: event.getAttributeValue(QName{name:"temp"})
                      text: event.getAttributeValue(QName{name:"text"})
                      code: event.getAttributeValue(QName{name:"code"})
                      unit: temp
                      wind: wind
                    } into forecast
                  }
                  else if (event.qname.name == "forecast") {
                    insert Forecast {
                      day:  event.getAttributeValue(QName{name:"day"})
                      low:  event.getAttributeValue(QName{name:"low"})
                      high: event.getAttributeValue(QName{name:"high"})
                      text: event.getAttributeValue(QName{name:"text"})
                      code: event.getAttributeValue(QName{name:"code"})
                      unit: temp
                    } into forecast
                  }
                }
              }
            }.parse()
          } finally {
            input.close()
          }
        }
      }.start()
    }
  }
  override function create() {
    Group {
      translateY: Config.SPACE
      content: [
        RoundRect {
          node: Text {
            content: bind location
            fill: Config.FONT_COLOR
            font: Config.LARGE_FONT
          }
        }
        RoundRect {
          visible: bind newForecast != null
          fit: false
          node: Text {
            content: bind newForecast.day
            fill: Config.FONT_COLOR
            font: Config.SMALL_FONT
          }
          cursor: Cursor.HAND
          onMousePressed: function(event) {
            index = if (1 + index < sizeof forecast)
              then 1 + index
              else 0
          }
        }
        Group {
          translateY: Config.SPACE_DOUBLE + Config.LARGE_FONT.size
          content: bind forecast
        }
      ]
    }
  }
}
