# Minimize the dependencies for the core library.
curveslib = static_library (
	'curveslib',
	sources: [
        'math.cpp',
        'Curve.cpp',
  			'ZeroCouponBond.cpp',
  			'ForwardRateAgreement.cpp',
  			'Swap.cpp',
    ],
	include_directories : ['../include'],
	dependencies : [gsl],
	# install : true,
)

subdir('tests')
