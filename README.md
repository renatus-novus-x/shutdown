# shutdown

[English](README.md) | [日本語](README.ja.md)

<p align="center">
  <img src="images/teaser.gif" alt="shutdown running on the Sharp X68000" width="824" height="580">
</p>

<p align="center">
  <strong><a href="https://uraraworks.github.io/WebX68k/?cpu=10&ram=12&fd1=https://cdn.jsdelivr.net/gh/renatus-novus-x/shutdown@main/dist/shutdown.zip&run=1">▶ Launch shutdown in WebX68k</a></strong>
</p>

A minimal Human68k utility for powering off or rebooting Sharp X68000 systems.

## Usage

```text
shutdown -h now        Power off the system
shutdown -r now        Reboot the system
shutdown --direct now  Power off through $E8E00F (experimental)
shutdown -?            Show usage
shutdown --help        Show usage
```

Running `shutdown` without arguments only displays the usage. This is also what
the generated disk image does from `AUTOEXEC.BAT` after Human68k starts.

The normal power-off and reboot commands use X68000 `TRAP #10`. The experimental
`--direct` command bypasses that handler and writes directly to `$E8E00F` for
hardware diagnosis. Complete file operations before using any shutdown command.

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