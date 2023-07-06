ffmpeg -re -stream_loop -1 -i "./test.mp4" -vcodec h264 -acodec aac -f rtsp -rtsp_transport tcp rtsp://127.0.0.1/live/test

