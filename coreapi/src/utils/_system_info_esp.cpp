#include "dbgutils.h"

#ifdef ESP8266

extern "C" {
#include <user_interface.h>
}


static const char * const RST_REASONS[] =
{
    "REASON_DEFAULT_RST",
    "REASON_WDT_RST",
    "REASON_EXCEPTION_RST",
    "REASON_SOFT_WDT_RST",
    "REASON_SOFT_RESTART",
    "REASON_DEEP_SLEEP_AWAKE",
    "REASON_EXT_SYS_RST"
};

static const char * const FLASH_SIZE_MAP_NAMES[] =
{
    "FLASH_SIZE_4M_MAP_256_256",
    "FLASH_SIZE_2M",
    "FLASH_SIZE_8M_MAP_512_512",
    "FLASH_SIZE_16M_MAP_512_512",
    "FLASH_SIZE_32M_MAP_512_512",
    "FLASH_SIZE_16M_MAP_1024_1024",
    "FLASH_SIZE_32M_MAP_1024_1024"
};

static const char * const OP_MODE_NAMES[]
{
    "NULL_MODE",
    "STATION_MODE",
    "SOFTAP_MODE",
    "STATIONAP_MODE"
};

static const char * const AUTH_MODE_NAMES[]
{
    "AUTH_OPEN",
    "AUTH_WEP",
    "AUTH_WPA_PSK",
    "AUTH_WPA2_PSK",
    "AUTH_WPA_WPA2_PSK",
    "AUTH_MAX"
};

static const char * const PHY_MODE_NAMES[]
{
    "",
    "PHY_MODE_11B",
    "PHY_MODE_11G",
    "PHY_MODE_11N"
};


static void print_softap_config(Stream & consolePort, softap_config const& config)
{
    consolePort.println();
    consolePort.println(F("SoftAP Configuration"));
    consolePort.println(F("--------------------"));

    consolePort.print(F("ssid:            "));
    consolePort.println((char *) config.ssid);

    consolePort.print(F("password:        "));
    consolePort.println((char *) config.password);

    consolePort.print(F("ssid_len:        "));
    consolePort.println(config.ssid_len);

    consolePort.print(F("channel:         "));
    consolePort.println(config.channel);

    consolePort.print(F("authmode:        "));
    consolePort.println(AUTH_MODE_NAMES[config.authmode]);

    consolePort.print(F("ssid_hidden:     "));
    consolePort.println(config.ssid_hidden);

    consolePort.print(F("max_connection:  "));
    consolePort.println(config.max_connection);

    consolePort.print(F("beacon_interval: "));
    consolePort.print(config.beacon_interval);
    consolePort.println("ms");

    consolePort.println(F("--------------------"));
    consolePort.println();
}

static void print_system_info(Stream & consolePort)
{
    const rst_info * resetInfo = system_get_rst_info();
    consolePort.print(F("system_get_rst_info() reset reason: "));
    consolePort.println(RST_REASONS[resetInfo->reason]);

    consolePort.print(F("system_get_free_heap_size(): "));
    consolePort.println(system_get_free_heap_size());

    consolePort.print(F("system_get_os_print(): "));
    consolePort.println(system_get_os_print());
    system_set_os_print(1);
    consolePort.print(F("system_get_os_print(): "));
    consolePort.println(system_get_os_print());

    system_print_meminfo();

    consolePort.print(F("system_get_chip_id(): 0x"));
    consolePort.println(system_get_chip_id(), HEX);

    consolePort.print(F("system_get_sdk_version(): "));
    consolePort.println(system_get_sdk_version());

    consolePort.print(F("system_get_boot_version(): "));
    consolePort.println(system_get_boot_version());

    consolePort.print(F("system_get_userbin_addr(): 0x"));
    consolePort.println(system_get_userbin_addr(), HEX);

    consolePort.print(F("system_get_boot_mode(): "));
    consolePort.println(system_get_boot_mode() == 0 ? F("SYS_BOOT_ENHANCE_MODE") : F("SYS_BOOT_NORMAL_MODE"));

    consolePort.print(F("system_get_cpu_freq(): "));
    consolePort.println(system_get_cpu_freq());

    consolePort.print(F("system_get_flash_size_map(): "));
    consolePort.println(FLASH_SIZE_MAP_NAMES[system_get_flash_size_map()]);
}


static void print_wifi_general(Stream & consolePort)
{
    consolePort.print(F("wifi_get_channel(): "));
    consolePort.println(wifi_get_channel());

    consolePort.print(F("wifi_get_phy_mode(): "));
    consolePort.println(PHY_MODE_NAMES[wifi_get_phy_mode()]);
}

static void esp8266_config(uint8_t cpu_freq, uint8_t sleep_type)
{
  // Try pushing frequency to 160MHz.
  //system_update_cpu_freq(SYS_CPU_160MHZ);

  // This doesn't work on an ESP-01.
  // wifi_set_sleep_type(LIGHT_SLEEP_T);
}

#endif

///////////////////////////////////////////////////
namespace baseutils {

#ifdef ESP8266

void print_esp8266Info(Stream & consolePort)
{
  consolePort.println(F("-------------ESP8266 Specific INFO:-------------"));
  consolePort.print(F("system_get_time(): "));
  consolePort.println(system_get_time());

  print_system_info(consolePort);

  consolePort.print(F("wifi_get_opmode(): "));
  consolePort.print(wifi_get_opmode());
  consolePort.print(F(" - "));
  consolePort.println(OP_MODE_NAMES[wifi_get_opmode()]);

  consolePort.print(F("wifi_get_opmode_default(): "));
  consolePort.print(wifi_get_opmode_default());
  consolePort.print(F(" - "));
  consolePort.println(OP_MODE_NAMES[wifi_get_opmode_default()]);

  consolePort.print(F("wifi_get_broadcast_if(): "));
  consolePort.println(wifi_get_broadcast_if());

  print_wifi_general(Serial);

  consolePort.println(F("------------------------------------------------"));

}




#elif defined ESP32 

void print_esp32Info(Stream & stream)
{
    //TODO
}


#endif

#if defined ESP8266 || defined ESP32 

float readTemp()
{
    DPRINTLN(F("readTemp() NOT IMPLEMENTED for ESP8266 nor for ESP32"));
    return 0.0f;
}

/*
Per una misura corretta bisogna:
    1- impostare ADC_MODE(ADC_VCC); // Con pin A0 disconnesso
    2- disattivare il WiFi
*/
float getVcc()
{
    #ifdef ESP32

    DPRINTLN(F("readTemp() NOT IMPLEMENTED for ESP32"));
    return 0.0f;

    #else
    return (float)ESP.getVcc() / 1024.0;
    #endif
}




} // END namespace baseutils


#endif