// System Verilog code for ALU
// Matt Kidder


module alu(input  logic [31:0] a, b,
           input  logic [1:0]  ALUControl,
           output logic [31:0] Result,
           output logic [3:0]  ALUFlags);


// internal variables
logic [31:0] sum, conintvb;
logic cout;
parameter V0 = 2'b00;
parameter V1 = 2'b01;
parameter V2 = 2'b10;
parameter V3 = 2'b11;

// using the code you provided in lab
assign conintvb = ALUControl[0] ? ~b : b;
assign {cout, sum} = a + conintvb + ALUControl[0];

// alu selection
always_comb

case (ALUControl)

V0: assign Result = sum; // set sum from ternary above if adding

V1:	assign Result = a + conintvb + 1'b1; // use flipped b from ternary above if subtracting

V2: assign Result = a & b;	// and

V3: assign Result = a | b;	// or

endcase


// set flags
assign ALUFlags[3] = Result[31];                // Set negative to MSB value

assign ALUFlags[2] = (Result == 0);             // Set zero if result zero

assign ALUFlags[1] = (cout & ~ALUControl[1]);   // Set carry if cout is 1 and add/sub

assign ALUFlags[0] = ((~ALUControl[1]) &        // Set overflow if add/sub and a/sum opposite signs
                      (a[31] ^ sum[31]) &       // and a/b same sign on add or a/b diff sign on sub
                      (~(ALUControl[0] ^ a[31] ^ b[31])));

endmodule
