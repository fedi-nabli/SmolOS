ORG 0x7c00
[BITS 16]

_start:
  jmp short start
  nop

start:
  ; RESET THE DISK
  ; DL Register contains current drive as passed to us by bios
  mov ah, 0x00
  int 0x13

  ; Segment where kernel will load
  mov ax, 0x7e0
  mov es, ax

; LOAD KERNEL INTO MEMORY
kerenl_load:
  mov ah, 0x02
  mov al, 15
  mov ch, 0
  mov cl, 2
  mov dh, 0
  mov bx, 0x00
  int 0x13
  jc .problem
  
  mov ax, 0x7e0
  mov ds, ax
  ; Jump to the loaded kernel
  jmp 0x7e0:00
.problem:
  mov si, problem_loading_kernel
  call print
  jmp $

print:
  push ax
  mov ah, 0eh
.nextc:
  lodsb
  cmp al, 0
  je .done
  int 0x10
  jmp .nextc
.done:
  pop ax
  ret

problem_loading_kernel: db 'Issue loading the kernel, please ensure medium is large enough and contains valid kernel', 0

times 510-($ -$$) db 0
dw 0xAA55