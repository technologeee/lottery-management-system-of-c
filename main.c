#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define true 1
#define false 0
#define day_of_1 1000*60*60*24
int user_no_auto=1;		//自动获取的用户编号
char admin_password[20] = "admin";	//管理员密码
int admin_in = 123456;		//隐藏的管理员页面进入
int gongzheng_in = 654321;	//隐藏的公证员页面进入
char gongzheng_password[20] = "gongzheng";	//公证员密码
struct caipiao{				//彩票结构体声明
	long no;				//彩票期号
	int price;				//彩票单价
	int open_close;				//开奖状态 (0为未开奖，1为已开奖)
	int zhongjiang_number[7];		//中奖号码数组
	int sum_price;				//奖池总数
	struct caipiao *next;			//下一期彩票指针
};
typedef struct caipiao Caipiao;				//结构体定义别名
struct caipiao *caipiao_nowsite;			//彩票结构体当前位置指针
struct caipiao *caipiao_head;				//彩票链表头指针

struct user{				//用户数据结构体
	int user_no; 		//用户编号	自动获取
	char user_name[20];	//用户昵称
	char user_password[20]; //用户密码
	int user_money;		//用户余额
	int caipiao_times;	//用户已购买彩票张数
	int ban;		//是否被禁用	0为未禁用 1为已禁用
	struct caipiao *bought[100];	//用户已购买彩票地址
	int bought_num[100][7];		//用户已购买彩票号码
	int caipiao_times_times[100];	//对应第caipiao_times张彩票的倍数
	struct user *next;	//用户链表下一个用户地址
	struct user *last;	//用户链表上一个用户地址
	
};
typedef struct user User;	//user结构提定义别名User
struct user *user_nowsite;		//当前位置指针

struct user *user_head;		//链表头指针



void add_user();			//添加用户
void sign_up_page();			//注册界面
void login_page();			//登录界面
void admin_operating_system();		//管理员操作系统
void faxingcaipiao();			//管理员发行彩票系统
void add_caipiao();			//添加彩票
void admin_search_user();		//管理员系统查找彩民信息
void admin_search_user_byid();		//管理员查找彩民信息分支byid
void display_user(User *site);		//用户信息显示
void admin_search_user_byname();	//管理员查找彩民信息分支byname
void admin_search_user_bymoney();	//管理员查找彩民信息分支bymoney
void admin_paixu();			//管理员排序
void display_all_user(struct user *);	//显示所有用户
void admin_paixu_byno();		//管理员排序分支byno
void admin_paixu_byname();		//管理员排序分支byname
void admin_paixu_bymoney();		//管理员排序分支bymoney
void user_change(User *,User *);	//两个结构体 user 全交换
void admin_save();			//管理员保存数据
void save_user();			//管理员保存数据分支 保存用户信息
void save_caipiao();			//管理员保存数据分支 保存彩票信息
void user_login_page();			//用户登录界面
void user_operating_system(struct user *);//用户操作界面
void display_caipiao(struct caipiao *p);	//花式显示彩票程序
void display_num(int a);		//彩票显示程序子程序
void add_money(struct user *);		//用户充值程序
void buy_caipiao(struct user *);	//用户买彩票程序
int yanzheng_user_password(struct user *); //用户密码验证程序  验证成功返回1,验证失败3次返回0,中途退出返回0
void display_bought_caipiao(struct user *p);	//显示用户已购买彩票
void change_password(struct user *p);		//更改用户密码
void ban_user(struct user *);			//禁用用户24h
void change_password_by_password(struct user *); //用新密码覆盖旧密码	两次验证
int write_off_user(struct user *p);		//注销用户
void gongzheng_operating_system();		//公证员操作系统
void caipiao_kaijiang(struct caipiao *p);	//公证员对彩票开奖
void admin_read();				//管理员读取已保存数据
void read_user();				//管理员读取已保存用户数据
void read_caipiao();				//管理员读取已保存彩票信息
void kaijiang_fun(struct caipiao *p);		//开奖程序
void printf_zhanghao(int);			//输出11位帐号
void gongzheng_operating_system(){
	printf("请输入公证员密码:");
	char try[20];
	int try_times = 0;
	for(;;){
		scanf("%s",try);
		if(strcmp(try,gongzheng_password) != 0 && try_times < 3){
			try_times++;
			printf("密码错误，已输错%d次。请重新输入:\n",try_times);
		}
		if(try_times == 3){
			printf("尝试次数过多！\n");
			login_page();	//循环出口
		}
		if(strcmp(try,gongzheng_password) == 0){
			break;		//循环出口
		}
	}
	printf("========================================================================\n");
	printf("\t\t\t\t公证员操作系统\n");
	printf("========================================================================\n\n\n\n");
	printf("1、彩票开奖\n2、退出\n");
	int i;
	scanf("%d",&i);
	struct caipiao *q;
	long no_temp;
	switch(i){
		case 1:
			printf("请输入彩票期号进行开奖：");
			scanf("%ld",&no_temp);
			q=caipiao_head;
			while(1){
				if(q->no != no_temp){
					q=q->next;
				}
				if(q->no == no_temp){
					caipiao_kaijiang(q);
					break;		//循环出口
				}
			}
			break;		//case 出口
			
		default:
			return login_page();
	}
}
void sign_up_page(){
	printf("========================================================================\n");
	printf("\t\t\t\t注册页面\n");
	printf("========================================================================\n\n\n\n");
	if(user_nowsite != NULL){
		struct user *temp;
		temp = user_nowsite;
		user_nowsite->next=malloc(sizeof(User));	//分配内存
		user_nowsite=user_nowsite->next;		//当前位置跟随移动
		user_nowsite->last=temp;			
		user_nowsite->next=NULL;
		add_user();
		return login_page();
	}
	if(user_nowsite==NULL){
		user_nowsite=malloc(sizeof(User));	//分配内存
		user_head=user_nowsite;			//首地址
		user_nowsite->last=NULL;
		user_nowsite->next=NULL;
		add_user();
		return login_page();
	}
}

void add_user(){
		user_nowsite->ban = 0;
		user_nowsite->user_money = 0;
		user_nowsite->caipiao_times = 0;
		user_nowsite->user_no = user_no_auto++;
		printf("输入昵称：");	
		scanf("%s",user_nowsite->user_name);
		for(;;){
			printf("请输入密码（密码最大为20位由数字、字符、字母组成，密码不小于6位数）：");
			int password_size = 0;
			scanf("%s",user_nowsite->user_password);
			if(strlen(user_nowsite->user_password)<6){
				printf("长度过低，请重新输入密码。\n");
				continue;	
			}else if(strlen(user_nowsite->user_password)>20){
				printf("密码长度大于20位，请检查后再输入。\n");
				continue;
			}else break;			//循环出口
		
		}
		printf("请再次输入密码:");
		char password_2[20];
		scanf("%s",password_2);
		if(strcmp(user_nowsite->user_password,password_2) == 0){
			printf("\n\n\n恭喜您，注册成功！您的帐号为:");		//给出帐号
			printf_zhanghao(user_nowsite->user_no);
			printf("登录时请使用该帐号登录！\n");
		}else{
			printf("两次密码输入不一致。");
			add_user();
		}
		
		
}
void printf_zhanghao(int a){
	int i=0;
	int temp=a;
	for(;;){			//i 为a的位数
		a/=10;
		i++;
		if(a == 0){
			break;
		}
	}
	for(int j=i;j<=11;j++){		//输出11位数，前面用0补齐
		printf("0");
	}
	printf("%d",temp);
}
void login_page(){
	printf("========================================================================\n");
	printf("\t\t\t\t登录页面\n");
	printf("========================================================================\n\n\n\n");
	int login_anser;
	printf("1、用户\n2、注册\n");
	printf("请输入编号（1~2）输入其他任意键退出程序:");
	scanf("%d",&login_anser);
	if(login_anser == 1){
		user_login_page();				//用户登录界面
	}
	if(login_anser == 2){
		sign_up_page();					//注册页面
	}
	if(login_anser == admin_in){
		admin_operating_system();			//管理员操作系统
	}
	if(login_anser == gongzheng_in){
		gongzheng_operating_system();			//公证员操作系统
	}
	else{
		exit(0);
	}
}




void admin_operating_system(){
	printf("请输入管理员密码:");
	char try[20];
	int try_times = 0;
	for(;;){
		scanf("%s",try);
		if(strcmp(try,admin_password) != 0 && try_times < 3){
			try_times++;
			printf("密码错误，已输错%d次。请重新输入:\n",try_times);
		}
		if(try_times == 3){
			printf("尝试次数过多！\n");
			login_page();	//循环出口
		}
		if(strcmp(try,admin_password) == 0){
			break;		//循环出口
		}
	}
	printf("========================================================================\n");
	printf("\t\t\t\t管理员操作系统\n");
	printf("========================================================================\n\n\n\n");
	printf("1、发行彩票\n2、查询彩民信息\n3、排序\n4、保存\n5、读取文件中所有用户信息\n6、读取文件中所有彩票信息\n7、退出\n");
	int admin_operating_system_ans;
	printf("请输入编号(1~6):");
	scanf("%d",&admin_operating_system_ans);
	printf("\n");
	switch(admin_operating_system_ans){
		case 1:faxingcaipiao();
			break;
		case 2:admin_search_user();
			break;
		case 3:admin_paixu();
			break;
		case 4:admin_save();
			break;
		case 5://admin_read();
			read_user();
			break;
		case 6:read_caipiao();
			break;
		case 7:
			printf("已退出，返回登录界面。\n");
			return login_page();
		default:
			printf("输入无效，返回登录界面。\n");
			return;
	}

}

void faxingcaipiao(){
	/*
	struct caipiao{				//彩票结构体声明
	long no;				//彩票期号	ok
	int price;				//彩票单价	ok
	int open_close;				//开奖状态 (0为未开奖，1为已开奖)	ok
	int zhongjiang_number[7];		//中奖号码数组		null
	int sum_price;				//奖池总数		ok
	struct caipiao *next;			//下一期彩票指针	ok
};
typedef struct caipiao Caipiao;				//结构体定义别名
struct caipiao *caipiao_nowsite;			//彩票结构体当前位置指针	ok
struct caipiao *caipiao_head;				//彩票链表头指针	ok
	*/
	printf("========================================================================\n");
	printf("\t\t\t\t彩票发行页面\n");
	printf("========================================================================\n\n\n\n");
	if(caipiao_nowsite != NULL){	
		
		caipiao_nowsite->next=malloc(sizeof(Caipiao));		//分配内存
		caipiao_nowsite=caipiao_nowsite->next;			//当前位置跟随移动
		
		printf("输入彩票期号：");
		scanf("%ld",&caipiao_nowsite->no);	//获取彩票期号
		printf("请输入彩票单价:");
		scanf("%d",&caipiao_nowsite->price);	//获取彩票单价
		caipiao_nowsite->next=NULL;
		caipiao_nowsite->open_close = 0;	//彩票未开奖
		caipiao_nowsite->sum_price = 0;		//奖池总金额初始化
		return admin_operating_system();
	}
	if(caipiao_nowsite == NULL){				//首张彩票
		caipiao_nowsite=malloc(sizeof(Caipiao));	//分配内存
		caipiao_head=caipiao_nowsite;			//首地址
		caipiao_nowsite->next=NULL;
		
		printf("输入彩票期号：");
		scanf("%ld",&caipiao_nowsite->no);	//获取彩票期号
		printf("请输入彩票单价:");
		scanf("%d",&caipiao_nowsite->price);	//获取彩票单价
		caipiao_nowsite->open_close = 0;	//彩票未开奖
		caipiao_nowsite->sum_price = 0;		//奖池总金额初始化
		return admin_operating_system();
	}

}

void caipiao_kaijiang(struct caipiao *p){
	/*long no;				//彩票期号
	int price;				//彩票单价
	int open_close;				//开奖状态 (0为未开奖，1为已开奖)
	int zhongjiang_number[7];		//中奖号码数组
	int num;				//售出总数
	long sum_price;				//奖池总数*/
	
	printf("中奖信息输入：");
	for(int i=0;i<7;i++){
		printf("请输入第%d位中奖号码",i+1);
		scanf("%d",&(p->zhongjiang_number[i]));	//获取中奖号码
		if(p->zhongjiang_number[i] > 10||p->zhongjiang_number[i] <= 0){
			printf("彩票格式不正确(1~10之间的数字),请重新输入。\n");
			i--;
		}
		p->open_close = 1;	//已开奖
		kaijiang_fun(p);	//按规则往用户余额中添加中奖金额
	}
	return gongzheng_operating_system();
}
void admin_search_user(){
	int i;
	printf("1、根据用户id查找\n2、根据用户昵称查找\n3、根据用户余额区间查找\n4、查看所有彩民信息\n5、退出\n");
	scanf("%d",&i);
	switch(i){
		case 1:admin_search_user_byid();
			break;
		case 2:admin_search_user_byname();
			break;
		case 3:admin_search_user_bymoney();
			break;
		case 4:display_all_user(user_head);
			break;
		case 5:return  admin_operating_system();
		default :exit(0);
	}
	
	
}
void admin_search_user_byid(){
	int i;
	struct user *search;
	search = user_head;
	printf("请输入用户id:");
	scanf("%d",&i);
	if(search != NULL){
		if(i != search->user_no){
			search = search->next;
		}if(i == search->user_no){
			display_user(search);
			return admin_operating_system();
		}
	}if(search == NULL){
		printf("查找完成。未找到该用户！");
		return admin_operating_system();
	}
	
}
void display_user(User *site){		//用户信息显示
	printf("用户id:%d\n用户昵称:%s\n用户余额:%d\n",site->user_no,site->user_name,site->user_money);
}
void admin_search_user_byname(){	//管理员查找彩民信息分支byname
	char aa[20];
	struct user *search;
	search = user_head;
	printf("请输入用户昵称:");
	scanf("%s",aa);
	if(search != NULL){
		if(strcmp(aa,search -> user_name) != 0){
			search = search->next;
		}else{
			display_user(search);
			return admin_operating_system();
		}
	}if(search == NULL){
		printf("查找完成。未找到该用户！");
		return admin_operating_system();
	}
}
void admin_search_user_bymoney(){	//管理员查找彩民信息分支bymoney
	int max,min;
	printf("请输入余额区间最小值:");
	scanf("%d",&min);
	printf("请输入余额区间最大值:");
	scanf("%d",&max);
	struct user *search;
	search = user_head;
	if(search != NULL){
		if(search->user_money >= min&&search->user_money <= max){
			display_user(search);
			search = search->next;
		}else{
			search = search->next;
		}
	
	}if(search == NULL){
		printf("查找完成。");
		return admin_operating_system();
	}
}
void display_all_user(struct user *head){
	if(head != NULL){
		display_user(head);
		head = head->next;
	}else{
		printf("输出完成。");
		admin_search_user();
	}
}
void admin_paixu(){
	int ans;
	printf("1、按编号排序\n2、按昵称字母排序\n3、按余额排序\n4、退出\n请输入编号(1~4):");
	scanf("%d",&ans);
	switch(ans){
		case 1:admin_paixu_byno();
			break;
		case 2:admin_paixu_byname();
			break;
		case 3:admin_paixu_bymoney();
			break;
		
		case 4:return admin_operating_system();
		default:exit(0);
	}
}
void admin_paixu_byno(){
	User paixu_no[user_no_auto-1];
	FILE *fp = fopen("user.bat","rb");
	if(fp != NULL){
		//int fread(void *buffer,int numBytes,int count,FILE*)
		fread(paixu_no,sizeof(struct user),user_no_auto-1,fp);		//读取
	}
	fclose(fp);
	/*冒泡排序(no小的在前)*/
	for(int i=0;i<user_no_auto-1;i++){					//循环趟数
		for(int j=user_no_auto-2;j>i;j--){				//j为内循环比较次数
		if(paixu_no[j].user_no < paixu_no[j-1].user_no){
		/*思路1、把next指向其的地址交换  按链表输出  *难以获取next地址指向其的地址
		  思路2、全交换 按数组编号输出		   *交换工作多
		 采用思路2*/
				user_change(&paixu_no[j],&paixu_no[j-1]);
			}
		}
	}//end of for
	display_all_user(paixu_no);
}
void admin_paixu_byname(){		//管理员排序分支byname
	User paixu_name[user_no_auto-1];
	FILE *fp = fopen("user.bat","rb");
	if(fp != NULL){
		//int fread(void *buffer,int numBytes,int count,FILE*)
		fread(paixu_name,sizeof(struct user),user_no_auto-1,fp);		//读取
	}
	fclose(fp);
	/*冒泡排序(按字母排序，小的在前)*/
	for(int i=0;i<user_no_auto-1;i++){					//循环趟数
		for(int j=user_no_auto-2;j>i;j--){				//j为内循环比较次数 从后往前比较 小的逐渐往前面靠
		if(strcmp(paixu_name[j-1].user_name,paixu_name[j].user_name) > 0){
				user_change(&paixu_name[j],&paixu_name[j-1]);
			}
		}
	}//end of for
	display_all_user(paixu_name);
}

void admin_paixu_bymoney(){		//管理员排序分支bymoney
	User paixu_money[user_no_auto-1];
	FILE *fp = fopen("user.bat","rb");
	if(fp != NULL){
		//int fread(void *buffer,int numBytes,int count,FILE*)
		fread(paixu_money,sizeof(struct user),user_no_auto-1,fp);		//读取
	}
	fclose(fp);
	/*冒泡排序(按字母排序，小的在前)*/
	for(int i=0;i<user_no_auto-1;i++){					//循环趟数
		for(int j=user_no_auto-2;j>i;j--){				//j为内循环比较次数 从后往前比较 钱数多的逐渐往前面靠
		if(paixu_money[j].user_money > paixu_money[j-1].user_money){
				user_change(&paixu_money[j],&paixu_money[j-1]);
			}
		}
	}//end of for
	display_all_user(paixu_money);
}

void user_change(User *p,User *q){	//两个结构体 user 全交换
	/*
	struct user{				//用户数据结构体
	int user_no; 		//用户编号	自动获取
	char user_name[20];	//用户昵称
	char user_password[20]; //用户密码
	int user_money;		//用户余额
	struct user *next;	//用户链表下一个用户地址
	
};*/
	struct user temp;
	temp.user_no = p->user_no;			//temp存p
	strcpy(temp.user_name,p->user_name);
	strcpy(temp.user_password,p->user_password);
	temp.user_money = p->user_money;
	temp.next = p->next;
	
	p->user_no = q->user_no;			//p存q
	strcpy(p->user_name,q->user_name);
	strcpy(p->user_password,q->user_password);
	p->user_money = q->user_money;
	p->next = q->next;
	
	q->user_no = temp.user_no;			//q存temp
	strcpy(q->user_name,temp.user_name);
	strcpy(q->user_password,temp.user_password);
	q->user_money = temp.user_money;
	q->next = temp.next;
}

void admin_save(){
	printf("1、保存用户信息\n2、保存彩票信息\n3、退出\n\n请输入(1~3):");
	int ans;
	scanf("%d",&ans);
	switch(ans){
		case 1:save_user();
			break;
		case 2:save_caipiao();
			break;
		case 3:return admin_operating_system();
		case 4:exit(0);
	}
}

void save_user(){
	FILE *fp=fopen("user.bat","wb");
	FILE *fp4=fopen("user_no_auto.bat","wb");
	fwrite(&user_no_auto,sizeof(int),1,fp4);	//用户注册自动帐号参数
	struct user *p;
	p = user_head;
	if(fp != NULL){
		while(p != NULL){		//用户链表写入
			fwrite(p,sizeof(User),1,fp);
//			free(p);		//释放内存
			printf("用户%s写入成功！\n",p->user_name);
			p = p->next;
		}
		
	}
	fclose(fp);
	FILE *fp2=fopen("user_head.bat","wb");
	if(fp2 != NULL){
		if(user_head != NULL){		//链表头指针写入
			fwrite(user_head,sizeof(User),1,fp2);
			printf("user_head写入成功！\n");
		}
	}
	fclose(fp2);
	FILE *fp3=fopen("user_nowsite.bat","wb");
	if(fp3 != NULL){
		if(user_nowsite != NULL){		//链表当前位置指针写入
			fwrite(user_nowsite,sizeof(User),1,fp3);
			printf("user_nowsite写入成功！\n");
		}
	}
	fclose(fp3);
	fclose(fp4);
	printf("保存用户信息成功！");
	return admin_operating_system();
}

void read_user(){
	struct user *p;
	struct user *user_head;
	
	FILE *fp2=fopen("user_head.bat","rb");
	if(fp2 != NULL){
		printf("user_head.bat打开成功！\n");
	}else{
		printf("error\n");
	}
	if(fp2 != NULL){
			//链表头指针读出到user_head
			//printf("2");									//测试
			fread(user_head,sizeof(User),1,fp2);
	}
	p=user_head;
	fclose(fp2);
	FILE *fp=fopen("user.bat","rb");
	if(fp != NULL){
		printf("user.bat打开成功！\n");
	}else{
		printf("error\n");
	}
	
	if(fp != NULL){
		while(1){
			if(p->next != NULL){		//用户链表读出
				fread(p=malloc(sizeof(User)),sizeof(User),1,fp);
				p->next=malloc(sizeof(User));	
				p=p->next;			//移动
			}
			if(p->next == NULL){		//链表最后一位
				fread(p=malloc(sizeof(User)),sizeof(User),1,fp);
				break;			//循环出口
			}
		}
	}
	fclose(fp);
	FILE *fp3=fopen("user_nowsite.bat","rb");
	if(fp3 != NULL){
		printf("user_nowsite.bat打开成功！\n");
	}else{
		printf("error\n");
	}
	if(fp3 != NULL){
			//链表当前位置指针读出到user_nowsite
			//printf("3");									//测试
			fread(user_nowsite,sizeof(User),1,fp3);
	}
	fclose(fp3);
	FILE *fp4=fopen("user_no_auto.bat","rb");
	if(fp4 != NULL){
		printf("user_no_auto.bat打开成功！\n");
	}else{
		printf("error\n");
	}
	fread(&user_no_auto,sizeof(int),1,fp4);
	fclose(fp4);
	
}
void save_caipiao(){
	FILE *fp=fopen("caipiao.bat","wb");
	struct caipiao *p;
	p = caipiao_head;
	if(fp != NULL){
		while(p != NULL){
			fwrite(p,sizeof(Caipiao),1,fp);
			p = p->next;
		}
		printf("caipiao.bat保存成功！\n");
	}
	fclose(fp);
	FILE *fp2=fopen("caipiao_head.bat","wb");
	
	if(fp2 != NULL){
		if(caipiao_head != NULL){		//链表头指针写入
			fwrite(caipiao_head,sizeof(Caipiao),1,fp2);
			printf("caipiao_head保存成功！\n");
		}
	}
	fclose(fp2);
	FILE *fp3=fopen("caipiao_nowsite.bat","wb");
	
	if(fp3 != NULL){
		if(caipiao_nowsite != NULL){		//链表当前位置指针写入
			fwrite(caipiao_nowsite,sizeof(Caipiao),1,fp3);
			printf("caipiao_nowsite写入成功！\n");
		}
	}
	fclose(fp3);
	printf("保存彩票信息成功！\n");
	return admin_operating_system();
}

void read_caipiao(){
	struct caipiao *p;
	struct caipiao *caipiao_head;
	
	FILE *fp2=fopen("caipiao_head.bat","rb");
	if(fp2 != NULL){
		printf("caipiao_head.bat打开成功！\n");
	}else{
		printf("error\n");
	}
	if(fp2 != NULL){
			//链表头指针读出到user_head
			//printf("2");									//测试
			fread(caipiao_head,sizeof(Caipiao),1,fp2);
	}
	p=caipiao_head;
	fclose(fp2);
	FILE *fp=fopen("caipiao.bat","rb");
	if(fp != NULL){
		printf("caipiao.bat打开成功！\n");
	}else{
		printf("error\n");
	}
	
	if(fp != NULL){
		while(1){
			if(p->next != NULL){		//用户链表读出
				fread(p=malloc(sizeof(Caipiao)),sizeof(Caipiao),1,fp);
				p->next=malloc(sizeof(Caipiao));	
				p=p->next;			//移动
			}
			if(p->next == NULL){		//链表最后一位
				fread(p=malloc(sizeof(Caipiao)),sizeof(Caipiao),1,fp);
				break;			//循环出口
			}
		}
	}
	fclose(fp);
	FILE *fp3=fopen("caipiao_nowsite.bat","rb");
	if(fp3 != NULL){
		printf("caipiao_nowsite.bat打开成功！\n");
	}else{
		printf("error\n");
	}
	if(fp3 != NULL){
			//链表当前位置指针读出到user_nowsite
			//printf("3");									//测试
			fread(caipiao_nowsite,sizeof(Caipiao),1,fp3);
	}
	fclose(fp3);
	
}
void user_login_page(){				//用户登录界面
	printf("请输入帐号:");
	
	int aa;
	char password[20];
	scanf("%d",&aa);
	struct user *p;
	p = user_head;
	for(;;){
		if(p != NULL){
			if(p->user_no != aa && p->next != NULL){	//帐号不对且不是帐号链表最后一个
				p = p->next;
			}
			if(p->user_no != aa && p->next == NULL){	//帐号不对且已到帐号链表最后一位
				printf("帐号不存在！\n");
				break;		//循环出口
			}
			if(p->user_no == aa){
					if(p->ban == 0){
						if(yanzheng_user_password(p) == 1){
					user_operating_system(p);	//验证通过，进入用户操作系统
				}
				else break;			//退出验证用户循环
			
					}
					if(p->ban == 1){
						printf("该用户已被禁用。");
					}
				}
		}if(p == NULL){
			printf("没有用户！\n");
			break;			//退出验证用户循环
		}
	}//end of for
	return login_page();
}
void admin_read(){
	read_user();
	printf("读取用户数据成功！\n");
	read_caipiao();
	printf("读取彩票信息成功！\n");
}
void user_operating_system(struct user *p){
	/*用户操作界面 显示 1、用户昵称 2、用户余额 3、本期彩票开奖信息*/
	/*选项1、充值2、购买彩票3、查看已购买彩票情况4、修改密码5、注销账户\n6、登出*/
	if(p != NULL){
		printf("========================================================================\n");
		printf("\t欢迎您！%s\n",p->user_name);
		printf("========================================================================\n");
		printf("余额:%d元\n",p->user_money);
		if(caipiao_nowsite != NULL){
			if(caipiao_nowsite->open_close == 1){				//本期彩票开奖信息
				printf("第%ld期彩票已开奖。\n",caipiao_nowsite->no);
				display_caipiao(caipiao_nowsite);
			}
			if(caipiao_nowsite->open_close == 0){
				printf("第%ld期彩票未开奖。\n",caipiao_nowsite->no);
			}
		}
		printf("1、充值\n2、购买彩票\n3、查看已购买彩票情况\n4、修改密码\n5、注销账户\n6、登出\n请输入选项(1~6):");
		int ans;
		scanf("%d",&ans);
		switch(ans){
			case 1:add_money(p);
				break;
			case 2:buy_caipiao(p);
				break;
			case 3:display_bought_caipiao(p);
				break;
			case 4:change_password(p);
				break;
			case 5:if(write_off_user(p) == 1){
				printf("注销成功，返回初始界面。\n");
				return login_page();
				}
				else{
				printf("注销失败。\n");	
				return user_operating_system(p);
				}
				
			case 6:return login_page();
			default:return user_operating_system(p);
		}
	}else{
		printf("程序错误。用户不存在！\n");
	}
}
void display_caipiao(struct caipiao *p){
	printf("中奖号码为：\t");
	for(int i=0;i<7;i++){
		display_num(p->zhongjiang_number[i]);
	}
	printf("\n");
}
void display_num(int a){
	switch(a){
		case 1:printf("①  ");
			break;
		case 2:printf("②  ");
			break;
		case 3:printf("③  ");
			break;
		case 4:printf("④  ");
			break;
		case 5:printf("⑤  ");
			break;
		case 6:printf("⑥  ");
			break;
		case 7:printf("⑦  ");
			break;
		case 8:printf("⑧  ");
			break;
		case 9:printf("⑨  ");
			break;
		case 10:printf("⑩  ");
			break;
		default:printf("*  ");
		
		
	}
}

void add_money(struct user *p){
	printf("请输入充值金额:");
	int m;
	scanf("%d",&m);
	if(yanzheng_user_password(p) == 1){			//验证通过
		p->user_money += m;
		return user_operating_system(p);
	}else{							//验证不通过或中途退出
		return login_page();		//返回初始界面
	}
	
	char password[20];
	int user_try_times = 0;
	for(int i=0;i<3;i++){
		printf("请输入用户密码:");
		scanf("%s",password);
		if(strcmp(password,p->user_password) == 0){	//密码正确
			//pay_fun();
			p->user_money += m;
			return user_operating_system(p);
			break;			//退出验证用户循环
		}
		if(strcmp(password,p->user_password) != 0&& user_try_times < 3){	//密码错误
			user_try_times++;
			printf("密码错误,已输入错误%d次，输入错误三次将冻结该账户24h！\n",user_try_times);	
		}
		if(user_try_times == 3){
			printf("已输错三次，帐号已冻结24h！\n");
			ban_user(p);			//冻结p指向的帐号
		}//end of if
			
	}//end of for
}

void buy_caipiao(struct user *p){

	/*1、（open_close == 0）未开奖的彩票都输出出来（彩票期号no 彩票单价price ）给选项选择购买
	2、获取购买号码
	3、购买倍数
	4、所需钱数
	5、判断余额是否足够，不够的话弹出充值界面
	6、继续进行余额验证，输入密码验证进行购买*/
	struct caipiao *q;
	q=caipiao_head;
	if(q == NULL){			//caipiao_head为空
			printf("无未开奖彩票。\n");
		}
	struct caipiao *option[20];
	int i=0;
		
			

	
	while(q != NULL){
		if(q -> open_close == 0){			//q 指向未开奖彩票
			option[i]=q;			//把未开奖彩票地址信息存入option中
			printf("%d、%ld期彩票 单价%d元\n",i+1,option[i]->no,option[i]->price);	//输出未开奖彩票信息
			i++;
			if(q->next == NULL){
				break;
					}
			else{
				q = q -> next;				//指向链表下一个
					}
			
				}
		if(q -> open_close == 1){			//q指向已开奖彩票
			if(q->next == NULL){
				break;
			}else{
				q = q -> next;				//指向链表下一个
				}
			
			}
			
		}
		printf("输出完成。\n");
		
			
	

	
	//option[i-1]就是选项i对应的地址
	printf("您要购买第几个选项的彩票(输入99退出)");
	int choose;
	scanf("%d",&choose);
	if(choose == 99){
		return user_operating_system(p);		//返回用户操作系统
	}
	int user_choose_num[7];
	int times;
	for(int j=i,i=1;i<=j;i++){
		if(choose == i){			//对应选项	要购买的彩票是option[choose-1]
			//option[choose-1]
			for(int m=0;m<7;m++){				//用户选择下注号码
				printf("请输入您下注的第%d个号码:",m+1);
				scanf("%d",&user_choose_num[m]);
			}
			printf("每注%d元，请输入下注倍数:",option[choose-1] -> price);			//用户输入下注倍数
			scanf("%d",&times);
			int sum;
			sum = option[choose-1] -> price * times;
			while(1){
				if(sum <= p->user_money){		//用户余额充足
					if(yanzheng_user_password(p) == 1){		//验证通过
						p->bought[p->caipiao_times] = option[choose-1];	//把要购买的彩票信息地址输入到用户数据中
						for(int aa=0;aa<7;aa++){
							p->bought_num[p->caipiao_times][aa]=user_choose_num[aa];
						}	//把购买的号码输入到用户bought二维数组中去
						p->caipiao_times_times[p->caipiao_times] = times;	//倍数信息输入到该用户信息中
						p->caipiao_times++;		//用户购买彩票的张数++
						p->user_money -= sum;		//扣钱
						option[choose-1]->sum_price += sum;	//奖池金额+
						break;		//出口
					}if(yanzheng_user_password(p) == 0){		//验证不通过
						return login_page();
					}
				
				}
				if(sum > p->user_money){			//用户余额不足
					printf("余额不足,是否充值（按1进入充值界面，其他键退出）\n");
					int t;
					scanf("%d",&t);
					if(t == 1){
						add_money(p);
						continue;		//继续while
					}else break;			//跳出while
				}
			}//end of while
			break;				//跳出for循环
		}
		//如果误操作选择了不存在的一个选项
		if(choose > j||choose < 1){
			printf("选项不存在，请重新输入：");
			scanf("%d",&choose);
			i=1;
		}
	}
	
}

int yanzheng_user_password(struct user *p){
	char password[20];
	int user_try_times = 0;
	for(int i=0;i<3;i++){
		printf("请输入用户密码:");
		scanf("%s",password);
		if(strcmp(password,p->user_password) == 0){	//密码正确
			return 1;
			break;			//退出验证用户循环
		}
		if(strcmp(password,p->user_password) != 0&& user_try_times < 3){	//密码错误
			user_try_times++;
			printf("密码错误,已输入错误%d次，输入错误三次将冻结该账户24h！\n",user_try_times);	
		}
		if(user_try_times == 3){
			printf("已输错三次，帐号已冻结24h！\n");
			ban_user(p);			//冻结p指向的帐号
			return 0;
		}//end of if
		return 0;	
	}//end of for
}

void display_bought_caipiao(struct user *p){			//显示用户已购买彩票
	if(p != NULL){
		if(p->caipiao_times != 0){
			for(int i = p->caipiao_times;i>=0;i--){
				display_caipiao(p -> bought[i]);	
			}
		}
		else{
			printf("无数据。\n");
		}
		
	}
}

void change_password(struct user *p){		//更改用户密码
	printf("1、验证原密码\n2、短信验证(暂未开发)\n");
	int i;
	scanf("%d",&i);
	switch(i){
		case 1:yanzheng_user_password(p);
			change_password_by_password(p);
			break;
		case 2://yanzheng_shot_note(p); 验证短信
			printf("功能尚未开发！");
			break;
	}
}

void change_password_by_password(struct user *p){
	char new_password_1[20];
	char new_password_2[20];
	printf("请输入新密码:");
	scanf("%s",new_password_1);
	printf("请再次输入新密码:");
	scanf("%s",new_password_2);
	if(strcmp(new_password_1,new_password_2) == 0){
		strcpy(p -> user_password,new_password_1);
	}
	if(strcmp(new_password_1,new_password_2) != 0){
		printf("两次输入不一致！请检查后再输入。");
		change_password_by_password(p);
	}
	
	
}
void ban_user(struct user *p){
	p->ban=1;
	clock_t start;
	start=clock();		//获取当前时间
	while(1){
		if((clock()-start) >= day_of_1){
			p->ban=0;		//解封
			break;		//循环出口
		}
	}
}

int write_off_user(struct user *p){
	if(yanzheng_user_password(p) == 1){
		printf("将要注销帐号，注销不可撤回，是否继续（1、继续2、返回）:");
		int i;
		scanf("%d",&i);
		switch(i){
			case 1:p->last = p->next;
				free(p);
				return 1;
				break;
			default:
				return 0;
		}
	}
}
void kaijiang_fun(struct caipiao *p){
	/*规则：一共7个号码，号码范围1～10,号码可重复
		仅中1个号码返还单价*0.5倍数（若单价为2）=0.5*倍数
		仅中2个号码返还单价*1倍数（若单价为2）=1*倍数
		仅中3个号码返还单价*4倍数（若单价为2）=2*倍数*2
		仅中4个号码返还单价*2*倍数（若单价为2）=4*倍数*2
		仅中5个号码返还单价*2*4*倍数（若单价为2）=16*倍数*2
		仅中6个号码返还单价*2*4*8*倍数（若单价为2）=128*倍数*2
		全中返还单价*2*4*8*16*倍数（若单价为2）=2048*倍数*2*/
	/*1、遍历用户链表
	  2、遍历用户所购买的彩票期号
	  3、与p彩票期号相同时检查中奖号码
	  4、根据中奖号码数量，倍数，单价向用户发放彩金*/
	  
	  struct user *q;			//q指向用户链表  p指向当前发行彩票
	  q=user_head;
	  while(1){
	  	if(q == NULL){
	  		break;			//循环出口
	  	}
	  	if(q != NULL){
	  		
	  		for(int i=0;i<=q->caipiao_times;i++){
	  			if(q->bought[i] != NULL && q->bought[i]->no == p->no){
	  				int cnt=0;
	  				for(int m=0;m<7;m++){
	  					if(p->zhongjiang_number[m] == q->bought_num[i][m]){
	  						cnt++;
	  					}
	  				}//end of for
	  				if(cnt == 1){
	  					q->user_money += q->caipiao_times_times[i]*p->price*5;
	  					p->sum_price -= q->caipiao_times_times[i]*p->price*5;
	  				}
	  				if(cnt == 2){
	  					q->user_money += q->caipiao_times_times[i]*p->price*10;
	  					p->sum_price -= q->caipiao_times_times[i]*p->price*10;
	  				}
	  				if(cnt == 3){
	  					q->user_money += q->caipiao_times_times[i]*p->price*20;
	  					p->sum_price -= q->caipiao_times_times[i]*p->price*20;
	  				}
	  				if(cnt == 4){
	  					q->user_money += q->caipiao_times_times[i]*p->price*2*20;
	  					p->sum_price -= q->caipiao_times_times[i]*p->price*2*20;
	  				}
	  				if(cnt == 5){
	  					q->user_money += q->caipiao_times_times[i]*p->price*2*4*20;
	  					p->sum_price -= q->caipiao_times_times[i]*p->price*2*4*20;
	  				}
	  				if(cnt == 6){
	  					q->user_money += q->caipiao_times_times[i]*p->price*2*4*8*20;
	  					p->sum_price -= q->caipiao_times_times[i]*p->price*2*4*8*20;
	  				}
	  				if(cnt == 7){
	  					q->user_money += q->caipiao_times_times[i]*p->price*2*4*8*16*20;
	  					p->sum_price -= q->caipiao_times_times[i]*p->price*2*4*8*16*20;
	  				}
	  				
	  			}
	  		}
	  		q=q->next;
	  	}
	  }
	
}
void main(){
	
	
	login_page();
	
}
