# ULF_MDU_EIN

[![build](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN/actions/workflows/build.yml/badge.svg)](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN/actions/workflows/build.yml) [![tests](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN/actions/workflows/tests.yml/badge.svg)](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN/actions/workflows/tests.yml) [![license](https://img.shields.io/github/license/ZIMO-Elektronik/ULF_MDU_EIN)](https://github.com/ZIMO-Elektronik/ULF_MDU_EIN/raw/master/LICENSE)

<img src="data/images/logo.svg" width="20%" align="right">

ULF_MDU_EIN is one of several [ULF_COM](https://github.com/ZIMO-Elektronik/ULF_COM) protocols. It is used to perform [ZPP](https://github.com/ZIMO-Elektronik/ZPP) and [ZSU](https://github.com/ZIMO-Elektronik/ZSU) updates over the track. ULF_MDU_EIN frames are transmitted using a serial/CDC interface (e.g. from [ZSP](https://www.zimo.at/web2010/products/zsp_zimo-sound-programmer.htm)) to a decoder update device, where they are translated into an [MDU](https://github.com/ZIMO-Elektronik/MDU) signal.

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#protocol">Protocol</a></li>
      <ul>
        <li><a href="#zpp--zsu">ZPP / ZSU</a></li>
        <li><a href="#special-command">Special Command</a></li>
        <li><a href="#timeout">Timeout</a></li>
      </ul>
    <li><a href="#getting-started">Getting Started</a></li>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#build">Build</a></li>
      </ul>
    <li><a href="#usage">Usage</a></li>
  </ol>
</details>

## Protocol
The protocol describes frames for ZPP / ZSU tunneled MDU packets and special commands.

### ZPP / ZSU 
A standard ULF_MDU_EIN frame contains the tunneled MDU packet.
| Length  | Value  | Description      |
| ------- | ------ | ---------------- |
| 4 byte  | `MDUB` | Frame prefix     |
| 2 byte  | N      | Length of packet |
| N byte  |        | MDU Packet       |
| 4 byte  | `MDUE` | Frame suffix     |

The response for a ULF_MDU_EIN packet frame is a pair of standard ASCII ACK(`\x06`) or NAK(`\x15`) separated with ASCII semicolon and ended with ASCII colon. This pair of characters essentially corresponds to the MDU response of a decoder and includes channel 1 and channel 2 acknowledgments.
```
[\x06\x15];[\x06\x15]:
```

### Special Command
| Length  | Value  | Description                    |
| ------- | ------ | ------------------------------ |
| 4 byte  | `MDUB` | Frame prefix                   | 
| 2 byte  | 0x00   |                                |
| 4 byte  |        | Command code                   |
| 1 byte  |        | Subcommand depends on command  |
| 16 byte | --     | Command payload                |
| 4 byte  | `MDUE` | Frame suffix                   |

The special commands are reserved commands to perform non-standard-tunneled actions such as entry sequence or transfer rate selection. Currently, possible commands are:
| Code    | Subcommand    | Payload                                     | Description           |
| ------- | ------------- | ------------------------------------------- | --------------------- |
| 'ETRY'  | 0x00          | `zeroed`                                    | ZSU entry usind MDU   |
|         | 0x01          | <a href="#dcc-zsu-entry">DCC ZSU Entry</a>  | ZSU entry using DCC   |
|         | 0x02          | <a href="#dcc-zpp-entry">DCC ZPP Entry</a>  | ZPP entry using DCC   |
| 'SPDS'  | 0x0X          | `zeroed`                                    | Set speed to X (0..4) |

The response for special commands is the same as in the general case. In case of error transmit `\x15;\x15:`, in the case of success `\x06;\x06:`. 

Since Commands may need additional data, an additional payload with 16 byte size is sent. If the payload is not needed, it is simply zeroed. All multi-byte values (e.g. Decoder-ID) are written as Big-Endian. 

#### DCC ZSU Entry 
The DCC ZSU entry may need one or multiple decoder serial numbers (SN) and / or decoder identifier (ID). Hence, the payload for this command is structured as follows
| Byte(s)  | Description              |
| -------- | ------------------------ |
| [0..3]   | Decoder Identifier       |
| [4..7]   | Decoder Serial number    |
| [8]      | `Continue` Flag          |
| [9..15]  | `zeroed`                 |

If the `Continue` Flag is set to 0x01, an additional entry command of the same type must follow. If it is set to 0x00, the entry sequence ends once finished and MDU tunnel operations begin.

#### DCC ZPP Entry
The DCC ZPP entry may need one or multiple decoder serial numbers (SN). Hence, the payload for this command is structured as follows
| Byte(s)  | Description              |
| -------- | ------------------------ |
| [0..3]   | Decoder Serial number    |
| [4]      | `Continue` Flag          |
| [5..15]  | `zeroed`                 |

If the `Continue` Flag is set to 0x01, an additional entry command of the same type must follow. If it is set to 0x00, the entry sequence ends once finished and MDU tunnel operations begin.

### Timeout
In case of unstable USB communication (e.g. bugs in CDC driver), timeouts need to be defined. The following timeout values are calculated using the worst case MDU packet (ZppWrite - 256 byte zero payload), while also respecting the bit timings of each transfer rate.
| Transfer rate | Timeout [ms] |
| ------------- | ------------ |
| 0 (fallback)  | 5600         |
| 1             | 100          |
| 2             | 200          |
| 3             | 400          |
| 4 (default)   | 800          |

Special commands need to be addressed individually because some operations take a while to complete.
| Command | Timeout [ms] |
| ------- | ------------ |
| ETRY    | 6000         |
| SPDS    | 100          |

## Getting Started
### Prerequisites
- C++23 compatible compiler
- [CMake](https://cmake.org/) ( >= 3.25 )

### Installation
This library is meant to be consumed with CMake.

```cmake
#Either by including it with CPM
cpmaddpackage("gh:ZIMO-Elektronik/ULF_MDU_EIN@0.1.0")

# or the FetchContent module
FetchContent_Declare(
  ULF_MDU_EIN
  GIT_REPOSITORY "https://github.com/ZIMO-Elektronik/ULF_MDU_EIN"
  GIT_TAG v0.1.0)
FetchContent_MakeAvailable(ULF_MDU_EIN)

target_link_libraries(YourTarget PRIVATE ULF::MDU_EIN)
```

### Build
:construction:

## Usage
To convert an ULF_MDU_EIN frame to a MDU packet, `mdu_ein2packet` can be used. In order to be able to distinguish between an error case and the case where the data is still incomplete, the return value of the function is `std::expected<std::optional<ulf::mdu_ein::MDUCopyOutput>, std::errc>`. If the pattern is not recognized at all, i.e. in the event of an error, then a `std::errc` is returned. If something is found but the data is not yet complete, a `std::nullopt` is returned. Otherwise the found data is returned as `ulf::mdu_ein::MDUCopyOutput`. The following snippet shows how `mdu_ein2packet` can be used.
```cpp
auto maybe_mdu{ulf::mdu_ein::mdu_ein2packet(mdu_ein_frm)};

// Could be MDU
if (maybe_mdu) {
  // Already complete?
  if (*maybe_mdu) {
    // Complete MDU
    auto mdu{**maybe_mdu};
  }
  // No, still missing data
  else {}
}
// Error, not MDU
else {}
```

Similar to above, `mdu_ein2bytes` can be used. This will output a `std::expected<std::optional<ulf::mdu_ein::MDURefOutput>, std::errc>`, which holds a `std::span` pointing to the MDU packet within the frame on success.

The return types are a union of either a MDU packet (or ref) or a special command. The following snippet shows how the types can be safely distinguished and used.
```cpp
ulf::mdu_ein::MDURefOutput output;
if (std::holds_alternative<mdu::Packet>(output)) {
  // Is packet
  auto packet = std::get<mdu::Packet>(output)
} else {
  // Is special command
  auto special = std::get<ulf::mdu_ein::Special>(output)
}
```

A mdu response can be constructed using `response2mdu_ein`. This will output a 4 byte `ztl::inplace_vector` containing the formatted response. Note, that the order of response channels is important, as shown in the following snippet.
```cpp
// Create response from channel1 and channel2 response
bool channel1{};
bool channel2{};
auto response{ulf::mdu_ein::response2mdu_ein(channel1, channel2)};
```