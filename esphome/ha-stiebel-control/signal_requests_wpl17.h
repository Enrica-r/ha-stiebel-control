/*
 * Signal Request Table — WPL17 Heat Pump
 *
 * VERIFIED ON REAL HARDWARE (CAN-B, X1.2, 50 kbps, 2026-08-30).
 *
 * WPL17 has TWO CAN buses:
 *  - CAN-A (20 kbps): heating-level data (Vorlauf/Ruecklauf/Aussentemp/Verdichter),
 *    Kessel=0x180, Manager=0x480, Heizmodul=0x500.
 *  - CAN-B (X1.2, 50 kbps): the COMPORT level — Speicher-Ist-Temp, Raum-Soll-Temp,
 *    Heizkurve, Energien, Betriebsmodus. THIS is where the ISG-Web is connected
 *    and where the values the display shows are readable.
 *
 * To use the WPL17 with this project, connect to CAN-B and set
 * `can_bit_rate: 50kbps` in heatingpump.yaml substitutions.
 *
 * The CAN-B member addresses (verified on a real WPL17 ICS classic):
 *   KESSEL_CANB  0x201  — Speicheristtemp, Raumsolltemp, Heizkurve, Aussentemp
 *   MANAGER_CANB 0x480  — Programm-Schalter, Sommerbetrieb, Raumsolltemp, Heizkurve
 *   HEIZMODUL_CANB 0x500 — Energien (Strom/Waerme MWh+kWh), Verdichter, Vorlauf/Ruecklauf
 *
 * Known facts from real hardware scan:
 *  - SPEICHERISTTEMP, RAUMSOLLTEMP_I (= Komforttemp), RAUMSOLLTEMP_NACHT (= Eco temp),
 *    HEIZKURVE (= Steigung, e.g. 0.40), AUSSENTEMP, WPVORLAUFIST, RUECKLAUFISTTEMP,
 *    VERDICHTER and the energy counters are all readable.
 *  - RAUMSOLLTEMP_II / _III return -255: this unit has only 1 heating circuit.
 *  - Cooling registers (KUEHLMODE, KUEHL_RAUMSOLL_*) return -255 until the cooling
 *    function is enabled on the display (menu > code 1000) AND the WPM is in
 *    Sommerbetrieb. They are built-in but disabled by default.
 */

#ifndef SIGNAL_REQUESTS_WPL17_H
#define SIGNAL_REQUESTS_WPL17_H

#include "config.h"
#include "signal_requests_base.h"

extern const SignalRequest signalRequests[] = {
    SIGNAL_REQUESTS_BASE   // date/time, EVU lock, operating mode, energy counters

    // ========================================================================
    // WPL17 (CAN-B): KESSEL_CANB (0x201) — Speicher/Raum/Heizkurve/Aussentemp
    // ========================================================================
    {"AUSSENTEMP",                 FREQ_30S,   cm_kessel_canb},
    {"SPEICHERISTTEMP",            FREQ_30S,   cm_kessel_canb},
    {"RAUMSOLLTEMP_I",             FREQ_1MIN,  cm_kessel_canb},   // Komforttemperatur
    {"RAUMSOLLTEMP_NACHT",         FREQ_1MIN,  cm_kessel_canb},   // Eco-Temperatur
    {"HEIZKURVE",                  FREQ_10MIN, cm_kessel_canb},   // Steigung (et_cent_val, 0.40)

    // ========================================================================
    // WPL17 (CAN-B): MANAGER_CANB (0x480) — Betriebsmodus + Raum/Heizkurve
    // ========================================================================
    {"PROGRAMMSCHALTER",           FREQ_1MIN,  cm_manager_canb},  // Automatik/Tagbetrieb/etc.
    {"SOMMERBETRIEB",              FREQ_1MIN,  cm_manager_canb},  // Heizung aus, nur WW (on/off)
    {"RAUMSOLLTEMP_I",             FREQ_1MIN,  cm_manager_canb},
    {"HEIZKURVE",                  FREQ_10MIN, cm_manager_canb},

    // ========================================================================
    // WPL17 (CAN-B): HEIZMODUL_CANB (0x500) — Energien + Vorlauf/Ruecklauf/Verdichter
    // ========================================================================
    {"WPVORLAUFIST",               FREQ_30S,   cm_heizmodul_canb},
    {"RUECKLAUFISTTEMP",           FREQ_30S,   cm_heizmodul_canb},
    {"VERDICHTER",                 FREQ_30S,   cm_heizmodul_canb},

    // Energie-Register (MWh = et_double_val, kWh = separate Register).
    // Die kombinierten "Gesamt kWh" berechneten Sensoren (MWh*1000+kWh) werden
    // in ha-stiebel-control.h aus diesen Werten abgeleitet.
    {"EL_AUFNAHMELEISTUNG_HEIZ_SUM_MWH", FREQ_10MIN, cm_heizmodul_canb},
    {"EL_AUFNAHMELEISTUNG_HEIZ_SUM_KWH", FREQ_10MIN, cm_heizmodul_canb},
    {"EL_AUFNAHMELEISTUNG_WW_SUM_MWH",   FREQ_10MIN, cm_heizmodul_canb},
    {"EL_AUFNAHMELEISTUNG_WW_SUM_KWH",   FREQ_10MIN, cm_heizmodul_canb},
    {"WAERMEERTRAG_HEIZ_SUM_MWH",        FREQ_10MIN, cm_heizmodul_canb},
    {"WAERMEERTRAG_HEIZ_SUM_KWH",        FREQ_10MIN, cm_heizmodul_canb},
    {"WAERMEERTRAG_WW_SUM_MWH",          FREQ_10MIN, cm_heizmodul_canb},
    {"WAERMEERTRAG_WW_SUM_KWH",          FREQ_10MIN, cm_heizmodul_canb},

    // ========================================================================
    // WPL17 (CAN-B): Kuehl-Register — vorhanden, aber default deaktiviert.
    // Liefern -255 bis die Kuehlfunktion am Display (Code 1000) freigeschaltet ist.
    // ========================================================================
    {"KUEHLMODE",                 FREQ_10MIN, cm_kessel_canb},
    {"KUEHL_RAUMSOLL_TAG",        FREQ_10MIN, cm_kessel_canb},
    {"KUEHL_RAUMSOLL_NACHT",      FREQ_10MIN, cm_kessel_canb},
};

extern const size_t SIGNAL_REQUEST_COUNT_VALUE = sizeof(signalRequests) / sizeof(SignalRequest);

#endif // SIGNAL_REQUESTS_WPL17_H