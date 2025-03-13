SECTIONS
{
  . = 0x80000;

  _text_boot_start = .;
  .text.boot :
  {
    *(.text.boot)      
  }
  _text_boot_end = .;
  
  . = ALIGN(8);
  _text_start = .;
  .text :
  { 
    *(.text .gnu.linkonce.t.*)         
  }
  . = ALIGN(8);
  _text_end = .;
  
  _rodata_start = .;
  .rodata :
  { 
    *(.rodata .rodata.* .gnu.linkonce.r.*)        
  }
  _rodata_end = .; 
    
  _data_start = .;  
  /*.data 0x90000 : */
  .data :   
  {     
    *(.data .data.* .gnu.linkonce.d.*)         
  }
  _data_end = .;
  
  . = ALIGN(8);
  _bss_start = .;  
  .bss : 
  { 
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)         
  }
  . = ALIGN(8);
  _bss_end = .;

  . = ALIGN(8);
  _pg_data_start = .;  
  .pg_data :
  { 
    init_pg_dir = .;
    . += 4194304; 
    init_pg_dir_end = .;
  }
  _pg_data_end = .;    
}