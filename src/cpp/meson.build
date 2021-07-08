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

# Minimize the dependencies for the core library.
curveslib = static_library (
	'curveslib',
	sources: [
        'math.cpp',
        'YieldCurve.cpp',
  			'ZeroCouponBond.cpp',
  			'ForwardRateAgreement.cpp',
  			'Swap.cpp',
    ],
	include_directories : ['include'],
	dependencies : [gsl],
	install : true,
)

subdir('tests')

if python3.found()
		subdir('python')
endif

subdir('mongodb')
subdir('web-server')
