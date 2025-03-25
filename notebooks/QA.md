# ubuntu打不开ch340串口
## /dev找不到ttyUSB0:
    sudo dmesg | grep brltty
    如果你能收到这样的结果

    [ 7033.078452] usb 1-13: usbfs: interface 0 claimed by ch341 while 'brltty' sets config #1

    问题就很明显了，是驱动占用问题

    sudo apt remove brltty
## 串口打不开:
权限问题

永久打开串口权限
首先查看用户组

 ls -l /dev/ttyUSB0
1
终端输出：

crw-rw-rw- 1 root dialout 188, 0 12月 19 14:47 /dev/ttyUSB0

可以看到用户 root ，所属用户组为 dialout, 因此一种方法是把我们的当前用户名 加入到这个用户组。

   $whoami

    tony

   $sudo usermod -aG dialout tony

## ssh免密码
### 1. 客户端生成公私钥对
```powershell
ssh-keygen -t rsa -b 4096 -f ~/.ssh/id_rsa
```
### 2. 将公钥拷贝到服务器（后缀为 .pub 的文件）
如果服务器没有`~/.ssh`文件夹，则新建，将公钥拷贝到其中
### 3. 在远程 Linux 服务器中追加公钥到文件 authorized_keys 中
```bash
cat id_rsa_.pub >> authorized_keys
```
### 4. 在本地 ssh 配置文件 ~/.ssh/config 中添加如下配置信息
```
Host kerman_nuc10
	HostName 192.168.31.164
	User tony
	IdentityFile "C:\Users\Lenovo\.ssh\id_rsa"
```
* Host：远程服务器的名称
* HostName：远程服务器的 IP 地址
* User：远程服务器的用户名称
* IdentityFile：本地私钥保存路径