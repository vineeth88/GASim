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

`timescale 1 ns / 1 ns // timescale for following modules


module b14 (
   clock,
   reset,
   addr,
   datai,
   datao,
   rd,
   wr);
 

input   clock; 
input   reset; 
output   [31:0] addr; 
input   [31:0] datai; 
output   [31:0] datao; 
output   rd; 
output   wr; 

reg     [31:0] addr; 
reg     [31:0] datao; 
reg     rd; 
reg     wr; 
parameter process_1_FETCH = 0; 
parameter process_1_EXEC = 1; 
integer  process_1_reg0; 
integer  process_1_reg1; 
integer  process_1_reg2; 
integer  process_1_reg3; 
reg      process_1_B; 
reg     [31:0]  process_1_MAR; 
integer  process_1_MBR; 
reg     [1:0]  process_1_mf; 
reg     [2:0]  process_1_df; 
reg      process_1_cf; 
reg     [3:0]  process_1_ff; 
reg     [31:0]  process_1_tail; 
integer  process_1_IR; 
reg      process_1_state; 
integer  process_1_r; 
integer  process_1_m; 
integer  process_1_t; 
integer  process_1_d; 
integer  process_1_temp; 
reg     [1:0]  process_1_s; 


always @(posedge clock or posedge reset)
   begin : process_1
   if (reset === 1'b 1)		// 0
      begin
      process_1_MAR = 0;   
      process_1_MBR = 0;   
      process_1_IR = 0;   
      process_1_d = 0;   
      process_1_r = 0;   
      process_1_m = 0;   
      process_1_s = 0;   
      process_1_temp = 0;   
      process_1_mf = 0;   
      process_1_df = 0;   
      process_1_ff = 0;   
      process_1_cf = 0;   
      process_1_tail = 0;   
      process_1_B = 1'b 0;   
      process_1_reg0 = 0;   
      process_1_reg1 = 0;   
      process_1_reg2 = 0;   
      process_1_reg3 = 0;   
      addr <= 0;   
      rd <= 1'b 0;   
      wr <= 1'b 0;   
      datao <= 0;   
      process_1_state = process_1_FETCH;   
      end
   else		//	 210 
      begin
      rd <= 1'b 0;   
      wr <= 1'b 0;   
      case (process_1_state)
      process_1_FETCH:		// 1
         begin
         process_1_MAR = process_1_reg3 %  /* VHDL ** operator */ 2**20;   
         addr <= process_1_MAR;   
         rd <= 1'b 1;   
         process_1_MBR = datai;   
         process_1_IR = process_1_MBR;   
         process_1_state = process_1_EXEC;   
         end
      process_1_EXEC:		// 209
         begin
         if (process_1_IR < 0)	// 2
            begin
            process_1_IR = -process_1_IR;
            end
         process_1_mf = process_1_IR /  /* VHDL ** operator */ (2**27) % 4;   
         process_1_df = process_1_IR /  /* VHDL ** operator */ (2**24) %  /* VHDL ** operator */ (2**3);
         process_1_ff = process_1_IR /  /* VHDL ** operator */ (2**19) %  /* VHDL ** operator */ (2**4);   
         process_1_cf = process_1_IR /  /* VHDL ** operator */ (2**23) % 2;   
         process_1_tail = process_1_IR %  /* VHDL ** operator */ 2**20;   
         process_1_reg3 = process_1_reg3 %  /* VHDL ** operator */ (2**29) + 8;   
         process_1_s = process_1_IR /  /* VHDL ** operator */ (2**29) % 4;   
         case (process_1_s)
         0:		// 3
            begin
            process_1_r = process_1_reg0;   
            end
         1:		// 4
            begin
            process_1_r = process_1_reg1;   
            end
         2:		// 5
            begin
            process_1_r = process_1_reg2;   
            end
         3:		// 6
            begin
            process_1_r = process_1_reg3;   
            end
         endcase
         case (process_1_cf)
         1:		// 63
            begin
            case (process_1_mf)
            0:		// 7
               begin
               process_1_m = process_1_tail;   
               end
            1:		// 8
               begin
               process_1_m = datai;   
               addr <= process_1_tail;   
               rd <= 1'b 1;   
               end
            2:		// 9
               begin
               addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
               rd <= 1'b 1;   
               process_1_m = datai;   
               end
            3:		// 10
               begin
               addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
               rd <= 1'b 1;   
               process_1_m = datai;   
               end
            endcase
            case (process_1_ff)
            0:		// 13
               begin
               if (process_1_r < process_1_m)	// 11
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 12
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            1:		// 16
               begin
               if (~(process_1_r < process_1_m))	// 14
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 15
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            2:		// 19
               begin
               if (process_1_r === process_1_m)		// 17
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 18
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            3:		// 22
               begin
               if (~(process_1_r === process_1_m))	// 20
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 21
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            4:		// 25
               begin
               if (~(process_1_r > process_1_m))	// 23
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 24
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            5:		//	28
               begin
               if (process_1_r > process_1_m)		// 26
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 27
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            6:		// 32
               begin
               if (process_1_r >  /* VHDL ** operator */ (2**30) - 1)	// 29
                  begin
                  process_1_r = process_1_r -  /* VHDL ** operator */ (2**30);   
                  end
               if (process_1_r < process_1_m)		// 30
                  begin
                  process_1_B = 1'b 1;   
                  end
               else			// 31
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            7:		// 36
               begin
               if (process_1_r >  /* VHDL ** operator */ (2**30) - 1)	// 33
                  begin
                  process_1_r = process_1_r -  /* VHDL ** operator */ (2**30);   
                  end
               if (~(process_1_r < process_1_m))		// 34
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 35
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            8:		// 39
               begin
               if (process_1_r < process_1_m | process_1_B === 1'b 1)	// 37
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 38
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            9:		// 42
               begin
               if (~(process_1_r < process_1_m) | process_1_B === 1'b 1)	// 40
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 41
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            10:		// 45
               begin
               if (process_1_r === process_1_m | process_1_B === 1'b 1)		// 43
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 44
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            11:		// 48
               begin
               if (~(process_1_r === process_1_m) | process_1_B === 1'b 1)		// 46
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 47
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            12:		// 51
               begin
               if (~(process_1_r > process_1_m) | process_1_B === 1'b 1)		// 49
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 50
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            13:		// 54
               begin
               if (process_1_r > process_1_m | process_1_B === 1'b 1)		// 52
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 53
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            14:		// 58
               begin
               if (process_1_r >  /* VHDL ** operator */ (2**30) - 1)		// 55
                  begin
                  process_1_r = process_1_r -  /* VHDL ** operator */ (2**30);   
                  end
               if (process_1_r < process_1_m | process_1_B === 1'b 1)		// 56
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 57
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            15:		// 62
               begin		
               if (process_1_r >  /* VHDL ** operator */ (2**30) - 1)		// 59
                  begin
                  process_1_r = process_1_r -  /* VHDL ** operator */ (2**30);   
                  end
               if (~(process_1_r < process_1_m) | process_1_B === 1'b 1)	// 60
                  begin
                  process_1_B = 1'b 1;   
                  end
               else		// 61
                  begin
                  process_1_B = 1'b 0;   
                  end
               end
            endcase
            end
         0:		// 208
            begin
            if (~(process_1_df === 7))		// 202
               begin
               if (process_1_df === 5)		// 65
                  begin
                  if (~process_1_B === 1'b 1)		// 64
                     begin
                     process_1_d = 3;   
                     end
                  end
               else if (process_1_df === 4 )		// 67
                  begin
                  if (process_1_B === 1'b 1)		// 66
                     begin
                     process_1_d = 3;   
                     end
                  end
               else if (process_1_df === 3 )		// 68
                  begin
                  process_1_d = 3;   
                  end
               else if (process_1_df === 2 )		// 69
                  begin
                  process_1_d = 2;   
                  end
               else if (process_1_df === 1 )		// 70
                  begin
                  process_1_d = 1;   
                  end
               else if (process_1_df === 0 )		// 71
                  begin
                  process_1_d = 0;   
                  end
               case (process_1_ff)
               0:	// 81
                  begin
                  case (process_1_mf)
                  0:		// 72
                     begin
                     process_1_m = process_1_tail;   
                     end
                  1:		// 73
                     begin
                     process_1_m = datai;   
                     addr <= process_1_tail;   
                     rd <= 1'b 1;   
                     end
                  2:		// 74
                     begin
                     addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end	
                  3:		// 75
                     begin
                     addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  endcase
                  process_1_t = 0;   
                  case (process_1_d)
                  0:	// 76
                     begin
                     process_1_reg0 = process_1_t - process_1_m;   
                     end
                  1:	// 77
                     begin
                     process_1_reg1 = process_1_t - process_1_m;   
                     end
                  2:	// 78
                     begin
                     process_1_reg2 = process_1_t - process_1_m;   
                     end
                  3:	// 79
                     begin
                     process_1_reg3 = process_1_t - process_1_m;   
                     end
                  default:	// 80
                     ;
                  endcase
                  end
               1:	// 86
                  begin
                  case (process_1_mf)
                  0:	// 82
                     begin
                     process_1_m = process_1_tail;   
                     end
                  1:	// 83
                     begin
                     process_1_m = datai;   
                     addr <= process_1_tail;   
                     rd <= 1'b 1;   
                     end
                  2:	// 84
                     begin
                     addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  3:	// 85
                     begin
                     addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  endcase
                  process_1_reg2 = process_1_reg3;   
                  process_1_reg3 = process_1_m;   
                  end
               2:		// 96
                  begin
                  case (process_1_mf)
                  0:	// 87
                     begin
                     process_1_m = process_1_tail;   
                     end
                  1:	// 88
                     begin
                     process_1_m = datai;   
                     addr <= process_1_tail;   
                     rd <= 1'b 1;   
                     end
                  2:	// 89
                     begin
                     addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  3:	// 90
                     begin
                     addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  endcase
                  case (process_1_d)
                  0:	// 91
                     begin
                     process_1_reg0 = process_1_m;   
                     end
                  1:	// 92
                     begin
                     process_1_reg1 = process_1_m;   
                     end
                  2:	// 93
                     begin	
                     process_1_reg2 = process_1_m;   
                     end
                  3:	// 94
                     begin
                     process_1_reg3 = process_1_m;   
                     end
                  default:	// 95
                     ;
                  endcase
                  end
               3:	// 106
                  begin
                  case (process_1_mf)
                  0:		// 97
                     begin
                     process_1_m = process_1_tail;   
                     end
                  1:		// 98
                     begin
                     process_1_m = datai;   
                     addr <= process_1_tail;   
                     rd <= 1'b 1;   
                     end
                  2:		// 99
                     begin
                     addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  3:		// 100
                     begin
                     addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  endcase
                  case (process_1_d)
                  0:	// 101
                     begin
                     process_1_reg0 = process_1_m;   
                     end
                  1:	// 102
                     begin
                     process_1_reg1 = process_1_m;   
                     end
                  2:	// 103
                     begin
                     process_1_reg2 = process_1_m;   
                     end
                  3:	// 104
                     begin
                     process_1_reg3 = process_1_m;   
                     end
                  default:	// 105
                     ;
                  endcase
                  end
               4:		// 116
                  begin
                  case (process_1_mf)	
                  0:		// 107
                     begin
                     process_1_m = process_1_tail;   
                     end
                  1:		// 108
                     begin
                     process_1_m = datai;   
                     addr <= process_1_tail;   
                     rd <= 1'b 1;   
                     end
                  2:		// 109
                     begin
                     addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  3:		// 110
                     begin
                     addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  endcase
                  case (process_1_d)
                  0:		// 111
                     begin
                     process_1_reg0 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:		// 112
                     begin
                     process_1_reg1 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:		// 113
                     begin
                     process_1_reg2 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:		// 114
                     begin
                     process_1_reg3 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:	// 115
                     ;
                  endcase
                  end
               5:		// 126
                  begin
                  case (process_1_mf)
                  0:		// 117
                     begin
                     process_1_m = process_1_tail;   
                     end
                  1:		// 118
                     begin
                     process_1_m = datai;   
                     addr <= process_1_tail;   
                     rd <= 1'b 1;   
                     end	
                  2:		// 119
                     begin
                     addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  3:		// 120
                     begin
                     addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  endcase
                  case (process_1_d)
                  0:
                     begin		// 121
                     process_1_reg0 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:
                     begin		// 122
                     process_1_reg1 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:		// 123
                     begin
                     process_1_reg2 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:		// 124
                     begin
                     process_1_reg3 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:	// 125
                     ;
                  endcase
                  end
               6:			// 136
                  begin
                  case (process_1_mf)
                  0:		// 127
                     begin
                     process_1_m = process_1_tail;   
                     end
                  1:		// 128
                     begin
                     process_1_m = datai;   
                     addr <= process_1_tail;   
                     rd <= 1'b 1;   
                     end
                  2:		// 129
                     begin
                     addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  3:		// 130
                     begin
                     addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  endcase
                  case (process_1_d)
                  0:		// 131
                     begin
                     process_1_reg0 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:		// 132
                     begin
                     process_1_reg1 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:		// 133
                     begin
                     process_1_reg2 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:		// 134
                     begin
                     process_1_reg3 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:	// 135
                     ;
                  endcase
                  end
               7:			// 146
                  begin
                  case (process_1_mf)
                  0:		// 137
                     begin
                     process_1_m = process_1_tail;   
                     end
                  1:		// 138
                     begin
                     process_1_m = datai;   
                     addr <= process_1_tail;   
                     rd <= 1'b 1;   
                     end
                  2:		// 139
                     begin
                     addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  3:		// 140
                     begin
                     addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  endcase
                  case (process_1_d)
                  0:		// 141
                     begin
                     process_1_reg0 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:		// 142
                     begin	
                     process_1_reg1 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:		// 143
                     begin
                     process_1_reg2 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:		// 144
                     begin
                     process_1_reg3 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:		// 145
                     ;
                  endcase
                  end
               8:			// 156
                  begin
                  case (process_1_mf)
                  0:		// 147
                     begin
                     process_1_m = process_1_tail;   
                     end
                  1:		// 148
                     begin
                     process_1_m = datai;   
                     addr <= process_1_tail;   
                     rd <= 1'b 1;   
                     end
                  2:		// 149
                     begin
                     addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  3:		// 150
                     begin
                     addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  endcase
                  case (process_1_d)
                  0:		// 151
                     begin
                     process_1_reg0 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:		// 152
                     begin
                     process_1_reg1 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:		// 153
                     begin
                     process_1_reg2 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:		// 154
                     begin
                     process_1_reg3 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:		// 155
                     ;
                  endcase
                  end
               9:			// 166
                  begin
                  case (process_1_mf)
                  0:		// 157
                     begin
                     process_1_m = process_1_tail;   
                     end
                  1:		// 158
                     begin
                     process_1_m = datai;   
                     addr <= process_1_tail;   
                     rd <= 1'b 1;   
                     end
                  2:		// 159
                     begin
                     addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  3:		// 160
                     begin
                     addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  endcase
                  case (process_1_d)
                  0:		// 161
                     begin
                     process_1_reg0 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:		// 162
                     begin
                     process_1_reg1 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:		// 163
                     begin
                     process_1_reg2 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:		// 164
                     begin
                     process_1_reg3 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:		// 165
                     ;
                  endcase
                  end
               10:		// 176
                  begin
                  case (process_1_mf)
                  0:
                     begin
                     process_1_m = process_1_tail;   
                     end
                  1:
                     begin
                     process_1_m = datai;   
                     addr <= process_1_tail;   
                     rd <= 1'b 1;   
                     end
                  2:
                     begin
                     addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  3:
                     begin
                     addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  endcase
                  case (process_1_d)
                  0:
                     begin
                     process_1_reg0 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:
                     begin
                     process_1_reg1 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:
                     begin
                     process_1_reg2 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:
                     begin
                     process_1_reg3 = (process_1_r + process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:
                     ;
                  endcase
                  end
               11:		// 186
                  begin
                  case (process_1_mf)
                  0:	// 177
                     begin
                     process_1_m = process_1_tail;   
                     end
                  1:	// 178
                     begin
                     process_1_m = datai;   
                     addr <= process_1_tail;   
                     rd <= 1'b 1;   
                     end
                  2:	// 179
                     begin
                     addr <= (process_1_tail + process_1_reg1) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  3:	// 180
                     begin
                     addr <= (process_1_tail + process_1_reg2) %  /* VHDL ** operator */ 2**20;   
                     rd <= 1'b 1;   
                     process_1_m = datai;   
                     end
                  endcase
                  case (process_1_d)
                  0:	// 181
                     begin
                     process_1_reg0 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  1:	// 182
                     begin
                     process_1_reg1 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  2:	// 183
                     begin
                     process_1_reg2 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  3:	// 184
                     begin
                     process_1_reg3 = (process_1_r - process_1_m) %  /* VHDL ** operator */ (2**30);   
                     end
                  default:	// 185
                     ;
                  endcase
                  end
               12:		// 200
                  begin
                  case (process_1_mf)
                  0:	// 187
                     begin
                     process_1_t = process_1_r / 2;   
                     end
                  1:	// 189
                     begin	
                     process_1_t = process_1_r / 2;   
                     if (process_1_B === 1'b 1)		// 188
                        begin
                        process_1_t = process_1_t %  /* VHDL ** operator */ (2**29);   
                        end
                     end
                  2:	// 190
                     begin
                     process_1_t = process_1_r %  /* VHDL ** operator */ (2**29) * 2;   
                     end
                  3:	// 193
                     begin
                     process_1_t = process_1_r %  /* VHDL ** operator */ (2**29) * 2;   
                     if (process_1_t >  /* VHDL ** operator */ (2**30) - 1)		// 191
                        begin
                        process_1_B = 1'b 1;   
                        end
                     else		// 192
                        begin
                        process_1_B = 1'b 0;   
                        end
                     end
                  default:	// 194
                     ;
                  endcase
                  case (process_1_d)
                  0:		// 195
                     begin
                     process_1_reg0 = process_1_t;   
                     end
                  1:		// 196
                     begin
                     process_1_reg1 = process_1_t;   
                     end
                  2:		// 197
                     begin
                     process_1_reg2 = process_1_t;   
                     end
                  3:		// 198
                     begin
                     process_1_reg3 = process_1_t;   
                     end
                  default:	// 199
                     ;
                  endcase
                  end
               13,
               14,
               15:		// 201
                  ;
               endcase
               end
            else if (process_1_df === 7 )	// 207
               begin
               case (process_1_mf)
               0:		// 203
                  begin
                  process_1_m = process_1_tail;   
                  end
               1:		// 204
                  begin
                  process_1_m = process_1_tail;   
                  end
               2:		// 205
                  begin
                  process_1_m = process_1_reg1 %  /* VHDL ** operator */ 2**20 + process_1_tail % 
       /* VHDL ** operator */ 2**20;   
                  end
               3:		// 206
                  begin
                  process_1_m = process_1_reg2 %  /* VHDL ** operator */ 2**20 + process_1_tail % 
       /* VHDL ** operator */ 2**20;   
                  end
               endcase
               addr <= process_1_m % (2**20);   
//  addr <= m;
//  removed (!)fs020699
               wr <= 1'b 1;   
               datao <= process_1_r;   
               end
            end
         endcase
         process_1_state = process_1_FETCH;   
         end
      endcase
      end
   end

endmodule // module b14

