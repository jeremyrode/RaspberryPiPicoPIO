#include <stdio.h>
#include <stdbool.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <blink.pio.h>
#define LED_BUILTIN 25

static PIO pio = pio0;
static uint state_machine_id = 0;

int sentZPadCommand(int command) {
  //To Do translate channel to pio and state_machine_id
  // ToDo put header and shift to MSBs
  if (command < 0 | command > 63) {
    printf("Error: Command out of range");
    return -1;
  }
  //Shift command to bits 20-26, where bits 27-32 are header (zeros)
  pio_sm_put_blocking(pio, state_machine_id, command << 20);
}


int main() {
  stdio_init_all();

  int offset = pio_add_program(pio, &blink_program);
  blink_program_init(pio, state_machine_id, offset, LED_BUILTIN);

  while(1) {
    uint8_t in;
    in = getchar(); // Get the command from USB serial
    if (in == '0') {
      sentZPadCommand(0);
      printf("Got 0\n"); //Test debug serial return
    }
    else if (in == '8') {
      sentZPadCommand(8);
      printf("Got 8\n"); //Test debug serial return
    }
    else if (in == '6') {
      sentZPadCommand(63);
      printf("Got 6\n"); //Test debug serial return
    }
  }
}
