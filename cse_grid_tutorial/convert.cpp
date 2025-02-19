// file: convert.h
#ifndef CONVERT_H
#define CONVERT_H

//function prototypes belong in .h file
double fahrenToCel(double fahren)
{
    double result = (fahren - 32) * (5.0 / 9.0);
    return result;
}

#endif