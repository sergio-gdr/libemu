sources = files(
	'ipc.c',
	'list.c',
	'init.c'
)

headers = files(
	'include/emu_ipc.h',
	'include/emu_cpu.h',
	'include/emu_ppu.h',
	'include/emu_list.h',
	'include/libemu.h'
)

libemu = library(
	'emu',
	include_directories: ['include'],
    sources: [sources],
	install: true
)
install_headers(headers)

pkg = import('pkgconfig')

pkg.generate(
    name: 'libemu',
    description: 'IPC library for libemu servers and clients',
    version: '1.0.0',
    libraries: [libemu],
    install_dir: get_option('libdir') / 'pkgconfig',
)
