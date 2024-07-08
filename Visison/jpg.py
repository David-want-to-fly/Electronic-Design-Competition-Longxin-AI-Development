import cv2
import os


def extract_frames(video_path, output_folder, frame_rate=1):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    cap = cv2.VideoCapture(video_path)
    fps = cap.get(cv2.CAP_PROP_FPS)  # 获取视频的帧率
    frame_interval = int(fps / frame_rate)  # 计算截取的时间间隔

    count = 0
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        if count % frame_interval == 0:
            frame_path = os.path.join(output_folder, f'frame2_{count:04d}.jpg')
            cv2.imwrite(frame_path, frame)
        count += 1

    cap.release()


# 使用示例
video_path = 'C:Users/94489/Desktop/images/video3.mp4'
output_folder = 'C:Users/94489/Desktop/images'
extract_frames(video_path, output_folder, frame_rate=1)
