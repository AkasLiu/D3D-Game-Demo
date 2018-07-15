#include "d3dUtility.h"
#include "gameObject.h"
#include "vertex.h"
#include "plane.h"
#include "DirectInputClass.h"
#include "camera.h"
#include "cube.h"
#include "font.h"
#include "skyBox.h"
#include "character.h"

//-----------------------------------【宏定义部分】--------------------------------------------
//	描述：定义一些辅助宏
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	800							//为窗口宽度定义的宏，以方便在此处修改窗口宽度
#define WINDOW_HEIGHT	600							//为窗口高度定义的宏，以方便在此处修改窗口高度

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "dinput8.lib")     // 使用DirectInput必须包含的头文件，注意这里有8
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "winmm.lib") 

//-----------------------------------【全局变量声明部分】-------------------------------------
//	描述：全局变量的声明
//------------------------------------------------------------------------------------------------
ID3DXFont*								g_pFont = NULL;    //字体COM接口
float											g_FPS = 0.0f;       //一个浮点型的变量，代表帧速率
wchar_t										g_strFPS[50];    //包含帧速率的字符数组

LPDIRECT3DDEVICE9							g_pd3dDevice = NULL;		//Direct3D设备对象
Plane*										floorPlane;					//地面
DInputClass*								g_pDInput = NULL;			//一个DInputClass类的指针
Cube*										cube[4];					//边界的围墙
SkyBox*										skyBox;
Font*										blackFont;
Character*									player;
Camera TheCamera(Camera::LANDOBJECT);
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

//-----------------------------------【全局函数声明部分】----------------------------------------
//	描述：全局函数声明，防止“未声明的标识”系列错误
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK	WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//窗口过程函数
HRESULT					Direct3D_Init(HWND hwnd);		 //在这个函数中进行Direct3D的初始化
HRESULT					Objects_Init(HWND hwnd); 		//在这个函数中进行要绘制的物体的资源初始化
VOID							Direct3D_Render(HWND hwnd); 	//在这个函数中进行Direct3D渲染代码的书写
VOID							Direct3D_CleanUp();				//在这个函数中清理COM资源以及其他资源
void								Direct3D_Update(HWND hwnd,float deltaTime);

//-----------------------------------【WinMain( )函数】------------------------------------------
//	描述：Windows应用程序的入口函数，我们的程序从这里开始
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//【1】窗口创建四步曲之一：开始设计一个完整的窗口类
	WNDCLASSEX wndClass = { 0 };							//用WINDCLASSEX定义了一个窗口类
	wndClass.cbSize = sizeof(WNDCLASSEX);			//设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//设置窗口的样式
	wndClass.lpfnWndProc = WndProc;					//设置指向窗口过程函数的指针
	wndClass.cbClsExtra = 0;								//窗口类的附加内存，取0就可以了
	wndClass.cbWndExtra = 0;							//窗口的附加内存，依然取0就行了
	wndClass.hInstance = hInstance;						//指定包含窗口过程的程序的实例句柄。
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //本地加载自定义ico图标
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //指定窗口类的光标句柄。
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);  //为hbrBackground成员指定一个白色画刷句柄	
	wndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
	wndClass.lpszClassName = L"D3DGame";		//用一个以空终止的字符串，指定窗口类的名字。

																//【2】窗口创建四步曲之二：注册窗口类
	if (!RegisterClassEx(&wndClass))				//设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
		return -1;

	//【3】窗口创建四步曲之三：正式创建窗口
	HWND hwnd = CreateWindow(L"D3DGame", L"D3D Game Demo",				//喜闻乐见的创建窗口函数CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//Direct3D资源的初始化，调用失败用messagebox予以显示
	if (!(S_OK == Direct3D_Init(hwnd)))
	{
		MessageBox(hwnd, _T("Direct3D初始化失败~！"), _T("浅墨的消息窗口"), 0); //使用MessageBox函数，创建一个消息窗口 
	}

	//【4】窗口创建四步曲之四：窗口的移动、显示与更新
	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);		//调整窗口显示时的位置，使窗口左上角位于（250,80）处
	ShowWindow(hwnd, nShowCmd);    //调用ShowWindow函数来显示窗口
	UpdateWindow(hwnd);						//对窗口进行更新，就像我们买了新房子要装修一样

	//PlaySound(L"War3XMainScreen.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //循环播放背景音乐 
	//MessageBox(hwnd, L"DirectX，等着瞧吧，我们来降服你了~!", L"浅墨的消息窗口", 0); //使用MessageBox函数，显示一个消息窗口

	//进行DirectInput类的初始化
	g_pDInput = new DInputClass();
	g_pDInput->Init(hwnd, hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//【5】消息循环过程
	MSG msg = { 0 };  //初始化msg
	while (msg.message != WM_QUIT)			//使用while循环
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage(&msg);		//将虚拟键消息转换为字符消息
			DispatchMessage(&msg);		//该函数分发一个消息给窗口程序。
		}
		else
		{
			static float  currentTime = 0.0f;//当前时间
			static float  lastTime = 0.0f;//持续时间
			static float deltaTime = 0.0f;//时间间隔
			currentTime = (float)timeGetTime();
			deltaTime = (currentTime - lastTime)*0.001f;
			lastTime = currentTime;

			Direct3D_Update(hwnd,deltaTime);
			Direct3D_Render(hwnd);   //进行渲染
		}
	}

	//【6】窗口类的注销
	UnregisterClass(L"D3DGame", wndClass.hInstance);  //程序准备结束，注销窗口类
	return 0;
}

//-----------------------------------【WndProc( )函数】--------------------------------------
//	描述：窗口过程函数WndProc,对窗口消息进行处理
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)						//switch语句开始
	{
	case WM_PAINT:						// 若是客户区重绘消息
		Direct3D_Render(hwnd);                 //调用Direct3D渲染函数
		ValidateRect(hwnd, NULL);		// 更新客户区的显示
		break;									//跳出该switch语句

	case WM_KEYDOWN:					// 若是键盘按下消息
		if (wParam == VK_ESCAPE)    // 如果被按下的键是ESC
			DestroyWindow(hwnd);		// 销毁窗口, 并发送一条WM_DESTROY消息
		break;									//跳出该switch语句

	case WM_DESTROY:					//若是窗口销毁消息
		Direct3D_CleanUp();			//调用自定义的资源清理函数Game_CleanUp（）进行退出前的资源清理
		PostQuitMessage(0);			//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;									//跳出该switch语句

	default:										//若上述case条件都不符合，则执行该default语句
		return DefWindowProc(hwnd, message, wParam, lParam);		//调用缺省的窗口过程
	}

	return 0;									//正常退出
}

//-----------------------------------【Direct3D_Init( )函数】--------------------------------------
//	描述：Direct3D初始化函数，进行Direct3D的初始化
//------------------------------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hwnd)
{
	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之一，创接口】：创建Direct3D接口对象, 以便用该Direct3D对象创建Direct3D设备对象
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D接口对象的创建
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) //初始化Direct3D接口对象，并进行DirectX版本协商
		return E_FAIL;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之二,取信息】：获取硬件设备信息
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //支持硬件顶点运算，我们就采用硬件顶点运算，妥妥的
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //不支持硬件顶点运算，无奈只好采用软件顶点运算

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之三，填内容】：填充D3DPRESENT_PARAMETERS结构体
	//--------------------------------------------------------------------------------------
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = WINDOW_WIDTH;
	d3dpp.BackBufferHeight = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之四，创设备】：创建Direct3D设备接口
	//--------------------------------------------------------------------------------------
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

		d3d::Release<LPDIRECT3D9>(pD3D);	//LPDIRECT3D9接口对象的使命完成，我们将其释放掉

		if (!(S_OK == Objects_Init(hwnd))) return E_FAIL;     //调用一次Objects_Init，进行渲染资源的初始化

	return S_OK;
}

//-----------------------------------【Object_Init( )函数】--------------------------------------
//	描述：渲染资源初始化函数，在此函数中进行要被渲染的物体的资源的初始化
//--------------------------------------------------------------------------------------------------
HRESULT Objects_Init(HWND hwnd)
{
	//
	//创建字体
	//
	blackFont = new Font(36, g_pd3dDevice, hwnd);
	blackFont->createFont();

	//
	//设置光照
	//
	D3DXVECTOR3 lightPos(0.0f, 30.0f, 0.0f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitPointLight(&lightPos, &col);

	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, true);
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	//创建天空
	//
	skyBox = new SkyBox(g_pd3dDevice);
	skyBox->LoadSkyTextureFromFile(L"Texture\\SkyBox\\frontsnow1.jpg", L"Texture\\SkyBox\\backsnow1.jpg", L"Texture\\SkyBox\\leftsnow1.jpg", L"Texture\\SkyBox\\rightsnow1.jpg", L"Texture\\SkyBox\\topsnow1.jpg");//从文件加载前、后、左、右、顶面5个面的纹理图
	skyBox->InitSkyBox(1000);  //设置天空盒的边长

	//
	//初始化地面
	//
	D3DXVECTOR3 floorPostion = { 0,0,0 };
	floorPlane = new Plane(500, 500, floorPostion, g_pd3dDevice);
	floorPlane->loadTexture(L"Texture\\desert.bmp");
	floorPlane->init();

	//
	//初始化cube构造四个边界的柱子
	//
	/*----初始化cube的位置---*/
	D3DXVECTOR3 cubePos[4];
	D3DXVECTOR3 cubeSize[4];
	cubePos[0] = D3DXVECTOR3{0,50,275};
	cubeSize[0] = D3DXVECTOR3{ 500,100,50 };
	cubePos[1] = D3DXVECTOR3{ 0,50,-275 };
	cubeSize[1] = D3DXVECTOR3{ 500,100,50 };
	cubePos[2] = D3DXVECTOR3{ 275,50,0 };
	cubeSize[2] = D3DXVECTOR3{ 50,100,500 };
	cubePos[3] = D3DXVECTOR3{ -275,50,0 };
	cubeSize[3] = D3DXVECTOR3{ 50,100,500 };

	for (int i = 0; i < 4; ++i)
	{		
		cube[i] = new Cube(cubePos[i], cubeSize[i], g_pd3dDevice);
		cube[i]->setMaterial(d3d::GRAY_MTRL);
		cube[i]->init();
	}
	//TheCamera.SetTargetPosition(&cube->getPosition());

	//
	//Test cube
	//
	/*D3DXVECTOR3 cubepos = { 0,0,0 };
	D3DXVECTOR3 szie = { 100,100,100 };
	cube[0] = new Cube(cubepos, szie, g_pd3dDevice);
	cube[0]->setMaterial(d3d::GRAY_MTRL);
	cube[0]->init();*/

	//
	//载入X文件，作为主角
	//
	Transform playerTransform;
	playerTransform.position = { 0,80,0 };
	playerTransform.scale = { 0.1f,0.1f,0.1f };
	player = new Character(playerTransform,g_pd3dDevice);
	player->loadXFile(L"XFile\\Player\\miki.X");
	player->init();

	//
	//创建一个摄像机跟随的物体，暂时用cube代替
	//
	/*D3DXVECTOR3 size = { 10,10,10 };
	D3DXVECTOR3 cubePos = { 0,0,30 };
	cubeTest = new Cube(cubePos, size, cubeMesh, Device);
	cubeTest->InitCube();
	TheCamera.SetTargetPosition(&cubePos);*/

	

	//
	// 设置投影矩阵
	//
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
		1.0f,
		1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &proj);


	return S_OK;
}

//-----------------------------------【Direct3D_Update( )函数】--------------------------------
//	描述：不是即时渲染代码但是需要即时调用的，如按键后的坐标的更改，都放在这里
//--------------------------------------------------------------------------------------------------
void Direct3D_Update(HWND hwnd,float deltaTime)
{
	//使用DirectInput类读取数据
	g_pDInput->GetInput();

	// 沿摄像机各分量移动视角
	if (g_pDInput->IsKeyDown(DIK_A))  TheCamera.strafe(-200.0f*deltaTime,player);
	if (g_pDInput->IsKeyDown(DIK_D))  TheCamera.strafe(200.0f*deltaTime,player);
	if (g_pDInput->IsKeyDown(DIK_W))  TheCamera.walk(200.0f*deltaTime,player);
	if (g_pDInput->IsKeyDown(DIK_S))  TheCamera.walk(-200.0f*deltaTime,player);
	if (g_pDInput->IsKeyDown(DIK_R))  TheCamera.fly(10.0f*deltaTime);
	if (g_pDInput->IsKeyDown(DIK_F))  TheCamera.fly(-10.0f*deltaTime);

	//沿摄像机各分量旋转视角
	if (g_pDInput->IsKeyDown(DIK_LEFT))   TheCamera.yaw(-1.0f*deltaTime);
	if (g_pDInput->IsKeyDown(DIK_RIGHT))  TheCamera.yaw(1.0f*deltaTime);
	if (g_pDInput->IsKeyDown(DIK_UP))     TheCamera.pitch(1.0f*deltaTime);
	if (g_pDInput->IsKeyDown(DIK_DOWN))   TheCamera.pitch(-1.0f*deltaTime);
		/*if (g_pDInput->IsKeyDown(DIK_Q)) TheCamera->RotationLookVec(0.001f);
		if (g_pDInput->IsKeyDown(DIK_E)) TheCamera->RotationLookVec(-0.001f);*/

	//自己加一个键
	if (g_pDInput->IsKeyDown(DIK_I))
	{
		/*Transform playerTf;
		playerTf = player->getTransform();
		playerTf.position += {0,0,10};
		player->setTransform(&playerTf);*/
		//MessageBox(hwnd, _T("失败~！"), _T("浅墨的消息窗口"), 0); //使用MessageBox函数，创建一个消息窗口 
	}

	D3DXMATRIX V;
	TheCamera.getViewMatrix(&V);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);

	//鼠标控制右向量和上向量的旋转
	//TheCamera->RotationUpVec(g_pDInput->MouseDX()* 0.001f);
	//TheCamera->RotationRightVec(g_pDInput->MouseDY() * 0.001f);

	////鼠标滚轮控制观察点收缩操作
	//static FLOAT fPosZ = 0.0f;
	//fPosZ += g_pDInput->MouseDZ()*0.03f;

	////计算并设置取景变换矩阵
	//D3DXMATRIX matView;
	//TheCamera->CalculateViewMatrix(&matView);
	//g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	////把正确的世界变换矩阵存到g_matWorld中
	//D3DXMatrixTranslation(&g_matWorld, 0.0f, 0.0f, fPosZ);

	////以下这段代码用于限制鼠标光标移动区域
	//POINT lt, rb;
	//RECT rect;
	//GetClientRect(hwnd, &rect);  //取得窗口内部矩形
	//							 //将矩形左上点坐标存入lt中
	//lt.x = rect.left;
	//lt.y = rect.top;
	////将矩形右下坐标存入rb中
	//rb.x = rect.right;
	//rb.y = rect.bottom;
	////将lt和rb的窗口坐标转换为屏幕坐标
	//ClientToScreen(hwnd, &lt);
	//ClientToScreen(hwnd, &rb);
	////以屏幕坐标重新设定矩形区域
	//rect.left = lt.x;
	//rect.top = lt.y;
	//rect.right = rb.x;
	//rect.bottom = rb.y;
	////限制鼠标光标移动区域
	//ClipCursor(&rect);

	//ShowCursor(false);		//隐藏鼠标光标
}

//-----------------------------------【Direct3D_Render( )函数】--------------------------------------
//	描述：使用Direct3D进行渲染
//--------------------------------------------------------------------------------------------------
void Direct3D_Render(HWND hwnd)
{
	g_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xffffffff, 1.0f, 0);
	g_pd3dDevice->BeginScene();

	//
	//绘制字体，用于测试
	//
	D3DXVECTOR3 cameraPos;
	TheCamera.getCameraPosition(&cameraPos);
	static wchar_t strInfo[256] = { 0 };
	swprintf_s(strInfo, -1, L"摄像机坐标: (%.2f, %.2f, %.2f)", player->getTransform()->position.x, player->getTransform()->position.y, player->getTransform()->position.z);
	blackFont->drawText(50, 0, strInfo, d3d::BLACK);

	//
	//绘制天空
	//
	D3DXMATRIX matSky, matTransSky, matRotSky;
	D3DXMatrixTranslation(&matTransSky, 0.0f, 0.0f, 0.0f);
	D3DXMatrixRotationY(&matRotSky, -0.000005f*timeGetTime());   //旋转天空网格, 简单模拟云彩运动效果
	matSky = matTransSky*matRotSky;
	skyBox->RenderSkyBox(&matSky, false);

	//
	//绘制地面
	//
	floorPlane->draw();

	//
	//绘制边界
	//
	for (int i = 0; i < 4; ++i)
	{
		cube[i]->draw();
	}

	//
	//绘制主角
	//
	player->draw();

	//
	//测试
	//
	//cube[0]->draw();

	g_pd3dDevice->EndScene();
	g_pd3dDevice->Present(0, 0, 0, 0);
}


//-----------------------------------【Direct3D_CleanUp( )函数】--------------------------------
//	描述：资源清理函数，在此函数中进行程序退出前资源的清理工作
//---------------------------------------------------------------------------------------------------
void Direct3D_CleanUp()
{
	d3d::Delete<Character*>(player);
	for (int i = 0; i < 4; ++i)
	{
		d3d::Delete<Cube*>(cube[i]);
	}
	d3d::Delete<Plane*>(floorPlane);
	d3d::Delete<SkyBox*>(skyBox);
	d3d::Delete<Font*>(blackFont);
	d3d::Release<LPDIRECT3DDEVICE9>(g_pd3dDevice);
}