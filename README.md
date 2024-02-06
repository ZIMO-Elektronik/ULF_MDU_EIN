# ULF_MDU_EIN

The aim of this of this project is to create an interface that gives developers access to the [MDU](https://github.com/ZIMO-Elektronik/MDU) protocoll.

As part of the [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) family of ZIMO protocolls, the usage of this interface is dependent on a working USB VCOM interface to the MXULF.

Although the MDU activation sequences are DCC CV Verify commands, and therefore not actually part of the MDU command set, this adaption layer will offer a streamlined way to use them.

## Protocol
### Entry
The activation of ULF_MDU_EIN is done through the  `"MDU_EIN\r"`  command, and `"RESET\r"` will deactivate ULF_MDU_EIN.

After the MXULF has detected the activation command, it will wait for an ULF_MDU_EIN command code.

| MDU coding         | ULF_MDU_EIN | meaning      |
| ------------------ | ------------|--------------|
| 0xFFFF'FFFF        | 0xFF        | Ping |
| 0xFFFF'FFFE        | 0xFE        | Data Rate |
| 0xFFFF'FFFA        | 0xFA        | Binary Search |
| 0xFFFF'FFF6        | 0xF6        | CV Read |
| 0xFFFF'FFF9 		 | 0xF9        | CV Write |
| 0xFFFF'FFF2        | 0xF2        | Busy |
| 0xFFFF'FF06        | 0x06        | ZPP valid |
| 0xFFFF'FF07        | 0x07        | lc dc query |
| 0xFFFF'FF05        | 0x05        | zpp erase |
| 0xFFFF'FF08        | 0x08        | zpp update |
| 0xFFFF'FF0B        | 0x0B        | zpp upd end |
| 0xFFFF'FF0C        | 0x0C        | zpp exit |
| 0xFFFF'FF0D        | 0x0D        | epp exit reset |
| 0xFFFF'FFF7        | 0xF7        | salsa20 |
| 0xFFFF'FFF5        | 0xF5        | zsu erase | 
| 0xFFFF'FFF8        | 0xF8        | zsu update |
| 0xFFFF'FFFB        | 0xFB        | crc32 start |
| 0xFFFF'FFFC        | 0xFC        | crc32 result |
| 0xFFFF'FFFD        | 0xFD        | crc32 result exit |
| *ZPP entry*        | 0x01        | |
| *ZSU entry*        | 0x02        | |


