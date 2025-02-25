SECTIONS
{
  . = 0x80000;
  .text : 
  { 
    *(.text.boot)
    *(.text .gnu.linkonce.t.*) 
  }
  .rodata : { *(.rodata .rodata.* .gnu.linkonce.r.*) }
  .data : { *(.data .data.* .gnu.linkonce.d.*) }
  .bss : 
  { 
    . = ALIGN(8);
    _bss_start = .;
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)   
    . = ALIGN(8);
    _bss_end = .;
  }

  /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }  
}