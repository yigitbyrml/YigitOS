#include "idt.h"
#include "config.h"
#include "kernel.h"
#include "io/io.h"
#include "memory/memory.h"

struct InterruptDescriptor32 idt_descriptors[YIGITOS_TOTAL_INTERRUPTS];
struct InterruptDescriptor32_R idtr_descriptor;

extern void idt_load(struct InterruptDescriptor32_R* ptr);
extern void int21h();
extern void no_interrupt();

void int21h_handler()
{
    print("Keyboard pressed!\n");
    outb(0x20, 0x20);
}

void no_interrupt_handler()
{
    outb(0x20, 0x20);
}

void idt_zero()
{
    print("Divide by zero error\n");
}

void idt_set(int interrupt_no, void* address)
{
    struct InterruptDescriptor32* desc = &idt_descriptors[interrupt_no];
    desc->offset_1 = (uint32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0x00;
    desc->type_attributes = 0xEE;
    desc->offset_2 = (uint32_t) address >> 16;
}

void idt_init(){

    memset(idt_descriptors,0,sizeof(idt_descriptors));

    idtr_descriptor.limit = sizeof(idt_descriptors) -1;
    idtr_descriptor.base = (uint32_t) idt_descriptors;
    for (int i = 0; i < YIGITOS_TOTAL_INTERRUPTS; i++)
    {
        idt_set(i, no_interrupt);
    }

    idt_set(0, idt_zero);
    idt_set(0x21, int21h);

    idt_load(&idtr_descriptor);
}