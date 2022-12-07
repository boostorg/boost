# Docca comparison scripts

The scripts in this directory help guard against docca regressions by
comparing a previous doc build's output with the latest output
(after normalizing/scrubbing the results to eliminate non-substantive
changes).

Probably the easiest thing to do is add this directory to your PATH
(or create an alias to the two scripts herein).

The normal sequence goes like this:

1. Run a baseline build from inside your project's doc directory by invoking
   *update-baseline-html.sh*
2. Implement new feature or bug fix in docca (usually by updating XSLT)
3. Run a comparison build from inside your project's doc directory by invoking
   *build-and-compare.sh*
4. Review the output (in diffs.txt) to make sure everything looks good and nothing broke
5. Otherwise, fix the regression and try again!

This utility can also be useful for examining the impacts of other changes
(such as a Doxygen upgrade!).

## Implementation details

The file named *grep-expressions* is used to filter out all but the matching files
that need scrubbing. The resulting files are passed to sed and updated *in situ* based
on the commands in the file named *sed-commands*.
