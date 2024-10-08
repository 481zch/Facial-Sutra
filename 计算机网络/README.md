#### 1. OSI七层网络模型

```html
1. 应用层
	为应用程序提供服务。
	DNS, HTTP, HTTPS, FTP, SMTP
2. 表示层
	解压密，数据转换。
3. 会话层
	建立、管理和维护会话。
4. 运输层
	数据分段、传输和重组。
	TCP, UDP
5. 网络层
	IP选址以及路由选择。
	IP
6. 数据链路层
	提供介质访问和链路管理。
7. 物理层
	设备之间比特流的传输，数字信号和电信号之间的转换。
```

#### 2. TCP/IP 四层网络模型

```html
1. 应用层
	包含应用软件和各种应用层协议：HTTP(S), FTP, SMTP。
2. 运输层
	TCP和UDP协议。
3. 网络层
	IP, ARP, ICMP
4. 数据链路层
	MAC
```

#### 3. 三次握手和四次挥手

```html
三次握手：
	(0) 初始状态服务端和客户端都处于closed状态，服务器进入listen，等待客户端的连接。
	(1) 客户端调用connect，向服务端发起连接请求。
	(2) 第一次握手
			客户端发送一个带有SYN标志的报文，报文中序列号随机生成，客户端进入SYN_SENT状态。
			生成随机初始序列号seq = x。
	(3) 第二次握手
			服务器收到客户端的SYN报文后，确认连接请求。
			服务器返回一个SYN-ACK报文，SYN置1，ACK置1，表示对客户端的SYN进行确认，服务器进入SYN_RCVD状态。
			报文中的确认应答ack = x + 1，同时服务器生成随机序列号seq = y。
	(4) 第三次握手
			客户端收到服务器的SYN-ACK报文后，发送一个ACK报文来确认，ACK报文中确认应答号为ack = y + 1。
			进入established状态，表示已经建立连接。
			服务器接收到客户端的ACK报文后，进入established状态，三次握手完成，开始传输数据。

四次挥手：
	(1) 第一次挥手
			客户端主动关闭连接，发送FIN报文，表示已经没有数据要发送给服务器了。
			客户端进入FIN_WAIT_1状态。
	(2) 第二次挥手
			服务端接收到客户端的FIN报文后，发送一个ACK报文，确认客户端的FIN，此时，服务器还可以继续发送数据给客户端。
			服务器进入CLOSE_WAIT状态，等待服务器关闭连接。
	(3) 第三次挥手
			服务器不需要再发送数据时，服务器发送FIN报文，告诉客户端服务端没有数据要发送。
			服务器进入LAST_ACK状态，等待客户的确认。
	(4) 第四次挥手
			客户端收到服务器的FIN报文，此时，客户端进入TIME_WAIT状态，等待一段时间，通常是2MSL（两个最大报文生存时间）确保
			服务器收到了客户端的ACK，然后真正关闭。
			服务器收到客户端的ACK后，直接进入CLOSED状态。
			客户端在2MSL后，进入CLOSED状态。
```

#### 4. TCP的流量控制

```html
防止发送端以过快的速度发送数据，导致接收端的缓冲区溢出。
流量控制确保了发送端不会超出接收端的处理能力，从而避免了数据丢失或者缓冲区溢出。
实现方式：滑动窗口
    TCP中的滑动窗口指的是发送端允许一次性发送的数据量，而不必等待确认。
    每次接收端会告诉它的接受窗口大小，即能够接受的数据量。
	发送端根据接收端的反馈，调整发送窗口的大小，确保不超过接收端的处理能力。
	窗口缩小：
		如果接收端缓冲区快满了，会将窗口缩小，告知发送端减少数据发送速度。
	零窗口：
		如果接收端的缓冲区已经满了，接收端会将窗口设置为0，通知发送端暂时停止发送数据，直到接收端能够处理更多的数据为止。
```

#### 5. TCP的拥塞控制

```html
拥塞控制是为了防止网络出现过载的机制，目的是确保网络不会因为发送的数据量过大而导致网络拥塞。
TCP 拥塞控制的四个阶段：
1. 慢启动（Slow Start）：
    刚开始发送数据时，TCP 不知道网络的带宽，因此采用一种保守的方式，初始发送窗口 cwnd 通常设为 1 或 2 个最大报文段（MSS）。
    每次收到一个 ACK，cwnd 加倍，呈指数级增长，直到到达一个预设的阈值 ssthresh。
2. 拥塞避免（Congestion Avoidance）：
    当 cwnd 达到阈值 ssthresh 后，进入拥塞避免阶段。
    在这个阶段，每次收到 ACK，cwnd 线性增加，而不是指数增加，以避免网络过度拥塞。
3. 快速重传（Fast Retransmit）：
	如果发送方在收到三个重复的 ACK（表示某个数据包丢失）时，立即重传丢失的数据包，而不必等待超时。
4. 快速恢复（Fast Recovery）：
    进入快速重传后，TCP 不会直接进入慢启动，而是会将 ssthresh 设置为当前 cwnd 的一半，然后进入拥塞避免阶段。
    这种机制减少了慢启动的影响，加快了网络的恢复。
    拥塞窗口（Congestion Window, cwnd）：除了接收窗口 rwnd 之外，TCP 还引入了拥塞窗口 cwnd，由发送方控制，表示当前网络条件	  下可以发送的数据量。实际的发送窗口大小由 rwnd 和 cwnd 共同决定，取其中较小值。

网络拥塞信号：
超时：发生超时时，表示网络可能拥塞，发送方会进入慢启动阶段，重置拥塞窗口 cwnd。
重复 ACK：若收到三个重复的 ACK，表明可能发生了轻微的网络拥塞，发送方会立即重传丢失的数据段，而不会等待超时。
```

#### 6. TCP的超时重传

```html
定义：超时重传是指当 TCP 发送数据后，未能在指定的时间内收到确认（ACK），则认为该数据包可能丢失，需要重新发送。
关键机制：
重传超时时间（RTO, Retransmission Timeout）：RTO 是 TCP 等待 ACK 的时间。如果超过了这个时间还没有收到 ACK，TCP 将认为该数据包丢失，并重新发送数据。
RTO 由 RTT（Round Trip Time，往返时间） 估计得出。
TCP 不断监控每个数据包的 RTT，并使用 加权平均算法 计算一个估计的 RTT 值。
指数退避机制：如果发生超时，TCP 会将 RTO 加倍。每次重传失败，RTO 值都会加倍，直到达到某个上限，逐步减小对网络的负载。
快速重传：如果接收端收到的数据包有丢失，会通过重复的 ACK 通知发送方。发送方收到三个重复的 ACK 后，会立即重传丢失的数据包，而不必等待超时。
```

#### 7. TCP和UDP的区别

```html
1. 连接与无连接
    TCP：面向连接的协议。在传输数据之前，TCP 需要通过 三次握手 建立连接。确保数据的可靠传输需要先建立和维护连接。
    UDP：无连接的协议。在传输数据之前，不需要建立连接，数据直接发送，不关心对方是否收到，也不保证顺序。
2. 可靠性
    TCP：提供可靠的数据传输。TCP 使用流量控制、拥塞控制和超时重传来确保数据正确到达接收方，保证数据的完整性、顺序和无丢失。
    通过序列号和确认机制来确保数据包的顺序和完整性。
    如果数据在传输中丢失，TCP 会自动重传。
	UDP：不提供可靠的数据传输。UDP 不进行重传，也没有确认机制。一旦数据发出，无法得知是否成功到达目标。适合不需要高可靠性的场景。
3. 数据传输顺序
    TCP：TCP 保障数据的顺序到达。发送的数据包会被按顺序编号，接收端也按顺序重组数据包。
    UDP：UDP 不保证数据的顺序到达。数据报是独立的，如果网络状况不好，数据可能会乱序到达。
4. 数据传输方式
    TCP：TCP 是面向字节流的协议。数据会被分成多个段来传输，接收方会将这些段重组为原始数据流。
    UDP：UDP 是面向数据报的协议。UDP 把数据看作独立的数据报文，发送方发送的每个数据报文都是完整的、独立的单位。
5. 速度
	TCP：由于要保证可靠性，TCP 的速度相对较慢。它需要进行三次握手、确认数据包、重传丢失的数据包，并进行流量控制和拥塞控制，增加了传输的延时。
	UDP：速度较快。UDP 不保证数据的可靠性和顺序，也没有复杂的连接建立过程，因此在传输速度上比 TCP 更快。
6. 头部开销
    TCP：头部开销较大。TCP 数据包的头部最小为 20 字节，包含了序列号、确认号、窗口大小等信息，用于确保数据可靠传输。
    UDP：头部开销小。UDP 头部只有 8 字节，包含源端口、目标端口、长度和校验和等基本信息。
7. 流量控制和拥塞控制
    TCP：支持流量控制和拥塞控制。TCP 会根据网络条件调整发送数据的速度，避免拥塞，保证网络的稳定性。
    UDP：不支持流量控制和拥塞控制。UDP 不关心网络的状况，它只是发送数据，不管对方是否能处理。
8. 应用场景
    TCP：适用于对数据可靠性要求高、顺序性要求严格的应用场景，例如：
        HTTP/HTTPS：用于网页浏览，要求数据的完整性和可靠性。
        FTP：文件传输协议，要求数据不能丢失和出错。
        SMTP：电子邮件传输，要求数据可靠传输。
    UDP：适用于对速度要求高，但对可靠性要求不高的场景，例如：
        视频直播、在线游戏：要求低延迟的场景，即使部分数据丢失，整体也能正常进行。
        DNS：域名解析服务，查询响应时间短且不需要复杂的连接。
        DHCP：动态主机配置协议，用于分配 IP 地址等配置。
9. 拥塞控制
    TCP：TCP 有拥塞控制机制，会检测网络的拥塞情况，自动调整传输速率，以避免网络过载。
    UDP：UDP 不具备拥塞控制机制，它会尽可能快地发送数据，不管网络状况如何，这可能导致网络拥塞。
10. 广播和组播
    TCP：不支持广播或组播，TCP 是点对点的协议，主要用于一对一的通信。
    UDP：支持广播和组播，适合在局域网中发送同一数据到多个接收者（如 IPTV 组播传输）。
```

#### 8.从输入网址到界面的流程

```html
URL解析：浏览器根据用户输入的URL，解析出协议（例如HTTP或HTTPS）、域名、端口号（如果有）、路径以及查询参数等信息。

DNS解析：浏览器向本地DNS服务器发送一个DNS查询请求，查询目标域名对应的IP地址。如果本地DNS服务器中不存在对应的IP地址，则会向上级DNS服务器发送查询请求，直到找到目标域名的IP地址为止。

建立TCP连接：浏览器通过解析得到的IP地址和端口号，与服务器建立TCP连接。这个过程包括三次握手，即客户端发送SYN包，服务器返回SYN+ACK包，最后客户端发送ACK包。

发起HTTP请求：建立TCP连接后，浏览器会向服务器发送一个HTTP请求，请求服务器提供对应URL的资源（通常是HTML页面）。

服务器处理请求：服务器接收到浏览器发送的HTTP请求后，会根据请求的内容进行处理，然后将处理结果（通常是HTML文档）作为HTTP响应返回给浏览器。

接收响应并渲染页面：浏览器接收到服务器的响应后，会解析HTML文档，然后根据HTML中的内容、样式表、JavaScript脚本等资源，渲染出页面的结构、样式和交互效果。

关闭TCP连接：页面渲染完毕后，浏览器会关闭与服务器之间的TCP连接。

显示页面：最后，浏览器会将渲染好的页面显示给用户。
```

#### 9. HTTP和HTTPS的区别

![img](https://uploadfiles.nowcoder.com/images/20231010/666389852_1696946717098/D2B5CA33BD970F64A6301FA75AE2EB22)

#### 10.什么是cookie和session

```html
Cookie实际上是一小段的文本信息。客户端请求服务器，如果服务器需要记录该用户状态，就使用response向客户端浏览器颁发一个Cookie。客户端会把Cookie保存起来。
当浏览器再请求该网站时，浏览器把请求的网址连同该Cookie一同提交给服务器。服务器检查该Cookie，以此来辨认用户状态。服务器还可以根据需要修改Cookie的内容。
信息保存的时间可以根据需要设置.

Session是另一种记录客户状态的机制，不同的是Cookie保存在客户端浏览器中，而Session保存在服务器上。客户端浏览器访问服务器的时候，服务器把客户端信息以某种形式记录在服务器上。这就是Session。客户端浏览器再次访问时只需要从该Session中查找该客户的状态就可以了。
每个用户访问服务器都会建立一个session，那服务器是怎么标识用户的唯一身份呢？事实上，用户与服务器建立连接的同时，服务器会自动为其分配一个SessionId。

1、数据存储位置：cookie数据存放在客户的浏览器上，session数据放在服务器上。
2、安全性：cookie不是很安全，别人可以分析存放在本地的cookie并进行cookie欺骗，考虑到安全应当使用session。
3、服务器性能：session会在一定时间内保存在服务器上。当访问增多，会比较占用你服务器的性能，考虑到减轻服务器性能方面，应当使用cookie。
4、数据大小：单个cookie保存的数据不能超过4K，很多浏览器都限制一个站点最多保存20个cookie。
5、信息重要程度：可以考虑将登陆信息等重要信息存放为session，其他信息如果需要保留，可以放在cookie中。
```

#### 11.HTTP常见状态码

```html
1XX		正在处理
200		OK
204		成功但是没有结果返回
206		断点续传
301		永久重定向
302		临时重定向
400		请求报文存在语法错误
401		需要认证或者认证失败
403		请求被服务器拒绝
404		服务器上无法找到请求的资源
500		服务端执行请求时出现了错误
503		服务器正在超负载或者停机维护，无法处理请求
```

#### 12. GET和POST的区别

![tubian](https://i-blog.csdnimg.cn/blog_migrate/20e46c263e79859d80614a6be9a300a6.png)

#### 13.对称加密和非对称加密

```html
对称加密和非对称加密是网络安全中最常用的两种加密方式。

对称加密：
对称加密又称为共享密钥加密，指使用同一把密钥进行加密和解密。在对称加密过程中，发送者和接收者之间必须共享相同的密钥，这样发送者可以使用该密钥加密消息，而接收者则使用相同的密钥解密接收到的消息。对称加密算法包括 DES、3DES、AES 等，它们广泛应用于保证网络通信的安全性，如 HTTPS 协议就是一种基于对称密钥的加密机制。

举例来说，假设 Alice 想要给 Bob 发送一个加密信息。Alice 使用协商好的密钥对信息进行加密，然后将其发送给 Bob，Bob再使用相同的密钥进行解密。

非对称加密：
非对称加密也称为公私钥加密，使用不同的密钥进行加密和解密。每个用户都有自己的一对密钥（即公钥和私钥），公钥可以通过任何渠道向用户分发，私钥则只有用户本人知道。在非对称加密过程中，发送者使用接收者的公钥进行加密，接收者再使用自己的私钥进行解密。

RSA 是一种非对称加密算法，其涵盖了数字签名、证书、密钥交换等安全技术，因此被广泛应用于网络通信中。

举例来说，假设 Alice 要给 Bob 发送一个加密的消息。首先，Bob 生成自己的一对密钥：公钥和私钥，将公钥发送给 Alice；接下来，Alice 使用 Bob 的公钥对消息进行加密并发送，只有 Bob 才能使用自己的私钥解密信息。

对称加密简单高效，但存在密钥密级管理困难的问题；而非对称加密可以实现更好的保密性和认证性，但计算复杂度较高，适合处理较小规模数据。
```

#### 14. https的加密过程

```html
ClientHello
首先，由客户端向服务端发起加密通信请求。客户端主要向服务端发送：

客户端支持的 SSL/TLS协议版本
客户端产生的的随机数(Client Random）
客户端支持的密码套件列表
#SeverHello
服务器收到客户端请求后，向客户端发出响应。服务端回应的内容有：

确认 SSL/ TLS 协议版本(如果浏览器不支持，则关闭加密通信）
服务端生产的随机数(Server Random）
确认的密码套件列表
服务端的数字证书
#客户端回应
客户端收到服务端的回应之后，首先通过浏览器或者操作系统中的 CA 公钥，确认服务端的数字证书的真实性。如果证书没有问题，客户端会从数字证书中取出服务端的公钥，然后使用它加密报文，向服务端发送如下信息：

一个随机数，该随机数会被服务端公钥加密
加密通信算法改变通知，表示随后的信息都将用会话秘钥加密通信
客户端握手结束通知，表示客户端的握手阶段已经结束
之前所有内容的发生的数据做个摘要，用来供服务端校验
服务端和客户端有了三个随机数，接着用双方协商的加密算法，各自生成本次通信的会话秘钥。

#服务端回应
服务端收到客户端的第三个随机数(pre-master key）之后，通过协商的加密算法，计算出本次通信的会话秘钥。服务端向客户端发送最后的信息：

加密通信算法改变通知，表示随后的信息都将用「会话秘钥」加密通信
服务端握手结束通知，表示服务端的握手阶段已经结束
之前所有内容的发生的数据做个摘要，用来供客户端校验
接下来，客户端与服务器进入加密通信，就完全是使用普通的 HTTP 协议。
```

```markdown
#### 关于https中的加密理解
在http中
```





