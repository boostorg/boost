cd ~/boost/tools/docca/example
build-and-compare.sh 2>&1 &

cd ~/boost/libs/static_string/doc
build-and-compare.sh 2>&1 &

cd ~/boost/libs/url/doc
build-and-compare.sh 2>&1 &

cd ~/boost/libs/json/doc
build-and-compare.sh 2>&1 &

cd ~/boost/libs/beast/doc
build-and-compare.sh 2>&1 &

wait

echo "~/boost/tools/docca/example/diffs.txt:"
cat ~/boost/tools/docca/example/diffs.txt
echo "~/boost/libs/static_string/doc/diffs.txt:"
cat ~/boost/libs/static_string/doc/diffs.txt
echo "~/boost/libs/url/doc/diffs.txt:"
cat ~/boost/libs/url/doc/diffs.txt
echo "~/boost/libs/json/doc/diffs.txt:"
cat ~/boost/libs/json/doc/diffs.txt
echo "~/boost/libs/beast/doc/diffs.txt:"
cat ~/boost/libs/beast/doc/diffs.txt
