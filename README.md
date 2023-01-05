# armcat
A small ARM (32-bit) disassembler library for quickly disassembling ARM code.

<div align="center">
    <img src="https://camo.githubusercontent.com/3228eb2bfeb3062439f91719631e72a32a2f9d7697597aba77ef50cced0bd52c/68747470733a2f2f692e67697068792e636f6d2f6d656469612f6f7a3435454c59674d6f5956735a716d6f722f67697068792e77656270" width="250px"><br>
</div>

## Description
`armcat` is a relatively small ARM (32-bit) disassembler engine that allows you to disassemble 32-bit ARM code on the fly.

## API
```c
void armcat_free(armcat_disasm_t *disassembly);
```
```c
armcat_disasm_t *armcat_disasm(const void *buffer, const size_t nbytes);
```

### Built with
- C

## Getting started
### Compiling
To compile `armcat`, simply execute the following script:
- `./build.sh`

## Example
```c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "src/armcat.h"

int main(void) {
  const uint8_t instructions[] = {
    "\x08\x50\xa0\xe1" /* mov r5, r8 */
    "\x08\x20\x8a\xe2" /* add r2, sl, #8 */
  };

  armcat_disasm_t *disassembly = armcat_disasm(instructions, sizeof(instructions));
  if (!disassembly)
    return EXIT_FAILURE;

  printf("Disassembly:\n");

  for (int i = 0; i < disassembly->ninstr; ++i)
    printf("\t%s\n", disassembly->instructions[i].disasm_instr);

  armcat_free(disassembly);
  return EXIT_SUCCESS;
}
```

## Credits
```
https://github.com/xmmword
```
### Contributions 🎉
###### All contributions are accepted, simply open an Issue / Pull request.
