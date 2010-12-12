vs.2.0

def				c0, 0,0,0,0
def				c1, 0.0f,-1.0f,0.0f,0.0f
dcl_position	v0
dcl_normal 		v1


// 1) Position�� World/view/projection matrix�� ���� ��ȯ�Ѵ�.
m4x4	r0,		v0,		c4
m4x4	oPos,	r0,		c8

// 2) Normal���� ���.
m3x3	r1.xyz,	v1,		c4		; r1�� Normal��

// 3) Eye Vector�� ���.
add		r5,		c3,		-r0		; C9�� Camera position
mov		r5.w,	c0.x			;
nrm		r6,		r5				; r6�� Eye Vector

// 4) Environment Mapping�� Vector���.
dp3		r7,		r6,		r1					; Eye ���� Normal��.
add		r7.w,	r7.w,	r7.w		; 2*Dot(Eye,Normal)
mad		oT0.xyz,r1,		r7.w,	-r6	; 2*Dot(Eye,Normal)xNormal-Eye