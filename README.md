## **[HỆ ĐIỀU HÀNH] ĐỒ ÁN 3: TÌM HIỂU VÀ LẬP TRÌNH LINUX KERNEL MODULE**
> **Thành viên trong nhóm:**
* 19127041 - Lê Thị Phương Linh
* 19127246 - Trần Văn Qúy Phước
* 19127250 - Phạm Hồng Quân

> **Tìm hiểu sơ nét về Linux kernel module:**
* Linux được thiết kế để làm việc với hàng tỉ thiết bị. Nhưng ta không thể đưa tất cả các driver vào trong kernel được, vì sẽ làm cho kích thước kernel rất lớn. Giải pháp cho vấn đề này đó là: thiết kế các driver dưới dạng module tách rời với kernel. Trong quá trình hoạt động, driver nào cần thiết sẽ được lắp vào kernel, còn driver nào không cần thiết sẽ bị tháo ra khỏi kernel (dynamic loading). 
* Linux kernel module là một file với tên mở rộng là (.ko). Nó sẽ được lắp vào hoặc tháo ra khỏi kernel khi cần thiết. Chính vì vậy, nó còn có một tên gọi khác là loadable kernel module. Một trong những kiểu loadable kernel module phổ biến đó là driver.

> **Yêu cầu**
* Viết một module dùng để tạo ra số ngẫu nhiên.
* Module này sẽ tạo một character device để cho phép các tiến trình ở userspace có thể open và read các số ngẫu nhiên.
* Báo cáo
* **Hoàn thành: 100%**

> **Mô tả thực hiện**
> > Tạo File <code>RandomNum.c</code>:
* File này chữa mã nguồn để tạo một kernel module. Trong file này, sẽ chứa module dùng để tạo ra số ngẫu nhiên. Module này sẽ tạo ra một character device cho phép các tiến trình ở userspace có thể open và read các số ngẫu nhiên. Gồm các hàm sau:
1. <code>static struct file_operations RandomNum_fops</code>: Cấu trúc này khai báo các thao tác muốn dùng đối với file thiết bị như open, read, close.
2. <code>static int _open(struct inode *i, struct file *f)</code>: Hàm thực hiện mở file thiết bị.
3. <code>static int _close(struct inode *i, struct file *f)</code>: Hàm thực hiện đóng file thiết bị.
4. <code>static ssize_t _read(struct file *f, char __user *buf, size_t len, loff_t *off)</code>: Hàm thực hiện đọc dữ liệu từ file thiết bị. Trong hàm này sẽ dùng hàm get_random_bytes(&res, sizeof(res)) để sinh số ngẫu nhiên từ character device và trả số lại từ hàm copy_to_user(buf, &res, sizeof(res)) cho người dùng (user).
5. <code>static int __init ofcd_init(void)</code>: Hàm khởi tạo module driver. Đầu tiên, trong hàm này sẽ thực hiện việc cấp phát động số hiệu thiết bị. Sau đó, tạo lớp thiết bị (device class). Sau đó, thông tin (major, minor) của thiết bị trong lớp này được tạo ra bởi hàm API. Cuối cùng, khởi tạo một cấu trúc thiết bị kiểu character bằng cách khai báo biến cấu trúc struct cdev c_dev và gọi hàm cdev_init(). Điều khiển cấu trúc này đến hệ thống file ảo VFS bằng cách gọi hàm cdev_add().
6. <code>static void __exit ofcd_exit(void)</code>: Hủy class, gỡ bỏ các module, device.
> > Tạo File <code>Test.c</code>: File mã nguồn bên người dùng để thực hiện mở, đóng file thiết bị, đọc các số ngẫu nhiên và xuất ra console.
* Để biên dịch kernel module, ta sử dụng phương pháp Kbuild. Theo phương pháp này, chúng ta cần tạo ra 2 file: một file có tên là Makefile, file còn lại có tên là Kbuild. 

> **Biên dịch**
> > Các bước thực hiện:
1.	Biên dịch file <code>RandomNum.c</code> sử dụng Makefile, tạo ra file <code>RandomNum.ko</code> và file thực thi Test (chạy bên user). 
2.	Lắp kernel module vào sử dụng lệnh insmod.
3.	Kiểm tra chức năng character device bằng đọc dữ liệu từ file /dev/ randomNum.
4.	Chạy file thực thi Test bằng lệnh sudo <code>./Test</code>, lúc này nếu mở file thiết bị thành công thì console sẽ xuất ra mở file thành công và số ngẫu nhiên.
5.	Tháo module ra khỏi kernel bằng cách sử dụng lệnh rmmod và thực hiện make clean các file.

> > Kết quả thực hiện:
***Hình ảnh thực hiện trong phần báo cáo***


