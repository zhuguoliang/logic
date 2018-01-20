/* Copyright 2018 Tymoteusz Blazejczyk
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

`include "logic.svh"

/* Module: logic_basic_gray2binary
 *
 * Converts input data coded in gray codes to binary data.
 *
 * Parameters:
 *  WIDTH   - Number of bits for input and output signals.
 *
 * Ports:
 *  i       - Input gray data.
 *  o       - Output binary data.
 */
module logic_basic_gray2binary #(
    int WIDTH = 1
) (
    input [WIDTH-1:0] i,
    output logic [WIDTH-1:0] o
);
    always_comb begin
        o[WIDTH-1] = i[WIDTH-1];

        for (int k = (WIDTH - 1); k > 0; --k) begin
            o[k - 1] = i[k - 1] ^ o[k];
        end
    end

`ifndef LOGIC_STD_OVL_DISABLED
    logic areset_n;
    logic [`OVL_FIRE_WIDTH-1:0] assert_unknown_input_fire;

    initial begin
        areset_n = '0;
        #1 areset_n = '1;
    end

    ovl_never_unknown_async #(
        .severity_level(`OVL_FATAL),
        .width(WIDTH),
        .property_type(`OVL_ASSERT),
        .msg("input signal i cannot be unknown")
    )
    assert_unknown_input (
        .reset(areset_n),
        .enable(1'b1),
        .test_expr(i),
        .fire(assert_unknown_input_fire)
    );
`endif

endmodule
