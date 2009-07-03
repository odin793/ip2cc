#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, char* argv[]){
    if (argc != 2) {
        printf("Usage:\n\tip2cc <IPADDRESS>\n\n");
        return 1;
    }
    char ip[strlen(argv[1])];
    strcpy(ip, argv[1]);
    ip[strlen(ip)] = '.';
    int c = 0, pos = 0, c_pos = 0, len = strlen(ip), i = 0, parsed_ip[4];
    char chunk[len];
    while (pos < len) {
        if (ip[pos] == '.') {
            parsed_ip[c] = atoi(chunk);
            for (i=0; i<len; i++) {
                chunk[i] = 0;
            }
            c ++;
            c_pos = 0;
        } else {
            chunk[c_pos] = ip[pos];
            c_pos ++;
        }
        pos ++;
    }
    int start, offset = 0, ivalue[4];
    char value[4];
    FILE* fp = fopen("ip2cc.db", "r");
    for (i=0; i<4; i++) {
        start = offset + parsed_ip[i] * 4;
        fseek(fp, start, SEEK_SET); 
        fread(value, sizeof(value), 4, fp);
        if (value[0] == -1 && value[1] == -1) {
           if (value[2] == 0 && value[3] == 0) {
               printf("not found\n");
               return 1;
           }
           printf("%c%c\n", value[2], value[3]);
           return 0;
        }
        for (c=0; c<4; c++) {
            if ((int) value[c] < 0) {
                ivalue[c] = 256 + (int) value[c];
            } else {
                ivalue[c] = (int) value[c];
            }
        }
        offset = 0;
        for (c=0; c<4; c++) {
            offset = offset + ivalue[c] * ((int)pow(256, 3-c));
        }
    }
    printf("broken db\n");
    return -1;
}