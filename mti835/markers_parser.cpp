#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "markers_parser.h"

#undef DEBUG

static char actline[256];

int read_raw_line(int f)  /* ignores # */
{
    char c;
    int readresult;
    int i=0;
    do{
//        fprintf(stderr,"loop: %c\n",c);
        readresult=read(f,&c,1);
        actline[i]=c;
        i++;
    }while( c!='\n' && readresult>0 );
    actline[i-1]=0;
    return((readresult<=0)?-1:0);
}

int read_line(int f)      /* ignores # */
{
    int i, rval;
    rval = read_raw_line(f);
    if(rval!=-1){
        for(i=0;actline[i]!=0;i++){
            if( actline[i]=='#' ){ actline[i]=0; break; }
        }
    }
    return(rval);
}

int line_is_empty()
{
    int i;
    int rval=1;
    for(i=0;actline[i]!=0;i++){
        if( actline[i]!=9 || actline[i]!=' ' ){ rval=0; break; }
    }
    return( rval );
}

int read_data_line(int f) /* ignores # and empty lines */
{
    int rval;
    rval=read_line(f);
    if(rval!=-1){
        if(line_is_empty()){
            rval=read_data_line(f);
        }
    }
    return(rval);
}

int markers_get_data( double * lon, double * lat, char * name, int f )
{
    int rval, scanresult, i, j, k;
    i=0; j=0; k=0;
    rval = read_data_line(f);
    if(rval!=-1){
        scanresult = (sscanf(actline, "%lf %lf", lon, lat )==2);
        for(i=0;actline[i]!=0;i++)   if( actline[i]=='\"' ){ j=i; break; }
        if( actline[i]==0 ) scanresult=0;
        for(i=j+1;actline[i]!=0;i++) if( actline[i]=='\"' ){ k=i; break; }
        if( actline[i]==0 ) scanresult=0;
        for(i=j+1;i<k;i++) name[i-j-1] = actline[i];
        name[i-j-1]=0;
        if (scanresult==0) rval=-1;
    }
    return( rval );
}

Marker * load_markers( char * filename, int * nr )
{
    int f;
    double lon, lat;
    char name[80];
    Marker * marker;
    int markernr;
    int i,j;

    if( (f = open(filename, O_RDONLY)) == -1){
        fprintf(stderr,"cant open file \n"); exit(1);
    }
    markernr=0;
    while( markers_get_data(&lon,&lat,name,f)!=-1 ){
#ifdef DEBUG
        fprintf(stderr," %f, %f, %s \n", lon, lat, name);
#endif
        markernr++;
    }
    close(f);

    marker=(Marker *)malloc(markernr*sizeof(Marker));

    if( (f = open(filename, O_RDONLY)) == -1){
        fprintf(stderr,"cant open file \n"); exit(1);
    }
    i=-1;
    while( markers_get_data(&lon,&lat,name,f)!=-1 ){
#ifdef DEBUG
        printf("-- %f, %f, %s \n", lon, lat, name);
#endif
        i++;
        marker[i].lon=lon;
        marker[i].lat=lat;
        for(j=0;(marker[i].name[j]=name[j])!=0;j++);
    }
    close(f);

    *nr=markernr;

    return marker;
}
/*
int main()
{
    int f;
    double lon, lat;
    char name[80];

    if( (f = open("/dev/stdin", O_RDONLY)) == -1){
        fprintf(stderr,"cant open file \n"); exit(1);
    }
    fprintf(stderr,"file opened \n");
    while( get_data(&lon,&lat,name,f)!=-1 ){
        printf(" %f, %f, %s \n", lon, lat, name);
    }

    return 0;
}
*/