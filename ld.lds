ENTRY(Reset_Handler);

MEMORY
{
        flash : ORIGIN = 0x08000000, LENGTH = 1024k
        ccm : ORIGIN = 0x10000000, LENGTH = 64k
        sram1 (rw) : ORIGIN = 0x20000000, LENGTH = 112k
        sram2 (rw) : ORIGIN = 0x20010000, LENGTH = 16k
        sram3 (rw) : ORIGIN = 0x20020000, LENGTH = 64k
        bkram: ORIGIN = 0x40024000, LENGTH = 4k
}

_heap_size = x;
_stack_size = x;

SECTIONS
{
        .isr_vector :
        {
                KEEP(*(.isr_vector))
        } > flash

        .text > flash
        .data > sram1
}
