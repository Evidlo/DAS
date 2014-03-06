#!/usr/bin/python

#Evan Widloski - 2013-09-20
#Dorm Automation System - Event Logging Utility

import serial
import sqlite3
import time
import sys

#Create objects for accessing sqlite database and serial connection
conn = sqlite3.connect('test.db')
cur = conn.cursor()
ser = serial.Serial('/dev/ttyUSB0')
print time.strftime("%Y-%m-%d_%H:%M :: "),"START"

#Function to grab commands and the door state from the serial input buffer
#Data is sent using a very simple protocol with no error-checking, bad data is simply discarded
#Protocol is very basic, in the form "[command] [argument];[command] [argument];[command] [argument];..."
def getCommands():
	inputBytes=[]
	command=[]
	commands=[]
	command_completed=False
	while command_completed == False:	#while there's still commands in the buffer
		byte=ser.read(1)				#read 1 byte in
		
		#if the byte marks the end of the command-argument pair
		if byte == ';':					
			#add the command argument to the command list (as string)
			command.append(''.join(inputBytes))							
			#add the command to the commands list
			commands.append(command)									
			inputBytes=[]				#clear lists
			command=[]
			#only stop reading if theres no bytes and if we end on a ;
			if ser.inWaiting() == 0:
				command_completed=True									
		
		#if the byte marks the end of the command
		elif byte == ' ':
			#add the command to the command list (as string)												
			command.append(''.join(inputBytes))							
			#clear list
			inputBytes=[]												
		else:
			inputBytes.append(byte)

	return commands

#Constantly waits for data from input buffer
while True:
	try:

		if ser.inWaiting() > 0:
			commands=getCommands()
			date_string=time.strftime("\"%Y-%m-%d_%H:%M\"")

			for command in commands:
			
				try:
					if command[0] == "control":								#insert control actions into Control table with timestamp
						print time.strftime("%Y-%m-%d_%H:%M :: "),"insertion",command
						cur.execute("INSERT INTO Controls(time, action) VALUES(?,?)",(date_string,command[1]))
						conn.commit()
					if command[0] == "door":								#insert into Doors table with timestamp
						print time.strftime("%Y-%m-%d_%H:%M :: "),"insertion",command
						cur.execute("INSERT INTO Doors(time, door) VALUES(?,?)",(date_string,command[1]))
						conn.commit()
			
				except sqlite3.OperationalError as exception_args:
					print "Error:"+str(exception_args)
					print "Creating table(s)"
					if str(exception_args) == "no such table: Controls":	#if either table does not exist, create it
						cur.execute("CREATE TABLE Controls(time VARCHAR(500), action VARCHAR(2))");
					if str(exception_args) == "no such table: Doors":
						cur.execute("CREATE TABLE Doors(time VARCHAR(500), door INTEGER)");
					conn.commit()
	except IOError as exception_args:
		print time.strftime("%Y-%m-%d_%H:%M :: "),"Error:"+str(exception_args)
	time.sleep(.3)
