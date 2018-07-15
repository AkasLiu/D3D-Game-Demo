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

//-----------------------------------���궨�岿�֡�--------------------------------------------
//	����������һЩ������
//------------------------------------------------------------------------------------------------
#define WINDOW_WIDTH	800							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "dinput8.lib")     // ʹ��DirectInput���������ͷ�ļ���ע��������8
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "winmm.lib") 

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ���������
//------------------------------------------------------------------------------------------------
ID3DXFont*								g_pFont = NULL;    //����COM�ӿ�
float											g_FPS = 0.0f;       //һ�������͵ı���������֡����
wchar_t										g_strFPS[50];    //����֡���ʵ��ַ�����

LPDIRECT3DDEVICE9							g_pd3dDevice = NULL;		//Direct3D�豸����
Plane*										floorPlane;					//����
DInputClass*								g_pDInput = NULL;			//һ��DInputClass���ָ��
Cube*										cube[4];					//�߽��Χǽ
SkyBox*										skyBox;
Font*										blackFont;
Character*									player;
Camera TheCamera(Camera::LANDOBJECT);
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

//-----------------------------------��ȫ�ֺ����������֡�----------------------------------------
//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK	WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//���ڹ��̺���
HRESULT					Direct3D_Init(HWND hwnd);		 //����������н���Direct3D�ĳ�ʼ��
HRESULT					Objects_Init(HWND hwnd); 		//����������н���Ҫ���Ƶ��������Դ��ʼ��
VOID							Direct3D_Render(HWND hwnd); 	//����������н���Direct3D��Ⱦ�������д
VOID							Direct3D_CleanUp();				//���������������COM��Դ�Լ�������Դ
void								Direct3D_Update(HWND hwnd,float deltaTime);

//-----------------------------------��WinMain( )������------------------------------------------
//	������WindowsӦ�ó������ں��������ǵĳ�������￪ʼ
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
	wndClass.cbSize = sizeof(WNDCLASSEX);			//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;					//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra = 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra = 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //ָ��������Ĺ������
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = L"D3DGame";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

																//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	if (!RegisterClassEx(&wndClass))				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow(L"D3DGame", L"D3D Game Demo",				//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//Direct3D��Դ�ĳ�ʼ��������ʧ����messagebox������ʾ
	if (!(S_OK == Direct3D_Init(hwnd)))
	{
		MessageBox(hwnd, _T("Direct3D��ʼ��ʧ��~��"), _T("ǳī����Ϣ����"), 0); //ʹ��MessageBox����������һ����Ϣ���� 
	}

	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
	ShowWindow(hwnd, nShowCmd);    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

	//PlaySound(L"War3XMainScreen.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //ѭ�����ű������� 
	//MessageBox(hwnd, L"DirectX�������ưɣ���������������~!", L"ǳī����Ϣ����", 0); //ʹ��MessageBox��������ʾһ����Ϣ����

	//����DirectInput��ĳ�ʼ��
	g_pDInput = new DInputClass();
	g_pDInput->Init(hwnd, hInstance, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//��5����Ϣѭ������
	MSG msg = { 0 };  //��ʼ��msg
	while (msg.message != WM_QUIT)			//ʹ��whileѭ��
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);		//�ú����ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			static float  currentTime = 0.0f;//��ǰʱ��
			static float  lastTime = 0.0f;//����ʱ��
			static float deltaTime = 0.0f;//ʱ����
			currentTime = (float)timeGetTime();
			deltaTime = (currentTime - lastTime)*0.001f;
			lastTime = currentTime;

			Direct3D_Update(hwnd,deltaTime);
			Direct3D_Render(hwnd);   //������Ⱦ
		}
	}

	//��6���������ע��
	UnregisterClass(L"D3DGame", wndClass.hInstance);  //����׼��������ע��������
	return 0;
}

//-----------------------------------��WndProc( )������--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)						//switch��俪ʼ
	{
	case WM_PAINT:						// ���ǿͻ����ػ���Ϣ
		Direct3D_Render(hwnd);                 //����Direct3D��Ⱦ����
		ValidateRect(hwnd, NULL);		// ���¿ͻ�������ʾ
		break;									//������switch���

	case WM_KEYDOWN:					// ���Ǽ��̰�����Ϣ
		if (wParam == VK_ESCAPE)    // ��������µļ���ESC
			DestroyWindow(hwnd);		// ���ٴ���, ������һ��WM_DESTROY��Ϣ
		break;									//������switch���

	case WM_DESTROY:					//���Ǵ���������Ϣ
		Direct3D_CleanUp();			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage(0);			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;									//������switch���

	default:										//������case�����������ϣ���ִ�и�default���
		return DefWindowProc(hwnd, message, wParam, lParam);		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}

//-----------------------------------��Direct3D_Init( )������--------------------------------------
//	������Direct3D��ʼ������������Direct3D�ĳ�ʼ��
//------------------------------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hwnd)
{
	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮һ�����ӿڡ�������Direct3D�ӿڶ���, �Ա��ø�Direct3D���󴴽�Direct3D�豸����
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D�ӿڶ���Ĵ���
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) //��ʼ��Direct3D�ӿڶ��󣬲�����DirectX�汾Э��
		return E_FAIL;

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮��,ȡ��Ϣ������ȡӲ���豸��Ϣ
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //֧��Ӳ���������㣬���ǾͲ���Ӳ���������㣬���׵�
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //��֧��Ӳ���������㣬����ֻ�ò��������������

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮���������ݡ������D3DPRESENT_PARAMETERS�ṹ��
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
	// ��Direct3D��ʼ���Ĳ���֮�ģ����豸��������Direct3D�豸�ӿ�
	//--------------------------------------------------------------------------------------
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;

		d3d::Release<LPDIRECT3D9>(pD3D);	//LPDIRECT3D9�ӿڶ����ʹ����ɣ����ǽ����ͷŵ�

		if (!(S_OK == Objects_Init(hwnd))) return E_FAIL;     //����һ��Objects_Init��������Ⱦ��Դ�ĳ�ʼ��

	return S_OK;
}

//-----------------------------------��Object_Init( )������--------------------------------------
//	��������Ⱦ��Դ��ʼ���������ڴ˺����н���Ҫ����Ⱦ���������Դ�ĳ�ʼ��
//--------------------------------------------------------------------------------------------------
HRESULT Objects_Init(HWND hwnd)
{
	//
	//��������
	//
	blackFont = new Font(36, g_pd3dDevice, hwnd);
	blackFont->createFont();

	//
	//���ù���
	//
	D3DXVECTOR3 lightPos(0.0f, 30.0f, 0.0f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitPointLight(&lightPos, &col);

	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, true);
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	//�������
	//
	skyBox = new SkyBox(g_pd3dDevice);
	skyBox->LoadSkyTextureFromFile(L"Texture\\SkyBox\\frontsnow1.jpg", L"Texture\\SkyBox\\backsnow1.jpg", L"Texture\\SkyBox\\leftsnow1.jpg", L"Texture\\SkyBox\\rightsnow1.jpg", L"Texture\\SkyBox\\topsnow1.jpg");//���ļ�����ǰ�������ҡ�����5���������ͼ
	skyBox->InitSkyBox(1000);  //������պеı߳�

	//
	//��ʼ������
	//
	D3DXVECTOR3 floorPostion = { 0,0,0 };
	floorPlane = new Plane(500, 500, floorPostion, g_pd3dDevice);
	floorPlane->loadTexture(L"Texture\\desert.bmp");
	floorPlane->init();

	//
	//��ʼ��cube�����ĸ��߽������
	//
	/*----��ʼ��cube��λ��---*/
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
	//����X�ļ�����Ϊ����
	//
	Transform playerTransform;
	playerTransform.position = { 0,80,0 };
	playerTransform.scale = { 0.1f,0.1f,0.1f };
	player = new Character(playerTransform,g_pd3dDevice);
	player->loadXFile(L"XFile\\Player\\miki.X");
	player->init();

	//
	//����һ���������������壬��ʱ��cube����
	//
	/*D3DXVECTOR3 size = { 10,10,10 };
	D3DXVECTOR3 cubePos = { 0,0,30 };
	cubeTest = new Cube(cubePos, size, cubeMesh, Device);
	cubeTest->InitCube();
	TheCamera.SetTargetPosition(&cubePos);*/

	

	//
	// ����ͶӰ����
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

//-----------------------------------��Direct3D_Update( )������--------------------------------
//	���������Ǽ�ʱ��Ⱦ���뵫����Ҫ��ʱ���õģ��簴���������ĸ��ģ�����������
//--------------------------------------------------------------------------------------------------
void Direct3D_Update(HWND hwnd,float deltaTime)
{
	//ʹ��DirectInput���ȡ����
	g_pDInput->GetInput();

	// ��������������ƶ��ӽ�
	if (g_pDInput->IsKeyDown(DIK_A))  TheCamera.strafe(-200.0f*deltaTime,player);
	if (g_pDInput->IsKeyDown(DIK_D))  TheCamera.strafe(200.0f*deltaTime,player);
	if (g_pDInput->IsKeyDown(DIK_W))  TheCamera.walk(200.0f*deltaTime,player);
	if (g_pDInput->IsKeyDown(DIK_S))  TheCamera.walk(-200.0f*deltaTime,player);
	if (g_pDInput->IsKeyDown(DIK_R))  TheCamera.fly(10.0f*deltaTime);
	if (g_pDInput->IsKeyDown(DIK_F))  TheCamera.fly(-10.0f*deltaTime);

	//���������������ת�ӽ�
	if (g_pDInput->IsKeyDown(DIK_LEFT))   TheCamera.yaw(-1.0f*deltaTime);
	if (g_pDInput->IsKeyDown(DIK_RIGHT))  TheCamera.yaw(1.0f*deltaTime);
	if (g_pDInput->IsKeyDown(DIK_UP))     TheCamera.pitch(1.0f*deltaTime);
	if (g_pDInput->IsKeyDown(DIK_DOWN))   TheCamera.pitch(-1.0f*deltaTime);
		/*if (g_pDInput->IsKeyDown(DIK_Q)) TheCamera->RotationLookVec(0.001f);
		if (g_pDInput->IsKeyDown(DIK_E)) TheCamera->RotationLookVec(-0.001f);*/

	//�Լ���һ����
	if (g_pDInput->IsKeyDown(DIK_I))
	{
		/*Transform playerTf;
		playerTf = player->getTransform();
		playerTf.position += {0,0,10};
		player->setTransform(&playerTf);*/
		//MessageBox(hwnd, _T("ʧ��~��"), _T("ǳī����Ϣ����"), 0); //ʹ��MessageBox����������һ����Ϣ���� 
	}

	D3DXMATRIX V;
	TheCamera.getViewMatrix(&V);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);

	//������������������������ת
	//TheCamera->RotationUpVec(g_pDInput->MouseDX()* 0.001f);
	//TheCamera->RotationRightVec(g_pDInput->MouseDY() * 0.001f);

	////�����ֿ��ƹ۲����������
	//static FLOAT fPosZ = 0.0f;
	//fPosZ += g_pDInput->MouseDZ()*0.03f;

	////���㲢����ȡ���任����
	//D3DXMATRIX matView;
	//TheCamera->CalculateViewMatrix(&matView);
	//g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	////����ȷ������任����浽g_matWorld��
	//D3DXMatrixTranslation(&g_matWorld, 0.0f, 0.0f, fPosZ);

	////������δ�����������������ƶ�����
	//POINT lt, rb;
	//RECT rect;
	//GetClientRect(hwnd, &rect);  //ȡ�ô����ڲ�����
	//							 //���������ϵ��������lt��
	//lt.x = rect.left;
	//lt.y = rect.top;
	////�����������������rb��
	//rb.x = rect.right;
	//rb.y = rect.bottom;
	////��lt��rb�Ĵ�������ת��Ϊ��Ļ����
	//ClientToScreen(hwnd, &lt);
	//ClientToScreen(hwnd, &rb);
	////����Ļ���������趨��������
	//rect.left = lt.x;
	//rect.top = lt.y;
	//rect.right = rb.x;
	//rect.bottom = rb.y;
	////����������ƶ�����
	//ClipCursor(&rect);

	//ShowCursor(false);		//���������
}

//-----------------------------------��Direct3D_Render( )������--------------------------------------
//	������ʹ��Direct3D������Ⱦ
//--------------------------------------------------------------------------------------------------
void Direct3D_Render(HWND hwnd)
{
	g_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0xffffffff, 1.0f, 0);
	g_pd3dDevice->BeginScene();

	//
	//�������壬���ڲ���
	//
	D3DXVECTOR3 cameraPos;
	TheCamera.getCameraPosition(&cameraPos);
	static wchar_t strInfo[256] = { 0 };
	swprintf_s(strInfo, -1, L"���������: (%.2f, %.2f, %.2f)", player->getTransform()->position.x, player->getTransform()->position.y, player->getTransform()->position.z);
	blackFont->drawText(50, 0, strInfo, d3d::BLACK);

	//
	//�������
	//
	D3DXMATRIX matSky, matTransSky, matRotSky;
	D3DXMatrixTranslation(&matTransSky, 0.0f, 0.0f, 0.0f);
	D3DXMatrixRotationY(&matRotSky, -0.000005f*timeGetTime());   //��ת�������, ��ģ���Ʋ��˶�Ч��
	matSky = matTransSky*matRotSky;
	skyBox->RenderSkyBox(&matSky, false);

	//
	//���Ƶ���
	//
	floorPlane->draw();

	//
	//���Ʊ߽�
	//
	for (int i = 0; i < 4; ++i)
	{
		cube[i]->draw();
	}

	//
	//��������
	//
	player->draw();

	//
	//����
	//
	//cube[0]->draw();

	g_pd3dDevice->EndScene();
	g_pd3dDevice->Present(0, 0, 0, 0);
}


//-----------------------------------��Direct3D_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
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