
import httplib

def http_get( site, source, destination ):
    h = httplib.HTTPConnection( site )
    h.request( 'GET', source )
    
    response = h.getresponse()
    f = open( destination, 'wb' )
    while True:
        data = response.read( 16*1024 )
        if len( data ) == 0: break
        f.write( data )

    f.close()
