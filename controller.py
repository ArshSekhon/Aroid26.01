from collections import deque 
import cv2
from cv2 import VideoCapture
import urllib.request
from numpy import ones,vstack
import numpy as np
import imutils
from statistics import mean 
from numpy.linalg import lstsq
import requests
import time

st=''
colorLower = (33,80,40)
colorUpper = (102,255,255)
pts = deque(maxlen=1024)

def do_segment(frame):
    # Since an image is a multi-directional array containing the relative intensities of each pixel in the image, we can use frame.shape to return a tuple: [number of rows, number of columns, number of channels] of the dimensions of the frame
    # frame.shape[0] give us the number of rows of pixels the frame has. Since height begins from 0 at the top, the y-coordinate of the bottom of the frame is its height
    height = frame.shape[0]
    width = frame.shape[1]
    # Creates a triangular polygon for the mask defined by three (x, y) coordinates
    polygons = np.array([
                            [(0, height), (width, height), (width, (int)(height*(140/400))) , ((int)(width/2), (int)(height*(60/400))) , (0, (int)(height*(140/400))) ]
                        ])
    # Creates an image filled with zero intensities with the same dimensions as the frame
    mask = np.zeros_like(frame)
    # Allows the mask to be filled with values of 1 and the other areas to be filled with values of 0
    cv2.fillPoly(mask, polygons, 255)
    # A bitwise and operation between the mask and frame keeps only the triangular area of the frame
    segment = cv2.bitwise_and(frame, mask) 

    return segment


#client = mqtt.Client(client_id="camPythonaa", clean_session=True, userdata=None) 

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
def make_points(frame, line):
    height, width = frame.shape
    slope, intercept = line
    y1 = height  # bottom of the frame
    y2 = int(y1 * 1 / 2)  # make points from middle of the frame down

    # bound the coordinates within the frame
    x1 = max(-width, min(2 * width, int((y1 - intercept) / slope)))
    x2 = max(-width, min(2 * width, int((y2 - intercept) / slope)))
    return [[x1, y1, x2, y2]]

def average_slope_intercept(frame, line_segments):
    """
    This function combines line segments into one or two lane lines
    If all line slopes are < 0: then we only have detected left lane
    If all line slopes are > 0: then we only have detected right lane
    """
    lane_lines = []
    if line_segments is None:
        #logging.info('No line_segment segments detected')
        return lane_lines

    height, width = frame.shape
    left_fit = []
    right_fit = []

    boundary = 2/3
    left_region_boundary = width * (1 - boundary)  # left lane line segment should be on left 2/3 of the screen
    right_region_boundary = width * boundary # right lane line segment should be on left 2/3 of the screen

    for line_segment in line_segments:
        for x1, y1, x2, y2 in line_segment:
            if x1 == x2:
                #logging.info('skipping vertical line segment (slope=inf): %s' % line_segment)
                continue
            fit = np.polyfit((x1, x2), (y1, y2), 1)
            slope = fit[0]
            intercept = fit[1]
            if slope < 0:
                if x1 < left_region_boundary and x2 < left_region_boundary:
                    left_fit.append((slope, intercept))
            else:
                if x1 > right_region_boundary and x2 > right_region_boundary:
                    right_fit.append((slope, intercept))

    left_fit_average = np.average(left_fit, axis=0)
    if len(left_fit) > 0:
        lane_lines.append(make_points(frame, left_fit_average))

    right_fit_average = np.average(right_fit, axis=0)
    if len(right_fit) > 0:
        lane_lines.append(make_points(frame, right_fit_average))

    #logging.debug('lane lines: %s' % lane_lines)  # [[[316, 720, 484, 432]], [[1009, 720, 718, 432]]]
    return lane_lines
def find_heading_lines(img, lanes):
    totalY=0
    totalX=0
    for line_segment in lanes:
        for x1, y1, x2, y2 in line_segment:
            if(y1<0):
                y1=999
            if(y2<0):
                y2=999

            if(y1>img.shape[0]):
                y1=img.shape[0]
            if(y2>img.shape[0]):
                y2=img.shape[0]
            
            if(x1<0):
                x1=999
            if(x2<0):
                x2=999

            if(x1>img.shape[1]):
                x1=img.shape[1]
            if(x2>img.shape[1]):
                x2=img.shape[1]

            totalY = totalY + min(y1, y2)
            if (min(y1,y2)==y2):
                totalX = totalX+x2
            else:
                totalX = totalX+x1
    
    return (totalX/2, totalY/2)

def draw_lines(img, lines, color=[0,255,0]):
    try:
        for line in lines:
            coords = line[0]
            cv2.line(img, (coords[0],coords[1]), (coords[2],coords[3]),color, 3)
    except:
        pass
 

def process_img(original_image):
    processed_img = cv2.cvtColor(original_image, cv2.COLOR_BGR2GRAY)
    processed_img = cv2.Canny(processed_img, threshold1=150, threshold2=200)
    processed_img = cv2.GaussianBlur(processed_img, (9,9), 0 )  
    #processed_img= do_segment(processed_img)
    #edges
    #processed_img = do_segment(processed_img)
    hough_lines = cv2.HoughLinesP(processed_img, 1, np.pi/180, 180, minLineLength=15, maxLineGap=10)

    lane_lines    = average_slope_intercept(processed_img, hough_lines)
    original_image_with_lanes=original_image
    original_image_with_hough_lines=original_image

    draw_lines(original_image_with_lanes, lane_lines) 
    draw_lines(original_image_with_hough_lines, hough_lines) 
    draw_lines(original_image_with_hough_lines, lane_lines, [255,0,0]) 
    #output = cv2.addWeighted(original_image, 0.9, original_image_with_lanes, 1, 1)
  
    cv2.imshow('res',original_image_with_hough_lines)

    heading_lanes=find_heading_lines(original_image_with_lanes, lane_lines)
    
    
    return original_image, heading_lanes
def spinMotors(lf, lr, rf, rr):
    url = "http://192.168.0.99/serialInput"

    payload = "input=W%2C{:d}%2C{:d}%2C{:d}%2C{:d}&undefined=".format(lf, lr, rf, rr)
    headers = {'Content-Type': "application/x-www-form-urlencoded",'cache-control': "no-cache"}

    response = requests.request("POST", url, data=payload, headers=headers)


def main():
    resp=urllib.request.urlopen("http://192.168.0.100/control?var=framesize&val=6")
    filtered_heading_x=0
    filtered_heading_y=0

    tick = 0
    waiting_ticks = 0
    while True:
        #client.loop()    
        resp=urllib.request.urlopen("http://192.168.0.100/capture")
        frame = np.asarray(bytearray(resp.read()), dtype="uint8")
        frame = cv2.imdecode(frame, cv2.IMREAD_COLOR)
        frame=cv2.rotate(frame, cv2.ROTATE_90_CLOCKWISE)

        
        
        frame, (heading_x, heading_y) = process_img(frame)  
        filtered_heading_x = filtered_heading_x*0.9+heading_x*0.1
        filtered_heading_y = filtered_heading_y*0.9+heading_y*0.1
        print(filtered_heading_x, filtered_heading_y)
        
        cv2.line(frame, ((int)(filtered_heading_x),0), ((int)(filtered_heading_x), (int)(frame.shape[0])), [0,0,255], 3)
        cv2.line(frame, (0, (int)(filtered_heading_y)), ((int)(frame.shape[1]), (int)(filtered_heading_y)), [0,0,255], 3)

        cv2.imshow("Frame", frame) 
        if(tick<20):
            tick=tick+1
        else:
            if(waiting_ticks==0):
                if(filtered_heading_x>(frame.shape[1]/2-100) and filtered_heading_x<(frame.shape[1]/2+100)):
                    print("go straight") 
                    spinMotors(90,90,90,90)
                    time.sleep(1)
                    spinMotors(0,0,0,0)

                    waiting_ticks=5  
                elif(filtered_heading_x<150 ):
                    print("turn left")  
                    spinMotors(-200,-200,200,200) 
                    time.sleep(0.15)
                    spinMotors(0,0,0,0)
                    waiting_ticks=15            
                elif(filtered_heading_x>frame.shape[1]-150 ):
                    print("turn right") 
                    spinMotors(200,200,-200,-200) 
                    time.sleep(.15)
                    spinMotors(0,0,0,0)
                    waiting_ticks=15         
                elif(filtered_heading_x<(frame.shape[1]/2-100)): 
                    print("turn right") 
                    spinMotors(200,200,-200,-200) 
                    time.sleep(.05)
                    spinMotors(0,0,0,0)
                    waiting_ticks=10   

                elif(filtered_heading_x>(frame.shape[1]/2+100)): 
                    print("turn left")  
                    spinMotors(-200,-200,200,200) 
                    time.sleep(0.05)
                    spinMotors(0,0,0,0)
                    waiting_ticks=10   
            else:
                pass    
 

        waiting_ticks= waiting_ticks-1
        if(waiting_ticks<0):
            waiting_ticks=0
 
        key = cv2.waitKey(1) & 0xFF

        # if the 'q' key is pressed, stop the loop
        if key == ord("q"):
            break

        # cleanup the camera and close any open windows

try:
    main()
except KeyboardInterrupt:
    spinMotors(0,0,0,0)