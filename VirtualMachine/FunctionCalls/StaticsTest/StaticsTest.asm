@256
D=A
@SP
M=D
@Sys.init_return0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@0
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.init_start
0;JMP
(Sys.init_return0)
(Sys.init_start)
D=0
@6
D=A
@SP
A=M
M=D
@SP
M=M+1
@8
D=A
@SP
A=M
M=D
@SP
M=M+1
@Class1.set_return1
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@2
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class1.set_start
0;JMP
(Class1.set_return1)
@SP
M=M-1
@SP
A=M
D=M
@5
M=D
@23
D=A
@SP
A=M
M=D
@SP
M=M+1
@15
D=A
@SP
A=M
M=D
@SP
M=M+1
@Class2.set_return2
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@2
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class2.set_start
0;JMP
(Class2.set_return2)
@SP
M=M-1
@SP
A=M
D=M
@5
M=D
@Class1.get_return3
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@0
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class1.get_start
0;JMP
(Class1.get_return3)
@Class2.get_return4
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
D=M
@5
D=D-A
@0
D=D-A
@ARG
M=D
@SP
D=M
@LCL
M=D
@Class2.get_start
0;JMP
(Class2.get_return4)
(WHILE)
@WHILE
0;JMP
(Class1.set_start)
D=0
@0
D=A
@ARG
A=D+M
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
@SP
A=M
D=M
@15
M=D
@1
D=A
@ARG
A=D+M
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
@SP
A=M
D=M
@16
M=D
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@frame0
M=D
@5
A=D-A
D=M
@retAddr0
M=D
@SP
M=M-1
@SP
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@frame0
D=M
@1
A=D-A
D=M
@THAT
M=D
@frame0
D=M
@2
A=D-A
D=M
@THIS
M=D
@frame0
D=M
@3
A=D-A
D=M
@ARG
M=D
@frame0
D=M
@4
A=D-A
D=M
@LCL
M=D
@retAddr0
A=M
0,JMP
(Class1.get_start)
D=0
@15
D=M
@SP
A=M
M=D
@SP
M=M+1
@16
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
@SP
A=M
D=M
@SP
A=M-1
A=M
D=A-D
@SP
M=M-1
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@frame1
M=D
@5
A=D-A
D=M
@retAddr1
M=D
@SP
M=M-1
@SP
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@frame1
D=M
@1
A=D-A
D=M
@THAT
M=D
@frame1
D=M
@2
A=D-A
D=M
@THIS
M=D
@frame1
D=M
@3
A=D-A
D=M
@ARG
M=D
@frame1
D=M
@4
A=D-A
D=M
@LCL
M=D
@retAddr1
A=M
0,JMP
(Class2.set_start)
D=0
@0
D=A
@ARG
A=D+M
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
@SP
A=M
D=M
@15
M=D
@1
D=A
@ARG
A=D+M
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
@SP
A=M
D=M
@16
M=D
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@frame2
M=D
@5
A=D-A
D=M
@retAddr2
M=D
@SP
M=M-1
@SP
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@frame2
D=M
@1
A=D-A
D=M
@THAT
M=D
@frame2
D=M
@2
A=D-A
D=M
@THIS
M=D
@frame2
D=M
@3
A=D-A
D=M
@ARG
M=D
@frame2
D=M
@4
A=D-A
D=M
@LCL
M=D
@retAddr2
A=M
0,JMP
(Class2.get_start)
D=0
@15
D=M
@SP
A=M
M=D
@SP
M=M+1
@16
D=M
@SP
A=M
M=D
@SP
M=M+1
@SP
M=M-1
@SP
A=M
D=M
@SP
A=M-1
A=M
D=A-D
@SP
M=M-1
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@frame3
M=D
@5
A=D-A
D=M
@retAddr3
M=D
@SP
M=M-1
@SP
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@frame3
D=M
@1
A=D-A
D=M
@THAT
M=D
@frame3
D=M
@2
A=D-A
D=M
@THIS
M=D
@frame3
D=M
@3
A=D-A
D=M
@ARG
M=D
@frame3
D=M
@4
A=D-A
D=M
@LCL
M=D
@retAddr3
A=M
0,JMP