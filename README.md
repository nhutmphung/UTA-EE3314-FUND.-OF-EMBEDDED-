# EE 3314 — Fundamentals of Embedded Systems
> **University of Texas at Arlington** | Taught by **Professor Jon Mitchell**

This repository contains files and homework assignments for EE 3314.
Below is a reference guide covering core concepts for embedded systems programming.

---

## 📚 Table of Contents

### 🔵 Core C Concepts
- [Rising & Falling Edge](#rising--falling-edge)
- [Structs](#structs)
- [Pointers](#pointers)
- [Bitwise Operations](#bitwise-operations)
- [Type Qualifiers](#type-qualifiers)

### 🟠 STM32 Architecture & Bare-Metal Programming
- [Memory Mapping](#memory-mapping)
- [Register Addressing](#register-addressing)
- [Peripheral Clock / Enable Register](#peripheral-clock--enable-register)
- [GPIO Output Data Register](#gpio-output-data-register)
- [NVIC](#nvic)
- [Polling](#polling)
- [Interrupts](#interrupts)
- [Interrupt Priority](#interrupt-priority)
- [ISR](#isr)

### 🟢 Hardware Abstraction Layer
- [HAL](#hal)
- [UART](#uart)
- [USART](#usart)
- [Timers](#timers)

---

## 🔵 Core C Concepts

---

### Rising & Falling Edge

| Edge | Trigger |
|---|---|
| **Rising Edge** | Signal goes from `LOW → HIGH` (False → True) |
| **Falling Edge** | Signal goes from `HIGH → LOW` (True → False) |

> 💡 The edge is the **exact instant** of change — not while it's HIGH, not while it's LOW, but the precise moment it switches.

<img src="assets/risingfallingedge.png" width="400">

---

### Structs

Structures (`struct` for short) are user-defined data types in C that let you **combine multiple different data types** into one grouped unit.

**Main purposes:**
- 📦 Data organization
- 🔧 Modularity
- 🏗️ Building complex data structures

<img src="assets/carstruct.png" width="400">

---

### Pointers

A **pointer** is a variable that stores the **memory address** of another variable.

#### Key Operators

| Operator | Name | What it does |
|---|---|---|
| `&` | Address-of | Gets the **memory address** of a variable |
| `*` | Dereference | Gets the **value** stored at the pointer's address |

#### Example — Basic Pointer Usage

```c
int x = 42;
int *ptr = &x; // ptr holds the address of x

printf("Value of x        : %d\n", x);
printf("Address of x      : %p\n", (void*)&x);
printf("ptr holds address : %p\n", (void*)ptr);
printf("Value via *ptr    : %d\n", *ptr);

// Modifying the value through the pointer
*ptr = 99;
printf("After *ptr = 99, x is now: %d\n", x);
```

**Output:**
```
Value of x        : 42
Address of x      : 0x7ffee4b4a8ac
ptr holds address : 0x7ffee4b4a8ac
Value via *ptr    : 42
After *ptr = 99, x is now: 99
```

---

#### Why Use Pointers?

**Reason 1 — Modifying a variable inside a function**

C is **pass-by-value** — functions always receive a *copy* of the variable.
The only way to modify the original is to pass its address.

```c
// ❌ This does NOT modify the original x
void changeValue(int x) {
    x = 99; // only changes the local copy!
}

// ✅ This DOES modify the original x
void setToFive(int *ptr) {
    *ptr = 5; // modifies the value at the address
}

int main() {
    int x = 0;
    setToFive(&x);
    printf("%d\n", x); // Output: 5
}
```

**Reason 2 — Avoiding expensive copies**

Passing large structs by value forces C to copy the entire thing — slow and wasteful.

```c
// ❌ BAD — entire struct is copied every call
void printStudent(Student s) { ... }

// ✅ GOOD — only the address is passed (just 8 bytes!)
void printStudent(Student *s) { ... }
```

---

### Bitwise Operations

There are **6 main bitwise operations** in C:

| Operator | Name | Purpose |
|---|---|---|
| `&` | AND | Clear bits (turn something **off**) |
| `\|` | OR | Set bits (turn something **on**) |
| `^` | XOR | Toggle bits (flip state) |
| `~` | NOT | Invert all bits |
| `<<` | Left Shift | Build a bit mask / multiply by power of 2 |
| `>>` | Right Shift | Extract a bit mask / divide by power of 2 |

---

#### AND — Clearing Bits
<img src="assets/bitclearing.png" width="400">

#### OR — Setting Bits
<img src="assets/bitsetting.png" width="400">

#### XOR — Toggling Bits
<img src="assets/bittoggling.png" width="400">

#### NOT — Inverting Bits
<img src="assets/bitinverting.png" width="400">

---

#### Bit Shifting

**Left Shift `<<`** — shifts bits to the left, filling with `0`s on the right

<img src="assets/bitleftshift.png" width="600">

**Right Shift `>>`** — shifts bits to the right

<img src="assets/bitrightshift.png" width="600">

> ⚠️ Right shifting doesn't *always* fill with zeroes:
> - **Unsigned types** (`uint`, `ulong`, etc.) → always fills with `0`
> - **Signed positive numbers** → fills with `0`
> - **Signed negative numbers** → fills with `1` (arithmetic shift)

<img src="assets/signedBitShift.png" width="600">

#### Shift Examples

```c
// Left Shift (multiply by power of 2)
00000011 << 1  →  00000110  =  6
00000011 << 2  →  00001100  =  12

// Right Shift (divide by power of 2)
00000110 >> 1  →  00000011  =  3
00000110 >> 2  →  00000001  =  1  // integer division, remainder dropped
```

> 📝 **When to use bit shifting?**
> Shifting is more CPU-efficient than multiplication/division, but use it sparingly.
> Code like `3 * 4` is far more readable than `00000011 << 2`.
> Reserve bit shifting for cases where **every cycle and byte truly counts**.

---

### Type Qualifiers

#### `const` — Read-Only Variable

Makes a variable **unchangeable after initialization**. The compiler will throw an error if you try to modify it.

**Benefits:**
- Prevents accidental modification of important values
- Allows the compiler to optimize (it knows the value won't change)
- `const` globals are stored in **Flash**, not RAM — saves memory

```c
const uint32_t SYSTEM_CLOCK_HZ = 84000000UL;       // 84 MHz, stored in Flash
const uint8_t LOOKUP_TABLE[] = {0x00, 0x01, 0x03, 0x07, 0x0F}; // ROM table
```

---

#### `volatile` — Always Read From Memory

Tells the compiler **never to optimize** accesses to this variable — always read/write directly from its actual memory location.

**Most common use cases:**
- Hardware registers (hardware can change them at any time)
- ISR-shared variables
- DMA buffers

```c
// Without volatile, the compiler might optimize this loop away entirely!
volatile uint32_t *pGPIOA_IDR = (volatile uint32_t *)0x40020010;

while ((*pGPIOA_IDR & (1 << 0)) == 0) {
    // Wait for PA0 to go HIGH
}
```

---

#### `const` + `volatile` with Pointers

When used with pointers, the **position** of `const`/`volatile` determines what is protected:

| Declaration | Address | Value at Address |
|---|---|---|
| `volatile int * const ptr` | ❌ Cannot change | ✅ Can change |
| `const int * volatile ptr` | ✅ Can change | ❌ Cannot change |

> 💡 Think of it this way: whatever keyword is **directly before** `*` applies to the **value**; whatever is **after** `*` applies to the **pointer itself**.

---

## 🟠 STM32 Architecture & Bare-Metal Programming

---

### Memory Mapping

*(Content coming soon)*

---

### Register Addressing

*(Content coming soon)*

---

### Peripheral Clock / Enable Register

*(Content coming soon)*

---

### GPIO Output Data Register

*(Content coming soon)*

---

### NVIC

*(Content coming soon)*

---

### Polling

*(Content coming soon)*

---

### Interrupts

*(Content coming soon)*

---

### Interrupt Priority

*(Content coming soon)*

---

### ISR

*(Content coming soon)*

---

## 🟢 Hardware Abstraction Layer

---

### HAL

The **Hardware Abstraction Layer** lets you write **portable code** using APIs to access peripherals and hardware-specific registers — without needing to manually manipulate every register yourself.

<img src="assets/HAL.png" width="400">

---

### UART

*(Content coming soon)*

---

### USART

*(Content coming soon)*

---

### Timers

*(Content coming soon)*
