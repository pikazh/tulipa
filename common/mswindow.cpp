#include "mswindow.h"
#include "resource.h"
#include "easy_app.h"

#include <string>
#include <cassert>

BEGIN_NS_TULIPA

ATOM MsWindow::wnd_class_ = 0;
const std::wstring MsWindow::wnd_class_name_ = L"tulipa_appwin";

LRESULT CALLBACK MsWindow::WinProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static MsWindow* wnd = dynamic_cast<MsWindow*>(Easy_App::global_instance()->gui()->main_window());
	if(wnd == nullptr)
		return ::DefWindowProcW(hwnd, message, wparam, lparam);

	return wnd->ProcessMessage(hwnd, message, wparam, lparam);
}

MsWindow::MsWindow()
{
	hwnd_ = nullptr;
	create_signal_ = std::make_unique<Signal<>>();
	idle_signal_ = std::make_unique<Signal<>>();
	draw_signal_ = std::make_unique<Signal<>>();
}

tulipa::MsWindow::~MsWindow()
{
	create_signal_.reset();
	idle_signal_.reset();
	draw_signal_.reset();
}

void MsWindow::SetIdleHandler(std::function<void(void)> handler)
{
	idle_signal_->Connect(handler);
}

void MsWindow::SetDrawHandler(std::function<void(void)> handler)
{
	draw_signal_->Connect(handler);
}

void MsWindow::SetCreateHandler(std::function<void(void)> handler)
{
	create_signal_->Connect(handler);
}

void MsWindow::OnIdle()
{
	idle_signal_->Emit();
}

bool MsWindow::Create()
{
	if (hwnd_ != nullptr && ::IsWindow(hwnd_))
	{
		return false;
	}

	if (wnd_class_ == 0)
	{
		wnd_class_ = RegisterWindowClass();
	}

	int width = 800;
	int height = 600;
	int left = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int top = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	hwnd_ = ::CreateWindowW(wnd_class_name_.c_str(), L"", WS_OVERLAPPEDWINDOW,
		left, top, width, height, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
	if (hwnd_ == nullptr)
	{
		DWORD dwErr = ::GetLastError();
		std::wstring errText = L"Create window failed with errcode " + std::to_wstring(dwErr);
		::MessageBoxW(nullptr, errText.c_str(), nullptr, MB_OK | MB_ICONERROR);
		return false;
	}

	
	ShowWindow(hwnd_, SW_SHOW);
	UpdateWindow(hwnd_);

	return true;
}

ATOM MsWindow::RegisterWindowClass()
{
	HINSTANCE hinst = GetModuleHandle(nullptr);
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &WinProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinst;
	wcex.hIcon = LoadIconW(hinst, MAKEINTRESOURCEW(IDI_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = wnd_class_name_.c_str();
	wcex.hIconSm = LoadIconW(hinst, MAKEINTRESOURCEW(IDI_ICON));

	return RegisterClassExW(&wcex);
}

LRESULT MsWindow::ProcessMessage(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CREATE:
		create_signal_->Emit();
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);
		draw_signal_->Emit();
		EndPaint(hwnd, &ps);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProcW(hwnd, message, wparam, lparam);
	}
	return 0;
}

END_NS_TULIPA
