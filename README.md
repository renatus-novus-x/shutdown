# shutdown

A minimal Human68k utility for powering off or rebooting Sharp X68000 systems.

## Usage

```text
shutdown -h now    Power off the system
shutdown -r now    Reboot the system
shutdown -?        Show usage
shutdown --help    Show usage
```

Running `shutdown` without arguments only displays the usage. This is also what
the generated disk image does from `AUTOEXEC.BAT` after Human68k starts.

The command uses X68000 `TRAP #10`. Complete file operations before shutting
down or rebooting.

## Build

Build under WSL or Linux with the elf2x68k toolchain installed and
`m68k-xelf-gcc` available in `PATH`.

Required host tools:

```sh
sudo apt install python3 curl unar
```

Build all artifacts:

```sh
make -C src
```

The Makefile downloads Human68k 3.02 and a pinned `xdftool.py`, then creates:

```text
src/shutdown.x     Human68k executable
src/shutdown.xdf   Bootable Human68k disk image
dist/shutdown.zip  Distribution archive with the Human68k license
```

Inspect the generated disk image with:

```sh
make -C src check-xdf
```

Remove generated files with `make -C src clean`, or also remove downloaded
support files with `make -C src distclean`.
