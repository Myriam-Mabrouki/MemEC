.text

.global _start

_start:
	mrs x0, SCTLR_EL3
	orr x0, x0, #(1 << 2)
	orr x0, x0, #(1 << 12)
	msr SCTLR_EL3, x0
	mov     x0, #0
	mov     w8, #93
	svc     #0
