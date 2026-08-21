#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <x68k/dos.h>
#include <x68k/iocs.h>

#define POWER_OFF_COMMAND  0x58363803UL
#define REBOOT_COMMAND     0x5836386bUL
#define REBOOT_MESSAGE_FRAMES 15U
#define POWER_OFF_REGISTER (*(volatile uint8_t *)0x00E8E00FUL)
#define MFP_GPIP ((const void *)0x00E88001UL)

#define CENTISEC_PER_DAY 8640000L
#define GPIP_VDISP 0x10
#define WAIT_VDISP_TIMEOUT_CS 100

static void print_usage(void){
  puts("Usage: shutdown {-h|-r|--direct} now");
  puts("  -h        Power off the system");
  puts("  -r        Reboot the system");
  puts("  --direct  Power off through $E8E00F (experimental)");
  puts("  -?        Show this help");
}

static inline long ontime_diff_cs(struct iocs_time start, struct iocs_time end){
  return ((long)end.day - (long)start.day) * CENTISEC_PER_DAY
       + (long)end.sec - (long)start.sec;
}

static inline uint8_t read_gpip(void){
  return (uint8_t)_iocs_b_bpeek(MFP_GPIP);
}

static int wait_vdisp(void){
  struct iocs_time start = _iocs_ontime();
  int level = read_gpip() & GPIP_VDISP;

  for (;;) {
    int next = read_gpip() & GPIP_VDISP;
    if (ontime_diff_cs(start, _iocs_ontime()) > WAIT_VDISP_TIMEOUT_CS) return -1;
    if (next != level) {
      level = next;
      if (level != 0) return 0;
    }
  }
}

static void wait_vdisp_frames(uint32_t frames){
  while (frames != 0) {
    if (wait_vdisp() != 0) return;
    --frames;
  }
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
    /* WebX68k and some other emulators may reset before presenting the current frame. */
    wait_vdisp_frames(REBOOT_MESSAGE_FRAMES);
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