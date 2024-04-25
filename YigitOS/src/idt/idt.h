#ifndef IDT_H
#define IDT_H

#include <stddef.h>
#include <stdint.h>


struct InterruptDescriptor32 {
   uint16_t offset_1;        // offset bits 0..15
   uint16_t selector;        // a code segment selector in GDT or LDT
   uint8_t  zero;            // unused, set to 0
   uint8_t  type_attributes; // gate type, dpl, and p fields
   uint16_t offset_2;        // offset bits 16..31
}__attribute__((packed));

struct InterruptDescriptor32_register {
   uint16_t limit;        // offset bits 0..15
   uint32_t base;
}__attribute__((packed));

void idt_init();

#endif