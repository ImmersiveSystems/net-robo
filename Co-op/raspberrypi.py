import serial

ser = serial.Serial("COM10", 9600)
print 'Opened serial'

exploSpeedMin = 30
exploSpeedMax = 100
exploSpeed = 60
goingForward = 0
goingBackward = 0
turningR = 0
turningL = 0
turningAngle = 80 # this is the difference between the speed of right and left motors

while True:
  var = raw_input("Enter something: ")
  print "You entered: ", var

  # this is to calculate turning speed 
  if exploSpeed > exploSpeedMax - int(turningAngle / 2):
      topSpeed = exploSpeedMax
      lowSpeed = exploSpeedMax - turningAngle
  elif (exploSpeed - int(turningAngle / 2)) <= 0:
      lowSpeed = 0
      topSpeed = turningAngle
  else:
      topSpeed = exploSpeed + int(turningAngle / 2)
      lowSpeed = exploSpeed - int(turningAngle / 2)        

  if var == 'forward':
      print 'Move FORWARD'
      if turningR:
          ser.write('H' + chr(2) + chr(lowSpeed) + chr(2) + chr(topSpeed))
      elif turningL:
          ser.write('H' + chr(2) + chr(topSpeed) + chr(2) + chr(lowSpeed))
      else:
          ser.write('H' + chr(2) + chr(exploSpeed) + chr(2) + chr(exploSpeed))
      goingForward = 1

  elif var == 'backward':
      print 'Move BACKWARD'
      if turningR:
          ser.write('H' + chr(0) + chr(lowSpeed) + chr(0) + chr(topSpeed))
      elif turningL:
          ser.write('H' + chr(0) + chr(topSpeed) + chr(0) + chr(lowSpeed))
      else:
          ser.write('H' + chr(0) + chr(exploSpeed) + chr(0) + chr(exploSpeed))

      goingBackward = 1


  elif (var == '-forward') or (var == '-right') or (var == '-left') or (var == '-backward'):
      print 'Stop movement'

      #stop turning but continue going forward/backward
      if (var == '-right' or var == '-left') and goingForward:
          ser.write('H' + chr(2) + chr(exploSpeed) + chr(2) + chr(exploSpeed))
      elif (var == '-right' or var == '-left') and goingBackward:
          ser.write('H' + chr(0) + chr(exploSpeed) + chr(0) + chr(exploSpeed))
      else:     
          ser.write('H' + chr(2) + chr(0) + chr(2) + chr(0))

      if var == '-forward':
          goingForward = 0
      elif var == '-backward':
          goingBackward = 0
      elif var == '-right':
          turningR = 0
      elif var == '-left':
          turningL = 0

  elif var == 'right':
      print 'Turn RIGHT'
      if goingForward == 1:
          ser.write('H' + chr(2) + chr(lowSpeed) + chr(2) + chr(topSpeed))
      elif goingBackward == 1:
          ser.write('H' + chr(0) + chr(lowSpeed) + chr(0) + chr(topSpeed))        
      else:
          ser.write('H' + chr(0) + chr(exploSpeed) + chr(2) + chr(exploSpeed))
      turningR = 1

  elif var == 'left':
      print 'Turn LEFT'
      if goingForward == 1:
          ser.write('H' + chr(2) + chr(topSpeed) + chr(2) + chr(lowSpeed))
      elif goingBackward == 1:
          ser.write('H' + chr(0) + chr(topSpeed) + chr(0) + chr(lowSpeed)) 
      else:
          ser.write('H' + chr(2) + chr(exploSpeed) + chr(0) + chr(exploSpeed))
      turningL = 1

  elif var == 'stop':
      print 'HALT'
      ser.write('H' + chr(2) + chr(0) + chr(2) + chr(0))
  elif var == 'speedup':
      if exploSpeed >= exploSpeedMax:
          exploSpeed = exploSpeedMax
          print 'Max speed reached'
      else:
          exploSpeed = exploSpeed + 10
          # print 'Speed increased to ' + str(exploSpeed)

      socketIO.emit('lynxToServer', str(exploSpeed))
  elif var == 'speeddown':
      if exploSpeed <= exploSpeedMin:
          exploSpeed = exploSpeedMin
          print 'Min speed reached'
      else:
          exploSpeed = exploSpeed - 10
          # print 'Speed decreased to ' + str(exploSpeed)

      socketIO.emit('lynxToServer', str(exploSpeed))
  elif var.startswith('pan'):
      ser.write('P' + chr(int(var.strip('pan'))))
      print 'PANNING'

  elif var.startswith('tilt'):
      ser.write('T' + chr(int(var.strip('tilt'))))
      print 'TILTING'

  elif var == 'elbowup':
      ser.write('E' + chr(1))
      print 'Move ELBOW UP'

  elif var == 'elbowdown':
      ser.write('E' + chr(0))
      print 'Move ELBOW DOWN'

  elif var == 'wristleft':
      ser.write('W' + chr(0))
      print 'Move WRIST LEFT'

  elif var == 'wristright':
      ser.write('W' + chr(1))
      print 'Move WRIST RIGHT'

  elif var == 'grab':
      ser.write('C')
      print 'Use CLAW'