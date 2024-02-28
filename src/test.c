#include <assert.h>
#include "I2C_SLG46826_REG.h"

int main(int argc, char* argv[])
{
  /*
   * Order of bits in bit-field is implementation defined. Do the runtime
   * check that assumption in the header matches compiler's. Order assumed
   * is as per GP bits layout specification.
   * (There isn't a static/compile time check that could do. Unless compiler
   * fixes same order for any arch).
   */
  slg_register_t test = { .virtual_input_7 = 1 };
  assert(test.reg_7a == 0x01);

  return 0;
}
