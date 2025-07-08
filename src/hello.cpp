#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <deque>
#include <cmath>

static SDL_Window  * window   = nullptr;
static SDL_Renderer* renderer = nullptr;

bool DISPLAY_FPS = false;
std::deque<double> fps_window;
constexpr int FPS_WINDOW_SIZE {255};
float RENDER_SCALE {4.0};

class WanderingDot {
  private:
    float initial_x{};
    float initial_y{};
    float x;
    float y;
    float x_vel;
    float y_vel;
  public:
    WanderingDot(float start_x, float start_y,
                 float start_x_vel, float start_y_vel) :
                 x(start_x), y(start_y),
                 x_vel(start_x_vel), y_vel(start_y_vel) {
        initial_x = start_x;
        initial_y = start_y;
    }
    void update() {
        x += x_vel;
        y += y_vel;
        if (x > initial_x+10 || y > initial_y+10 || x < initial_x-10 || y < initial_y-10){
            float new_x_vel = x_vel*std::cos(0.01) - y_vel*std::sin(0.01);
            float new_y_vel = x_vel*std::sin(0.01) + y_vel*std::cos(0.01);
            x_vel = new_x_vel;
            y_vel = new_y_vel;
        }
    }
    void render() {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderPoint(renderer, x, y);
    }
};

WanderingDot THE_DOT(32, 32, 0.1, 0.1);

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    if (!SDL_CreateWindowAndRenderer("Hello World", 1600, 900, /*SDL_WINDOW_BORDERLESS*/0, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    std::cout << std::setw(24) << "System RAM: " << SDL_GetSystemRAM() << " MiB."<< std::endl;
    std::cout << std::setw(24) << "CPU L1 Cache: " << SDL_GetCPUCacheLineSize() << " bytes." << std::endl;
    std::cout << std::setw(24) << "CPU Logical Cores: " << SDL_GetNumLogicalCPUCores() << std::endl;
    std::cout << std::setw(24) << "SIMD Alignment: " << SDL_GetSIMDAlignment() << " bytes." << std::endl;

    const int compiled = SDL_VERSION;  /* hardcoded number from SDL headers */
    const int linked = SDL_GetVersion();  /* reported by linked SDL library */

    SDL_Log("We compiled against SDL version %d.%d.%d ...\n",
            SDL_VERSIONNUM_MAJOR(compiled),
            SDL_VERSIONNUM_MINOR(compiled),
            SDL_VERSIONNUM_MICRO(compiled));

    SDL_Log("But we are linking against SDL version %d.%d.%d.\n",
            SDL_VERSIONNUM_MAJOR(linked),
            SDL_VERSIONNUM_MINOR(linked),
            SDL_VERSIONNUM_MICRO(linked));

    auto rev = SDL_GetRevision();
    std::cout << "Revision: " << rev << std::endl;

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_KEY_DOWN) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_ESCAPE: return SDL_APP_SUCCESS;
            case SDL_SCANCODE_F: DISPLAY_FPS = true;
        }
    }
    if (event->type == SDL_EVENT_KEY_UP) {
        switch (event->key.scancode) {
            case SDL_SCANCODE_F: DISPLAY_FPS = false;
        }
    }
    if (event->type == SDL_EVENT_MOUSE_WHEEL) {
        if (SDL_GetModState() & SDL_KMOD_CTRL) {
            if (event->wheel.y == 1) { RENDER_SCALE += 0.5; }
            else if (event->wheel.y == -1) { RENDER_SCALE -= 0.5; }
            if (RENDER_SCALE <= 1.0) { RENDER_SCALE = 1.0; }
        }
    }

    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate) {
    static Uint64 last_tick = 0;
    Uint64        cur_tick  = 0;
    const char* message = "Hello World!";
    int               w = 0, h = 0;
    float             x, y;
    std::stringstream fpsStrStream;

    /* Center the message and scale it up */
    SDL_GetRenderOutputSize(renderer, &w, &h);
    SDL_SetRenderScale(renderer, RENDER_SCALE, RENDER_SCALE);

    x = ((w / RENDER_SCALE) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * SDL_strlen(message)) / 2;
    y = ((h / RENDER_SCALE) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

    /* Draw the message */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, x, y, message);

    // Calculate an FPS and save value into a rolling window.
    cur_tick = SDL_GetTicksNS();
    double timeDelta = (double)(cur_tick - last_tick) / 1000000000.0;
    last_tick = cur_tick;
    double fps = 1.0 / timeDelta;

    fps_window.push_back(fps);
    if (fps_window.size() > FPS_WINDOW_SIZE) {
        fps_window.pop_front();
    }

    // Calculate the rolling average for FPS.
    double total = 0.0;
    for (double fps_measurement : fps_window) {
        total += fps_measurement;
    }
    double avg_fps = total / fps_window.size();

    if (DISPLAY_FPS){
        fpsStrStream.precision(2);
        fpsStrStream << "FPS: " << std::fixed << fps;
        SDL_RenderDebugText(renderer, 0, (h / RENDER_SCALE) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * 2,
                            fpsStrStream.str().c_str());
        // std::cout << fpsStrStream.str() << " ";

        fpsStrStream.str("");
        fpsStrStream << "Avg FPS: " << avg_fps;
        SDL_RenderDebugText(renderer, 0, (h / RENDER_SCALE) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE,
                            fpsStrStream.str().c_str());
        // std::cout << fpsStrStream.str() << std::endl;
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128);
    SDL_RenderPoint(renderer, 5, 5);
    SDL_RenderPoint(renderer, 15, 8);
    SDL_RenderPoint(renderer, 3, 12);

    THE_DOT.update();
    THE_DOT.render();

    SDL_RenderPresent(renderer);
    // SDL_Delay(16);

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
}
