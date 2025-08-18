.text
.global _start
_start:

mrs r0, SCTLR_EL3 ; Read SCTLR_EL3 into Xt
orr r0, r0, #(1 << 2) ; Clear C bit (bit 2)
orr r0, r0, #(1 << 12) ; Clear I bit (bit 12)
msr SCTLR_EL3, r0 ; Write Xt to SCTLR_EL3
