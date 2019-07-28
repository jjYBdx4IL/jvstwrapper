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
package jvst.examples.javafx.ui;

import javafx.scene.*;
import javafx.scene.paint.*;
import javafx.scene.shape.*;
import javafx.stage.*;

/**
 * @author campbell
 */

def thumbWidth = 20;
def thumbHeight = 12;

class SliderThumb extends CustomNode {

    override protected function create() : Node {
        Group {
            cursor: Cursor.HAND
            content: [
            Rectangle {
                width: thumbWidth
                height: thumbHeight
                arcWidth: 7
                arcHeight: 7
                fill: LinearGradient {
                    startX: 0
                    startY: 0
                    endX: 0
                    endY: 1
                    stops: [
                        Stop { offset: 0.0  color: Color.rgb(107, 107, 107) },
                        Stop { offset: 0.55 color: Color.BLACK },
                        Stop { offset: 0.7  color: Color.rgb( 75,  75,  75) },
                        Stop { offset: 1.0  color: Color.rgb( 23,  23,  23) },
                    ]
                }
                stroke: LinearGradient {
                    startX: 0
                    startY: 0
                    endX: 0
                    endY: 1
                    stops: [
                        Stop { offset: 0.0 color: Color.rgb(  3,   3,   3) },
                        Stop { offset: 1.0 color: Color.rgb( 82,  82,  82) },
                    ]
                }
                strokeWidth: 1
            },
            Polygon {
                translateX: thumbWidth/2-3.8
                translateY: thumbHeight/2
                points: [0, 0, 3, -3, 3, 3]
                fill: Color.WHITE
            },
            Polygon {
                translateX: thumbWidth/2+1.8
                translateY: thumbHeight/2
                points: [3, 0, 0, -3, 0, 3]
                fill: Color.WHITE
            },
            ]
        }
    }
}

package class Slider extends CustomNode {

    public var value = 0.0;
    public var minimum = 0.0;
    public var maximum = 1.0;

    public var width = 150;
    
    // the value adjusted to the range [0,1]
    var adjValue = bind (value - minimum) / (maximum - minimum);
    var origValue = 0.0;

    override protected function create() : Node {
        Group {
            content: [
            Rectangle {
                width: bind width
                height: 8
                arcWidth: 10
                arcHeight: 10
                fill: LinearGradient {
                    startX: 0
                    startY: 0
                    endX: 0
                    endY: 1
                    stops: [
                        Stop { offset: 0.0 color: Color.rgb(172, 172, 172) },
                        Stop { offset: 0.6 color: Color.rgb(115, 115, 115) },
                        Stop { offset: 1.0 color: Color.rgb(124, 124, 124) },
                    ]
                }
                stroke: LinearGradient {
                    startX: 0
                    startY: 0
                    endX: 0
                    endY: 1
                    stops: [
                        Stop { offset: 0.0 color: Color.rgb( 15,  15,  15) },
                        Stop { offset: 1.0 color: Color.rgb(224, 224, 224) },
                    ]
                }
            },
            SliderThumb {
                translateX: bind (adjValue * width) - (thumbWidth / 2)
                translateY: -2
                onMousePressed: function(e) {
                    origValue = adjValue;
                }
                onMouseDragged: function(e) {
                    var v = origValue + (e.dragX / width);
                    if (v < 0) {
                        v = 0;
                    } else if (v > 1) {
                        v = 1;
                    }
                    value = minimum + (v * (maximum-minimum));
                }
            }
            ]
        }
    }
}

function run() {
Stage {
    width: 300
    height: 100
    scene: Scene {
        content: [
            Slider {translateX: 20 translateY: 20}
        ]
    }
}
}
