import ClientClass

globVars = ClientClass.GlobalVariables();
Clnt = ClientClass.Client(globVars);

Clnt.OpenSerialPort();

Clnt.BatteryStatusThread();

Clnt.DrivingThread();

Clnt.ListenIO();