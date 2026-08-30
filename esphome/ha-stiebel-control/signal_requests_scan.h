/*
 * Signal Request Table — WPL17 CAN-B Scan (DIAGNOSE)
 *
 * Diese Tabelle ist NUR fuer die Diagnose-Firmware heatingpump_scan.yaml.
 * Sie fragt gezielt Signale/Members ab, um unbekannte Register zu finden
 * oder Writes zu testen. Fuer die Produktion siehe signal_requests_wpl17.h.
 *
 * Konfiguriert fuer WPL17 auf CAN-B (X1.2, 50 kbps). Die Member-Adressen
 * auf CAN-B unterscheiden sich von CAN-A (Kessel=0x201 statt 0x180 etc.).
 *
 * TIPP fuer zukuenftige Entwicklungen (z.B. Kuehlfunktion): Um ein Register
 * zu testen, trage es hier gegen die passenden Members ein und beobachte
 * die Logs ("processCanMessage" / "ENERGIE" / "UNKNOWN").
 */

#ifndef SIGNAL_REQUESTS_SCAN_H
#define SIGNAL_REQUESTS_SCAN_H

#include "config.h"
#include "signal_requests_base.h"

extern const SignalRequest signalRequests[] = {
    // === ENERGIEDATEN: MWh + kWh (fuer kombinierte Gesamtwerte) ===
    {"WAERMEERTRAG_HEIZ_SUM_MWH",  FREQ_10S, cm_heizmodul_canb},
    {"WAERMEERTRAG_HEIZ_SUM_KWH",  FREQ_10S, cm_heizmodul_canb},
    {"WAERMEERTRAG_WW_SUM_MWH",    FREQ_10S, cm_heizmodul_canb},
    {"WAERMEERTRAG_WW_SUM_KWH",    FREQ_10S, cm_heizmodul_canb},
    {"EL_AUFNAHMELEISTUNG_HEIZ_SUM_MWH", FREQ_10S, cm_heizmodul_canb},
    {"EL_AUFNAHMELEISTUNG_HEIZ_SUM_KWH", FREQ_10S, cm_heizmodul_canb},
    {"EL_AUFNAHMELEISTUNG_WW_SUM_MWH",   FREQ_10S, cm_heizmodul_canb},
    {"EL_AUFNAHMELEISTUNG_WW_SUM_KWH",   FREQ_10S, cm_heizmodul_canb},

    // === Referenzwerte (funktionieren sicher) ===
    {"SPEICHERISTTEMP",          FREQ_10S, cm_kessel_canb},
    {"RAUMSOLLTEMP_I",           FREQ_10S, cm_kessel_canb},
    {"AUSSENTEMP",               FREQ_10S, cm_kessel_canb},

    // === KUEHLFUNKTION (nur aktiv, wenn am Display freigeschaltet) ===
    // KUEHLMODE, KUEHL_RAUMSOLL_TAG, KUEHL_RAUMSOLL_NACHT, KUEHL_VORLAUFSOLLTEMP
    // liefern -255, bis die Kuehlfunktion aktiviert ist (Menue > Code 1000).
    // Hier auskommentiert, da nicht freigeschaltet. Fuer spaetere Tests eintragen:
    // {"KUEHLMODE",             FREQ_10S, cm_kessel_canb},
    // {"KUEHL_RAUMSOLL_TAG",    FREQ_10S, cm_kessel_canb},
    // {"KUEHL_RAUMSOLL_NACHT",  FREQ_10S, cm_kessel_canb},
};

extern const size_t SIGNAL_REQUEST_COUNT_VALUE = sizeof(signalRequests) / sizeof(SignalRequest);

#endif // SIGNAL_REQUESTS_SCAN_H