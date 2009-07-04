#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int ip2cc(char* cc, char* raw_ip, const char* db_file) {
    char ip[strlen(raw_ip)];
    strcpy(ip, raw_ip);
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
    FILE* fp = fopen(db_file, "r");
    for (i=0; i<4; i++) {
        start = offset + parsed_ip[i] * 4;
        fseek(fp, start, SEEK_SET); 
        fread(&value[0], (size_t) 1, 4, fp);
        if (value[0] == -1 && value[1] == -1) {
            if (value[2] == 0 && value[3] == 0) {
                fclose(fp);
                return 1;
            }
            cc[0] = value[2];
            cc[1] = value[3];
            cc[2] = '\0';
            fclose(fp);
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
    fclose(fp);
    return 2;
}

int main(int argc, char* argv[]){
    if (argc != 2) {
        printf("Usage:\n\tip2cc <IPADDRESS>\n\n");
        return 1;
    }
    char cc[3];
    int ret = ip2cc(&cc[0], argv[1], "../../ip2cc.db");
    if (ret) {
        if (ret == 1) {
            printf("not found\n");
        } else if (ret == 2) {
            printf("broken db\n");
        }
    } else {
        printf("%s\n", &cc[0]);
    }
    return ret;
}