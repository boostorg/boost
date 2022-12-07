"""

	Copyright 2018-2022 Emil Dotchevski and Reverge Studios, Inc.

	Distributed under the Boost Software License, Version 1.0. (See accompanying
	file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

	This program generates a single header file from a file including multiple C/C++ headers.

	Usage:

		python3 generate_single_header.py  --help

		e.g. python3 generate_single_header.py -i include/boost/qvm/all.hpp -p include -o test/qvm.hpp boost/qvm

"""

import argparse
import os
import re
from datetime import date
import subprocess

included = {}
total_line_count = 10

def append(input_file_name, input_file, output_file, regex_includes, include_folder):
	global total_line_count
	line_count = 1
	for line in input_file:
		line_count += 1
		result = regex_includes.search(line)
		if result:
			next_input_file_name = result.group("include")
			if next_input_file_name in included:
				output_file.write("// Expanded at line %d: %s" % (included[next_input_file_name], line))
				total_line_count += 1
			else:
				included[next_input_file_name] = total_line_count
				print("%s (%d)" % (next_input_file_name, total_line_count))
				with open(os.path.join(include_folder, next_input_file_name), "r") as next_input_file:
					output_file.write('// >>> %s#line 1 "%s"\n' % (line, next_input_file_name))
					total_line_count += 2
					append(next_input_file_name, next_input_file, output_file, regex_includes, include_folder)
					output_file.write('// <<< %s#line %d "%s"\n' % (line, line_count, input_file_name))
					total_line_count += 2
		else:
			output_file.write(line)
			total_line_count += 1

def _main():
	parser = argparse.ArgumentParser(
		description="Generates a single include file from a file including multiple C/C++ headers")
	parser.add_argument("-i", "--input", action="store", type=str, default="in.cpp",
		help="Input file including the headers to be merged")
	parser.add_argument("-o", "--output", action="store", type=str, default="out.cpp",
		help="Output file. NOTE: It will be overwritten!")
	parser.add_argument("-p", "--path", action="store", type=str, default=".",
		help="Include path")
	parser.add_argument("--hash", action="store", type=str,
		help="The git hash to print in the output file, e.g. the output of \"git rev-parse HEAD\"")
	parser.add_argument("prefix", action="store", type=str,
		help="Non-empty include file prefix (e.g. a/b)")
	args = parser.parse_args()

	regex_includes = re.compile(r"""^\s*#[\t\s]*include[\t\s]*("|\<)(?P<include>%s.*)("|\>)""" % args.prefix)
	print("Rebuilding %s:" % args.input)
	with open(args.output, 'w') as output_file, open(args.input, 'r') as input_file:
		output_file.write(
			'// QVM single header distribution. Do not edit.\n'
			'\n'
			'// Generated on ' + date.today().strftime("%m/%d/%Y"))
		if args.hash:
			output_file.write(
				' from https://github.com/boostorg/qvm/tree/' + args.hash[0:7])
		output_file.write(
			'.\n'
			'// Latest versions:\n'
			'//      https://raw.githubusercontent.com/boostorg/qvm/gh-pages/qvm.hpp\n'
			'//      https://raw.githubusercontent.com/boostorg/qvm/gh-pages/qvm_lite.hpp\n'
			'\n' )
		append(args.input, input_file, output_file, regex_includes, args.path)

if __name__ == "__main__":
	_main()
