#!/bin/bash

# CHECKS THAT ALL XMLs THAT ARE TO BE COMMITED ARE PARSEABLE

REPOS="$1"
TXN="$2"

OPTS="-t $TXN"

SVNLOOK=/usr/bin/svnlook
type=
for token in `$SVNLOOK changed $OPTS $REPOS`
do
  if [ -z "$type" ]
      then
      type=$token
      else
      # Only checking a file if it wasn't deleted and is an .xml file
      if [ "$type" != "D" -a -n "`echo $token | grep .*\.xml`" ]
          then
          $SVNLOOK cat $OPTS $REPOS $token | /usr/bin/xmllint -noout - || {
              echo "Error parsing XML; aborting commit." >&2
              exit 1
              }
          fi

      # Resetting type to get the type for the next file
      type=
  fi      
done

exit 0