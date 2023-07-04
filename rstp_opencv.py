import cv2
import threading
stream_url = "rtsp://127.0.0.1/live/test"

class VideoCapture:
  def __init__(self, url):
    self.cap = cv2.VideoCapture(url)
    self.ret, self.frame = self.cap.read()
    threading.Thread(target=self.update, args=()).start()

  def update(self):
    while True:
      self.ret, self.frame = self.cap.read()

  def get_frame(self):
    return self.ret, self.frame

cap = VideoCapture(stream_url)

while True:
    ret, frame = cap.get_frame()
    if not ret:
        break
    # 使用你的帧...
    cv2.imshow('Frame', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.cap.release()
cv2.destroyAllWindows()
