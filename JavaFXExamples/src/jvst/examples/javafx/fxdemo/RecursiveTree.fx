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
import javafx.scene.paint.*;
import javafx.scene.transform.*;
import javafx.scene.shape.*;

public class RecursiveTree extends CustomNode, SceneFromNode {
    public var x = 0;
    public var y = 100;
    public var startDepth: Number = 3;
    public var leftFactor = 1.12;
    public var rightFactor = 1.6;
    public var leftAngle = 20.0;
    public var rightAngle = -45.0;
    public-init var maxAngle = 30.0;
    public var angle = 0.0;
    public var segmentLength = 15.0;
    public-init var cycleLength:Duration = 5s;

    public var colorR = 0;
    public var colorB = 0;

    public-read var anim = Timeline {
        repeatCount: Timeline.INDEFINITE
        keyFrames: [
            KeyFrame { time: 0s values: angle => 0 tween Interpolator.EASEBOTH},
            KeyFrame { time: cycleLength/4 values: angle => -maxAngle tween Interpolator.EASEBOTH},
            KeyFrame { time: cycleLength/4*2 values: angle => 0 },
            KeyFrame { time: cycleLength/4*3 values: angle => maxAngle tween Interpolator.EASEBOTH},
            KeyFrame { time: cycleLength/4*4 values: angle => 0  tween Interpolator.EASEBOTH},
            KeyFrame { time: cycleLength+2s values: angle => 0},
        ]
    };

    override public function create():Node {
        return Group {
            translateX: bind x
            translateY: bind y
            content: [
                create(startDepth,1)
            ]
        }
    }
    
    function create(depth:Number, angleAdjust:Number):Node {
        var adj = angleAdjust;
        var color = (depth*255/startDepth);
        var g = Group {
            transforms:  bind Transform.rotate(angle+adj, 0, 0),
            content: Line { 
                startX:0 startY:0 endX:0 endY:-depth*segmentLength
                stroke: Color.rgb(colorR,color,colorB)
            }
        };
        
        // recurse
        if(depth >= 1) {
            insert Group {
                transforms: Transform.translate(0,-depth*segmentLength)
                content: [
                    create(depth/leftFactor, leftAngle),
                    create(depth/rightFactor, rightAngle)
                ]
            } into g.content;
        }
        
        return g;
    }

    public override function getScene() : Object {
        var tree =  RecursiveTree {
            x: 150 y: 200
            angle: 45
            leftFactor: 1.3
            rightFactor: 1.3
        };
        tree.anim.play();

        return Scene {
            fill: Color.WHITE
            content: [
                tree
            ]
        }
    }

}

