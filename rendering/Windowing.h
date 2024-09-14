/**
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2024 David Vitez
 * @license This Source Code Form is subject to the terms of the Mozilla Public
 *          License, v. 2.0. If a copy of the MPL was not distributed with this
 *          file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef GOLD_CARTRIDGE_WINDOWING_H
#define GOLD_CARTRIDGE_WINDOWING_H

#include <chrono>
#include <functional>
#include <memory>
#include <string>

struct SDL_Window;
struct SDL_Renderer;

namespace Rendering {

    class Window {
    public:
        using UpdateCallback = std::function<void()>;
        using DrawCallback = std::function<void(SDL_Renderer* renderer)>;
        using Milliseconds = std::chrono::duration<double, std::milli>;
        using SDL_WindowPtr = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>;
        using SDL_RendererPtr = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)>;

    public:
        Window();
        Window(int window_width, int window_height, std::string window_title);
        ~Window();

        void set_user_update_callback(UpdateCallback update_fn);
        void set_user_draw_callback(DrawCallback draw_fn);

        int update_limit_per_frame() const;
        void update_limit_per_frame(int new_update_limit);
        double target_update_time_ms() const;
        void target_update_time_ms(double new_ms_interval);

        int width() const;
        int height() const;

        void run();
        void close();

    private:
        void update();
        void render();

    private:
        SDL_RendererPtr m_renderer;
        SDL_WindowPtr   m_window;
        int             m_window_width;
        int             m_window_height;
        std::string     m_window_title;
        bool            m_window_is_open;

        int            m_max_updates_per_frame;
        Milliseconds   m_update_interval_ms;
        UpdateCallback m_process_user_updates;
        DrawCallback   m_process_user_rendering;
    };

} // Rendering namespace

#endif //GOLD_CARTRIDGE_WINDOWING_H
