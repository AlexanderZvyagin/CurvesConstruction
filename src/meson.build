# meson build
# cd build
# meson configure -Dprefix=$VIRTUAL_ENV -Doptimization=3 -Dcpp_std=c++17 -Ddebug=false

project (
	'Curves Construction',
	'cpp',
)

gsl        = dependency('gsl',  			required:true	)
python3    = dependency('python3',		required:false)
threads    = dependency('threads',		required:false)

subdir('core')
subdir('python')
subdir('mongodb')
subdir('web-server')
