"""
#Test program for DARM, a robotic arm
"""
import uarm
import time
print('Darm in python:')
def setposition(x,y,z):
	time.sleep(0.1)
	uarm.calculate(x,y,z)
	

#uarm.add('monkey.stl',1,1,0)

#uarm.gears(-81,118,90)
#time.sleep(1)
#uarm.gears(-81,118,0)


setposition(10,0,15)
setposition(10,3,6)
setposition(10,0,6)
setposition(10,3,6)
setposition(10,3,15)


