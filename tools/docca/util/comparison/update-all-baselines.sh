cd ~/boost/tools/docca/example
update-baseline-html.sh 2>&1 &

cd ~/boost/libs/static_string/doc
update-baseline-html.sh >build_output.txt 2>&1 &

cd ~/boost/libs/url/doc
update-baseline-html.sh >build_output.txt 2>&1 &

cd ~/boost/libs/json/doc
update-baseline-html.sh >build_output.txt 2>&1 &

cd ~/boost/libs/beast/doc
update-baseline-html.sh >build_output.txt 2>&1 &

wait
