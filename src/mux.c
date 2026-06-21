// need to combine two 2-to-1 multiplexers
// consider inputs A, B and control S
// evaluates to A if S = 0, or B if S = 1
// so output is 1 if A = 1 and S = 0, or if B = 1 and S = 1
// therefore 2-to-1 multiplexer output is OR(AND(A, NOT(S)), AND(B, S))

// how to combine this into a 4-to-1?
