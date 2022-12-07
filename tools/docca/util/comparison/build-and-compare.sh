# This script runs a build and compares the scrubbed/normalized
# results against the scrubbed results of a previous build (run using
# update-baseline-html.sh). It is primarily used to guard against
# XSLT regressions in docca.

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

! test -d htmlPREVIOUS_SCRUBBED && echo "First run update-baseline-html.sh" && exit

rm -rf html_SCRUBBED

echo "Running clean build..."
b2 clean
b2

echo "Scrubbing results and storing as html_SCRUBBED..."
cp -r html html_SCRUBBED
cd html_SCRUBBED
grep -rl -f ${SCRIPT_DIR}/grep-expressions . | xargs sed -i -f ${SCRIPT_DIR}/sed-commands

echo "Showing diffs from previous build (stored in diffs.txt):"
cd ..
diff -r htmlPREVIOUS_SCRUBBED html_SCRUBBED >diffs.txt
cat diffs.txt
