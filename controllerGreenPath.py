import cv2
import urllib.request
import numpy as np
import requests
import pickle
import time
import PID
import travel_mapping

import matplotlib.pyplot as plt


def segment_roi(frame, polygons):
    height = frame.shape[0]
    width = frame.shape[1]
    mask = np.zeros_like(frame)
    cv2.fillPoly(mask, polygons, 255)
    segment = cv2.bitwise_and(frame, mask)
    return segment


def draw_lines(img, lines, color=[0,0,255]):
    try:
        for line in lines:
            coords = line[0]
            cv2.line(img, (coords[0],coords[1]), (coords[2],coords[3]),color, 3)
    except:
        pass


def sign(x):
    return (1, -1)[x < 0]


def slope_sign(x1, y1, x2, y2):
    return sign(y2 * 1.0 - y1) / sign(x2 * 1.0 - x1)


def find_heading_lines(img, lanes, last_heading, heading_x_from_hist):
    if(len(lanes)>1):
        totalY = 0
        totalX = 0
        for line_segment in lanes:
            for x1, y1, x2, y2 in line_segment:
                if (y1 < 0):
                    y1 = 999
                if (y2 < 0):
                    y2 = 999

                if (y1 > img.shape[0]):
                    y1 = img.shape[0]
                if (y2 > img.shape[0]):
                    y2 = img.shape[0]

                if (x1 < 0):
                    x1 = 999
                if (x2 < 0):
                    x2 = 999

                if (x1 > img.shape[1]):
                    x1 = img.shape[1]
                if (x2 > img.shape[1]):
                    x2 = img.shape[1]

                totalY = totalY + min(y1, y2)
                if (min(y1, y2) == y2):
                    totalX = totalX + x2
                else:
                    totalX = totalX + x1
        return (totalX / 2, totalY / 2)
    elif (len(lanes)==1):
        x1, y1, x2, y2 = lanes[0][0][0], lanes[0][0][1], lanes[0][0][2], lanes[0][0][3]
        max_x = max(x1,x2)
        min_x  = min(x1,x2)
        if((slope_sign(x1, y1, x2, y2))<0):
            current_heading = (((max_x+img.shape[1])//2 + 1.5*heading_x_from_hist)//2.5, (y1+y2)//2)
            last_heading = current_heading
            return current_heading
        else:
            current_heading = (((min_x)/2.5 + 1.5*heading_x_from_hist)//2.5, (y1+y2)//2)
            last_heading = current_heading
            return current_heading

    else:
        return (heading_x_from_hist, img.shape[0]//2)

def create_windows():
    cv2.namedWindow("Original Undistorted")
    cv2.namedWindow("Canny Edges Road")
    cv2.namedWindow("Color Hist")
    cv2.namedWindow("Road Mask")
    cv2.namedWindow("Hough lines with Lanes")


    cv2.moveWindow("Original Undistorted", 0, 0)
    cv2.moveWindow("Color Hist", 1050, 0)
    cv2.moveWindow("Road Mask", 575, 0)
    cv2.moveWindow("Canny Edges Road", 575, 490)
    cv2.moveWindow("Hough lines with Lanes", 1050, 490)


def process_img(original_image, last_heading):
    processed_img = cv2.cvtColor(original_image, cv2.COLOR_BGR2HSV)
    sensitivity=50
    sensitivity = 50
    green_mask = cv2.inRange(processed_img, (60 - sensitivity, 10, 10), (60 + sensitivity, 255, 255))
    in_mask = green_mask>0
    green = np.zeros_like(processed_img, np.uint8)
    green[in_mask] = processed_img[in_mask]
    cv2.imshow("Road Mask", green)

    processed_img = cv2.GaussianBlur(green, (7,7), 0 )
    processed_img = cv2.Canny(processed_img, threshold1=110, threshold2=200)
    processed_img = cv2.GaussianBlur(processed_img, (3,3), 0 )

    cv2.imshow("Canny Edges Road", processed_img)

    height, width = original_image.shape[0],original_image.shape[1]

    roi_polygons = np.array([
        [(0, height), (width, height), (width, (int)(height * (0.5))), (0, (int)(height * (0.5)))]
    ])
    processed_img_roi = segment_roi(processed_img, roi_polygons)

    hough_lines = cv2.HoughLinesP(processed_img, 10 ,1*np.pi/180, 200, np.array([]), minLineLength=15, maxLineGap=1)
    hough_lines_roi = cv2.HoughLinesP(processed_img_roi, 10 ,1*np.pi/180, 200,  np.array([]), minLineLength=15, maxLineGap=1)

    lane_lines = average_slope_intercept(processed_img, hough_lines_roi)
    original_image_with_lanes = original_image
    original_image_with_hough_lines = original_image

    draw_lines(original_image_with_lanes, lane_lines)
    draw_lines(original_image_with_hough_lines, hough_lines,[0,0,255])
    draw_lines(original_image_with_hough_lines, hough_lines_roi)
    draw_lines(original_image_with_hough_lines, lane_lines, [255, 0, 0])
    # output = cv2.addWeighted(original_image, 0.9, original_image_with_lanes, 1, 1)
    hue_filter = green[:, :, 0]
    fig = plt.figure()
    plot =fig.add_subplot(111)

    # If we haven't already shown or saved the plot, then we need to
    # draw the figure first..
    histogram = np.sum(hue_filter[hue_filter.shape[0] // 2:, :], axis=0)

    plot.plot(histogram)
    fig.canvas.draw()

    # Now we can save it to a numpy array.
    data = np.fromstring(fig.canvas.tostring_rgb(), dtype=np.uint8, sep='')
    data = data.reshape(fig.canvas.get_width_height()[::-1] + (3,))

    cv2.imshow("Color Hist", data)

    max_val = np.argmax(histogram[:])


    area_undercurve_path_hue=np.sum(histogram[:])
    half_area_undercurve = area_undercurve_path_hue//2


    temp_area=0
    heading_x_from_hist = -1
    for i, val in enumerate(histogram):
        temp_area+=val
        if(temp_area>half_area_undercurve):
            #print(temp_area)
            heading_x_from_hist = i
            break

    heading_point = find_heading_lines(original_image_with_lanes, lane_lines, last_heading, heading_x_from_hist)
    return original_image, heading_point


def make_points(frame, line):
    height, width = frame.shape
    slope, intercept = line
    y1 = height  # bottom of the frame
    y2 = int(y1 * 1 / 1.8)

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
        return lane_lines

    height, width = frame.shape
    left_fit = []
    right_fit = []

    boundary = 1/2
    left_region_boundary = width * (1 - boundary)  # left lane line segment should be on left 2/3 of the screen
    right_region_boundary = width * boundary # right lane line segment should be on left 2/3 of the screen

    for line_segment in line_segments:
        for x1, y1, x2, y2 in line_segment:
            if x1 == x2:
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
    right_fit_average = np.average(right_fit, axis=0)

    if len(left_fit) > 0 :
        avg_slope_left,_ = left_fit_average
    if len(right_fit) > 0 :
        avg_slope_right,_ = right_fit_average

    if len(left_fit) > 0 and len(right_fit) > 0 and not sign(avg_slope_left)==sign(avg_slope_right):
        lane_lines.append(make_points(frame, left_fit_average))
        lane_lines.append(make_points(frame, right_fit_average))

    elif len(left_fit) > 0:
        lane_lines.append(make_points(frame, left_fit_average))

    elif len(right_fit) > 0:
        lane_lines.append(make_points(frame, right_fit_average))

    return lane_lines


def spinMotors(lf, lr, rf, rr):
    url = "http://192.168.0.99/serialInput"

    payload = "input=WRITE_MOTORS%3B{:d}%3B{:d}%3B{:d}%3B{:d}%3B&undefined=".format(lf, lr, rf, rr)
    headers = {'Content-Type': "application/x-www-form-urlencoded",'cache-control': "no-cache"}

    response = requests.request("POST", url, data=payload, headers=headers)


def moveDistance(distance, speed):
    url = "http://192.168.0.99/serialInput"

    payload = "input=SET_TARGET_DIST%3B{:d}%3B{:d}%3B&undefined=".format(distance, speed)
    headers = {'Content-Type': "application/x-www-form-urlencoded",'cache-control': "no-cache"}

    response = requests.request("POST", url, data=payload, headers=headers)


frame_to_save = None


def move():
    resp=urllib.request.urlopen("http://192.168.0.100/control?var=framesize&val=6")
    h, w = 480,640
    calib_params = pickle.load( open( "camera_calibration.pickle", "rb" ) )
    mtx = calib_params["mtx"]
    dist = calib_params["dist"]
    filtered_heading_x = 0
    filtered_heading_y = 0
    last_heading = (0,0)
    P_turn = 0.001/320
    I_turn = 0.003/1000
    D_turn = 0.0005/50
    pid_turn = PID.PID_Control(P_turn, I_turn, D_turn)
    pid_turn.SetPoint=0
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(mtx, dist, (640, 480), 1, (640, 480))
    tick = 0
    waiting_ticks = 0
    delta_time = 0
    motorSpeed1 = 180
    motorSpeed2 = 150

    create_windows()
    while True:
        resp=urllib.request.urlopen("http://192.168.0.100/capture")
        frame = np.asarray(bytearray(resp.read()), dtype="uint8")
        frame = cv2.imdecode(frame, cv2.IMREAD_COLOR)
        frame=cv2.rotate(frame, cv2.ROTATE_180)
        frame = cv2.undistort(frame, mtx, dist, None, newcameramtx)
        # crop the frame to get rid of distortion
        x, y, w, h = roi
        frame = frame[y:y + h, x:x + w]
        global frame_to_save
        frame_to_save = frame.copy()
        cv2.imshow("Original Undistorted", frame_to_save)

        frame, (heading_x, heading_y) = process_img(frame, last_heading)
        filtered_heading_x = filtered_heading_x * 0.8 + heading_x * 0.2
        filtered_heading_y = filtered_heading_y * 0.8 + heading_y * 0.2
        # print(filtered_heading_x, filtered_heading_y)

        error = (int)(filtered_heading_x-frame.shape[1]/2)
        distance =  (int)(frame.shape[0]-filtered_heading_y)

        print("Error:", error,  "Dist:", distance, "Heading X: ", filtered_heading_x)

        cv2.line(frame, ((int)(filtered_heading_x), 0), ((int)(filtered_heading_x), (int)(frame.shape[0])), [255, 255, 0], 3)
        cv2.line(frame, (0, (int)(filtered_heading_y)), ((int)(frame.shape[1]), (int)(filtered_heading_y)), [255, 255, 0], 3)

        cv2.imshow("Hough lines with Lanes", frame)
        if tick < 20:
            tick = tick + 1
        else:
            if waiting_ticks == 0:
                error_heading = frame.shape[1]/2-filtered_heading_x
                travel_mapping.get_orientation_and_odometry_point()

                if -100 < error_heading < 100:
                    print("go straight")
                    spinMotors(motorSpeed1, motorSpeed1, motorSpeed1, motorSpeed1)
                    time.sleep(1.2)
                    spinMotors(0, 0, 0, 0)
                    delta_time += 1
                    waiting_ticks = 10
                elif -100 < error_heading:
                    pid_turn.update(error_heading, delta_time)
                    print("go left: ", pid_turn.output)
                    delta_time = pid_turn.output

                    spinMotors(-motorSpeed1, -motorSpeed1, motorSpeed1, motorSpeed1)
                    time.sleep(abs(pid_turn.output))
                    spinMotors(0, 0, 0, 0)
                    # we dont want to count rotation wheel movements
                    travel_mapping.reset_odometer()

                    spinMotors(motorSpeed2, motorSpeed2, motorSpeed2, motorSpeed2)
                    time.sleep(0.5)
                    delta_time += 0.5
                    spinMotors(0, 0, 0, 0)

                    waiting_ticks = 10
                else:
                    delta_time = 0
                    pid_turn.update(error_heading, delta_time)
                    print("go right: ", pid_turn.output)
                    delta_time = pid_turn.output
                    delta_time += 0.5

                    spinMotors(motorSpeed1, motorSpeed1, -motorSpeed1, -motorSpeed1)
                    time.sleep(abs(pid_turn.output))
                    spinMotors(0, 0, 0, 0)

                    # we dont want to count rotation wheel movements
                    travel_mapping.reset_odometer()

                    spinMotors(90, 90, 90, 90)
                    time.sleep(0.5)
                    spinMotors(0, 0, 0, 0)
                    waiting_ticks = 10

            else:
                pass
        waiting_ticks = waiting_ticks - 1
        if waiting_ticks < 0:
            waiting_ticks = 0

        key = cv2.waitKey(1) & 0xFF

        # if the 'q' key is pressed, stop the loop
        if key == ord("q"):
            break

        # cleanup the camera and close any open windows


try:
    move()
except KeyboardInterrupt:
    spinMotors(0, 0, 0, 0)
    travel_mapping.save_travel_history()
