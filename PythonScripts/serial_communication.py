import serial
import struct

SERIAL_DEBUG = 1  # Set to '1' To Display DEBUG Messages
SERIAL_UNIT_TEST = 1  # Set to '1' For Serial Module Unit Test

DEFAULT_PORT = '/dev/ttyUSB1'
DEFAULT_BAUDRATE = 9600
DEFAULT_TIMEOUT = 0.1


class serial_communication:

    # Serial Communication Constructor
    def __init__(self, port=DEFAULT_PORT, baudrate=DEFAULT_BAUDRATE, timeout=DEFAULT_TIMEOUT):
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout

        if SERIAL_DEBUG:
            print('SERIAL :: ')
            print(" port = " + str(self.port) + " ,")
            print(" baudrate = " + str(self.baudrate) + " ,")
            print(" timeout = " + str(self.timeout) + "\n")

        self.setup_connection()

    # Read String Line from
    def readline(self):
        data = self.device.readline()
        if SERIAL_DEBUG:
            print("SERIAL :: StrData = " + str(data))

            # try:
            #     print(" BYTES : " + str(struct.pack('I', float(data))))
            # except:
            #     print(" NO BYTES")

        return str(data)

    def readbytes(self,num_bytes=1):
        data = self.device.read(num_bytes)
        try:
            data = float(struct.unpack_from('f', data)[0])
        except:
            data = 0
        if SERIAL_DEBUG:
            print("SERIAL :: ReadByteData = " + str(data))
        return data

    def writebytes(self,value):
        """Packs a python 4 byte unsigned integer to an arduino unsigned long"""
        data = struct.pack('f', value)
        if SERIAL_DEBUG:
            print("SERIAL :: WriteByteData = " + str(data))
        self.device.write(data)

    def setup_connection(self):
        self.device = serial.Serial(self.port, self.baudrate, timeout=self.timeout)
        self.device.flush()
        if SERIAL_DEBUG:
            if self.device:
                print("SERIAL :: Connection Setup Complete")
            else:
                print("SERIAL :: Connection Setup Failed")

        # def write(self):

    # def packIntegerAsULong(self, value):
    #     """Packs a python 4 byte unsigned integer to an arduino unsigned long"""
    #     return struct.pack('I', value)  # should check bounds




if SERIAL_UNIT_TEST:
    import time
    if __name__ == '__main__':
        arduino = serial_communication(port='/dev/ttyACM0', baudrate=115200)
        # d=arduino.readline()
        # while d!='1001':
        #     print("PAUSE")
        #     d = arduino.readline()
        #     print(" d ="+str(d))
        #     print(" d.len ="+str(d.__len__()))
        #     print(" d=='1001' ::  "+str(d=='1001\n'))
        # arduino.writebytes(65)
        # arduino.readline()

        while(1):

            # arduino.readbytes(4)
            # time.sleep(0.001)
            arduino.writebytes(67)
            arduino.readline()



# arduino.write('c')
# data = arduino.readline()
# arduino.write('c')
# for x in range(5):
# 	data = arduino.readline()
# 	if data!='':
# 		d.append(int(data))
# 		print d
# 	else:
# 		d.append(1)
# time.sleep(15)


