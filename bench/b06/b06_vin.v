/***********************************************************************
        Copyright (C) 2012,
        Virginia Polytechnic Institute & State University
        
        This verilog file is transformed from ITC 99 benchmark
        which is available from:
        http://www.cad.polito.it/downloads/tools/itc99.html.
        This verilog file was originally coverted by Mr. Min Li
        under the supervision of Dr. Michael S. Hsiao, in the
        Bradley Department of Electrical Engineering, VPI&SU, in
        2012. We made the conversion manually and verified it by 
        random simulation.

        This verilog file is released for research use only. This
        verilog file, or any derivative thereof, may not be
        reproduced nor used for any commercial product without the
        written permission of the authors.

        Mr. Min Li
        Research Assistant
        Bradley Department of Electrical Engineering
        Virginia Polytechnic Institute & State University
        Blacksburg, VA 24061

        Ph.: (540) 808-8129
        Fax: (540) 231-3362
        E-Mail: min.li@vt.edu
        Web: http://www.ece.vt.edu/mhsiao

***********************************************************************/

module b06(
	input clock,
	input reset,
	input eql,
	input cont_eql,
	output reg[2:1] cc_mux,
	output reg[2:1] uscite,
	output reg enable_count,
	output reg ackout
	);
	
`define s_init 		3'b000
`define s_wait		3'b001
`define s_enin 		3'b010
`define s_enin_w 	3'b011
`define s_intr 		3'b100
`define s_intr_1 	3'b101
`define s_intr_w 	3'b110

reg[2:0] curr_state;


always @ (posedge clock)
begin
	if (reset)	// 0
	begin
		curr_state <= `s_init;
		cc_mux <= 2'b00;
		enable_count <= 1'b0;
		ackout <= 1'b0;
		uscite <= 2'b00;
	end
	else
	begin
		if (cont_eql)
		begin
			ackout <= 1'b0;
			enable_count <= 1'b0;
		end
		else
		begin
			ackout <= 1'b1;
			enable_count <= 1'b1;
		end
		
		case (curr_state)
			`s_init:
			begin
				cc_mux <= 2'b01;
				uscite <= 2'b01;
				curr_state <= `s_wait;
			end
			
			`s_wait:
			begin
				if (eql)
				begin
					uscite <= 2'b00;
					cc_mux <= 2'b11;
					curr_state <= `s_enin;
				end
				else
				begin
					uscite <= 2'b01;
					cc_mux <= 2'b10;
					curr_state <= `s_intr_1;
				end
			end
			
			`s_intr_1:
			begin
				if (eql)
				begin
					uscite <= 2'b00;
					cc_mux <= 2'b11;
					curr_state <= `s_intr;
				end
				else
				begin
					uscite <= 2'b01;
					cc_mux <= 2'b01;
					curr_state <= `s_wait;
				end
			end
			
			`s_enin:
			begin
				if (eql)
				begin
					uscite <= 2'b00;
					cc_mux <= 2'b11;
					curr_state <= `s_enin;
				end
				else
				begin
					uscite <= 2'b01;
					ackout <= 1'b1;
					enable_count <= 1'b1;
					cc_mux <= 2'b01;
					curr_state <= `s_enin_w;
				end
			end
			
			`s_enin_w:
			begin
				if (eql)
				begin
					uscite <= 2'b01;
					cc_mux <= 2'b01;
					curr_state <= `s_enin_w;
				end
				else
				begin
					uscite <= 2'b01;
					cc_mux <= 2'b01;
					curr_state <= `s_wait;
				end
			end
			
			`s_intr:
			begin
				if (eql)
				begin
					uscite <= 2'b00;
					cc_mux <= 2'b11;
					curr_state <= `s_intr;
				end
				else
				begin
					uscite <= 2'b11;
					cc_mux <= 2'b10;
					curr_state <= `s_intr_w;
				end
			end
			
			`s_intr_w:
			begin
				if (eql)
				begin
					uscite <= 2'b11;
					cc_mux <= 2'b10;
					curr_state <= `s_intr_w;
				end
				else
				begin
					uscite <= 2'b01;
					cc_mux <= 2'b01;
					curr_state <= `s_wait;
				end
			end
			
			default:	// 22
				curr_state <= `s_init;
		endcase
	end
end
	
endmodule
