微小卫星项目
=======
  项目旨在实现利用WIFI实现微小卫星间的信息沟通，研究以后小卫星群之间的通信问题
#### 项目配置  
1. QT  
   - 版本 QT5.6.3
   - 编译器 MinGW 4.9.2 32bit
   - [项目地址](https://github.com/VictorySong/ls_android)
2. Android Studio  
   - minSdkVersion 19
   - [项目地址](https://github.com/VictorySong/ls)
#### 数据结构  
- 认证  
   - 手机端向服务端发送字符 "phone" 
   - 卫星端的数据客户端向服务端发送  {"id":"2334","type":"client","secret":"jfksjflk"}
   - 卫星的文件客户端向服务端发送  {"id":"2334","type":"clientfile"}
   - 注意：  
       - 只有在数据通道建立完成后文件传输通道才能建立成功  
       - 手机端接入认证成功后服务端不会发送任何消息，其他客户端认证成功后服务端会发送字符串 "allow"
- 数据传送  
   - 位置信息
       - 数据客户端->服务端  
       {"type":"location","x":12,"y":14}
       - 服务端->其他客户端  
       {"ip":"xxx.xxx.xxx.xxx","port":8948,"id":"2334","type":"location","x":"12","y":"14"}
       - 服务端->手机端  
       {"ip":"xxx.xxx.xxx.xxx","port":8948,"id":"2334","type":"location","x":"12","y":"14","R":222,"G":222,"B":222}
   - 状态信息(一个卫星客户端断连，服务端通知手机端)  
   {"id":"2334","type":"status",status":0}  
   - 文件传送  
      - 文件头  
      文件总大小(qint64)+文件名大小(qint64)+文件名
   - 命令(服务端->卫星客户端)
   {"type":"order","order":"videostart"}
   
