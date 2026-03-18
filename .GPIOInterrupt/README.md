# GPIO STRUCTURE IN EMBEDDED C 

Nearly all GPIO interrupt implemntation follow this pattern: 

### 1) Congiure the GPIO Pin -> set the direction, pull up/down resistrs

### 2) 2Congiure the EXTI Line (External Interrupt/Event) -> Map the GPIO pin to an external interrupt line

### 3) Set trigger edge -> rising, falling, or both

### 4) Enable the interrupt in NVIC -> the NVIC (Nested Vector Interrupt Controller) routes the interrupt to the CPU

### 5) Write the ISR (Interrupt Service Routine) -> the function that runs when the interrupt fires

### 6) Clear the pending flag -> Required inside ISR, or the interrupt fires again immediately

Clear visualization of the GPIO Interrupt Flow: 


<img src="GPIOInterruptFlow" width="400">
