/**
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2023-2024 David Vitez
 * @license This Source Code Form is subject to the terms of the Mozilla Public
 *          License, v. 2.0. If a copy of the MPL was not distributed with this
 *          file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "core/System.h"
#include "rendering/Windowing.h"
#include "ui/Button.h"
#include <SDL.h>

int main(int num_args, char** args) {
    (void)num_args; // Ignored.
    (void)args;     // Ignored.
    Core::System::start_up();
    Rendering::Window main_window;

    main_window.set_user_update_callback([&]() {
        // Your update code here...
    });

    main_window.set_user_draw_callback([&](SDL_Renderer* renderer) {
        // Your graphics rendering code here...
    });

    main_window.run();
    return 0;
}