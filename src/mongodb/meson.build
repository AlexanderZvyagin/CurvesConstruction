threads = dependency('threads')
libmongocxx = dependency('libmongocxx',required:false)
libsystemd = dependency('libsystemd',required:false)
fmt = dependency('fmt',required:false)
date = dependency('date',required:false)

if libmongocxx.found() and libsystemd.found() and fmt.found() and date.found()

    include_directories = [
        '../core',
        '../include',
        '/usr/local/include/bsoncxx/v_noabi',
        '/usr/local/include/mongocxx/v_noabi',
    ]

    curvesdb = static_library (
        'curvesdb',
        sources : ['DB.cpp','utils.cpp','../core/log.cpp'],
        include_directories : include_directories,
        link_with           : [],
        dependencies : [libmongocxx,threads,libsystemd,fmt,date],
    )

    mongodb_test = executable (
        'mongodb_test',
        sources : ['mongodb_test.cpp'],
        include_directories : include_directories,
        link_with           : [curvesdb],
        dependencies : [],
    )
    test('mongodb_test', mongodb_test)
endif
