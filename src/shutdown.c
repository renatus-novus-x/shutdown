#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <x68k/dos.h>

#define POWER_OFF_COMMAND  0x58363803UL
#define REBOOT_COMMAND     0x5836386bUL
#define POWER_OFF_REGISTER (*(volatile uint8_t *)0x00E8E00FUL)

static void print_usage(void){
  puts("Usage: shutdown {-h|-r|--direct} now");
  puts("  -h        Power off the system");
  puts("  -r        Reboot the system");
  puts("  --direct  Power off through $E8E00F (experimental)");
  puts("  -?        Show this help");
}

__attribute__((noreturn))
static void system_shutdown(unsigned long command){
  __asm__ volatile(
    "move.l %0,%%d0\n\t"
    "trap #10"
    :
    : "d"(command)
    : "d0", "cc", "memory"
  );

  for (;;) {
    __asm__ volatile("nop");
  }
}

static void system_power_off_direct(void){
  int old_ssp;

  old_ssp = _dos_super(0);
  POWER_OFF_REGISTER = 0x00;
  POWER_OFF_REGISTER = 0x0f;
  POWER_OFF_REGISTER = 0x0f;
  (void)_dos_super(old_ssp);
}

int main(int argc, char *argv[]){
  if (argc == 1) {
    print_usage();
    return 0;
  }

  if (argc == 2 &&
      (strcmp(argv[1], "-?") == 0 || strcmp(argv[1], "--help") == 0)) {
    print_usage();
    return 0;
  }

  if (argc != 3 || strcmp(argv[2], "now") != 0) {
    print_usage();
    return 1;
  }

  if (strcmp(argv[1], "-h") == 0) {
    puts("Powering off...");
    fflush(stdout);
    system_shutdown(POWER_OFF_COMMAND);
  }

  if (strcmp(argv[1], "-r") == 0) {
    puts("Rebooting...");
    fflush(stdout);
    system_shutdown(REBOOT_COMMAND);
  }

  if (strcmp(argv[1], "--direct") == 0) {
    puts("Powering off directly through $E8E00F...");
    fflush(0);
    system_power_off_direct();
    fputs("Direct power-off request failed.\n", stderr);
    return 1;
  }

  print_usage();
  return 1;
}