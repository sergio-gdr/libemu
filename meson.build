project(
	'emu',
	'c',
	version: '1.0-dev',
	license: 'MIT',
	meson_version: '>=1.3',
	default_options: [
		'c_std=c23',
		'warning_level=2',
		'wrap_mode=nodownload',
	],
)

add_project_arguments(
	[
		'-std=c23',
		'-Wno-unused-parameter',
		'-Wno-unused-result',
		'-Wno-missing-braces',
		'-Wno-format-zero-length',
		'-Wundef',
		'-Wvla',
	],
	language: 'c',
)

subdir('lib')
