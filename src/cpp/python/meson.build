py3 = import('python3')

py3.extension_module (
	'curves',
	[
    'binding.cpp',
  ],
  include_directories : [
      '..',
      '../include',
      '/usr/include/python3.8',
#      '/home/zvyagin/Projects/Swaptions.git/lib/python3.8/site-packages/pybind11/include'
  ],
  link_with           : [curveslib],
  dependencies        : [],
	install             : true,
	install_dir         : get_option('prefix')+'/lib/python3.8/site-packages/',
)
