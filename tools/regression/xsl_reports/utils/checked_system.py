
import os
import string

class failure_exception:
    def __init__( self, rc ):
        self.rc_ = rc

def system( commands ):
    f = open( 'tmp.cmd', 'w' )
    f.write( string.join( commands, '\n' ) )
    f.close()
    rc = os.system( 'tmp.cmd' )
    return rc

def checked_system( commands ):
    rc = system( commands ) 
    if 0 != rc : raise failure_exception( rc )
    return rc
