# Nhà thông minh freeRTOS

## Cài đặt

1. Clone mã nguồn từ github về máy tính:

	```bash
	git clone https://github.com/HM-Huong/smart-home-freeRTOS.git final-project
	cd final-project
	```

2. Mở file `final-project.ino` bằng Arduino IDE và chạy chương trình.

**Chú ý**: Tên của thư mục gốc của dự án phải là `final-project` để có thể chạy được bằng arduino IDE.

## Sơ đồ khối

![Sơ đồ khối](./attachments/so-do-khoi.drawio.png)

## Sơ đồ nối chân

Các chân được sử dụng trong dự án được định nghĩa ở trong file [global.h](./global.h).

![Sơ đồ nối chân](./attachments/so-do-noi-day.png)

## Lưu đồ thuật toán

### [loopTask](./final-project.ino)

![Lưu đồ thuật toán của loopTask](./attachments/loopTask.drawio.png)

### [printTask](./printTask.cpp)

![Lưu đồ thuật toán của printTask](./attachments/printTask.drawio.png)

### [cloudTask](./cloudTask.cpp)

![Lưu đồ thuật toán của cloudTask](./attachments/cloudTask.drawio.png)

### [rfidTask](./rfidTask.cpp)

![Lưu đồ thuật toán của rfidTask](./attachments/rfidTask.drawio.png)

### [dhtTask](./dhtTask.cpp)

![Lưu đồ thuật toán của dhtTask](./attachments/dhtTask.drawio.png)

### [doorTask](./doorTask.cpp)

![Lưu đồ thuật toán của doorTask](./attachments/doorTask.drawio.png)

### [flameSensorTask](./flameSensorTask.cpp)

![Lưu đồ thuật toán của flameSensorTask](./attachments/flameSensorTask.drawio.png)
