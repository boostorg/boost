#
# Copyright (C) 2005 The Trustees of Indiana University 
# Author: Douglas Gregor
#
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#
import re
import smtplib
import os
import time
import string

got_issues = False
print 'Retrieving issues email...'

for x in range(30):
    # Update issues-email.txt
    os.system('curl -O http://engineering.meta-comm.com/boost-regression/CVS-HEAD/developer/issues-email.txt')

    # Determine the set of libraries that have unresolved failures
    failing_libraries = {}
    library_regex = re.compile('\|(.*)\|')
    email_text = ''

    for line in file('issues-email.txt', 'r'):
        if line.count('Report time') > 0:
          got_issues = True
        m = library_regex.match(line)
        if m:
           failing_libraries[m.group(1)] = ""
        email_text += line


    if got_issues:
      break;

    time.sleep (30) 

# Determine the maintainers of each library
maintainers = {}
lib_maintainer_regex = re.compile('(\S+)\s*(.*)')
name_email_regex = re.compile('\s*(\w*(\s*\w+)+)\s*<\s*(\S*(\s*\S+)+)\S*>')
at_regex = re.compile('\s*-\s*at\s*-\s*')
for line in file('../../../libs/maintainers.txt', 'r'):
    m = lib_maintainer_regex.match (line)
    if m:
       lib = m.group(1)
       maintainers[lib] = list()
       for person in re.split('\s*,\s*', m.group(2)):
           nmm = name_email_regex.match(person)
           if nmm:
              name = nmm.group(1)
              email = nmm.group(3)
              email = at_regex.sub('@', email)
              maintainers[lib].append((name, email))

# Compute the set of maintainers that should receive notification        
cc_list = {}
for lib in failing_libraries:
    for maintainer in maintainers[lib]:
           cc_list[maintainer] = {}

cc_line = ''
for maintainer in cc_list:
    if cc_line != '':
      cc_line += ', '
    cc_line += maintainer[0] + ' <' + maintainer[1] + '>'

to_line = 'boost@lists.boost.org'
from_line = 'Douglas Gregor <dgregor@cs.indiana.edu>'

message = """From: Douglas Gregor <dgregor@cs.indiana.edu>
"""

message += "CC: "
message += cc_line

message += """
Subject: Boost regression notification

"""

message += email_text

print 'Sending notification email...'
print 'Recipients: ', cc_line

# Send the e-mail out
smtp = smtplib.SMTP('milliways.osl.iu.edu')
smtp.sendmail(from_addr = from_line, to_addrs = to_line, msg = message)

print 'Done.'
