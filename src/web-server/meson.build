# meson build
# cd build
# meson configure -Doptimization=3 -Dcpp_std=c++2a -Ddebug=false
## meson configure -Dprefix=$PWD/../../.. -Doptimization=3 -Dcpp_std=c++2a -Ddebug=false

#project (
#	'ws',
#	'cpp'
#)


#spdlog     = dependency('spdlog')
fmt        = dependency('fmt',          required:false)
libsystemd = dependency('libsystemd',   required:false)
threads    = dependency('threads',      required:false)
lws        = dependency('libwebsockets',required:false)



#mclib = static_library (
#	'mclib',
#	sources: [
#        'log.cpp','mc.cu','cu.cpp'
#    ],
#    dependencies : [python3, fmt, libsystemd],
#    #dependencies : [python3, gsl, fmt, libsystemd, threads],
#    #include_directories : include_directories('include')),
#    install : true,
#)

#cu1 = executable('cu1', 'cu1.cu')

if fmt.found() and libsystemd.found() and threads.found() and lws.found()
    ws = executable (
        'ws',
    	sources: [
    		'../log.cpp',
        # 'helper.cpp',
    		'protocol.cpp',
    		'UserData.cpp',
    		# 'UserData_Request1.cpp',
    		'server.cpp'
    	],
      include_directories : [
          '..',
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
