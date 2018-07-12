deps_config := \
	/Users/stdafx/esp/esp-idf/components/app_trace/Kconfig \
	/Users/stdafx/esp/esp-idf/components/aws_iot/Kconfig \
	/Users/stdafx/esp/esp-idf/components/bt/Kconfig \
	/Users/stdafx/esp/esp32-cam-demo/components/camera/Kconfig \
	/Users/stdafx/esp/esp-idf/components/driver/Kconfig \
	/Users/stdafx/esp/esp-idf/components/esp32/Kconfig \
	/Users/stdafx/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/Users/stdafx/esp/esp-idf/components/esp_http_client/Kconfig \
	/Users/stdafx/esp/esp-idf/components/ethernet/Kconfig \
	/Users/stdafx/esp/esp-idf/components/fatfs/Kconfig \
	/Users/stdafx/esp/esp-idf/components/freertos/Kconfig \
	/Users/stdafx/esp/esp-idf/components/heap/Kconfig \
	/Users/stdafx/esp/esp-idf/components/libsodium/Kconfig \
	/Users/stdafx/esp/esp-idf/components/log/Kconfig \
	/Users/stdafx/esp/esp-idf/components/lwip/Kconfig \
	/Users/stdafx/esp/esp-idf/components/mbedtls/Kconfig \
	/Users/stdafx/esp/esp-idf/components/mdns/Kconfig \
	/Users/stdafx/esp/esp-idf/components/openssl/Kconfig \
	/Users/stdafx/esp/esp-idf/components/pthread/Kconfig \
	/Users/stdafx/esp/esp-idf/components/spi_flash/Kconfig \
	/Users/stdafx/esp/esp-idf/components/spiffs/Kconfig \
	/Users/stdafx/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/Users/stdafx/esp/esp-idf/components/vfs/Kconfig \
	/Users/stdafx/esp/esp-idf/components/wear_levelling/Kconfig \
	/Users/stdafx/esp/esp-idf/Kconfig.compiler \
	/Users/stdafx/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/Users/stdafx/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/Users/stdafx/esp/esp32-cam-demo/main/Kconfig.projbuild \
	/Users/stdafx/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/Users/stdafx/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
