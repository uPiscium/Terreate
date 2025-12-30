#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <spdlog/spdlog.h>

int main() {
  // 1. spdlogの動作確認
  spdlog::info("=== Terreate Environment Check Start ===");

  // 2. GLMの動作確認
  // 単にvec3を定義してログに出すことで、リンクとヘッダが正しいか確認
  glm::vec3 v(1.0f, 2.0f, 3.0f);
  spdlog::info("GLM Check: vec3({}, {}, {})", v.x, v.y, v.z);

  // 3. SDL3初期化
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    spdlog::error("SDL_Init Failed: {}", SDL_GetError());
    return -1;
  }

  // ウィンドウ作成
  SDL_Window *window =
      SDL_CreateWindow("Terreate Build Check", 800, 600, SDL_WINDOW_RESIZABLE);
  if (!window) {
    spdlog::error("Window Creation Failed: {}", SDL_GetError());
    return -1;
  }

  // 4. SDL3 GPUデバイス作成 (ここが最重要)
  // Vulkan, Metal, D3D12などを自動選択
  SDL_GPUDevice *gpuDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV |
                                                     SDL_GPU_SHADERFORMAT_DXIL |
                                                     SDL_GPU_SHADERFORMAT_MSL,
                                                 true, // debug_mode
                                                 nullptr);

  if (!gpuDevice) {
    spdlog::error("GPU Device Creation Failed: {}", SDL_GetError());
    return -1;
  }
  spdlog::info("SDL3 GPU Device Created. Backend: {}",
               SDL_GetGPUDeviceDriver(gpuDevice));

  // ウィンドウとGPUを紐付け
  if (!SDL_ClaimWindowForGPUDevice(gpuDevice, window)) {
    spdlog::error("Claim Window Failed: {}", SDL_GetError());
    return -1;
  }

  // メインループ
  bool running = true;
  spdlog::info("Entering Main Loop. Press ESC to quit.");

  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
          spdlog::info("ESC pressed. Exiting.");
          running = false;
        }
      }
    }

    // --- 最小限の描画処理 (画面を緑にする) ---
    SDL_GPUCommandBuffer *cmdBuf = SDL_AcquireGPUCommandBuffer(gpuDevice);
    if (cmdBuf) {
      SDL_GPUTexture *swapchainTex;
      if (SDL_AcquireGPUSwapchainTexture(cmdBuf, window, &swapchainTex, nullptr,
                                         nullptr)) {

        SDL_GPUColorTargetInfo colorInfo = {};
        colorInfo.texture = swapchainTex;
        colorInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        colorInfo.store_op = SDL_GPU_STOREOP_STORE;
        colorInfo.clear_color = {0.0f, 1.0f, 0.0f, 1.0f}; // 緑色

        SDL_GPURenderPass *pass =
            SDL_BeginGPURenderPass(cmdBuf, &colorInfo, 1, nullptr);
        SDL_EndGPURenderPass(pass);

        SDL_SubmitGPUCommandBuffer(cmdBuf);
      } else {
        SDL_CancelGPUCommandBuffer(cmdBuf);
      }
    }
  }

  // クリーンアップ
  SDL_ReleaseWindowFromGPUDevice(gpuDevice, window);
  SDL_DestroyGPUDevice(gpuDevice);
  SDL_DestroyWindow(window);
  SDL_Quit();

  spdlog::info("=== Terreate Environment Check Passed ===");
  return 0;
}
