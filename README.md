# lottery-management-system-of-c
一个main从头写到尾，问为什么不加预处理命令，问就是不需要

初始登录界面没有管理员登录和公证员登录，
登录方式为在初始界面输入选项填123456进入管理员界面
输入654321进入公证员登录界面
管理员初始密码为admin
公证员初始密码为gongzheng
上述功能可在头文件中修改
规则说明：
一共7个号码，号码范围1～10,号码可重复
		仅中一个号码返还单价*5*倍数			买一注两元中1个号码 返10元
		仅中2个号码返还单价*10*倍数					     20
		仅中3个号码返还单价*20*倍数					     40
		仅中4个号码返还单价*20*2*倍数				     80
		仅中5个号码返还单价*20*4*2*倍数				     320
		仅中6个号码返还单价*20*4*8*2*倍数				     2560
		全中返还单价*20*4*8*16*2*倍数（若单价为2）			     40960
规则在void kaijiang_fun(struct caipiao *p);中
此规则下（2元）中将期望为0.660219元 嘎嘎良心有没有



注册的帐号和发布的彩票等信息可在管理员操作系统中保存入本地文件中
也可以在管理员操作系统中选择读取本地文件中的信息

扣费程序未具体实现



麻雀虽小，五脏俱全， 里面包括用户操作系统、管理员操作系统、公证员操作系统、常见的注册、登录、界面、查找、排序、链表、文件i/o等都有。
不过只是个c语言初学者的练手项目，i/o操作也只在本地运行，看看也就图一乐。

具体实现：操作页面是用cout输出的、用户信息、彩票信息是用结构体+链表进行实现的、注册防止重复是遍历的链表信息、文件i/o用的是fileopen、排序写了个冒泡和选择排序。