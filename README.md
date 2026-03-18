This is files/HWs for the class EE 3314 taught at UTA 
by the professor Jon Mitchell. 

Some concepts and definitions to get you started for Fundamentals of Embedded Systems. 

# Vocabulary 

### Rising Edge: Triggers when something goes from False -> True 
### Falling Edge: Triggers when somethign goes from True -> False 

The edge is just the **EXACT** moment of change - not while it's on, not while it's off, but the instant it switches. 

<img src="assets/risingfallingedge.png" width="400">


### Structs: Structures (strcuts for short) are data structures used to create user-defined data types in 'C'. They allow us to combine different data types.
Main purpose are for data organization, modularity, and creation of complex data structures. 

<img src="assets/carstruct.png" width="400">

## Pointers
### Pointer: A variable that stores the **MEMORY ADDRESS** of another variable 
The main two symbols when using a pointer are the *&* and * operators. 
& - Gets the **MEMORY ADDRESS** of a variable 
* - Gets the **VALUE** at the pointer's ADDRESS
Example 1: 

    int x = 42;

    int *ptr = &x; //ptr holds the address of x (some random hex code) 

    printf("Value of x        : %d\n", x);
    printf("Address of x      : %p\n", (void*)&x);
    printf("ptr holds address : %p\n", (void*)ptr);
    printf("Value via *ptr    : %d\n", *ptr);


Modifying the value through the pointer: 
*ptr = 99; 
printf("After *ptr = 99, x is now: %d\n", x);

== Pointer Output == 
Value of x        : 42
Address of x      : 0x7ffee4b4a8ac
ptr holds address : 0x7ffee4b4a8ac
Value via *ptr    : 42

=== Modify via Pointer ===
After *ptr = 99, x is now: 99

The 2 main reasons you use pointers are:

1) No direct access to variable inside a function 

Example: 

    void setToFive(int *ptr) {
        *ptr = 5;  // only way to reach x from here
    }

    int main() {
        int x = 0;
        setToFive(&x);
        printf("%d\n", x); // Output: 5
    }

Example 2: 

    void changeValue(int x) {  // C makes a COPY of x here
        x = 99;                // only changes the COPY, not the original!
    }

    int main() {
        int x = 42;
        printf("Before: %d\n", x); // Output: 42
        changeValue(x);
        printf("After : %d\n", x); // Output: 42  <-- x never changed!
        return 0;
    }

C is passed by value, where functions always get a copy. The only way to modify it is to pass its address(pointer) so the function knows exactly where in the memory to make the change. 

2) Avoiding expensive copies 

When you pass a variable to a function without a pointer, C makes a full copy of it. For large data structures, its slow and wasteful

// ❌ BAD - entire struct is copied every call (expensive!)

void printStudent(Student s) { ... }

// ✅ GOOD - only the address is passed (just 8 bytes!)

void printStudent(Student *s) { ... }


### Bitwise Operations 
There are 6 main bitwise operations in C: 

& - AND - Clears the Bits (turning something off) 

<img src="assets/bitclearing.png" width="400">

| - OR - Set the bits (turning something on) 


<img src="assets/bitsetting.png" width="400">


^ - XOR - Toggle the bits (flip states)

<img src="assets/bittoggling.png" width="400">

~ - NOT - Invert the bits 

<img src="assets/bitinverting.png" width="400">


<< - Left Shift - Build bit mask

<img src="assets/bitleftshift.png" width="400">


>> - Right Shift - Extract bit mask 

<img src="assets/bitrightshift.png" width="400">




### Bit shifting 
### Bit Masking 
### Type Qualifiers 

## STM32 Architecture & Bare-metal Programming 

### Memory Mapping 
### Register Addressing 
### Peripheral Clock/Enable Register 
### GPIO Output Data Register


### NVIC 
### Polling 
### Interrupts 
### Interrupt Priority 
### ISR 

## Hardware Abstraction Layer

### HAL: The Hardware Abstraction Layer allows us to create portable code that uses APIs to access peripherals and other hardware-specific registers (makes life so much easier) 

<img src="assets/HAL.png" width="400">

### UART 

### USART

### Timers 

