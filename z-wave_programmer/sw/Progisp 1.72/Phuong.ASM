								;Laptrinh: Duc_friendly@yahoo.com
								;Gui tang Phuong yeu
									org 0
									main:
									mov p0,#0ffh
									mov p1,#0ffh
									mov p2,#0ffh
									mov p3,#0ffh
									mov r1,#0
								a1:
									lcall kieu3
									lcall 2ben1
									
									lcall 2ben2
									lcall choptat
									inc r1
									cjne r1,#1,a1
								a2:
									lcall kieu1
									
									lcall kieu2
									lcall choptat
									inc r1
									cjne r1,#2,a2
								a3:
									lcall kieu3
									lcall choptat
									inc r1
									cjne r1,#3,a3
								a4:
									lcall kieu4
									lcall kieu5
									lcall choptat
									inc r1
									cjne r1,#4,a4
								a5: 
									lcall kieu6
									lcall kieu7
									lcall choptat
									inc r1
									cjne r1,#5,a5
								a6:
									lcall kieu8
									lcall kieu9
									lcall choptat
									inc r1
									cjne r1,#6,a6
								a7:
									lcall kieu10
									lcall kieu11
									lcall choptat
									
									inc r1
									cjne r1,#7,a7
								a8:
									lcall kieu12
									lcall kieu13
									lcall choptat
									
									inc r1
									cjne r1,#8,a8
								a9:
									lcall sang32
									lcall sang33
									lcall choptat
									lcall kieu3
									inc r1
									cjne r1,#9,a9
									
									jmp main
							2ben1:
									
									mov 40h,#0ffh
									mov 41h,#0ffh
									mov 42h,#0ffh
									mov r0,#16
							lap2:
									mov 30h,#0ffh
									mov 31h,#0ffh
									mov 32h,#0ffh
									mov r3,0
									clr c
							lap1:	
									mov a,30h
									rlc a
									mov 30h,a
									mov a,31h
									rlc a
									mov 31h,a
									rrc a
									mov a,32h
									rrc a
									mov 32h,a
									anl a,42h
									mov p2,a
									mov a,31h
									anl a,40h
									mov p3,a
									mov a,30h
									anl a,41h
									mov p1,a
									mov p0,a
									call delay
									djnz r3,lap1
									mov 41h,p1
									mov 40h,p3
									mov 42h,p2
									djnz r0,lap2
									ret
									
							2ben2:
									
									mov 40h,#0ffh
									mov 41h,#0ffh
									mov 42h,#0ffh
									mov r0,#16
							lap4:
									mov 30h,#0ffh
									mov 31h,#0ffh
									mov 32h,#0ffh
									mov r3,0
									clr c
							lap3:	
									mov a,30h
									rrc a
									mov 30h,a
									rlc a
									mov a,31h
									rlc a
									mov 31h,a
									
									mov a,32h
									rrc a
									mov 32h,a
									anl a,42h
									mov p0,a
									mov p1,a
									mov a,31h
									anl a,40h
									mov p2,a
									mov a,30h
									anl a,41h
									mov p3,a
									
									call delay
									djnz r3,lap3
									mov 42h,p1
									mov 41h,p3
									mov 40h,p2
									djnz r0,lap4
									ret
						kieu1:
									
									mov 40h,#0ffh
									mov 41h,#0ffh
									mov 42h,#0ffh
									mov 43h,#0ffh
									mov 44h,#0
									mov 45h,#0
									mov r0,#16
							lap6:
									mov 30h,#0ffh
									mov 31h,#0ffh
									mov 33h,#0ffh
									mov 32h,#0ffh
									mov r3,0
									clr c
							lap5:	
									mov a,30h
									rlc a
									mov 44h,c
									mov 30h,a
									rrc a
									mov a,33h
									rlc a
									mov 33h,a
									mov 45h,c
									mov c,44h
									mov a,31h
									rlc a
									mov 31h,a
									mov c,45h
									mov a,32h
									rrc a
									mov 32h,a
									anl a,42h
									mov p0,a
									mov a,31h
									anl a,40h
									mov p3,a
									mov a,30h
									anl a,41h
									mov p1,a
									mov a,33h
									anl a,43h
									mov p2,a
									call delay
									djnz r3,lap5
									mov 41h,p1
									mov 40h,p3
									mov 42h,p0
									mov 43h,p2
									djnz r0,lap6
									ret
						kieu2:
									
									mov 40h,#0
									mov 41h,#0
									mov 42h,#0
									mov 43h,#0
									mov 45h,#0
									mov r0,#16
							lap8:
									mov 30h,#0
									mov 31h,#0
									mov 33h,#0
									mov 32h,#0
									mov r3,0
									setb c
							lap7:	
									mov a,30h
									rlc a
									mov 44h,c
									mov 30h,a
									rrc a
									mov a,33h
									rlc a
									mov 33h,a
									mov 45h,c
									mov c,44h
									mov a,31h
									rlc a
									mov 31h,a
									mov c,45h
									mov a,32h
									rrc a
									mov 32h,a
									orl a,42h
									mov p0,a
									mov a,31h
									orl a,40h
									mov p3,a
									mov a,30h
									orl a,41h
									mov p1,a
									mov a,33h
									orl a,43h
									mov p2,a
									call delay
									djnz r3,lap7
									mov 41h,p1
									mov 40h,p3
									mov 42h,p0
									mov 43h,p2
									djnz r0,lap8
									ret
						kieu3:
									mov p0,#0ffh
									mov p1,#0ffh
									mov p2,#0ffh
									mov p3,#0ffh
									mov r3,16
									setb c
							lap9:	
									mov a,p0
									rlc a
									mov p0,a
									mov p1,a
									mov a,p3
									rlc a
									mov p3,a
									rrc a
									mov a,p2
									rrc a
									mov p2,a
									lcall delay2
									dec r3
									cjne r3,#0,tieptuc
									jmp thoat
							tieptuc:
									mov a,r3
									mov b,#2
									div ab
									mov a,b
									cjne a,#0,nhan
									setb c
									jmp lap9
							nhan:
									clr c
									jmp lap9
							thoat:
									ret
							kieu4:
									mov 40h,#0ffh
									mov 41h,#0ffh
									mov 42h,#0ffh
									mov r0,#8
							nhan2:
									mov 30h,#0ffh
									mov 31h,#0ffh
									mov 32h,#0ffh
									mov r3,0
									clr c
						nhan1:	
									mov a,30h
									rlc a
									mov 30h,a
									
									rrc a
									mov a,32h
									rrc a
									mov 32h,a
									anl a,42h
									mov p0,a
									mov a,31h
									
									mov a,30h
									anl a,41h
									mov p1,a
									mov p2,a
									mov p3,a
									call delay
									djnz r3,nhan1
									mov 41h,p1
								
									mov 42h,p0
									djnz r0,nhan2
									ret	
						kieu5:
									mov 40h,#0ffh
									mov 41h,#0ffh
									mov 42h,#0ffh
									mov r0,#8
							nhan4:
									mov 30h,#0ffh
									mov 31h,#0ffh
									mov 32h,#0ffh
									mov r3,0
									clr c
						nhan3:	
									mov a,30h
									rrc a
									mov 30h,a
									
									rlc a
									mov a,32h
									rlc a
									mov 32h,a
									anl a,42h
									mov p0,a
									mov a,31h
									
									mov a,30h
									anl a,41h
									mov p1,a
									mov p2,a
									mov p3,a
									call delay
									djnz r3,nhan3
									mov 41h,p1
								
									mov 42h,p0
									djnz r0,nhan4
									ret	
							kieu6:
									mov 40h,#0
									mov 41h,#0
									mov 42h,#0
									mov r0,#8
							nhan6:
									mov 30h,#0
									mov 31h,#0
									mov 32h,#0
									mov r3,0
									setb c
						nhan5:	
									mov a,30h
									rrc a
									mov 30h,a
									
									rlc a
									mov a,32h
									rlc a
									mov 32h,a
									orl a,42h
									mov p0,a
									mov a,31h
									
									mov a,30h
									orl a,41h
									mov p1,a
									mov p2,a
									mov p3,a
									call delay
									djnz r3,nhan5
									mov 41h,p1
								
									mov 42h,p0
									djnz r0,nhan6
									ret	
							kieu7:
									mov 40h,#0
									mov 41h,#0
									mov 42h,#0
									mov r0,#8
							nhan8:
									mov 30h,#0
									mov 31h,#0
									mov 32h,#0
									mov r3,0
									setb c
						nhan7:	
									mov a,30h
									rlc a
									mov 30h,a
									
									rrc a
									mov a,32h
									rrc a
									mov 32h,a
									orl a,42h
									mov p0,a
									mov a,31h
									
									mov a,30h
									orl a,41h
									mov p1,a
									mov p2,a
									mov p3,a
									call delay
									djnz r3,nhan7
									mov 41h,p1
								
									mov 42h,p0
									djnz r0,nhan8
									ret	
						kieu8:
									mov p0,#0ffh
									mov p1,#0ffh
									mov p2,#0ffh
									mov p3,#0ffh
									mov r3,#8
									clr c
							nhan9:	
									mov a,p1
									rlc a
									mov p2,a
									mov p1,a
									mov p3,a
									mov a,p3
									rrc a
									mov a,p0
									rrc a
									mov p0,a
									
									call delay
									djnz r3,nhan9
									ret
						kieu9:
									mov p0,#0ffh
									mov p1,#0ffh
									mov p2,#0ffh
									mov p3,#0ffh
									mov r3,#8
									clr c
							nhan10:	
									mov a,p1
									rrc a
									mov p2,a
									mov p1,a
									mov p3,a
									mov a,p3
									rlc a
									mov a,p0
									rlc a
									mov p0,a
									
									call delay
									djnz r3,nhan10
									ret
							kieu10:
									mov p0,#0
									mov p1,#0
									mov p2,#0
									mov p3,#0
									mov r3,#8
									setb c
							nhan11:	
									mov a,p1
									rrc a
									mov p2,a
									mov p1,a
									mov p3,a
									mov a,p3
									rlc a
									mov a,p0
									rlc a
									mov p0,a
									
									call delay
									djnz r3,nhan11
									ret
							kieu11:
									mov p0,#0
									mov p1,#0
									mov p2,#0
									mov p3,#0
									mov r3,8
									setb c
							nhan12:	
									mov a,p1
									rlc a
									mov p2,a
									mov p1,a
									mov p3,a
									mov a,p3
									rrc a
									mov a,p0
									rrc a
									mov p0,a
									
									call delay
									djnz r3,nhan12
									ret
							kieu12:
									mov p0,#0ffh
									mov p1,#11111100b
									mov p2,#0ffh
									mov p3,#0ffh
									mov r3,#32
									clr c
							nhan13:	
									mov a,p1
									rlc a
									
									mov p1,a
									mov a,p3
									rlc a
									mov p3,a
									mov a,p2
									rlc a
									mov p2,a
									mov a,p0
									rrc a
									mov p0,a
									
									call delay
									djnz r3,nhan13
									ret
							kieu13:
									mov p0,#0ffh
									mov p1,#00111111b
									mov p2,#0ffh
									mov p3,#0ffh
									mov r3,#32
									clr c
							nhan14:	
									mov a,p1
									rrc a
									
									mov p1,a
									mov a,p0
									rlc a
									mov p0,a
									mov a,p2
									rrc a
									mov p2,a
									mov a,p3
									rlc a
									mov p3,a
									
									call delay
									djnz r3,nhan14
									ret
							choptat:
									mov r7,#10
									ll:
									mov p0,#0ffh
									mov p1,#0ffh
									mov p2,#0ffh
									mov p3,#0ffh
									call delay
									mov p0,#0
									mov p1,#0
									mov p2,#0
									mov p3,#0
									call delay
									djnz r7,ll
									ret
							sang32:
									mov 40h,#0ffh
									mov 41h,#0ffh
									mov 42h,#0ffh
									mov 43h,#0ffh
									mov r0,#32
							d2:
									mov 30h,#0ffh
									mov 31h,#0ffh
									mov 32h,#0ffh
									mov 33h,#0ffh
									mov r3,0
									clr c
							d1:	
									mov a,30h
									rlc a
									mov 30h,a
									mov a,31h
									rlc a
									mov 31h,a
									
									mov a,32h
									rlc a
									mov 32h,a
									mov a,33h
									rrc a
									mov 33h,a
									anl a,43h
									mov p0,a
									mov a,32h
									anl a,42h
									mov p2,a
									mov a,31h
									anl a,40h
									mov p3,a
									mov a,30h
									anl a,41h
									mov p1,a
									
									call delay1
									djnz r3,d1
									mov 41h,p1
									mov 40h,p3
									mov 42h,p2
									mov 43h,p0
									djnz r0,d2
									ret
						sang33:
									mov 40h,#0ffh
									mov 41h,#0ffh
									mov 42h,#0ffh
									mov 43h,#0ffh
									mov r0,#32
							d4:
									mov 30h,#0ffh
									mov 31h,#0ffh
									mov 32h,#0ffh
									mov 33h,#0ffh
									mov r3,0
									clr c
							d3:	
									mov a,30h
									rlc a
									mov 30h,a
									mov a,31h
									rrc a
									mov 31h,a
									
									mov a,32h
									rrc a
									mov 32h,a
									mov a,33h
									rrc a
									mov 33h,a
									anl a,43h
									mov p1,a
									mov a,32h
									anl a,42h
									mov p3,a
									mov a,31h
									anl a,40h
									mov p2,a
									mov a,30h
									anl a,41h
									mov p0,a
									
									call delay1
									djnz r3,d3
									mov 41h,p0
									mov 40h,p2
									mov 42h,p3
									mov 43h,p1
									djnz r0,d4
									ret
							
						
						
							delay:
									mov r5,#200
								lap:
									mov r6,#150
									
									djnz r6,$
									djnz r5,lap
									ret
									
							delay1:
									mov r5,#100
								lap91:
									mov r6,#100
									
									djnz r6,$
									djnz r5,lap91
									ret
							delay2:
									mov r5,#255
								lap99:
									mov r6,#255
									
								
									djnz r6,$
									djnz r5,lap99
									ret
									end