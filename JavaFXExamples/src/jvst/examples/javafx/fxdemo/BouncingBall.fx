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

import javafx.animation.*;
import javafx.scene.*;
import javafx.scene.shape.*;
import javafx.scene.paint.*;
import javafx.scene.transform.*;
import javafx.scene.input.*;
import javafx.scene.text.*;

import javafx.animation.transition.ParallelTransition;


public class BouncingBall extends SceneFromNode {
    //width and height of the screen
    var w = 240;
    var h = 320;
    // model variables
    def rad = 50; // radius of the ball
    var x = rad;    // x location of the ball
    var y = rad;    // y location of the ball
    var sx = 1.0;   // scale x
    var sy = 1.0;   // scale y
    var color1  = Color.web("#098cec");
    var color2  = Color.web("#7dd6ff");
    var color2b = Color.web("#d2eddf");
    var color3  = Color.web("#858789");

    // animate side to side
    var ax = Timeline {
        keyFrames:[
            KeyFrame { time: 0s values:  x => rad },
            KeyFrame { time: 3s values: x => w-rad tween Interpolator.LINEAR },
        ]
        autoReverse: true
        repeatCount: Timeline.INDEFINITE
    }

    // animate up and down
    var ay = Timeline {
        repeatCount: Timeline.INDEFINITE
        keyFrames: [
            KeyFrame { time: 0s values: y => rad },
            KeyFrame { time: 2.2s values: y => h-rad tween Interpolator.SPLINE(0,0,.5,0) },
            KeyFrame { time: 2.25s values: y => h-rad },
            KeyFrame { time: 4.5s values: y => rad tween Interpolator.SPLINE(0,0,0,0.5) },
        ]
    }

    // animate the squish when boucing
    var sxy =  Timeline {
        repeatCount: Timeline.INDEFINITE
        keyFrames: [
            KeyFrame { time: 2s values: [sx => 1.0, sy => 1.0 ] },
            KeyFrame { time: 2.25s
                values: [
                    sx => 1.2 tween Interpolator.LINEAR,
                    sy => 0.7 tween Interpolator.LINEAR,
                ]
            },
            KeyFrame { time: 2.5s
                values: [
                    sx => 1.0 tween Interpolator.LINEAR,
                    sy => 1.0 tween Interpolator.LINEAR,
                ]
            },
            KeyFrame { time: 4.5s
                values: [
                    sx => 1.0 tween Interpolator.LINEAR,
                    sy => 1.0 tween Interpolator.LINEAR,
                ]
            },
        ]
    }


    // loop the other timelines forever
    var clip = ParallelTransition {
        repeatCount: Timeline.INDEFINITE
        content: [ax,ay,sxy]
    }


    public override function getScene() : Object {
        return Scene {
            width: w
            height: h
            fill: color3
            content: [
                Text { font: Font { size: 20}  x: 3 y: h-12 content: "Click ball to start" fill: Color.WHITE},

                // the actual animated circle
                Circle {
                    centerX: 0  centerY: 0  radius: rad
                    stroke: Color.BLACK
                    fill: RadialGradient {
                        proportional: false  radius: 70
                        centerX: 0  centerY: 0
                        focusX: 20  focusY: -50
                        stops: [
                            Stop { offset: 0.0  color: color1 },
                            Stop { offset: 0.60 color: color2 },
                            Stop { offset: 1.0  color: color2b }
                        ]
                    }
                    transforms: bind [
                        Transform.translate(x, y),        // move left and right and up and down
                        Transform.translate(50.0, 50.0),  // translate to the center of the ball
                        Transform.scale(sx, sy),          // do the squish
                        Transform.translate(-50.0, -50.0) // un-translate
                    ]
                    onMousePressed: function(e:MouseEvent) {
                        clip.play();
                    }
                    onKeyPressed: function(e:KeyEvent) {
                        clip.play();
                    }
                }
            ]
        }
    }


}

