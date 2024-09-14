/**
 * @file Colors.h
 * @brief A collection of all 148 standard web colors.
 *
 * For more information visit
 * [W3Schools](https://www.w3schools.com/colors/colors_names.asp)
 * or search online for "web colors".
 *
 * @author David Vitez (AKA: Robotic Forest)
 * @copyright All rights reserved © 2023 David Vitez
 * @par License:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef GOLD_CARTRIDGE_COLORS_H
#define GOLD_CARTRIDGE_COLORS_H

#include <SDL_pixels.h>

namespace Color {

    /**
    * @brief Gets color values of the given name.
    *
    * This function returns an SDL_Color struct initialized with the RGBA values
    * corresponding to the specified web color name.
    */
    ///@{
    [[maybe_unused]] inline SDL_Color aliceBlue()            { return SDL_Color {240, 248, 255, 255}; }
    [[maybe_unused]] inline SDL_Color antiqueWhite()         { return SDL_Color {250, 235, 215, 255}; }
    [[maybe_unused]] inline SDL_Color aqua()                 { return SDL_Color {0, 255, 255, 255}; }
    [[maybe_unused]] inline SDL_Color aquamarine()           { return SDL_Color {127, 255, 212, 255}; }
    [[maybe_unused]] inline SDL_Color azure()                { return SDL_Color {240, 255, 255, 255}; }
    [[maybe_unused]] inline SDL_Color beige()                { return SDL_Color {245, 245, 220, 255}; }
    [[maybe_unused]] inline SDL_Color bisque()               { return SDL_Color {255, 228, 196, 255}; }
    [[maybe_unused]] inline SDL_Color black()                { return SDL_Color {0, 0, 0, 255}; }
    [[maybe_unused]] inline SDL_Color blanchedAlmond()       { return SDL_Color {255, 235, 205, 255}; }
    [[maybe_unused]] inline SDL_Color blue()                 { return SDL_Color {0, 0, 255, 255}; }
    [[maybe_unused]] inline SDL_Color blueViolet()           { return SDL_Color {138, 43, 226, 255}; }
    [[maybe_unused]] inline SDL_Color brown()                { return SDL_Color {165, 42, 42, 255}; }
    [[maybe_unused]] inline SDL_Color burlyWood()            { return SDL_Color {222, 184, 135, 255}; }
    [[maybe_unused]] inline SDL_Color cadetBlue()            { return SDL_Color {95, 158, 160, 255}; }
    [[maybe_unused]] inline SDL_Color chartreuse()           { return SDL_Color {127, 255, 0, 255}; }
    [[maybe_unused]] inline SDL_Color chocolate()            { return SDL_Color {210, 105, 30, 255}; }
    [[maybe_unused]] inline SDL_Color coral()                { return SDL_Color {255, 127, 80, 255}; }
    [[maybe_unused]] inline SDL_Color cornflowerBlue()       { return SDL_Color {100, 149, 237, 255}; }
    [[maybe_unused]] inline SDL_Color cornsilk()             { return SDL_Color {255, 248, 220, 255}; }
    [[maybe_unused]] inline SDL_Color crimson()              { return SDL_Color {220, 20, 60, 255}; }
    [[maybe_unused]] inline SDL_Color cyan()                 { return SDL_Color {0, 255, 255, 255}; }
    [[maybe_unused]] inline SDL_Color darkBlue()             { return SDL_Color {0, 0, 139, 255}; }
    [[maybe_unused]] inline SDL_Color darkCyan()             { return SDL_Color {0, 139, 139, 255}; }
    [[maybe_unused]] inline SDL_Color darkGoldenRod()        { return SDL_Color {184, 134, 11, 255}; }
    [[maybe_unused]] inline SDL_Color darkGray()             { return SDL_Color {169, 169, 169, 255}; }
    [[maybe_unused]] inline SDL_Color darkGreen()            { return SDL_Color {0, 100, 0, 255}; }
    [[maybe_unused]] inline SDL_Color darkGrey()             { return SDL_Color {169, 169, 169, 255}; }
    [[maybe_unused]] inline SDL_Color darkKhaki()            { return SDL_Color {189, 183, 107, 255}; }
    [[maybe_unused]] inline SDL_Color darkMagenta()          { return SDL_Color {139, 0, 139, 255}; }
    [[maybe_unused]] inline SDL_Color darkOliveGreen()       { return SDL_Color {85, 107, 47, 255}; }
    [[maybe_unused]] inline SDL_Color darkOrange()           { return SDL_Color {255, 140, 0, 255}; }
    [[maybe_unused]] inline SDL_Color darkOrchid()           { return SDL_Color {153, 50, 204, 255}; }
    [[maybe_unused]] inline SDL_Color darkRed()              { return SDL_Color {139, 0, 0, 255}; }
    [[maybe_unused]] inline SDL_Color darkSalmon()           { return SDL_Color {233, 150, 122, 255}; }
    [[maybe_unused]] inline SDL_Color darkSeaGreen()         { return SDL_Color {143, 188, 143, 255}; }
    [[maybe_unused]] inline SDL_Color darkSlateBlue()        { return SDL_Color {72, 61, 139, 255}; }
    [[maybe_unused]] inline SDL_Color darkSlateGray()        { return SDL_Color {47, 79, 79, 255}; }
    [[maybe_unused]] inline SDL_Color darkSlateGrey()        { return SDL_Color {47, 79, 79, 255}; }
    [[maybe_unused]] inline SDL_Color darkTurquoise()        { return SDL_Color {0, 206, 209, 255}; }
    [[maybe_unused]] inline SDL_Color darkViolet()           { return SDL_Color {148, 0, 211, 255}; }
    [[maybe_unused]] inline SDL_Color deepPink()             { return SDL_Color {255, 20, 147, 255}; }
    [[maybe_unused]] inline SDL_Color deepSkyBlue()          { return SDL_Color {0, 191, 255, 255}; }
    [[maybe_unused]] inline SDL_Color dimGray()              { return SDL_Color {105, 105, 105, 255}; }
    [[maybe_unused]] inline SDL_Color dimGrey()              { return SDL_Color {105, 105, 105, 255}; }
    [[maybe_unused]] inline SDL_Color dodgerBlue()           { return SDL_Color {30, 144, 255, 255}; }
    [[maybe_unused]] inline SDL_Color firebrick()            { return SDL_Color {178, 34, 34, 255}; }
    [[maybe_unused]] inline SDL_Color floralWhite()          { return SDL_Color {255, 250, 240, 255}; }
    [[maybe_unused]] inline SDL_Color forestGreen()          { return SDL_Color {34, 139, 34, 255}; }
    [[maybe_unused]] inline SDL_Color fuchsia()              { return SDL_Color {255, 0, 255, 255}; }
    [[maybe_unused]] inline SDL_Color gainsboro()            { return SDL_Color {220, 220, 220, 255}; }
    [[maybe_unused]] inline SDL_Color ghostWhite()           { return SDL_Color {248, 248, 255, 255}; }
    [[maybe_unused]] inline SDL_Color gold()                 { return SDL_Color {255, 215, 0, 255}; }
    [[maybe_unused]] inline SDL_Color goldenRod()            { return SDL_Color {218, 165, 32, 255}; }
    [[maybe_unused]] inline SDL_Color gray()                 { return SDL_Color {128, 128, 128, 255}; }
    [[maybe_unused]] inline SDL_Color green()                { return SDL_Color {0, 128, 0, 255}; }
    [[maybe_unused]] inline SDL_Color greenYellow()          { return SDL_Color {173, 255, 47, 255}; }
    [[maybe_unused]] inline SDL_Color grey()                 { return SDL_Color {128, 128, 128, 255}; }
    [[maybe_unused]] inline SDL_Color honeydew()             { return SDL_Color {240, 255, 240, 255}; }
    [[maybe_unused]] inline SDL_Color hotPink()              { return SDL_Color {255, 105, 180, 255}; }
    [[maybe_unused]] inline SDL_Color indianRed()            { return SDL_Color {205, 92, 92, 255}; }
    [[maybe_unused]] inline SDL_Color indigo()               { return SDL_Color {75, 0, 130, 255}; }
    [[maybe_unused]] inline SDL_Color ivory()                { return SDL_Color {255, 255, 240, 255}; }
    [[maybe_unused]] inline SDL_Color khaki()                { return SDL_Color {240, 230, 140, 255}; }
    [[maybe_unused]] inline SDL_Color lavender()             { return SDL_Color {230, 230, 250, 255}; }
    [[maybe_unused]] inline SDL_Color lavenderBlush()        { return SDL_Color {255, 240, 245, 255}; }
    [[maybe_unused]] inline SDL_Color lawnGreen()            { return SDL_Color {124, 252, 0, 255}; }
    [[maybe_unused]] inline SDL_Color lemonChiffon()         { return SDL_Color {255, 250, 205, 255}; }
    [[maybe_unused]] inline SDL_Color lightBlue()            { return SDL_Color {173, 216, 230, 255}; }
    [[maybe_unused]] inline SDL_Color lightCoral()           { return SDL_Color {240, 128, 128, 255}; }
    [[maybe_unused]] inline SDL_Color lightCyan()            { return SDL_Color {224, 255, 255, 255}; }
    [[maybe_unused]] inline SDL_Color lightGoldenRodYellow() { return SDL_Color {250, 250, 210, 255}; }
    [[maybe_unused]] inline SDL_Color lightGray()            { return SDL_Color {211, 211, 211, 255}; }
    [[maybe_unused]] inline SDL_Color lightGreen()           { return SDL_Color {144, 238, 144, 255}; }
    [[maybe_unused]] inline SDL_Color lightGrey()            { return SDL_Color {211, 211, 211, 255}; }
    [[maybe_unused]] inline SDL_Color lightPink()            { return SDL_Color {255, 182, 193, 255}; }
    [[maybe_unused]] inline SDL_Color lightSalmon()          { return SDL_Color {255, 160, 122, 255}; }
    [[maybe_unused]] inline SDL_Color lightSeaGreen()        { return SDL_Color {32, 178, 170, 255}; }
    [[maybe_unused]] inline SDL_Color lightSkyBlue()         { return SDL_Color {135, 206, 250, 255}; }
    [[maybe_unused]] inline SDL_Color lightSlateGray()       { return SDL_Color {119, 136, 153, 255}; }
    [[maybe_unused]] inline SDL_Color lightSlateGrey()       { return SDL_Color {119, 136, 153, 255}; }
    [[maybe_unused]] inline SDL_Color lightSteelBlue()       { return SDL_Color {176, 196, 222, 255}; }
    [[maybe_unused]] inline SDL_Color lightYellow()          { return SDL_Color {255, 255, 224, 255}; }
    [[maybe_unused]] inline SDL_Color lime()                 { return SDL_Color {0, 255, 0, 255}; }
    [[maybe_unused]] inline SDL_Color limeGreen()            { return SDL_Color {50, 205, 50, 255}; }
    [[maybe_unused]] inline SDL_Color linen()                { return SDL_Color {250, 240, 230, 255}; }
    [[maybe_unused]] inline SDL_Color magenta()              { return SDL_Color {255, 0, 255, 255}; }
    [[maybe_unused]] inline SDL_Color maroon()               { return SDL_Color {128, 0, 0, 255}; }
    [[maybe_unused]] inline SDL_Color mediumAquamarine()     { return SDL_Color {102, 205, 170, 255}; }
    [[maybe_unused]] inline SDL_Color mediumBlue()           { return SDL_Color {0, 0, 205, 255}; }
    [[maybe_unused]] inline SDL_Color mediumOrchid()         { return SDL_Color {186, 85, 211, 255}; }
    [[maybe_unused]] inline SDL_Color mediumPurple()         { return SDL_Color {147, 112, 219, 255}; }
    [[maybe_unused]] inline SDL_Color mediumSeaGreen()       { return SDL_Color {60, 179, 113, 255}; }
    [[maybe_unused]] inline SDL_Color mediumSlateBlue()      { return SDL_Color {123, 104, 238, 255}; }
    [[maybe_unused]] inline SDL_Color mediumSpringGreen()    { return SDL_Color {0, 250, 154, 255}; }
    [[maybe_unused]] inline SDL_Color mediumTurquoise()      { return SDL_Color {72, 209, 204, 255}; }
    [[maybe_unused]] inline SDL_Color mediumVioletRed()      { return SDL_Color {199, 21, 133, 255}; }
    [[maybe_unused]] inline SDL_Color midnightBlue()         { return SDL_Color {25, 25, 112, 255}; }
    [[maybe_unused]] inline SDL_Color mintCream()            { return SDL_Color {245, 255, 250, 255}; }
    [[maybe_unused]] inline SDL_Color mistyRose()            { return SDL_Color {255, 228, 225, 255}; }
    [[maybe_unused]] inline SDL_Color moccasin()             { return SDL_Color {255, 228, 181, 255}; }
    [[maybe_unused]] inline SDL_Color navajoWhite()          { return SDL_Color {255, 222, 173, 255}; }
    [[maybe_unused]] inline SDL_Color navy()                 { return SDL_Color {0, 0, 128, 255}; }
    [[maybe_unused]] inline SDL_Color oldLace()              { return SDL_Color {253, 245, 230, 255}; }
    [[maybe_unused]] inline SDL_Color olive()                { return SDL_Color {128, 128, 0, 255}; }
    [[maybe_unused]] inline SDL_Color oliveDrab()            { return SDL_Color {107, 142, 35, 255}; }
    [[maybe_unused]] inline SDL_Color orange()               { return SDL_Color {255, 165, 0, 255}; }
    [[maybe_unused]] inline SDL_Color orangeRed()            { return SDL_Color {255, 69, 0, 255}; }
    [[maybe_unused]] inline SDL_Color orchid()               { return SDL_Color {218, 112, 214, 255}; }
    [[maybe_unused]] inline SDL_Color paleGoldenRod()        { return SDL_Color {238, 232, 170, 255}; }
    [[maybe_unused]] inline SDL_Color paleGreen()            { return SDL_Color {152, 251, 152, 255}; }
    [[maybe_unused]] inline SDL_Color paleTurquoise()        { return SDL_Color {175, 238, 238, 255}; }
    [[maybe_unused]] inline SDL_Color paleVioletRed()        { return SDL_Color {219, 112, 147, 255}; }
    [[maybe_unused]] inline SDL_Color papayaWhip()           { return SDL_Color {255, 239, 213, 255}; }
    [[maybe_unused]] inline SDL_Color peachPuff()            { return SDL_Color {255, 218, 185, 255}; }
    [[maybe_unused]] inline SDL_Color peru()                 { return SDL_Color {205, 133, 63, 255}; }
    [[maybe_unused]] inline SDL_Color pink()                 { return SDL_Color {255, 192, 203, 255}; }
    [[maybe_unused]] inline SDL_Color plum()                 { return SDL_Color {221, 160, 221, 255}; }
    [[maybe_unused]] inline SDL_Color powderBlue()           { return SDL_Color {176, 224, 230, 255}; }
    [[maybe_unused]] inline SDL_Color purple()               { return SDL_Color {128, 0, 128, 255}; }
    [[maybe_unused]] inline SDL_Color rebeccaPurple()        { return SDL_Color {102, 51, 153, 255}; }
    [[maybe_unused]] inline SDL_Color red()                  { return SDL_Color {255, 0, 0, 255}; }
    [[maybe_unused]] inline SDL_Color rosyBrown()            { return SDL_Color {188, 143, 143, 255}; }
    [[maybe_unused]] inline SDL_Color royalBlue()            { return SDL_Color {65, 105, 225, 255}; }
    [[maybe_unused]] inline SDL_Color saddleBrown()          { return SDL_Color {139, 69, 19, 255}; }
    [[maybe_unused]] inline SDL_Color salmon()               { return SDL_Color {250, 128, 114, 255}; }
    [[maybe_unused]] inline SDL_Color sandyBrown()           { return SDL_Color {244, 164, 96, 255}; }
    [[maybe_unused]] inline SDL_Color seaGreen()             { return SDL_Color {46, 139, 87, 255}; }
    [[maybe_unused]] inline SDL_Color seaShell()             { return SDL_Color {255, 245, 238, 255}; }
    [[maybe_unused]] inline SDL_Color sienna()               { return SDL_Color {160, 82, 45, 255}; }
    [[maybe_unused]] inline SDL_Color silver()               { return SDL_Color {192, 192, 192, 255}; }
    [[maybe_unused]] inline SDL_Color skyBlue()              { return SDL_Color {135, 206, 235, 255}; }
    [[maybe_unused]] inline SDL_Color slateBlue()            { return SDL_Color {106, 90, 205, 255}; }
    [[maybe_unused]] inline SDL_Color slateGray()            { return SDL_Color {112, 128, 144, 255}; }
    [[maybe_unused]] inline SDL_Color slateGrey()            { return SDL_Color {112, 128, 144, 255}; }
    [[maybe_unused]] inline SDL_Color snow()                 { return SDL_Color {255, 250, 250, 255}; }
    [[maybe_unused]] inline SDL_Color springGreen()          { return SDL_Color {0, 255, 127, 255}; }
    [[maybe_unused]] inline SDL_Color steelBlue()            { return SDL_Color {70, 130, 180, 255}; }
    [[maybe_unused]] inline SDL_Color tan()                  { return SDL_Color {210, 180, 140, 255}; }
    [[maybe_unused]] inline SDL_Color teal()                 { return SDL_Color {0, 128, 128, 255}; }
    [[maybe_unused]] inline SDL_Color thistle()              { return SDL_Color {216, 191, 216, 255}; }
    [[maybe_unused]] inline SDL_Color tomato()               { return SDL_Color {255, 99, 71, 255}; }
    [[maybe_unused]] inline SDL_Color turquoise()            { return SDL_Color {64, 224, 208, 255}; }
    [[maybe_unused]] inline SDL_Color violet()               { return SDL_Color {238, 130, 238, 255}; }
    [[maybe_unused]] inline SDL_Color wheat()                { return SDL_Color {245, 222, 179, 255}; }
    [[maybe_unused]] inline SDL_Color white()                { return SDL_Color {255, 255, 255, 255}; }
    [[maybe_unused]] inline SDL_Color whiteSmoke()           { return SDL_Color {245, 245, 245, 255}; }
    [[maybe_unused]] inline SDL_Color yellow()               { return SDL_Color {255, 255, 0, 255}; }
    [[maybe_unused]] inline SDL_Color yellowGreen()          { return SDL_Color {154, 205, 50, 255}; }
    ///@}

} // Color

#endif //GOLD_CARTRIDGE_COLORS_H
