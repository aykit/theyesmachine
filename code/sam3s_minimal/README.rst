Reset
-----

- On reset, the processor loads the PC with the value of the reset vector, which is at address 0x00000004. This address is mapped to flash memory after reset (0x00400004). The address of `Reset_Handler` is stored at this location.
- `Reset_Handler` initializes the RAM, relocates the vector table, calls `__libc_init_array` and calls `main`.
