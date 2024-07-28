/* Copyright 2024 keymagichorse
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bluetooth_bhq.h"
#include "bhq.h"
#include "report_buffer.h"
// #include "lpm.h"



void bluetooth_bhq_init(void) {
    bhq_init(false);
    report_buffer_init();
    // lpm_init();
}

void bluetooth_bhq_task(void) {
    bhq_task();
    report_buffer_task();
    // lpm_task();
}


/**
 * \brief Send a keyboard report.
 *
 * \param report The keyboard report to send.
 */
void bluetooth_bhq_send_keyboard(report_keyboard_t *report)
{

    bool firstBuffer = false;
    if (report_buffer_is_empty() && report_buffer_next_inverval()) {
        firstBuffer = true;
    }

    report_buffer_t report_buffer;
    report_buffer.type = REPORT_TYPE_KB;
    memcpy(&report_buffer.report_data, report, sizeof(report_keyboard_t));
    report_buffer_enqueue(&report_buffer);

    if (firstBuffer) {
        report_buffer_task();
    }


return;
    if (report_buffer_is_empty() && report_buffer_next_inverval()) {
        bhq_send_keyboard((uint8_t*)report);
        report_buffer_update_timer();
    } else {
        report_buffer_t report_buffer;
        report_buffer.type = REPORT_TYPE_KB;
        memcpy(&report_buffer.report_data, report, sizeof(report_keyboard_t));
        report_buffer_enqueue(&report_buffer);
    }
    
}

/**
 * \brief Send a mouse report.
 *
 * \param report The mouse report to send.
 */
void bluetooth_bhq_send_mouse(report_mouse_t *report)
{
    bhq_send_mouse((uint8_t *)report);
}


/**
 * \brief Send a consumer usage.
 *
 * \param usage The consumer usage to send.
 */
void bluetooth_bhq_send_consumer(uint16_t usage)
{
    bool firstBuffer = false;
    if (report_buffer_is_empty() && report_buffer_next_inverval()) {
        firstBuffer = true;
    }

    report_buffer_t report_buffer;
    report_buffer.type     = REPORT_TYPE_CONSUMER;
    report_buffer.consumer = usage;
    report_buffer_enqueue(&report_buffer);

    if (firstBuffer) {
        report_buffer_task();
    }
}

/**
 * \brief Send a system usage.
 *
 * \param usage The system usage to send.
 */
void bluetooth_bhq_send_system(uint16_t usage)
{
    bool firstBuffer = false;
    if (report_buffer_is_empty() && report_buffer_next_inverval()) {
        firstBuffer = true;
    }

    report_buffer_t report_buffer;
    report_buffer.type     = REPORT_TYPE_SYSTEM;
    report_buffer.consumer = usage;
    report_buffer_enqueue(&report_buffer);

    if (firstBuffer) {
        report_buffer_task();
    }
}

/**
 * \brief Send a nkro report.
 *
 * \param report The nkro report to send.
 */
void bluetooth_bhq_send_nkro(report_nkro_t *report)
{
    bool firstBuffer = false;
    if (report_buffer_is_empty() && report_buffer_next_inverval()) {
        firstBuffer = true;
    }

    report_buffer_t report_buffer;
    report_buffer.type = REPORT_TYPE_NKRO;

    report_buffer.report_data[0] = report->mods;
    for (size_t i = 0; i < NKRO_REPORT_BITS; i++)
    {
        report_buffer.report_data[1 + i] = report->bits[i];
    }
    
    report_buffer_enqueue(&report_buffer);

    if (firstBuffer) {
        report_buffer_task();
    }
}

