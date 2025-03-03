SECTIONS
{
  . = 0xFFFF000000080000;
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
  . = ALIGN(8);
  .rela.dyn : {
    _rel_dyn_start = .;
    *(.rela*)
    _rel_dyn_end = .;
  }
  
  .pg_data :
  {
    init_pg_dir = .;
    . += 4194304; 
    init_pg_dir_end = .;
  }

  .gnu.hash : { *(.gnu.hash) }
  .gnu : { *(.gnu*) }
  /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }  
}

_rel_dyn_size = (_rel_dyn_end - _rel_dyn_start);