// Testbench for ALU
// Matt Kidder

module testbench();

logic[31:0] a, b, yexpected;
logic[1:0] ALUControl;
logic[3:0] ALUFlags;
logic[31:0] Result;
// Chase Weaver helped me to see I was not setting the logic types
// correctly in the line above and to move the internals inside of
// the instantiated dut

// instantiate device under test
alu dut(.a, .b, .ALUControl, .Result, .ALUFlags);

logic [31:0] vectornum, errors; // bookkeeping variables
logic [103:0] testvectors[10000:0]; // array of testvectors
logic clk, reset;
// generate clock
always // no sensitivity list, so it always executes
begin
clk = 1; #5; clk = 0; #5;
end

// at start of test, load vectors and pulse reset
initial
begin
$readmemh("alu.tv", testvectors);
vectornum = 0; errors = 0;
reset = 1; #27; reset = 0;
end
// Note: $readmemh reads testvector files written in
// hexadecimal
// apply test vectors on rising edge of clk

always @(posedge clk)
begin
#1; {ALUControl, a, b, yexpected, ALUFlags} = testvectors[vectornum];
end

// check results on falling edge of clk
always @(negedge clk)
if (~reset) begin // skip during reset
if (Result !== yexpected) begin
	$display("Error: inputs = %h", {a, b, ALUControl});
	$display(" outputs = %h (%h expected)",Result,yexpected);
	errors = errors + 1;
	end

// Note: to print in hexadecimal, use %h. For example,
// $display(“Error: inputs = %h”, {a, b, c});

// increment array index and read next testvector
vectornum = vectornum + 1;
if (testvectors[vectornum] === 104'bx) begin
	$display("%d tests completed with %d errors",
	vectornum, errors);
	$stop;
	end
		end
endmodule
// === and !== can compare values that are 1, 0, x, or z.
