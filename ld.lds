ENTRY(main);

MEMORY
{
        flash (rx)  : ORIGIN = 0x08000000, LENGTH = 2048k
        ccm         : ORIGIN = 0x10000000, LENGTH = 64k
        sram1 (rwx) : ORIGIN = 0x20000000, LENGTH = 112k
        sram2 (rwx) : ORIGIN = 0x20010000, LENGTH = 16k
        sram3 (rwx) : ORIGIN = 0x20020000, LENGTH = 64k
        bkram       : ORIGIN = 0x40024000, LENGTH = 4k
}

_heap_size = 0;
_stack_size = 0;

_estack = ORIGIN(sram1) + LENGTH(sram1);

SECTIONS
{
        .isr_vector :
        {
                KEEP(*(.isr_vector))
        } > flash
        
        .text :
        {                
        } > flash

        .data :
        {
        } > sram1
}
