project('bzstruct', 'c',
  version : '0.1')
  

installdir=meson.build_root() + '/install'

incdir = include_directories('thirdparty')
srcs = [    'bzstruct.c',
           'bzstruct_cjson.c',
           'demo.c']


cflags = ['-Wno-all']
if get_option('build_demo')
	if get_option('build_cjson')		   
		subdir('thirdparty')
	endif
	cflags += ['-Ddemo_main=main']
	exe = executable('demo',
          include_directories:incdir,
		  c_args:cflags,
		  sources: srcs)

	test('demo', exe)
else
	lib = static_library('bzstruct', sources:srcs,include_directories:incdir, c_args:cflags, install:true, install_dir:installdir)
endif



