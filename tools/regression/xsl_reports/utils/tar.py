
import utils.checked_system

def tar( source_dir, archive_name ):
    utils.checked_system( [
          'cd %s' % source_dir
        , 'tar -c -f ../%s -z *' % archive_name
        ] )
