# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
#Copyright (c) 2014-2015 Regents of the University of California.
#@author Lijing Wang wanglj11@mails.tsinghua.edu.cn

VERSION = '0.2'
APPNAME = 'NDNlive'

from waflib import Build, Logs, Utils, Task, TaskGen, Configure

def options(opt):
    opt.load('compiler_c compiler_cxx gnu_dirs')
    opt.load('boost default-compiler-flags doxygen', tooldir=['.waf-tools'])

    ropt = opt.add_option_group('ndnlive Options')

def configure(conf):
    conf.load("compiler_c compiler_cxx gnu_dirs boost default-compiler-flags")

    conf.check_cfg(package='libndn-cxx', args=['--cflags', '--libs'],
                   uselib_store='NDN_CXX', mandatory=True)
		
    conf.check_cfg(package='gstreamer-1.0', args=['--cflags', '--libs'], 
         uselib_store='GSTREAMER', mandatory=True) 

    conf.env.LIB_PTHREAD = 'pthread'

    USED_BOOST_LIBS = ['system', 'iostreams', 'filesystem', 'random']
    conf.check_boost(lib=USED_BOOST_LIBS, mandatory=True)
#   ['system', 'filesystem', 'date_time', 'iostreams',
#                      'regex', 'program_options', 'chrono', 'random']

    try:
        conf.load("doxygen")
    except:
        pass

    conf.define('DEFAULT_CONFIG_FILE', '%s/ndn/next-ndnvideo.conf' % conf.env['SYSCONFDIR'])

    conf.write_config_header('src/config.hpp')

def build(bld):


    bld(target="producer",
        features=["cxx", "cxxprogram"],
        source= "src/producer.cpp src/video-generator.cpp src/producer-callback.cpp",
        use='GSTREAMER BOOST NDN_CXX PTHREAD',
        )

    bld(target="consumer",
        features=["cxx", "cxxprogram"],
        source= "src/consumer.cpp src/video-player.cpp src/consumer-callback.cpp src/video-frame.cpp",
        use='GSTREAMER BOOST NDN_CXX PTHREAD',
        )
      
    bld.install_files('${SYSCONFDIR}/ndn', 'next-ndnvideo.conf.sample')
