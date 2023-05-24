ORG 0x7c00
[BITS 16]

_start:
  jmp short start
  nop

start:
  mov si, message
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

message: db 'Hello World!', 0

times 510-($ -$$) db 0
dw 0xAA55