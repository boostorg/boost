Boost 1.32 Release Progress
***************************
    
These pages serve as an information board/navigation helper for
tracking the release progress. They are regenerated automatically twice 
a day at 12:00 p.m. and 12:00 a.m. US Central Time from its `source 
document in the CVS`__. 

**To library authors/maintainers**: please update the correspoding sections of 
the TODO list as you make progress on them.


.. contents:: Table of Contents

__ http://cvs.sourceforge.net/viewcvs.py/*checkout*/boost/boost/status/1_32_release_progress.rst 

Release Schedule
++++++++++++++++

June 24-27: preparation (passed)
////////////////////////////////

Announce a preliminary release schedule, collect information about 
everyone's plans for the release, adjust/confirm the schedule, 
make regression reports to reflect the actual status of the main 
trunk, setup nighly runs of the inspection report 
(http://tinyurl.com/2vo5p), construct a "Release Progress" page 
publishing the milestones, the collective TODO list, and links to 
all the reports. 

June 27-July 16: problem fixing 
///////////////////////////////

Use regression reports to identify and resolve the outstanding 
issues with the main trunk. Walk through and deal with all the 
patches and bug reports submitted to the list/SourceForge tracker 
since the last release. In accordance with the TODO list, track 
the progress of individual developers. By July 15, 24:00 US 
Central Time every line of code that is intended to be put into 
this release should be in the CVS, and the next round of 
regression reports should show no regressions against the 
then-current expected failures markup. 

July 16: branch for release
///////////////////////////

Branch for release. Switch regression tests to run on the branch. 

July 17-18: code freeze
///////////////////////

No code checkins are allowed expect to fix the failures resulted 
from branching (hopefully none). Documentation checkins are still 
OK. Perform the rest of the pre-release tasks listed in 
http://tinyurl.com/2meuh. July 18, 24:00 US Central Time marks a 
complete freeze of the branch. 

July 19-20: release candidate
/////////////////////////////

Prepare and test the release candidate archives. Fix any found 
issues and repeat. 

July 21: release
////////////////

Release according to the http://tinyurl.com/2meuh procedure. 


Outstanding Issues
++++++++++++++++++

Regression Failures
///////////////////

Current unresolved issues:

- Win32__ 
- Linux__
    
__ http://boost.sourceforge.net/regression-logs/cs-win32_metacomm/issues.html
__ http://boost.sourceforge.net/regression-logs/cs-Linux/developer_summary_page.html

Licensing
/////////

Graph Library
-------------

Walter Landry writes:

Since it has been awhile, has there been any progress on the licensing
issues for the graph library?  The last thing I heard on this list is
that it was being worked on.  Is that still true?



TODO list
+++++++++

Circular Buffer
///////////////

Pavel Vozenilek writes:

I asked Jan Galdun and he will try to make circular_buffer available
for 1.32.


Date-Time
/////////

Jeff Garland writes:

And while I'm on the subject date-time plans for release, the only 'major
issue' for this release is whether we are switching to boost-book for this
release.  We would like to, but if we don't make it we will do it for the next
release.  One thing that would help a bit (although I think we've mostly
worked around it) is if you could reply to this question on the boost-doc list ;-)

http://sourceforge.net/mailarchive/forum.php?thread_id=4960807&forum_id=13340


Iterators
/////////

David Abrahams writes:
 
There are a couple of things that ideally I would like to handle, but
which I am not promising (yet):

1. Minor adjustments to code in the iterators library
2. Iterators library doc fixes


MPL
///

New version of the library will be checked in the main trunk before July 11.


Multi-Index
///////////

Joaquín Mª López Muñoz writes:

Current status: commited in the CVS since 6 weeks or
so. Regression tests are OK for all the compilers that
I intend to support. If someone is interested in some
other compiler being supported, please contact me.

Pending issues:
#1 Minor updates to the docs. No big deal.
#2 Some changes related to the new defect macro
BOOST_NO_POINTER_TO_MEMBER_TEMPLATE_PARAMETERS

These issues will be resolved way before the branching
date, I hope.


Numeric Conversions
///////////////////

Fernando Cacciola writes:

In preparation for the upcoming 1.32 release I added this library to CVS.
The code and docs have been changed according to the extensive review
results.
I still need to extend the documentation adding examples and motivating
cases though, which I plan to do before the release.

I haven't yet changed "boost/cast.hpp" to use the new stuff, I will as soon
as I see the code stablilized among different compilers.

In the meantime, new users can use "boost/numeric/conversion/cast.hpp" which
contains the new version of numeric_cast<>


Program Options
///////////////

Vladimir Prus writes:

1. Add markup for the current failures on Windows. I've done all workarounds I 
   planned.
2. Finally get some results on gcc2.95-stlport. I expect that the library 
   should work there, but due to Boost.Test failure I don't have any results.
3. Try to make Sun compiler happy. I'm comitted a workaround some time ago, 
   but tests on Solaris were not run since then.
4. Check dynamic linking on Windows.


Serialization
/////////////

Robert Ramey writes:

I would very much like to see the serialization library in the 1.32 Release.
However, I'm not sure it will be possible to make the posted schedule.

I have made all changes to address issues raised in the review.  On my
personal machine I have passed tests for the compilers VC 7.1 VC 6, gcc 3.3.
Borland compiles (almost everything but requires some debug - it will pass
most tests)  Comeau- compiles everthing - accept for a show stopping problem
I've been unable to resolve.  My intention has been:

a) post version compatible with 1.31 next week.

b) build and test against the CVS tree on my computer

c) check in

In order to do this I need:

a) authority to check in 

b) information on how to mark tests/compilers known to fail.

c) I have a couple of questions that I need an expert opinion on.

   i) the first one is that in order to work, versions for vc 6.0 and
      borland need to be compiled with spirit 1.61.  So the a special
      environmental variable indicating the directory path where this version of
      spirit is located on the testing machine.  SPIRIT_ROOT.  This variable has
      to be set when serialization libraries are built for these compilers.

I should say I'm more than a little bit concerned about changes to mpl and
iterators.  I'm very much aware of the delays and difficulties associated
with the 1.31 release - and it didn't even include this package.

So I would like to continue on the path I've established above and we'll see
where things stand July 16 when/if you are ready to branch for release.


String Algorithms
/////////////////

Pavol Droba writes:

I have few outstanding issues to fix, but they are mostly related to 
the documentation. I think, I can manage it.

As it seems, that the Range library will not make it to this release,
I will use the internal versions of facilities that are currently in the string_algo lib.

So my todo list is as follows:

1) Finish the documentation
    - Add functions list
    - Add exception specifications
    - Cleanup

2) Fix examples
3) If there will be time, improve find_iterator 
4) Fix possible regressions.


Questions:
 - I have posted some requestes/bugs on the boost-doc list. What is the status
   of this. Will they be fixed before release?

 - According to metacom's tables regex is failing on comeau a so string_algo regex
   test. Is this a setup or regex problem?

 - There is an problem with Intel compiler, split tests are failing and I cannot figureout
   why. The message is very vague. Could somebody with an access to this platform,
   try to debug the cause of the problem?

 - In regards to the older compiler support. I cannot resonably support compilers, that I
   don't have an access to. Therefor if somebody is interested to use string_algo library
   with one of such compilers, I will need an active help with porting.


uBLAS
/////

Michael Stevens writes:

The current status of uBLAS is that it has been relatively untouched in the 
Boost CVS single 1.30.0. The active development takes place on a separate 
CVS.

Therefore now would be a good time to bring in the latest code which as some 
quite significant improvements. The code is relatively stable and tested on 
core compilers.

Issues:
On exotic (old) compiler the code is relatively untested and expected to fail. 
Therefore the real need with regard to 1.32 is to get feedback from extensive 
regression testing results in good time.

As ever the documentation could do with further work to bring it up to date 
with the to be release version.
