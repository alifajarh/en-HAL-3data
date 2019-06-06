#ifndef ENCODER_H
#define ENCODER_H

void encoder_init();

void enc1_reset();
void enc2_reset();

void ext_handler1();
void ext_handler2();

double enc1();
double enc2();

#endif