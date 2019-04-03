#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include <cstdint>
#include "math_types.h"
#include "debug_renderer.h"
#include "pools.h"

int main(int argc, char* argv[]);

namespace end
{
	class main_window_t
	{
	public:
		main_window_t() = default;
		HWND window_hwnd() { return hwnd; }

	protected:
		HWND hwnd = 0;
		int width = 1280;
		int height = 720;

		//grid
		float sec = 0;
		float seconds = 0;
		float seconds1 = 0;
	private:
		struct particle
		{
			float3 pos, prev_pos;
			float4 color;

			particle()
			{
				pos = { 0,0,0 };

				prev_pos = { rand() % 10 * -0.0015f , rand() % 10 * -0.005f , rand() % 10 * -0.0015f };

				color = { rand() % 10 * 0.10f, rand() % 10 * 0.10f, rand() % 10 * 0.10f };
				color.w = 1.0f;

			}
		};
		struct particle2
		{
			float3 pos;
			float3 prev_pos;
			float4 color;

			particle2()
			{
				pos = { 0,0,0 };

				prev_pos = { rand() % 10 * 0.0015f, rand() % 10 * -0.005f, rand() % 10 * 0.0015f };

				color = { rand() % 10 * 0.10f, rand() % 10 * 0.10f, rand() % 10 * 0.10f };
				color.w = 1.0f;

			}
		};

		pool <particle, 800> free_list;
		sorted_pool <particle2, 800> sorted_pool;
		particle* particle_array[800];
		particle temp_particle;

		friend int ::main(int argc, char* argv[]);

		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		inline virtual void update(int64_t us) 
		{
#pragma region GRID
			sec += (us * 0.000001f);
			float3 red = { 1.0, 0.0f, 0.0f };
			float3 green = { 0.0f, 1.0f, 0.0f };
			float3 blue = { 0.0f, 0.0f, 1.0f };

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
				sec = 0;
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
			}
#pragma endregion GRID
#pragma region PARTICLE
			seconds = 0;
			seconds += (us * 0.000001f);
			float t = (us * 0.000001f);

			float3 acc = { 0.0f, -1.0f, 0.0f };
			float3 pos, prev_pos, delta;

			if (seconds < 0.8f)
			{
				for (auto & i : particle_array)
				{
					if (i == nullptr)
					{
						particle _particle{};
						i = free_list.allocate(_particle);
					}
				}
				
			}

			if (seconds < 0.8f)
			{
				for (int i = 0; i < 800; i++)
				{

					if (sorted_pool.allocate() == nullptr)
						break;
				}
			}

			for (auto & i : particle_array)
			{
				if (i == nullptr)
					continue;

				debug_renderer::add_line(i->pos, i->prev_pos, i->color.xyz);
			}

			for (int i = 0; i < sorted_pool.activeParticle(); i++)
			{
				debug_renderer::add_line(sorted_pool[i].pos, sorted_pool[i].prev_pos, sorted_pool[i].color.xyz);
			}

			for (auto & i : particle_array)
			{
				if (i == nullptr)
				{
					particle Part{};

					i = free_list.allocate(Part);
					if (i == nullptr)
						continue;
				}


				pos = { i->pos.x, i->pos.y, i->pos.z };

				prev_pos = { i->prev_pos.x, i->prev_pos.y, i->prev_pos.z };


				delta = { pos.x - prev_pos.x, pos.y - prev_pos.y, pos.z - prev_pos.z };

				prev_pos = { pos.x, pos.y, pos.z };

				pos.x += (delta.x) + acc.x * (t);
				pos.y += (delta.y) + acc.y * (t);
				pos.z += (delta.z) + acc.z * (t);

				i->pos = { pos.x, pos.y, pos.z };

				i->prev_pos = { prev_pos.x, prev_pos.y, prev_pos.z };

				if (pos.y < 0 || pos.y > 100)
				{
					free_list.free(i);
					i = nullptr;
				}

			}

			for (int i = 0; i < sorted_pool.activeParticle(); i++)
			{
				pos = { sorted_pool[i].pos.x, sorted_pool[i].pos.y, sorted_pool[i].pos.z };

				prev_pos = { sorted_pool[i].prev_pos.x, sorted_pool[i].prev_pos.y, sorted_pool[i].prev_pos.z };

				delta = { pos.x - prev_pos.x, pos.y - prev_pos.y, pos.z - prev_pos.z };

				prev_pos = { pos.x, pos.y, pos.z };

				pos.x += (delta.x) + acc.x * (t);
				pos.y += (delta.y) + acc.y * (t);
				pos.z += (delta.z) + acc.z * (t);

				sorted_pool[i].pos = { pos.x, pos.y, pos.z };
				sorted_pool[i].prev_pos = { prev_pos.x, prev_pos.y, prev_pos.z };

				if (pos.y < 0 || pos.y > 100)
				{
					sorted_pool.free(&sorted_pool[i]);

					sorted_pool.allocate();
				}

			}

#pragma endregion PARTICLE
		};

		inline virtual void initialize() 
		{
			for (auto & i : particle_array)
			{
				i = nullptr;
			}
		};

		inline virtual void shutdown() {};

		virtual LRESULT on_message(UINT uMsg, WPARAM wParam, LPARAM lParam);

		static int launch(main_window_t& main_window);

		
	};
};