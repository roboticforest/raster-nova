/**
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2024 David Vitez
 * @license This Source Code Form is subject to the terms of the Mozilla Public
 *          License, v. 2.0. If a copy of the MPL was not distributed with this
 *          file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Windowing.h"
#include "../core/System.h"
#include "Colors.h"

#include <cassert>
#include <chrono>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>

namespace Rendering {

////////////////////////////////////////////////////////////////////////////////
/// Reasonable window display defaults.
////////////////////////////////////////////////////////////////////////////////

    static const int         DEFAULT_WINDOW_WIDTH          = 1024;
    static const int         DEFAULT_WINDOW_HEIGHT         = 768;
    static const std::string DEFAULT_WINDOW_TITLE          = "Gold Cartridge";
    static const SDL_Color   DEFAULT_CLEAR_COLOR           = Color::black();
    static const double      DEFAULT_UPDATE_INTERVAL       = 1000.00 / 60.0;
    static const int         DEFAULT_MAX_UPDATES_PER_FRAME = 4;

////////////////////////////////////////////////////////////////////////////////
/// Window constructors and destructors.
////////////////////////////////////////////////////////////////////////////////

    Window::Window(int window_width, int window_height, std::string window_title)
            : m_window_width(window_width),
              m_window_height(window_height),
              m_window_title(std::move(window_title)),
              m_update_interval_ms(DEFAULT_UPDATE_INTERVAL),
              m_max_updates_per_frame(DEFAULT_MAX_UPDATES_PER_FRAME),
              m_window(nullptr, SDL_DestroyWindow),
              m_renderer(nullptr, SDL_DestroyRenderer) {

        assert(Core::System::is_initialized());
        m_window.reset(SDL_CreateWindow(m_window_title.c_str(),
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        m_window_width,
                                        m_window_height,
                                        SDL_WINDOW_SHOWN));
        assert(m_window);

        constexpr int first_valid_driver = -1;
        m_renderer.reset(SDL_CreateRenderer(m_window.get(),
                                            first_valid_driver,
                                            SDL_RENDERER_ACCELERATED /*| SDL_RENDERER_PRESENTVSYNC*/));
        assert(m_renderer);

        m_window_is_open = true;
    }

    Window::Window() : Window(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_TITLE) {}

    Window::~Window() {
        m_renderer.reset();
        m_window.reset();
    }

////////////////////////////////////////////////////////////////////////////////
/// Public API functions and helpers.
////////////////////////////////////////////////////////////////////////////////

    void Window::set_user_update_callback(UpdateCallback update_fn) { m_process_user_updates = std::move(update_fn); }
    void Window::set_user_draw_callback(DrawCallback draw_fn) { m_process_user_rendering = std::move(draw_fn); }

    [[maybe_unused]] int Window::update_limit_per_frame() const { return m_max_updates_per_frame; }
    [[maybe_unused]] void Window::update_limit_per_frame(int new_update_limit) {
        if (new_update_limit <= 0) { m_max_updates_per_frame = 1; }
        else { m_max_updates_per_frame = new_update_limit; }
    }

    [[maybe_unused]] double Window::target_update_time_ms() const { return m_update_interval_ms.count(); }
    [[maybe_unused]] void Window::target_update_time_ms(double new_ms_interval) {
        m_update_interval_ms = Milliseconds(new_ms_interval);
    }

    [[maybe_unused]] int Window::width() const { return m_window_width; }
    [[maybe_unused]] int Window::height() const { return m_window_height; }

    /// Helper function for Window::run()
    static void START_WATCHING_FOR_WINDOW_CLOSE(Rendering::Window* window) {
        // NOTE: Multiple open windows are not supported by this code. SDL2
        // generates SDL_QUIT events when "the last" (or only) window is closed
        // vs when multiple windows are open, and that's not accounted for here
        // yet.

        // NOTE: An event filter allows us to handle quit events both before
        // framework users and without interfering with their event handling
        // code.
        auto quit_event_filter = [](void* window, SDL_Event* event) -> int {
            if (event->type == SDL_QUIT) { static_cast<Rendering::Window*>(window)->close(); }
            return 0; // Value will be ignored by SDL2.
        };
        SDL_AddEventWatch(quit_event_filter, window);
    }

    void Window::run() {

        START_WATCHING_FOR_WINDOW_CLOSE(this);

        using Clock = std::chrono::high_resolution_clock;
        using TimeStamp = std::chrono::time_point<Clock>;

        this->update(); // Create an initial state to render.

        Milliseconds lag_time(0.0);
        TimeStamp    previous_time = Clock::now();

        while (m_window_is_open) {
            TimeStamp    current_time = Clock::now();
            Milliseconds elapsed_time = current_time - previous_time;
            previous_time = current_time;
            lag_time += elapsed_time;

            int update_count = 0;
            while (lag_time >= m_update_interval_ms && update_count < m_max_updates_per_frame) {
                this->update();
                lag_time -= m_update_interval_ms;
                update_count++;
            }

            this->render();
        }
    }

    void Window::close() { m_window_is_open = false; }

////////////////////////////////////////////////////////////////////////////////
/// Private API functions and helpers.
////////////////////////////////////////////////////////////////////////////////

    void Window::update() {
        // Give users the first shot at consuming events.
        if (m_process_user_updates) { m_process_user_updates(); }

        // Processing SDL2's event queue *MUST* be done somewhere or the
        // window freezes, even if the events are just thrown away. SDL event
        // filters don't count, and users might not create their own event
        // processing loop.
        SDL_Event current_event;
        while (SDL_PollEvent(&current_event)) {}
    }

    void Window::render() {
        SDL_SetRenderDrawColor(m_renderer.get(),
                               DEFAULT_CLEAR_COLOR.r,
                               DEFAULT_CLEAR_COLOR.g,
                               DEFAULT_CLEAR_COLOR.b,
                               DEFAULT_CLEAR_COLOR.a);
        SDL_RenderClear(m_renderer.get());

        if (m_process_user_rendering) { m_process_user_rendering(m_renderer.get()); }

        SDL_RenderPresent(m_renderer.get());
    }

// TODO: Change the unique_ptr<Renderer> to a shared_ptr, and create an accessor function that
//  returns a weak_ptr reference. This will make it very easy to create buttons that have access
//  to the renderer for the main_window since user code generally only has access to the renderer
//  during render time.

} // Rendering namespace