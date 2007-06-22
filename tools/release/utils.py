import string

import os

class failure_exception:
    def __init__( self, rc ):
        self.rc_ = rc

    def __str__( self ):
        return "rc: %d" % self.rc_

def system( commands ):
    if os.path.exists( "tmp.cmd" ):
        os.chmod( "tmp.cmd", 0777 )
        os.unlink( "tmp.cmd" )

    f = open( "tmp.cmd", "w" )
    f.write( string.join( commands, "\n" ) )
    f.close()
    rc = os.system( "tmp.cmd" )
    os.chmod( "tmp.cmd", 0777 )
    os.unlink( "tmp.cmd" )
    return rc

def checked_system( commands, valid_return_codes = [ 0 ] ):
    rc = system( commands ) 
    if rc not in [ 0 ] + valid_return_codes: raise failure_exception( rc )
    return rc

class step_controller:
    def __init__( self, start_step ):
        self.current_step_ = None;
        self.skip_to_step_ = start_step

    def start_step( self, step_name, start_message ):
        self.current_step_ = step_name
        if self.is_skipping( step_name ):
            print "[%s] Skipping." % step_name
            return 0
        else:
            self.skip_to_step_ = ""
            print "[%s] %s" % ( step_name, start_message )
            return 1

    def finish_step( self, step_name ):
        print "[%s] Finished" % step_name

    def is_skipping( self, step_name = None ):
        if step_name is None: step_name = self.current_step_
        return self.skip_to_step_ != "" and self.skip_to_step_ != step_name

    
