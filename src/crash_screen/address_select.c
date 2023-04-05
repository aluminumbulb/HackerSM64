#include <ultra64.h>
#include "types.h"
#include "sm64.h"
#include "crash_screen.h"
#include "game/game_init.h"
#include "pages/disasm.h"


_Bool gAddressSelectMenuOpen = FALSE;
static uintptr_t sAddressSelectTarget = 0x00000000;
static s8 sAddressSelecCharIndex = 2;


void draw_address_select(void) {
    crash_screen_draw_dark_rect(
        (JUMP_MENU_X1 -  JUMP_MENU_MARGIN_X     ), (JUMP_MENU_Y1 -  JUMP_MENU_MARGIN_Y     ),
        (JUMP_MENU_W  + (JUMP_MENU_MARGIN_X * 2)), (JUMP_MENU_H  + (JUMP_MENU_MARGIN_Y * 2)),
        3
    );

    // "GO TO:"
    crash_screen_print((SCREEN_CENTER_X - TEXT_WIDTH(3)), JUMP_MENU_Y1, "%s:", "GO TO");

    // Up arrow:
    crash_screen_draw_vertical_triangle(
        ((SCREEN_CENTER_X - TEXT_WIDTH(4)) + (sAddressSelecCharIndex * TEXT_WIDTH(1)) - 1),
        ((JUMP_MENU_Y1 + TEXT_HEIGHT(1)) + CRASH_SCREEN_CHAR_SPACING_Y),
        TEXT_WIDTH(1), TEXT_WIDTH(1),
        COLOR_RGBA32_CRASH_SELECT_ARROWS,
        FALSE
    );
    // Down arrow:
    crash_screen_draw_vertical_triangle(
        ((SCREEN_CENTER_X - TEXT_WIDTH(4)) + (sAddressSelecCharIndex * TEXT_WIDTH(1)) - 1),
        ((JUMP_MENU_Y1 + TEXT_HEIGHT(3)) - CRASH_SCREEN_CHAR_SPACING_Y + 1),
        TEXT_WIDTH(1), TEXT_WIDTH(1),
        COLOR_RGBA32_CRASH_SELECT_ARROWS,
        TRUE
    );

    // "0x[80XXXXXX]"
    crash_screen_print((SCREEN_CENTER_X - TEXT_WIDTH(8 / 2) - TEXT_WIDTH(2)), (JUMP_MENU_Y1 + TEXT_HEIGHT(2)), (STR_HEX_PREFIX STR_HEX_WORD), sAddressSelectTarget);

#ifdef INCLUDE_DEBUG_MAP
    uintptr_t checkAddr = sAddressSelectTarget;
    const char* fname = parse_map(&checkAddr);
    if (fname != NULL) {
        // "[function name]"
        crash_screen_print_scroll(JUMP_MENU_X1, (JUMP_MENU_Y1 + TEXT_HEIGHT(4)), JUMP_MENU_CHARS_X, STR_COLOR_PREFIX"%s", COLOR_RGBA32_CRASH_FUNCTION_NAME, fname);
    }
#endif

    osWritebackDCacheAll();
}

void crash_screen_select_address(void) {
    if (gCrashScreenDirectionFlags.pressed.left) {
        sAddressSelecCharIndex = ((sAddressSelecCharIndex - 1) & 0x7); // % 8
        gCrashScreenUpdateFramebuffer = TRUE;
    }
    if (gCrashScreenDirectionFlags.pressed.right) {
        sAddressSelecCharIndex = ((sAddressSelecCharIndex + 1) & 0x7); // % 8
        gCrashScreenUpdateFramebuffer = TRUE;
    }

    uintptr_t nextSelectedAddress = sAddressSelectTarget;
    u32 shift = ((32 - 4) - (sAddressSelecCharIndex * 4));
    u8 digit = GET_HEX_DIGIT(sAddressSelectTarget, shift);
    s8 new = digit;

    if (gCrashScreenDirectionFlags.pressed.up) {
        // Increment the selected digit.
        new = ((digit + 1) & BITMASK(4));
        if (!IS_IN_RAM(SET_HEX_DIGIT(sAddressSelectTarget, new, shift))) {
            // Find the digit to wrap to
            for (new = 0x0; new < 0xF; new++) {
                if (IS_IN_RAM(SET_HEX_DIGIT(sAddressSelectTarget, new, shift))) {
                    break;
                }
            }
        }
    }
    if (gCrashScreenDirectionFlags.pressed.down) {
        // Decrement the selected digit.
        new = ((digit - 1) & BITMASK(4));
        if (!IS_IN_RAM(SET_HEX_DIGIT(sAddressSelectTarget, new, shift))) {
            // Find the digit to wrap to
            for (new = 0xF; new > 0x0; new--) {
                if (IS_IN_RAM(SET_HEX_DIGIT(sAddressSelectTarget, new, shift))) {
                    break;
                }
            }
        }
    }

    if (new != digit) {
        nextSelectedAddress = SET_HEX_DIGIT(sAddressSelectTarget, new, shift);

        if (IS_IN_RAM(nextSelectedAddress)) {
            sAddressSelectTarget = nextSelectedAddress;
            gCrashScreenUpdateFramebuffer = TRUE;
        }
    }

    if (gPlayer1Controller->buttonPressed & A_BUTTON) { //! TODO: Not if address select was just opened
        gPlayer1Controller->buttonPressed &= A_BUTTON;
        // Jump to the address and close the popup.
        gAddressSelectMenuOpen = FALSE;
#ifdef INCLUDE_DEBUG_MAP
        if (gCrashPage == PAGE_DISASM) {
            if (!is_in_same_function(gSelectedAddress, sAddressSelectTarget)) {
                gFillBranchBuffer = TRUE;
            }
        }
#endif
        gSelectedAddress = sAddressSelectTarget;
        gCrashScreenUpdateFramebuffer = TRUE;
    }

    if (gPlayer1Controller->buttonPressed & B_BUTTON) {
        // Close the popup without jumping.
        gAddressSelectMenuOpen = FALSE;
        gCrashScreenUpdateFramebuffer = TRUE;
    }
}

// Open the jump to address popup.
void open_address_select(uintptr_t dest) {
    gAddressSelectMenuOpen = TRUE;
    sAddressSelectTarget = dest;
    gCrashScreenUpdateFramebuffer = TRUE;
}