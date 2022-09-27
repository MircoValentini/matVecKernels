#!/usr/bin/python3

import argparse
import sqlite3
import numpy as np
import matplotlib.pyplot as plt
from os.path import exists


#
# Define a timings concatenation table
class timings:
	def __init__(self,name):
		self.sqlInsert="""INSERT INTO profiling(date, machine, compiler, flags, hostname, prefix, isRCM, nRows, procID, kernel, kernelFlavour, caseIndex, mpiIndex, mpiSize, bandWidth, jumpParam, ElapsedTime, ClockTime,CudaTime)
                                  VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"""
		self.sqlCreateTable = """CREATE TABLE IF NOT EXISTS profiling (date text, machine text, compiler text, flags text, hostname text, prefix text, isRCM int, nRows int, procID int, kernel text, kernelFlavour text, caseIndex int, mpiIndex int, mpiSize int, bandWidth double, jumpParam double, ElapsedTime double, ClockTime double, CudaTime double); """
		self.name = name
		try:
			self.con = sqlite3.connect(self.name)
		except Error as e:
			print(e)
		try:
			cur = self.con.cursor()
		except Error as e:
			print(e)
		try:
			cur.execute(self.sqlCreateTable)
		except Error as e:
			print(e)
	
	
	
	def concatenateDB(self, name):
		print( "Connect to: " + name )
		hostCon=sqlite3.connect(name.replace('\'',''))
		hostCur=hostCon.cursor()
		hostRows = hostCur.execute( "SELECT * FROM profiling" )
		cur = self.con.cursor()
		for hostRow in hostRows:
			task = (hostRow[0],hostRow[1],hostRow[2],hostRow[3],hostRow[4],hostRow[5],hostRow[6],hostRow[7],hostRow[8],hostRow[9],hostRow[10],hostRow[11],hostRow[12],hostRow[13],hostRow[14],hostRow[15],hostRow[16],hostRow[17],hostRow[18])
			# print( hostRow )
			cur.execute(self.sqlInsert,hostRow)
		hostCon.close()
		self.con.commit()
	

def main():
	
	# Instantiate the parser
	parser = argparse.ArgumentParser(description='Optional app description')
	# Required positional argument
	parser.add_argument('--input', nargs='*', type=ascii, help='An optional integer positional argument')
	parser.add_argument('--output', type=ascii, help='the output database')
	# parser.parse_args(['input.txt', 'output.txt'])
	args = parser.parse_args()
	
	# print("Argument values:")
	# print(args.input)
	# print(args.output)
	outname=args.output.replace('\'','').replace('\"','')
	
	print(outname)
	t = timings( outname )
	for inputFile in args.input:
		inp=inputFile.replace('\'','').replace('\"','')
		file_exists = exists(inp)
		if ( file_exists ):
			print( "concatenate :: " + inp + " to :: " + args.output )
			t.concatenateDB( inputFile )
	
	


if __name__ == '__main__':
    main()
