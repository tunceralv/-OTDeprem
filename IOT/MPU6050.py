import serial

ser = serial.Serial('COM7', 9600)  # 'COMX'yi Arduino'nuzun bağlı olduğu port ile değiştirin

while True:
    data = ser.readline().decode('utf-8').rstrip()
    print(data)
  
