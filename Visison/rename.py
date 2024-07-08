import os

def rename_images(directory):
    # 获取指定目录下所有文件，并按文件名排序
    files = sorted(os.listdir(directory))
    # 初始化计数器
    count = 1
    # 遍历文件列表
    for filename in files:
        # 构建完整的文件路径
        old_path = os.path.join(directory, filename)
        # 检查是否为文件
        if os.path.isfile(old_path):
            # 构建新的文件名和路径
            new_path = os.path.join(directory, f"{count}.jpg")
            # 重命名文件
            os.rename(old_path, new_path)
            # 更新计数器
            count += 1
    print(f"All images in '{directory}' have been renamed.")

# 使用示例
directory_path = 'C:Users/94489/Desktop/images'
rename_images(directory_path)
