/*
 * PWM.h
 *
 * Created: 15.01.2020 22:11:19
 *  Author: Veremeev Andrey
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>

#define SERVO_MIN 8
#define SERVO_MAX 39

void InitTimer1 (void);
void InitTimer2 (void);
long Scaling (long x, long in_min, long in_max, long out_min, long out_max);



#endif /* PWM_H_ */