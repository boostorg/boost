
import utils.checked_system
import os.path

def rmtree( path ):
    if os.path.exists( path ):
        utils.checked_system( [ "rm -f -r \"%s\"" % path ] )
