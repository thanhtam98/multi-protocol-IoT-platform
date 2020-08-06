Implement Modbus protocol on STM32F4

/*Installation*/
- Open STM32F4_MODBUS_RTU.ioc
- Switch to Project Manager and set the Toolchain/IDE: MKARM V5 for Keil C or SW4STM32 for SystemSTM32WorkBench
- Click GENERATE CODE on the top right.

/*Description*/
- Master:
	+ Modbus protocol stack and callback functions for 4 master serial ports.
	+ LWIP protocol stack for communicate over Ethernet.
	+ TODO: Webserver for user configuration.
- Slave:
	+ Modbus protocol stack and callback functions for slave.
	+ TODO: Template for interract with sensors and other periphrals.

/*Porting*/
Make sure the platform included FREERTOS
- Modbus: FreeModbus is included
- LWIP: LAN7820A with RMII
