#ubuntu打不开ch340串口
## /dev找不到ttyUSB0:
    sudo dmesg | grep brltty
    如果你能收到这样的结果

    [ 7033.078452] usb 1-13: usbfs: interface 0 claimed by ch341 while 'brltty' sets config #1

    问题就很明显了，是驱动占用问题

    sudo apt remove brltty
##串口打不开:
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
