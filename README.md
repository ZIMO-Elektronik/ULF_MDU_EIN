# ULF_MDU_EIN

The aim of this of this project is to create an interface that gives developers access to the [MDU](https://github.com/ZIMO-Elektronik/MDU) protocoll.

As part of the [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) family of ZIMO protocolls, the usage of this interface is dependent on a working USB VCOM interface to the MXULF.

Although the MDU activation sequences are DCC CV Verify commands, and therefore not actually part of the MDU command set, this adaption layer will offer a streamlined way to use them.

## Protocol

### Activation
The activation of ULF_MDU_EIN is done through the  `"MDU_EIN\r"`  command, and `"RESET\r"` will deactivate ULF_MDU_EIN.

After the MXULF has detected the activation command, it will wait for an ULF_MDU_EIN command.

### Command Codes

| MDU coding         | ULF_MDU_EIN coding | meaning      |
| ------------------ | ------------|--------------|
| 0xFFFF'FFFF        | 0xFF        | Ping |
| 0xFFFF'FFFE        | 0xFE        | Data Rate |
| 0xFFFF'FFFA        | 0xFA        | Binary Search |
| 0xFFFF'FFF6        | 0xF6        | CV Read |
| 0xFFFF'FFF9 		 | 0xF9        | CV Write |
| 0xFFFF'FFF2        | 0xF2        | Busy |
| 0xFFFF'FF06        | 0x06        | ZPP valid |
| 0xFFFF'FF07        | 0x07        | LC DC query |
| 0xFFFF'FF05        | 0x05        | Zpp erase |
| 0xFFFF'FF08        | 0x08        | Zpp update |
| 0xFFFF'FF0B        | 0x0B        | Zpp update end |
| 0xFFFF'FF0C        | 0x0C        | Zpp exit |
| 0xFFFF'FF0D        | 0x0D        | Zpp exit&reset |
| 0xFFFF'FFF7        | 0xF7        | Salsa20 Initialization|
| 0xFFFF'FFF5        | 0xF5        | Zsu Erase | 
| 0xFFFF'FFF8        | 0xF8        | Zsu Update |
| 0xFFFF'FFFB        | 0xFB        | Crc32 start |
| 0xFFFF'FFFC        | 0xFC        | Crc32 result |
| 0xFFFF'FFFD        | 0xFD        | Crc32 result exit |
| *ZPP entry*        | 0x01        | enter MDU zsu load|
| *ZSU entry*        | 0x02        | enter MDU Firmware|

The command codes have been shortened to a single byte, and the receiced command code and data bytes are supplemented by a preamble and the following acknowledgement bits.  Other than that, each command has the same structure and contents, therefore a detailed description of each of them will be omitted here. 

### ZPP Load Entry

Activation of the MDU sound load state in a compatible decoder happens via a sequence of DCC configuration variable verify commands.

As described [here](https://github.com/ZIMO-Elektronik/MDU?tab=readme-ov-file#entry),  Bytes 1-5 are of fixed value in this sequence. Bytes 6-9 may contain a specific serial number, or all zeros to address any connected decoder.

Therefore, to start the entry procedure, the following packet has to be sent:

| Code | Byte 1 | Byte 2 | Byte 3 | Byte 4 |
| --- | --- | --- | --- | --- | 
| 0x01 | SN | SN | SN | SN |

### ZSU Load Entry

Similarly, activation of the MDU firmware load state happens via a sequence of DCC configuration variable verify commands.

| Code | Byte 1 | Byte 2 | Byte 3 | Byte 4 | Byte 5| Byte 6 | Byte 7 | Byte 8 |
| --- | --- | --- | --- | --- | --- | --- | --- |--- |
| 0x02 | ID | ID | ID | ID | SN | SN | SN | SN |



### Packet Creation and Response

After  the protocol activation, the device is ready to receive an MDU command made up of a command byte and data bytes.
A basic on the fly check is performed during reception:    
1. Since the first byte received has to be a command byte, the value has to correspond to one entry in the command code table. If this is not the case, a protocol error response is generated.
2. If the number of received data bytes does not match the expected value, an error response is generated

Once a valid packet is in the buffer, the device will generate the corresponding MDU track signal and look for a response during the Acknowledgment bits.

A byte value is sent back to the host depending on the outcome:

| response code | meaning |
| --------------| ------- |
| 0 | no response |
| 1 | channel 1 response |
| 2 | channel 2 response |
| 3 | protocol error |



### MDU Command Response




