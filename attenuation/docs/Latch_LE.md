В контексте **RF аттенюаторов, цифровых шаговых аттенюаторов (Digital Step Attenuator, DSA)**, `Latch` или `LE (Latch Enable)` — это **сигнал фиксации (записи) уже переданных цифровых данных во внутренний регистр устройства**.

Проще говоря:

> **SPI передает значение аттенюации, а LE/latch говорит устройству: "теперь примени эти данные".**

---

## Зачем нужен Latch/LE

Многие RF аттенюаторы имеют последовательный интерфейс:

```
MCU / FPGA
     |
     | SPI
     |
     v

+----------------+
| Digital        |
| Attenuator     |
|
| Shift Register |
|
| Latch Register |
|
| RF Switches    |
+----------------+
```

Внутри обычно есть два регистра:

### 1. Shift Register

Сюда приходят биты через SPI.

Например:

```
SPI CLK

bit0 bit1 bit2 bit3 bit4 bit5
 |    |    |    |    |    |
 v    v    v    v    v    v

+----------------+
| Shift Register |
+----------------+
```

Пока данные сдвигаются — RF тракт не меняется.

---

### 2. Latch Register

После прихода LE:

```
LE = HIGH
```

данные копируются:

```
Shift Register
       |
       |
       v

Latch Register
       |
       |
       v

RF attenuation cells
```

И только теперь меняется затухание.

---

# Пример PE4312

Возьмем наш знакомый:

## pSemi PE4312

Характеристики:

```
Attenuation:
0 ... 31.5 dB

Step:
0.5 dB

Control:
6 bit serial data
+
CLK
+
LE
```

У него есть:

```
SDI
CLK
LE
```

---

Допустим нужно установить:

```
10.5 dB
```

PE4312:

```
0.5 dB step

10.5 / 0.5 = 21
```

В регистр надо отправить:

```
010101
```

(примерно)

---

Последовательность:

### 1. LE = LOW

```
LE
__
```

Устройство принимает данные.

---

### 2. SPI clock

Передаем:

```
CLK:

_|-|_|-|_|-|_|-|_

DATA:

0 1 0 1 0 1
```

Данные попали в shift register.

---

### 3. LE pulse

Делаем:

```
LE:

____----____
```

или

```
LOW
 |
 HIGH
 |
 LOW
```

В этот момент:

```
shift register
        |
        |
        v

latch register
```

---

### 4. RF тракт меняется

Теперь:

```
RF IN

 |
 |
 v

[ Attenuator ]

 |
 |
 v

RF OUT


Attenuation = 10.5 dB
```

---

# Почему нельзя просто менять во время SPI?

Представим:

Нужно передать:

```
101011
```

А пока SPI идет:

```
101
```

Если бы RF менялся сразу:

```
0 dB
 |
8 dB
 |
16 dB
 |
21 dB
```

Получились бы:

* скачки мощности;
* паразитная модуляция;
* glitches;
* помехи в приемнике;
* проблемы при измерениях.

Для RF это критично.

---

# Где еще встречается LE

## PE43711

Например:

```
PE43711

SPI:
SCLK
SDI
LE
```

Тоже:

```
SPI -> shift register

LE -> apply
```

---

## HMC624A

Analog Devices:

```
CLK
DATA
LE
```

---

## SKY123xx

Skyworks:

```
DATA
CLK
LE
```

---

# В Firmware архитектуре

Для нашей библиотеки аттенюаторов я бы обязательно вынес это отдельно.

Например:

```cpp
class ISpiBus
{
public:

    virtual Error transfer(
        const uint8_t* tx,
        uint8_t* rx,
        size_t size) = 0;
};
```

и

```cpp
class IGpio
{
public:

    virtual Error set(bool state) = 0;
};
```

Тогда драйвер:

```cpp
class PE4312Spi : public DigitalStepAttenuator
{
public:

    PE4312Spi(
        ISpiBus& spi,
        IGpio& latch);

private:

    ISpiBus& spi;
    IGpio& latch;
};
```

---

И применение:

```cpp
Error PE4312Spi::apply()
{
    uint8_t value = encodeAttenuation();

    latch.set(false);

    spi.transfer(&value, nullptr, 1);

    latch.set(true);

    latch.set(false);

    return Error::Success;
}
```

---

# В RF есть еще похожие сигналы

Не путать:

| Сигнал     | Назначение              |
| ---------- | ----------------------- |
| CLK        | тактирование SPI        |
| DATA / SDI | последовательные данные |
| LE / LATCH | применить данные        |
| EN         | включение устройства    |
| RESET      | сброс                   |
| CE         | chip enable             |

---

# В нашей архитектуре аттенюаторов

Для `DigitalStepAttenuator` я бы даже добавил понятие:

```
DigitalStepAttenuator
        |
        |
        +-- SPI interface
        |
        +-- Latch control
        |
        +-- Enable control (optional)
```

То есть:

```cpp
class IDigitalControl
{
public:

    virtual Error write(
        uint16_t value) = 0;

    virtual Error latch() = 0;
};
```

Потому что у RF компонентов очень часто:

```
SPI != Apply
```

SPI только загружает конфигурацию.

А **LE/Latch — это аппаратный commit операции**.

Для библиотеки RF-аттенюаторов это очень важная концепция, и ее стоит учитывать в базовом `DigitalStepAttenuator`, иначе каждый конкретный драйвер (`PE4312`, `PE43711`, `HMC624`) начнет реализовывать этот механизм по-своему.
