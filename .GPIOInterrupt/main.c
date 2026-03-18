#include "stm32f4xx.h"

/* ─── 1. GPIO & EXTI Configuration ─────────────────────────── */
void GPIO_EXTI_Init(void)
{
    /* Enable clocks for GPIOA and SYSCFG */
    RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;   // GPIOA clock
    RCC->APB2ENR  |= RCC_APB2ENR_SYSCFGEN;  

    /* Configure PA0 as input with pull-up resistor */
    GPIOA->MODER  &= ~(0x3 << (0 * 2));      // PA0 = input (00)
    GPIOA->PUPDR  &= ~(0x3 << (0 * 2));
    GPIOA->PUPDR  |=  (0x1 << (0 * 2));      // PA0 = pull-up (01)

    /* ─── 2. Map PA0 to EXTI line 0 via SYSCFG ───────────── */
    // EXTICR[0] controls EXTI lines 0–3
    // Bits [3:0] = 0000 means PORTA is selected for EXTI0
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;   // select PORTA (0b0000)

    /* ─── 3. Configure the trigger edge ──────────────────── */
    EXTI->IMR  |= EXTI_IMR_MR0;    // Unmask (enable) interrupt on line 0
    EXTI->RTSR |= EXTI_RTSR_TR0;   // Trigger on rising edge (button release if pull-up)
    EXTI->FTSR &= ~EXTI_FTSR_TR0;  // (optional) disable falling edge trigger

    /* ─── 4. Configure NVIC ───────────────────────────────── */
    NVIC_SetPriority(EXTI0_IRQn, 1);  // Priority level 1 (0 = highest)
    NVIC_EnableIRQ(EXTI0_IRQn);       // Enable EXTI0 in the NVIC
}

/* ─── 5. The ISR — fires when PA0 sees a rising edge ────────── */
void EXTI0_IRQHandler(void)
{
    /* Always check the pending register — critical on shared handlers */
    if (EXTI->PR & EXTI_PR_PR0)
    {
        /* ── Your application logic goes here ── */
        GPIOA->ODR ^= (1 << 5);   // Example: toggle LED on PA5

        /* ─── 6. Clear the pending flag (write 1 to clear) ─ */
        EXTI->PR |= EXTI_PR_PR0;
        // Writing 1 to this bit clears it — counter-intuitive but correct!
    }
}

/* ─── Main ──────────────────────────────────────────────────── */
int main(void)
{
    GPIO_EXTI_Init();

    while (1)
    {
        // Main loop runs freely.
        // The CPU jumps to EXTI0_IRQHandler automatically on interrupt,
        // then returns here when the ISR finishes.
    }
}
