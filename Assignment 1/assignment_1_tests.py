#!/usr/bin/env python

import os
import sys
import shutil
from subprocess import Popen, PIPE

def exec_prog(prog, exit_on_failure = True):
	try:
		process = Popen(prog, stdout=PIPE, stderr=PIPE)
		(output, err) = process.communicate()
		exit_code = process.wait()
		if exit_code != 0 and exit_on_failure:
			print str(prog), "returned ", exit_code
			sys.exit(exit_code)
		return output
	except Exception as inst:
		if exit_on_failure:
			print "Failed to exec program", str(prog)
			sys.exit(1)
		else:
			return ""


def clean():
	exec_prog(["make", "clean"])

def build():
	exec_prog(["make"])

def test(args):
	my_echo_output = exec_prog(["./my_echo"] + args, False);
	echo_output = exec_prog(["echo"] + args);

	return my_echo_output == echo_output

def tests():
	success_list = []
	test_cases = [
		["-n", "fdas fdsa dfs"],
		["-n"],
		[],
		["asdf"],
		["-n", "asdf"],
		["asdf dfsa fdas", "fasdf", "asdf"],
		]

	for test_case in test_cases:
		result = test(test_case)
		success_list.append(result)

	return success_list

def print_results(success_list):
	i = 0
	success_count = 0;
	for s in success_list:
		i += 1
		if s:
			success_count += 1
		else:
			print "Failed test", i

	print "Grade:", success_count, "/", len(success_list)


def main():
	clean()
	build()
	print_results(tests())

if __name__ == "__main__":
		main()
