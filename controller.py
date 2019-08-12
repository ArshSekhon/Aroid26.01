from collections import deque
import paho.mqtt.client as mqtt
import cv2
import urllib.request
import numpy as np
import imutils
st=''
colorLower = (33,80,40)
colorUpper = (102,255,255)
pts = deque(maxlen=1024)

#client = mqtt.Client(client_id="camPythonaa", clean_session=True, userdata=None) 

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.

resp=urllib.request.urlopen("http://192.168.0.100/control?var=framesize&val=10")
while True:
    #client.loop()    

    resp=urllib.request.urlopen("http://192.168.0.100/capture")
    frame = np.asarray(bytearray(resp.read()), dtype="uint8")
    frame = cv2.imdecode(frame, cv2.IMREAD_COLOR)
    frame=cv2.rotate(frame, cv2.ROTATE_90_CLOCKWISE)
    cv2.imshow("Frame", frame)
    key = cv2.waitKey(1) & 0xFF

    # if the 'q' key is pressed, stop the loop
    if key == ord("q"):
        break

    # cleanup the camera and close any open windows

