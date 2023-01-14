#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "settings_handler.h"


typedef struct {
    float         temperature;
    unsigned long timestamp;
} measurement_t;

int initSensors();

int takeMeasurement(measurement_t* measurement);

int publishMeasurement(const measurement_t* measurement, const settings_t* settings);


#endif /* MEASUREMENT_H */
