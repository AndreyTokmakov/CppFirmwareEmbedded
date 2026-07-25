# RF Attenuators Library

A modern C++ library providing a unified interface for controlling RF attenuators used in embedded systems, firmware, Software Defined Radio (SDR), radar, wireless communication and RF test equipment.

The goal of this project is to provide a hardware-independent abstraction for various types of RF attenuators while keeping the implementation portable across different operating systems, RTOSes and embedded platforms.

---

# Overview

An **attenuator** is an electronic device that reduces the amplitude or power of an RF signal by a known amount while preserving its waveform and minimizing signal distortion.

Unlike an amplifier, which increases signal power, an attenuator intentionally decreases it.

Attenuation is almost always expressed in **decibels (dB)**.

Attenuation is expressed in decibels (dB), a logarithmic unit describing the reduction
of signal power. Since RF systems typically operate with matched impedances
(e.g. 50 Ω), attenuation can also be expressed as the corresponding reduction
in signal voltage.

Typical attenuation values include:

| Attenuation | Power Ratio | Power Reduction | Voltage Ratio* | Typical Usage |
|-------------|------------:|----------------:|---------------:|---------------|
| 0 dB  | 1.000× | 1× | 1.000× | No attenuation, bypass state |
| 0.5 dB | 0.891× | 1.12× | 0.944× | Fine gain adjustment |
| 1 dB  | 0.794× | 1.26× | 0.891× | Small signal correction |
| 2 dB  | 0.631× | 1.58× | 0.794× | RF level optimization |
| 3 dB  | 0.500× | 2× | 0.708× | Half power point |
| 4 dB  | 0.398× | 2.51× | 0.631× | Moderate signal reduction |
| 5 dB  | 0.316× | 3.16× | 0.562× | Front-end gain adjustment |
| 6 dB  | 0.251× | 4× | 0.501× | Quarter power reduction |
| 8 dB  | 0.158× | 6.31× | 0.398× | RF chain optimization |
| 10 dB | 0.100× | 10× | 0.316× | Ten times power reduction |
| 12 dB | 0.063× | 15.8× | 0.251× | Receiver protection |
| 15 dB | 0.032× | 31.6× | 0.178× | Strong signal reduction |
| 20 dB | 0.010× | 100× | 0.100× | Laboratory measurements |
| 25 dB | 0.003× | 316× | 0.056× | High dynamic range receivers |
| 30 dB | 0.001× | 1000× | 0.032× | Strong attenuation |
| 35 dB | 0.0003× | 3162× | 0.018× | High isolation applications |
| 40 dB | 0.0001× | 10000× | 0.010× | RF measurement setups |
| 45 dB | 0.00003× | 31623× | 0.006× | Receiver sensitivity testing |
| 50 dB | 0.00001× | 100000× | 0.003× | Weak signal simulation |
| 60 dB | 0.000001× | 1000000× | 0.001× | Extreme attenuation |
| 70 dB | 0.0000001× | 10000000× | 0.0003× | High isolation RF paths |
| 80 dB | 0.00000001× | 100000000× | 0.0001× | Extreme signal suppression |
| 90 dB | 0.000000001× | 1000000000× | 0.00003× | Calibration systems |

\* Voltage ratio assumes identical source and load impedances (typically 50 Ω RF systems).

\* Voltage ratio assumes identical source and load impedances (typically 50 Ω RF systems).
\* Voltage ratio assumes identical source and load impedances (typically 50 Ω RF systems).
Attenuators are fundamental components of almost every RF system.

---

# Typical Applications

Attenuators are commonly used in:

- RF front-end protection
- Automatic Gain Control (AGC)
- Software Defined Radio (SDR)
- GNSS receivers
- Radar systems
- Wireless communication
- Satellite communication
- RF test equipment
- Spectrum analyzers
- Signal generators
- Vector Network Analyzers (VNA)
- Production calibration systems

---

# Why Are Attenuators Needed?

Typical use cases include:

- Preventing ADC saturation
- Protecting sensitive RF inputs
- Controlling receiver gain
- Simulating cable losses
- Improving impedance matching
- Calibrating RF equipment
- Maintaining optimal signal level
- Implementing AGC algorithms

A typical RF receive chain may look like:

```
Antenna
    │
    ▼
LNA
    │
    ▼
Band Pass Filter
    │
    ▼
Digital Attenuator
    │
    ▼
Mixer
    │
    ▼
ADC
```

If the received signal becomes too strong, firmware increases attenuation to keep the ADC operating within its dynamic range.

---

# Types of Attenuators

This library is designed to support different categories of RF attenuators.

## Fixed Attenuators

Provide a constant attenuation level.

Examples:

- Mini-Circuits VAT series
- Pasternack PE70xx
- Keysight fixed attenuators

Characteristics:

- No software control
- Passive components
- Fixed attenuation

Typical values:

- 3 dB
- 6 dB
- 10 dB
- 20 dB
- 30 dB

---

## Digital Step Attenuators

The most common programmable attenuators used in modern embedded systems.

Examples:

- pSemi PE4312
- pSemi PE43711
- Analog Devices HMC624
- Analog Devices ADRF5730

Characteristics:

- SPI controlled
- Precise attenuation
- Discrete step size
- High repeatability

Typical parameters:

```
Range : 0 ... 31.5 dB
Step  : 0.5 dB
```

---

## Integrated RF Transceivers

Some RF chips integrate programmable attenuation directly into the receive chain.

Examples:

- Analog Devices AD9361
- ADRV9002
- LMS7002M

In these devices attenuation is part of the receiver architecture rather than a standalone IC.

---

## Voltage Controlled Attenuators

Attenuation is controlled using an analog voltage instead of SPI.

Typical applications:

- Analog AGC
- Legacy receivers
- Microwave systems

---

## PIN Diode Attenuators

Used in high-performance RF systems.

Typical applications:

- Radar
- Military communication
- Satellite communication
- Phased array antennas

---

# Library Goals

The library is designed around several principles.

## Hardware Independent

Business logic is completely separated from hardware access.

```
Application
      │
      ▼
IAttenuator
      │
      ▼
PE4312
      │
      ▼
Platform Driver
      │
      ▼
SPI
```

The attenuation algorithm never depends on:

- STM32 HAL
- Linux spidev
- Zephyr
- ESP-IDF
- RTOS
- Bare-metal implementation

---

## Common Programming Interface

Every attenuator exposes exactly the same public interface.

```cpp
std::unique_ptr<IAttenuator> attenuator = ...;

attenuator->SetAttenuation(12.5);
attenuator->Apply();
```

The application does not need to know whether the hardware is:

- PE4312
- PE43711
- HMC624
- AD9361
- Fixed attenuator

---

## Deferred Configuration

Programmable attenuators do not immediately access hardware.

Instead:

```
SetAttenuation()
        │
        ▼
Store pending configuration
        │
        ▼
Apply()
        │
        ▼
SPI transaction
```

This allows multiple configuration changes to be committed using a single hardware transaction.

---

# Library Architecture

```
IAttenuator
      ▲
      │
AttenuatorBase
      ▲
      ├───────────────┐
      │               │
FixedAttenuator   PE4312
                      │
                      ▼
              Platform-specific Driver
```

Integrated RF transceivers use an adapter.

```
IAttenuator
      ▲
      │
AD9361RxAttenuator
      │
      ▼
IAd9361RxChannel
      │
      ▼
AD9361 Driver
```

---

# Design Principles

The library follows several software engineering principles.

## Interface Segregation Principle

The attenuator library depends only on small hardware-specific interfaces.

Example:

```cpp
class IAd9361RxChannel
{
public:

    virtual Error SetRxAttenuation(double attenuationDb) = 0;
    virtual double GetRxAttenuation() const = 0;
};
```

This keeps the attenuator implementation independent from any particular AD9361 software stack.

---

## Transport Layer Separation

Device logic and transport layer are separated.

The PE4312 driver knows:

- attenuation limits
- step size
- control word format

It does **not** know:

- SPI controller
- DMA
- GPIO
- RTOS
- operating system

---

## Deferred Hardware Updates

Configuration changes are cached locally.

```
SetAttenuation(10.5)
        │
        ▼
pendingAttenuation = 10.5
        │
        ▼
Apply()
        │
        ▼
Hardware Update
```

This behavior is particularly useful for firmware where several RF devices must be synchronized.

---

# Supported Operations

Every attenuator implements the same core functionality.

| Operation | Description |
|----------|-------------|
| GetName() | Returns device name |
| IsProgrammable() | Indicates whether attenuation can be modified |
| SetAttenuation() | Requests a new attenuation |
| GetAttenuation() | Returns active attenuation |
| GetMinimumAttenuation() | Minimum supported attenuation |
| GetMaximumAttenuation() | Maximum supported attenuation |
| GetStepSize() | Hardware resolution |
| IsValidAttenuation() | Validates attenuation |
| QuantizeAttenuation() | Rounds to nearest supported value |
| Enable() | Enables RF path |
| Disable() | Disables RF path |
| IsEnabled() | Returns RF path state |
| Apply() | Commits pending changes |
| Reset() | Restores default configuration |
| IsBusy() | Indicates hardware update |

---

# Example

```cpp
std::unique_ptr<rf::IAttenuator> attenuator = CreateAttenuator();

attenuator->SetAttenuation(12.37);

std::cout << attenuator->QuantizeAttenuation(12.37);
// 12.5

attenuator->Apply();
```

---

# Error Handling

Operations return an `Error` value.

```cpp
enum class Error
{
    None,
    NotSupported,
    InvalidParameter,
    Busy,
    CommunicationError,
    NotInitialized,
    HardwareError
};
```

This avoids exceptions and makes the library suitable for embedded environments.

---

# Platform Independence

The library contains no platform-specific code.

Platform-specific implementations provide hardware access.

Examples include:

- STM32
- Linux
- Raspberry Pi
- ESP32
- Zephyr RTOS
- FreeRTOS
- Bare Metal

---

# Current Implementations

Current drivers include:

- FixedAttenuator
- PE4312
- AD9361RxAttenuator

Future drivers may include:

- PE43711
- HMC624
- ADRF5730
- HMC540
- ADRV9002
- LMS7002M
- Custom FPGA-based attenuators

---

# Project Objectives

The project aims to provide:

- Modern C++ design
- Hardware abstraction
- Portable firmware architecture
- Easy unit testing
- Platform independence
- Clean separation between RF logic and transport layer
- Consistent API for all attenuator types
- Straightforward integration into embedded and SDR applications