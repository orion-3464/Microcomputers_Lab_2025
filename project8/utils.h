#ifndef _UTILS_
#define _UTILS_

#include <string.h>

void print_message(void) {
    lcd_data('C');
    lcd_data('A');
    lcd_data('L');
    lcd_data('L');
    lcd_data(' ');
    lcd_data('N');
    lcd_data('U');
    lcd_data('R');
    lcd_data('S');
    lcd_data('E');
    lcd_data('?');
}
void print_connect_attemp() {
    lcd_data('A');
    lcd_data('T');
    lcd_data('T');
    lcd_data('E');
    lcd_data('M');
    lcd_data('P');
    lcd_data('I');
    lcd_data('N');
    lcd_data('G');
    lcd_data(' ');
    lcd_data('T');
    lcd_data('O');
    lcd_command(0xC0);
    lcd_data('C');
    lcd_data('O');
    lcd_data('N');
    lcd_data('N');
    lcd_data('E');
    lcd_data('C');
    lcd_data('T');
    lcd_data('.');
    lcd_data('.');
    lcd_data('.');
}

void lcd_nurse_call() {
    lcd_data('N');
    lcd_data('U');
    lcd_data('R');
    lcd_data('S');
    lcd_data('E');
    lcd_data(' ');
    lcd_data('C');
    lcd_data('A');
    lcd_data('L');
    lcd_data('L');
}

void print_lcd_success(int num) {
    switch(num) {
        case 1:
            lcd_data('1');
            lcd_data('.');
            lcd_data('S');
            lcd_data('u');
            lcd_data('c');
            lcd_data('c');
            lcd_data('e');
            lcd_data('s');
            lcd_data('s');
            break;
            
        case 2:
            lcd_data('2');
            lcd_data('.');
            lcd_data('S');
            lcd_data('u');
            lcd_data('c');
            lcd_data('c');
            lcd_data('e');
            lcd_data('s');
            lcd_data('s');
            break;
        case 3:
            lcd_data('3');
            lcd_data('.');
            lcd_data('S');
            lcd_data('u');
            lcd_data('c');
            lcd_data('c');
            lcd_data('e');
            lcd_data('s');
            lcd_data('s');
            break;
    }
}

void print_lcd_fail(int num) {
    switch(num) {
        case 1:
            lcd_data('1');
            lcd_data('.');
            lcd_data('F');
            lcd_data('A');
            lcd_data('I');
            lcd_data('L');
            break;
            
        case 2:
            lcd_data('2');
            lcd_data('.');
            lcd_data('F');
            lcd_data('A');
            lcd_data('I');
            lcd_data('L');
            break;
        case 3:
            lcd_data('3');
            lcd_data('.');
            lcd_data('F');
            lcd_data('A');
            lcd_data('I');
            lcd_data('L');
            break;
    }
}

void form_payload(char *buf, float temp, float pres, int status) {
    strcat(buf, "ESP:payload:[{\"name\": \"temperature\",\"value\": \"");   // payload head
    
    // Add temperature value to payload
    uint8_t tint = (uint8_t) temp;
    
    float tdec = temp - tint;
    uint8_t tdec1 = tdec * 10;
    tdec *= 10;
    tdec -= tdec1;
    uint8_t tdec2 = tdec * 10;
    
    uint8_t t1 = tint / 10;
    uint8_t t2 = tint % 10;
    
    char c1[2],c2[2],c3[2],c4[2];
    c1[0] = t1 + 48;
    c1[1]='\0';
    const char *te1 = (const char *) c1;
    c2[0] = t2 + 48;
    c2[1]='\0';
    const char *te2 = (const char *) c2;
    c3[0] = tdec1 + 48;
    c3[1]='\0';
    const char *dec1 = (const char *) c3;
    c4[0] = tdec2 + 48;
    c4[1]='\0';
    const char *dec2 = (const char *) c4;
 
    strcat(buf, te1);
    strcat(buf, te2);
    strcat(buf, ".");
    strcat(buf, dec1);
    strcat(buf, dec2);
    
    strcat(buf, "\",{\"name\": \"pressure\",\"value\": \"");
    
    // Add pressure value to payload
    uint8_t pint = (uint8_t) pres;
    pres -= pint;
    uint8_t pdec1 =  pres * 10;
    pres *= 10;
    pres -= pdec1;
    uint8_t pdec2 =  pres * 10;
    
    uint8_t p1 = pint / 10;
    uint8_t p2 = pint % 10;
    
    p1 += 48;    // to ASCII
    p2 += 48;
    pdec1 += 48;    // to ASCII
    pdec2 += 48;    // to ASCII
    
    c1[0] = p1;
    c1[1]='\0';
    const char *pre1 = (const char *) c1;
    c2[0] = p2;
    c2[1]='\0';
    const char *pre2 = (const char *) c2;
    c3[0] = pdec1;
    c3[1]='\0';
    const char *predec1 = (const char *) c3;
    c4[0] = pdec2;
    c4[1]='\0';
    const char *predec2 = (const char *) c4;
    
    strcat(buf, pre1);
    strcat(buf, pre2);
    strcat(buf, ".");
    strcat(buf, predec1);
    strcat(buf, predec2);
    
    strcat(buf, "\"},{\"name\": \"team\",\"value\": \"12\"},{\"name\": \"status\",\"value\": \"");
    
    // Add status value to payload
    char s[16] = {0};
    switch(status) {
        case 1:
            strcpy(s, "NURSE CALL");
            break;
            
        case 2:
            strcpy(s, "CHECK PRESSURE");
            break;
            
        case 3:
            strcpy(s, "CHECK TEMP");
            break;
            
        case 4:
            strcpy(s, "OK");
    }
    
    const char *stat = (const char *) s;
    strcat(buf, stat);
    
    strcat(buf, "\"}]\n");
}

void print_pressure(float pressure) {
    uint8_t pint = (uint8_t) pressure;
    pressure -= pint;
    uint8_t pdec1 =  pressure * 10;
    pressure *= 10;
    pressure -= pdec1;
    uint8_t pdec2 =  pressure * 10;
    
    uint8_t p1 = pint / 10;
    uint8_t p2 = pint % 10;
    
    p1 += 48;    // to ASCII
    p2 += 48;
    pdec1 += 48;    // to ASCII
    pdec2 += 48;    // to ASCII
    lcd_data(p1);
    lcd_data(p2);
    lcd_data('.');
    lcd_data(pdec1);
    lcd_data(pdec2);
}

void print_status(int status) {
    switch(status) {
        case 1:
            lcd_data('N');
            lcd_data('U');
            lcd_data('R');
            lcd_data('S');
            lcd_data('E');
            lcd_data(' ');
            lcd_data('C');
            lcd_data('A');
            lcd_data('L');
            lcd_data('L');
            break;
            
        case 2:
            lcd_data('C');
            lcd_data('H');
            lcd_data('E');
            lcd_data('C');
            lcd_data('K');
            lcd_data(' ');
            lcd_data('P');
            lcd_data('R');
            lcd_data('E');
            lcd_data('S');
            lcd_data('S');
            lcd_data('U');
            lcd_data('R');
            lcd_data('E');
            break;
            
        case 3:
            lcd_data('C');
            lcd_data('H');
            lcd_data('E');
            lcd_data('C');
            lcd_data('K');
            lcd_data(' ');
            lcd_data('T');
            lcd_data('E');
            lcd_data('M');
            lcd_data('P');
            break;
            
        default:
            lcd_data('O');
            lcd_data('K');
    }
}

float adc_read() {
    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1 << ADSC));
    return ADC;
}

#endif /*_UTILS_*/
