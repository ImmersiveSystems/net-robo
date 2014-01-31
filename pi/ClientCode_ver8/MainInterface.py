import sys

sys.path.append('./Modules')
import ClientClass

Clnt = ClientClass.Client();

Clnt.OpenSerialPort();

Clnt.BatteryStatusThread();

Clnt.DrivingThread();
Clnt.ServosThread();

Clnt.ListenIO();