
# Copyright Redshift Software, Inc. 2005-2007
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import buildbot
import buildbot.changes.changes
import buildbot.changes.mail
import buildbot.status.builder
import buildbot.status.html
import buildbot.util
import email.Utils
import os.path
import re
import rfc822
import string
import time
import types
import twisted.python
import twisted.python.components
import twisted.web.static
import urllib

waterfall_content_html = '''<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <title>BuildBot: %(project_name)s</title>
  <link href="buildbot.css" rel="stylesheet" type="text/css" />
</head>

<body>
%(heading)s
%(body)s
%(footer)s
</body>
</html>
'''

waterfall_body_html = '''
  <table id="waterfall">
    <tr id="builders">
      <td colspan="2" class="project">
        <a href="%(project_url)s">%(project_name)s</a>
      </td>
      
      %(builders)s
    </tr>

    <tr id="current-activity">
      <td colspan="2" class="heading">
        CURRENT&nbsp;ACTIVITY
      </td>
      
      %(current_activity)s
    </tr>

    <tr id="last-activity">
      <td class="heading">
        TIME %(timezone)+02.0f
      </td>

      <td class="heading">
        <a href="changes">CHANGES</a>
      </td>
      
      %(last_activity)s
    </tr>
    
    %(waterfall)s
  </table>
'''

waterfall_footer_html = '''
  <div id="footer">
    <p><a href="http://buildbot.sourceforge.net/">Buildbot</a>-%(version)s working
    for the <a href="%(project_url)s">%(project_name)s</a> project.</p>

    <p>Page built: %(page_time)s</p>
  </div>
'''

class Boost_WaterfallStatusResource(buildbot.status.html.WaterfallStatusResource):
    
    def __init__(self, status, changemaster, categories, css=None):
        buildbot.status.html.WaterfallStatusResource.__init__(self,status,changemaster,categories,css)
        
    def content(self, request):
        self.page_time = time.strftime("%a %d %b %Y %H:%M:%S",time.localtime(buildbot.util.now()))
        return waterfall_content_html % {
            "project_name"      : self.status.getProjectName(),
            "project_url"       : self.status.getProjectURL(),
            "page_time"         : self.page_time,
            "heading"           : self.heading(request),
            "body"              : self.body(request),
            "footer"            : self.footer(request) }

    def heading(self, request):
        return ""

    def body(self, request):
        "This method builds the main waterfall display."
        phase = request.args.get("phase",["2"])
        phase = int(phase[0])

        showBuilders = request.args.get("show", None)
        allBuilders = self.status.getBuilderNames(categories=self.categories)
        if showBuilders:
            builderNames = []
            for b in showBuilders:
                if b not in allBuilders:
                    continue
                if b in builderNames:
                    continue
                builderNames.append(b)
        else:
            builderNames = allBuilders
        builders = map(
            lambda name: self.status.getBuilder(name),
            builderNames)

        if phase == -1:
            return self.body0(request, builders)
        
        (changeNames, builderNames, timestamps, eventGrid, sourceEvents) = self.buildGrid(request, builders)
        if phase == 0:
            return self.phase0(request, changeNames, timestamps, eventGrid)
        
        last_activity_html = "";
        for b in builders:
            box = buildbot.status.html.ITopBox(b).getBox()
            last_activity_html += box.td()
        
        current_activity_html = "";
        for b in builders:
            box = buildbot.status.html.ICurrentBox(b).getBox()
            current_activity_html += box.td()
        
        builders_html = "";
        for name in builderNames:
            builders_html += "<td class=\"builder\"><a href=\"%s\">%s</a></td>" % (
                urllib.quote(name),
                string.join(string.split(name,'-'),'<br />') )
        
        if phase == 1:
            f = self.phase1
        else:
            f = self.phase2
        waterfall_html = f(request, changeNames+builderNames, timestamps, eventGrid, sourceEvents)
        
        return waterfall_body_html % {
            "project_name"      : self.status.getProjectName(),
            "project_url"       : self.status.getProjectURL(),
            "last_activity"     : last_activity_html,
            "current_activity"  : current_activity_html,
            "builders"          : builders_html,
            "waterfall"         : waterfall_html,
            "version"           : buildbot.version,
            "page_time"         : self.page_time,
            "timezone"          : time.timezone/60
            }

    def footer(self, request):
        return waterfall_footer_html % {
            "project_name"      : self.status.getProjectName(),
            "project_url"       : self.status.getProjectURL(),
            "version"           : buildbot.version,
            "page_time"         : self.page_time
            }

## Override some of the display elements to make them CSS friendly.

def td(text="", parms={}, **props):
    props.update(parms)
    
    td_props_html = "";
    for prop in ("colspan", "rowspan", "class", "style"):
        p = props.get(prop, None)
        if p != None:
            td_props_html += " %s=\"%s\"" % (prop, p)
    
    if type(text) == types.ListType:
        td_text_html = "<div>%s</div>" % string.join(text, "</div><div>")
    else:
        td_text_html = "<div>%s</div>" % text
    
    return "<td%s>%s</td>\n" % (td_props_html,td_text_html)

color_map = {
    '#c000c0' : 'purple'
    }
def c(a_color):
    if a_color == None:
        return 'none'
    elif color_map.has_key(a_color):
        return color_map[a_color]
    else:
        return a_color

class Boost_Box:

    spacer = False
    
    def __init__(self, other_box, props={}):
        self.text = other_box.text
        self.urlbase = other_box.urlbase
        self.show_idle = other_box.show_idle
        self.parms = other_box.parms
        self.parms.update(props)
    
    def td(self, **props):
        props.update(self.parms)
        text = self.text
        if not text and self.show_idle:
            text = ["[idle]"]
        return td(text, props)

class Boost_CurrentBox(buildbot.status.html.CurrentBox):

    def getBox(self):
        state, ETA, build = self.original.getState()
        return Boost_Box( buildbot.status.html.CurrentBox.getBox(self),
            { 'class': "activity-%s" % state } )

twisted.python.components.theAdapterRegistry.adapterRegistry[
    (buildbot.status.builder.BuilderStatus, buildbot.status.html.ICurrentBox)] = Boost_CurrentBox

class Boost_ChangeBox(buildbot.status.html.ChangeBox):

    def getBox(self):
        return Boost_Box( buildbot.status.html.ChangeBox.getBox(self),
            { 'class': "commit" } )

twisted.python.components.theAdapterRegistry.adapterRegistry[
    (buildbot.changes.changes.Change, buildbot.status.html.IBox)] = Boost_ChangeBox

class Boost_BuildBox(buildbot.status.html.BuildBox):

    def getBox(self):
        return Boost_Box( buildbot.status.html.BuildBox.getBox(self),
            { 'class': "build" } )

twisted.python.components.theAdapterRegistry.adapterRegistry[
    (buildbot.status.builder.BuildStatus, buildbot.status.html.IBox)] = Boost_BuildBox

class Boost_StepBox(buildbot.status.html.StepBox):

    def getBox(self):
        return Boost_Box( buildbot.status.html.StepBox.getBox(self),
            { 'class': "step-%s" % c(self.original.getColor()) } )

twisted.python.components.theAdapterRegistry.adapterRegistry[
    (buildbot.status.builder.BuildStepStatus, buildbot.status.html.IBox)] = Boost_StepBox

class Boost_EventBox(buildbot.status.html.EventBox):

    def getBox(self):
        return Boost_Box( buildbot.status.html.EventBox.getBox(self),
            { 'class': "event-%s" % c(self.original.getColor()) } )

twisted.python.components.theAdapterRegistry.adapterRegistry[
    (buildbot.status.builder.Event, buildbot.status.html.IBox)] = Boost_EventBox

class Boost_BuildTopBox(buildbot.status.html.BuildTopBox):

    def getBox(self):
        box = buildbot.status.html.BuildTopBox.getBox(self)
        return Boost_Box( box,
            { 'class': "build-%s" % c(box.color) } )

twisted.python.components.theAdapterRegistry.adapterRegistry[
    (buildbot.status.builder.BuilderStatus, buildbot.status.html.ITopBox)] = Boost_BuildTopBox

##

class Boost_StatusResource(buildbot.status.html.StatusResource):
    
    def __init__(self, status, control, changemaster, categories, root):
        buildbot.status.html.StatusResource.__init__(self,
            status, control, changemaster, categories,
            twisted.web.static.File(os.path.join(root,"buildbot.css")))
        self.putChild("",
            Boost_WaterfallStatusResource(self.status, self.changemaster,
                self.categories, self.css))
        self.putChild("buildbot.css",
            twisted.web.static.File(os.path.join(root,"buildbot.css")))

class Boost_Waterfall(buildbot.status.html.Waterfall):

    root = None

    def __init__(self, http_port=None, distrib_port=None, allowForce=True, root=None):
        buildbot.status.html.Waterfall.__init__(self,http_port,distrib_port,allowForce)
        self.root = root
    
    def setup(self):
        buildbot.status.html.Waterfall.setup(self)
        self.site.resource = Boost_StatusResource(
            self.site.resource.status,
            self.site.resource.control,
            self.site.resource.changemaster,
            self.site.resource.categories,
            self.root)

def Boost_parseSyncmail(self, fd, prefix=None, sep="/"):
    m = rfc822.Message(fd)
    
    # The mail is sent from the person doing the checkin. Assume that the
    # local username is enough to identify them (this assumes a one-server
    # cvs-over-rsh environment rather than the server-dirs-shared-over-NFS
    # model)
    name, addr = m.getaddr("from")
    if not addr:
        return None # no From means this message isn't from FreshCVS
    at = addr.find("@")
    if at == -1:
        who = addr # might still be useful
    else:
        who = addr[:at]
    
    # take the date of the email as the time of checkin, but fall back to
    # delivery time
    when = buildbot.util.now()
    email_time = m.getheader("date")
    if email_time:
        email_time = email.Utils.parsedate_tz(email_time)
        if email_time:
            when = email.Utils.mktime_tz(email_time)
    
    # syncmail puts the repository-relative directory in the subject:
    # "CVS: %(dir)s %(file)s,%(oldversion)s,%(newversion)s"
    # this is the only reasonable way to determine the directory name
    subject = m.getheader("subject")
    bits = subject.split(" ")
    while bits:
        bit = bits.pop(0)
        if bit == "CVS:":
            break;
    directory = bits.pop(0)
    
    files = []
    comments = ""
    isdir = 0
    lines = m.fp.readlines()
    while lines:
        line = lines.pop(0)
        if (line.find("Modified Files:") == 0 or
            line.find("Added Files:") == 0 or
            line.find("Removed Files:") == 0):
            break
    while lines:
        line = lines.pop(0)
        if line == "\n" or line == "\r" or line == "\r\n" or line == "\n\r":
            break
        if line.find("Log Message:") == 0:
            lines.insert(0, line)
            break
        if (line.find("Modified Files:") == 0 or
            line.find("Added Files:") == 0 or
            line.find("Removed Files:") == 0):
            continue
        line = line.lstrip()
        line = line.rstrip()
        # note: syncmail will send one email per directory involved in a
        # commit, with multiple files if they were in the same directory.
        # Unlike freshCVS, it makes no attempt to collect all related
        # commits into a single message.
        thesefiles = line.split(" ")
        for file in thesefiles:
            file = sep.join([directory, file])
            file = file.replace("\\",sep)
            file = file.replace("/",sep)
            if prefix:
                # insist that the file start with the prefix: we may get
                # changes we don't care about too
                bits = file.split(sep)
                if bits[0] == prefix:
                    file = sep.join(bits[1:])
                else:
                    break
            # TODO: figure out how new directories are described, set .isdir
            files.append(file)
    
    if not files:
        return None
    
    while lines:
        line = lines.pop(0)
        if line.find("Log Message:") == 0:
            break
    # message is terminated by "Index:..." (patch) or "--- NEW FILE.."
    # or "--- filename DELETED ---". Sigh.
    while lines:
        line = lines.pop(0)
        if line.find("Index: ") == 0:
            break
        if re.search(r"^--- NEW FILE", line):
            break
        if re.search(r" DELETED ---$", line):
            break
        comments += line
    comments = comments.rstrip() + "\n"
    
    change = buildbot.changes.changes.Change(who, files, comments, isdir, when=when)
    
    return change

class Boost_SyncmailMaildirSource(buildbot.changes.mail.SyncmailMaildirSource):
    parser = Boost_parseSyncmail
    def messageReceived(self, filename):
        twisted.python.log.msg("Boost_SyncmailMaildirSource.messageReceived: "+filename)
        buildbot.changes.mail.SyncmailMaildirSource.messageReceived(self,filename)
