# This script runs a new build and moves the results so they
# can be used as a comparison baseline against subsequent builds
# that are run using build-and-compare.sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

rm -rf htmlPREVIOUS htmlPREVIOUS_SCRUBBED

echo "Running clean build..."
b2 clean
b2

echo "Moving results to htmlPREVIOUS..."
mv html htmlPREVIOUS

echo "Scrubbing htmlPREVIOUS and storing as htmlPREVIOUS_SCRUBBED for use by build-and-compare.sh..."
cp -r htmlPREVIOUS htmlPREVIOUS_SCRUBBED
cd htmlPREVIOUS_SCRUBBED
grep -rl -f ${SCRIPT_DIR}/grep-expressions . | xargs sed -i -f ${SCRIPT_DIR}/sed-commands
