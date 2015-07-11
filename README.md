## Typing-Assistant ##
###1 功能说明
&emsp;&emsp;可以实现局域网内所有登录用户的打字速度比拼练习，实时显示当前登录用户的打字的各种状况和显示打字速度排名，很好地提高打字练习的兴趣。
###2 工程说明
####2.1 开发工具和运行环境
	工具：Source Insight、gcc编译器
	环境：Linux系统、gtk2.0
####2.2 工程目录

- files	：存放打字练习的对照文本
- images：存放工程所需图片
- include：存放头文件
- music	：存放声音文件
- Screen-Shots：软件运行部分截图
- SI：source insight工程目录
- src：源文件、makefile
###3 版本 and Bug
	Version	：0.0.9
	2013年4月3日 
		完成初版，后断断续续更新了几次；
	2014年4月1日
		1、增添了自动获取广播地址
		2、去除了只能在终端登录的缺点
	2014年4月10日
		1、Gdk:ERROR:/build/buildd-gtk+2.0_2.24.10-2-armhf-GEXIqc/gtk+2.0-2.24.10/gdk/...
		2、GTK是一个API库，以前应该是基于X api的，现在GTK重写了，更好用，结构没有变，只保留了一部分必要的X函数。很多应用程序用到了X的api，直接的或者间接的，但是由于GTK不再完全依赖于X api， 而这些应用又都是基于GTK的，所以当应用程序使用X的函数时会出现问题。一切问题都是由于 GTK的重写引起的。所以老的版本不会有这样的问题。GTK为了解决兼容的问题，增加了一个选项GDK_NATIVE_WINDOWS=1，这样就可以使 用原始的GTK环境了。
	2014年4月11日
		1、新增自动检测用户是否离线功能，防止异常退出而不能检测到用户已经离线（对status每隔2秒加加，每隔两秒对接收到的用户的status置1，如果status大于2则说明该用户已经离线了。）
		2、解决了异常关闭时程序仍在后台运行的bug
###注：此工程上传后没更新了，也不会更新了。
###&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;FreeApe 
###&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;2015年7月11日