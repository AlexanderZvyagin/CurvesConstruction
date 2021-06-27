interpolation = executable (
    'interpolation',
    sources : ['interpolation.cpp'],
    include_directories : ['..','../include'],
    link_with           : [],
    dependencies : [gsl],
)
test('test interpolation', interpolation)

minimization = executable (
    'minimization',
    sources : ['minimization.cpp'],
    include_directories : ['..','../include'],
    link_with           : [],
    dependencies : [gsl],
)
test('test minimization', minimization)

# swap = executable (
#     'swap',
#     sources : ['swap.cpp'],
#     include_directories : ['..','../include'],
#     link_with           : [],
#     dependencies : [fmt,gsl,libsystemd,threads],
# )
# test('test swap', swap)

curves = executable (
    'curves',
    sources : ['curves.cpp'],
    include_directories : ['..','../include'],
    link_with           : [curveslib],
    dependencies : [gsl],
)
test('test curves', curves)

gsl_init = executable (
    'gsl_init',
    sources : ['gsl_init.cpp'],
    include_directories : ['..','../include'],
    link_with           : [],
    dependencies : [gsl],
)
test('test gsl_init', gsl_init)
