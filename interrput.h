#ifndef INTERRUPT_H          // Check if INTERRUPT_H is not already defined
#define INTERRUPT_H          // Define INTERRUPT_H to prevent multiple inclusion

// Function prototype to initialize and enable External Interrupt 1 (EINT1)
void eint1_enable(void);

// Function prototype for the External Interrupt 1 Interrupt Service Routine (ISR)
// The '__irq' keyword tells the compiler that this function is an IRQ handler.
void eint1_isr(void) __irq;

#endif                       // End of include guard for INTERRUPT_H