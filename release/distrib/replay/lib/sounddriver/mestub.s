# -----------------------------------------
#include <regdef.h>

# -----------------------------------------
INTCTRL = $0
STATUS = $12
CAUSE = $13
EPC = $14
CONFIG = $16
INTERRUPT = $25
TAGLO = $28
TAGHI = $29

IXIST = 0x01
DXIST = 0x11

# -----------------------------------------
		.global me_disable_int
		.global me_sceKernelDcacheWritebackInvalidateAll
		.global me_stub
		.global me_stub_end
		.set    noreorder

# -----------------------------------------
    	.align   4
me_disable_int:
    	li      $v0, 0
	    mtic    $v0, $0                 # INTCTRL
	    sync
    	ctc0    $v0, $14
    	ctc0    $v0, $15
    	sync
    	li      $k0, 0xbc300000
    	sw      $v0, 8($k0)
    	sw      $v0, 0x18($k0)
	    sync
	    lui     $v0, 0x8000         	# int 31
	    sw      $v0, 8($k0)          	# set interrupt controller mask
	    mfc0    $v0, $12                # STATUS
	    li		$v0, 0
		or		$v0, $v0, 0b11110000000000000000000000000000
	    mtc0    $v0, $12                # STATUS set mips interrupt line mask
	    sync
	    nop
	    nop
	    nop
	    nop
	    nop
	    j		me_sceKernelDcacheWritebackInvalidateAll
	    nop
	    jr      $ra
	    nop

# -----------------------------------------
    	.align   4
me_sceKernelDcacheWritebackInvalidateAll:
	    cache   20, 0($0)
	    cache   20, 0($0)
	    cache   20, 64($0)
	    cache   20, 64($0)
	    cache   20, 128($0)
	    cache   20, 128($0)
	    cache   20, 192($0)
	    cache   20, 192($0)
	    cache   20, 256($0)
	    cache   20, 256($0)
	    cache   20, 320($0)
	    cache   20, 320($0)
	    cache   20, 384($0)
	    cache   20, 384($0)
	    cache   20, 448($0)
	    cache   20, 448($0)
	    cache   20, 512($0)
	    cache   20, 512($0)
	    cache   20, 576($0)
	    cache   20, 576($0)
	    cache   20, 640($0)
	    cache   20, 640($0)
	    cache   20, 704($0)
	    cache   20, 704($0)
	    cache   20, 768($0)
	    cache   20, 768($0)
	    cache   20, 832($0)
	    cache   20, 832($0)
	    cache   20, 896($0)
	    cache   20, 896($0)
	    cache   20, 960($0)
	    cache   20, 960($0)
	    cache   20, 1024($0)
	    cache   20, 1024($0)
	    cache   20, 1088($0)
	    cache   20, 1088($0)
	    cache   20, 1152($0)
	    cache   20, 1152($0)
	    cache   20, 1216($0)
	    cache   20, 1216($0)
	    cache   20, 1280($0)
	    cache   20, 1280($0)
	    cache   20, 1344($0)
	    cache   20, 1344($0)
	    cache   20, 1408($0)
	    cache   20, 1408($0)
	    cache   20, 1472($0)
	    cache   20, 1472($0)
	    cache   20, 1536($0)
	    cache   20, 1536($0)
	    cache   20, 1600($0)
	    cache   20, 1600($0)
	    cache   20, 1664($0)
	    cache   20, 1664($0)
	    cache   20, 1728($0)
	    cache   20, 1728($0)
	    cache   20, 1792($0)
	    cache   20, 1792($0)
	    cache   20, 1856($0)
	    cache   20, 1856($0)
	    cache   20, 1920($0)
	    cache   20, 1920($0)
	    cache   20, 1984($0)
	    cache   20, 1984($0)
	    cache   20, 2048($0)
	    cache   20, 2048($0)
	    cache   20, 2112($0)
	    cache   20, 2112($0)
	    cache   20, 2176($0)
	    cache   20, 2176($0)
	    cache   20, 2240($0)
	    cache   20, 2240($0)
	    cache   20, 2304($0)
	    cache   20, 2304($0)
	    cache   20, 2368($0)
	    cache   20, 2368($0)
	    cache   20, 2432($0)
	    cache   20, 2432($0)
	    cache   20, 2496($0)
	    cache   20, 2496($0)
	    cache   20, 2560($0)
	    cache   20, 2560($0)
	    cache   20, 2624($0)
	    cache   20, 2624($0)
	    cache   20, 2688($0)
	    cache   20, 2688($0)
	    cache   20, 2752($0)
	    cache   20, 2752($0)
	    cache   20, 2816($0)
	    cache   20, 2816($0)
	    cache   20, 2880($0)
	    cache   20, 2880($0)
	    cache   20, 2944($0)
	    cache   20, 2944($0)
	    cache   20, 3008($0)
	    cache   20, 3008($0)
	    cache   20, 3072($0)
	    cache   20, 3072($0)
	    cache   20, 3136($0)
	    cache   20, 3136($0)
	    cache   20, 3200($0)
	    cache   20, 3200($0)
	    cache   20, 3264($0)
	    cache   20, 3264($0)
	    cache   20, 3328($0)
	    cache   20, 3328($0)
	    cache   20, 3392($0)
	    cache   20, 3392($0)
	    cache   20, 3456($0)
	    cache   20, 3456($0)
	    cache   20, 3520($0)
	    cache   20, 3520($0)
	    cache   20, 3584($0)
	    cache   20, 3584($0)
	    cache   20, 3648($0)
	    cache   20, 3648($0)
	    cache   20, 3712($0)
	    cache   20, 3712($0)
	    cache   20, 3776($0)
	    cache   20, 3776($0)
	    cache   20, 3840($0)
	    cache   20, 3840($0)
	    cache   20, 3904($0)
	    cache   20, 3904($0)
	    cache   20, 3968($0)
	    cache   20, 3968($0)
	    cache   20, 4032($0)
	    cache   20, 4032($0)
	    cache   20, 4096($0)
	    cache   20, 4096($0)
	    cache   20, 4160($0)
	    cache   20, 4160($0)
	    cache   20, 4224($0)
	    cache   20, 4224($0)
	    cache   20, 4288($0)
	    cache   20, 4288($0)
	    cache   20, 4352($0)
	    cache   20, 4352($0)
	    cache   20, 4416($0)
	    cache   20, 4416($0)
	    cache   20, 4480($0)
	    cache   20, 4480($0)
	    cache   20, 4544($0)
	    cache   20, 4544($0)
	    cache   20, 4608($0)
	    cache   20, 4608($0)
	    cache   20, 4672($0)
	    cache   20, 4672($0)
	    cache   20, 4736($0)
	    cache   20, 4736($0)
	    cache   20, 4800($0)
	    cache   20, 4800($0)
	    cache   20, 4864($0)
	    cache   20, 4864($0)
	    cache   20, 4928($0)
	    cache   20, 4928($0)
	    cache   20, 4992($0)
	    cache   20, 4992($0)
	    cache   20, 5056($0)
	    cache   20, 5056($0)
	    cache   20, 5120($0)
	    cache   20, 5120($0)
	    cache   20, 5184($0)
	    cache   20, 5184($0)
	    cache   20, 5248($0)
	    cache   20, 5248($0)
	    cache   20, 5312($0)
	    cache   20, 5312($0)
	    cache   20, 5376($0)
	    cache   20, 5376($0)
	    cache   20, 5440($0)
	    cache   20, 5440($0)
	    cache   20, 5504($0)
	    cache   20, 5504($0)
	    cache   20, 5568($0)
	    cache   20, 5568($0)
	    cache   20, 5632($0)
	    cache   20, 5632($0)
	    cache   20, 5696($0)
	    cache   20, 5696($0)
	    cache   20, 5760($0)
	    cache   20, 5760($0)
	    cache   20, 5824($0)
	    cache   20, 5824($0)
	    cache   20, 5888($0)
	    cache   20, 5888($0)
	    cache   20, 5952($0)
	    cache   20, 5952($0)
	    cache   20, 6016($0)
	    cache   20, 6016($0)
	    cache   20, 6080($0)
	    cache   20, 6080($0)
	    cache   20, 6144($0)
	    cache   20, 6144($0)
	    cache   20, 6208($0)
	    cache   20, 6208($0)
	    cache   20, 6272($0)
	    cache   20, 6272($0)
	    cache   20, 6336($0)
	    cache   20, 6336($0)
	    cache   20, 6400($0)
	    cache   20, 6400($0)
	    cache   20, 6464($0)
	    cache   20, 6464($0)
	    cache   20, 6528($0)
	    cache   20, 6528($0)
	    cache   20, 6592($0)
	    cache   20, 6592($0)
	    cache   20, 6656($0)
	    cache   20, 6656($0)
	    cache   20, 6720($0)
	    cache   20, 6720($0)
	    cache   20, 6784($0)
	    cache   20, 6784($0)
	    cache   20, 6848($0)
	    cache   20, 6848($0)
	    cache   20, 6912($0)
	    cache   20, 6912($0)
	    cache   20, 6976($0)
	    cache   20, 6976($0)
	    cache   20, 7040($0)
	    cache   20, 7040($0)
	    cache   20, 7104($0)
	    cache   20, 7104($0)
	    cache   20, 7168($0)
	    cache   20, 7168($0)
	    cache   20, 7232($0)
	    cache   20, 7232($0)
	    cache   20, 7296($0)
	    cache   20, 7296($0)
	    cache   20, 7360($0)
	    cache   20, 7360($0)
	    cache   20, 7424($0)
	    cache   20, 7424($0)
	    cache   20, 7488($0)
	    cache   20, 7488($0)
	    cache   20, 7552($0)
	    cache   20, 7552($0)
	    cache   20, 7616($0)
	    cache   20, 7616($0)
	    cache   20, 7680($0)
	    cache   20, 7680($0)
	    cache   20, 7744($0)
	    cache   20, 7744($0)
	    cache   20, 7808($0)
	    cache   20, 7808($0)
	    cache   20, 7872($0)
	    cache   20, 7872($0)
	    cache   20, 7936($0)
	    cache   20, 7936($0)
	    cache   20, 8000($0)
	    cache   20, 8000($0)
	    cache   20, 8064($0)
	    cache   20, 8064($0)
	    cache   20, 8128($0)
	    cache   20, 8128($0)
	    jr      $ra
	    nop

# -----------------------------------------
me_stub:
    	li      $v0, 0
	    mtic    $v0, $0                 # disable interrupt controller
	    sync
	
	    li      $k0, 0xbc100000
	    li      $t0, 7
	    sw      $t0, 80($k0)
	    li      $t0, 2
	    sw      $t0, 64($k0)
	    mtc0    $zero, $28              # TAGLO
	    mtc0    $zero, $29              # TAGHI
	    li      $k1, 8192
a:
	    addi    $k1, $k1, -64
	    bne     $k1, $zero, a
	    cache   IXIST, 0($k1)
	    li      $k1, 8192
b:
	    addi    $k1, $k1, -64
	    bne     $k1, $zero, b
	    cache   DXIST, 0($k1)
	    mtc0    $zero, $13              # CAUSE
    
	    # Disable kernel memory protection
	    li      $t0, 0xbc00002c
	    li      $t1, 0xbc000000
	    li      $t2, -1
c:
	    sw      $t2, 0($t1)
	    bne     $t1, $t0, c
	    addiu   $t1, $t1, 4
	
	    li      $k0, 0x20000000
	    mtc0    $k0, $12                # STATUS
	    sync
	
	    li      $t0, 0xbfc00000
	    lw      $k0, 0x600($t0)
	    li      $sp, 0x80200000		    # Top of ME EDRAM as stack
	    jr      $k0                     # Run function located at 0xbfc00600
	    nop

# -----------------------------------------
me_stub_end:
