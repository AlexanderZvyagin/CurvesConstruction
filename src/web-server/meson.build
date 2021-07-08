#spdlog     = dependency('spdlog')
fmt        = dependency('fmt',          required:false)
libsystemd = dependency('libsystemd',   required:false)
threads    = dependency('threads',      required:false)
lws        = dependency('libwebsockets',required:false)

if fmt.found() and libsystemd.found() and threads.found() and lws.found()
    ws = executable (
        'ws',
    	sources: [
    		'../core/log.cpp',
        # 'helper.cpp',
    		'protocol.cpp',
    		'UserData.cpp',
    		# 'UserData_Request1.cpp',
    		'server.cpp'
    	],
      include_directories : [
          '../core',
          '../include',
          '../mongodb',
          '/usr/local/include/bsoncxx/v_noabi',
          '/usr/local/include/mongocxx/v_noabi',
      ],
      link_with           : [curveslib,curvesdb],
    	dependencies : [fmt, libsystemd, threads, lws, gsl],
      install : true,
    )
endif
