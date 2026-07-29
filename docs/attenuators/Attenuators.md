В контексте **RF (Radio Frequency), схемотехники, FPGA и Firmware** аттенюатор (**attenuator**) — это устройство, которое **уменьшает уровень сигнала**, обычно **без существенного изменения его формы и частотных характеристик**.

Проще говоря:

> **Аттенюатор — это управляемый или фиксированный "регулятор громкости" для радиочастотного сигнала.**

Он работает с сигналами от kHz до GHz (и выше) и измеряется в **децибелах (dB)**.

---

## 1. Зачем нужны аттенюаторы в RF системах

Типичный RF тракт:

```
        RF Input
            |
            v
       ┌─────────┐
       │ Filter  │
       └─────────┘
            |
            v
       ┌─────────┐
       │ LNA     │  Low Noise Amplifier
       └─────────┘
            |
            v
       ┌─────────┐
       │ Mixer   │
       └─────────┘
            |
            v
       ┌─────────┐
       │ ADC     │
       └─────────┘
```

Проблема:

* сигнал слишком сильный → перегруз ADC/LNA/mixer
* сигнал слишком слабый → плохое отношение сигнал/шум

Аттенюатор позволяет:

```
Strong signal

       0 dBm
          |
          |
      [ Attenuator ]
          |
          v
      -20 dBm
```

---

# 2. Что такое dB у аттенюатора

Ослабление считается:

[
A_{dB}=10\log_{10}\frac{P_{in}}{P_{out}}
]

Например:

Аттенюатор:

```
10 dB
```

означает:

[
P_{out}=P_{in}/10
]

То есть:

```
Input:
+10 dBm

Attenuator:
10 dB

Output:
0 dBm
```

---

## Таблица

| Attenuation | Мощность на выходе |
| ----------- | ------------------ |
| 3 dB        | примерно 50%       |
| 6 dB        | 25%                |
| 10 dB       | 10%                |
| 20 dB       | 1%                 |
| 30 dB       | 0.1%               |

---

# 3. Виды аттенюаторов

## 3.1 Fixed Attenuator (фиксированный)

Самый простой.

Например:

```
RF IN ----[ 20dB ]---- RF OUT
```

Характеристики:

* фиксированное ослабление
* нет управления
* высокая линейность

Используется:

* согласование трактов
* защита входов
* настройка усиления

Пример:

```
PA ---- 3dB ---- Antenna
```

Чтобы уменьшить мощность передатчика.

---

# 3.2 Variable Attenuator (переменный)

Можно менять ослабление:

```
       SPI
        |
        v

RF --->[ Digital Attenuator ]---> RF OUT

        0...31 dB
```

Например:

```
0 dB
5 dB
10 dB
15 dB
...
```

---

# 4. Digital Step Attenuator (DSA)

Это очень распространенный вариант в современных RF системах.

Например:

**PE4312**

(мы как раз обсуждали такие микросхемы)

Внутри:

```
              RF
              |
        +-------------+
        |             |
        |  Switches   |
        |             |
        +-------------+

        0.5dB
        1dB
        2dB
        4dB
        8dB
```

Каждый бит управляет своим элементом:

```
Bit:

0 0 0 1 1

=
1dB + 2dB

=
3dB attenuation
```

---

Например:

PE4312:

```
5-bit

0
1
2
4
8
16 dB

Total:

31.5 dB
```

---

# 5. Как управляется аттенюатор

В embedded системах:

## SPI

Самый частый вариант:

```
MCU

SPI
 |
 |
 v

+-------------+
| Attenuator |
+-------------+

```

Пример:

```cpp
attenuator.setAttenuation(12.5_dB);
```

Внутри:

```
12.5 dB

binary:

011001
```

Отправляется:

```
MOSI:
0 1 1 0 0 1

CLK:
_|-|_|-|_|-|_

CS:
____----____
```

---

## GPIO control

Некоторые имеют параллельное управление:

```
MCU

GPIO0 ----\
GPIO1 -----\
GPIO2 ------> Attenuator
GPIO3 -----/
GPIO4 ----/
```

Например:

```
GPIO:

10101

= 21dB
```

---

# 6. Основные параметры RF аттенюатора

## 1. Frequency Range

Например:

```
DC - 6 GHz
```

или

```
100 MHz - 40 GHz
```

---

## 2. Attenuation Range

Например:

```
0 - 31.5 dB
```

---

## 3. Step Size

Минимальный шаг:

```
0.5 dB
```

или

```
1 dB
```

---

## 4. Insertion Loss

Потери даже при 0 dB:

Например:

```
Set = 0 dB

Actual:
1.2 dB loss
```

---

## 5. Return Loss

Насколько хорошо согласован вход:

```
50 Ohm system
```

---

## 6. Switching Time

Очень важно для SDR:

Например:

```
1 µs
```

или

```
100 ns
```

---

# 7. Где стоят аттенюаторы в RF тракте

## Receiver

Например SDR:

```
Antenna

 |
 |
 v

LNA
 |
 |
 v

Mixer
 |
 |
 v

ADC
```

Добавляем:

```
Antenna

 |
 |
 v

Digital Attenuator

 |
 |
 v

LNA

 |
 |
 v

ADC
```

Задача:

не дать ADC уйти в saturation.

---

## Transmitter

```
DAC

 |
 |
 v

Mixer

 |
 |
 v

PA

 |
 |
 v

Attenuator

 |
 |
 v

Antenna
```

Регулируем мощность:

```
Output:

+20 dBm
+10 dBm
0 dBm
```

---

# 8. Аттенюатор + FPGA + FFT

Очень распространенная архитектура:

```
        RF
        |
        v
   Attenuator
        |
        v
       ADC
        |
        v
      FPGA
        |
        |
     FFT Core
        |
        v
   Spectrum
```

FPGA делает:

* Digital Down Conversion
* FFT
* Filtering
* AGC

Аттенюатор делает:

* аналоговый уровень до ADC

---

# 9. Аттенюатор и AGC

Автоматическая регулировка усиления:

```
          ADC
           |
           |
        Measure
        amplitude
           |
           v

        FPGA

           |
           |
          SPI

           |
           v

      Attenuator
```

Алгоритм:

```
if ADC > threshold:

    attenuation += 1dB


if ADC < threshold:

    attenuation -= 1dB
```

---

# 10. Реализация в Firmware

Типичная архитектура C++:

```cpp
class IAttenuator
{
public:

    virtual void set(double db) = 0;

    virtual double get() const = 0;

    virtual ~IAttenuator() = default;
};
```

Драйвер:

```cpp
class PE4312 : public IAttenuator
{
public:

    void set(double db) override
    {
        uint8_t value = encode(db);

        spi.write(value);
    }

private:

    uint8_t encode(double db);
};
```

---

# 11. Важный инженерный момент

Аттенюатор — это **аналоговое устройство**, но управляется цифровым интерфейсом.

То есть цепочка:

```
C++ Firmware

      |
      |
     SPI

      |
      |
 Digital Attenuator IC

      |
      |
 RF signal (GHz)
```

Firmware отвечает за:

* SPI timing
* калибровку
* таблицы компенсации
* AGC алгоритмы
* управление мощностью

А сам RF сигнал проходит через микросхему.

---

В современных SDR/FPGA системах аттенюатор — это один из ключевых элементов между **аналоговым RF миром (GHz)** и **цифровым миром (ADC/FPGA/C++)**.
