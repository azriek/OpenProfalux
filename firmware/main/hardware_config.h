/*
 * hardware_config.h - Multi-target hardware definitions
 *
 * Target External : ESP32-WROOM DevKit + CC1101 breakout externe
 * Target M5Stack  : M5Stack ATOM Lite (=ESP32-PICO)
 */
#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

/* Indispensable : les CONFIG_OPENPROFALUX_TARGET_* viennent de la. Sans cet
 * include, tout fichier qui inclut hardware_config.h avant un en-tete IDF
 * (profalux.c, mqtt_bridge.c) declenche le #error de fin de fichier. */
#include "sdkconfig.h"

/* ═══════════════════════════════════════════════════════════════
 * Select target at compile time via sdkconfig or CMake
 * ═══════════════════════════════════════════════════════════════ */

#if defined(SIM_MODE)

    /* ─── PC simulation - no real hardware ─── */
    #define TARGET_NAME               "sim"
    #define CC1101_PIN_CS             (-1)
    #define CC1101_PIN_GDO0           (-1)
    #define LED_PIN                   (-1)

#elif defined(CONFIG_OPENPROFALUX_TARGET_EXTERNAL)

    /* ─── Target External: ESP32 DevKit ─── */
    #define TARGET_NAME               "external"

    /* CC1101 SPI pins (=VSPI par défaut ESP32) */
    #define CC1101_SPI_HOST           VSPI_HOST
    #define CC1101_PIN_MISO           19
    #define CC1101_PIN_MOSI           23
    #define CC1101_PIN_SCK            18
    #define CC1101_PIN_CS              5
    #define CC1101_PIN_GDO0            4  /* Interrupt data */
    #define CC1101_PIN_GDO2            2  /* Debug (optional) */
    #define CC1101_SPI_FREQ_HZ  6000000  /* 6 MHz safe */

    /* Optional debug buttons (=disabled by default, trigger via MQTT/HA) */
    #define BTN_PIN_DEBUG_UP          -1
    #define BTN_PIN_DEBUG_STOP        -1
    #define BTN_PIN_DEBUG_DOWN        -1

    /* LED status (=built-in blue LED devkit) */
    #define LED_PIN                    2
    #define LED_ACTIVE_HIGH           1

#elif defined(CONFIG_OPENPROFALUX_TARGET_M5STACK)

    /* ─── Target M5Stack ATOM Lite ESP32-PICO-D4 ─── */
    /* Câblage Dupont femelle-femelle sur bottom header ATOM Lite */
    #define TARGET_NAME               "m5stack_atom"

    /* CC1101 SPI - tous pins sur bottom header ATOM Lite (=Dupont-friendly) */
    #define CC1101_SPI_HOST           HSPI_HOST
    #define CC1101_PIN_MISO           33  /* bottom G33 - 🟢 vert */
    #define CC1101_PIN_MOSI           23  /* bottom G23 - 🟡 jaune */
    #define CC1101_PIN_SCK            19  /* bottom G19 - 🔵 bleu */
    #define CC1101_PIN_CS             22  /* bottom G22 - ⚪ blanc */
    #define CC1101_PIN_GDO0           25  /* bottom G25 - 🟠 orange */
    #define CC1101_PIN_GDO2           21  /* bottom G21 - 🟣 violet (optionnel) */
    #define CC1101_SPI_FREQ_HZ  4000000

    /* VCC = 3.3V bottom - 🔴 rouge / GND = bottom - ⚫ noir */
    /* Aucun bouton — tout via MQTT/HA */

    /* LED RGB (=WS2812 pin 27) */
    #define LED_PIN                   27
    #define LED_ACTIVE_HIGH           1
    #define LED_IS_WS2812             1

#elif defined(CONFIG_OPENPROFALUX_TARGET_S3)

    /* ─── Target ESP32-S3 DevKitC-1 + CC1101 externe ─── */
    /* Brochage aligne sur la carte v2 Eveno/Profalux (module CC1101-A).
     * Sur S3 il n'y a plus VSPI/HSPI : les hosts SPI generiques sont
     * SPI2_HOST / SPI3_HOST. On prend SPI2_HOST (equiv. de l'ex-HSPI). */
    #define TARGET_NAME               "esp32s3"

    #define CC1101_SPI_HOST           SPI2_HOST
    #define CC1101_PIN_MISO           13
    #define CC1101_PIN_MOSI           11
    #define CC1101_PIN_SCK            12
    #define CC1101_PIN_CS             10
    #define CC1101_PIN_GDO0            9  /* donnees OOK : RMT RX + bit-bang TX */
    #define CC1101_PIN_GDO2            8  /* debug (optionnel, non utilise par le code) */
    #define CC1101_SPI_FREQ_HZ  4000000  /* 4 MHz : sur du Dupont c'est le plus sur */

    /* Aucun bouton — tout via MQTT/HA/UI web */

    /* LED RGB embarquee du DevKitC-1 (WS2812 sur GPIO48).
     * Macro decorative : le firmware ne pilote pas la LED (pas de dep led_strip). */
    #define LED_PIN                   48
    #define LED_ACTIVE_HIGH           1
    #define LED_IS_WS2812             1

#else
    #error "Please select CONFIG_OPENPROFALUX_TARGET_EXTERNAL, _M5STACK or _S3"
#endif

/* ═══════════════════════════════════════════════════════════════
 * KEELOQ / Profalux constants (=target-independent)
 * ═══════════════════════════════════════════════════════════════ */

/* Valeurs MESUREES sur l'air le 2026-08-10 (captures/2026-08-10-profalux-868). */
#define PROFALUX_FREQ_HZ          868425000UL  /* 868,425 MHz (etait 868,350 = hors bande) */
#define PROFALUX_TE_US                  455    /* element de base T_E mesure */
#define PROFALUX_BIT_TIME_US           1365    /* periode de bit = 3*Te */
#define PROFALUX_PREAMBLE_ELEMENTS       23    /* 23 alternances de Te */
#define PROFALUX_PREAMBLE_US            455    /* = Te */
#define PROFALUX_HEADER_US             4450    /* silence mesure 4420-4491 us */
#define PROFALUX_REPEAT_COUNT            10  /* HCS301 typical */
#define PROFALUX_PAIR_FRAMES             60  /* 1 par seconde */
#define PROFALUX_PAIR_DELAY_MS         1000

#endif /* HARDWARE_CONFIG_H */
