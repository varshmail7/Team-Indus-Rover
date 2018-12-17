import matplotlib.pyplot as plt
from serial_communication import *

if __name__ == '__main__':
    arduino = serial_communication(port='/dev/ttyACM0', baudrate=115200)
    t = 1
    t_prev =0
    tm=10
    y_prev=0
    x=[]
    # y=[]
    fig = plt.figure()
    ax = fig.add_subplot(111)
    plt.axis([0, tm, -1.2, 1.2])
    arduino.writebytes(65)
    arduino.readline()
    while (1):
        data = arduino.readbytes(4)
        y=data
        if(data<=1 and data>=-1):
            print(" Y : "+ str(data) )
            # y.append(data)
            # x.append(t)
            ax.plot([t_prev,t], [y_prev,y],'ro-' )
            y_prev=y
            t_prev=t
        t=t+1
        if (t>tm):
            tm = 1.5*tm
            plt.axis([0, tm , -1.2, 1.2])
            # ax.plot(x, y ,'r')
        plt.pause(1e-3)
        # data = arduino.readline()

plt.show()

# import numpy as np
# import matplotlib.pyplot as plt
#
# plt.axis([0, 10, 0, 1])
#
# for i in range(10):
#     y = np.random.random()
#     plt.scatter(i, y)
#     plt.pause(0.05)
#
# plt.show()