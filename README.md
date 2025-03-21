# GameAT  

## I. Thông Tin Cá Nhân  
- **Họ và Tên:** Võ Hoài Anh Tuấn  
- **Mã Sinh Viên:** 24020348  
- **Lớp:** INT2215_4  

## II. Giới Thiệu Chủ Đề Game  
*"Journey in the Dark"* là một trò chơi phiêu lưu kịch tính, nơi người chơi phải vượt qua một mê cung bí ẩn trong bóng tối. Nhiệm vụ chính là ghi nhớ đường đi, né tránh chướng ngại vật và tìm đến kho báu trong thời gian giới hạn. Trò chơi kết hợp yếu tố trí nhớ, chiến thuật và tốc độ, mang đến trải nghiệm đầy thử thách và hấp dẫn.  
### Cách chơi
#### 1.Menu
- Phím K: Bật/tắt âm thanh nền menu
- Phím ESC: Thoát game
- Play: Chơi từ đầu
- Continue: Chơi tiếp mà không reset game
- Exit: Thoát game
#### 2.Game
- Người chơi có 90s để ghi nhớ map (không được di chuyển) sau 90s map sẽ tắt và người chơi có 120s để tìm kho báu.
- Màu trắng là cỏ, màu đen là tường, kho báu ở giữa map
- Nhấn phím UP để đi lên, DOWN để đi xuống, LEFT để đi sang trái, RIGHT để đi sang phải.
- Phím P: Để quay lại menu
## III. Ngưỡng Điểm Đề Xuất  
**Mức điểm mong muốn:** *9 - 10*  

## IV. Lý Do Đề Xuất Ngưỡng Điểm  
### **1. Ý tưởng và sáng tạo**  
- Game được lên ý tưởng hoàn toàn từ bản thân, không sao chép từ các trò chơi khác.  
- Kết hợp nhiều yếu tố: trí nhớ, chiến thuật, tư duy và phản xạ nhanh.  

### **2. Các tính năng đã hoàn thiện**  
-  **Âm thanh:** Nhạc nền, âm thanh click nút, âm thanh khi thắng/thua
-  **Hình ảnh:** Menu, nhân vật, các vật phẩm (màu đen,trắng,kho báu)
-  **Hiệu ứng:**  nút ấn

### **3. Các thuật toán đã cài đặt**  
#### *Menu.cpp*:
-  **Xử lý trạng thái game:** Kiểm tra va chạm chuột với các nút (Play, Continue, Exit). 
-  **Phát nhạc nền:** Chạy nhạc nền khi vào menu và tắt khi vào game. 
- **Phát âm thanh nút bấm**: hiệu ứng âm thanh khi nhấn nút.
#### *Character.cpp*:
- **Di chuyển nhân vật:** Cập nhật vị trí dựa vào phím điều khiển.
- **Kiểm tra va chạm:** Phát hiện nhân vật có chạm vào tường hoặc kho báu hay không. 
- **Cập nhật và reset vị trí:** cập nhật vị trí nhân vật sau mỗi lần di chuyển và thiết lập lại khi người chơi thua hoặc restart.
- **Đọc và ghi dữ liệu nhân vật :** lưu dữ liệu nhân vật khi thoát
#### *map.cpp*:
- **Vẽ bản đồ:** Đọc dữ liệu từ file và vẽ map.
- **Kiểm tra va chạm:** Phát hiện nhân vật có chạm vào tường hoặc kho báu hay không. 
- **Ẩn/hiện bản đồ:** Hiển thị bản đồ trong 30 giây để ghi nhớ, sau đó tắt đi để tăng độ khó.  
#### *timeManager.cpp*:
-  **Đếm ngược thời gian:** 
-  **kiểm tra trạng thái thời gian:** Sau thời gian ghi nhớ, bản đồ sẽ ẩn đi và người chơi phải dựa vào trí nhớ để tìm kho báu.
## V. Nguồn Tham Khảo  
- **hình ảnh:** 
+ *Menu: * https://images.gamebanana.com/img/ss/mods/6302c5e6c248d.jpg  
+ *Nhân vật: * https://i.pinimg.com/originals/4a/6c/06/4a6c064c30d2346481000e1cf07ce952.png
- **âm thanh:** https://pixabay.com/vi/sound-effects/search/game/

## VI. Điểm Nổi Bật  
- **Cơ chế ẩn/hiện bản đồ:** Người chơi phải ghi nhớ đường đi trước khi màn hình tối lại.  
- **Hệ thống thời gian tạo áp lực:** Người chơi cần tìm kho báu trước khi hết giờ. 
- **Hệ thống lưu dữ liệu:** Người chơi không lo lắng khi thoát game 

