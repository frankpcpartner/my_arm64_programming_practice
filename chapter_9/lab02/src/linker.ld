SECTIONS
{
  . = 0x80000;

  .text.boot :
  {
    _text_boot_start = .;
    *(.text.boot)  
    _text_boot_end = .;
  }
  
  .text : 
  { 
    _text_start = .;
    *(.text .gnu.linkonce.t.*)     
    _text_end = .;
  }
  
  .rodata : 
  { 
    _rodata_start = .;
    *(.rodata .rodata.* .gnu.linkonce.r.*)    
    _rodata_end = .; 
  }
    
  #.data : 
  .data 0x90000 : 
  {     
    _data_start = .;  
    *(.data .data.* .gnu.linkonce.d.*)     
    _data_end = .;
  }
  
  . = ALIGN(8);
  .bss : 
  { 
    _bss_start = .;  
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)       
    _bss_end = .;
  }
  
  .pg_data :
  {
    init_pg_dir = .;
    . += 4194304; 
    init_pg_dir_end = .;
  }

  /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }  
}