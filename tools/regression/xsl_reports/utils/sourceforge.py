
import utils.checked_system

site_dir = '/home/groups/b/bo/boost/htdocs'

def download( source, destination, user ):
    utils.checked_system( [ 
          'rsync -v -r -z --progress %(user)s@shell.sourceforge.net:%(site_dir)s/%(source)s %(dest)s'
                % { 'user': user, 'site_dir': site_dir, 'source': source, 'dest': destination }
        ] )

def upload( source, destination, user ):
    utils.checked_system( [ 
          'rsync -v -r -z --progress %(source)s %(user)s@shell.sourceforge.net:%(site_dir)s/%(dest)s'
                % { 'user': user, 'site_dir': site_dir, 'source': source, 'dest': destination }
        ] )
