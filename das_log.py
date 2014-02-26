#!/usr/bin/python
import serial
import sqlite3
import time

conn = sqlite3.connect('test.db')
cur = conn.cursor()
ser = serial.Serial('/dev/ttyUSB0')


def getCommands():
	inputBytes=[]
	command=[]
	commands=[]
	command_completed=False
	while command_completed == False:									#while there's still commands in the buffer
		byte=ser.read(1)												#read 1 byte in
		if byte == ';':													#if the byte marks the end of the command argument
			command.append(''.join(inputBytes))							#add the command parameter to the command list (as string)
			commands.append(command)									#add the command to the commands list
			inputBytes=[]												#clear lists
			command=[]
			if ser.inWaiting() == 0:
				command_completed=True									#only stop reading if theres no bytes and if we end on a ;
		elif byte == ' ':												#if the byte marks the end of the command
			command.append(''.join(inputBytes))							#add the command to the command list (as string)
			inputBytes=[]												#clear list
		else:
			inputBytes.append(byte)

	return commands

while True:
	if ser.inWaiting() > 0:
		commands=getCommands()
		date_string=time.strftime("\"%Y-%m-%d_%H:%M\"")
		for command in commands:
			try:
				if command[0] == "control":								#insert control actions into Control table with timestamp
					print "insertion",command
					cur.execute("INSERT INTO Controls(time, action) VALUES(?,?)",(date_string,command[1]))
					conn.commit()
				if command[0] == "door":								#insert knocks into Doors table with timestamp
					print "insertion",command
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
	time.sleep(5)
