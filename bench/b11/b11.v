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

/*verilator lint_off WIDTH*/

module b11(
        input clock,
        input reset,
        input[5:0] x_in,
        input stbi,
        output reg[5:0] x_out
        );

`define s_reset 4'b0000
`define s_datain 4'b0001
`define s_spazio 4'b0010
`define s_mul 4'b0011
`define s_somma 4'b0100
`define s_rsum 4'b0101
`define s_rsot 4'b0110
`define s_compl 4'b0111
`define s_dataout 4'b1000

//reg[6:0] r_in;
reg[5:0] r_in;
//reg[3:0] stato;
reg[3:0] stato;
//reg[6:0] cont;
reg[5:0] cont;
//reg[8:0] cont1;
//reg[9:0] cont1;
   integer cont1;

always @ (posedge clock)
begin
        if (reset)
        begin
                stato = `s_reset;
                r_in = 1'b0;
                cont = 1'b0;
                cont1 = 1'b0;
                x_out = 1'b0;
        end
        else
        begin
                case (stato)
                        `s_reset:
                        begin
                                cont = 1'b0;
                                r_in = x_in;
                                x_out = 1'b0;
                                stato = `s_datain;
                        end

                        `s_datain:
                        begin
                                r_in = x_in;
                                if (stbi)
                                        stato = `s_datain;
                                else
                                        stato = `s_spazio;
                        end

                        `s_spazio:
                        begin
                                if (r_in == 0 || r_in == 63)
                                begin
                                        if (cont < 25)
                                                cont = cont + 1;
                                        else
                                                cont = 1'b0;
                                        cont1 = r_in;
                                        stato = `s_dataout;
                                end
                                else if (r_in <= 26)
                                        stato = `s_mul;
                                else
                                        stato = `s_datain;
                        end

                        `s_mul:
                        begin
                                if (r_in % 2 == 1)
                                        cont1 = cont * 2;
                                else
                                        cont1 = cont;
                                stato = `s_somma;
                        end

                        `s_somma:
                        begin
                                if ( ((r_in % 4) / 2) == 1)
                                begin
                                        cont1 = r_in + cont1;
                                        stato = `s_rsum;
                                end
                                else
                                begin
                                        cont1 = r_in - cont1;
                                        stato = `s_rsot;
                                end
                        end

                        `s_rsum:
                        begin
                                if (cont1 > 26)
                                begin
                                        cont1 = cont1 - 26;
                                        stato = `s_rsum;
                                end
                                else
                                        stato = `s_compl;
                        end

                        `s_rsot:
                        begin
                                if (cont1 > 63)
                                begin
                                        cont1 = cont1 + 26;
                                        stato = `s_rsot;
                                end
                                else
                                        stato = `s_compl;
                        end

                        `s_compl:
                        begin
                                if (((r_in / 4) % 4) == 0)
                                        cont1 = cont1 - 21;
                                else if (((r_in / 4) % 4) == 1)
                                        cont1 = cont1 - 42;
                                else if (((r_in / 4) % 4 )== 2)
                                        cont1 = cont1 + 7;
                                else
                                        cont1 = cont1 + 28;
                                stato = `s_dataout;
                        end

                        `s_dataout:
                          begin
                 if ( cont1 < 0)
                   x_out = (-cont1) % 64;
                 else
                   x_out = (cont1) % 64;
                     stato = `s_datain;
                                // x_out = cont1;
                                // stato = `s_datain;
                        end

                        default:
                             stato = `s_reset;

                endcase
        end
end

endmodule
