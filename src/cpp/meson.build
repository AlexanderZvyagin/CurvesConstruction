# meson build
# cd build
# meson configure -Dprefix=$VIRTUAL_ENV -Doptimization=3 -Dcpp_std=c++2a -Ddebug=false

project (
	'Curves Construction',
	'cpp',
)

gsl        = dependency('gsl')
python3    = dependency('python3')

curveslib = static_library (
	'curveslib',
	sources: [
        'math.cpp',
        # 'log.cpp',
        'YieldCurve.cpp',
  			'ZeroCouponBond.cpp',
  			'ForwardRateAgreement.cpp',
  			'Swap.cpp',
    ],
	include_directories : ['include'],
	dependencies : [gsl],
    #include_directories : include_directories('include')),
	install : true,
)

subdir('tests')
subdir('python')
