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

# curveslib = static_library (
# 	'curveslib',
# 	sources: [
#         'math.cpp',
#         'log.cpp',
#         'Curve.cpp',
#   			'ZeroCouponBond.cpp',
#   			'ForwardRateAgreement.cpp',
#   			'Swap.cpp',
#     ],
# 	include_directories : ['include'],
# 	dependencies : [python3, gsl, fmt, libsystemd, threads],
#     #include_directories : include_directories('include')),
# 	install : true,
# )

#
#
# py3 = import('python3')
#
# py3.extension_module (
# 	'curves',
# 	[
#     'python-binding.cpp',
#     'curves.cpp',
#     'Curve.cpp',
#     'ZeroCouponBond.cpp',
#     'ForwardRateAgreement.cpp',
#     'Swap.cpp',
#     '../math.cpp',
#     '../log.cpp',
#   ],
#   include_directories : [
#       '..',
#       '../include',
#       '/usr/include/python3.8',
#       '/home/zvyagin/Projects/Swaptions.git/lib/python3.8/site-packages/pybind11/include'
#   ],
#   link_with           : [],
#   dependencies : [fmt,gsl,libsystemd,threads],
# 	install             : true,
# 	install_dir         : get_option('prefix')+'/lib/python3.8/site-packages/',
# )

subdir('tests')
