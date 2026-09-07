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
