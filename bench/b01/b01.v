/***********************************************************************
        Copyright (C) 2012,
        Virginia Polytechnic Institute & State University
        
        This verilog file is transformed from ITC 99 benchmark
        which is available from
        http://www.cad.polito.it/downloads/tools/itc99.html This
        verilog file was originally coverted by Mr. Min Li under
        the supervision of Dr. Michael S. Hsiao, in the Bradley
        Department of Electrical Engineering, VPI&SU, in 2012. We
        used a commertial Tool: VHDL2verilog 17.01g for initial
        convertion and made necessary changes manually. The
        verilog file is verified by random simulation.

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

`define false 1'b 0
`define FALSE 1'b 0
`define true 1'b 1
`define TRUE 1'b 1

module b01 (
   line1,
   line2,
   reset,
   outp,
   overflw,
   clock);
 
input   line1; 
input   line2; 
input   reset; 
output   outp; 
output   overflw; 
input   clock; 

reg     outp; 
reg     overflw; 
parameter a = 0; 
parameter b = 1; 
parameter c = 2; 
parameter e = 3; 
parameter f = 4; 
parameter g = 5; 
parameter wf0 = 6; 
parameter wf1 = 7; 
reg     [2:0]  process_1_stato; 


always @(posedge clock or posedge reset)
   begin : process_1
   if (reset === 1'b 1)
      begin
      process_1_stato = a;   
      outp <= 1'b 0;   
      overflw <= 1'b 0;   
      end
   else
      begin
      case (process_1_stato)
      a:
         begin
         if (line1 === 1'b 1 & line2 === 1'b 1)
            begin
            process_1_stato = f;   
            end
         else
            begin
            process_1_stato = b;   
            end
         outp <= line1 ^ line2;   
         overflw <= 1'b 0;   
         end
      e:
         begin
         if (line1 === 1'b 1 & line2 === 1'b 1)
            begin
            process_1_stato = f;   
            end
         else
            begin
            process_1_stato = b;   
            end
         outp <= line1 ^ line2;   
         overflw <= 1'b 1;   
         end
      b:
         begin
         if (line1 === 1'b 1 & line2 === 1'b 1)
            begin
            process_1_stato = g;   
            end
         else
            begin
            process_1_stato = c;   
            end
         outp <= line1 ^ line2;   
         overflw <= 1'b 0;   
         end
      f:
         begin
         if (line1 === 1'b 1 | line2 === 1'b 1)
            begin
            process_1_stato = g;   
            end
         else
            begin
            process_1_stato = c;   
            end
         outp <= ~(line1 ^ line2);   
         overflw <= 1'b 0;   
         end
      c:
         begin
         if (line1 === 1'b 1 & line2 === 1'b 1)
            begin
            process_1_stato = wf1;   
            end
         else
            begin
            process_1_stato = wf0;   
            end
         outp <= line1 ^ line2;   
         overflw <= 1'b 0;   
         end
      g:
         begin
         if (line1 === 1'b 1 | line2 === 1'b 1)
            begin
            process_1_stato = wf1;   
            end
         else
            begin
            process_1_stato = wf0;   
            end
         outp <= ~(line1 ^ line2);   
         overflw <= 1'b 0;   
         end
      wf0:
         begin
         if (line1 === 1'b 1 & line2 === 1'b 1)
            begin
            process_1_stato = e;   
            end
         else
            begin
            process_1_stato = a;   
            end
         outp <= line1 ^ line2;   
         overflw <= 1'b 0;   
         end
      wf1:
         begin
         if (line1 === 1'b 1 | line2 === 1'b 1)
            begin
            process_1_stato = e;   
            end
         else
            begin
            process_1_stato = a;   
            end
         outp <= ~(line1 ^ line2);   
         overflw <= 1'b 0;   
         end
      endcase
      end
   end


endmodule // module b01

