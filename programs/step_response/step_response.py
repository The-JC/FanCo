#imports
import smbus
import time

#slave adress
adress = 0x04

#rpm
w = 1000

#split bytes for communication
data = [0, 0]
lowByte = (w >> 8) & 0xFF
highByte = w & 0xFF
data[0] = lowByte
data[1] = highByte

#rpi directory of csv file
fb = open('Projekte/FanPy/values.csv', 'w')
fb.write("w")
fb.write(";")
fb.write("x")
fb.write(";")
fb.write("y")
fb.write('\n')
fb.close()

#open bus
bus = smbus.SMBus(1)

#loop every 1s
i = 0
while i<60:

    #send data
    bus.write_i2c_block_data(adress, 0, data)
    time.sleep(0.5)

    #read answer
    block = bus.read_i2c_block_data(adress, 0, 3)
    read_buffer = block[0]
    read_buffer = (read_buffer << 8) + block[1]

    #filter
    if block[2] <= 100:

        #write data in csv file
        fb = open('Projekte/FanPy/values.csv', 'a+')
        fb.write(str(w))
        fb.write(";")
        fb.write(str(read_buffer))
        fb.write(";")
        fb.write(str(block[2]*10))
        fb.write('\n')
        fb.close()

        #clear console
        print("\033c")

        #print rpm
        print("w: " + str(w))
        print("x: " + str(read_buffer))
    
    #delay 1s
    time.sleep(1)
    i = i+1