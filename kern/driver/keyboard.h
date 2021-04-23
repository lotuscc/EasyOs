#ifndef __KERN_DRIVER_KEYBOARD_H__
#define __KERN_DRIVER_KEYBOARD_H__

void keyboard_init(void); 
void intr_keyboard_handler(void);

#endif /* !__KERN_DRIVER_KEYBOARD_H__ */

