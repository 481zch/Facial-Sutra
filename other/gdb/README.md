#### 1. 什么是GDB？主要用于做什么？

```html
GNU的组件，是一个开源调试器，主要用来调试C和C++程序，可以允许用户检查程序的执行过程，设置断点，逐步执行，逐函数执行，调试多线程程序，查看变量值等功能。
```

#### 2. 常用的GDB命令有哪些？

```html
启动命令: gdb myprogram
查看源代码: list
设置断点: break main.c:10
清除断点: clear main.c:10
查看断点: info breakpoints
运行代码: run
显示变量值: print variable_name
数据改变时中断: watch variable_name
单步执行: step
继续执行: continue
退出GDB: quit
查看线程: info threads
切换线程: thread 线程号
跳转执行: jump 10
```

#### 3. 如何使用GDB调试定位内存泄漏问题？

```html
结合valgrind工具的使用，valgrind可以给出详细的泄漏信息，包括没有释放的内存大小，泄漏位置的调用栈。根据给出的信息，结合GDB进行调试进行定位内存泄漏的位置和产生的原因。
```

#### 4. 解释GCC和G++之间的关系和区别

```html
GCC主要用来编译C语言代码，而G++主要用来编译C++代码。GCC虽然可以用来编译C++代码，但是使用G++更加方便，因为G++可以自动的链接标准库，但是GCC不能够自动链接，需要手动链接标准库。此外，G++可以自动识别C++代码，调用对应的编译器。使用G++编译C++代码的时候，后缀名需要修改为.cpp，如果没有修改，而是使用.c作为后缀的话，就会将代码识别为C语言进行代码编译，而不是C++代码。
```

#### 5. 如何在GCC/G++中指定语言版本的标准进行编译？

```html
加上参数: -std=c++11
```

#### 6.GDB如何定位和调试死锁

```html
启动GDB
当陷入死锁，即程序卡住的时候，使用Ctrl + c中断程序。
使用info threads查看所有线程的信息。
切换到不同线程，并且查看调用堆栈。
分析锁的状态，找出哪个线程持有锁以及哪个线程正在等待锁。
```

```cpp
root@ubuntu:/code/deadlock# g++ -std=c++11 -g -o deadlock deadlock.cpp -lpthread
root@ubuntu:/code/deadlock# gdb ./deadlock
GNU gdb (Ubuntu 9.2-0ubuntu1~20.04.2) 9.2
Copyright (C) 2020 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
--Type <RET> for more, q to quit, c to continue without paging--
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./deadlock...
(gdb) run
Starting program: /code/deadlock/deadlock 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
[New Thread 0x7ffff7a4d700 (LWP 2713)]
thread1: trying to lock mutex1
[New Thread 0x7ffff724c700 (LWP 2714)]
thread2: trying to lock mutex2
Thread1: trying to lock mutex2
Thread2: trying to lock mutex1
^C
Thread 1 "deadlock" received signal SIGINT, Interrupt.
__pthread_clockjoin_ex (threadid=140737348163328, thread_return=0x0, clockid=<optimized out>, abstime=<optimized out>, block=<optimized out>)
    at pthread_join_common.c:145
145     pthread_join_common.c: No such file or directory.
(gdb) interrupt
(gdb) info threads
  Id   Target Id                                   Frame 
* 1    Thread 0x7ffff7a4e740 (LWP 2709) "deadlock" __pthread_clockjoin_ex (threadid=140737348163328, thread_return=0x0, clockid=<optimized out>, 
    abstime=<optimized out>, block=<optimized out>) at pthread_join_common.c:145
  2    Thread 0x7ffff7a4d700 (LWP 2713) "deadlock" __lll_lock_wait (futex=futex@entry=0x55555555a1a0 <mutex2>, private=0) at lowlevellock.c:52
  3    Thread 0x7ffff724c700 (LWP 2714) "deadlock" __lll_lock_wait (futex=futex@entry=0x55555555a160 <mutex1>, private=0) at lowlevellock.c:52
(gdb) thread 2
[Switching to thread 2 (Thread 0x7ffff7a4d700 (LWP 2713))]
#0  __lll_lock_wait (futex=futex@entry=0x55555555a1a0 <mutex2>, private=0) at lowlevellock.c:52
52      lowlevellock.c: No such file or directory.
(gdb) bt
#0  __lll_lock_wait (futex=futex@entry=0x55555555a1a0 <mutex2>, private=0) at lowlevellock.c:52
#1  0x00007ffff7f9c0a3 in __GI___pthread_mutex_lock (mutex=0x55555555a1a0 <mutex2>) at ../nptl/pthread_mutex_lock.c:80
#2  0x000055555555576f in __gthread_mutex_lock (__mutex=0x55555555a1a0 <mutex2>) at /usr/include/x86_64-linux-gnu/c++/9/bits/gthr-default.h:749
#3  0x00005555555558c4 in std::mutex::lock (this=0x55555555a1a0 <mutex2>) at /usr/include/c++/9/bits/std_mutex.h:100
#4  0x0000555555555960 in std::lock_guard<std::mutex>::lock_guard (this=0x7ffff7a4cdf0, __m=...) at /usr/include/c++/9/bits/std_mutex.h:159
#5  0x0000555555555420 in thread1 () at deadlock.cpp:16
#6  0x0000555555556682 in std::__invoke_impl<void, void (*)()> (__f=@0x55555556ceb8: 0x555555555368 <thread1()>) at /usr/include/c++/9/bits/invoke.h:60
#7  0x000055555555661a in std::__invoke<void (*)()> (__fn=@0x55555556ceb8: 0x555555555368 <thread1()>) at /usr/include/c++/9/bits/invoke.h:95
#8  0x00005555555565ac in std::thread::_Invoker<std::tuple<void (*)()> >::_M_invoke<0ul> (this=0x55555556ceb8) at /usr/include/c++/9/thread:244
#9  0x0000555555556569 in std::thread::_Invoker<std::tuple<void (*)()> >::operator() (this=0x55555556ceb8) at /usr/include/c++/9/thread:251
#10 0x000055555555653a in std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()> > >::_M_run (this=0x55555556ceb0)
--Type <RET> for more, q to quit, c to continue without paging--
    at /usr/include/c++/9/thread:195
#11 0x00007ffff7e85df4 in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#12 0x00007ffff7f99609 in start_thread (arg=<optimized out>) at pthread_create.c:477
#13 0x00007ffff7cc1353 in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:95
(gdb) q
A debugging session is active.

        Inferior 1 [process 2709] will be killed.

Quit anyway? (y or n) y
root@ubuntu:/code/deadlock# 
```

