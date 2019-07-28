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
package jvst.examples.javafx.fxdemo;

/**
 * @author Vaibhav Choudhary
 */

import javafx.scene.*;
import javafx.scene.paint.*;
import javafx.scene.text.*;
import javafx.scene.transform.*;
import javafx.animation.*;
import javafx.scene.shape.*;

// Java Legacy
import java.util.*;

var months:String[] = ["JAN", "FEB", "MA80R", "APR", "MAY", "JUN",
				"JUL", "AUG", "SEP", "OCT", "NOV", "DEC"];

public class AnalogClock extends CustomNode, SceneFromNode {

    // Decides the main size of the clock.
    var radius: Number = 80;
    // shifting the X center
    var centerX: Number = 115 ;
    // shifting the Y center
    var centerY: Number = 144 ; 
    var hours:Number;
    var minutes:Number;
    var seconds:Number;
    var date:String;
    var month:String;
    var year:String;
    var combination: String;


    // intializing the watch
    init {
        var timeline = Timeline {
            repeatCount: Timeline.INDEFINITE
            keyFrames : [
                KeyFrame {
                    time : 1s
                    action: function() {
                        actionOnTick();
                    }
                }
            ]
        }
        timeline.play();
    }
    function createCalendar() {
        var date = new Date();
        def calendar = Calendar.getInstance();
        calendar.setTime(new Date()); // fix for mobile
        calendar
    }
    // action taken on one tick
    public function actionOnTick () {
        var calendar = createCalendar();
        seconds = calendar.get(Calendar.SECOND);
        minutes = calendar.get(Calendar.MINUTE);
        hours = calendar.get(Calendar.HOUR);
        date = String.valueOf(calendar.get(Calendar.DATE));
        if(date.length()!=2)  {
            date = "0{date}";
        }
        month = months[(calendar.get(Calendar.MONTH))];
        year = String.valueOf((calendar.get(Calendar.YEAR)));
        combination = "{date}-{month}-{year}";
    }

    // overriding the method create()
    public override function create(): Node {
        return Group {
            transforms: [
                Transform.translate(centerX, centerY)
            ]
            content: [
                Circle {
                    radius: radius + 20
                    fill: RadialGradient {
                        centerX: 0
                        centerY: 0
                        radius: radius + 20
                        proportional: false
                        stops: [

                            Stop { offset: 0.9 color: Color.SILVER },
                            Stop { offset: 1.0 color: Color.BLACK }
                        ]
                    }

                },
                Circle {
                    radius: radius + 10
                    stroke: Color.BLACK
                    fill: RadialGradient {
                        centerX: 0
                        centerY: 0
                        radius: 90
                        proportional: false
                        stops: [
                            Stop { offset: 0.0 color: Color.WHITE },
                            Stop { offset: 1.0 color: Color.CADETBLUE }

                        ]
                    }

                },
                Rectangle {
                    x: -35, y: 2*radius/3 - 15
                    width: 71 height: 20
                    fill: Color.GRAY
                    opacity:0.4
                    strokeWidth: 2
                    stroke: Color.BLACK
                    arcHeight:10
                    arcWidth:10
                },
                Text {
                    font: Font {
                        size: 11
                        name: "Arial"
                    }
                    x: -31 , y: 2 * radius / 3
                    content: bind combination
                },
                //setting the main digits 3,6,9,12
                for (i in [3, 6, 9, 12])
                Text {
                    transforms:bind [
                        Transform.translate(-5, 5)
                    ]
                    x: radius * (( i + 0 ) mod 2 * ( 2 - i / 3))
                    y: radius * (( i + 1 ) mod 2 * ( 3 - i / 3))
                    content: "{i}"
                    font: Font {
                        size: 11
                        name: "Arial"
                    }
                },
                // making dots on rest of the place
                for (i in [1..12])
                if (i mod 3 != 0 ) then Circle {
                    transforms:Rotate { angle: 30 * i }
                    centerX: radius
                    radius: 3
                    fill: Color.BLACK
                } else [ ],

                // circle at the core center
                Circle {
                    radius: 5
                    fill: Color.BLACK
                },
                // one more circle inside the above circle
                Circle {
                    radius: 3
                    fill: Color.GRAY
                },
                // second arm
                Line {
                    transforms: Rotate { angle: bind seconds * 6 }
                    endY: -radius - 3
                    strokeWidth: 2
                    stroke: Color.RED

                },
                // hour arm
                Path {
                    transforms: Rotate {
				angle: bind (hours + minutes / 60) * 30 - 90
			  }
                    fill: Color.BLACK
                    elements: [
                        MoveTo {x: 4, y: 4},
                        ArcTo {x: 4 y: -4 radiusX: 1 radiusY: 1},
                        LineTo{ x: radius - 15  y: 0},
                    ]
                },
                // minute arm
                Path {
                    transforms: Rotate { angle: bind minutes * 6 - 90 }
                    fill: Color.BLACK
                    elements: [
                        MoveTo {
                            x: 4,
                            y: 4},
                        ArcTo {
                            x: 4
                            y: -4
                            radiusX: 1
                            radiusY: 1},
                        LineTo{ 
                            x: radius
                            y: 0},
                    ]
                }

            ]
        };
    }


    public override function getScene() : Object {
        return Scene {
            fill: Color.WHITE
            content: [
                AnalogClock{}
            ]
        }
    }
}
