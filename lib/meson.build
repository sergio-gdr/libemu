sources = files(
	'libemu.c',
	'ipc.c',
	'list.c',
)

headers = files(
	'include/libemu_ipc.h',
	'include/libemu_cpu.h',
	'include/libemu_ppu.h',
	'include/libemu_list.h',
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
