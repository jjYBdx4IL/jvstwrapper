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
import javafx.stage.*;

/**
 * @author campbell
 */

public class LabeledKnob extends CustomNode {
    public var text = "Blank";
    public var value = 0.0;
    public var minimum = 0.0;
    public var maximum = 1.0;

    public var minAngle = 45.0;
    public var maxAngle = -225.0;

    var lbl:Label;

    override protected function create() : Node {
        Group {
            content: [
                Knob {
                    minimum: bind minimum
                    maximum: bind maximum
                    value:   bind value with inverse
                    minAngle: bind minAngle
                    maxAngle: bind maxAngle
                },
                lbl = Label {
                    translateX: bind -(lbl.layoutBounds.width / 2)
                    translateY: 28
                    text: bind text
                    value: bind value
                },
            ]
        }
    }
}

function run() {
Stage {
    width: 400
    height: 300
    scene: Scene {
        fill: javafx.scene.paint.Color.BLACK
        content: [
            LabeledKnob {
                translateX: 40
                translateY: 30
                minimum: 45
                maximum: -225
                value: 45
                minAngle: 45
                maxAngle: -225
            }
        ]
    }
}
}
