#ifndef MARKERS_PARSER_H
#define MARKERS_PARSER_H

typedef struct MarkerStruct{
    double lon, lat;
    char name[80];
} Marker;

int markers_get_data( double * lon, double * lat, char * name, int f );
Marker * load_markers( char * filename, int * nr );
#endif /* MARKERS_PARSER_H */
