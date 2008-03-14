
# Copyright Redshift Software, Inc. 2005-2007
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import os
import os.path
import sys
import getopt
import re
import boost


def show_revision( **unused ):
    re_keyword_value = re.compile( r'^\$\w+:\s+(.*)\s+\$$' )
    print '\n\tResivion: %s' % re_keyword_value.match( boost.buildbot.revision ).group( 1 )
    print '\tLast modified on: %s\n' % re_keyword_value.match( boost.buildbot.modified ).group( 1 )
    sys.exit(0)


def create_tester( root, server, runner, passwd, debug_level, **unused ):
    import twisted.scripts.mktap

    root = os.path.abspath(root)
    if os.path.exists(root):
        print "Testing root location %s exists." % root
        print "Skipping to prevent corruption of existing setup."
        sys.exit(1)
    if not os.path.exists(root):
        if debug_level > 0: print "mkdir", root
        os.mkdir(root)
    if debug_level > 0: print "chdir", root
    os.chdir(root)
    sys.argv = [
        'mktap', 'buildbot', 'slave',
        '--basedir', root,
        '--master', server,
        '--name', runner,
        '--passwd', passwd
        ]
    if debug_level > 0: print ' '.join( sys.argv )
    twisted.scripts.mktap.run()
    if debug_level > 0: print "Tester configured in %s." % root
    sys.exit(0)

def create_server( root, debug_level, **unused ):
    import twisted.scripts.mktap

    root = os.path.abspath(root)
    if os.path.exists(root):
        print "Testing root location %s exists." % root
        print "Skipping to prevent corruption of existing setup."
        sys.exit(1)
    if not os.path.exists(root):
        if debug_level > 0: print "mkdir", root
        os.mkdir(root)
    if debug_level > 0: print "chdir", root
    os.chdir(root)
    sys.argv = [
        'mktap', 'buildbot', 'master',
        '--basedir', root
        ]
    if debug_level > 0: print ' '.join( sys.argv )
    twisted.scripts.mktap.run()
    if debug_level > 0: print "Server configured in %s." % root
    sys.exit(0)

def start_daemon( root, debug_level, **unused ):
    import twisted.python.runtime
    
    # import the various built in slave commands so that we can add our own
    import buildbot.slave.registry
    import buildbot.slave.commands
    import boost.buildbot.remote
    
    root = os.path.abspath(root)
    if debug_level > 0: print "chdir", root
    os.chdir(root)
    sys.argv = [
        'twistd',
        '--no_save',
        '--file=buildbot.tap'
        ]
    if sys.platform == "win32":
        sys.arvg.append("--reactor=win32")
    if debug_level > 0: print ' '.join( sys.argv )
    if twisted.python.runtime.platformType == "Win32":
        import twisted.scripts.twistw
        twisted.scripts.twistw.run()
    else:
        import twisted.scripts.twistd
        twisted.scripts.twistd.run()
    sys.exit(0)

def stop_daemon( root, debug_level, **unused ):
    import signal
    
    twistd_pid_file = os.path.join(root,'twistd.pid')
    if os.path.isfile(twistd_pid_file):
        twistd_pid = file(twistd_pid_file,'r').read()
        os.kill(int(re.search(r'^(\d+)',twistd_pid).group(1)),signal.SIGTERM);
        sys.exit(0)
    else:
        sys.exit(1)

def accept_args( args ):
    args_spec = [
        'root=',
        'server=',
        'runner=',
        'passwd=',
        ##
        'debug-level=',
        'help'
        ]
    
    options = {
        '--root'            : None,
        '--server'          : None,
        '--runner'          : None,
        '--passwd'          : None,
        ##
        '--debug-level'     : 0
        }
    
    ( option_pairs, other_args ) = getopt.getopt( args, '', args_spec )
    map( lambda x: options.__setitem__( x[0], x[1] ), option_pairs )

    if options.has_key( '--help' ):
        usage()
        sys.exit( 1 )

    return {
        'root'              : options[ '--root' ],
        'server'            : options[ '--server' ],
        'runner'            : options[ '--runner' ],
        'passwd'            : options[ '--passwd' ],
        ##
        'debug_level'       : int(options[ '--debug-level' ]),
        'args'              : other_args
        }


commands = {
    'show-revision'         : show_revision,
    'create-tester'         : create_tester,
    'create-server'         : create_server,
    'start'                 : start_daemon,
    'stop'                  : stop_daemon
    }

def lp( l ):
    print l;

def usage():
    lp('Usage:')
    lp('')
    lp('python %s [command] options' % os.path.basename( sys.argv[0] ))
    lp('')
    lp('Commands:')
    lp('')
    lp('\n'.join( commands.keys() ))
    lp('')
    lp('Options:')
    lp('')
    lp('--root          Directory of server or runner.')
    lp('--server        The server address for the runner to connect to')
    lp('                in the for of DNSNAME:PORT.')
    lp('--runner        The name of the runner.')
    lp('--passwd        The password for the runner to connect ro the server.')
    lp('--debug-level   Debugging level; controls the amount of debugging')
    lp('                output printed; 0 by default (no debug output).')
    lp('')

def run():
    if len(sys.argv) > 1 and sys.argv[1] in commands:
        command = sys.argv[1]
        args = sys.argv[ 2: ]
    else:
        command = 'show-revision'
        args = sys.argv[ 1: ]
        
    commands[ command ]( **accept_args( args ) )
