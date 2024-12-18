//External includes
#ifdef ENABLE_VLD
#include "vld.h"
#endif
#include "SDL.h"
#include "SDL_surface.h"

#undef main

//Standard includes
#include <iostream>

//Project includes
#include "Timer.h"
#include "Renderer.h"

using namespace dae;

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	const uint32_t width = 640;
	const uint32_t height = 480;

	SDL_Window* pWindow = SDL_CreateWindow(
		"Rasterizer - Ivans Minajevs",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	const auto pTimer = new Timer();
	const auto pRenderer = new Renderer(pWindow);

	//Start loop
	pTimer->Start();

	// Start Benchmark
	// TODO pTimer->StartBenchmark();

	float printTimer = 0.f;
	bool isLooping = true;
	bool takeScreenshot = false;
	while (isLooping)
	{
		//--------- Get input events ---------
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				if (e.key.keysym.scancode == SDL_SCANCODE_X)
					takeScreenshot = true;

				if (e.key.keysym.scancode == SDL_SCANCODE_F4)
				{
					
					if(pRenderer->GetDisplayMode() == Renderer::DisplayMode::FinalColor)
					{
						std::cout << "Current display mode: DEPTH BUFFER" << std::endl;
						pRenderer->SetDisplayMode(Renderer::DisplayMode::DepthBuffer);
					}
					else
					{
						std::cout << "Current display mode: FINAL COLOR" << std::endl;
						pRenderer->SetDisplayMode(Renderer::DisplayMode::FinalColor);
					}
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_F5)
				{
					if (pRenderer->GetIsRotating())
					{
						std::cout << "Rotation: OFF" << std::endl;
						pRenderer->SetIsRotating(false);
					}
					else
					{
						std::cout << "Rotation: ON" << std::endl;
						pRenderer->SetIsRotating(true);
					}
				}
				if (e.key.keysym.scancode == SDL_SCANCODE_F6)
				{
					if (pRenderer->GetIsNormalMap())
					{
						std::cout << "Normal map: OFF" << std::endl;
						pRenderer->SetIsNormalMap(false);
					}
					else
					{
						std::cout << "Normal map: ON" << std::endl;
						pRenderer->SetIsNormalMap(true);
					}
					
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_F7)
				{
					if (pRenderer->GetDisplayMode() == Renderer::DisplayMode::ShadingMode)
					{
						pRenderer->CycleShadingMode();
					}
					else
					{
						std::cout << "Current display mode: SHADING" << std::endl;
						pRenderer->SetDisplayMode(Renderer::DisplayMode::ShadingMode);
					}
				}
				break;
			}
		}

		//--------- Update ---------
		pRenderer->Update(pTimer);

		//--------- Render ---------
		pRenderer->Render();

		//--------- Timer ---------
		pTimer->Update();
		printTimer += pTimer->GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "dFPS: " << pTimer->GetdFPS() << std::endl;
		}

		//Save screenshot after full render
		if (takeScreenshot)
		{
			if (!pRenderer->SaveBufferToImage())
				std::cout << "Screenshot saved!" << std::endl;
			else
				std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
			takeScreenshot = false;
		}
	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pRenderer;
	delete pTimer;

	ShutDown(pWindow);
	return 0;
}