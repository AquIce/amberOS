# OS Dev Notes

## Memory Loading

For the first call to the function

- Point ES:DI at the destination buffer for the list
- Clear EBX
- Set EDX to the magic number 0x534D4150
- Set EAX to 0xE820 (note that the upper 16-bits of EAX should be set to 0)
- Set ECX to 24
- Do an INT 0x15

### Layout

| Address Space | Description |
| - | - |
| 0x1000, ... | Kernel buffer |
| 0x3FFE, 0x3FFF | E820 entry count |
| 0x4000, 0x4FFF | E820 buffer |
| 0x5000, 0x6FFF | Real-mode stack |
| 0x7C00, ... | Boot sector |
| ..., 0x90000 | Protected-mode stack |

## Interrupts

### IDT

| 63 .. 48 | 47 | 46 .. 45 | 44 | 43 .. 40 | 39 .. 32 | 31 .. 16 | 15 .. 0 |
| - | - | - | - | - | - | - | - |
| Offset (31 .. 16) | P | DPL | 0 | Gate Type | Reserved | Segment selector | Offset (15 .. 0) |


> Offset

32-bit split value, address of the entry point of the [ISR](https://wiki.osdev.org/Interrupt_Service_Routines)

> Selector

Segment selector with multiple fields that must points to a valid GDT segment

> Gate Type

4-bit value which defines the type of this **Interrupt Descriptor**:

- `0x5`: Task Gate (=> Offset should be zero because unused)
- `0x6`: 16-bit Interrupt Gate
- `0x7`: 16-bit Trap Gate
- `0xE`: 32-bit Interrupt Gate
- `0xF`: 32-bit Trap Gate

> DPL

2-bit value defining the [CPU Privilege Levels](https://wiki.osdev.org/Security#Rings) which are allowed to access this interrupt via `int` (hardware interrupts ignore this mechanism)

> P

Present bit (must be set to 1 for the entry to be valid)
