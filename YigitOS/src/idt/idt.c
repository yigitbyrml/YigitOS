#include "idt.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"

struct InterruptDescriptor32 idt_descriptors[YIGITOS_TOTAL_INTERRUPTS];
struct InterruptDescriptor32_register idtr_descriptor;

extern void idt_load(struct InterruptDescriptor32_register* ptr);

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

    idt_set(0, idt_zero);

    idt_load(&idtr_descriptor);
}