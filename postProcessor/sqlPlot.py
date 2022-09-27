#!/usr/bin/python3

import sqlite3
import numpy as np
import matplotlib.pyplot as plt

class timeData:
	
	def __init__( self,x, mean, stdDev ):
		self.x      = x
		self.mean   = mean
		self.stdDev = stdDev
	
	def print(self):
		print( self.x )
		print( self.mean )
		print( self.stdDev )


#
# Define a timings
class timings:
	
	def __init__(self,name):
		self.name = name
		self.con = sqlite3.connect(self.name)
		self.cur = self.con.cursor()
		#self.cur.execute('SELECT * FROM profiling WHERE nRows=15000 AND kernelFlavour = \'COO_COO\' ORDER BY ElapsedTime')
		#self.t0 = self.cur.fetchall()
		#self.cur.execute('SELECT * FROM profiling WHERE nRows=15000 AND kernelFlavour = \'CSC_CSR\' ORDER BY ElapsedTime')
		#self.t1 = self.cur.fetchall()
		#self.cur.execute('SELECT * FROM profiling WHERE kernelFlavour = \'COO_COO\' ORDER BY nRows')
		#self.t2 = self.cur.fetchall()
		#self.cur.execute('SELECT * FROM profiling WHERE kernelFlavour = \'COO_CSR\' ORDER BY nRows')
		#self.t3 = self.cur.fetchall()
		#self.cur.close()
	
	def __del__(self):
		self.cur.close()
	
	def extractData(self,machine,isRCM,nRows,Flavour,Prefix):
		# self.cur.execute('SELECT * FROM profiling WHERE machine=\'m100\' AND isRCM=1 AND nRows=? AND kernelFlavour=? AND prefix=?', (nRows,Flavour,Prefix))
		self.cur.execute('SELECT * FROM profiling WHERE machine=? AND isRCM=? AND nRows=? AND kernelFlavour=? AND prefix=?', (machine,isRCM,nRows,Flavour,Prefix))
		t0 = self.cur.fetchall()
		L = len(t0)
		y0 = np.zeros( [L] )
		cnt = 0
		for row in t0:
			y0[cnt] = row[16]
			cnt = cnt + 1
		return timeData( nRows, np.mean(y0), np.std(y0) )



#
# Initialize a timing strucutre
t = timings( "concatenatedDB.db" )
# t = timings( "./timings/m100/000003/timings_COO_COO.sqlite.db" )

list1 = ['COO_COO', 'CSC_CSR', 'CSR_CSR', 'CSR', 'BlockRowPerm_2', 'BlockRowPerm_4', 'BlockRowPerm_6', 'BlockRowPerm_8', 'BlockRowPerm_10', 'BlockRowPerm_12', 'BlockRowPerm_14', 'BlockRowPerm_16', 'BlockRowPerm_18', 'BlockRowPerm_20', 'BlockRowPerm_22', 'BlockRowPerm_24', 'BlockRowPerm_26', 'BlockRowPerm_28', 'BlockRowPerm_30', 'BlockRowPerm_32', 'BlockRowPerm_34', 'BlockRowPerm_36', 'BlockRowPerm_38', 'BlockRowPerm_40', 'BlockRowPerm_42', 'BlockRowPerm_44', 'BlockRowPerm_46', 'BlockRowPerm_48', 'BlockRowPerm_50', 'BlockRowPerm_55', 'BlockRowPerm_60', 'BlockRowPerm_65', 'BlockRowPerm_70', 'BlockRowPerm_75', 'BlockRowPerm_80', 'BlockRowPerm_85', 'BlockRowPerm_90', 'BlockRowPerm_95', 'BlockRowPerm_100']

list2 = [ 'HEXmats', 'SHMmats' ]
list3 = [ 5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000 ]



labels = ['LDU-orderedCOO', 'LDU-CSC_CSR', 'LDU-CSR', 'MSR', 'LDU-orderedCOO-RCM', 'LDU-CSC_CSR-RCM', 'LDU-CSR-RCM', 'LDU-MSR-RCM','BlockRowPerm_2', 'BlockRowPerm_4', 'BlockRowPerm_6', 'BlockRowPerm_8', 'BlockRowPerm_10', 'BlockRowPerm_12', 'BlockRowPerm_14', 'BlockRowPerm_16', 'BlockRowPerm_18', 'BlockRowPerm_20', 'BlockRowPerm_22', 'BlockRowPerm_24', 'BlockRowPerm_26', 'BlockRowPerm_28', 'BlockRowPerm_30', 'BlockRowPerm_32', 'BlockRowPerm_34', 'BlockRowPerm_36', 'BlockRowPerm_38', 'BlockRowPerm_40', 'BlockRowPerm_42', 'BlockRowPerm_44', 'BlockRowPerm_46', 'BlockRowPerm_48', 'BlockRowPerm_50', 'BlockRowPerm_55', 'BlockRowPerm_60', 'BlockRowPerm_65', 'BlockRowPerm_70', 'BlockRowPerm_75', 'BlockRowPerm_80', 'BlockRowPerm_85', 'BlockRowPerm_90', 'BlockRowPerm_95', 'BlockRowPerm_100']

LL = len(list3)
L = len(list1)


x0 = np.zeros(L+4)
y0 = np.zeros(L+4)
z0 = np.zeros(L+4)

x1 = np.zeros(L+4)
y1 = np.zeros(L+4)
z1 = np.zeros(L+4)

x2 = np.zeros(L+4)
y2 = np.zeros(L+4)
z2 = np.zeros(L+4)

x3 = np.zeros(L+4)
y3 = np.zeros(L+4)
z3 = np.zeros(L+4)



width=0.18
for dim in list3:
	cnt=0
	for i in range(4):
		xx = t.extractData('m100',0,dim,list1[i],'HEXmats')
		x0[i] = cnt
		y0[i] = xx.mean*1000000
		z0[i] = xx.stdDev*1000000
		xx = t.extractData('m100',0,dim,list1[i],'SHMmats')
		x1[i] = cnt
		y1[i] = xx.mean*1000000
		z1[i] = xx.stdDev*1000000
		xx = t.extractData('g100',0,dim,list1[i],'HEXmats')
		x2[i] = cnt
		y2[i] = xx.mean*1000000
		z2[i] = xx.stdDev*1000000
		xx = t.extractData('g100',0,dim,list1[i],'SHMmats')
		x3[i] = cnt
		y3[i] = xx.mean*1000000
		z3[i] = xx.stdDev*1000000
		cnt=cnt+1
	for i in range(L):
		xx = t.extractData('m100',1,dim,list1[i],'HEXmats')
		x0[i+4] = cnt
		y0[i+4] = xx.mean*1000000
		z0[i+4] = xx.stdDev*1000000
		xx = t.extractData('m100',1,dim,list1[i],'SHMmats')
		x1[i+4] = cnt
		y1[i+4] = xx.mean*1000000
		z1[i+4] = xx.stdDev*1000000
		xx = t.extractData('g100',1,dim,list1[i],'HEXmats')
		x2[i+4] = cnt
		y2[i+4] = xx.mean*1000000
		z2[i+4] = xx.stdDev*1000000
		xx = t.extractData('g100',1,dim,list1[i],'SHMmats')
		x3[i+4] = cnt
		y3[i+4] = xx.mean*1000000
		z3[i+4] = xx.stdDev*1000000
		cnt=cnt+1
	# Plot Data
	plt.figure()
	plt.title( "number of Rows = " + str(dim),fontsize=35 )
	aa = plt.bar( x0-width*3/2, y0, yerr=z0, width=width, capsize=4, color='red',   label='HEXmats - m100' )
	bb = plt.bar( x1-width/2,   y1, yerr=z1, width=width, capsize=4, color='blue',  label='SHMmats - m100' )
	cc = plt.bar( x2+width/2,   y2, yerr=z2, width=width, capsize=4, color='green', label='HEXmats - g100' )
	dd = plt.bar( x3+width*3/2, y3, yerr=z3, width=width, capsize=4, color='black', label='SHMmats - g100' )
	plt.legend( handles=[aa,bb,cc,dd], fontsize=20 )
	plt.yticks( fontsize=20 )
	plt.xticks(range(len(labels)), labels, size='small', rotation='vertical', fontsize=20)
	plt.ylabel( 'Time [us]', fontsize=30 )
	plt.xlabel( 'Kernel Type', fontsize=30 )
	plt.show()

#plt.bar( x0-width/2, y0, yerr=z0, width=width, color='red', capsize=4 )
#plt.bar( x1+width/2, y1, yerr=z0, width=width, color='blue', capsize=4 )


# xx.print()

#
# Plot 
# t.plot01()


exit()


