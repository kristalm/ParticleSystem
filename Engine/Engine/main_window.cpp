#include <chrono>
#include "renderer.h"
#include "main_window.h"
#include "debug_renderer.h"
#include <time.h>
#include "pools.h"
namespace
{
	HWND create_window(int width, int height, void* user_data);
}

namespace end
{
#define particle_size 1000

	
	int main_window_t::launch(main_window_t& main_window)
	{
		main_window.hwnd = create_window(main_window.width, main_window.height, &main_window);

		if (main_window.hwnd == nullptr)
			return -1;
		//time_t start = time(0);
		auto last_time = std::chrono::high_resolution_clock::now();

		main_window.initialize();
		ShowWindow(main_window.hwnd, SW_SHOW);

		MSG msg = { nullptr };
		auto start = clock();
		end::renderer *render;

		//pool <particle, 800> free_list;
		//sorted_pool <particle2, 800> sorted_pool;
		//particle* particle_array[800];
		//particle temp_particle;

		
		render = new renderer(main_window.hwnd);
		while (true)
		{
			BOOL message_recieved = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);

			if (message_recieved)
			{
				if (msg.message == WM_QUIT)
				{
					main_window.shutdown();
					return (int)msg.wParam;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
#pragma region GRID
				/*float3 color;
				float3 red = { 1.0, 0.0f, 0.0f };
				float3 green = { 0.0f, 1.0f, 0.0f };
				float3 blue = { 0.0f, 0.0f, 1.0f };


				auto now = clock();
				float diff = (float)now - (float)start;
				auto sec = diff / CLOCKS_PER_SEC;

				auto ratio = (sec / 10) * 5;

				float3 c;

				if (sec >= 0 && sec < 3)
				{
					c = lerp(red, green, ratio);
				}
				if (sec >= 3 && sec < 8)
				{
					ratio = ((sec - 3) / 10) * 5;
					c = lerp(green, blue, ratio);
				}
				if (sec >= 8 && sec < 11)
				{
					ratio = ((sec - 8) / 10) * 5;
					c = lerp(blue, red, ratio);
				}
				if (sec >= 11)
				{
					start = clock();
					c = lerp(red, green, ratio);
				}

				for (float i = -20.0f; i <= 20.0f; i += 1.0f)
				{
					float3 point1 = { 0 };
					point1.x = i;
					point1.z = -20.0f;

					float3 point2 = { 0 };
					point2.x = i;
					point2.z = 20.0f;

					float3 point3 = { 0 };
					point3.x = -20.0f;
					point3.z = i;

					float3 point4 = { 0 };
					point4.x = 20.0f;
					point4.z = i;
					debug_renderer::add_line(point1, point2, c);
					debug_renderer::add_line(point3, point4, c);
				}*/
#pragma endregion GRID
#pragma region PARTICLES
				////Variables
				//auto particle_now = clock();
				//float difference = (float)particle_now - (float)particle_start;
				////auto timer = difference / CLOCKS_PER_SEC;

				//double t = delta_sec.count();
				//

				//float timer = delta_sec.count();
				//if (timer > 0.0f)
				//{
				//	seconds += timer;
				//	seconds1 += timer;
				//}
				////float3 acc = { -10.0f, -1000.0f, -10.0f };
				//float3 acc = { -10.0f, -1000.0f, -10.0f };
				//float3 pos, prev_pos, delta;

				//if (seconds > 1.0f)
				//{
				//	for (auto & i : particle_array)
				//	{
				//		if (i == nullptr)
				//		{
				//			particle _particle{};
				//			i = free_list.allocate(_particle);
				//		}
				//	}
				//	seconds = 0;
				//}

				//if (seconds1  > 1.0f)
				//{
				//	for (int i = 0; i < 800; i++)
				//	{

				//		if (sorted_pool.allocate() == nullptr)
				//			break;
				//	}
				//	seconds1 = 0;
				//}

				//for (auto & i : particle_array)
				//{
				//	if (i == nullptr)
				//		continue;

				//	debug_renderer::add_line(i->pos, i->prev_pos, i->color.xyz);
				//}

				//for (int i = 0; i < sorted_pool.activeParticle(); i++)
				//{
				//	debug_renderer::add_line(sorted_pool[i].pos, sorted_pool[i].prev_pos, sorted_pool[i].color.xyz);
				//}

				//for (auto & i : particle_array)
				//{
				//	if (i == nullptr)
				//	{
				//		particle Part{};

				//		i = free_list.allocate(Part);
				//		if (i == nullptr)
				//			continue;
				//	}


				//	pos = { i->pos.x, i->pos.y, i->pos.z };

				//	prev_pos = { i->prev_pos.x, i->prev_pos.y, i->prev_pos.z };


				//	delta = { pos.x - prev_pos.x, pos.y - prev_pos.y, pos.z - prev_pos.z };

				//	prev_pos = { pos.x, pos.y, pos.z };

				//	pos.x += (delta.x) + acc.x * (t * t);
				//	pos.y += (delta.y) + acc.y * (t * t);
				//	pos.z += (delta.z) + acc.z * (t * t);

				//	i->pos = { pos.x, pos.y, pos.z };

				//	i->prev_pos = { prev_pos.x, prev_pos.y, prev_pos.z };

				//	if (pos.y < 0)
				//	{
				//		free_list.free(i);
				//		i = nullptr;
				//	}

				//}

				//for (int i = 0; i < sorted_pool.activeParticle(); i++)
				//{
				//	pos = { sorted_pool[i].pos.x, sorted_pool[i].pos.y, sorted_pool[i].pos.z };

				//	prev_pos = { sorted_pool[i].prev_pos.x, sorted_pool[i].prev_pos.y, sorted_pool[i].prev_pos.z };

				//	delta = { pos.x - prev_pos.x, pos.y - prev_pos.y, pos.z - prev_pos.z };

				//	prev_pos = { pos.x, pos.y, pos.z };

				//	pos.x += (delta.x) + acc.x * (t * t);
				//	pos.y += (delta.y) + acc.y * (t * t);
				//	pos.z += (delta.z) + acc.z * (t * t);

				//	sorted_pool[i].pos= {pos.x, pos.y, pos.z};
				//	sorted_pool[i].prev_pos = {prev_pos.x, prev_pos.y, prev_pos.z};

				//	if (pos.y < 0)
				//	{
				//		sorted_pool.free(&sorted_pool[i]);

				//		sorted_pool.allocate();
				//	}

				//}

#pragma endregion PARTICLES
				auto current_time = std::chrono::high_resolution_clock::now();
				auto delta_us = std::chrono::duration_cast<std::chrono::microseconds>(current_time - last_time);
				last_time = current_time;

				
				main_window.update(delta_us.count());
				
				render->draw();
			}
		}
		delete render;
		return 0;
	}



	//HWND main_window_t::window_hwnd()
	//{
	//	return hwnd;
	//}

	LRESULT main_window_t::on_message(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CREATE:
		{
			CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
			break;
		}

		if (main_window_t* main_window = (main_window_t*)GetWindowLongPtr(hwnd, GWLP_USERDATA))
			return main_window->on_message(uMsg, wParam, lParam);

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

namespace
{
	HWND create_window(int width, int height, void* user_data)
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);

		WNDCLASSEX wcex;

		ZeroMemory(&wcex, sizeof(WNDCLASSEX));

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = end::WindowProc;
		wcex.hInstance = hInstance;
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszClassName = L"main_window_class";

		if (size_t wnd_class_id = RegisterClassEx(&wcex))
		{
			DWORD wnd_style = WS_POPUPWINDOW | WS_CAPTION;
			DWORD wnd_exstyle = 0;
			LPCTSTR wnd_title = L"main_window";

			return CreateWindowEx
			(
				wnd_exstyle, 
				(LPCWSTR)wnd_class_id, 
				wnd_title,
				wnd_style, 
				CW_USEDEFAULT, 
				0, 
				width, 
				height, 
				0, 
				0, 
				hInstance, 
				user_data
			);
		}

		return 0;
	}
}