/**
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2024 David Vitez
 * @license This Source Code Form is subject to the terms of the Mozilla Public
 *          License, v. 2.0. If a copy of the MPL was not distributed with this
 *          file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef GOLD_CARTRIDGE_SYSTEM_H
#define GOLD_CARTRIDGE_SYSTEM_H

namespace Core {

    class System {
    public:
        static bool is_initialized();
        static bool start_up();
        static void shut_down();

        System(const System&) = delete;
        void operator=(const System&) = delete;

    private:
        System();
        ~System();

    private:
        static bool m_core_system_initialized;
    };

} // Core

#endif //GOLD_CARTRIDGE_SYSTEM_H
