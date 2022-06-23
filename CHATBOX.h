#ifndef CHATBOX
#define CHATBOX

#include <graphics.h>
#include <queue>


class ChatTextBox
{
private:
	int left = 0, top = 0, right = 0, bottom = 0;	// 控件坐标
	wchar_t* text = NULL;							// 控件内容
	size_t maxlen = 0;									// 文本框最大内容长度

public:
	void Create(int x1, int y1, int x2, int y2, int max)
	{
		maxlen = max;
		text = new wchar_t[maxlen];
		text[0] = 0;
		left = x1, top = y1, right = x2, bottom = y2;
		// 绘制用户界面
		// Show();
	}

	~ChatTextBox()
	{
		if (text != NULL)
			delete[] text;
	}

	wchar_t* Text()
	{
		return text;
	}

	bool Check(int x, int y)
	{
		return (left <= x && x <= right && top <= y && y <= bottom);
	}

	// 绘制界面
	void Show()
	{
		// 备份环境值

		int oldlinecolor = getlinecolor();
		int oldbkcolor = getbkcolor();
		int oldfillcolor = getfillcolor();
		//FlushBatchDraw();
		setlinecolor(LIGHTGRAY);		// 设置画线颜色
		setbkcolor(0xeeeeee);			// 设置背景颜色
		setfillcolor(0xeeeeee);			// 设置填充颜色
		fillroundrect(left, top, right, bottom, 10, 10);
		outtextxy(left + 10, top + 5, text);

		// 恢复环境值
		setlinecolor(oldlinecolor);
		setbkcolor(oldbkcolor);
		setfillcolor(oldfillcolor);
		//FlushBatchDraw();
	}

	void OnMessage()
	{
		// 备份环境值
		int oldlinecolor = getlinecolor();
		int oldbkcolor = getbkcolor();
		int oldfillcolor = getfillcolor();

		setlinecolor(BLACK);			// 设置画线颜色
		setbkcolor(WHITE);				// 设置背景颜色
		setfillcolor(WHITE);			// 设置填充颜色
		fillroundrect(left, top, right, bottom, 10, 10);

		//setbkcolor(WHITE);
		//settextcolor(BLACK);
		outtextxy(left + 10, top + 5, text);

		int width = textwidth(text);	// 字符串总宽度
		int counter = 0;				// 光标闪烁计数器
		bool binput = true;				// 是否输入中

		ExMessage msg;
		while (binput)
		{
			FlushBatchDraw();
			while (binput && peekmessage(&msg, EM_MOUSE | EM_CHAR, false))	// 获取消息，但不从消息队列拿出
			{
				if (msg.message == WM_LBUTTONDOWN)
				{
					// 如果鼠标点击文本框外面，结束文本输入
					if (msg.x < left || msg.x > right || msg.y < top || msg.y > bottom)
					{
						//exit(0);
						binput = false;
						break;
					}
				}
				else if (msg.message == WM_CHAR)
				{
					size_t len = wcslen(text);
					switch (msg.ch)
					{
					case '\b':				// 用户按退格键，删掉一个字符
						if (len > 0)
						{
							text[len - 1] = 0;
							width = textwidth(text);
							counter = 0;
							clearrectangle(left + 10 + width, top + 1, right - 1, bottom - 1);
						}
						break;
					case '\r':				// 用户按回车键，结束文本输入
					case '\n':
						binput = false;
						break;
					default:				// 用户按其它键，接受文本输入
						if (len < maxlen - 1)
						{
							text[len++] = msg.ch;
							text[len] = 0;

							clearroundrect(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3, 10, 10);	// 清除画的光标
							width = textwidth(text);				// 重新计算文本框宽度
							counter = 0;

							outtextxy(left + 10, top + 5, text);		// 输出新的字符串
						}
					}
				}
				peekmessage(NULL, EM_MOUSE | EM_CHAR);				// 从消息队列抛弃刚刚处理过的一个消息
			}

			// 绘制光标（光标闪烁周期为 20ms * 32）
			counter = (counter + 1) % 32;
			if (counter < 16)
				line(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3);				// 画光标
			else
				clearroundrect(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3, 10, 10);		// 擦光标

			// 延时 20ms
			FlushBatchDraw();
			Sleep(20);
		}

		clearroundrect(left + 10 + width + 1, top + 3, left + 10 + width + 1, bottom - 3, 10, 10);	// 擦光标

		// 恢复环境值
		setlinecolor(oldlinecolor);
		setbkcolor(oldbkcolor);
		setfillcolor(oldfillcolor);
		FlushBatchDraw();
		Show();

	}
};

ChatTextBox chat_input_box;

const int chat_size = 8;


struct chat_text {
	TCHAR inputtext[50];

}array[chat_size];

TCHAR kep[50];
std::queue<chat_text>chat_msg;//在CHATBOX中 维护全局队列

void chatshow()//显示信息框显示
{
	/*
	rectangle(1080 - 260, 200, 1080 - 50, 230);//1
	rectangle(1080 - 260, 240, 1080 - 50, 270);//2
	rectangle(1080 - 260, 280, 1080 - 50, 310);//3
	rectangle(1080 - 260, 320, 1080 - 50, 350);//4
	rectangle(1080 - 260, 360, 1080 - 50, 390);//5
	rectangle(1080 - 260, 400, 1080 - 50, 430);//6
	rectangle(1080 - 260, 440, 1080 - 50, 470);//7
	rectangle(1080 - 260, 480, 1080 - 50, 510);//8
	*/
	int init_x = 1080 - 260, init_y = 200;

	int con = 0;
	while (!chat_msg.empty()) {
		array[con++] = chat_msg.front();
		chat_msg.pop();
	}
	for (int i = 0; i < con; ++i) {
		chat_msg.push(array[i]);
	}
	for (int i = 0; i < chat_size; ++i) {
		//rectangle(init_x, init_y, init_x + 210, init_y + 30);
		_stprintf(kep, _T("%s"), array[i].inputtext);
		setbkcolor(RGB(250, 248, 244));
		setcolor(BLACK);
		settextstyle(19, 0, _T("楷体"));
		outtextxy(init_x, init_y, (kep));
		init_y += 40;
	}

}



void chatbox()//聊天框
{
	//puts("now inter chatbox");
	ExMessage msg;
	peekmessage(&msg, EM_MOUSE, true);
	//m = getmessage(EM_MOUSE);
	//puts("454545");
	setcolor(BLACK);

	fillrectangle(1080 - 6 - 280, 94, 1080 - 5, 595);
	putimage(1080 - 5 - 280, 95, &chat_option[1]);
	//显示框
	rectangle(1080 - 268, 119, 1080 - 23, 525);

	//显示信息框显示
	chatshow();

	//输入框
	//roundrect(1080 - 268, 535, 1080 - 70, 570, 10, 10);

	//putimage(1080 - 61, 532, &send_option[1]);
	//putimage(1080 - 61, 532, &send_option[2]);

	rectangle(1080 - 5 - 280, 5, 1080 - 5, 5 + 80);
	putimage(1080 - 5 - 279, 6, &sys_option[3]);//背景

	putimage(1080 - 265, 10, &sys_option[6], 0x008800C6);
	putimage(1080 - 265, 10, &sys_option[7], 0x00EE0086);

	//游戏说明
	putimage(1080 - 180, 10, &sys_option[10], 0x008800C6);
	putimage(1080 - 180, 10, &sys_option[11], 0x00EE0086);

	//设置
	putimage(1080 - 95, 10, &sys_option[14], 0x008800C6);
	putimage(1080 - 95, 10, &sys_option[15], 0x00EE0086);

	if (msg.x >= 1080 - 61 && msg.x <= 1080 - 23 && msg.y >= 532 && msg.y <= 569) {
		if (msg.message == WM_LBUTTONDOWN) {//左键按下
			//发送信息 清空文本框中的内容
			//发送信息 myname由服务器发送

			sprintf(sendbuffer, "_chat username %s inputtext %S ", myname, chat_input_box.Text());
			send(fd, sendbuffer, strlen(sendbuffer), 0);
			printf("debug enter send %s\n", sendbuffer);

			//printf("-%s\n", sendbuffer);
			//清空文本框中的内容
			memset(chat_input_box.Text(), 0, sizeof(chat_input_box.Text()));

			putimage(1080 - 61, 532, &send_option[1], 0x008800C6);
			putimage(1080 - 61, 532, &send_option[2], 0x00EE0086);
		}
		else {
			putimage(1080 - 61, 532, &send_option[3], 0x008800C6);
			putimage(1080 - 61, 532, &send_option[4], 0x00EE0086);
		}
		//peekmessage(NULL, EM_MOUSE);
	}
	else {
		putimage(1080 - 61, 532, &send_option[3], 0x008800C6);
		putimage(1080 - 61, 532, &send_option[4], 0x00EE0086);
	}
	chat_input_box.Show();
	if (msg.message == WM_LBUTTONDOWN) {
		if (chat_input_box.Check(msg.x, msg.y)) {
			//puts("debug check");
			chat_input_box.OnMessage();
		}
		//peekmessage(NULL, EM_MOUSE);
	}
}

#endif