ENTRY(entry);

MEMORY
{
        flash (rx)  : ORIGIN = 0x08000000, LENGTH = 2048k
        ccm         : ORIGIN = 0x10000000, LENGTH = 64k
        ram (rwx)   : ORIGIN = 0x20000000, LENGTH = 192k
        bkram       : ORIGIN = 0x40024000, LENGTH = 4k
}

_heap_size = 256;
_stack_size = 1024;

SECTIONS
{
        .text :
        {
                KEEP(*(.vectors))
                *(.text*)
                . = ALIGN(4);
        } > flash

        . = ALIGN(4);
        _etext = .;

        .data :
        {
                _data = .;
                *(.data*)
                . = ALIGN(4);
                _edata = .;
        } > ram AT > flash
        _data_loadaddr = LOADADDR(.data);

        .bss :
        {
                *(.bss*)
                *(COMMON)
                . = ALIGN(4);
                _ebss = .;
        } > ram

        .heap (NOLOAD) :
        {
                *(.heap*)
                _heap = .;
        } > ram

        . = ALIGN(4);
        end = .;
}

PROVIDE(_stack = ORIGIN(ram) + LENGTH(ram));
